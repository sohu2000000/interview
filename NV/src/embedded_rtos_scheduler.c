/*
 * ================================================================================
 * NVIDIA 面试题：嵌入式RTOS调度器实现 (Embedded RTOS Scheduler Implementation)
 * ================================================================================
 * 
 * 题目描述：
 * 设计并实现一个简化的嵌入式实时操作系统(RTOS)任务调度器。
 * 
 * 要求：
 * 1. 支持多个任务的创建和管理
 * 2. 实现基于优先级的抢占式调度
 * 3. 支持任务状态管理（就绪、运行、阻塞、挂起）
 * 4. 实现时间片轮转调度（同优先级任务）
 * 5. 支持任务同步原语（信号量、互斥锁）
 * 6. 内存占用小，适合嵌入式环境
 * 7. 支持中断处理和任务切换
 * 
 * 考察点：
 * - 操作系统原理理解
 * - 数据结构设计能力
 * - 实时系统概念
 * - 中断和上下文切换
 * - 内存管理
 * - 同步机制
 * - 嵌入式系统约束
 * 
 * 实现特性：
 * - 优先级调度（0-31，数字越大优先级越高）
 * - 时间片轮转（同优先级任务）
 * - 任务状态机
 * - 简单的同步原语
 * - 系统调用接口
 * ================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* ================================================================================
 * 系统配置和常量定义
 * ================================================================================ */

#define MAX_TASKS           16      // 最大任务数量
#define MAX_PRIORITY        31      // 最高优先级
#define DEFAULT_TIME_SLICE  10      // 默认时间片（毫秒）
#define STACK_SIZE          1024    // 任务栈大小
#define IDLE_TASK_PRIORITY  0       // 空闲任务优先级

/* 任务状态枚举 */
typedef enum {
    TASK_STATE_READY = 0,      // 就绪状态
    TASK_STATE_RUNNING,        // 运行状态
    TASK_STATE_BLOCKED,        // 阻塞状态
    TASK_STATE_SUSPENDED,      // 挂起状态
    TASK_STATE_TERMINATED      // 终止状态
} task_state_t;

/* 调度算法类型 */
typedef enum {
    SCHED_PRIORITY = 0,        // 优先级调度
    SCHED_ROUND_ROBIN,         // 时间片轮转
    SCHED_PREEMPTIVE          // 抢占式调度
} sched_policy_t;

/* ================================================================================
 * 数据结构定义
 * ================================================================================ */

/* 任务控制块 (Task Control Block) */
typedef struct task_cb {
    uint32_t        task_id;           // 任务ID
    char            name[16];          // 任务名称
    uint32_t        priority;          // 任务优先级 (0-31)
    task_state_t    state;             // 任务状态
    
    /* 栈信息 */
    void*           stack_ptr;         // 栈指针
    void*           stack_base;        // 栈基址
    uint32_t        stack_size;        // 栈大小
    
    /* 调度信息 */
    uint32_t        time_slice;        // 时间片
    uint32_t        remaining_slice;   // 剩余时间片
    uint32_t        total_runtime;     // 总运行时间
    
    /* 任务函数 */
    void (*task_func)(void* param);    // 任务入口函数
    void*           param;             // 任务参数
    
    /* 链表指针 */
    struct task_cb* next;              // 下一个任务
    struct task_cb* prev;              // 前一个任务
    
    /* 同步对象 */
    void*           waiting_on;        // 等待的同步对象
    uint32_t        wait_timeout;      // 等待超时时间
    
} task_cb_t;

/* 就绪队列 - 每个优先级一个队列 */
typedef struct {
    task_cb_t*      head;              // 队列头
    task_cb_t*      tail;              // 队列尾
    uint32_t        count;             // 队列中任务数量
} ready_queue_t;

/* 调度器控制块 */
typedef struct {
    task_cb_t*      current_task;      // 当前运行任务
    task_cb_t*      idle_task;         // 空闲任务
    ready_queue_t   ready_queues[MAX_PRIORITY + 1]; // 就绪队列数组
    
    uint32_t        ready_bitmap;      // 就绪位图 (优先级位图)
    uint32_t        task_count;        // 总任务数
    uint32_t        system_tick;       // 系统时钟计数
    
    bool            scheduler_running; // 调度器运行状态
    bool            need_reschedule;   // 需要重新调度标志
    
    sched_policy_t  policy;            // 调度策略
    
} scheduler_t;

/* 信号量结构 */
typedef struct {
    int32_t         count;             // 信号量计数
    task_cb_t*      wait_list;         // 等待队列
    char            name[16];          // 信号量名称
} semaphore_t;

