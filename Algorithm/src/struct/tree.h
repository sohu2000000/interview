#ifndef __TREE_H__
#define __TREE_H__

#include <stdbool.h>
#include "queue.h"

#define SUCCESS		0
#define PARAM_ERR	-1
#define ALLOC_ERR   -2

typedef struct trNode {
	int data;
	int tag;
	struct trNode * left;
	struct trNode * right;
	struct trNode * parent;
} st_trNode;

typedef struct tree {
	struct trNode * root;
}st_tree;

/* 全局BST指针（用于测试） */
st_tree * gBSTree;

/* ===== 二叉搜索树操作函数（统一命名规范：动词_tree_名词_形容词） ===== */

/* 创建操作 */
st_trNode *create_tree_node_with_value(int node_data);
int create_tree_structure_empty(st_tree **tree_ptr);
int insert_tree_node_to_bst(st_trNode **root_ptr, st_trNode *new_node);

/* 查找操作 */
st_trNode *search_tree_node_by_value(st_trNode *tree_root, int target_value);
st_trNode *find_tree_node_leftmost(st_trNode *subtree_root);

/* 修改操作 */
int remove_tree_node_by_value(st_trNode **root_ptr, int target_value);

/* 检验操作 */
bool validate_tree_is_bst(st_trNode *tree_root);

/* 查询操作 */
int get_tree_depth(st_trNode *tree_root);

/* 显示操作 */
int display_tree_contents(st_tree *tree_ptr);

/* 兼容性宏：保持旧接口可用 */
#define createTreeNode   create_tree_node_with_value
#define createTree       create_tree_structure_empty
#define insertBSTNode    insert_tree_node_to_bst
#define SearchBSTreeNode search_tree_node_by_value
#define getMostLeftNode  find_tree_node_leftmost
#define removeBSTreeNode remove_tree_node_by_value
#define isBSTree         validate_tree_is_bst
#define dumpTree         display_tree_contents

/* 树遍历函数（已优化命名） */
int traverse_tree_inorder_recursive(st_trNode *root);      // 中序遍历（递归）
int traverse_tree_inorder_iterative(st_trNode *root);      // 中序遍历（迭代）
int traverse_tree_preorder_recursive(st_trNode *root);     // 前序遍历（递归）
int traverse_tree_preorder_iterative(st_trNode *root);     // 前序遍历（迭代）
int traverse_tree_postorder_recursive(st_trNode *root);    // 后序遍历（递归）
int traverse_tree_postorder_iterative(st_trNode *root);    // 后序遍历（迭代）
int traverse_tree_level_order(st_trNode *root, st_queue *queue);  // 层序遍历

/* 兼容性宏：树遍历函数 */
#define TraverseTreeInOrder          traverse_tree_inorder_recursive
#define TraverseTreeInOrderUnrec     traverse_tree_inorder_iterative
#define TraverseTreePreOrder         traverse_tree_preorder_recursive
#define TraverseTreePreOrderUnrec    traverse_tree_preorder_iterative
#define TraverseTreePostOrder        traverse_tree_postorder_recursive
#define TraverseTreePostOrderUnrec   traverse_tree_postorder_iterative
#define TraverseTreeLevelOrder       traverse_tree_level_order

/* 待优化的函数 */
int insertBSTNode(st_trNode ** proot, st_trNode * node);
int dumpTree(st_tree * tree);
int removeBSTreeNode(st_trNode ** proot, int data);
bool isBSTree(st_trNode * root);


void testBSTree(void);

#endif