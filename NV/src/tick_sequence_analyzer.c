/**
 * @file tick_sequence_analyzer.c
 * @brief Tick序列分析器 - 检测连续1的模式
 * 
 * 题目：给定一个tick序列（0和1的序列），判断是否存在连续10次的1。
 * 返回true表示存在，false表示不存在。
 * 
 * 这是一个经典的模式匹配和状态机设计面试题，考察：
 * 1. 字符串/序列模式匹配算法
 * 2. 状态机设计和实现
 * 3. 滑动窗口技术
 * 4. 位操作和优化技巧
 * 5. 流式数据处理
 * 6. 算法复杂度分析
 * 
 * @author feliu@nvidia.com
 * @date 2025-10-23
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <time.h>

// 配置常量
#define TARGET_CONSECUTIVE_ONES 10    // 目标连续1的个数
#define MAX_SEQUENCE_LENGTH 10000     // 最大序列长度
#define BUFFER_SIZE 1024              // 流式处理缓冲区大小

/**
 * @brief Tick序列分析器结构体
 */
typedef struct TickAnalyzer {
    int target_count;           // 目标连续1的个数
    int current_count;          // 当前连续1的个数
    bool found;                 // 是否已找到目标模式
    int total_ticks;           // 总tick数
    int total_ones;            // 总1的个数
    int max_consecutive;       // 遇到的最大连续1个数
    int sequences_found;       // 找到的目标序列个数
} TickAnalyzer;

/**
 * @brief 方法1：简单遍历法
 * 
 * 算法思路：
 * 1. 遍历整个序列
 * 2. 遇到1时递增计数器，遇到0时重置计数器
 * 3. 当计数器达到目标值时返回true
 * 
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 * 
 * @param sequence tick序列
 * @param length 序列长度
 * @param target_count 目标连续1的个数
 * @return true表示找到目标模式，false表示未找到
 */
bool has_consecutive_ones_simple(const int *sequence, int length, int target_count) {
    printf("\n=== 方法1：简单遍历法 ===\n");
    printf("算法：遍历序列，维护连续1的计数器\n");
    printf("时间复杂度：O(n), 空间复杂度：O(1)\n");
    
    if (!sequence || length <= 0 || target_count <= 0) {
        printf("参数无效\n");
        return false;
    }
    
    int consecutive_count = 0;
    int max_consecutive = 0;
    
    printf("开始遍历序列（长度=%d，目标=%d个连续1）：\n", length, target_count);
    
    for (int i = 0; i < length; i++) {
        if (sequence[i] == 1) {
            consecutive_count++;
            printf("位置%d: 1, 当前连续计数=%d\n", i, consecutive_count);
            
            // 更新最大连续计数
            if (consecutive_count > max_consecutive) {
                max_consecutive = consecutive_count;
            }
            
            // 检查是否达到目标
            if (consecutive_count >= target_count) {
                printf("找到%d个连续的1！位置范围：[%d, %d]\n", 
                       target_count, i - target_count + 1, i);
                printf("最大连续1个数：%d\n", max_consecutive);
                return true;
            }
        } else {
            if (consecutive_count > 0) {
                printf("位置%d: 0, 连续1序列中断，重置计数器\n", i);
            }
            consecutive_count = 0;
        }
    }
    
    printf("遍历完成，未找到%d个连续的1\n", target_count);
    printf("最大连续1个数：%d\n", max_consecutive);
    return false;
}

/**
 * @brief 方法2：状态机实现
 * 
 * 算法思路：
 * 1. 设计状态机，每个状态表示当前连续1的个数
 * 2. 状态0：刚遇到0或开始状态
 * 3. 状态1-9：连续1的个数为1-9
 * 4. 状态10：找到10个连续1，成功状态
 * 
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 * 
 * @param sequence tick序列
 * @param length 序列长度
 * @param target_count 目标连续1的个数
 * @return true表示找到目标模式，false表示未找到
 */
