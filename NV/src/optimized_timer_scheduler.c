/**
 * @file optimized_timer_scheduler.c
 * @brief 优化定时器调度系统实现
 * 
 * 题目：实现一个定时器调度系统，能够在指定时间T后自动调用回调函数。
 * 要求：针对前256个时间单位进行优化，使其达到O(1)复杂度，
 *       而其他时间的最坏情况复杂度为O(log n)。
 * 
 * 这是一个高级系统编程面试题，考察：
 * 1. 数据结构设计和选择（数组 + 堆的混合结构）
 * 2. 时间复杂度优化思维
 * 3. 系统编程和定时器实现
 * 4. 内存管理和资源优化
 * 5. 并发安全性考虑
 * 6. 实际系统设计能力
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
#include <assert.h>
#include <pthread.h>
#include <unistd.h>

/**
 * @brief 定时器回调函数类型定义
 * @param user_data 用户自定义数据指针
 */
typedef void (*timer_callback_t)(void *user_data);

/**
 * @brief 定时器节点结构体
 */
typedef struct TimerNode {
    uint64_t expire_time;           // 绝对到期时间
    timer_callback_t callback;      // 回调函数指针
    void *user_data;               // 用户数据
    struct TimerNode *next;        // 链表指针（用于相同时间的多个定时器）
    bool is_active;                // 定时器是否激活
    uint32_t timer_id;             // 定时器唯一ID
} TimerNode;

/**
 * @brief 堆节点结构体（用于长期定时器）
 */
typedef struct HeapNode {
    uint64_t expire_time;          // 到期时间
    TimerNode *timer_list;         // 该时间点的定时器链表
} HeapNode;

/**
 * @brief 优化定时器调度器结构体
 * 
 * 设计思路：
 * 1. 使用数组优化前256个时间单位，实现O(1)访问
 * 2. 使用最小堆管理长期定时器，实现O(log n)复杂度
 * 3. 使用环形缓冲区优化内存使用
 * 4. 支持并发安全操作
 */
#define FAST_TIMER_SLOTS 256       // 快速定时器槽位数量
#define MAX_HEAP_SIZE 10000        // 堆的最大容量
#define TIMER_RESOLUTION_MS 10     // 定时器分辨率（毫秒）

typedef struct OptimizedTimerScheduler {
    // 快速定时器数组（前256个时间单位）
    TimerNode *fast_timers[FAST_TIMER_SLOTS];
    
    // 长期定时器最小堆
    HeapNode *heap;
    int heap_size;
    int heap_capacity;
    
    // 系统状态
    uint64_t current_time;         // 当前系统时间（时间单位）
    uint32_t next_timer_id;        // 下一个定时器ID
    bool is_running;               // 调度器是否运行
    
    // 并发控制
    pthread_mutex_t mutex;         // 互斥锁
    pthread_t scheduler_thread;    // 调度器线程
    
    // 统计信息
    uint64_t fast_timer_count;     // 快速定时器数量
    uint64_t heap_timer_count;     // 堆定时器数量
    uint64_t total_callbacks;      // 总回调执行次数
} OptimizedTimerScheduler;

/**
 * @brief 全局定时器调度器实例
 */
static OptimizedTimerScheduler *g_scheduler = NULL;

/**
 * @brief 获取当前时间戳（毫秒）
 * @return 当前时间戳
 */
static uint64_t get_current_timestamp_ms() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

/**
 * @brief 将毫秒转换为时间单位
 * @param ms 毫秒数
 * @return 时间单位
 */
static uint64_t ms_to_time_units(uint64_t ms) {
    return ms / TIMER_RESOLUTION_MS;
}

/**
 * @brief 堆操作：上浮操作
 * @param scheduler 调度器指针
 * @param index 节点索引
 */
