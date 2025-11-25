/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */

/**
 * 删除链表的倒数第N个节点 - 双指针（快慢指针）
 * 
 * 核心思想：
 * - 快指针先走n+1步
 * - 快慢指针一起走，直到快指针到NULL
 * - 慢指针此时指向要删除节点的前一个
 * - 删除操作：slow->next = slow->next->next
 * 
 * 使用哑节点简化边界处理（删除头节点）
 * 
 * 时间复杂度：O(L) L是链表长度
 * 空间复杂度：O(1)
 */
struct ListNode* removeNthFromEnd(struct ListNode* head, int n) {
	struct ListNode *dummy = (struct ListNode*)malloc(sizeof(struct ListNode));
	struct ListNode *slow = NULL, *fast = NULL;
	struct ListNode *result = NULL;
	int i;

	// 哑节点，简化边界处理
	dummy->next = head;
	slow = fast = dummy;

	// 快指针先走n+1步
	for (i = 0; i < n + 1; i++) {
		fast = fast->next;
	}

	// 快慢指针一起移动，直到快指针到末尾
	while (fast != NULL) {
		slow = slow->next;
		fast = fast->next;
	}

	// 此时slow指向要删除节点的前一个
	// 删除操作：跳过slow的下一个节点
	slow->next = slow->next->next;
	
	// 保存结果再释放dummy
	result = dummy->next;
	free(dummy);

	return result;
}