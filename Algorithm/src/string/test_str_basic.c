#include <stdio.h>
#include "str.h"

int main() {
    printf("========================================\n");
    printf("  字符串库基础测试\n");
    printf("========================================\n\n");

    // 测试基础函数
    testint2str();
    teststr2int();
    testmystrcpy();
    testmystrlen();
    testmystrstr();
    testisPalindromeStr();
    testmystrcmp();

    printf("\n========================================\n");
    printf("  基础测试完成！\n");
    printf("========================================\n");

    return SUCCESS;
}

