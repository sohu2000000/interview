/**
 * @file error_rate_monitor.c
 * @brief 错误率监控系统实现
 * 
 * 题目：实现一个错误监控系统，如果在最近TIME秒内错误次数超过MAX_ERROR，则退出程序。
 * 要求实现checkRC(bool rc)函数，其中rc=0表示成功，rc=1表示错误。
 * 
 * 这是一个经典的系统设计面试题，考察：
 * 1. 滑动窗口算法设计
 * 2. 时间序列数据处理
 * 3. 数据结构选择和优化
 * 4. 系统监控和异常处理
 * 5. 内存管理和性能优化
 * 6. 多线程安全性考虑
 * 
 * @author feliu@nvidia.com
 * @date 2025-10-23
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>

// 系统配置常量
#define MAX_ERROR 5        // 最大允许错误数
#define TIME 10           // 时间窗口（秒）
#define MAX_RECORDS 1000  // 最大记录数（用于环形缓冲区）

/**
 * @brief 错误记录结构体
 */
typedef struct ErrorRecord {
    time_t timestamp;      // 错误发生的时间戳
    bool is_error;        // 是否为错误（true=错误，false=成功）
} ErrorRecord;

/**
 * @brief 错误监控器结构体
 * 
 * 设计思路：
 * 1. 使用环形缓冲区存储最近的错误记录
 * 2. 维护当前时间窗口内的错误计数
 * 3. 支持多线程安全操作
 */
typedef struct ErrorMonitor {
    // 环形缓冲区
    ErrorRecord records[MAX_RECORDS];
    int head;                    // 头指针
    int tail;                    // 尾指针
    int count;                   // 当前记录数
    
    // 统计信息
    int current_errors;          // 当前时间窗口内的错误数
    int total_operations;        // 总操作数
    int total_errors;           // 总错误数
    
    // 时间信息
    time_t last_cleanup_time;   // 上次清理过期记录的时间
    
    // 线程安全
    pthread_mutex_t mutex;      // 互斥锁
    
    // 配置参数
    int max_errors;             // 最大错误数阈值
    int time_window;            // 时间窗口大小
    
    // 状态标志
    bool should_exit;           // 是否应该退出程序
} ErrorMonitor;

// 全局错误监控器实例
static ErrorMonitor *g_monitor = NULL;

/**
 * @brief 获取当前系统时间
 * @return 当前时间戳
 */
time_t get_time() {
    return time(NULL);
}

/**
 * @brief 获取高精度时间戳（微秒级）
 * @return 微秒级时间戳
 */
long long get_time_microseconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000LL + tv.tv_usec;
}

/**
 * @brief 初始化错误监控器
 * @param max_errors 最大错误数阈值
 * @param time_window 时间窗口大小（秒）
 * @return 初始化成功的监控器指针，失败返回NULL
 */
ErrorMonitor* error_monitor_init(int max_errors, int time_window) {
    printf("=== 初始化错误监控器 ===\n");
    printf("最大错误数：%d\n", max_errors);
    printf("时间窗口：%d 秒\n", time_window);
    
    ErrorMonitor *monitor = (ErrorMonitor*)malloc(sizeof(ErrorMonitor));
    if (!monitor) {
        printf("内存分配失败\n");
        return NULL;
    }
    
    // 初始化环形缓冲区
    memset(monitor->records, 0, sizeof(monitor->records));
    monitor->head = 0;
    monitor->tail = 0;
    monitor->count = 0;
    
    // 初始化统计信息
    monitor->current_errors = 0;
    monitor->total_operations = 0;
    monitor->total_errors = 0;
    
    // 初始化时间信息
    monitor->last_cleanup_time = get_time();
    
    // 初始化配置
    monitor->max_errors = max_errors;
    monitor->time_window = time_window;
    
    // 初始化状态
    monitor->should_exit = false;
    
    // 初始化互斥锁
    if (pthread_mutex_init(&monitor->mutex, NULL) != 0) {
        printf("互斥锁初始化失败\n");
        free(monitor);
        return NULL;
    }
    
    printf("错误监控器初始化成功\n");
    return monitor;
}

