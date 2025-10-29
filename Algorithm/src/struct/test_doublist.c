#include <stdio.h>
#include "doublist.h"

int main(int argc, char **argv) {
	printf("========================================\n");
	printf("  双向链表测试程序\n");
	printf("========================================\n\n");

	// 只运行前5个基础测试
	testCreateDoubList();
	testgetDoubListLen();
	testSearchDoubList();
	testFindDoubListPos();
	testInsertDoubNode();

	printf("\n========================================\n");
	printf("  基础测试完成！\n");
	printf("========================================\n");

	return SUCCESS;
}

