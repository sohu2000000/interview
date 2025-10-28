#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_SIZE 1000
#define MAX_VALUE 10000
#define MAX_FREQ 1000

/**
 * 通用栈结构
 */
typedef struct {
    int data[MAX_SIZE];
    int top;
} Stack;

/**
 * 频率优先栈实现 - LeetCode官方多栈方案 (最优解)
 * 
 * 核心思想：
 * 1. frequency[]: 哈希表记录每个值的当前频率
 * 2. freq_stacks[]: 数组of栈，freq_stacks[i]存储频率为i的所有元素
 * 3. max_freq: 当前最大频率，用于快速定位最高频率的栈
 * 
 * 关键洞察：
 * - 每个元素在多个频率栈中都有副本
 * - 频率为k的元素同时存在于freq_stacks[1], freq_stacks[2], ..., freq_stacks[k]中
 * - Pop时只需从freq_stacks[max_freq]中取栈顶元素，天然保证LIFO
 * 
 * 时间复杂度：Push O(1), Pop O(1)
 * 空间复杂度：O(N×F) 其中N是元素数量，F是最大频率
 */
typedef struct {
    Stack freq_stacks[MAX_FREQ + 1];  // freq_stacks[i] = 频率为i的元素栈
    int frequency[MAX_VALUE];          // frequency[value] = 该值的当前频率
    int max_freq;                      // 当前最大频率
} FreqStack;

/**
 * 栈的基本操作
 */
void stack_init(Stack* s) {
    s->top = -1;
}

bool stack_empty(Stack* s) {
    return s->top == -1;
}

bool stack_full(Stack* s) {
    return s->top == MAX_SIZE - 1;
}

void stack_push(Stack* s, int value) {
    if (!stack_full(s)) {
        s->data[++s->top] = value;
    }
}

int stack_pop(Stack* s) {
    if (!stack_empty(s)) {
        return s->data[s->top--];
    }
    return -1;
}

int stack_top(Stack* s) {
    if (!stack_empty(s)) {
        return s->data[s->top];
    }
    return -1;
}

/**
 * 创建频率栈
 */
FreqStack* freq_stack_create() {
    FreqStack* stack = (FreqStack*)malloc(sizeof(FreqStack));
    
    // 初始化所有频率栈
    for (int i = 0; i <= MAX_FREQ; i++) {
        stack_init(&stack->freq_stacks[i]);
    }
    
    // 初始化频率表
    memset(stack->frequency, 0, sizeof(stack->frequency));
    
    // 初始化最大频率
    stack->max_freq = 0;
    
    return stack;
}

/**
 * 释放频率栈
 */
void freq_stack_free(FreqStack* stack) {
    free(stack);
}

/**
 * 入栈操作 - 多栈方案 O(1)
 * @param stack: 频率栈指针
 * @param value: 要入栈的值
 * 
 * 算法步骤：
 * 1. 增加该值的频率计数
 * 2. 更新最大频率（如果需要）
 * 3. 将元素压入对应频率的栈中
 * 
 * 关键理解：
 * 元素会被放入其"新频率"对应的栈中
 * 例如：第一次push(5)时放入freq_stacks[1]
 *       第二次push(5)时放入freq_stacks[2]
 *       此时freq_stacks[1]和freq_stacks[2]都包含5
 */
void freq_stack_push(FreqStack* stack, int value) {
    // 1. 增加该值的频率
    stack->frequency[value]++;
    int freq = stack->frequency[value];
    
    // 2. 更新最大频率
    if (freq > stack->max_freq) {
        stack->max_freq = freq;
    }
    
    // 3. 将元素压入对应频率的栈中
    stack_push(&stack->freq_stacks[freq], value);
    
    printf("Push %d (频率: %d)\n", value, freq);
}

/**
 * 出栈操作 - 多栈方案 O(1)
 * @param stack: 频率栈指针
 * @return: 出栈的值，如果栈为空返回-1
 * 
 * 算法步骤：
 * 1. 从最高频率的栈中弹出栈顶元素
 * 2. 减少该元素的频率计数
 * 3. 如果最高频率的栈变空，则减少max_freq
 * 
 * 关键理解：
 * - 最高频率栈的栈顶元素就是答案（天然满足LIFO）
 * - 不需要搜索，直接O(1)获取
 * - 元素在低频率栈中的副本保持不变，这很关键！
 * 
 * 例子：push(5), push(5), push(7), push(5)
 * freq_stacks[1]: [5, 7]  
 * freq_stacks[2]: [5]
 * freq_stacks[3]: [5]
 * 
 * pop() -> 从freq_stacks[3]弹出5, 5的频率变成2
 * pop() -> 从freq_stacks[2]弹出5, 5的频率变成1  
 * pop() -> 从freq_stacks[1]弹出7, 7的频率变成0
 * pop() -> 从freq_stacks[1]弹出5, 5的频率变成0
 */
