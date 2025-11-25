/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */

/**
 * 分隔链表 - 双链表法
 * 
 * 核心思想：
 * - 将链表分成两个子链表：小于x的和大于等于x的
 * - 使用两个哑节点分别管理两个子链表
 * - 最后连接两个子链表
 * 
 * 关键：必须断开greater的尾部，避免形成环
 * 
 * 时间复杂度：O(n)
 * 空间复杂度：O(1) - 只用了两个哑节点
 */
struct ListNode* partition(struct ListNode* head, int x) {
	// 两个哑节点
	struct ListNode *lessDummy = (struct ListNode*)malloc(sizeof(struct ListNode));
	struct ListNode *greaterDummy = (struct ListNode*)malloc(sizeof(struct ListNode));
	lessDummy->next = NULL;
	greaterDummy->next = NULL;
	
	// 两个尾指针
	struct ListNode *lessTail = lessDummy;
	struct ListNode *greaterTail = greaterDummy;
	struct ListNode *current = head;

	// 遍历原链表，分配到两个子链表
	while (current != NULL) {
		if (current->val < x) {
			// 小于x：添加到less链表
			lessTail->next = current;
			lessTail = lessTail->next;
		} else {
			// 大于等于x：添加到greater链表
			greaterTail->next = current;
			greaterTail = greaterTail->next;
		}
		current = current->next;
	}

	// 关键：断开greater尾部，避免形成环
	greaterTail->next = NULL;
	
	// 连接两个链表：less链表 + greater链表
	lessTail->next = greaterDummy->next;
	
	// 保存结果并释放哑节点
	struct ListNode *result = lessDummy->next;
	free(lessDummy);
	free(greaterDummy);

	return result;
}

