/**
 * @file multiply_assembly.c
 * @brief 使用有限指令集实现乘法运算的面试题
 * 
 * 题目：使用只有5条指令的汇编语言实现 X*Y 乘法运算
 * 可用指令：LABEL、inc a、dec a、jnz X、mov a,x
 * 
 * 这是一个经典的计算机科学面试题，考察：
 * 1. 对基础汇编指令的理解
 * 2. 算法设计能力（用加法实现乘法）
 * 3. 循环控制和条件跳转
 * 4. 寄存器使用和内存管理
 * 
 * @author feliu@nvidia.com
 * @date 2025-10-23
 */

#include <stdio.h>
#include <stdint.h>

/**
 * @brief 模拟汇编环境的寄存器和内存
 */
typedef struct {
    int32_t a;      // 累加器寄存器
    int32_t x;      // X寄存器
    int32_t y;      // Y寄存器
    int32_t result; // 结果寄存器
} cpu_state_t;

/**
 * @brief 汇编指令枚举
 */
typedef enum {
    LABEL,  // 标签（用于跳转目标）
    INC_A,  // inc a - 累加器加1
    DEC_A,  // dec a - 累加器减1
    JNZ,    // jnz X - 如果累加器不为0则跳转到标签X
    MOV_A_X // mov a,x - 将x的值移动到累加器a
} instruction_t;

/**
 * @brief 指令结构体
 */
typedef struct {
    instruction_t op;
    int operand;    // 操作数（用于跳转地址或寄存器编号）
    const char* label; // 标签名称
} asm_instruction_t;

/**
 * @brief 乘法算法的伪汇编实现
 * 
 * 算法思路：
 * X * Y = X + X + ... + X (Y次)
 * 
 * 伪汇编代码：
 * 1. mov a, 0      ; 初始化结果为0（使用多次dec实现）
 * 2. mov counter, Y ; 设置循环计数器为Y
 * 3. LOOP:
 * 4.   add result, X ; 结果累加X（使用多次inc实现）
 * 5.   dec counter   ; 计数器减1
 * 6.   jnz LOOP      ; 如果计数器不为0，继续循环
 * 7. 返回result
 * 
 * 由于只能使用inc/dec，需要将加法分解为多次inc操作
 */

/**
 * @brief 使用有限指令集实现乘法（模拟版本）
 * 
 * @param x 乘数X
 * @param y 乘数Y
 * @return 乘积 X*Y
 * 
 * 实现思路：
 * 1. 使用循环Y次，每次将结果加上X
 * 2. 由于只能使用inc/dec，需要用inc来实现加法
 * 3. 使用jnz实现循环控制
 */
int32_t multiply_with_limited_instructions(int32_t x, int32_t y) {
    printf("\n=== 使用有限指令集实现 %d * %d ===\n", x, y);
    
    // 处理特殊情况
    if (x == 0 || y == 0) {
        printf("特殊情况：任一操作数为0，结果为0\n");
        return 0;
    }
    
    // 处理负数情况
    int sign = 1;
    if (x < 0) {
        sign = -sign;
        x = -x;
        printf("X为负数，转换为正数处理，记录符号\n");
    }
    if (y < 0) {
        sign = -sign;
        y = -y;
        printf("Y为负数，转换为正数处理，记录符号\n");
    }
    
    printf("算法：通过循环%d次，每次累加%d来实现乘法\n", y, x);
    
    cpu_state_t cpu = {0};
    int32_t result = 0;
    int32_t counter = y;
    
    printf("\n伪汇编执行过程：\n");
    printf("初始化：result = 0, counter = %d\n", y);
    
    int loop_count = 0;
    // 主循环：相当于汇编中的循环标签
    while (counter > 0) {
        printf("循环 %d: ", ++loop_count);
        
        // 将X加到result上（使用inc实现）
        // 这里模拟多次inc a操作
        for (int i = 0; i < x; i++) {
            result++;  // 相当于 inc result
        }
        
        counter--;  // 相当于 dec counter
        printf("result += %d, counter = %d, 当前result = %d\n", x, counter, result);
        
        // 相当于 jnz LOOP_START
        // 如果counter不为0，继续循环
    }
    
    result *= sign;  // 应用符号
    printf("最终结果：%d\n", result);
    
    return result;
}

/**
 * @brief 生成实际的伪汇编代码
 * 
 * @param x 乘数X
 * @param y 乘数Y
 */
