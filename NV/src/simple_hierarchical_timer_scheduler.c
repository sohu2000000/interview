/**
 * @file simple_hierarchical_timer.c
 * @brief 简化的分层时间轮定时器调度系统
 * 
 * 改进方案：去掉复杂的current_index概念，直接使用槽位编号
 * 
 * 核心思想：
 * 1. 5级时间轮：level 0-4
 * 2. 每级256个槽位：slot 0-255
 * 3. 直接根据时间计算槽位，无需维护指针
 * 4. 级联时直接移动链表，更简单直观
 * 
 * 时间计算：
 * - 第0级：slot = time % 256
 * - 第1级：slot = (time / 256) % 256  
 * - 第2级：slot = (time / 256²) % 256
 * - 第3级：slot = (time / 256³) % 256
 * - 第4级：slot = (time / 256⁴) % 256
 * 
 * @author feliu@nvidia.com
 * @date 2025-10-23
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

/**
 * @brief 定时器回调函数类型
 */
typedef void (*timer_callback_t)(void *user_data);

/**
 * @brief 定时器节点
 */
typedef struct TimerNode {
    uint64_t expire_time;           // 绝对到期时间
    timer_callback_t callback;      // 回调函数
    void *user_data;               // 用户数据
    struct TimerNode *next;        // 链表指针
    bool is_active;                // 是否激活
    uint32_t timer_id;             // 定时器ID
} TimerNode;

/**
 * @brief 简化的分层时间轮调度器
 */
#define WHEEL_SIZE 256             // 每级256个槽位
#define WHEEL_LEVELS 5             // 5级时间轮
#define TIMER_RESOLUTION_MS 10     // 10ms分辨率

typedef struct SimpleHierarchicalTimer {
    // 5级时间轮，每级256个槽位，每个槽位是链表
    TimerNode *wheels[WHEEL_LEVELS][WHEEL_SIZE];
    
    // 系统状态
    uint64_t current_time;         // 当前时间（时间单位）
    uint32_t next_timer_id;        // 下一个定时器ID
    bool is_running;               // 是否运行
    
    // 并发控制
    pthread_mutex_t mutex;
    pthread_t scheduler_thread;
    
    // 统计信息
    uint64_t total_timers;
    uint64_t total_callbacks;
    uint64_t cascade_count;
} SimpleHierarchicalTimer;

/**
 * @brief 全局调度器
 */
static SimpleHierarchicalTimer *g_timer = NULL;

/**
 * @brief 获取当前时间戳（毫秒）
 */
static uint64_t get_current_timestamp_ms() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

/**
 * @brief 毫秒转时间单位
 */
static uint64_t ms_to_time_units(uint64_t ms) {
    return ms / TIMER_RESOLUTION_MS;
}

/**
 * @brief 根据时间计算槽位编号
 * 
 * 这是简化设计的核心：直接根据时间计算槽位
 * 无需维护复杂的current_index
 */
static int get_slot_for_time(uint64_t time, int level) {
    switch (level) {
        case 0: return time % WHEEL_SIZE;
        case 1: return (time / WHEEL_SIZE) % WHEEL_SIZE;
        case 2: return (time / (WHEEL_SIZE * WHEEL_SIZE)) % WHEEL_SIZE;
        case 3: return (time / ((uint64_t)WHEEL_SIZE * WHEEL_SIZE * WHEEL_SIZE)) % WHEEL_SIZE;
        case 4: return (time / ((uint64_t)WHEEL_SIZE * WHEEL_SIZE * WHEEL_SIZE * WHEEL_SIZE)) % WHEEL_SIZE;
        default: return 0;
    }
}

/**
 * @brief 计算定时器应该放在哪一级
 */
static int calculate_level(uint64_t delay) {
    if (delay < WHEEL_SIZE) return 0;
    if (delay < WHEEL_SIZE * WHEEL_SIZE) return 1;
    if (delay < WHEEL_SIZE * WHEEL_SIZE * WHEEL_SIZE) return 2;
    if (delay < (uint64_t)WHEEL_SIZE * WHEEL_SIZE * WHEEL_SIZE * WHEEL_SIZE) return 3;
    return 4;
}

/**
 * @brief 创建定时器节点
 */
