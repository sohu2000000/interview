#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "list.h"

/**
 * 创建一个新的链表节点
 * @param data: 节点数据
 * @return: 成功返回新节点指针，失败返回NULL
 */
ListNode *create_node(int data) {
    ListNode *new_node = (ListNode *)malloc(sizeof(ListNode));
    if (new_node == NULL) {
        printf("Error: Failed to allocate memory in %s\n", __func__);
		return NULL;
	}

    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

/**
 * 打印链表内容
 * @param head: 链表头指针
 */
void print_list(ListNode *head) {
    if (head == NULL) {
        printf("Empty list\n");
		return;
	}

    printf("List: ");
    ListNode *current = head;
    while (current != NULL) {
        printf("%d", current->data);
        if (current->next != NULL) {
            printf(" -> ");
        }
        current = current->next;
    }
    printf(" -> NULL\n");
}

/**
 * 从数组创建链表
 * @param array: 数据数组
 * @param size: 数组大小
 * @return: 成功返回链表头指针，失败返回NULL
 */
ListNode *create_list_from_array(int *array, int size) {
    if (array == NULL || size <= 0) {
        printf("Error: Invalid parameters in %s\n", __func__);
		return NULL;
	}

    // 创建第一个节点
    ListNode *head = create_node(array[0]);
    if (head == NULL) {
        return NULL;
    }

    ListNode *current = head;
    
    // 创建剩余节点
    for (int i = 1; i < size; i++) {
        ListNode *new_node = create_node(array[i]);
        if (new_node == NULL) {
            return head; // 返回部分链表
        }
        
        current->next = new_node;
        current = new_node;
    }

	return head;
}

/**
 * 获取链表长度
 * @param head: 链表头指针
 * @return: 链表长度
 */
int get_list_length(ListNode *head) {
    int length = 0;
    ListNode *current = head;

    while (current != NULL) {
        length++;
        current = current->next;
    }

    return length;
}

/**
 * 根据值搜索节点
 * @param head: 链表头指针
 * @param value: 要搜索的值
 * @return: 找到返回节点指针，否则返回NULL
 */
ListNode *search_node_by_value(ListNode *head, int value) {
    ListNode *current = head;

    while (current != NULL) {
        if (current->data == value) {
            return current;
        }
        current = current->next;
    }

    return NULL; // 未找到
}

/**
 * 根据位置查找节点
 * @param head: 链表头指针
 * @param position: 位置索引（从0开始）
 * @return: 找到返回节点指针，否则返回NULL
 */
ListNode *find_node_by_position(ListNode *head, int position) {
    if (head == NULL || position < 0) {
		return NULL;
	}

    ListNode *current = head;
    int current_position = 0;

    while (current != NULL) {
        if (current_position == position) {
            return current;
        }
        current = current->next;
        current_position++;
    }

    return NULL; // 位置超出范围
}

/**
 * 在指定位置插入节点
 * @param head: 链表头指针的地址
 * @param position: 插入位置（从0开始）
 * @param data: 节点数据
 * @return: 成功返回新节点指针，失败返回NULL
 */
ListNode *insert_node(ListNode **head, int position, int data) {
    if (head == NULL || position < 0) {
        printf("Error: Invalid parameters in %s\n", __func__);
        return NULL;
    }

    ListNode *new_node = create_node(data);
    if (new_node == NULL) {
		return NULL;
	}

    // 插入到头部
    if (position == 0) {
        new_node->next = *head;
        *head = new_node;
        return new_node;
    }

    // 找到插入位置的前一个节点
    ListNode *prev = find_node_by_position(*head, position - 1);
    if (prev == NULL) {
        printf("Error: Position %d is out of range\n", position);
        free(new_node);
		return NULL;
	}

    // 插入节点
    new_node->next = prev->next;
    prev->next = new_node;

    return new_node;
}

/**
 * 删除指定位置的节点
 * @param head: 链表头指针的地址
 * @param position: 删除位置（从0开始）
 * @return: 被删除的节点指针，失败返回NULL
 */
ListNode *remove_node(ListNode **head, int position) {
    if (head == NULL || *head == NULL || position < 0) {
        printf("Error: Invalid parameters in %s\n", __func__);
		return NULL;
	}

    ListNode *removed_node;

    // 删除头节点
    if (position == 0) {
        removed_node = *head;
        *head = (*head)->next;
        removed_node->next = NULL;
        return removed_node;
    }

    // 找到要删除节点的前一个节点
    ListNode *prev = find_node_by_position(*head, position - 1);
    if (prev == NULL || prev->next == NULL) {
        printf("Error: Position %d is out of range\n", position);
		return NULL;
	}

    // 删除节点
    removed_node = prev->next;
    prev->next = removed_node->next;
    removed_node->next = NULL;

    return removed_node;
}

/**
 * 反转链表
 * @param head: 链表头指针的地址
 * @return: 成功返回SUCCESS，失败返回错误码
 */
int reverse_list(ListNode **head) {
    if (head == NULL || *head == NULL) {
        return SUCCESS; // 空链表无需反转
    }

    ListNode *prev = NULL;
    ListNode *current = *head;
    ListNode *next = NULL;

    while (current != NULL) {
        next = current->next;  // 保存下一个节点
        current->next = prev;  // 反转当前节点的指针
        prev = current;        // 移动prev指针
        current = next;        // 移动current指针
    }

    *head = prev; // 更新头指针
	return SUCCESS;
}

/**
 * 查找链表中间节点（普通方法：两次遍历）
 * @param head: 链表头指针
 * @return: 中间节点指针
 */
ListNode *find_middle_node(ListNode *head) {
    if (head == NULL) {
		return NULL;
	}	

    int length = get_list_length(head);
    int middle_position = (length / 2) - 1;

    if (middle_position <= 0) {
		return head;
	}
	
    return find_node_by_position(head, middle_position);
}

/**
 * 查找链表中间节点（快慢指针法）
 * @param head: 链表头指针
 * @return: 中间节点指针
 */
ListNode *find_middle_node_fast(ListNode *head) {
    if (head == NULL) {
		return NULL;
	}	

    ListNode *slow = head;
    ListNode *fast = head;

    // 快指针每次走两步，慢指针每次走一步
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow; // 慢指针指向中间节点
}

/**
 * 检测链表是否有环
 * @param head: 链表头指针
 * @return: 有环返回true，无环返回false
 */
bool is_list_circular(ListNode *head) {
    if (head == NULL) {
		return false;
	}	

    ListNode *slow = head;
    ListNode *fast = head;

    // 使用快慢指针检测环
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
        
        if (slow == fast) {
            return true; // 发现环
        }
    }

    return false; // 无环
}