/**
 * @brief 销毁错误监控器
 * @param monitor 监控器指针
 */
void error_monitor_destroy(ErrorMonitor *monitor) {
    if (!monitor) return;
    
    printf("销毁错误监控器\n");
    pthread_mutex_destroy(&monitor->mutex);
    free(monitor);
}

/**
 * @brief 清理过期的错误记录
 * 
 * 算法思路：
 * 1. 从尾部开始检查记录
 * 2. 移除超过时间窗口的记录
 * 3. 更新错误计数
 * 
 * @param monitor 监控器指针
 * @param current_time 当前时间
 */
static void cleanup_expired_records(ErrorMonitor *monitor, time_t current_time) {
    time_t cutoff_time = current_time - monitor->time_window;
    
    while (monitor->count > 0) {
        // 检查尾部记录是否过期
        ErrorRecord *tail_record = &monitor->records[monitor->tail];
        
        if (tail_record->timestamp >= cutoff_time) {
            // 尾部记录仍在时间窗口内，停止清理
            break;
        }
        
        // 移除过期记录
        if (tail_record->is_error) {
            monitor->current_errors--;
        }
        
        // 移动尾指针
        monitor->tail = (monitor->tail + 1) % MAX_RECORDS;
        monitor->count--;
    }
    
    monitor->last_cleanup_time = current_time;
}

/**
 * @brief 添加新的操作记录
 * @param monitor 监控器指针
 * @param is_error 是否为错误
 * @param timestamp 时间戳
 */
static void add_record(ErrorMonitor *monitor, bool is_error, time_t timestamp) {
    // 如果缓冲区满了，需要移除最旧的记录
    if (monitor->count >= MAX_RECORDS) {
        ErrorRecord *tail_record = &monitor->records[monitor->tail];
        if (tail_record->is_error) {
            monitor->current_errors--;
        }
        monitor->tail = (monitor->tail + 1) % MAX_RECORDS;
        monitor->count--;
    }
    
    // 添加新记录
    ErrorRecord *new_record = &monitor->records[monitor->head];
    new_record->timestamp = timestamp;
    new_record->is_error = is_error;
    
    monitor->head = (monitor->head + 1) % MAX_RECORDS;
    monitor->count++;
    
    // 更新统计信息
    if (is_error) {
        monitor->current_errors++;
        monitor->total_errors++;
    }
    monitor->total_operations++;
}

/**
 * @brief 方法1：简单实现 - 每次都遍历所有记录
 * 
 * 算法思路：
 * 1. 记录所有操作的时间戳和结果
 * 2. 每次检查时遍历所有记录，统计时间窗口内的错误数
 * 
 * 时间复杂度：O(n) - n为记录数
 * 空间复杂度：O(n)
 * 
 * @param rc 操作结果（0=成功，1=错误）
 */
void checkRC_simple(bool rc) {
    static ErrorRecord simple_records[MAX_RECORDS];
    static int simple_count = 0;
    
    time_t current_time = get_time();
    bool is_error = (rc == 1);
    
    printf("检查操作结果：%s (时间：%ld)\n", is_error ? "错误" : "成功", current_time);
    
    // 添加新记录
    if (simple_count < MAX_RECORDS) {
        simple_records[simple_count].timestamp = current_time;
        simple_records[simple_count].is_error = is_error;
        simple_count++;
    }
    
    // 统计时间窗口内的错误数
    int errors_in_window = 0;
    time_t cutoff_time = current_time - TIME;
    
    for (int i = 0; i < simple_count; i++) {
        if (simple_records[i].timestamp >= cutoff_time && simple_records[i].is_error) {
            errors_in_window++;
        }
    }
    
    printf("  当前时间窗口内错误数：%d/%d\n", errors_in_window, MAX_ERROR);
    
    // 检查是否超过阈值
    if (errors_in_window > MAX_ERROR) {
        printf("错误数超过阈值！程序退出。\n");
        exit(1);
    }
}