/* ================================================================================
 * 全局变量
 * ================================================================================ */

static scheduler_t g_scheduler;
static task_cb_t g_task_pool[MAX_TASKS];
static bool g_task_pool_used[MAX_TASKS];

/* ================================================================================
 * 位操作工具函数
 * ================================================================================ */

/* 查找最高优先级（最高位） */
static inline uint32_t find_highest_priority(uint32_t bitmap) {
    if (bitmap == 0) return 0;
    
    uint32_t priority = 0;
    if (bitmap & 0xFFFF0000) { priority += 16; bitmap >>= 16; }
    if (bitmap & 0xFF00)     { priority += 8;  bitmap >>= 8;  }
    if (bitmap & 0xF0)       { priority += 4;  bitmap >>= 4;  }
    if (bitmap & 0xC)        { priority += 2;  bitmap >>= 2;  }
    if (bitmap & 0x2)        { priority += 1; }
    
    return priority;
}

/* 设置优先级位 */
static inline void set_ready_bit(uint32_t priority) {
    g_scheduler.ready_bitmap |= (1U << priority);
}

/* 清除优先级位 */
static inline void clear_ready_bit(uint32_t priority) {
    if (g_scheduler.ready_queues[priority].count == 0) {
        g_scheduler.ready_bitmap &= ~(1U << priority);
    }
}

/* ================================================================================
 * 任务队列管理函数
 * ================================================================================ */

/* 将任务添加到就绪队列 */
static void add_to_ready_queue(task_cb_t* task) {
    if (!task) return;
    
    uint32_t priority = task->priority;
    ready_queue_t* queue = &g_scheduler.ready_queues[priority];
    
    task->next = NULL;
    task->prev = queue->tail;
    
    if (queue->tail) {
        queue->tail->next = task;
    } else {
        queue->head = task;
    }
    
    queue->tail = task;
    queue->count++;
    
    set_ready_bit(priority);
    task->state = TASK_STATE_READY;
}

/* 从就绪队列移除任务 */
static void remove_from_ready_queue(task_cb_t* task) {
    if (!task) return;
    
    uint32_t priority = task->priority;
    ready_queue_t* queue = &g_scheduler.ready_queues[priority];
    
    if (task->prev) {
        task->prev->next = task->next;
    } else {
        queue->head = task->next;
    }
    
    if (task->next) {
        task->next->prev = task->prev;
    } else {
        queue->tail = task->prev;
    }
    
    queue->count--;
    clear_ready_bit(priority);
    
    task->next = task->prev = NULL;
}

/* 获取最高优先级的就绪任务 */
static task_cb_t* get_highest_priority_task(void) {
    if (g_scheduler.ready_bitmap == 0) {
        return g_scheduler.idle_task;
    }
    
    uint32_t highest_priority = find_highest_priority(g_scheduler.ready_bitmap);
    ready_queue_t* queue = &g_scheduler.ready_queues[highest_priority];
    
    return queue->head;
}

/* ================================================================================
 * 任务管理函数
 * ================================================================================ */

/* 分配任务控制块 */
static task_cb_t* allocate_task_cb(void) {
    for (int i = 0; i < MAX_TASKS; i++) {
        if (!g_task_pool_used[i]) {
            g_task_pool_used[i] = true;
            memset(&g_task_pool[i], 0, sizeof(task_cb_t));
            g_task_pool[i].task_id = i;
            return &g_task_pool[i];
        }
    }
    return NULL;
}

/* 释放任务控制块 */
static void free_task_cb(task_cb_t* task) {
    if (task && task->task_id < MAX_TASKS) {
        g_task_pool_used[task->task_id] = false;
        
        /* 释放栈内存 */
        if (task->stack_base) {
            free(task->stack_base);
            task->stack_base = NULL;
        }
    }
}