bool has_consecutive_ones_state_machine(const int *sequence, int length, int target_count) {
    printf("\n=== 方法2：状态机实现 ===\n");
    printf("算法：使用状态机跟踪连续1的个数\n");
    printf("时间复杂度：O(n), 空间复杂度：O(1)\n");
    
    if (!sequence || length <= 0 || target_count <= 0) {
        printf("参数无效\n");
        return false;
    }
    
    int state = 0;  // 当前状态，表示连续1的个数
    
    printf("状态机初始化：状态0（无连续1）\n");
    printf("目标状态：状态%d（%d个连续1）\n", target_count, target_count);
    
    for (int i = 0; i < length; i++) {
        if (sequence[i] == 1) {
            state++;
            printf("位置%d: 1, 状态转移：%d -> %d\n", i, state - 1, state);
            
            if (state >= target_count) {
                printf("达到目标状态%d！找到%d个连续的1\n", state, target_count);
                return true;
            }
        } else {
            if (state > 0) {
                printf("位置%d: 0, 状态重置：%d -> 0\n", i, state);
            }
            state = 0;
        }
    }
    
    printf("状态机结束，最终状态：%d，未达到目标状态%d\n", state, target_count);
    return false;
}

/**
 * @brief 方法3：滑动窗口法
 * 
 * 算法思路：
 * 1. 维护一个大小为target_count的滑动窗口
 * 2. 检查窗口内是否全部为1
 * 3. 窗口滑动，逐个检查
 * 
 * 时间复杂度：O(n*k) - k为目标长度，可优化为O(n)
 * 空间复杂度：O(1)
 * 
 * @param sequence tick序列
 * @param length 序列长度
 * @param target_count 目标连续1的个数
 * @return true表示找到目标模式，false表示未找到
 */
bool has_consecutive_ones_sliding_window(const int *sequence, int length, int target_count) {
    printf("\n=== 方法3：滑动窗口法 ===\n");
    printf("算法：使用大小为%d的滑动窗口检查\n", target_count);
    printf("时间复杂度：O(n*k), 空间复杂度：O(1)\n");
    
    if (!sequence || length < target_count || target_count <= 0) {
        printf("参数无效或序列太短\n");
        return false;
    }
    
    // 遍历所有可能的窗口位置
    for (int i = 0; i <= length - target_count; i++) {
        bool all_ones = true;
        
        printf("检查窗口[%d, %d]: ", i, i + target_count - 1);
        
        // 检查当前窗口内是否全为1
        for (int j = 0; j < target_count; j++) {
            printf("%d", sequence[i + j]);
            if (sequence[i + j] != 1) {
                all_ones = false;
            }
        }
        
        printf(" - %s\n", all_ones ? "全为1！" : "不全为1");
        
        if (all_ones) {
            printf("找到%d个连续的1！位置范围：[%d, %d]\n", 
                   target_count, i, i + target_count - 1);
            return true;
        }
    }
    
    printf("所有窗口检查完毕，未找到%d个连续的1\n", target_count);
    return false;
}

/**
 * @brief 方法4：优化滑动窗口法
 * 
 * 算法思路：
 * 1. 维护窗口内1的个数
 * 2. 窗口滑动时，只需要更新边界元素
 * 3. 避免重复计算窗口内的和
 * 
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 * 
 * @param sequence tick序列
 * @param length 序列长度
 * @param target_count 目标连续1的个数
 * @return true表示找到目标模式，false表示未找到
 */
