/*
 * ================================================================================
 * NVIDIA 面试题：无符号整数中交换两位 (Swap Two Bits in Unsigned Integer)
 * ================================================================================
 * 
 * 题目描述：
 * 给定一个无符号整数和两个位位置i和j，交换第i位和第j位的值。
 * 位置从0开始计数，0表示最低位（LSB），31表示最高位（MSB）。
 * 
 * 例如：
 * 输入：num = 47 (二进制：101111), i = 1, j = 4
 * 输出：45 (二进制：101101)
 * 解释：第1位=1，第4位=0，交换后第1位=0，第4位=1
 * 
 * 位置分析：
 * 47 = 101111 (位置: 543210)
 *      ↑   ↑
 *      4   1  ← 交换位置4和位置1
 *      0   1  ← 对应的位值
 * 结果：101101 = 45
 * 
 * 要求：
 * 1. 时间复杂度：O(1)
 * 2. 空间复杂度：O(1)
 * 3. 处理边界情况（i == j, 越界等）
 * 4. 支持32位和64位整数
 * 
 * 考察点：
 * - 位操作基础知识
 * - 边界条件处理
 * - 代码健壮性
 * - XOR操作的巧妙应用
 * 
 * 解法思路：
 * 方法1：检查位是否相同，不同则使用XOR翻转
 * 方法2：无条件XOR交换（更简洁）
 * 方法3：使用位掩码的通用方法
 * ================================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <time.h>

/**
 * 方法1：位掩码通用方法 - 最直观的解法
 * 使用位掩码创建和应用交换模式
 * 
 * 位掩码原理详解：
 * 
 * 1. 掩码的概念：
 *    掩码(mask)是用来选择、提取或操作特定位的工具
 *    就像物理世界的面具，只露出我们关心的部分
 * 
 * 2. 三种基本掩码操作：
 *    - 提取位：num & mask (AND操作保留mask为1的位，其他位变0)
 *    - 清除位：num & ~mask (AND反掩码，清除mask为1的位)
 *    - 设置位：num | mask (OR操作设置mask为1的位)
 * 
 * 3. 位交换的掩码策略：
 *    步骤1：创建掩码 - 标记要操作的位
 *    步骤2：提取位值 - 保存原来的位值
 *    步骤3：清除原位 - 将目标位清0
 *    步骤4：设置新值 - 将交换后的值写入
 * 
 * 4. 举例说明 (num=47, 交换第1位和第4位)：
 *    
 *    原始数字：00101111 (47)
 *                 ↑   ↑
 *                 4   1  (要交换的位置)
 *                 0   1  (对应的位值)
 *    
 *    步骤1 - 创建掩码：
 *    mask_i = 1 << 1 = 00000010 (标记第1位)
 *    mask_j = 1 << 4 = 00010000 (标记第4位)
 *    combined_mask = 00000010 | 00010000 = 00010010 (组合掩码)
 *    
 *    步骤2 - 提取位值：
 *    bit_i = (00101111 & 00000010) ? 1 : 0 = 1 (第1位的值)
 *    bit_j = (00101111 & 00010000) ? 1 : 0 = 0 (第4位的值)
 *    
 *    步骤3 - 清除目标位：
 *    ~combined_mask = 11101101 (反转组合掩码)
 *    num &= ~combined_mask:
 *    00101111 & 11101101 = 00101101 (清除第1位和第4位)
 *    
 *    步骤4 - 设置新值：
 *    num |= (bit_j << i) = num |= (0 << 1) = num |= 0
 *    00101101 | 00000000 = 00101101 (设置第1位为0)
 *    
 *    num |= (bit_i << j) = num |= (1 << 4) = num |= 16
 *    00101101 | 00010000 = 00111101 (设置第4位为1)
 *    
 *    最终结果：00111101 = 61 ✓
 * 
 * 5. 掩码方法的优势：
 *    - 步骤清晰：每步操作都有明确的目的
 *    - 易于理解：符合"清除-设置"的直观思维
 *    - 通用性强：可以轻松扩展到多位交换
 *    - 调试友好：每步都可以单独验证
 */
