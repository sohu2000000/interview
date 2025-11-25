/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */

/**
 * 删除排序链表中的重复元素II - 删除所有重复节点
 * 
 * 核心思想：
 * - 遇到重复值，整组删除（一个不留）
 * - 使用哑节点简化边界处理
 * - prev指向最后一个确定保留的节点
 * 
 * 算法流程：
 * 1. 检查current和后续节点是否有重复
 * 2. 有重复：跳过整组，prev不动
 * 3. 无重复：保留current，prev前进
 * 
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */
struct ListNode* deleteDuplicates(struct ListNode* head) {
	struct ListNode *dummy = (struct ListNode*)malloc(sizeof(struct ListNode));
	struct ListNode *prev, *current, *nextNode;
	struct ListNode *result;
	int currentValue;

	// 哑节点指向head
	dummy->next = head;
	prev = dummy;
	current = head;

	while (current != NULL) {
		currentValue = current->val;
		nextNode = current->next;

		// 跳过所有值为currentValue的节点
		while (nextNode != NULL && nextNode->val == currentValue) {
			nextNode = nextNode->next;
		}

		// 判断是否有重复
		if (current->next != nextNode) {
			// 有重复：整组删除，prev->next跳到nextNode
			prev->next = nextNode;
		} else {
			// 无重复：保留current，prev前进到current
			prev = current;
		}

		current = nextNode;
	}

	// 保存结果并释放dummy
	result = dummy->next;
	free(dummy);

	return result;
}


