#include <stdio.h>
#include <stdlib.h>

/*
 * LeetCode 148: 排序链表
 * 
 * 问题描述：
 * 给定链表的头节点 head，将链表升序排列并返回排序后的链表
 * 要求：时间复杂度 O(nlogn)，空间复杂度 O(1)
 * 
 * 示例：
 * 输入：head = [4,2,1,3]
 * 输出：[1,2,3,4]
 * 
 * 核心思路：
 * - 归并排序（自顶向下）
 * - 快慢指针找中点
 * - 递归排序左右两部分
 * - 合并两个有序链表
 */

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */

/*
 * 快慢指针找链表中点
 * @param head: 链表头
 * @param prevMiddlePtr: 中点的前驱节点（用于分割链表）
 * @return: 中点节点
 * 
 * 算法：
 * - fast每次走2步，slow每次走1步
 * - fast到末尾时，slow在中点
 * - prev记录slow的前驱
 */
struct ListNode *findMiddleNode(struct ListNode *head, struct ListNode **prevMiddlePtr) {
	struct ListNode *fastPtr = head;
	struct ListNode *slowPtr = head;
	struct ListNode *prevNode = NULL;

	while (fastPtr != NULL && fastPtr->next != NULL) {
		prevNode = slowPtr;
		slowPtr = slowPtr->next;
		fastPtr = fastPtr->next->next;
	}

	*prevMiddlePtr = prevNode;
	return slowPtr;
}

/*
 * 合并两个有序链表
 * @param leftList: 左侧有序链表
 * @param rightList: 右侧有序链表
 * @return: 合并后的有序链表
 * 
 * 算法：
 * 1. 使用哑节点简化边界处理
 * 2. 比较左右节点，较小的接入结果链表
 * 3. 处理剩余节点
 * 4. 释放哑节点，返回结果
 */
struct ListNode *mergeTwoSortedLists(struct ListNode *leftList, struct ListNode *rightList) {

	struct ListNode *dummyHead;
	struct ListNode *currentTail;
	struct ListNode *sortedList;

	dummyHead = (struct ListNode *)malloc(sizeof(struct ListNode));
	dummyHead->next = NULL;
	currentTail = dummyHead;

	/* 合并两个有序链表 */
	while (leftList != NULL && rightList != NULL) {
		if (leftList->val < rightList->val) {
			currentTail->next = leftList;
			leftList = leftList->next;
		} else {
			currentTail->next = rightList;
			rightList = rightList->next;
		}
		currentTail = currentTail->next;
	}

	/* 连接剩余节点 */
	if (leftList != NULL) {
		currentTail->next = leftList;
	} else {
		currentTail->next = rightList;
	}

	/* 释放哑节点 */
	sortedList = dummyHead->next;
	free(dummyHead);
	
	return sortedList;
}

/*
 * 归并排序链表
 * @param head: 链表头
 * @return: 排序后的链表头
 * 
 * 算法步骤：
 * 1. 终止条件：空链表或单节点
 * 2. 找中点，分割成两个链表
 * 3. 递归排序左右两部分
 * 4. 合并两个有序链表
 * 
 * 时间复杂度: O(nlogn)
 * 空间复杂度: O(logn)，递归栈
 */
struct ListNode* sortList(struct ListNode* head) {
	struct ListNode *prevMiddle = NULL;
	struct ListNode *middleNode = NULL;
	struct ListNode *leftHalf = NULL;
	struct ListNode *rightHalf = NULL;
	struct ListNode *sortedList = NULL;

	/* 终止条件：空链表或单节点 */
	if (head == NULL || head->next == NULL)
		return head;
	
	/* 找中点 */
	middleNode = findMiddleNode(head, &prevMiddle);

	/* 分割链表 */
	leftHalf = head;
	prevMiddle->next = NULL;  /* 断开左右两部分 */
	rightHalf = middleNode;

	/* 递归排序左右两部分 */
	leftHalf = sortList(leftHalf);
	rightHalf = sortList(rightHalf);

	/* 合并两个有序链表 */
	sortedList = mergeTwoSortedLists(leftHalf, rightHalf);

	return sortedList;
}

/*
 * ==================== 算法详解 ====================
 * 
 * 核心思想：归并排序（Merge Sort）
 * 
 * 归并排序三步骤：
 * 1. 分割：找中点，分成两半
 * 2. 递归：分别排序左右两部分
 * 3. 合并：合并两个有序链表
 * 
 * 为什么用快慢指针？
 * - 找链表中点：fast走2步，slow走1步
 * - fast到末尾时，slow在中点
 * 
 * 示例：[4,2,1,3]
 * 
 *      [4,2,1,3]
 *       /      \
 *    [4,2]    [1,3]    ← 分割
 *     / \      / \
 *   [4] [2]  [1] [3]   ← 递归到单节点
 *     \ /      \ /
 *    [2,4]    [1,3]    ← 合并
 *       \      /
 *      [1,2,3,4]       ← 最终合并
 */