/* 创建任务 */
task_cb_t* task_create(const char* name, 
                      void (*task_func)(void*), 
                      void* param,
                      uint32_t priority, 
                      uint32_t stack_size) {
    
    if (!task_func || priority > MAX_PRIORITY) {
        return NULL;
    }
    
    task_cb_t* task = allocate_task_cb();
    if (!task) {
        return NULL;
    }
    
    /* 初始化任务控制块 */
    strncpy(task->name, name ? name : "unnamed", sizeof(task->name) - 1);
    task->priority = priority;
    task->state = TASK_STATE_SUSPENDED;
    task->task_func = task_func;
    task->param = param;
    task->time_slice = DEFAULT_TIME_SLICE;
    task->remaining_slice = DEFAULT_TIME_SLICE;
    
    /* 分配栈空间 */
    task->stack_size = stack_size ? stack_size : STACK_SIZE;
    task->stack_base = malloc(task->stack_size);
    if (!task->stack_base) {
        free_task_cb(task);
        return NULL;
    }
    
    task->stack_ptr = (char*)task->stack_base + task->stack_size - sizeof(uint32_t);
    
    g_scheduler.task_count++;
    
    printf("任务创建成功: %s (ID: %d, 优先级: %d)\n", 
           task->name, task->task_id, task->priority);
    
    return task;
}

/* 启动任务 */
void task_start(task_cb_t* task) {
    if (!task || task->state != TASK_STATE_SUSPENDED) {
        return;
    }
    
    add_to_ready_queue(task);
    g_scheduler.need_reschedule = true;
    
    printf("任务启动: %s\n", task->name);
}

/* 终止任务 */
void task_terminate(task_cb_t* task) {
    if (!task) return;
    
    if (task->state == TASK_STATE_READY) {
        remove_from_ready_queue(task);
    }
    
    task->state = TASK_STATE_TERMINATED;
    
    if (task == g_scheduler.current_task) {
        g_scheduler.need_reschedule = true;
    }
    
    printf("任务终止: %s\n", task->name);
}

/* 任务主动让出CPU */
void task_yield(void) {
    g_scheduler.need_reschedule = true;
}

/* 任务延时 */
void task_delay(uint32_t ticks) {
    if (g_scheduler.current_task) {
        g_scheduler.current_task->state = TASK_STATE_BLOCKED;
        g_scheduler.current_task->wait_timeout = g_scheduler.system_tick + ticks;
        g_scheduler.need_reschedule = true;
    }
}

/* ================================================================================
 * 调度器核心函数
 * ================================================================================ */

/* 任务切换 */
static void context_switch(task_cb_t* from, task_cb_t* to) {
    if (from == to) return;
    
    /* 保存当前任务上下文 */
    if (from && from->state == TASK_STATE_RUNNING) {
        from->state = TASK_STATE_READY;
        /* 在实际系统中，这里会保存CPU寄存器到任务栈 */
    }
    
    /* 恢复目标任务上下文 */
    if (to) {
        to->state = TASK_STATE_RUNNING;
        g_scheduler.current_task = to;
        /* 在实际系统中，这里会从任务栈恢复CPU寄存器 */
        
        printf("任务切换: %s -> %s\n", 
               from ? from->name : "NULL", to->name);
    }
}

/* 调度器主函数 */
void schedule(void) {
    if (!g_scheduler.scheduler_running) {
        return;
    }
    
    task_cb_t* current = g_scheduler.current_task;
    task_cb_t* next = get_highest_priority_task();
    
    /* 检查是否需要抢占 */
    bool need_switch = false;
    
    if (!current) {
        need_switch = true;
    } else if (current->state != TASK_STATE_RUNNING) {
        need_switch = true;
    } else if (next && next->priority > current->priority) {
        /* 高优先级任务抢占 */
        need_switch = true;
        if (current->state == TASK_STATE_RUNNING) {
            add_to_ready_queue(current);
        }
    } else if (next && next->priority == current->priority && 
               current->remaining_slice == 0) {
        /* 同优先级时间片轮转 */
        need_switch = true;
        add_to_ready_queue(current);
        current->remaining_slice = current->time_slice;
    }
    
    if (need_switch && next) {
        if (next->state == TASK_STATE_READY) {
            remove_from_ready_queue(next);
        }
        context_switch(current, next);
    }
    
    g_scheduler.need_reschedule = false;
}

/* 系统时钟中断处理 */
void system_tick_handler(void) {
    g_scheduler.system_tick++;
    
    /* 更新当前任务时间片 */
    if (g_scheduler.current_task && 
        g_scheduler.current_task->state == TASK_STATE_RUNNING) {
        
        g_scheduler.current_task->total_runtime++;
        
        if (g_scheduler.current_task->remaining_slice > 0) {
            g_scheduler.current_task->remaining_slice--;
        }
        
        if (g_scheduler.current_task->remaining_slice == 0) {
            g_scheduler.need_reschedule = true;
        }
    }
    
    /* 检查阻塞任务超时 */
    for (int i = 0; i < MAX_TASKS; i++) {
        if (g_task_pool_used[i] && 
            g_task_pool[i].state == TASK_STATE_BLOCKED &&
            g_task_pool[i].wait_timeout <= g_scheduler.system_tick) {
            
            add_to_ready_queue(&g_task_pool[i]);
            g_scheduler.need_reschedule = true;
        }
    }
    
    /* 如果需要重新调度 */
    if (g_scheduler.need_reschedule) {
        schedule();
    }
}