bool has_consecutive_ones_optimized_window(const int *sequence, int length, int target_count) {
    printf("\n=== 方法4：优化滑动窗口法 ===\n");
    printf("算法：维护窗口内1的个数，避免重复计算\n");
    printf("时间复杂度：O(n), 空间复杂度：O(1)\n");
    
    if (!sequence || length < target_count || target_count <= 0) {
        printf("参数无效或序列太短\n");
        return false;
    }
    
    // 初始化第一个窗口
    int window_ones = 0;
    for (int i = 0; i < target_count; i++) {
        window_ones += sequence[i];
    }
    
    printf("初始窗口[0, %d]内1的个数：%d\n", target_count - 1, window_ones);
    
    if (window_ones == target_count) {
        printf("初始窗口就找到%d个连续的1！\n", target_count);
        return true;
    }
    
    // 滑动窗口
    for (int i = target_count; i < length; i++) {
        // 移除窗口左边的元素，添加窗口右边的新元素
        window_ones = window_ones - sequence[i - target_count] + sequence[i];
        
        printf("窗口[%d, %d]内1的个数：%d\n", 
               i - target_count + 1, i, window_ones);
        
        if (window_ones == target_count) {
            printf("找到%d个连续的1！位置范围：[%d, %d]\n", 
                   target_count, i - target_count + 1, i);
            return true;
        }
    }
    
    printf("所有窗口检查完毕，未找到%d个连续的1\n", target_count);
    return false;
}

/**
 * @brief 方法5：位操作优化法（适用于小目标值）
 * 
 * 算法思路：
 * 1. 使用位移操作维护最近target_count个bit的状态
 * 2. 每次新的bit到来时，左移并添加新bit
 * 3. 检查是否等于全1的模式
 * 
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 * 
 * @param sequence tick序列
 * @param length 序列长度
 * @param target_count 目标连续1的个数（必须 <= 32）
 * @return true表示找到目标模式，false表示未找到
 */
bool has_consecutive_ones_bit_ops(const int *sequence, int length, int target_count) {
    printf("\n=== 方法5：位操作优化法 ===\n");
    printf("算法：使用位操作维护最近%d个bit的状态\n", target_count);
    printf("时间复杂度：O(n), 空间复杂度：O(1)\n");
    
    if (!sequence || length <= 0 || target_count <= 0 || target_count > 32) {
        printf("参数无效或target_count > 32\n");
        return false;
    }
    
    uint32_t pattern = 0;                          // 当前bit模式
    uint32_t target_pattern = (1U << target_count) - 1;  // 目标模式（全1）
    uint32_t mask = target_pattern;                // 掩码，保留最低target_count位
    
    printf("目标模式：0x%X（%d个连续1）\n", target_pattern, target_count);
    printf("掩码：0x%X\n", mask);
    
    for (int i = 0; i < length; i++) {
        // 左移并添加新bit
        pattern = ((pattern << 1) | sequence[i]) & mask;
        
        printf("位置%d: %d, 当前模式：0x%X", i, sequence[i], pattern);
        
        if (pattern == target_pattern) {
            printf(" - 匹配目标模式！\n");
            printf("找到%d个连续的1！位置范围：[%d, %d]\n", 
                   target_count, i - target_count + 1, i);
            return true;
        } else {
            printf("\n");
        }
    }
    
    printf("遍历完成，未找到目标模式\n");
    return false;
}

/**
 * @brief 初始化tick分析器
 * @param analyzer 分析器指针
 * @param target_count 目标连续1的个数
 */
void init_tick_analyzer(TickAnalyzer *analyzer, int target_count) {
    analyzer->target_count = target_count;
    analyzer->current_count = 0;
    analyzer->found = false;
    analyzer->total_ticks = 0;
    analyzer->total_ones = 0;
    analyzer->max_consecutive = 0;
    analyzer->sequences_found = 0;
}

/**
 * @brief 流式处理单个tick
 * @param analyzer 分析器指针
 * @param tick 当前tick值（0或1）
 * @return true表示找到目标模式，false表示未找到
 */
bool process_tick(TickAnalyzer *analyzer, int tick) {
    analyzer->total_ticks++;
    
    if (tick == 1) {
        analyzer->total_ones++;
        analyzer->current_count++;
        
        // 更新最大连续计数
        if (analyzer->current_count > analyzer->max_consecutive) {
            analyzer->max_consecutive = analyzer->current_count;
        }
        
        // 检查是否达到目标
        if (analyzer->current_count >= analyzer->target_count && !analyzer->found) {
            analyzer->found = true;
            analyzer->sequences_found++;
            printf("流式处理：在位置%d找到%d个连续的1！\n", 
                   analyzer->total_ticks - 1, analyzer->target_count);
            return true;
        }
    } else {
        analyzer->current_count = 0;
    }
    
    return false;
}