/**
 * 链表插入排序
 * @param head: 链表头指针的地址
 * @return: 成功返回SUCCESS，失败返回错误码
 */
int sort_list_insertion(ListNode **head) {
    if (head == NULL || *head == NULL || (*head)->next == NULL) {
        return SUCCESS; // 空链表或单节点无需排序
    }

    ListNode *sorted_head = NULL;  // 已排序部分的头指针
    ListNode *current = *head;     // 当前处理的节点

    while (current != NULL) {
        ListNode *next_node = current->next;  // 保存下一个节点

        // 将current节点插入到已排序部分的正确位置
        if (sorted_head == NULL || current->data <= sorted_head->data) {
            // 插入到已排序部分的头部
            current->next = sorted_head;
            sorted_head = current;
	} else {		
            // 在已排序部分中找到正确的插入位置
            ListNode *search_ptr = sorted_head;
            while (search_ptr->next != NULL && search_ptr->next->data < current->data) {
                search_ptr = search_ptr->next;
            }
            
            // 插入current节点
            current->next = search_ptr->next;
            search_ptr->next = current;
        }

        current = next_node;  // 处理下一个节点
    }

    *head = sorted_head;  // 更新头指针
	return SUCCESS;
}

/**
 * 合并两个有序链表
 * @param head1: 第一个链表头指针的地址
 * @param head2: 第二个链表头指针
 * @return: 成功返回SUCCESS，失败返回错误码
 */
int merge_sorted_lists(ListNode **head1, ListNode *head2) {
    if (head1 == NULL) {
        printf("Error: Invalid parameters in %s\n", __func__);
        return PARAM_ERROR;
    }

    if (*head1 == NULL) {
        *head1 = head2;
	return SUCCESS;
}

    if (head2 == NULL) {
	return SUCCESS;
}

    ListNode dummy;  // 虚拟头节点
    ListNode *tail = &dummy;
    ListNode *list1 = *head1;
    ListNode *list2 = head2;

    // 合并两个有序链表
    while (list1 != NULL && list2 != NULL) {
        if (list1->data <= list2->data) {
            tail->next = list1;
            list1 = list1->next;
        } else {
            tail->next = list2;
            list2 = list2->next;
        }
		tail = tail->next;
	}

    // 连接剩余的节点
    if (list1 != NULL) {
        tail->next = list1;
    } else {
        tail->next = list2;
    }

    *head1 = dummy.next;
    return SUCCESS;
}