/* ================================================================================
 * 同步原语实现
 * ================================================================================ */

/* 创建信号量 */
semaphore_t* semaphore_create(int32_t initial_count, const char* name) {
    semaphore_t* sem = malloc(sizeof(semaphore_t));
    if (!sem) return NULL;
    
    sem->count = initial_count;
    sem->wait_list = NULL;
    strncpy(sem->name, name ? name : "unnamed", sizeof(sem->name) - 1);
    
    return sem;
}

/* 获取信号量 */
bool semaphore_wait(semaphore_t* sem, uint32_t timeout) {
    if (!sem) return false;
    
    if (sem->count > 0) {
        sem->count--;
        return true;
    }
    
    /* 需要阻塞等待 */
    if (g_scheduler.current_task) {
        g_scheduler.current_task->state = TASK_STATE_BLOCKED;
        g_scheduler.current_task->waiting_on = sem;
        g_scheduler.current_task->wait_timeout = 
            timeout ? g_scheduler.system_tick + timeout : UINT32_MAX;
        
        g_scheduler.need_reschedule = true;
        schedule();
    }
    
    return false;
}

/* 释放信号量 */
void semaphore_signal(semaphore_t* sem) {
    if (!sem) return;
    
    sem->count++;
    
    /* 唤醒等待的任务 */
    for (int i = 0; i < MAX_TASKS; i++) {
        if (g_task_pool_used[i] && 
            g_task_pool[i].state == TASK_STATE_BLOCKED &&
            g_task_pool[i].waiting_on == sem) {
            
            g_task_pool[i].waiting_on = NULL;
            add_to_ready_queue(&g_task_pool[i]);
            g_scheduler.need_reschedule = true;
            break;  // 只唤醒一个任务
        }
    }
}

/* ================================================================================
 * 调度器初始化和管理
 * ================================================================================ */

/* 空闲任务函数 */
static void idle_task_func(void* param) {
    while (1) {
        /* 空闲任务：可以进入低功耗模式 */
        printf("空闲任务运行...\n");
        
        /* 模拟空闲处理 */
        for (volatile int i = 0; i < 1000000; i++);
    }
}

/* 初始化调度器 */
void scheduler_init(void) {
    memset(&g_scheduler, 0, sizeof(g_scheduler));
    memset(g_task_pool_used, 0, sizeof(g_task_pool_used));
    
    /* 初始化就绪队列 */
    for (int i = 0; i <= MAX_PRIORITY; i++) {
        g_scheduler.ready_queues[i].head = NULL;
        g_scheduler.ready_queues[i].tail = NULL;
        g_scheduler.ready_queues[i].count = 0;
    }
    
    g_scheduler.policy = SCHED_PREEMPTIVE;
    g_scheduler.scheduler_running = false;
    
    /* 创建空闲任务 */
    g_scheduler.idle_task = task_create("idle", idle_task_func, NULL, 
                                       IDLE_TASK_PRIORITY, STACK_SIZE);
    if (g_scheduler.idle_task) {
        task_start(g_scheduler.idle_task);
    }
    
    printf("RTOS调度器初始化完成\n");
}

/* 启动调度器 */
void scheduler_start(void) {
    g_scheduler.scheduler_running = true;
    g_scheduler.system_tick = 0;
    
    printf("RTOS调度器启动\n");
    
    /* 开始第一次调度 */
    schedule();
}

/* 停止调度器 */
void scheduler_stop(void) {
    g_scheduler.scheduler_running = false;
    printf("RTOS调度器停止\n");
}

/* 获取系统信息 */
void print_system_info(void) {
    printf("\n=== RTOS系统信息 ===\n");
    printf("系统时钟: %u\n", g_scheduler.system_tick);
    printf("任务总数: %u\n", g_scheduler.task_count);
    printf("当前任务: %s\n", 
           g_scheduler.current_task ? g_scheduler.current_task->name : "NULL");
    printf("就绪位图: 0x%08X\n", g_scheduler.ready_bitmap);
    
    printf("\n任务列表:\n");
    printf("ID  名称     优先级  状态    运行时间\n");
    printf("--- -------- ------- ------- --------\n");
    
    for (int i = 0; i < MAX_TASKS; i++) {
        if (g_task_pool_used[i]) {
            task_cb_t* task = &g_task_pool[i];
            const char* state_str[] = {
                "READY", "RUNNING", "BLOCKED", "SUSPENDED", "TERMINATED"
            };
            
            printf("%2d  %-8s %7d %-7s %8d\n",
                   task->task_id, task->name, task->priority,
                   state_str[task->state], task->total_runtime);
        }
    }
    printf("\n");
}

