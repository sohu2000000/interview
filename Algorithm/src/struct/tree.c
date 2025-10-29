#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "stack.h"
#include "queue.h"

/**
 * 创建树节点 - 分配并初始化一个新的树节点
 * @param node_data: 节点数据
 * @return: 成功返回节点指针，失败返回NULL
 */
st_trNode *create_tree_node_with_value(int node_data) {
	st_trNode *new_node = NULL;

	new_node = (st_trNode *)malloc(sizeof(st_trNode));
	if (new_node == NULL) {
		printf("Error: create_binary_tree_node allocation failed\n");
		return NULL;
	}

	new_node->data = node_data;
	new_node->tag = 0;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->parent = NULL;

	return new_node;
}

/**
 * 创建二叉树 - 分配并初始化一个新的树结构
 * @param tree_ptr: 树指针的指针
 * @return: 成功返回SUCCESS，失败返回ALLOC_ERR
 */
int create_tree_structure_empty(st_tree **tree_ptr) {
	st_tree *new_tree = NULL;

	new_tree = (st_tree *)malloc(sizeof(st_tree));
	if (new_tree == NULL) {
		printf("Error: create_binary_search_tree allocation failed\n");
		return ALLOC_ERR;
	}

	new_tree->root = NULL;
	*tree_ptr = new_tree;

	return SUCCESS;
}

int insertBSTNode(st_trNode ** proot, st_trNode * node){
	if(NULL == proot || NULL == node){
		printf("insertBSTNode param error\n");
		return PARAM_ERR;
	}

	st_trNode * root = *proot;	
	st_trNode * left = NULL, * right = NULL, * p = NULL;;

	/* BST的首个节点 */
	if(NULL == root) {
		root = node;
#ifdef DEBUG		
		printf("[DEBUG] 插入首个节点: root = %d\n", root->data);		
#endif
		goto out;
	}

	/*递归结束条件：叶子节点*/
	if(NULL == root->left && NULL == root->right) {
#ifdef DEBUG
		printf("[DEBUG] 到达叶子节点: root = %d, 插入值 = %d\n", root->data, node->data);
#endif
		if(node->data < root->data) {
			root->left = node;
			node->parent = root;
#ifdef DEBUG		
			printf("[DEBUG] 作为左孩子插入: left = %d, parent = %d\n", root->left->data, node->parent->data);	
#endif
		} else {
			root->right = node;
			node->parent = root;
#ifdef DEBUG	
			printf("[DEBUG] 作为右孩子插入: right = %d, parent = %d\n", root->right->data, node->parent->data);	
#endif
		}
		goto out;
	}
	
	left= root->left;
	right = root->right;

	/*向左子树插入*/
	if(node->data < root->data) {
		if(NULL == left) {
#ifdef DEBUG
			printf("[DEBUG] 左子树为空，直接插入: 父节点 = %d, 新节点 = %d\n", root->data, node->data);	
#endif
			root->left = node;
			node->parent = root;
		} else{
#ifdef DEBUG			
			printf("[DEBUG] 递归插入左子树: 当前根 = %d, 左孩子 = %d, 插入值 = %d\n", root->data, left->data, node->data);	
#endif
			insertBSTNode(&left, node);			
			/*左子树根可能发生变化*/
			root->left = left;
			left->parent = root;
		}
	} else { /*向右子树插入*/
		if(NULL == right) {
#ifdef DEBUG			
			printf("[DEBUG] 右子树为空，直接插入: 父节点 = %d, 新节点 = %d\n", root->data, node->data);
#endif
			root->right = node;
			node->parent = root;
		} else {
#ifdef DEBUG	
			printf("[DEBUG] 递归插入右子树: 当前根 = %d, 右孩子 = %d, 插入值 = %d\n", root->data, right->data, node->data);	
#endif
			insertBSTNode(&right, node);
			/*右子树根可能发生变化*/
			root->right = right;
			right->parent = root;
		}		
	}
	
out:
	* proot = root;
	return SUCCESS;
	
}

/**
 * 前序遍历（递归） - 按"根-左-右"顺序遍历二叉树
 * @param tree_root: 树的根节点
 * @return: 成功返回SUCCESS
 * 
 * 顺序：根节点 → 左子树 → 右子树
 */