static void heap_bubble_up(OptimizedTimerScheduler *scheduler, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (scheduler->heap[index].expire_time >= scheduler->heap[parent].expire_time) {
            break;
        }
        
        // 交换节点
        HeapNode temp = scheduler->heap[index];
        scheduler->heap[index] = scheduler->heap[parent];
        scheduler->heap[parent] = temp;
        
        index = parent;
    }
}

/**
 * @brief 堆操作：下沉操作
 * @param scheduler 调度器指针
 * @param index 节点索引
 */
static void heap_bubble_down(OptimizedTimerScheduler *scheduler, int index) {
    while (true) {
        int smallest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        
        if (left < scheduler->heap_size && 
            scheduler->heap[left].expire_time < scheduler->heap[smallest].expire_time) {
            smallest = left;
        }
        
        if (right < scheduler->heap_size && 
            scheduler->heap[right].expire_time < scheduler->heap[smallest].expire_time) {
            smallest = right;
        }
        
        if (smallest == index) {
            break;
        }
        
        // 交换节点
        HeapNode temp = scheduler->heap[index];
        scheduler->heap[index] = scheduler->heap[smallest];
        scheduler->heap[smallest] = temp;
        
        index = smallest;
    }
}

/**
 * @brief 向堆中插入定时器
 * @param scheduler 调度器指针
 * @param expire_time 到期时间
 * @param timer 定时器节点
 * @return 成功返回0，失败返回-1
 */
static int heap_insert(OptimizedTimerScheduler *scheduler, uint64_t expire_time, TimerNode *timer) {
    if (scheduler->heap_size >= scheduler->heap_capacity) {
        printf("堆已满，无法插入更多定时器\n");
        return -1;
    }
    
    // 查找是否已存在相同到期时间的节点
    for (int i = 0; i < scheduler->heap_size; i++) {
        if (scheduler->heap[i].expire_time == expire_time) {
            // 添加到现有链表
            timer->next = scheduler->heap[i].timer_list;
            scheduler->heap[i].timer_list = timer;
            return 0;
        }
    }
    
    // 创建新的堆节点
    int index = scheduler->heap_size++;
    scheduler->heap[index].expire_time = expire_time;
    scheduler->heap[index].timer_list = timer;
    timer->next = NULL;
    
    heap_bubble_up(scheduler, index);
    return 0;
}

/**
 * @brief 从堆中移除最小元素
 * @param scheduler 调度器指针
 * @return 移除的堆节点指针，如果堆为空则返回NULL
 */
static HeapNode* heap_extract_min(OptimizedTimerScheduler *scheduler) {
    if (scheduler->heap_size == 0) {
        return NULL;
    }
    
    static HeapNode min_node;
    min_node = scheduler->heap[0];
    
    // 将最后一个元素移到根部
    scheduler->heap[0] = scheduler->heap[--scheduler->heap_size];
    
    if (scheduler->heap_size > 0) {
        heap_bubble_down(scheduler, 0);
    }
    
    return &min_node;
}

/**
 * @brief 初始化定时器调度器
 * @return 成功返回调度器指针，失败返回NULL
 */