uint32_t swap_bits_mask(uint32_t num, int i, int j) {
    // 边界检查
    if (i < 0 || i >= 32 || j < 0 || j >= 32) {
        printf("错误：位位置超出范围 [0, 31]\n");
        return num;
    }
    
    // 如果i和j相同，无需交换
    if (i == j) {
        return num;
    }
    
    // 创建位掩码
    uint32_t mask_i = 1U << i;
    uint32_t mask_j = 1U << j;
    uint32_t combined_mask = mask_i | mask_j;
    
    // 提取两位的值
    int bit_i = (num & mask_i) ? 1 : 0;
    int bit_j = (num & mask_j) ? 1 : 0;
    
    // 清除原来的两位
    num &= ~combined_mask;
    
    // 设置交换后的两位
    num |= (bit_j << i);
    num |= (bit_i << j);
    
    return num;
}

/**
 * 方法2：条件交换法
 * 先检查两位是否相同，只在不同时才交换
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
uint32_t swap_bits_conditional(uint32_t num, int i, int j) {
    // 边界检查
    if (i < 0 || i >= 32 || j < 0 || j >= 32) {
        printf("错误：位位置超出范围 [0, 31]\n");
        return num;
    }
    
    // 如果i和j相同，无需交换
    if (i == j) {
        return num;
    }
    
    // 提取第i位和第j位
    int bit_i = (num >> i) & 1;
    int bit_j = (num >> j) & 1;
    
    // 只有当两位不同时才需要交换
    if (bit_i != bit_j) {
        // 使用XOR翻转这两位
        num ^= (1U << i);
        num ^= (1U << j);
    }
    
    return num;
}

/**
 * 方法3：无条件XOR交换法 - 最高效的解法
 * 直接使用XOR操作，无需预先检查
 * 更简洁，性能更好
 * 
 * 核心原理详解：
 * 
 * 1. XOR操作的对象：
 *    - 我们不是对单个位进行XOR
 *    - 而是对整个32位数字进行XOR操作
 *    - XOR的对象是 (xor_result << i) 这个32位数
 * 
 * 2. 巧妙之处：
 *    当两位相同时：bit_i ^ bit_j = 0
 *    - xor_result = 0
 *    - (xor_result << i) = 0 << i = 0 (全0的32位数)
 *    - (xor_result << j) = 0 << j = 0 (全0的32位数)
 *    - num ^ 0 = num (任何数与0 XOR保持不变)
 * 
 *    当两位不同时：bit_i ^ bit_j = 1  
 *    - xor_result = 1
 *    - (xor_result << i) = 1 << i (只有第i位为1的32位数)
 *    - (xor_result << j) = 1 << j (只有第j位为1的32位数)
 *    - num ^ (1 << i) 翻转第i位
 *    - num ^ (1 << j) 翻转第j位
 * 
 * 3. 举例说明：
 *    假设 num = 47 (101111), 交换第1位(1)和第4位(0)
 *    
 *    xor_result = 1 ^ 0 = 1
 *    
 *    第一次XOR: num ^= (1 << 1) = num ^= 2
 *               101111 ^ 000010 = 101101 (翻转第1位：1→0)
 *    
 *    第二次XOR: num ^= (1 << 4) = num ^= 16
 *               101101 ^ 010000 = 111101 (翻转第4位：0→1)
 *    
 *    最终结果：111101 = 61 (成功交换)
 * 
 * 4. 为什么无需条件判断：
 *    XOR操作自动处理了两种情况：
 *    - 相同位：xor_result=0，XOR全0数字，原值不变
 *    - 不同位：xor_result=1，XOR对应掩码，翻转目标位
 */
uint32_t swap_bits_xor(uint32_t num, int i, int j) {
    // 边界检查
    if (i < 0 || i >= 32 || j < 0 || j >= 32) {
        printf("错误：位位置超出范围 [0, 31]\n");
        return num;
    }
    
    // 如果i和j相同，无需交换
    if (i == j) {
        return num;
    }
    
    // 提取第i位和第j位
    int bit_i = (num >> i) & 1;
    int bit_j = (num >> j) & 1;
    
    // 计算XOR结果，如果两位相同则为0，不同则为1
    int xor_result = bit_i ^ bit_j;
    
    // 如果两位不同，则翻转它们
    num ^= (xor_result << i);
    num ^= (xor_result << j);
    
    return num;
}