static TimerNode* create_timer_node(uint64_t expire_time, timer_callback_t callback,
                                   void *user_data, uint32_t timer_id) {
    TimerNode *node = malloc(sizeof(TimerNode));
    if (!node) return NULL;
    
    node->expire_time = expire_time;
    node->callback = callback;
    node->user_data = user_data;
    node->next = NULL;
    node->is_active = true;
    node->timer_id = timer_id;
    
    return node;
}

/**
 * @brief 初始化简化的分层时间轮
 */
SimpleHierarchicalTimer* simple_timer_init() {
    printf("=== 初始化简化分层时间轮 ===\n");
    
    SimpleHierarchicalTimer *timer = calloc(1, sizeof(SimpleHierarchicalTimer));
    if (!timer) {
        printf("内存分配失败\n");
        return NULL;
    }
    
    // 初始化所有槽位为NULL
    memset(timer->wheels, 0, sizeof(timer->wheels));
    
    // 初始化状态
    timer->current_time = ms_to_time_units(get_current_timestamp_ms());
    timer->next_timer_id = 1;
    timer->is_running = false;
    timer->total_timers = 0;
    timer->total_callbacks = 0;
    timer->cascade_count = 0;
    
    // 初始化互斥锁
    if (pthread_mutex_init(&timer->mutex, NULL) != 0) {
        printf("互斥锁初始化失败\n");
        free(timer);
        return NULL;
    }
    
    printf("简化分层时间轮初始化成功\n");
    printf("- 级数：%d\n", WHEEL_LEVELS);
    printf("- 每级槽位：%d\n", WHEEL_SIZE);
    printf("- 总槽位：%d\n", WHEEL_LEVELS * WHEEL_SIZE);
    printf("- 最大时间范围：%lu T\n", 
           (uint64_t)WHEEL_SIZE * WHEEL_SIZE * WHEEL_SIZE * WHEEL_SIZE * WHEEL_SIZE);
    
    return timer;
}

/**
 * @brief 添加定时器 - 超简单的O(1)实现
 * 
 * 算法：
 * 1. 计算到期时间
 * 2. 确定级别和槽位
 * 3. 插入到对应链表
 * 
 * 就这么简单！
 */
uint32_t add_simple_timer(SimpleHierarchicalTimer *timer, uint64_t delay_ms,
                         timer_callback_t callback, void *user_data) {
    if (!timer || !callback) return 0;
    
    pthread_mutex_lock(&timer->mutex);
    
    uint64_t delay_units = ms_to_time_units(delay_ms);
    uint64_t expire_time = timer->current_time + delay_units;
    uint32_t timer_id = timer->next_timer_id++;
    
    // 计算级别和槽位 - 直接计算，无需复杂逻辑
    int level = calculate_level(delay_units);
    int slot = get_slot_for_time(expire_time, level);
    
    printf("添加定时器 ID=%u, 延迟=%lu ms → 第%d级槽位%d\n", 
           timer_id, delay_ms, level, slot);
    
    TimerNode *node = create_timer_node(expire_time, callback, user_data, timer_id);
    if (!node) {
        pthread_mutex_unlock(&timer->mutex);
        return 0;
    }
    
    // 插入到链表头部 - O(1)
    node->next = timer->wheels[level][slot];
    timer->wheels[level][slot] = node;
    timer->total_timers++;
    
    pthread_mutex_unlock(&timer->mutex);
    return timer_id;
}

/**
 * @brief 级联操作 - 超简单实现
 * 
 * 级联的本质：从第N级降到第N-1级
 * 不需要重新计算级别，只需要计算在下一级的槽位
 */