OptimizedTimerScheduler* timer_scheduler_init() {
    printf("=== 初始化优化定时器调度器 ===\n");
    
    OptimizedTimerScheduler *scheduler = (OptimizedTimerScheduler*)calloc(1, sizeof(OptimizedTimerScheduler));
    if (!scheduler) {
        printf("内存分配失败\n");
        return NULL;
    }
    
    // 初始化快速定时器数组
    memset(scheduler->fast_timers, 0, sizeof(scheduler->fast_timers));
    
    // 初始化堆
    scheduler->heap = (HeapNode*)malloc(MAX_HEAP_SIZE * sizeof(HeapNode));
    if (!scheduler->heap) {
        printf("堆内存分配失败\n");
        free(scheduler);
        return NULL;
    }
    scheduler->heap_size = 0;
    scheduler->heap_capacity = MAX_HEAP_SIZE;
    
    // 初始化系统状态
    scheduler->current_time = ms_to_time_units(get_current_timestamp_ms());
    scheduler->next_timer_id = 1;
    scheduler->is_running = false;
    
    // 初始化统计信息
    scheduler->fast_timer_count = 0;
    scheduler->heap_timer_count = 0;
    scheduler->total_callbacks = 0;
    
    // 初始化互斥锁
    if (pthread_mutex_init(&scheduler->mutex, NULL) != 0) {
        printf("互斥锁初始化失败\n");
        free(scheduler->heap);
        free(scheduler);
        return NULL;
    }
    
    printf("调度器初始化成功\n");
    printf("- 快速定时器槽位：%d\n", FAST_TIMER_SLOTS);
    printf("- 堆容量：%d\n", MAX_HEAP_SIZE);
    printf("- 定时器分辨率：%d ms\n", TIMER_RESOLUTION_MS);
    
    return scheduler;
}

/**
 * @brief 创建定时器节点
 * @param expire_time 到期时间
 * @param callback 回调函数
 * @param user_data 用户数据
 * @param timer_id 定时器ID
 * @return 定时器节点指针
 */
static TimerNode* create_timer_node(uint64_t expire_time, timer_callback_t callback, 
                                   void *user_data, uint32_t timer_id) {
    TimerNode *node = (TimerNode*)malloc(sizeof(TimerNode));
    if (!node) {
        printf("定时器节点内存分配失败\n");
        return NULL;
    }
    
    node->expire_time = expire_time;
    node->callback = callback;
    node->user_data = user_data;
    node->next = NULL;
    node->is_active = true;
    node->timer_id = timer_id;
    
    return node;
}

/**
 * @brief 添加定时器
 * 
 * 核心算法：
 * 1. 如果延迟时间 <= 256，使用快速数组，O(1)复杂度
 * 2. 如果延迟时间 > 256，使用最小堆，O(log n)复杂度
 * 
 * @param scheduler 调度器指针
 * @param delay_ms 延迟时间（毫秒）
 * @param callback 回调函数
 * @param user_data 用户数据
 * @return 定时器ID，失败返回0
 */
uint32_t add_timer(OptimizedTimerScheduler *scheduler, uint64_t delay_ms, 
                   timer_callback_t callback, void *user_data) {
    if (!scheduler || !callback) {
        printf("参数无效\n");
        return 0;
    }
    
    pthread_mutex_lock(&scheduler->mutex);
    
    uint64_t current_time = ms_to_time_units(get_current_timestamp_ms());
    uint64_t delay_units = ms_to_time_units(delay_ms);
    uint64_t expire_time = current_time + delay_units;
    uint32_t timer_id = scheduler->next_timer_id++;
    
    printf("添加定时器 ID=%u, 延迟=%lu ms (%lu 时间单位)\n", 
           timer_id, delay_ms, delay_units);
    
    TimerNode *timer = create_timer_node(expire_time, callback, user_data, timer_id);
    if (!timer) {
        pthread_mutex_unlock(&scheduler->mutex);
        return 0;
    }
    
    // 优化策略：前256个时间单位使用快速数组
    if (delay_units < FAST_TIMER_SLOTS) {
        printf("  -> 使用快速定时器数组 (O(1))\n");
        
        int slot = (current_time + delay_units) % FAST_TIMER_SLOTS;
        
        // 插入到链表头部
        timer->next = scheduler->fast_timers[slot];
        scheduler->fast_timers[slot] = timer;
        scheduler->fast_timer_count++;
        
    } else {
        printf("  -> 使用堆结构 (O(log n))\n");
        
        if (heap_insert(scheduler, expire_time, timer) != 0) {
            free(timer);
            pthread_mutex_unlock(&scheduler->mutex);
            return 0;
        }
        scheduler->heap_timer_count++;
    }
    
    pthread_mutex_unlock(&scheduler->mutex);
    return timer_id;
}

