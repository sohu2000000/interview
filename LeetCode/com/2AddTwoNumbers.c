/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */

/**
 * 两数相加 - 链表模拟加法
 * 
 * 核心思想：
 * - 模拟小学加法：从低位到高位逐位相加
 * - 维护进位carry
 * - 同时遍历两个链表
 * 
 * 时间复杂度：O(max(m,n))
 * 空间复杂度：O(max(m,n))
 */
struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2) {
	struct ListNode *p1 = l1, *p2 = l2;
	struct ListNode *resultHead = NULL, *currentNode = NULL, *newNode = NULL;
	int carry = 0, digitSum = 0, digit = 0;

	// 遍历两个链表
	while (p1 != NULL || p2 != NULL) {
		// 计算当前位的和（包括进位）
		digitSum = carry;
		if (p1 != NULL) {
			digitSum += p1->val;
			p1 = p1->next;
		}
		if (p2 != NULL) {
			digitSum += p2->val;
			p2 = p2->next;
		}

		// 计算当前位数字和进位
		digit = digitSum % 10;
		carry = digitSum / 10;

		// 创建新节点
		newNode = (struct ListNode *)malloc(sizeof(struct ListNode));
		newNode->val = digit;
		newNode->next = NULL;

		// 添加到结果链表
		if (resultHead == NULL) {
			resultHead = newNode;
			currentNode = newNode;
		} else {
			currentNode->next = newNode;
			currentNode = newNode;
		}
	}

	// 处理最后的进位
	if (carry > 0) {
		newNode = (struct ListNode *)malloc(sizeof(struct ListNode));
		newNode->val = carry;
		newNode->next = NULL;
		currentNode->next = newNode;
	}

	return resultHead;
}