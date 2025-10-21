#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "stack.h"
#include "queue.h"

st_trNode * createTreeNode(int data){
	st_trNode * p = NULL;

	p = (st_trNode * ) malloc (sizeof(st_trNode));
	if(NULL ==  p){
		printf("createTreeNode alloc failed \n");
		return NULL;
	}

	p->data = data;
	p->tag = 0;
	p->left = NULL;
	p->right = NULL;
	p->parent = NULL;

	return p;
}


int createTree(st_tree ** ptree){

	st_tree * tree = NULL;

	tree = (st_tree * ) malloc (sizeof(st_tree));
	if(NULL ==  tree){
		printf("createTree alloc failed \n");
		return ALLOC_ERR;
	}

	tree->root = NULL;

	*ptree = tree;

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
	if(NULL == root){
		root = node;
#ifdef DEBUG		
		printf("1.1  root = %d\n", root->data);		
#endif
		goto out;
	}

	/*递归结束条件：叶子节点*/
	if(NULL == root->left && NULL == root->right){
#ifdef DEBUG
		printf("1.6\n");
#endif
		if(node->data < root->data)	{
			root->left = node;
			node->parent = root;
#ifdef DEBUG		
			printf("1.2  left = %d, parent = %d \n", root->left->data, node->parent->data);	
#endif
		} else {
			root->right = node;
			node->parent = root;
#ifdef DEBUG	
			printf("1.3  right = %d, parent = %d \n", root->right->data, node->parent->data);	
#endif
		}
		goto out;
	}
	
	left= root->left;
	right = root->right;

#ifdef DEBUG
	printf("1.7\n");
#endif

	/*向左子树插入*/
	if(node->data < root->data){
		if(NULL == left){
#ifdef DEBUG
			printf("1.4.1  root = %d \n", root->data);	
#endif
			root->left = node;
			node->parent = root;
		} else{
#ifdef DEBUG			
			printf("1.4.2  root = %d, left = %d \n", root->data, left->data);	
#endif
			insertBSTNode(&left, node);			
			/*左子树根可能发生变化*/
			root->left = left;
			left->parent = root;
		}
	} else { /*向右子树插入*/
		if(NULL == right){
#ifdef DEBUG			
			printf("1.5.1  root = %d \n", root->data);
#endif
			root->right = node;
			node->parent = root;
		} else {
#ifdef DEBUG	
			printf("1.5.2  root = %d, right = %d \n", root->data, right->data);	
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

int TraverseTreePreOrder(st_trNode * root){

	/*递归结束条件*/
	if(NULL == root){
		return SUCCESS;
	}

	printf(" %d ", root->data);

	/*左子树*/
	if(NULL != root->left)
		TraverseTreePreOrder(root->left);

	/*右子树*/	
	if(NULL != root->right)
		TraverseTreePreOrder(root->right);

	return SUCCESS;
}


/*
 * 对于 BST 来说，中序打印出来的是升序的
 */
int TraverseTreeInOrder(st_trNode * root){

	/*递归结束条件*/
	if(NULL == root){
		return SUCCESS;
	}

	/*左子树*/
	if(NULL != root->left)
		TraverseTreeInOrder(root->left);

	printf(" %d ", root->data);

	/*右子树*/	
	if(NULL != root->right)
		TraverseTreeInOrder(root->right);

	return SUCCESS;
}

/*
 * 对于 BST 来说，中序打印出来的是升序的
 */
int TraverseTreePostOrder(st_trNode * root){

	/*递归结束条件*/
	if(NULL == root){
		return SUCCESS;
	}

	/*左子树*/
	if(NULL != root->left)
		TraverseTreePostOrder(root->left);

	/*右子树*/	
	if(NULL != root->right)
		TraverseTreePostOrder(root->right);

	printf(" %d ", root->data);

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

st_trNode * SearchBSTreeNode(st_trNode * root, int data){

	if(NULL == root){
		return NULL;
	}

	st_trNode * node = NULL;
	st_trNode * left = root->left, * right = root->right;
	
	if(data == root->data){
		return root;
	}

	if(NULL == left && NULL == right){
		return NULL;
	}

	if(data < root->data){
		if(NULL == left){
			return NULL;
		} else {
			node = SearchBSTreeNode(root->left, data);
		}
	} else {
		if(NULL == right){
			return NULL;
		} else {
			node = SearchBSTreeNode(root->right, data);
		}		
	}

	return node;

}


st_trNode * getMostLeftNode(st_trNode * root){
	if(NULL == root){
		return root;
	}

	st_trNode * p = NULL;

	if(NULL == root->left){
		return root;
	}

	p = root->left;
	while(NULL != p->left){
		p = p->left;
	}

	return p;
}

int removeBSTreeNode(st_trNode ** proot, int data){
	if(NULL == proot){
		printf("removeBSTreeNode param error\n");
		return PARAM_ERR;
	}

	st_trNode * root = *proot;
	st_trNode * node = NULL;
	st_trNode * left = NULL, * right = NULL, * parent = NULL;
	st_trNode * mostsubLeft = NULL; /*右子树的最左子节点*/
	int atLeft = 0, atRight = 0;

	/* 1. 找data对应的节点 */
	node = SearchBSTreeNode(root, data);
	if(NULL == node){
		printf("removeBSTreeNode can not find %d \n", data);
		return PARAM_ERR;
	}

	left = node->left;
	right = node->right;
	parent = node->parent;

	if(NULL != right){
		mostsubLeft = getMostLeftNode(right);
	}

	/* 非根节点 */
	if(NULL != parent){

		/*1. 摘下node和它的子树*/
		if(node->data < parent->data){
			parent->left = NULL;
			node->parent = NULL;
			atLeft = 1;
		} else {
			parent->right = NULL;
			node->parent = NULL;
			atRight = 1;
		}

		/*2. 挂子树*/
		
		/*node 是叶子节点，什么也不用做 */
		if(NULL == left && NULL == right){
		}

		/*非叶子节点*/

		/*有左子没右子*/
		if(NULL != left && NULL == right){ 
			if(1 == atLeft){
				parent->left = left;
				left->parent = parent;
			}  else if(1 == atRight) {
				parent->right = left;
				left->parent = parent;
			}
		}

		/*没左子有右子*/
		if(NULL == left &&  NULL != right){ 
			if(1 == atLeft){
				parent->left = right;
				right->parent = parent;
			}  else if(1 == atRight) {
				parent->right = right;
				right->parent = parent;
			}	
		}

		/*有左子有右子， 左子挂在右子的最左边节点的左边, 右子树根节点继承原来节点位置*/
		if(NULL != left && NULL != right){
			mostsubLeft->left = left;
			left->parent = mostsubLeft;
		
			if(1 == atLeft){
				parent->left = right;
				right->parent = parent;
			}  else if(1 == atRight) {
				parent->right = right;
				right->parent = parent;
			}	
			
		}
	} else { /* 根节点 */

		/*node 是叶子节点，什么也不用做 */
		if(NULL == left && NULL == right){
			root = NULL;
		}

		/*有左子没右子*/
		if(NULL != left && NULL == right){ 
			root = left;
		}

		/*没左子有右子*/
		if(NULL == left &&  NULL != right){ 	
			root = right;
		}		

		/*没左子有右子*/
		if(NULL == left &&  NULL != right){ 
			if(1 == atLeft){
				parent->left = right;
				right->parent = parent;
			}  else if(1 == atRight) {
				parent->right = right;
				right->parent = parent;
			}	
		}

		/*有左子有右子， 左子挂在右子的最左边节点的左边, 右子树根节点继承原来节点位置*/
		if(NULL != left && NULL != right){
			mostsubLeft->left = left;
			left->parent = mostsubLeft;
			/*更新根节点*/
			root = right;			
		}
		
		*proot = root;
	}

	free(node);
	node = NULL;

	return SUCCESS;
}


int TraverseTreeInOrderUnrec(st_trNode * root){
	st_dataNode * s = NULL;
	st_trNode * p = root;
	st_stack * stack = createStack();
	
	while(NULL != p || !isStackEmpty(stack)){

		/* 1. 左子树*/
		while(NULL != p){
			pushStack(stack, p->data);
			p = p->left;
		}

		if(!isStackEmpty(stack)){

			/* 2. 打印*/
			s = popStack(stack);			
			printf(" %d ", s->data);


			/* 3. 右子树 */
			/*
			 * 如果stack可以保存树节点，可以不用这步，
			 * 主要用来查找到p后寻找p的子树的关系
		     */
			p = SearchBSTreeNode(root, s->data); 
			if(NULL != s){
				free(s);
				s = NULL;
			}
			p = p->right;
		}
	}	
}


int TraverseTreePreOrderUnrec(st_trNode * root){
	st_dataNode * s = NULL;
	st_trNode * p = root;
	st_stack * stack = createStack();
	
	while(NULL != p || !isStackEmpty(stack)){

		while(NULL != p){
			/* 1. 打印*/
			printf(" %d ", p->data);

			/* 2. 左子树*/					
			pushStack(stack, p->data);
			p = p->left;
		}

		if(!isStackEmpty(stack)){

			s = popStack(stack);			

			/* 3. 右子树 */
			/*
			 * 如果stack可以保存树节点，可以不用这步，
			 * 主要用来查找到p后寻找p的子树的关系
		     */
			p = SearchBSTreeNode(root, s->data); 
			if(NULL != s){
				free(s);
				s = NULL;
			}
			p = p->right;
		}
	}	
}


int TraverseTreePostOrderUnrec(st_trNode * root){
	st_dataNode * s = NULL;
	st_trNode * p = root;
	st_stack * stack = createStack();
	
	while(NULL != p || !isStackEmpty(stack)){

		while(NULL != p){
			/* 1. 左子树*/					
			pushStack(stack, p->data);
			p = p->left;
		}

		/*
		 * 结点 入栈 时， 配 一个 标志 tag 一同 入栈
		 *（ tag 为 0 表示 遍历 左 子 树 前 的 现场 保护， 
		 * tag 为 1 表示 遍历 右 子 树 前 的 现场 保护）。 
		 * 首先 将 T 和 tag（ 为 0） 入栈， 遍历 左 子 树； 
		 * 返回 后， 修改 栈 顶 tag 为 1， 遍历 右 子 树； 
		 * 最后 访问 根 结点。
		 */

		if(!isStackEmpty(stack)){

			s = stack->top;
			p = SearchBSTreeNode(root, s->data); 
			if(1 == p->tag){
				/* 3. 打印 */
				s = popStack(stack);
				if(NULL != s){
					free(s);
					s = NULL;
				}				
				printf(" %d ", p->data);
				p->tag = 0;
				p = NULL; /*第二次访问其右子树已经遍历*/
			} else {
				/*2. 右子树*/
				p->tag = 1;
				p = p->right;				
			}
			
		}
	}	
}


int TraverseTreeLevelOrder(st_trNode * root, st_queue * queue){

	if(NULL == queue){
		printf("TraverseTreeLevelOrder param error\n");
		return PARAM_ERR;
	}

	if(NULL == root){
		return SUCCESS;
	}

	st_trNode * p = NULL;
	st_dataNode * q = NULL;
	int data = 0;

	inQueue(queue, root->data);

	while(!isQueueEmpty(queue)){
		q = deQueue(queue);
		data = q->data;
		free(q);
		q = NULL;
		printf(" %d ", data);

		p = SearchBSTreeNode(root, data);
		if(NULL != p->left){
			inQueue(queue, p->left->data);
		}
		if(NULL != p->right){
			inQueue(queue, p->right->data);
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