/**
 * @brief 方法2：优化实现 - 使用滑动窗口和环形缓冲区
 * 
 * 算法思路：
 * 1. 使用环形缓冲区存储记录，避免数组移动
 * 2. 维护当前错误计数，避免每次重新统计
 * 3. 定期清理过期记录
 * 
 * 时间复杂度：O(1) 摊还复杂度
 * 空间复杂度：O(k) - k为缓冲区大小
 * 
 * @param rc 操作结果（0=成功，1=错误）
 */
void checkRC_optimized(bool rc) {
    if (!g_monitor) {
        printf("错误监控器未初始化\n");
        return;
    }
    
    pthread_mutex_lock(&g_monitor->mutex);
    
    time_t current_time = get_time();
    bool is_error = (rc == 1);
    
    printf("检查操作结果：%s (时间：%ld)\n", is_error ? "错误" : "成功", current_time);
    
    // 定期清理过期记录（每秒清理一次）
    if (current_time - g_monitor->last_cleanup_time >= 1) {
        cleanup_expired_records(g_monitor, current_time);
    }
    
    // 添加新记录
    add_record(g_monitor, is_error, current_time);
    
    printf("  当前时间窗口内错误数：%d/%d\n", 
           g_monitor->current_errors, g_monitor->max_errors);
    printf("  总操作数：%d，总错误数：%d，错误率：%.2f%%\n",
           g_monitor->total_operations, g_monitor->total_errors,
           g_monitor->total_operations > 0 ? 
           (float)g_monitor->total_errors / g_monitor->total_operations * 100 : 0);
    
    // 检查是否超过阈值
    if (g_monitor->current_errors > g_monitor->max_errors) {
        printf("错误数超过阈值！程序应该退出。\n");
        g_monitor->should_exit = true;
        
        // 可以选择直接退出或设置标志
        // exit(1);  // 直接退出
    }
    
    pthread_mutex_unlock(&g_monitor->mutex);
}

/**
 * @brief 方法3：高性能实现 - 使用时间桶算法
 * 
 * 算法思路：
 * 1. 将时间划分为固定大小的桶（如1秒一个桶）
 * 2. 每个桶记录该时间段内的错误数
 * 3. 滑动窗口只需要检查相关的桶
 * 
 * 时间复杂度：O(1)
 * 空间复杂度：O(TIME) - 时间窗口大小
 * 
 * @param rc 操作结果（0=成功，1=错误）
 */

#define BUCKET_SIZE 1  // 每个桶的时间大小（秒）
#define NUM_BUCKETS (TIME / BUCKET_SIZE + 1)

typedef struct TimeBucket {
    time_t start_time;    // 桶的开始时间
    int error_count;      // 该时间段内的错误数
    int total_count;      // 该时间段内的总操作数
} TimeBucket;

static TimeBucket time_buckets[NUM_BUCKETS];
static int current_bucket_index = 0;
static bool buckets_initialized = false;

void checkRC_time_bucket(bool rc) {
    time_t current_time = get_time();
    bool is_error = (rc == 1);
    
    printf("检查操作结果：%s (时间：%ld)\n", is_error ? "错误" : "成功", current_time);
    
    // 初始化时间桶
    if (!buckets_initialized) {
        for (int i = 0; i < NUM_BUCKETS; i++) {
            time_buckets[i].start_time = current_time - TIME + i * BUCKET_SIZE;
            time_buckets[i].error_count = 0;
            time_buckets[i].total_count = 0;
        }
        buckets_initialized = true;
    }
    
    // 找到当前时间对应的桶
    int bucket_index = (current_time / BUCKET_SIZE) % NUM_BUCKETS;
    
    // 如果桶过期，重置桶
    if (current_time - time_buckets[bucket_index].start_time >= TIME) {
        time_buckets[bucket_index].start_time = current_time;
        time_buckets[bucket_index].error_count = 0;
        time_buckets[bucket_index].total_count = 0;
    }
    
    // 更新桶中的统计信息
    time_buckets[bucket_index].total_count++;
    if (is_error) {
        time_buckets[bucket_index].error_count++;
    }
    
    // 统计时间窗口内的总错误数
    int total_errors = 0;
    time_t cutoff_time = current_time - TIME;
    
    for (int i = 0; i < NUM_BUCKETS; i++) {
        if (time_buckets[i].start_time >= cutoff_time) {
            total_errors += time_buckets[i].error_count;
        }
    }
    
    printf("  当前时间窗口内错误数：%d/%d\n", total_errors, MAX_ERROR);
    
    // 检查是否超过阈值
    if (total_errors > MAX_ERROR) {
        printf("错误数超过阈值！程序退出。\n");
        exit(1);
    }
}