/**
 * @brief 打印分析器统计信息
 * @param analyzer 分析器指针
 */
void print_analyzer_stats(const TickAnalyzer *analyzer) {
    printf("\n=== Tick分析器统计信息 ===\n");
    printf("总tick数：%d\n", analyzer->total_ticks);
    printf("总1的个数：%d\n", analyzer->total_ones);
    printf("1的比例：%.2f%%\n", 
           analyzer->total_ticks > 0 ? 
           (float)analyzer->total_ones / analyzer->total_ticks * 100 : 0);
    printf("最大连续1个数：%d\n", analyzer->max_consecutive);
    printf("找到的目标序列个数：%d\n", analyzer->sequences_found);
    printf("是否找到%d个连续1：%s\n", 
           analyzer->target_count, analyzer->found ? "是" : "否");
}

/**
 * @brief 生成随机tick序列
 * @param sequence 输出序列数组
 * @param length 序列长度
 * @param ones_probability 1出现的概率（0-100）
 */
void generate_random_sequence(int *sequence, int length, int ones_probability) {
    printf("生成长度为%d的随机序列，1的概率为%d%%\n", length, ones_probability);
    
    for (int i = 0; i < length; i++) {
        sequence[i] = (rand() % 100) < ones_probability ? 1 : 0;
    }
}

/**
 * @brief 生成包含目标模式的测试序列
 * @param sequence 输出序列数组
 * @param length 序列长度
 * @param target_count 目标连续1的个数
 * @param insert_position 插入位置
 */
void generate_test_sequence_with_pattern(int *sequence, int length, 
                                       int target_count, int insert_position) {
    printf("生成包含%d个连续1的测试序列，插入位置：%d\n", 
           target_count, insert_position);
    
    // 先填充随机0和1（避免连续1）
    for (int i = 0; i < length; i++) {
        if (i % 3 == 0) {
            sequence[i] = 1;
        } else {
            sequence[i] = 0;
        }
    }
    
    // 在指定位置插入目标模式
    if (insert_position + target_count <= length) {
        for (int i = 0; i < target_count; i++) {
            sequence[insert_position + i] = 1;
        }
        
        // 确保模式前后不是1（避免更长的连续序列）
        if (insert_position > 0) {
            sequence[insert_position - 1] = 0;
        }
        if (insert_position + target_count < length) {
            sequence[insert_position + target_count] = 0;
        }
    }
}

/**
 * @brief 打印序列（截断显示）
 * @param sequence 序列数组
 * @param length 序列长度
 * @param max_display 最大显示长度
 */