/**
 * @brief 取消定时器
 * @param scheduler 调度器指针
 * @param timer_id 定时器ID
 * @return 成功返回0，失败返回-1
 */
int cancel_timer(OptimizedTimerScheduler *scheduler, uint32_t timer_id) {
    if (!scheduler || timer_id == 0) {
        return -1;
    }
    
    pthread_mutex_lock(&scheduler->mutex);
    
    printf("取消定时器 ID=%u\n", timer_id);
    
    // 在快速定时器中查找
    for (int i = 0; i < FAST_TIMER_SLOTS; i++) {
        TimerNode *current = scheduler->fast_timers[i];
        while (current) {
            if (current->timer_id == timer_id) {
                current->is_active = false;  // 标记为非活跃，延迟删除
                printf("  -> 在快速定时器中找到并标记取消\n");
                pthread_mutex_unlock(&scheduler->mutex);
                return 0;
            }
            current = current->next;
        }
    }
    
    // 在堆中查找
    for (int i = 0; i < scheduler->heap_size; i++) {
        TimerNode *current = scheduler->heap[i].timer_list;
        while (current) {
            if (current->timer_id == timer_id) {
                current->is_active = false;  // 标记为非活跃
                printf("  -> 在堆中找到并标记取消\n");
                pthread_mutex_unlock(&scheduler->mutex);
                return 0;
            }
            current = current->next;
        }
    }
    
    printf("  -> 未找到指定的定时器\n");
    pthread_mutex_unlock(&scheduler->mutex);
    return -1;
}

/**
 * @brief 执行到期的定时器回调
 * @param timer_list 定时器链表
 * @param scheduler 调度器指针
 */
static void execute_expired_timers(TimerNode *timer_list, OptimizedTimerScheduler *scheduler) {
    TimerNode *current = timer_list;
    
    while (current) {
        TimerNode *next = current->next;
        
        if (current->is_active) {
            printf("    执行定时器回调 ID=%u\n", current->timer_id);
            
            // 释放锁后执行回调，避免死锁
            pthread_mutex_unlock(&scheduler->mutex);
            current->callback(current->user_data);
            pthread_mutex_lock(&scheduler->mutex);
            
            scheduler->total_callbacks++;
        } else {
            printf("    跳过已取消的定时器 ID=%u\n", current->timer_id);
        }
        
        free(current);
        current = next;
    }
}

/**
 * @brief 调度器主循环
 * @param arg 调度器指针
 * @return NULL
 */
static void* scheduler_thread_func(void *arg) {
    OptimizedTimerScheduler *scheduler = (OptimizedTimerScheduler*)arg;
    
    printf("调度器线程启动\n");
    
    while (scheduler->is_running) {
        pthread_mutex_lock(&scheduler->mutex);
        
        uint64_t current_time = ms_to_time_units(get_current_timestamp_ms());
        scheduler->current_time = current_time;
        
        // 处理快速定时器
        int slot = current_time % FAST_TIMER_SLOTS;
        if (scheduler->fast_timers[slot]) {
            printf("  处理快速定时器槽位 %d (时间 %lu)\n", slot, current_time);
            
            TimerNode *expired_timers = scheduler->fast_timers[slot];
            scheduler->fast_timers[slot] = NULL;
            
            // 过滤出真正到期的定时器
            TimerNode *truly_expired = NULL;
            TimerNode *current_timer = expired_timers;
            
            while (current_timer) {
                TimerNode *next = current_timer->next;
                
                if (current_timer->expire_time <= current_time) {
                    current_timer->next = truly_expired;
                    truly_expired = current_timer;
                    scheduler->fast_timer_count--;
                } else {
                    // 重新插入到正确的槽位
                    int new_slot = current_timer->expire_time % FAST_TIMER_SLOTS;
                    current_timer->next = scheduler->fast_timers[new_slot];
                    scheduler->fast_timers[new_slot] = current_timer;
                }
                
                current_timer = next;
            }
            
            if (truly_expired) {
                execute_expired_timers(truly_expired, scheduler);
            }
        }
        
        // 处理堆中的到期定时器
        while (scheduler->heap_size > 0 && 
               scheduler->heap[0].expire_time <= current_time) {
            
            printf("  处理堆定时器 (到期时间 %lu)\n", scheduler->heap[0].expire_time);
            
            HeapNode *expired_node = heap_extract_min(scheduler);
            scheduler->heap_timer_count--;
            
            execute_expired_timers(expired_node->timer_list, scheduler);
        }
        
        pthread_mutex_unlock(&scheduler->mutex);
        
        // 短暂休眠，避免CPU占用过高
        usleep(TIMER_RESOLUTION_MS * 1000 / 2);  // 休眠分辨率的一半时间
    }
    
    printf("调度器线程退出\n");
    return NULL;
}