int traverse_tree_preorder_recursive(st_trNode *tree_root) {
	// 递归结束条件
	if (tree_root == NULL) {
		return SUCCESS;
	}

	// 1. 访问根节点
	printf(" %d ", tree_root->data);

	// 2. 递归遍历左子树
	if (tree_root->left != NULL)
		traverse_tree_preorder_recursive(tree_root->left);

	// 3. 递归遍历右子树
	if (tree_root->right != NULL)
		traverse_tree_preorder_recursive(tree_root->right);

	return SUCCESS;
}


/**
 * 中序遍历（递归） - 按"左-根-右"顺序遍历二叉树
 * @param tree_root: 树的根节点
 * @return: 成功返回SUCCESS
 * 
 * 顺序：左子树 → 根节点 → 右子树
 * 重要：对于BST，中序遍历结果是升序！
 */
int traverse_tree_inorder_recursive(st_trNode *tree_root) {
	// 递归结束条件
	if (tree_root == NULL) {
		return SUCCESS;
	}

	// 1. 递归遍历左子树
	if (tree_root->left != NULL)
		traverse_tree_inorder_recursive(tree_root->left);

	// 2. 访问根节点
	printf(" %d ", tree_root->data);

	// 3. 递归遍历右子树
	if (tree_root->right != NULL)
		traverse_tree_inorder_recursive(tree_root->right);

	return SUCCESS;
}

/**
 * 后序遍历（递归） - 按"左-右-根"顺序遍历二叉树
 * @param tree_root: 树的根节点
 * @return: 成功返回SUCCESS
 * 
 * 顺序：左子树 → 右子树 → 根节点
 * 用途：常用于删除树（先删孩子再删父母）
 */
int traverse_tree_postorder_recursive(st_trNode *tree_root) {
	// 递归结束条件
	if (tree_root == NULL) {
		return SUCCESS;
	}

	// 1. 递归遍历左子树
	if (tree_root->left != NULL)
		traverse_tree_postorder_recursive(tree_root->left);

	// 2. 递归遍历右子树
	if (tree_root->right != NULL)
		traverse_tree_postorder_recursive(tree_root->right);

	// 3. 访问根节点
	printf(" %d ", tree_root->data);

	return SUCCESS;
}



int dumpTree(st_tree * tree){

	st_queue * queue;

	/*递归结束条件*/
	if(NULL == tree){
		return SUCCESS;
	}
	
#if 0
	printf("========= Dump Tree PreOder: %p ===========\n\t", tree);
	TraverseTreePreOrder(tree->root);
	printf("\n===================================\n");


	printf("========= Dump Tree PreOder UnRec: %p ===========\n\t", tree);
	TraverseTreePreOrderUnrec(tree->root);
	printf("\n===================================\n");

#endif

#if 1
	printf("========= Dump Tree InOder: %p ===========\n\t", tree);
	TraverseTreeInOrder(tree->root);
	printf("\n===================================\n");

	printf("========= Dump Tree InOderUnrec: %p ===========\n\t", tree);
	TraverseTreeInOrderUnrec(tree->root);
	printf("\n===================================\n");	
#endif

#if 0
	printf("========= Dump Tree PostOrder: %p ===========\n\t", tree);
	TraverseTreePostOrder(tree->root);
	printf("\n===================================\n");

	printf("========= Dump Tree PostOrder unRec: %p ===========\n\t", tree);
	TraverseTreePostOrderUnrec(tree->root);
	printf("\n===================================\n");	
#endif


#if 0
	queue = createQueue();
	printf("========= Dump Tree LevelOrder: %p ===========\n\t", tree);
	TraverseTreeLevelOrder(tree->root, queue);
	printf("\n===================================\n");
	free(queue);
	queue = NULL;
	
#endif


	return SUCCESS;
}

/**
 * 搜索BST节点 - 在二叉搜索树中查找指定值的节点
 * @param tree_root: 树的根节点
 * @param target_value: 要查找的值
 * @return: 找到返回节点指针，否则返回NULL
 * 
 * 算法：递归查找
 * - 值等于根节点：返回根节点
 * - 值小于根节点：递归搜索左子树
 * - 值大于根节点：递归搜索右子树
 */
st_trNode *search_tree_node_by_value(st_trNode *tree_root, int target_value) {
	if (tree_root == NULL) {
		return NULL;
	}

	st_trNode *found_node = NULL;
	st_trNode *left_child = tree_root->left;
	st_trNode *right_child = tree_root->right;
	
	// 找到目标节点
	if (target_value == tree_root->data) {
		return tree_root;
	}

	// 叶子节点，未找到
	if (left_child == NULL && right_child == NULL) {
		return NULL;
	}

	// 在左子树中搜索
	if (target_value < tree_root->data) {
		if (left_child == NULL) {
			return NULL;
		} else {
			found_node = search_tree_node_by_value(left_child, target_value);
		}
	} else {
		// 在右子树中搜索
		if (right_child == NULL) {
			return NULL;
		} else {
			found_node = search_tree_node_by_value(right_child, target_value);
		}		
	}

	return found_node;
}