void print_sequence(const int *sequence, int length, int max_display) {
    printf("序列内容（");
    if (length <= max_display) {
        printf("完整显示）：");
        for (int i = 0; i < length; i++) {
            printf("%d", sequence[i]);
        }
    } else {
        printf("前%d个）：", max_display);
        for (int i = 0; i < max_display; i++) {
            printf("%d", sequence[i]);
        }
        printf("...");
    }
    printf("\n");
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
    printf("│ 1. 简单遍历     │    O(n)      │    O(1)      │ 简单直观       │\n");
    printf("│ 2. 状态机       │    O(n)      │    O(1)      │ 逻辑清晰       │\n");
    printf("│ 3. 滑动窗口     │   O(n*k)     │    O(1)      │ 直观但效率低   │\n");
    printf("│ 4. 优化窗口     │    O(n)      │    O(1)      │ 窗口优化       │\n");
    printf("│ 5. 位操作       │    O(n)      │    O(1)      │ 位运算优化     │\n");
    printf("└─────────────────┴──────────────┴──────────────┴────────────────┘\n");
    printf("注：n为序列长度，k为目标连续长度\n");
    
    printf("\n核心思想分析：\n");
    printf("1. 简单遍历法：\n");
    printf("   - 维护连续1的计数器\n");
    printf("   - 遇到0时重置计数器\n");
    printf("   - 最直观的解法\n\n");
    
    printf("2. 状态机法：\n");
    printf("   - 每个状态表示连续1的个数\n");
    printf("   - 状态转移清晰明确\n");
    printf("   - 便于扩展和维护\n\n");
    
    printf("3. 滑动窗口法：\n");
    printf("   - 固定大小窗口检查\n");
    printf("   - 可以获得更多信息\n");
    printf("   - 基础版本效率较低\n\n");
    
    printf("4. 优化窗口法：\n");
    printf("   - 避免重复计算窗口和\n");
    printf("   - 边界更新技巧\n");
    printf("   - 时间复杂度优化到O(n)\n\n");
    
    printf("5. 位操作法：\n");
    printf("   - 利用位运算的高效性\n");
    printf("   - 适合小目标值的情况\n");
    printf("   - CPU指令级别的优化\n");
}

/**
 * @brief 性能测试
 */
void performance_test() {
    printf("\n=== 性能测试 ===\n");
    
    const int test_length = 100000;
    int *test_sequence = (int*)malloc(test_length * sizeof(int));
    
    if (!test_sequence) {
        printf("内存分配失败\n");
        return;
    }
    
    // 生成测试序列（30%的1）
    generate_random_sequence(test_sequence, test_length, 30);
    
    printf("测试序列长度：%d\n", test_length);
    printf("目标：寻找%d个连续的1\n", TARGET_CONSECUTIVE_ONES);
    
    // 测试不同方法的性能
    clock_t start, end;
    double time_used;
    
    // 方法1：简单遍历
    start = clock();
    bool result1 = has_consecutive_ones_simple(test_sequence, test_length, TARGET_CONSECUTIVE_ONES);
    end = clock();
    time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("简单遍历法：%.6f 秒，结果：%s\n", time_used, result1 ? "找到" : "未找到");
    
    // 方法2：状态机
    start = clock();
    bool result2 = has_consecutive_ones_state_machine(test_sequence, test_length, TARGET_CONSECUTIVE_ONES);
    end = clock();
    time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("状态机法：%.6f 秒，结果：%s\n", time_used, result2 ? "找到" : "未找到");
    
    // 方法4：优化窗口（跳过方法3，因为它效率较低）
    start = clock();
    bool result4 = has_consecutive_ones_optimized_window(test_sequence, test_length, TARGET_CONSECUTIVE_ONES);
    end = clock();
    time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("优化窗口法：%.6f 秒，结果：%s\n", time_used, result4 ? "找到" : "未找到");
    
    // 方法5：位操作
    start = clock();
    bool result5 = has_consecutive_ones_bit_ops(test_sequence, test_length, TARGET_CONSECUTIVE_ONES);
    end = clock();
    time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("位操作法：%.6f 秒，结果：%s\n", time_used, result5 ? "找到" : "未找到");
    
    // 验证结果一致性
    printf("\n结果一致性检查：");
    if (result1 == result2 && result2 == result4 && result4 == result5) {
        printf("所有方法结果一致 ✓\n");
    } else {
        printf("结果不一致 ✗\n");
    }
    
    free(test_sequence);
}

/**
 * @brief 边界情况测试
 */