static void simple_cascade(SimpleHierarchicalTimer *timer, uint64_t time) {
    for (int level = 1; level < WHEEL_LEVELS; level++) {
        int slot = get_slot_for_time(time, level);
        
        // 检查这个槽位是否需要级联
        if ((time % (1ULL << (8 * level))) == 0) {  // 256^level的倍数
            printf("  级联：第%d级槽位%d → 第%d级\n", level, slot, level-1);
            timer->cascade_count++;
            
            TimerNode *cascade_list = timer->wheels[level][slot];
            timer->wheels[level][slot] = NULL;
            
            // 简单级联：直接降一级
            while (cascade_list) {
                TimerNode *next = cascade_list->next;
                cascade_list->next = NULL;
                
                if (cascade_list->is_active) {
                    // 直接级联到下一级（level-1）
                    int target_level = level - 1;
                    int new_slot = get_slot_for_time(cascade_list->expire_time, target_level);
                    
                    cascade_list->next = timer->wheels[target_level][new_slot];
                    timer->wheels[target_level][new_slot] = cascade_list;
                    
                    printf("    级联定时器 ID=%u 到第%d级槽位%d\n",
                           cascade_list->timer_id, target_level, new_slot);
                } else {
                    free(cascade_list);  // 清理已取消的定时器
                }
                
                cascade_list = next;
            }
        }
    }
}

/**
 * @brief 执行当前时间的定时器 - 超简单实现
 */
static void execute_current_timers(SimpleHierarchicalTimer *timer, uint64_t time) {
    // 只处理第0级（最精确级别）的当前槽位
    int slot = get_slot_for_time(time, 0);
    
    TimerNode *timer_list = timer->wheels[0][slot];
    timer->wheels[0][slot] = NULL;
    
    if (timer_list) {
        printf("执行第0级槽位%d的定时器（时间%lu）\n", slot, time);
    }
    
    // 执行所有定时器
    while (timer_list) {
        TimerNode *next = timer_list->next;
        
        if (timer_list->is_active && timer_list->expire_time <= time) {
            printf("  执行定时器 ID=%u\n", timer_list->timer_id);
            
            pthread_mutex_unlock(&timer->mutex);
            timer_list->callback(timer_list->user_data);
            pthread_mutex_lock(&timer->mutex);
            
            timer->total_callbacks++;
        } else if (!timer_list->is_active) {
            printf("  跳过已取消的定时器 ID=%u\n", timer_list->timer_id);
        } else {
            printf("  定时器 ID=%u 未到期，重新插入\n", timer_list->timer_id);
            // 重新插入（这种情况应该很少发生）
            timer_list->next = timer->wheels[0][slot];
            timer->wheels[0][slot] = timer_list;
            timer_list = next;
            continue;
        }
        
        free(timer_list);
        timer_list = next;
    }
}

/**
 * @brief 时间推进 - 极简实现
 * 
 * 算法：
 * 1. 执行当前时间的定时器
 * 2. 检查是否需要级联
 * 3. 推进时间
 */
static void tick_simple_timer(SimpleHierarchicalTimer *timer) {
    uint64_t time = timer->current_time;
    
    // 先处理级联（在执行定时器之前）
    simple_cascade(timer, time);
    
    // 执行当前时间的定时器
    execute_current_timers(timer, time);
    
    // 推进时间
    timer->current_time++;
}

/**
 * @brief 取消定时器 - O(1)延迟删除
 */
int cancel_simple_timer(SimpleHierarchicalTimer *timer, uint32_t timer_id) {
    if (!timer || timer_id == 0) return -1;
    
    pthread_mutex_lock(&timer->mutex);
    
    printf("取消定时器 ID=%u\n", timer_id);
    
    // 在所有级别和槽位中查找
    for (int level = 0; level < WHEEL_LEVELS; level++) {
        for (int slot = 0; slot < WHEEL_SIZE; slot++) {
            TimerNode *current = timer->wheels[level][slot];
            while (current) {
                if (current->timer_id == timer_id) {
                    current->is_active = false;
                    printf("  -> 在第%d级槽位%d中标记取消\n", level, slot);
                    pthread_mutex_unlock(&timer->mutex);
                    return 0;
                }
                current = current->next;
            }
        }
    }
    
    printf("  -> 未找到定时器\n");
    pthread_mutex_unlock(&timer->mutex);
    return -1;
}

/**
 * @brief 调度器主循环
 */
static void* simple_scheduler_thread(void *arg) {
    SimpleHierarchicalTimer *timer = (SimpleHierarchicalTimer*)arg;
    
    printf("简化分层时间轮调度器启动\n");
    
    while (timer->is_running) {
        pthread_mutex_lock(&timer->mutex);
        
        // 更新当前时间
        uint64_t current_ms = get_current_timestamp_ms();
        uint64_t current_units = ms_to_time_units(current_ms);
        
        // 处理所有错过的时间点
        while (timer->current_time < current_units) {
            tick_simple_timer(timer);
        }
        
        pthread_mutex_unlock(&timer->mutex);
        
        // 休眠
        usleep(TIMER_RESOLUTION_MS * 1000 / 2);
    }
    
    printf("简化分层时间轮调度器退出\n");
    return NULL;
}

