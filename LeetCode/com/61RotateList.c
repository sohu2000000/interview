/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */

/**
 * 旋转链表 - 快慢指针
 * 
 * 核心思想：
 * - 找到新的头节点和尾节点
 * - 旧尾连接到旧头（成环）
 * - 新尾断开（断环）
 * 
 * 关键：
 * - k可能>length，需要取模
 * - 使用快慢指针定位新头和新尾
 * 
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */

// 辅助函数：获取链表长度
int getListLength(struct ListNode* head) {
	int length = 0;
	while (head != NULL) {
		head = head->next;
		length++;
	}
	return length;
}

struct ListNode* rotateRight(struct ListNode* head, int k) {
	struct ListNode *newHead, *oldTail, *newTail;
	struct ListNode *fast, *slow;
	int length, i;

	// 边界检查
	if (head == NULL || head->next == NULL) return head;

	// 获取长度并处理k
	length = getListLength(head);
	k = k % length;
	if (k == 0) return head;  // 不需要旋转

	// 快指针先走k步
	fast = head;
	slow = head;
	for (i = 0; i < k; i++) {
		fast = fast->next;
	}

	// 快慢指针一起走，直到fast到最后一个节点
	while (fast->next != NULL) {
		slow = slow->next;
		fast = fast->next;
	}

	// 此时：
	// slow指向新尾节点（旧的倒数第k+1个）
	// fast指向旧尾节点
	// slow->next指向新头节点
	newHead = slow->next;
	newTail = slow;
	oldTail = fast;

	// 重新连接：旧尾->旧头，新尾->NULL
	oldTail->next = head;
	newTail->next = NULL;

	return newHead;
}