/**
 * 获取最左节点 - 找到以指定节点为根的子树中最左边的节点
 * @param subtree_root: 子树根节点
 * @return: 返回最左节点指针
 * 
 * 功能：一直向左遍历直到没有左孩子
 * 用途：在BST中找最小值节点
 */
st_trNode *find_tree_node_leftmost(st_trNode *subtree_root) {
	if (subtree_root == NULL) {
		return NULL;
	}

	st_trNode *current_node = NULL;

	// 如果没有左孩子，当前节点就是最左节点
	if (subtree_root->left == NULL) {
		return subtree_root;
	}

	// 一直向左遍历
	current_node = subtree_root->left;
	while (current_node->left != NULL) {
		current_node = current_node->left;
	}

	return current_node;
}

/**
 * 辅助函数：将replacement连接到parent的指定位置
 */
static void connect_replacement_to_parent(st_trNode *parent, st_trNode *old_node, st_trNode *replacement) {
	if (parent == NULL) {
		return;
	}
	
	// 判断old_node是父节点的左孩子还是右孩子
	if (old_node->data < parent->data) {
		parent->left = replacement;
	} else {
		parent->right = replacement;
	}
	
	// 更新replacement的父指针
	if (replacement != NULL) {
		replacement->parent = parent;
	}
}

/**
 * 删除BST节点（简化版）- 从二叉搜索树中删除指定值的节点
 * @param tree_root_ptr: 树根节点指针的指针
 * @param target_value: 要删除的值
 * @return: 成功返回SUCCESS，失败返回PARAM_ERR
 * 
 * BST删除的4种情况（简单易懂）：
 * 1. 叶子节点（无孩子）      → 直接删除
 * 2. 只有左孩子              → 用左孩子替换
 * 3. 只有右孩子              → 用右孩子替换
 * 4. 有两个孩子              → 用右子树最小值替换
 */
int remove_tree_node_by_value(st_trNode **tree_root_ptr, int target_value) {
	if (tree_root_ptr == NULL) {
		printf("Error: remove_tree_node_by_value parameter error\n");
		return PARAM_ERR;
	}

	st_trNode *root = *tree_root_ptr;
	st_trNode *node_to_delete = NULL;
	st_trNode *left_child = NULL, *right_child = NULL, *parent_node = NULL;
	st_trNode *replacement_node = NULL;  // 替换节点
	st_trNode *right_minimum = NULL;     // 右子树最小节点

	// 步骤1: 查找要删除的节点
	node_to_delete = search_tree_node_by_value(root, target_value);
	if (node_to_delete == NULL) {
		printf("Error: remove_tree_node_by_value cannot find %d\n", target_value);
		return PARAM_ERR;
	}

	left_child = node_to_delete->left;
	right_child = node_to_delete->right;
	parent_node = node_to_delete->parent;

	// 步骤2: 根据孩子情况确定替换节点
	if (left_child == NULL && right_child == NULL) {
		// 情况1: 叶子节点（无孩子） - 直接删除
		replacement_node = NULL;
		
	} else if (left_child != NULL && right_child == NULL) {
		// 情况2: 只有左孩子 - 用左孩子替换
		replacement_node = left_child;
		
	} else if (left_child == NULL && right_child != NULL) {
		// 情况3: 只有右孩子 - 用右孩子替换
		replacement_node = right_child;
		
		} else {
		// 情况4: 有两个孩子 - 用右子树最小值替换
		right_minimum = find_tree_node_leftmost(right_child);
		
		// 将左子树挂到右子树最左节点的左边
		right_minimum->left = left_child;
		left_child->parent = right_minimum;
		
		// 用右子树根节点作为替换节点
		replacement_node = right_child;
	}

	// 步骤3: 连接替换节点到正确位置
	if (parent_node != NULL) {
		// 非根节点：连接到父节点
		connect_replacement_to_parent(parent_node, node_to_delete, replacement_node);
	} else {
		// 根节点：更新根指针
		root = replacement_node;
		if (root != NULL) {
			root->parent = NULL;
		}
		*tree_root_ptr = root;
	}

	// 步骤4: 释放被删除的节点
	free(node_to_delete);

	return SUCCESS;
}


