/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */

/**
 * 反转链表II - 反转指定区间[left, right]
 * 
 * 核心思想：
 * - 一次遍历找到关键节点，同时反转区间
 * - 重新连接：before -> 反转后 -> after
 * 
 * 关键节点：
 * - beforeLeft: left前一个节点
 * - leftNode: 反转区间的起始（反转后变尾）
 * - rightNode: 反转区间的结束（反转后变头）
 * - afterRight: right后一个节点
 * 
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */
struct ListNode* reverseBetween(struct ListNode* head, int left, int right) {
	struct ListNode *prev = NULL, *current = head, *nextNode = NULL;
	struct ListNode *beforeLeft = NULL, *afterRight = NULL;
	struct ListNode *leftNode = NULL, *rightNode = NULL;
	int position = 0;

	// 遍历链表：记录关键节点 + 反转指定区间
	while (current != NULL) {
		nextNode = current->next;
		position++;
		
		// 记录left前一个节点
		if (position == left - 1) {
			beforeLeft = current;
		}

		// 记录right后一个节点
		if (position == right + 1) {
			afterRight = current;
		}

		// 记录left节点（反转后会变成尾部）
		if (position == left) {
			leftNode = current;
		}

		// 记录right节点（反转后会变成头部）
		if (position == right) {
			rightNode = current;
		}

		// 在反转区间内，反转指针
		if (position >= left && position <= right) {
			current->next = prev;
		}

		prev = current;
		current = nextNode;
	}

	// ===== 重新连接三段 =====
	// before -> 反转后的头
	if (beforeLeft != NULL) {
		beforeLeft->next = rightNode;
	}

	// 反转后的尾 -> after
	if (leftNode != NULL) {
		leftNode->next = afterRight;
	}

	// 如果left=1，头节点变了，返回新头
	return (left == 1) ? rightNode : head;
}