void generate_assembly_code(int32_t x, int32_t y) {
    printf("\n=== 生成的伪汇编代码 ===\n");
    printf("; 计算 %d * %d\n", x, y);
    printf("; 使用指令集：LABEL, inc a, dec a, jnz X, mov a,x\n\n");
    
    printf("MULTIPLY:\n");
    printf("    mov a, 0          ; 初始化结果寄存器为0\n");
    printf("    mov x, %d         ; 设置X值\n", x);
    printf("    mov y, %d         ; 设置Y值（循环计数器）\n", y);
    printf("\n");
    
    printf("OUTER_LOOP:\n");
    printf("    mov a, %d         ; 设置内循环计数器为X\n", x);
    printf("\n");
    
    printf("INNER_LOOP:\n");
    printf("    inc result        ; result += 1\n");
    printf("    dec a             ; 内循环计数器减1\n");
    printf("    jnz INNER_LOOP    ; 如果a不为0，继续内循环\n");
    printf("\n");
    
    printf("    dec y             ; 外循环计数器减1\n");
    printf("    jnz OUTER_LOOP    ; 如果y不为0，继续外循环\n");
    printf("\n");
    
    printf("END:\n");
    printf("    ; 结果在result寄存器中\n");
    printf("    ; result = %d\n", x * y);
}

/**
 * @brief 详细的算法分析
 */
void algorithm_analysis() {
    printf("\n=== 算法分析 ===\n");
    
    printf("1. 问题分析：\n");
    printf("   - 只能使用5种基础指令\n");
    printf("   - 没有乘法指令，需要用加法实现\n");
    printf("   - 没有加法指令，需要用inc实现\n");
    printf("   - 需要循环控制结构\n\n");
    
    printf("2. 算法设计：\n");
    printf("   - 外循环：执行Y次\n");
    printf("   - 内循环：每次执行X次inc操作\n");
    printf("   - 总的inc操作次数：X * Y\n");
    printf("   - 时间复杂度：O(X * Y)\n\n");
    
    printf("3. 指令使用：\n");
    printf("   - LABEL：定义循环标签\n");
    printf("   - inc a：实现加1操作\n");
    printf("   - dec a：实现减1操作（计数器）\n");
    printf("   - jnz X：实现条件跳转（循环控制）\n");
    printf("   - mov a,x：初始化和数据传输\n\n");
    
    printf("4. 优化考虑：\n");
    printf("   - 如果X < Y，可以交换X和Y减少外循环次数\n");
    printf("   - 处理特殊情况：0, 负数\n");
    printf("   - 考虑溢出问题\n\n");
    
    printf("5. 实际应用：\n");
    printf("   - 早期计算机和微控制器\n");
    printf("   - 资源受限的嵌入式系统\n");
    printf("   - 理解CPU指令集设计\n");
}

/**
 * @brief 优化版本：交换操作数减少循环次数
 */
int32_t multiply_optimized(int32_t x, int32_t y) {
    printf("\n=== 优化版本：减少循环次数 ===\n");
    
    // 交换操作数，让较小的数作为外循环计数器
    if (x > y && y > 0) {
        int32_t temp = x;
        x = y;
        y = temp;
        printf("交换操作数：x=%d, y=%d（减少循环次数）\n", x, y);
    }
    
    return multiply_with_limited_instructions(x, y);
}

/**
 * @brief 测试函数
 */
void test_multiply() {
    printf("=== 乘法实现测试 ===\n");
    
    // 测试用例
    int test_cases[][2] = {
        {3, 4},     // 基本情况
        {0, 5},     // 零乘法
        {7, 1},     // 乘以1
        {-3, 4},    // 负数
        {-2, -3},   // 两个负数
        {12, 8}     // 较大数值
    };
    
    int num_tests = sizeof(test_cases) / sizeof(test_cases[0]);
    
    for (int i = 0; i < num_tests; i++) {
        int x = test_cases[i][0];
        int y = test_cases[i][1];
        
        printf("\n--- 测试 %d: %d * %d ---\n", i + 1, x, y);
        
        int result = multiply_with_limited_instructions(x, y);
        int expected = x * y;
        
        printf("实现结果：%d\n", result);
        printf("期望结果：%d\n", expected);
        printf("测试%s\n", (result == expected) ? "通过" : "失败");
        
        // 生成对应的汇编代码
        if (x > 0 && y > 0 && x <= 5 && y <= 5) {
            generate_assembly_code(x, y);
        }
    }
}

/**
 * @brief 主函数
 */
int main() {
    printf("题目：使用有限指令集实现乘法运算\n");
    printf("可用指令：LABEL, inc a, dec a, jnz X, mov a,x\n");
    printf("========================================\n");
    
    // 算法分析
    algorithm_analysis();
    
    // 测试实现
    test_multiply();
    
    // 演示优化版本
    printf("\n=== 优化演示 ===\n");
    multiply_optimized(12, 3);
    
    return 0;
}

/**
 * @brief 编译和运行说明
 * 
 * 编译：
 * gcc -o multiply_assembly multiply_assembly.c
 * 
 * 运行：
 * ./multiply_assembly
 * 
 * 这个实现展示了：
 * 1. 如何用基础指令构建复杂操作
 * 2. 循环控制的汇编实现
 * 3. 算法优化思路
 * 4. 特殊情况处理
 * 
 * 面试要点：
 * - 理解指令集限制下的编程
 * - 掌握循环和条件跳转
 * - 考虑算法优化
 * - 处理边界情况
 */