/**
 * @brief 标准的checkRC实现（题目要求的接口）
 * @param rc 操作结果（false=成功，true=错误）
 */
void checkRC(bool rc) {
    // 使用优化实现
    checkRC_optimized(rc);
    
    // 如果监控器指示应该退出，则退出程序
    if (g_monitor && g_monitor->should_exit) {
        printf("程序因错误率过高而退出\n");
        error_monitor_destroy(g_monitor);
        exit(1);
    }
}

/**
 * @brief 模拟doSomething函数
 * @return 操作结果（false=成功，true=错误）
 */
bool doSomething() {
    static int call_count = 0;
    call_count++;
    
    // 模拟不同的错误模式
    if (call_count <= 20) {
        // 前20次操作，10%的错误率
        return (rand() % 10 == 0);
    } else if (call_count <= 40) {
        // 第21-40次操作，30%的错误率
        return (rand() % 10 < 3);
    } else {
        // 后续操作，50%的错误率（触发阈值）
        return (rand() % 2 == 0);
    }
}

/**
 * @brief 算法复杂度分析
 */
void complexity_analysis() {
    printf("\n=== 算法复杂度分析 ===\n");
    
    printf("方法对比：\n");
    printf("┌─────────────────┬──────────────┬──────────────┬────────────────┐\n");
    printf("│      方法       │  时间复杂度  │  空间复杂度  │      特点      │\n");
    printf("├─────────────────┼──────────────┼──────────────┼────────────────┤\n");
    printf("│ 1. 简单遍历     │    O(n)      │    O(n)      │ 实现简单       │\n");
    printf("│ 2. 滑动窗口     │    O(1)*     │    O(k)      │ 摊还复杂度     │\n");
    printf("│ 3. 时间桶       │    O(1)      │   O(TIME)    │ 真正O(1)      │\n");
    printf("└─────────────────┴──────────────┴──────────────┴────────────────┘\n");
    printf("注：k为缓冲区大小，*表示摊还复杂度\n");
    
    printf("\n核心思想分析：\n");
    printf("1. 简单遍历法：\n");
    printf("   - 每次检查都遍历所有记录\n");
    printf("   - 实现简单但效率低\n");
    printf("   - 适合记录数量少的场景\n\n");
    
    printf("2. 滑动窗口法：\n");
    printf("   - 使用环形缓冲区存储记录\n");
    printf("   - 维护当前错误计数，避免重复统计\n");
    printf("   - 定期清理过期记录\n");
    printf("   - 摊还复杂度为O(1)\n\n");
    
    printf("3. 时间桶法：\n");
    printf("   - 将时间划分为固定大小的桶\n");
    printf("   - 每个桶统计该时间段的错误数\n");
    printf("   - 检查时只需要遍历相关桶\n");
    printf("   - 真正的O(1)复杂度\n\n");
    
    printf("选择建议：\n");
    printf("- 记录数少：选择简单遍历\n");
    printf("- 需要详细记录：选择滑动窗口\n");
    printf("- 高频调用：选择时间桶\n");
    printf("- 内存受限：选择滑动窗口\n");
}

/**
 * @brief 性能测试
 */