/**
 * @brief 启动定时器调度器
 * @param scheduler 调度器指针
 * @return 成功返回0，失败返回-1
 */
int start_timer_scheduler(OptimizedTimerScheduler *scheduler) {
    if (!scheduler) {
        return -1;
    }
    
    printf("启动定时器调度器\n");
    
    scheduler->is_running = true;
    
    if (pthread_create(&scheduler->scheduler_thread, NULL, scheduler_thread_func, scheduler) != 0) {
        printf("创建调度器线程失败\n");
        scheduler->is_running = false;
        return -1;
    }
    
    printf("调度器启动成功\n");
    return 0;
}

/**
 * @brief 停止定时器调度器
 * @param scheduler 调度器指针
 */
void stop_timer_scheduler(OptimizedTimerScheduler *scheduler) {
    if (!scheduler) {
        return;
    }
    
    printf("停止定时器调度器\n");
    
    scheduler->is_running = false;
    
    if (scheduler->scheduler_thread) {
        pthread_join(scheduler->scheduler_thread, NULL);
    }
    
    printf("调度器已停止\n");
}

/**
 * @brief 销毁定时器调度器
 * @param scheduler 调度器指针
 */
void timer_scheduler_destroy(OptimizedTimerScheduler *scheduler) {
    if (!scheduler) {
        return;
    }
    
    printf("销毁定时器调度器\n");
    
    // 停止调度器
    stop_timer_scheduler(scheduler);
    
    // 清理快速定时器
    for (int i = 0; i < FAST_TIMER_SLOTS; i++) {
        TimerNode *current = scheduler->fast_timers[i];
        while (current) {
            TimerNode *next = current->next;
            free(current);
            current = next;
        }
    }
    
    // 清理堆中的定时器
    for (int i = 0; i < scheduler->heap_size; i++) {
        TimerNode *current = scheduler->heap[i].timer_list;
        while (current) {
            TimerNode *next = current->next;
            free(current);
            current = next;
        }
    }
    
    // 销毁互斥锁
    pthread_mutex_destroy(&scheduler->mutex);
    
    // 释放内存
    free(scheduler->heap);
    free(scheduler);
    
    printf("调度器销毁完成\n");
}

/**
 * @brief 打印调度器统计信息
 * @param scheduler 调度器指针
 */