/**
 * 约瑟夫环问题（约瑟夫斯问题）
 * @param head: 环形链表头指针
 * @param result: 存储结果的数组
 * @param length: 链表长度
 * @param step: 步长（每隔step个人淘汰一个）
 * @return: 成功返回SUCCESS，失败返回错误码
 */
int josephus_problem(ListNode *head, int *result, int length, int step) {
    if (head == NULL || result == NULL || length <= 0 || step <= 0) {
        printf("Error: Invalid parameters in %s\n", __func__);
        return PARAM_ERROR;
    }

    // 构建环形链表
    ListNode *tail = head;
    while (tail->next != NULL) {
		tail = tail->next;
	}
    tail->next = head;  // 形成环

    ListNode *current = head;
    ListNode *prev = tail;
    int result_index = 0;

    // 开始约瑟夫环游戏
    while (current->next != current) {  // 当只剩一个节点时停止
        // 数step步
        for (int i = 1; i < step; i++) {
            prev = current;
            current = current->next;
        }

        // 记录被淘汰的节点
        result[result_index++] = current->data;

        // 删除当前节点
        prev->next = current->next;
        ListNode *to_delete = current;
        current = current->next;
        free(to_delete);
    }

    // 记录最后一个存活的节点
    result[result_index] = current->data;
    free(current);

    return SUCCESS;
}

/**
 * 释放链表内存
 * @param head: 链表头指针的地址
 */
void free_list(ListNode **head) {
    if (head == NULL || *head == NULL) {
	return;
}

    ListNode *current = *head;
    ListNode *next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    *head = NULL;
}

/**
 * 测试函数 - 基本操作
 */
void test_basic_operations(void) {
    printf("=== Testing Basic List Operations ===\n");

    // 创建链表
    int array[] = {1, 2, 3, 4, 5};
    ListNode *head = create_list_from_array(array, 5);
    
    printf("Original list:\n");
    print_list(head);

    // 测试长度
    printf("List length: %d\n", get_list_length(head));

    // 测试搜索
    ListNode *found = search_node_by_value(head, 3);
    if (found) {
        printf("Found node with value 3\n");
    }

    // 测试插入
    insert_node(&head, 0, 0);  // 插入到头部
    insert_node(&head, 3, 10); // 插入到中间
    printf("After insertions:\n");
    print_list(head);

    // 测试删除
    ListNode *removed = remove_node(&head, 0);
    if (removed) {
        printf("Removed node with value: %d\n", removed->data);
        free(removed);
    }
    printf("After removal:\n");
    print_list(head);

    // 测试反转
    reverse_list(&head);
    printf("After reversal:\n");
    print_list(head);

    // 测试中间节点
    ListNode *middle = find_middle_node_fast(head);
    if (middle) {
        printf("Middle node value: %d\n", middle->data);
    }

    // 测试环检测
    printf("Is circular: %s\n", is_list_circular(head) ? "Yes" : "No");

    // 清理内存
    free_list(&head);
    printf("List freed\n");
}

/**
 * 测试函数 - 高级操作
 */
void test_advanced_operations(void) {
    printf("\n=== Testing Advanced List Operations ===\n");

    // 测试插入排序
    int unsorted_array[] = {4, 2, 7, 1, 9, 3};
    ListNode *unsorted_head = create_list_from_array(unsorted_array, 6);
    printf("Before insertion sort:\n");
    print_list(unsorted_head);
    
    sort_list_insertion(&unsorted_head);
    printf("After insertion sort:\n");
    print_list(unsorted_head);

    // 测试合并有序链表
    int array1[] = {1, 3, 5, 7};
    int array2[] = {2, 4, 6, 8, 10};
    ListNode *list1 = create_list_from_array(array1, 4);
    ListNode *list2 = create_list_from_array(array2, 5);
    
    printf("List 1:\n");
    print_list(list1);
    printf("List 2:\n");
    print_list(list2);
    
    merge_sorted_lists(&list1, list2);
    printf("After merging:\n");
    print_list(list1);

    // 测试约瑟夫环问题
    int josephus_array[] = {1, 2, 3, 4, 5};
    ListNode *josephus_head = create_list_from_array(josephus_array, 5);
    int result[5];
    
    printf("Josephus problem with step 3:\n");
    printf("Original circle:\n");
    print_list(josephus_head);
    
    josephus_problem(josephus_head, result, 5, 3);
    printf("Elimination order: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", result[i]);
    }
    printf("\n");

    // 清理内存
    free_list(&unsorted_head);
    free_list(&list1);
    // josephus_head 已在 josephus_problem 中释放
}

/**
 * 主函数 - 测试所有功能
 */
// main函数已移到main.c中，此处注释
/*
int main(void) {
    test_basic_operations();
    test_advanced_operations();
    return 0;
}
*/