/**
 * 64位版本：交换64位整数中的两位
 */
uint64_t swap_bits_64(uint64_t num, int i, int j) {
    // 边界检查
    if (i < 0 || i >= 64 || j < 0 || j >= 64) {
        printf("错误：位位置超出范围 [0, 63]\n");
        return num;
    }
    
    // 如果i和j相同，无需交换
    if (i == j) {
        return num;
    }
    
    // 提取第i位和第j位
    int bit_i = (num >> i) & 1;
    int bit_j = (num >> j) & 1;
    
    // 计算XOR结果
    int xor_result = bit_i ^ bit_j;
    
    // 交换位
    num ^= ((uint64_t)xor_result << i);
    num ^= ((uint64_t)xor_result << j);
    
    return num;
}

/**
 * 批量交换：交换多对位
 */
uint32_t swap_multiple_bits(uint32_t num, int pairs[][2], int num_pairs) {
    for (int k = 0; k < num_pairs; k++) {
        num = swap_bits_mask(num, pairs[k][0], pairs[k][1]);
    }
    return num;
}

/**
 * 辅助函数：打印二进制表示（32位）
 */
void print_binary_32(uint32_t n, const char* label) {
    printf("%s: ", label);
    for (int i = 31; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
        if (i % 4 == 0 && i > 0) printf(" ");
    }
    printf(" (十进制: %u)\n", n);
}

/**
 * 辅助函数：打印二进制表示（64位）
 */
void print_binary_64(uint64_t n, const char* label) {
    printf("%s: ", label);
    for (int i = 63; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
        if (i % 4 == 0 && i > 0) printf(" ");
        if (i == 32) printf("\n          ");  // 换行显示高32位和低32位
    }
    printf(" (十进制: %llu)\n", (unsigned long long)n);
}

/**
 * 测试函数：基本功能测试
 */
void test_basic_functionality() {
    printf("=== 基本功能测试 ===\n\n");
    
    // 测试用例1：修正后的示例数据
    uint32_t num1 = 47;  // 二进制：101111
    int i1 = 1, j1 = 4;  // 交换第1位(1)和第4位(0)
    
    printf("测试用例1：\n");
    print_binary_32(num1, "原数字");
    printf("交换位置: %d 和 %d\n", i1, j1);
    
    uint32_t result1 = swap_bits_mask(num1, i1, j1);
    uint32_t result2 = swap_bits_conditional(num1, i1, j1);
    uint32_t result3 = swap_bits_xor(num1, i1, j1);
    
    print_binary_32(result1, "方法1结果");
    print_binary_32(result2, "方法2结果");
    print_binary_32(result3, "方法3结果");
    
    if (result1 == result2 && result2 == result3) {
        printf("✓ 三种方法结果一致\n");
    } else {
        printf("✗ 方法结果不一致！\n");
    }
    printf("\n");
    
    // 测试用例2：交换相同位
    printf("测试用例2：交换相同位\n");
    uint32_t num2 = 0xAAAAAAAA;
    print_binary_32(num2, "原数字");
    uint32_t result_same = swap_bits_mask(num2, 5, 5);
    print_binary_32(result_same, "交换结果");
    printf("应该保持不变: %s\n\n", (num2 == result_same) ? "✓" : "✗");
    
    // 测试用例3：边界位交换
    printf("测试用例3：最高位和最低位交换\n");
    uint32_t num3 = 0x80000001;  // 最高位和最低位都是1
    print_binary_32(num3, "原数字");
    uint32_t result_boundary = swap_bits_mask(num3, 0, 31);
    print_binary_32(result_boundary, "交换结果");
    printf("\n");
}

/**
 * 测试函数：边界条件测试
 */