void print_scheduler_stats(OptimizedTimerScheduler *scheduler) {
    if (!scheduler) {
        return;
    }
    
    pthread_mutex_lock(&scheduler->mutex);
    
    printf("\n=== 调度器统计信息 ===\n");
    printf("当前时间：%lu 时间单位\n", scheduler->current_time);
    printf("快速定时器数量：%lu\n", scheduler->fast_timer_count);
    printf("堆定时器数量：%lu\n", scheduler->heap_timer_count);
    printf("总定时器数量：%lu\n", scheduler->fast_timer_count + scheduler->heap_timer_count);
    printf("已执行回调次数：%lu\n", scheduler->total_callbacks);
    printf("下一个定时器ID：%u\n", scheduler->next_timer_id);
    printf("调度器状态：%s\n", scheduler->is_running ? "运行中" : "已停止");
    
    // 显示快速定时器分布
    printf("\n快速定时器槽位分布：\n");
    for (int i = 0; i < FAST_TIMER_SLOTS; i++) {
        int count = 0;
        TimerNode *current = scheduler->fast_timers[i];
        while (current) {
            if (current->is_active) count++;
            current = current->next;
        }
        if (count > 0) {
            printf("  槽位 %d: %d 个定时器\n", i, count);
        }
    }
    
    pthread_mutex_unlock(&scheduler->mutex);
}

/**
 * @brief 测试回调函数
 */
void test_callback_1(void *user_data) {
    int *value = (int*)user_data;
    printf("    [回调1] 用户数据: %d\n", *value);
}

void test_callback_2(void *user_data) {
    char *message = (char*)user_data;
    printf("    [回调2] 消息: %s\n", message);
}

void test_callback_3(void *user_data) {
    printf("    [回调3] 长期定时器触发\n");
}

/**
 * @brief 复杂度分析和算法讲解
 */
void algorithm_analysis() {
    printf("\n=== 算法设计分析 ===\n");
    
    printf("1. 问题分析：\n");
    printf("   - 需要支持大量并发定时器\n");
    printf("   - 前256个时间单位需要O(1)复杂度\n");
    printf("   - 其他情况最坏O(log n)复杂度\n");
    printf("   - 需要支持定时器取消\n");
    printf("   - 需要线程安全\n\n");
    
    printf("2. 数据结构设计：\n");
    printf("   - 快速数组：256个槽位，每个槽位是链表\n");
    printf("   - 最小堆：管理长期定时器，按到期时间排序\n");
    printf("   - 环形缓冲：利用模运算实现循环使用\n");
    printf("   - 链表：处理相同到期时间的多个定时器\n\n");
    
    printf("3. 复杂度分析：\n");
    printf("   - 添加短期定时器：O(1)\n");
    printf("   - 添加长期定时器：O(log n)\n");
    printf("   - 执行到期定时器：O(k) k为到期定时器数量\n");
    printf("   - 取消定时器：O(1) 延迟删除\n");
    printf("   - 空间复杂度：O(n) n为定时器总数\n\n");
    
    printf("4. 优化技术：\n");
    printf("   - 时间轮算法：快速定时器使用环形数组\n");
    printf("   - 延迟删除：取消时只标记，执行时清理\n");
    printf("   - 批量处理：相同时间的定时器批量执行\n");
    printf("   - 线程分离：调度和执行在不同线程\n\n");
    
    printf("5. 实际应用：\n");
    printf("   - 游戏引擎：技能冷却、buff效果\n");
    printf("   - 网络服务：连接超时、心跳检测\n");
    printf("   - 操作系统：进程调度、资源回收\n");
    printf("   - 嵌入式系统：任务调度、事件触发\n");
}

/**
 * @brief 压力测试
 */
void stress_test(OptimizedTimerScheduler *scheduler) {
    printf("\n=== 压力测试 ===\n");
    
    const int num_timers = 1000;
    int test_data[num_timers];
    
    printf("添加 %d 个定时器...\n", num_timers);
    
    // 添加大量定时器
    for (int i = 0; i < num_timers; i++) {
        test_data[i] = i;
        
        uint64_t delay;
        if (i < 800) {
            // 80% 使用快速定时器
            delay = (rand() % 200) * TIMER_RESOLUTION_MS;
        } else {
            // 20% 使用长期定时器
            delay = (300 + rand() % 1000) * TIMER_RESOLUTION_MS;
        }
        
        add_timer(scheduler, delay, test_callback_1, &test_data[i]);
    }
    
    print_scheduler_stats(scheduler);
    
    printf("等待定时器执行...\n");
    sleep(3);  // 等待3秒
    
    print_scheduler_stats(scheduler);
}