/**
 * 中序遍历（迭代版） - 使用栈实现的非递归中序遍历
 * @param tree_root: 树的根节点
 * @return: 成功返回SUCCESS
 * 
 * ============================================================================
 * 核心思想：用栈模拟递归
 * ============================================================================
 * 
 * 递归的本质：
 * - 递归调用时，系统用"调用栈"保存现场（参数、局部变量、返回地址）
 * - 递归返回时，从栈中恢复现场
 * - 我们可以用显式栈来模拟这个过程！
 * 
 * 递归版本（系统自动用栈）：
 *   traverse_inorder(node) {
 *       traverse_inorder(node->left);   // 系统压栈保存"5的现场"
 *       printf(node->data);              // 左子树完成后打印
 *       traverse_inorder(node->right);
 *   }
 * 
 * 迭代版本（我们手动用栈）：
 *   while (current != NULL or stack非空) {
 *       一路向左压栈;        // 模拟"递归到左子树"
 *       弹栈并打印;          // 模拟"左子树完成，处理根"
 *       转向右子树;          // 模拟"递归到右子树"
 *   }
 * 
 * ============================================================================
 * 执行过程示例（树: 5(3(2,4),7)）
 * ============================================================================
 * 
 * 第1轮：current=5, stack=[]
 *   内循环（一路向左）：
 *     5→压栈→左(3) → 3→压栈→左(2) → 2→压栈→左(NULL)
 *     stack: [5, 3, 2] (底→顶)
 *   弹栈2，打印2，转向2的右子树(NULL)
 * 
 * 第2轮：current=NULL, stack=[5,3]
 *   弹栈3，打印3，转向3的右子树(4)
 * 
 * 第3轮：current=4, stack=[5]
 *   内循环：4→压栈→左(NULL)
 *   弹栈4，打印4，转向4的右子树(NULL)
 * 
 * 第4轮：current=NULL, stack=[5]
 *   弹栈5，打印5，转向5的右子树(7)
 * 
 * 第5轮：current=7, stack=[]
 *   内循环：7→压栈→左(NULL)
 *   弹栈7，打印7，转向7的右子树(NULL)
 * 
 * 第6轮：current=NULL, stack=[] → 退出
 * 
 * 输出：2 3 4 5 7 ✓
 * 
 * ============================================================================
 * 关键理解
 * ============================================================================
 * 
 * 1. 外层循环条件：
 *    while (current_node != NULL || !isStackEmpty(stack))
 *    - current != NULL：还有节点要处理
 *    - stack非空：还有"保存的现场"未处理
 * 
 * 2. 一路向左（模拟递归前）：
 *    while (current != NULL) {
 *        pushStack(current);    // 保存现场（等会儿要回来处理）
 *        current = current->left;  // "递归调用"左子树
 *    }
 * 
 * 3. 弹栈并访问（模拟递归返回）：
 *    data = popStack();      // 从栈中取出"保存的现场"
 *    printf(data);           // 左子树已完成，现在处理根节点
 * 
 * 4. 转向右子树（继续递归）：
 *    current = current->right;  // 处理右子树（下一轮会对它"一路向左"）
 * 
 * ============================================================================
 * 为什么需要栈？
 * ============================================================================
 * 
 * 问题：处理完左子树后，怎么"回到"父节点？
 * - 递归：系统栈自动记录
 * - 迭代：我们用显式栈保存
 * 
 * 栈的作用：
 * - 保存"待处理的节点"
 * - 按LIFO（后进先出）顺序处理
 * - 确保"左-根-右"的访问顺序
 * 
 * ============================================================================
 * 算法步骤总结
 * ============================================================================
 */
int traverse_tree_inorder_iterative(st_trNode *tree_root) {
	ListNode *stack_node = NULL;
	st_trNode *current_node = tree_root;
	st_stack *traversal_stack = createStack();
	
	while (current_node != NULL || !isStackEmpty(traversal_stack)) {
		// 步骤1: 一路向左，压栈所有左节点
		while (current_node != NULL) {
			pushStack(traversal_stack, current_node->data);
			current_node = current_node->left;
		}

		if (!isStackEmpty(traversal_stack)) {
			// 步骤2: 弹栈并访问节点
			stack_node = popStack(traversal_stack);			
			printf(" %d ", stack_node->data);

			// 步骤3: 转向右子树
			// 注意：因为栈只存data，需要重新查找节点
			current_node = search_tree_node_by_value(tree_root, stack_node->data); 
			if (stack_node != NULL) {
				free(stack_node);
				stack_node = NULL;
			}
			current_node = current_node->right;
		}
	}

	return SUCCESS;
}

