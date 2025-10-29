#include <stdio.h>
#include "list.h"

int main(void) {
    printf("========================================\n");
    printf("  单链表测试程序\n");
    printf("========================================\n\n");

    // 运行所有链表测试
    test_basic_operations();
    test_advanced_operations();

    printf("\n========================================\n");
    printf("  所有测试完成！\n");
    printf("========================================\n");

    return 0;
}

