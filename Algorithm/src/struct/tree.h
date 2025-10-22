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

st_tree * gBSTree;

st_trNode * createTreeNode(int data);
int createTree(st_tree ** ptree);
int insertBSTNode(st_trNode ** proot, st_trNode * node);
int TraverseTreePreOrder(st_trNode * root);
int TraverseTreeInOrder(st_trNode * root);
int TraverseTreePostOrder(st_trNode * root);
int dumpTree(st_tree * tree);
st_trNode * SearchBSTreeNode(st_trNode * root, int data);
st_trNode * getMostLeftNode(st_trNode * root);
int removeBSTreeNode(st_trNode ** proot, int data);
bool isBSTree(st_trNode * root);

int TraverseTreeInOrderUnrec(st_trNode * root);
int TraverseTreePreOrderUnrec(st_trNode * root);
int TraverseTreePostOrderUnrec(st_trNode * root);
int TraverseTreeLevelOrder(st_trNode * root, st_queue * queue);


void testBSTree(void);

#endif