/**
 * 前序遍历（迭代版） - 使用栈实现的非递归前序遍历
 * @param tree_root: 树的根节点
 * @return: 成功返回SUCCESS
 * 
 * 算法步骤：
 * 1. 访问节点并压栈
 * 2. 一路向左
 * 3. 弹栈，转向右子树
 */
int traverse_tree_preorder_iterative(st_trNode *tree_root) {
	ListNode *stack_node = NULL;
	st_trNode *current_node = tree_root;
	st_stack *traversal_stack = createStack();
	
	while (current_node != NULL || !isStackEmpty(traversal_stack)) {
		while (current_node != NULL) {
			// 1. 先访问节点（前序特点）
			printf(" %d ", current_node->data);

			// 2. 压栈并向左遍历
			pushStack(traversal_stack, current_node->data);
			current_node = current_node->left;
		}

		if (!isStackEmpty(traversal_stack)) {
			stack_node = popStack(traversal_stack);			

			// 3. 转向右子树
			current_node = search_tree_node_by_value(tree_root, stack_node->data); 
			if (stack_node != NULL) {
				free(stack_node);
				stack_node = NULL;
			}
			current_node = current_node->right;
		}
	}
	
	return SUCCESS;
}

/**
 * 后序遍历（迭代版） - 使用栈和标志位实现的非递归后序遍历
 * @param tree_root: 树的根节点
 * @return: 成功返回SUCCESS
 * 
 * ============================================================================
 * 为什么后序遍历需要tag标志位？
 * ============================================================================
 * 
 * 关键区别：节点需要被访问的次数
 * 
 * 【前序遍历】根→左→右
 *   - 第1次遇到节点：立即打印 ✓
 *   - 只需访问1次
 *   - 不需要tag
 * 
 * 【中序遍历】左→根→右
 *   - 第1次遇到节点：压栈（等左子树完成）
 *   - 第2次遇到节点：弹栈打印 ✓
 *   - 只需访问1次就能确定打印时机
 *   - 不需要tag
 * 
 * 【后序遍历】左→右→根 ⭐
 *   - 第1次遇到节点：压栈（等左子树完成）
 *   - 第2次遇到节点：还不能打印！要先处理右子树
 *   - 第3次遇到节点：左右都完成了，才打印 ✓
 *   - 需要区分"第几次访问" → 必须用tag！
 * 
 * ============================================================================
 * tag标志位的作用
 * ============================================================================
 * 
 * tag = 0：第1次访问（还未处理右子树）
 *   - 动作：改tag=1，转向右子树
 *   
 * tag = 1：第2次访问（右子树已完成）
 *   - 动作：弹栈并打印节点
 * 
 * ============================================================================
 * 执行过程示例（树: 5(3(2,4),7)）
 * ============================================================================
 * 
 * 以节点5为例，说明为什么需要tag：
 * 
 * 第1次遇到5：
 *   - 压栈5，tag=0
 *   - 向左走处理左子树(3)
 *   状态：左子树未处理
 * 
 * 第2次回到5（左子树处理完）：
 *   - 检查tag=0，说明还没处理右子树
 *   - 改tag=1
 *   - 向右走处理右子树(7)
 *   状态：左子树完成，右子树未处理
 * 
 * 第3次回到5（右子树也处理完）：
 *   - 检查tag=1，说明左右都完成了
 *   - 弹栈并打印5 ✓
 *   状态：左右子树都完成
 * 
 * ============================================================================
 * 三种遍历的对比
 * ============================================================================
 * 
 * 以节点5为例：
 * 
 * 前序（根→左→右）：
 *   第1次遇到5 → 立即打印5 ✓ → 处理左右子树
 *   不需要tag：第一次就打印完成
 * 
 * 中序（左→根→右）：
 *   第1次遇到5 → 压栈 → 处理左子树
 *   第2次遇到5 → 打印5 ✓ → 处理右子树
 *   不需要tag：弹栈 = 该打印了
 * 
 * 后序（左→右→根）：
 *   第1次遇到5 → 压栈，tag=0 → 处理左子树
 *   第2次遇到5 → tag=0改为1 → 处理右子树  ← 需要tag区分
 *   第3次遇到5 → tag=1，打印5 ✓          ← 需要tag区分
 *   必须tag：要区分是第2次还是第3次
 * 
 * ============================================================================
 * 形象比喻
 * ============================================================================
 * 
 * 想象你要完成三个任务：写作业、复习、睡觉
 * 
 * 前序：先做主任务（写作业✓）→ 复习 → 睡觉
 *   - 一开始就完成主任务
 *   - 不需要记录状态
 * 
 * 中序：复习 → 做主任务（写作业✓）→ 睡觉
 *   - 复习完就知道该写作业了
 *   - 不需要记录状态
 * 
 * 后序：复习 → 睡觉 → 做主任务（写作业✓）
 *   - 第1次：还没复习，去复习
 *   - 第2次：复习完了，但还没睡觉，去睡觉 ← 需要标记
 *   - 第3次：都完成了，现在才能写作业 ← 需要标记
 *   - 必须记录：已经完成了哪些任务
 * 
 * ============================================================================
 * 算法实现
 * ============================================================================
 */