int freq_stack_pop(FreqStack* stack) {
    if (stack->max_freq == 0) {
        printf("栈为空！\n");
        return -1;
    }
    
    // 1. 从最高频率的栈中弹出栈顶元素
    int value = stack_pop(&stack->freq_stacks[stack->max_freq]);
    
    // 2. 减少该元素的频率计数
    stack->frequency[value]--;
    
    // 3. 如果最高频率的栈变空，则减少max_freq
    if (stack_empty(&stack->freq_stacks[stack->max_freq])) {
        stack->max_freq--;
    }
    
    printf("Pop %d (剩余频率: %d)\n", value, stack->frequency[value]);
    return value;
}

/**
 * 检查栈是否为空
 */
bool freq_stack_empty(FreqStack* stack) {
    return stack->max_freq == 0;
}

/**
 * 打印栈的状态（调试用）- 多栈方案
 */
void freq_stack_print_status(FreqStack* stack) {
    printf("\n=== 多栈状态 ===\n");
    printf("最大频率: %d\n", stack->max_freq);
    
    if (stack->max_freq == 0) {
        printf("栈为空\n");
    } else {
        // 显示每个频率级别的栈
        for (int freq = 1; freq <= stack->max_freq; freq++) {
            if (!stack_empty(&stack->freq_stacks[freq])) {
                printf("频率%d的栈: [", freq);
                for (int i = 0; i <= stack->freq_stacks[freq].top; i++) {
                    printf("%d", stack->freq_stacks[freq].data[i]);
                    if (i < stack->freq_stacks[freq].top) printf(", ");
                }
                printf("]\n");
            }
        }
        
        // 显示频率统计
        printf("\n当前频率统计:\n");
        bool found_any = false;
        for (int val = 0; val < MAX_VALUE; val++) {
            if (stack->frequency[val] > 0) {
                printf("  值%d: 频率%d\n", val, stack->frequency[val]);
                found_any = true;
            }
        }
        if (!found_any) {
            printf("  无元素\n");
        }
    }
    printf("================\n\n");
}

/**
 * 测试函数
 */
void test_frequency_stack() {
    printf("=== 频率优先栈测试 ===\n\n");
    
    FreqStack* stack = freq_stack_create();
    
    // 按题目要求的操作序列
    printf("执行操作序列: Push(2), Push(1), Push(3), Push(1), Push(4)\n\n");
    
    freq_stack_push(stack, 2);
    freq_stack_push(stack, 1);
    freq_stack_push(stack, 3);
    freq_stack_push(stack, 1);  // 1的频率变成2
    freq_stack_push(stack, 4);
    
    freq_stack_print_status(stack);
    
    printf("开始Pop操作:\n");
    printf("期望顺序: (1, 4, 3, 1, 2)\n");
    printf("实际顺序: (");
    
    // 执行pop操作
    bool first = true;
    while (!freq_stack_empty(stack)) {
        if (!first) printf(", ");
        int value = freq_stack_pop(stack);
        printf("%d", value);
        first = false;
    }
    printf(")\n\n");
    
    freq_stack_free(stack);
}

/**
 * 额外测试用例
 */
void test_additional_cases() {
    printf("=== 额外测试用例 ===\n\n");
    
    FreqStack* stack = freq_stack_create();
    
    // 测试用例：多个元素有相同频率
    printf("测试用例: Push(5), Push(7), Push(5), Push(7), Push(4), Push(5)\n\n");
    
    freq_stack_push(stack, 5);  // 5: freq=1
    freq_stack_push(stack, 7);  // 7: freq=1  
    freq_stack_push(stack, 5);  // 5: freq=2
    freq_stack_push(stack, 7);  // 7: freq=2
    freq_stack_push(stack, 4);  // 4: freq=1
    freq_stack_push(stack, 5);  // 5: freq=3
    
    freq_stack_print_status(stack);
    
    printf("Pop顺序: ");
    printf("(");
    bool first = true;
    while (!freq_stack_empty(stack)) {
        if (!first) printf(", ");
        int value = freq_stack_pop(stack);
        printf("%d", value);
        first = false;
    }
    printf(")\n\n");
    
    freq_stack_free(stack);
}

/**
 * 主函数
 */
int main() {
    test_frequency_stack();
    test_additional_cases();
    return 0;
}
