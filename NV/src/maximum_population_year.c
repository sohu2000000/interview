#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * ============================================================================
 * LeetCode 1854: Maximum Population Year
 * ============================================================================
 * 
 * 问题描述：
 * 给定一个人口记录数组，每条记录包含一个人的出生和死亡年份。
 * 某年x的人口数量是该年活着的人数。
 * 
 * 一个人在年份x时被计入人口，当且仅当 x 在闭区间 [birthi, deathi - 1] 内。
 * 注意：人在死亡的那一年不被计入。
 * 
 * 返回人口最多的最早年份。
 * 
 * 示例：
 * 输入：logs = [{1993,1999}, {2000,2010}]
 * 输出：1993
 * 解释：1993-1998年有1人，1999年0人，2000-2009年有1人
 * 
 * 约束：
 * - 1 <= logs.length <= 100
 * - 1950 <= birthi < deathi <= 2050
 */

#define MIN_YEAR 1950
#define MAX_YEAR 2050
#define YEAR_RANGE (MAX_YEAR - MIN_YEAR + 1)

/**
 * 人口记录结构体
 */
typedef struct PersonLifeLog {
    int birth_year;  // 出生年份
    int death_year;  // 死亡年份
} PersonLifeLog;

/**
 * 方法1：差分数组法（最优解）
 * 
 * 核心思想：
 * 使用差分数组记录每年人口的变化量：
 * - 出生年份：人口+1
 * - 死亡年份：人口-1
 * 然后通过前缀和计算每年的实际人口
 * 
 * 时间复杂度：O(n + range)，n是人数，range是年份范围
 * 空间复杂度：O(range)
 * 
 * 优点：
 * - 时间复杂度最优
 * - 代码简洁
 * - 易于理解
 */
int find_maximum_population_year_differential(PersonLifeLog* logs, int logs_size) {
    if (logs == NULL || logs_size == 0) {
        return MIN_YEAR;
    }

    // 差分数组：记录每年人口的变化量
    int population_delta[YEAR_RANGE];
    memset(population_delta, 0, sizeof(population_delta));

    // 步骤1: 构建差分数组
    for (int i = 0; i < logs_size; i++) {
        int birth_year = logs[i].birth_year;
        int death_year = logs[i].death_year;
        
        // 出生年份：人口+1
        population_delta[birth_year - MIN_YEAR]++;
        
        // 死亡年份：人口-1（注意：死亡当年不计入）
        population_delta[death_year - MIN_YEAR]--;
    }

    // 步骤2: 通过前缀和计算每年的实际人口，并找到最大值
    int current_population = 0;
    int max_population = 0;
    int max_year = MIN_YEAR;

    for (int year_offset = 0; year_offset < YEAR_RANGE; year_offset++) {
        current_population += population_delta[year_offset];
        
        if (current_population > max_population) {
            max_population = current_population;
            max_year = MIN_YEAR + year_offset;
        }
    }

    return max_year;
}

/**
 * 方法2：暴力枚举法（直观但效率低）
 * 
 * 核心思想：
 * 对每一年，遍历所有人，统计这一年有多少人活着
 * 
 * 时间复杂度：O(n × range)
 * 空间复杂度：O(1)
 * 
 * 优点：
 * - 逻辑简单直观
 * - 容易理解
 * 
 * 缺点：
 * - 效率低
 */
int find_maximum_population_year_bruteforce(PersonLifeLog* logs, int logs_size) {
    if (logs == NULL || logs_size == 0) {
        return MIN_YEAR;
    }

    int max_population = 0;
    int max_year = MIN_YEAR;

    // 遍历每一年
    for (int year = MIN_YEAR; year <= MAX_YEAR; year++) {
        int current_population = 0;
        
        // 统计这一年有多少人活着
        for (int i = 0; i < logs_size; i++) {
            int birth_year = logs[i].birth_year;
            int death_year = logs[i].death_year;
            
            // 判断这个人在year年是否活着
            if (year >= birth_year && year < death_year) {
                current_population++;
            }
        }
        
        // 更新最大人口和对应的年份
        if (current_population > max_population) {
            max_population = current_population;
            max_year = year;
        }
    }

    return max_year;
}

/**
 * 测试函数
 */
void test_maximum_population_year() {
    printf("========================================\n");
    printf("  LeetCode 1854: Maximum Population Year\n");
    printf("========================================\n\n");

    // 测试用例1
    printf("【测试1】\n");
    PersonLifeLog test1[] = {
        {1993, 1999},
        {2000, 2010}
    };
    
    int result1_diff = find_maximum_population_year_differential(test1, 2);
    int result1_brute = find_maximum_population_year_bruteforce(test1, 2);
    
    printf("输入: [{1993,1999}, {2000,2010}]\n");
    printf("差分数组法结果: %d\n", result1_diff);
    printf("暴力枚举法结果: %d\n", result1_brute);
    printf("期望输出: 1993\n");
    printf("验证: %s\n\n", (result1_diff == 1993 && result1_brute == 1993) ? "✅ 通过" : "❌ 失败");

    // 测试用例2
    printf("【测试2】\n");
    PersonLifeLog test2[] = {
        {1950, 1961},
        {1960, 1971},
        {1970, 1981}
    };
    
    int result2_diff = find_maximum_population_year_differential(test2, 3);
    int result2_brute = find_maximum_population_year_bruteforce(test2, 3);
    
    printf("输入: [{1950,1961}, {1960,1971}, {1970,1981}]\n");
    printf("差分数组法结果: %d\n", result2_diff);
    printf("暴力枚举法结果: %d\n", result2_brute);
    printf("期望输出: 1960（1960-1960年有2人）\n");
    printf("验证: %s\n\n", (result2_diff == 1960 && result2_brute == 1960) ? "✅ 通过" : "❌ 失败");

    // 测试用例3：复杂情况
    printf("【测试3: 复杂情况】\n");
    PersonLifeLog test3[] = {
        {1950, 1961}, {1960, 1971}, {1970, 1981},
        {1960, 1965}, {1970, 1980}, {1975, 1985}
    };
    
    int result3_diff = find_maximum_population_year_differential(test3, 6);
    int result3_brute = find_maximum_population_year_bruteforce(test3, 6);
    
    printf("输入: 6个人的出生死亡记录\n");
    printf("详细分析:\n");
    printf("  1950-1959: 1人\n");
    printf("  1960-1960: 3人 ← 最多\n");
    printf("  1961-1964: 2人\n");
    printf("  1965-1969: 1人\n");
    printf("  1970-1974: 3人\n");
    printf("  1975-1979: 4人\n");
    printf("  ...\n");
    printf("差分数组法结果: %d\n", result3_diff);
    printf("暴力枚举法结果: %d\n", result3_brute);
    printf("验证: 两种方法结果%s\n\n", 
           (result3_diff == result3_brute) ? "✅ 一致" : "❌ 不一致");

    printf("========================================\n");
    printf("  所有测试完成！\n");
    printf("========================================\n");
}

int main() {
    test_maximum_population_year();
    return 0;
}