/**
 * @brief 启动调度器
 */
int start_simple_timer(SimpleHierarchicalTimer *timer) {
    if (!timer) return -1;
    
    printf("启动简化分层时间轮调度器\n");
    
    timer->is_running = true;
    
    if (pthread_create(&timer->scheduler_thread, NULL, simple_scheduler_thread, timer) != 0) {
        printf("创建线程失败\n");
        timer->is_running = false;
        return -1;
    }
    
    return 0;
}

/**
 * @brief 停止调度器
 */
void stop_simple_timer(SimpleHierarchicalTimer *timer) {
    if (!timer) return;
    
    printf("停止简化分层时间轮调度器\n");
    timer->is_running = false;
    
    if (timer->scheduler_thread) {
        pthread_join(timer->scheduler_thread, NULL);
    }
}

/**
 * @brief 销毁调度器
 */
void destroy_simple_timer(SimpleHierarchicalTimer *timer) {
    if (!timer) return;
    
    printf("销毁简化分层时间轮调度器\n");
    
    stop_simple_timer(timer);
    
    // 清理所有定时器
    for (int level = 0; level < WHEEL_LEVELS; level++) {
        for (int slot = 0; slot < WHEEL_SIZE; slot++) {
            TimerNode *current = timer->wheels[level][slot];
            while (current) {
                TimerNode *next = current->next;
                free(current);
                current = next;
            }
        }
    }
    
    pthread_mutex_destroy(&timer->mutex);
    free(timer);
    
    printf("销毁完成\n");
}

/**
 * @brief 打印统计信息
 */
void print_simple_timer_stats(SimpleHierarchicalTimer *timer) {
    if (!timer) return;
    
    pthread_mutex_lock(&timer->mutex);
    
    printf("\n=== 简化分层时间轮统计 ===\n");
    printf("当前时间：%lu\n", timer->current_time);
    printf("总定时器：%lu\n", timer->total_timers);
    printf("已执行：%lu\n", timer->total_callbacks);
    printf("级联次数：%lu\n", timer->cascade_count);
    
    // 显示每级的定时器分布
    for (int level = 0; level < WHEEL_LEVELS; level++) {
        int count = 0;
        for (int slot = 0; slot < WHEEL_SIZE; slot++) {
            TimerNode *current = timer->wheels[level][slot];
            while (current) {
                if (current->is_active) count++;
                current = current->next;
            }
        }
        printf("第%d级：%d个定时器\n", level, count);
    }
    
    pthread_mutex_unlock(&timer->mutex);
}

/**
 * @brief 测试回调函数
 */
void test_callback_1(void *data) {
    int *value = (int*)data;
    printf("    [回调] 数据: %d\n", *value);
}

void test_callback_2(void *data) {
    char *msg = (char*)data;
    printf("    [回调] 消息: %s\n", msg);
}

/**
 * @brief 算法分析
 */
void simple_algorithm_analysis() {
    printf("\n=== 简化分层时间轮算法分析 ===\n");
    
    printf("1. 简化设计理念：\n");
    printf("   - 去掉复杂的current_index概念\n");
    printf("   - 直接根据时间计算槽位：slot = time % 256\n");
    printf("   - 级联时直接移动链表\n");
    printf("   - 代码更简洁，逻辑更清晰\n\n");
    
    printf("2. 时间轮映射：\n");
    printf("   - 第0级：slot = time %% 256\n");
    printf("   - 第1级：slot = (time / 256) %% 256\n");
    printf("   - 第2级：slot = (time / 256²) %% 256\n");
    printf("   - 第3级：slot = (time / 256³) %% 256\n");
    printf("   - 第4级：slot = (time / 256⁴) %% 256\n\n");
    
    printf("3. 级联触发条件：\n");
    printf("   - 当 time %% 256^level == 0 时触发级联\n");
    printf("   - 无需维护复杂的指针状态\n");
    printf("   - 自然的数学关系\n\n");
    
    printf("4. 优势：\n");
    printf("   - 实现更简单：少了很多状态管理\n");
    printf("   - 逻辑更清晰：直接的数学映射\n");
    printf("   - 调试更容易：槽位编号直观\n");
    printf("   - 扩展更方便：容易修改级数和大小\n");
}