int traverse_tree_postorder_iterative(st_trNode *tree_root) {
	ListNode *stack_node = NULL;
	st_trNode *current_node = tree_root;
	st_stack *traversal_stack = createStack();
	
	while (current_node != NULL || !isStackEmpty(traversal_stack)) {

		while (current_node != NULL) {
			// 1. 一路向左压栈
			pushStack(traversal_stack, current_node->data);
			current_node = current_node->left;
		}

		// 后序遍历需要tag标志位来记录访问状态
		// tag=0: 第一次访问（左子树遍历前）
		// tag=1: 第二次访问（右子树遍历前）
		// 第三次访问时才打印节点

		if (!isStackEmpty(traversal_stack)) {
			stack_node = traversal_stack->top;
			current_node = search_tree_node_by_value(tree_root, stack_node->data); 
			
			if (current_node->tag == 1) {
				// tag=1，说明左右子树都已遍历，可以访问根节点
				stack_node = popStack(traversal_stack);
				if (stack_node != NULL) {
					free(stack_node);
					stack_node = NULL;
				}				
				printf(" %d ", current_node->data);
				current_node->tag = 0;  // 重置tag
				current_node = NULL;    // 该节点已完成
			} else {
				// tag=0，标记为1并转向右子树
				current_node->tag = 1;
				current_node = current_node->right;				
			}
		}
	}
	
	return SUCCESS;
}


/**
 * 获取树的深度（层数）- 计算二叉树的最大深度
 * @param tree_root: 树的根节点
 * @return: 树的深度（层数）
 * 
 * 算法：递归计算
 * - 空树：深度为0
 * - 非空树：深度 = max(左子树深度, 右子树深度) + 1
 * 
 * 示例：
 *       5          深度 = 3
 *      / \
 *     3   7        深度 = 2
 *    / \
 *   2   4          深度 = 1
 * 
 * 计算过程：
 * - 节点2：max(0, 0) + 1 = 1
 * - 节点4：max(0, 0) + 1 = 1
 * - 节点3：max(1, 1) + 1 = 2
 * - 节点7：max(0, 0) + 1 = 1
 * - 节点5：max(2, 1) + 1 = 3
 */
int get_tree_depth(st_trNode *tree_root) {
	// 空树深度为0
	if (tree_root == NULL) {
		return 0;
	}

	// 递归计算左右子树深度
	int left_depth = get_tree_depth(tree_root->left);
	int right_depth = get_tree_depth(tree_root->right);

	// 树的深度 = 较深子树的深度 + 1（根节点）
	int max_depth = (left_depth > right_depth) ? left_depth : right_depth;
	
	return max_depth + 1;
}