void test_edge_cases() {
    printf("=== 边界条件测试 ===\n\n");
    
    uint32_t num = 0x12345678;
    
    // 测试越界情况
    printf("测试越界位置：\n");
    uint32_t result_invalid1 = swap_bits_mask(num, -1, 5);
    uint32_t result_invalid2 = swap_bits_mask(num, 5, 32);
    printf("原数字应保持不变\n\n");
    
    // 测试全0和全1
    printf("测试特殊值：\n");
    uint32_t zero = 0;
    uint32_t ones = 0xFFFFFFFF;
    
    print_binary_32(zero, "全0原值");
    uint32_t zero_result = swap_bits_mask(zero, 0, 31);
    print_binary_32(zero_result, "全0交换");
    
    print_binary_32(ones, "全1原值");
    uint32_t ones_result = swap_bits_mask(ones, 0, 31);
    print_binary_32(ones_result, "全1交换");
    printf("\n");
}

/**
 * 测试函数：64位测试
 */
void test_64bit() {
    printf("=== 64位整数测试 ===\n\n");
    
    uint64_t num64 = 0x123456789ABCDEF0ULL;
    printf("测试64位整数交换：\n");
    print_binary_64(num64, "原数字");
    
    uint64_t result64 = swap_bits_64(num64, 0, 63);
    print_binary_64(result64, "交换0位和63位");
    printf("\n");
}

/**
 * 测试函数：批量交换测试
 */
void test_multiple_swaps() {
    printf("=== 批量交换测试 ===\n\n");
    
    uint32_t num = 0x12345678;
    int swap_pairs[][2] = {
        {0, 7},   // 交换第0位和第7位
        {8, 15},  // 交换第8位和第15位
        {16, 23}, // 交换第16位和第23位
        {24, 31}  // 交换第24位和第31位
    };
    int num_pairs = sizeof(swap_pairs) / sizeof(swap_pairs[0]);
    
    print_binary_32(num, "原数字");
    uint32_t result = swap_multiple_bits(num, swap_pairs, num_pairs);
    print_binary_32(result, "批量交换后");
    printf("\n");
}

/**
 * 性能测试
 */
void performance_test() {
    printf("=== 性能测试 ===\n");
    const int iterations = 10000000;
    uint32_t test_num = 0x12345678;
    
    clock_t start, end;
    
    // 测试方法1：掩码交换法
    start = clock();
    for (int i = 0; i < iterations; i++) {
        swap_bits_mask(test_num, 5, 10);
    }
    end = clock();
    printf("掩码交换法: %f 秒\n", ((double)(end - start)) / CLOCKS_PER_SEC);
    
    // 测试方法2：条件交换法
    start = clock();
    for (int i = 0; i < iterations; i++) {
        swap_bits_conditional(test_num, 5, 10);
    }
    end = clock();
    printf("条件交换法: %f 秒\n", ((double)(end - start)) / CLOCKS_PER_SEC);
    
    // 测试方法3：XOR交换法
    start = clock();
    for (int i = 0; i < iterations; i++) {
        swap_bits_xor(test_num, 5, 10);
    }
    end = clock();
    printf("XOR交换法: %f 秒\n", ((double)(end - start)) / CLOCKS_PER_SEC);
    printf("\n");
}

/**
 * 主函数
 */
int main() {
    printf("NVIDIA 面试题：无符号整数中交换两位\n");
    printf("=============================================\n\n");
    
    test_basic_functionality();
    test_edge_cases();
    test_64bit();
    test_multiple_swaps();
    performance_test();
    
    return 0;
}

/*
 * ================================================================================
 * 面试要点总结：
 * 
 * 1. 核心概念：
 *    - 位操作的基本原理：提取、设置、清除、翻转
 *    - XOR操作的特性：a ^ a = 0, a ^ 0 = a
 *    - 位掩码的使用技巧
 * 
 * 2. 算法优化：
 *    - 条件检查 vs 无条件操作的权衡
 *    - 分支预测对性能的影响
 *    - 位操作比算术操作更高效
 * 
 * 3. 边界处理：
 *    - 输入验证的重要性
 *    - 越界访问的防护
 *    - 特殊情况的处理（i == j）
 * 
 * 4. 扩展应用：
 *    - 支持不同位宽的整数类型
 *    - 批量位操作的实现
 *    - 在图形处理、加密算法中的应用
 * 
 * 5. 面试中可能的后续问题：
 *    - 如何交换任意长度的位段？
 *    - 如何实现位的循环移位？
 *    - 在SIMD指令集中如何优化？
 *    - 如何处理大端和小端字节序？
 * ================================================================================
 */