/**
 * @brief 功能测试
 */
void simple_functional_test() {
    printf("\n=== 简化分层时间轮功能测试 ===\n");
    
    SimpleHierarchicalTimer *timer = simple_timer_init();
    if (!timer) return;
    
    if (start_simple_timer(timer) != 0) {
        destroy_simple_timer(timer);
        return;
    }
    
    // 测试数据
    int data1 = 100, data2 = 200;
    char *msg1 = "短期", *msg2 = "长期";
    
    // 添加不同级别的定时器
    uint32_t t1 = add_simple_timer(timer, 100, test_callback_1, &data1);   // 第0级
    uint32_t t2 = add_simple_timer(timer, 3000, test_callback_2, msg1);    // 第1级  
    uint32_t t3 = add_simple_timer(timer, 70000, test_callback_2, msg2);   // 第2级
    
    printf("添加的定时器：%u, %u, %u\n", t1, t2, t3);
    
    sleep(1);
    print_simple_timer_stats(timer);
    
    printf("\n等待级联和执行...\n");
    sleep(4);
    
    print_simple_timer_stats(timer);
    
    destroy_simple_timer(timer);
}

/**
 * @brief 压力测试
 */
void simple_stress_test() {
    printf("\n=== 简化分层时间轮压力测试 ===\n");
    
    SimpleHierarchicalTimer *timer = simple_timer_init();
    if (!timer || start_simple_timer(timer) != 0) return;
    
    const int num_timers = 500;  // 减少数量，避免输出过多
    int test_data[num_timers];
    
    printf("添加 %d 个定时器...\n", num_timers);
    
    for (int i = 0; i < num_timers; i++) {
        test_data[i] = i;
        
        uint64_t delay;
        if (i < 200) {
            delay = (rand() % 200) * TIMER_RESOLUTION_MS;        // 第0级
        } else if (i < 350) {
            delay = (300 + rand() % 1000) * TIMER_RESOLUTION_MS; // 第1级
        } else if (i < 450) {
            delay = (2000 + rand() % 3000) * TIMER_RESOLUTION_MS; // 第2级
        } else {
            delay = (8000 + rand() % 5000) * TIMER_RESOLUTION_MS; // 第3级+
        }
        
        add_simple_timer(timer, delay, test_callback_1, &test_data[i]);
    }
    
    print_simple_timer_stats(timer);
    
    printf("等待执行...\n");
    sleep(3);
    
    print_simple_timer_stats(timer);
    
    destroy_simple_timer(timer);
}

/**
 * @brief 主函数
 */
int main() {
    printf("简化分层时间轮定时器调度系统\n");
    printf("去掉复杂的current_index，直接用槽位编号\n");
    printf("========================================\n");
    
    simple_algorithm_analysis();
    simple_functional_test();
    simple_stress_test();
    
    printf("\n=== 简化设计的优势 ===\n");
    printf("1. 概念更简单：直接的槽位编号，无需指针管理\n");
    printf("2. 实现更清晰：数学映射关系一目了然\n");
    printf("3. 调试更容易：槽位编号直观可读\n");
    printf("4. 扩展更方便：容易修改和调整\n");
    printf("5. 性能一致：仍然保持O(1)复杂度\n");
    
    return 0;
}

/**
 * @brief 编译运行说明
 * 
 * 编译：
 * gcc -o simple_hierarchical_timer simple_hierarchical_timer.c -lpthread
 * 
 * 运行：
 * ./simple_hierarchical_timer
 * 
 * 这个简化版本展示了：
 * 1. 如何去掉不必要的复杂性
 * 2. 直接的数学映射关系
 * 3. 更清晰的代码结构
 * 4. 保持算法核心不变的同时简化实现
 * 
 * 面试价值：
 * - 展示简化复杂系统的能力
 * - 体现对算法本质的深刻理解
 * - 显示优秀的工程直觉
 * - 证明能够在保持功能的同时简化设计
 */