/* ================================================================================
 * 测试任务函数
 * ================================================================================ */

void test_task_high(void* param) {
    int count = 0;
    while (count < 5) {
        printf("高优先级任务运行 (计数: %d)\n", count++);
        task_delay(20);  // 延时20个时钟周期
    }
    printf("高优先级任务完成\n");
}

void test_task_medium(void* param) {
    int count = 0;
    while (count < 8) {
        printf("中优先级任务运行 (计数: %d)\n", count++);
        if (count == 4) {
            printf("中优先级任务主动让出CPU\n");
            task_yield();
        }
        task_delay(15);
    }
    printf("中优先级任务完成\n");
}

void test_task_low(void* param) {
    int count = 0;
    while (count < 10) {
        printf("低优先级任务运行 (计数: %d)\n", count++);
        task_delay(10);
    }
    printf("低优先级任务完成\n");
}

/* ================================================================================
 * 主测试函数
 * ================================================================================ */

int main() {
    printf("NVIDIA 面试题：嵌入式RTOS调度器实现\n");
    printf("=====================================\n\n");
    
    /* 初始化调度器 */
    scheduler_init();
    
    /* 创建测试任务 */
    task_cb_t* task_high = task_create("TaskHigh", test_task_high, NULL, 20, 0);
    task_cb_t* task_med = task_create("TaskMed", test_task_medium, NULL, 15, 0);
    task_cb_t* task_low = task_create("TaskLow", test_task_low, NULL, 10, 0);
    
    /* 创建同步测试 */
    semaphore_t* test_sem = semaphore_create(1, "TestSem");
    
    /* 启动任务 */
    if (task_low) task_start(task_low);
    if (task_med) task_start(task_med);
    if (task_high) task_start(task_high);
    
    /* 启动调度器 */
    scheduler_start();
    
    /* 模拟系统运行 */
    for (int i = 0; i < 200; i++) {
        system_tick_handler();  // 模拟时钟中断
        
        /* 每50个时钟周期打印一次系统信息 */
        if (i % 50 == 0) {
            print_system_info();
        }
        
        /* 模拟一些延时 */
        for (volatile int j = 0; j < 100000; j++);
    }
    
    /* 停止调度器 */
    scheduler_stop();
    
    /* 清理资源 */
    if (test_sem) free(test_sem);
    
    return 0;
}

/*
 * ================================================================================
 * 面试要点总结：
 * 
 * 1. RTOS核心概念：
 *    - 实时性要求：确定性响应时间
 *    - 抢占式调度：高优先级任务可以打断低优先级任务
 *    - 任务状态管理：就绪、运行、阻塞、挂起等状态转换
 *    - 同步机制：信号量、互斥锁、消息队列等
 * 
 * 2. 调度算法：
 *    - 优先级调度：基于任务优先级进行调度
 *    - 时间片轮转：同优先级任务的公平调度
 *    - 抢占式调度：支持任务抢占和上下文切换
 * 
 * 3. 数据结构设计：
 *    - 任务控制块(TCB)：保存任务所有信息
 *    - 就绪队列：按优先级组织的任务队列
 *    - 位图优化：快速查找最高优先级任务
 * 
 * 4. 系统调用接口：
 *    - 任务管理：创建、启动、终止、延时、让出
 *    - 同步原语：信号量等待、释放
 *    - 系统信息：状态查询、调试信息
 * 
 * 5. 嵌入式特性：
 *    - 内存受限：静态内存分配，避免动态分配
 *    - 实时性：中断处理、快速上下文切换
 *    - 低功耗：空闲任务可进入低功耗模式
 * 
 * 6. 可能的扩展讨论：
 *    - 如何实现任务间通信？
 *    - 如何处理优先级反转问题？
 *    - 如何支持软实时和硬实时？
 *    - 如何在多核系统中扩展？
 *    - 如何进行系统调试和性能分析？
 * 
 * 7. 实际应用考虑：
 *    - 中断嵌套处理
 *    - 栈溢出检测
 *    - 内存保护
 *    - 功耗管理
 *    - 错误处理和恢复
 * ================================================================================
 */