void performance_test() {
    printf("\n=== 性能测试 ===\n");
    
    const int test_operations = 10000;
    printf("执行 %d 次操作的性能测试\n", test_operations);
    
    // 初始化监控器
    g_monitor = error_monitor_init(MAX_ERROR, TIME);
    if (!g_monitor) {
        printf("监控器初始化失败\n");
        return;
    }
    
    // 测试优化方法的性能
    clock_t start = clock();
    
    for (int i = 0; i < test_operations; i++) {
        bool rc = (rand() % 20 == 0);  // 5%错误率，不会触发阈值
        checkRC_optimized(rc);
        
        if (i % 1000 == 0) {
            printf("已完成 %d 次操作\n", i);
        }
    }
    
    clock_t end = clock();
    double time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("性能测试完成\n");
    printf("总耗时：%.6f 秒\n", time_used);
    printf("平均每次操作耗时：%.6f 微秒\n", time_used * 1000000 / test_operations);
    
    // 打印统计信息
    printf("\n最终统计：\n");
    printf("总操作数：%d\n", g_monitor->total_operations);
    printf("总错误数：%d\n", g_monitor->total_errors);
    printf("当前窗口错误数：%d\n", g_monitor->current_errors);
    printf("错误率：%.2f%%\n", 
           (float)g_monitor->total_errors / g_monitor->total_operations * 100);
    
    error_monitor_destroy(g_monitor);
    g_monitor = NULL;
}

/**
 * @brief 边界情况测试
 */
void test_edge_cases() {
    printf("\n=== 边界情况测试 ===\n");
    
    // 初始化监控器
    g_monitor = error_monitor_init(3, 5);  // 5秒内最多3个错误
    
    printf("测试1：连续错误（应该触发退出）\n");
    for (int i = 0; i < 5; i++) {
        printf("第 %d 次错误操作\n", i + 1);
        checkRC_optimized(true);
        if (g_monitor->should_exit) {
            printf("检测到应该退出的标志\n");
            break;
        }
        sleep(1);
    }
    
    // 重置监控器
    error_monitor_destroy(g_monitor);
    g_monitor = error_monitor_init(3, 5);
    
    printf("\n测试2：时间窗口滑动（错误应该过期）\n");
    
    // 先产生一些错误
    for (int i = 0; i < 3; i++) {
        printf("产生错误 %d\n", i + 1);
        checkRC_optimized(true);
        sleep(1);
    }
    
    printf("等待时间窗口滑动...\n");
    sleep(6);  // 等待超过时间窗口
    
    // 现在应该可以再次产生错误而不会退出
    printf("时间窗口滑动后，产生新错误\n");
    checkRC_optimized(true);
    printf("程序仍在运行，时间窗口滑动正常\n");
    
    error_monitor_destroy(g_monitor);
    g_monitor = NULL;
}

/**
 * @brief 多线程安全测试
 */
void* thread_test_function(void* arg) {
    int thread_id = *(int*)arg;
    
    for (int i = 0; i < 100; i++) {
        bool rc = (rand() % 10 == 0);  // 10%错误率
        checkRC_optimized(rc);
        usleep(10000);  // 10ms
        
        if (g_monitor && g_monitor->should_exit) {
            break;
        }
    }
    
    printf("线程 %d 完成\n", thread_id);
    return NULL;
}

void test_thread_safety() {
    printf("\n=== 多线程安全测试 ===\n");
    
    g_monitor = error_monitor_init(MAX_ERROR, TIME);
    if (!g_monitor) {
        printf("监控器初始化失败\n");
        return;
    }
    
    const int num_threads = 4;
    pthread_t threads[num_threads];
    int thread_ids[num_threads];
    
    printf("启动 %d 个线程进行并发测试\n", num_threads);
    
    // 创建线程
    for (int i = 0; i < num_threads; i++) {
        thread_ids[i] = i;
        if (pthread_create(&threads[i], NULL, thread_test_function, &thread_ids[i]) != 0) {
            printf("线程 %d 创建失败\n", i);
        }
    }
    
    // 等待所有线程完成
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("多线程测试完成\n");
    printf("最终统计：总操作=%d, 总错误=%d, 当前窗口错误=%d\n",
           g_monitor->total_operations, g_monitor->total_errors, g_monitor->current_errors);
    
    error_monitor_destroy(g_monitor);
    g_monitor = NULL;
}