/**
 * 层序遍历（广度优先）- 按层从左到右遍历二叉树
 * @param tree_root: 树的根节点
 * @param traversal_queue: 用于辅助遍历的队列
 * @return: 成功返回SUCCESS，失败返回PARAM_ERR
 * 
 * ============================================================================
 * 什么是层序遍历？
 * ============================================================================
 * 
 * 定义：按照从上到下、从左到右的顺序，逐层遍历二叉树的所有节点
 * 
 * 示例树：
 *        5          ← 第1层
 *       / \
 *      3   7       ← 第2层
 *     / \
 *    2   4         ← 第3层
 *   /
 *  1               ← 第4层
 * 
 * 层序遍历结果：5 → 3 → 7 → 2 → 4 → 1
 * 
 * ============================================================================
 * 层序遍历 vs 其他遍历
 * ============================================================================
 * 
 * 前序（根→左→右，用栈，DFS）：5, 3, 2, 1, 4, 7
 * 中序（左→根→右，用栈，DFS）：1, 2, 3, 4, 5, 7
 * 后序（左→右→根，用栈，DFS）：1, 2, 4, 3, 7, 5
 * 层序（逐层，用队列，BFS）：   5, 3, 7, 2, 4, 1  ← 不同！
 * 
 * 关键区别：
 * - 前中后序：深度优先（DFS），用栈
 * - 层序：广度优先（BFS），用队列
 * 
 * ============================================================================
 * 为什么用队列？
 * ============================================================================
 * 
 * 队列的FIFO（先进先出）特性保证：
 * 1. 同层节点按从左到右的顺序出队
 * 2. 下层节点在上层全部处理完后才出队
 * 
 * 队列状态变化示例：
 *   初始：[5]           → 第1层
 *   处理5：[3, 7]       → 第2层
 *   处理3：[7, 2, 4]    → 第2层剩余 + 第3层新增
 *   处理7：[2, 4]       → 第3层
 *   处理2：[4, 1]       → 第3层剩余 + 第4层新增
 *   处理4：[1]          → 第4层
 *   处理1：[]           → 空，结束
 * 
 * ============================================================================
 * 完整执行过程
 * ============================================================================
 * 
 * 步骤1: 根节点入队
 *   queue = [5]
 * 
 * 循环1: 处理第1层
 *   - 出队：5
 *   - 打印：5 ✓
 *   - 5的孩子入队：3, 7
 *   queue = [3, 7]
 * 
 * 循环2: 处理第2层（左）
 *   - 出队：3
 *   - 打印：3 ✓
 *   - 3的孩子入队：2, 4
 *   queue = [7, 2, 4]
 * 
 * 循环3: 处理第2层（右）
 *   - 出队：7
 *   - 打印：7 ✓
 *   - 7的孩子：无
 *   queue = [2, 4]
 * 
 * 循环4: 处理第3层（左）
 *   - 出队：2
 *   - 打印：2 ✓
 *   - 2的孩子入队：1
 *   queue = [4, 1]
 * 
 * 循环5: 处理第3层（右）
 *   - 出队：4
 *   - 打印：4 ✓
 *   - 4的孩子：无
 *   queue = [1]
 * 
 * 循环6: 处理第4层
 *   - 出队：1
 *   - 打印：1 ✓
 *   - 1的孩子：无
 *   queue = []
 * 
 * queue为空，循环结束
 * 输出：5 3 7 2 4 1 ✓
 * 
 * ============================================================================
 * 队列如何保证逐层遍历？
 * ============================================================================
 * 
 * 关键洞察：
 * 
 * queue = [3, 7, 2, 4]
 *          ↑  ↑  ↑  ↑
 *        第2层  第3层
 * 
 * FIFO保证：
 * - 先处理完3和7（第2层）
 * - 3和7的孩子入队后，才轮到2和4（第3层）
 * - 自然形成"逐层"的效果
 * 
 * ============================================================================
 * 应用场景
 * ============================================================================
 * 
 * 1. LeetCode 102: Binary Tree Level Order Traversal
 *    - 输出：[[5], [3,7], [2,4], [1]]
 * 
 * 2. 计算树的宽度
 *    - 找出哪一层节点最多
 * 
 * 3. 找到某一层的所有节点
 *    - 例如：找到第3层的所有节点
 * 
 * 4. 判断完全二叉树
 *    - 层序遍历时检查是否有空洞
 * 
 * ============================================================================
 * 算法实现
 * ============================================================================
 */
int traverse_tree_level_order(st_trNode *tree_root, st_queue *traversal_queue) {
	if (traversal_queue == NULL) {
		printf("Error: traverse_tree_level_order parameter error\n");
		return PARAM_ERR;
	}

	if (tree_root == NULL) {
		return SUCCESS;
	}

	st_trNode *current_tree_node = NULL;
	ListNode *queue_node = NULL;
	int node_value = 0;

	// 根节点入队
	inQueue(traversal_queue, tree_root->data);

	while (!isQueueEmpty(traversal_queue)) {
		// 出队并获取节点值
		queue_node = deQueue(traversal_queue);
		node_value = queue_node->data;
		free(queue_node);
		queue_node = NULL;
		
		// 访问节点
		printf(" %d ", node_value);

		// 查找树节点并将其孩子入队
		current_tree_node = search_tree_node_by_value(tree_root, node_value);
		if (current_tree_node->left != NULL) {
			inQueue(traversal_queue, current_tree_node->left->data);
		}
		if (current_tree_node->right != NULL) {
			inQueue(traversal_queue, current_tree_node->right->data);
		}
	}
	
	return SUCCESS;
}