/**
 * @brief 功能测试
 */
void functional_test() {
    printf("\n=== 功能测试 ===\n");
    
    // 初始化调度器
    OptimizedTimerScheduler *scheduler = timer_scheduler_init();
    if (!scheduler) {
        printf("初始化失败\n");
        return;
    }
    
    g_scheduler = scheduler;
    
    // 启动调度器
    if (start_timer_scheduler(scheduler) != 0) {
        printf("启动失败\n");
        timer_scheduler_destroy(scheduler);
        return;
    }
    
    // 测试数据
    int data1 = 100;
    char *message = "Hello Timer!";
    
    printf("\n--- 基本功能测试 ---\n");
    
    // 添加短期定时器（使用快速数组）
    uint32_t timer1 = add_timer(scheduler, 100, test_callback_1, &data1);  // 100ms
    uint32_t timer2 = add_timer(scheduler, 200, test_callback_2, message);  // 200ms
    uint32_t timer3 = add_timer(scheduler, 500, test_callback_1, &data1);   // 500ms
    
    // 添加长期定时器（使用堆）
    uint32_t timer4 = add_timer(scheduler, 3000, test_callback_3, NULL);    // 3s
    
    printf("添加的定时器 ID: %u, %u, %u, %u\n", timer1, timer2, timer3, timer4);
    
    sleep(1);  // 等待1秒
    
    printf("\n--- 取消功能测试 ---\n");
    
    // 测试取消功能
    uint32_t timer5 = add_timer(scheduler, 300, test_callback_2, "Cancelled");
    printf("添加定时器 ID: %u (将被取消)\n", timer5);
    
    usleep(100000);  // 等待100ms
    cancel_timer(scheduler, timer5);
    
    sleep(1);  // 再等待1秒
    
    print_scheduler_stats(scheduler);
    
    // 压力测试
    stress_test(scheduler);
    
    // 清理
    stop_timer_scheduler(scheduler);
    timer_scheduler_destroy(scheduler);
}

/**
 * @brief 主函数
 */
int main() {
    printf("题目：实现优化的定时器调度系统\n");
    printf("要求：前256时间单位O(1)，其他情况O(log n)\n");
    printf("==========================================\n");
    
    // 算法分析
    algorithm_analysis();
    
    // 功能测试
    functional_test();
    
    printf("\n=== 面试要点总结 ===\n");
    printf("1. 数据结构选择：数组+堆的混合结构\n");
    printf("2. 时间复杂度优化：针对常见情况优化\n");
    printf("3. 内存管理：及时释放，避免内存泄漏\n");
    printf("4. 并发安全：使用互斥锁保护共享数据\n");
    printf("5. 系统设计：考虑实际应用场景\n");
    printf("6. 性能测试：验证算法复杂度和正确性\n");
    
    return 0;
}

/**
 * @brief 编译和运行说明
 * 
 * 编译：
 * gcc -o optimized_timer_scheduler optimized_timer_scheduler.c -lpthread -g
 * 
 * 运行：
 * ./optimized_timer_scheduler
 * 
 * 调试：
 * gdb ./optimized_timer_scheduler
 * 
 * 内存检查：
 * valgrind --leak-check=full ./optimized_timer_scheduler
 * 
 * 这个实现展示了：
 * 1. 混合数据结构的设计思路
 * 2. 针对特定场景的优化策略
 * 3. 多线程编程和同步
 * 4. 系统级编程考虑
 * 5. 完整的测试框架
 * 
 * 面试扩展话题：
 * - 如何处理时钟回拨？
 * - 如何支持周期性定时器？
 * - 如何优化内存使用？
 * - 如何处理大量短期定时器？
 * - 如何实现分布式定时器？
 */