/**
 * @brief 主测试函数
 */
void test_error_monitoring() {
    printf("=== 错误率监控系统测试 ===\n");
    
    // 初始化随机数生成器
    srand(time(NULL));
    
    // 初始化全局监控器
    g_monitor = error_monitor_init(MAX_ERROR, TIME);
    if (!g_monitor) {
        printf("监控器初始化失败\n");
        return;
    }
    
    printf("\n--- 基本功能测试 ---\n");
    printf("模拟程序运行，监控错误率\n");
    
    // 模拟主程序循环
    for (int i = 0; i < 50; i++) {
        bool rc = doSomething();
        checkRC(rc);
        
        // 如果程序应该退出，则跳出循环
        if (g_monitor->should_exit) {
            printf("程序因错误率过高而停止\n");
            break;
        }
        
        sleep(1);  // 每秒一次操作
    }
    
    error_monitor_destroy(g_monitor);
    g_monitor = NULL;
}

/**
 * @brief 信号处理函数
 */
void signal_handler(int sig) {
    printf("\n接收到信号 %d，程序退出\n", sig);
    if (g_monitor) {
        error_monitor_destroy(g_monitor);
    }
    exit(0);
}

/**
 * @brief 主函数
 */
int main() {
    printf("题目：实现错误率监控系统\n");
    printf("要求：在最近%d秒内错误数超过%d则退出程序\n", TIME, MAX_ERROR);
    printf("========================================\n");
    
    // 注册信号处理函数
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    // 算法复杂度分析
    complexity_analysis();
    
    // 基本功能测试
    test_error_monitoring();
    
    // 边界情况测试
    test_edge_cases();
    
    // 性能测试
    performance_test();
    
    // 多线程安全测试
    test_thread_safety();
    
    printf("\n=== 面试要点总结 ===\n");
    printf("1. 问题分析：滑动窗口 + 阈值检测\n");
    printf("2. 数据结构：环形缓冲区 vs 时间桶\n");
    printf("3. 算法优化：O(n) -> O(1) 复杂度优化\n");
    printf("4. 系统设计：监控、统计、告警\n");
    printf("5. 并发安全：多线程环境下的数据保护\n");
    printf("6. 性能考虑：内存使用 vs 时间复杂度权衡\n");
    printf("7. 实际应用：服务监控、限流、熔断\n");
    
    printf("\n=== 扩展思考 ===\n");
    printf("1. 如何处理时钟回拨问题？\n");
    printf("2. 如何实现动态调整阈值？\n");
    printf("3. 如何支持多种错误类型的监控？\n");
    printf("4. 如何实现分布式错误率监控？\n");
    printf("5. 如何优化内存使用？\n");
    printf("6. 如何实现错误率预测？\n");
    
    return 0;
}

/**
 * @brief 编译和运行说明
 * 
 * 编译：
 * gcc -o error_rate_monitor error_rate_monitor.c -lpthread -g
 * 
 * 运行：
 * ./error_rate_monitor
 * 
 * 调试：
 * gdb ./error_rate_monitor
 * 
 * 内存检查：
 * valgrind --leak-check=full ./error_rate_monitor
 * 
 * 这个实现涵盖了：
 * 1. 三种不同复杂度的算法实现
 * 2. 完整的系统监控框架
 * 3. 多线程安全性考虑
 * 4. 性能测试和优化
 * 5. 边界条件处理
 * 6. 实际应用场景分析
 * 
 * 面试重点：
 * - 理解滑动窗口算法
 * - 掌握时间序列数据处理
 * - 能够进行复杂度分析和优化
 * - 考虑系统设计的各个方面
 * - 注重代码质量和测试覆盖
 * 
 * 实际应用：
 * - 服务监控和告警系统
 * - API限流和熔断机制
 * - 系统健康检查
 * - 质量监控系统
 * - 实时数据分析
 */