static int g_lastvalue = 0;

bool isBSTree(st_trNode * root){
	if(NULL == root){
		printf("isBSTree param error\n");
		return PARAM_ERR;
	}

	bool rst = true, rst1 = false;
		
	/*递归结束条件*/
	if(NULL == root){
		return rst;
	}
	
	/*左子树*/
	if(NULL != root->left){
		rst1 = isBSTree(root->left);
		rst = rst & rst1;
	}

	if(g_lastvalue > root->data){
		return false;
	}
	g_lastvalue = root->data;

	/*右子树*/	
	if(NULL != root->right){
		rst1 = isBSTree(root->right);
		rst = rst & rst1;
	}
	
	return rst;
}

void testBSTree(void){

	printf("\n************  testBSTree ************ \n");

	st_trNode * p  = NULL;
	st_trNode * root = NULL;
	st_trNode * mostLeft = NULL;
	bool rst = false;

	gBSTree = NULL;	
	createTree(&gBSTree);

	root = gBSTree->root;

	printf("insert 5\n");
	p = createTreeNode(5);		
	insertBSTNode(&root, p);

	printf("insert 3\n");
	p = createTreeNode(3);
	insertBSTNode(&root, p);

	printf("insert 7\n");
	p = createTreeNode(7);
	insertBSTNode(&root, p);

	printf("insert 2\n");
	p = createTreeNode(2);
	insertBSTNode(&root, p);

	printf("insert 4\n");
	p = createTreeNode(4);
	insertBSTNode(&root, p);

	printf("insert 6\n");
	p = createTreeNode(6);
	insertBSTNode(&root, p);

	printf("insert 8\n");
	p = createTreeNode(8);
	insertBSTNode(&root, p);

	printf("insert 1\n");
	p = createTreeNode(1);
	insertBSTNode(&root, p);


	gBSTree->root = root;
	dumpTree(gBSTree);

	p = NULL;
	p = SearchBSTreeNode(gBSTree->root, 5);
	if(NULL != p){
		printf("node[%p] = %d\n", p, p->data);
	} else {
		printf("Cannot find this node\n");
	}

	p = SearchBSTreeNode(gBSTree->root, 10);
	if(NULL != p){
		printf("node[%p] = %d\n", p, p->data);
	} else {
		printf("Cannot find this node\n");
	}

	p = SearchBSTreeNode(gBSTree->root, 8);
	if(NULL != p){
		printf("node[%p] = %d\n", p, p->data);
	} else {
		printf("Cannot find this node\n");
	}

	mostLeft = getMostLeftNode(gBSTree->root);
	if(NULL != mostLeft){
		printf("mostLeft[%p] = %d\n", mostLeft, mostLeft->data);
	} else {
		printf("Null Tree\n");
	}	

	mostLeft = getMostLeftNode(gBSTree->root->right);
	if(NULL != mostLeft){
		printf("mostLeft[%p] = %d\n", mostLeft, mostLeft->data);
	} else {
		printf("Null Tree\n");
	}	

	g_lastvalue = 0;
	rst = isBSTree(gBSTree->root);
	if(rst){
		printf("Tree %p is Binary Sorted Tree \n", gBSTree->root);
	} else {
		printf("Tree %p is not Binary Sorted Tree \n", gBSTree->root);
	}

	/*删除在最后测试*/
	root = gBSTree->root;
	//removeBSTreeNode(&root, 3);
	//removeBSTreeNode(&root, 1);	
	//removeBSTreeNode(&root, 7);
	//removeBSTreeNode(&root, 8); /*删除右子树*/
	removeBSTreeNode(&root, 5); /*删除根节点*/
	gBSTree->root = root;
	dumpTree(gBSTree);	

	/*手动修改一个节点，使得树不在排序*/
	p = SearchBSTreeNode(gBSTree->root, 4);
	if(NULL != p){
		printf("node[%p] = %d\n", p, p->data);
	} else {
		printf("Cannot find this node\n");
	}	
	p->data = 100;
	printf("========= Traverse Tree InOder: %p ===========\n\t", gBSTree);
	TraverseTreeInOrder(gBSTree->root);
	printf("\n===================================\n");


	g_lastvalue = 0;
	rst = isBSTree(gBSTree->root);
	if(rst){
		printf("Tree %p is Binary Sorted Tree \n", gBSTree->root);
	} else {
		printf("Tree %p is not Binary Sorted Tree \n", gBSTree->root);
	}

	return;
}