void test_edge_cases() {
    printf("\n=== 边界情况测试 ===\n");
    
    // 测试1：空序列
    printf("测试1：空序列\n");
    bool result = has_consecutive_ones_simple(NULL, 0, 5);
    printf("结果：%s\n", result ? "找到（错误）" : "未找到（正确）");
    
    // 测试2：序列长度小于目标长度
    printf("\n测试2：序列长度小于目标长度\n");
    int short_seq[] = {1, 1, 1};
    result = has_consecutive_ones_simple(short_seq, 3, 5);
    printf("结果：%s\n", result ? "找到（错误）" : "未找到（正确）");
    
    // 测试3：全0序列
    printf("\n测试3：全0序列\n");
    int zero_seq[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    result = has_consecutive_ones_simple(zero_seq, 12, 5);
    printf("结果：%s\n", result ? "找到（错误）" : "未找到（正确）");
    
    // 测试4：全1序列
    printf("\n测试4：全1序列\n");
    int one_seq[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    result = has_consecutive_ones_simple(one_seq, 12, 5);
    printf("结果：%s\n", result ? "找到（正确）" : "未找到（错误）");
    
    // 测试5：目标长度为1
    printf("\n测试5：目标长度为1\n");
    int mixed_seq[] = {0, 1, 0, 0, 1, 0};
    result = has_consecutive_ones_simple(mixed_seq, 6, 1);
    printf("结果：%s\n", result ? "找到（正确）" : "未找到（错误）");
    
    // 测试6：刚好达到目标长度
    printf("\n测试6：刚好达到目标长度\n");
    int exact_seq[] = {0, 0, 1, 1, 1, 1, 1, 0, 0};
    result = has_consecutive_ones_simple(exact_seq, 9, 5);
    printf("结果：%s\n", result ? "找到（正确）" : "未找到（错误）");
    
    // 测试7：差一个达到目标
    printf("\n测试7：差一个达到目标\n");
    int almost_seq[] = {0, 0, 1, 1, 1, 1, 0, 0};
    result = has_consecutive_ones_simple(almost_seq, 8, 5);
    printf("结果：%s\n", result ? "找到（错误）" : "未找到（正确）");
}

/**
 * @brief 流式处理测试
 */
void test_streaming_processing() {
    printf("\n=== 流式处理测试 ===\n");
    
    TickAnalyzer analyzer;
    init_tick_analyzer(&analyzer, TARGET_CONSECUTIVE_ONES);
    
    // 模拟实时tick流
    int tick_stream[] = {0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0};
    int stream_length = sizeof(tick_stream) / sizeof(tick_stream[0]);
    
    printf("模拟实时tick流处理：\n");
    print_sequence(tick_stream, stream_length, stream_length);
    
    for (int i = 0; i < stream_length; i++) {
        printf("处理tick %d: %d", i, tick_stream[i]);
        
        bool found = process_tick(&analyzer, tick_stream[i]);
        
        if (found) {
            printf(" - 找到目标模式！\n");
        } else {
            printf(" - 当前连续1个数：%d\n", analyzer.current_count);
        }
    }
    
    print_analyzer_stats(&analyzer);
}

/**
 * @brief 主测试函数
 */
void test_tick_sequence_analyzer() {
    printf("=== Tick序列分析器测试 ===\n");
    
    // 创建包含目标模式的测试序列
    const int test_length = 30;
    int test_sequence[test_length];
    
    generate_test_sequence_with_pattern(test_sequence, test_length, 
                                      TARGET_CONSECUTIVE_ONES, 10);
    
    print_sequence(test_sequence, test_length, test_length);
    
    // 测试所有方法
    printf("\n--- 测试所有算法方法 ---\n");
    
    bool result1 = has_consecutive_ones_simple(test_sequence, test_length, TARGET_CONSECUTIVE_ONES);
    bool result2 = has_consecutive_ones_state_machine(test_sequence, test_length, TARGET_CONSECUTIVE_ONES);
    bool result3 = has_consecutive_ones_sliding_window(test_sequence, test_length, TARGET_CONSECUTIVE_ONES);
    bool result4 = has_consecutive_ones_optimized_window(test_sequence, test_length, TARGET_CONSECUTIVE_ONES);
    bool result5 = has_consecutive_ones_bit_ops(test_sequence, test_length, TARGET_CONSECUTIVE_ONES);
    
    // 验证结果一致性
    printf("\n=== 结果验证 ===\n");
    printf("简单遍历法：%s\n", result1 ? "找到" : "未找到");
    printf("状态机法：%s\n", result2 ? "找到" : "未找到");
    printf("滑动窗口法：%s\n", result3 ? "找到" : "未找到");
    printf("优化窗口法：%s\n", result4 ? "找到" : "未找到");
    printf("位操作法：%s\n", result5 ? "找到" : "未找到");
    
    bool all_consistent = (result1 == result2) && (result2 == result3) && 
                         (result3 == result4) && (result4 == result5);
    printf("结果一致性：%s\n", all_consistent ? "一致 ✓" : "不一致 ✗");
    
    // 测试无目标模式的序列
    printf("\n--- 测试无目标模式的序列 ---\n");
    int no_pattern_seq[20];
    generate_random_sequence(no_pattern_seq, 20, 40);  // 40%的1，但避免长连续
    
    // 确保没有长连续序列
    for (int i = 0; i < 20; i++) {
        if (i % 3 == 2) no_pattern_seq[i] = 0;  // 每3个中至少有一个0
    }
    
    print_sequence(no_pattern_seq, 20, 20);
    
    bool no_result = has_consecutive_ones_simple(no_pattern_seq, 20, TARGET_CONSECUTIVE_ONES);
    printf("结果：%s\n", no_result ? "找到（可能错误）" : "未找到（正确）");
}

/**
 * @brief 主函数
 */
int main() {
    printf("题目：判断tick序列中是否有%d次连续的1\n", TARGET_CONSECUTIVE_ONES);
    printf("=========================================\n");
    
    // 初始化随机数生成器
    srand(time(NULL));
    
    // 算法复杂度分析
    complexity_analysis();
    
    // 主要功能测试
    test_tick_sequence_analyzer();
    
    // 边界情况测试
    test_edge_cases();
    
    // 流式处理测试
    test_streaming_processing();
    
    // 性能测试
    performance_test();
    
    printf("\n=== 面试要点总结 ===\n");
    printf("1. 问题理解：模式匹配 vs 简单计数\n");
    printf("2. 算法选择：状态机 vs 滑动窗口 vs 位操作\n");
    printf("3. 复杂度优化：从O(n*k)到O(n)的优化思路\n");
    printf("4. 实现技巧：位操作、窗口优化、流式处理\n");
    printf("5. 边界处理：空序列、极端情况、参数验证\n");
    printf("6. 实际应用：信号处理、模式识别、状态监控\n");
    
    printf("\n=== 扩展思考 ===\n");
    printf("1. 如何处理多种模式的同时检测？\n");
    printf("2. 如何优化内存使用（超长序列）？\n");
    printf("3. 如何实现模糊匹配（允许少量0）？\n");
    printf("4. 如何处理实时流数据？\n");
    printf("5. 如何扩展到2D模式匹配？\n");
    printf("6. 如何实现高效的多模式匹配？\n");
    
    return 0;
}

/**
 * @brief 编译和运行说明
 * 
 * 编译：
 * gcc -o tick_sequence_analyzer tick_sequence_analyzer.c -g
 * 
 * 运行：
 * ./tick_sequence_analyzer
 * 
 * 调试：
 * gdb ./tick_sequence_analyzer
 * 
 * 内存检查：
 * valgrind --leak-check=full ./tick_sequence_analyzer
 * 
 * 这个实现涵盖了：
 * 1. 五种不同的算法实现
 * 2. 详细的算法思路解释
 * 3. 完整的测试框架
 * 4. 边界情况处理
 * 5. 性能测试和比较
 * 6. 流式数据处理
 * 7. 实际应用场景分析
 * 
 * 面试重点：
 * - 理解不同算法的优缺点
 * - 掌握状态机设计思想
 * - 能够进行复杂度分析和优化
 * - 考虑实际应用中的流式处理
 * - 注重代码质量和测试覆盖
 * 
 * 实际应用：
 * - 数字信号处理
 * - 网络协议分析
 * - 系统状态监控
 * - 模式识别系统
 * - 实时数据分析
 */
