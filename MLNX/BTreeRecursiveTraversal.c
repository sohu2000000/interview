#include <stdio.h>
#include <stdlib.h>

// 二叉树节点结构体
typedef struct Node {
    int data;
    struct Node* left;
    struct Node* right;
} Node;

// 创建新节点 - 改进：添加错误处理
Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// 前序遍历
void preorder(Node* root) {
    if (root == NULL) return;
    printf("%d ", root->data);
    preorder(root->left);
    preorder(root->right);
}

// 中序遍历
void inorder(Node* root) {
    if (root == NULL) return;
    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
}

// 后序遍历
void postorder(Node* root) {
    if (root == NULL) return;
    postorder(root->left);
    postorder(root->right);
    printf("%d ", root->data);
}

// 递归释放树
void destroyTree(Node* root) {
    if (root == NULL) return;
    destroyTree(root->left);
    destroyTree(root->right);
    free(root);
}

// 辅助函数：打印树的结构（简单的层次显示）
void printTreeStructure(Node* root, int level) {
    if (root == NULL) return;
    
    printTreeStructure(root->right, level + 1);
    
    for (int i = 0; i < level; i++) {
        printf("    ");
    }
    printf("%d\n", root->data);
    
    printTreeStructure(root->left, level + 1);
}

int main() {
    printf("=== 二叉树递归遍历测试 ===\n\n");
    
    // 测试1：空树
    printf("1. 测试空树：\n");
    Node* emptyTree = NULL;
    printf("前序遍历: ");
    preorder(emptyTree);
    printf("(空)\n");
    printf("中序遍历: ");
    inorder(emptyTree);
    printf("(空)\n");
    printf("后序遍历: ");
    postorder(emptyTree);
    printf("(空)\n\n");
    
    // 测试2：单节点树
    printf("2. 测试单节点树：\n");
    Node* singleNode = createNode(42);
    if (singleNode) {
        printf("前序遍历: ");
        preorder(singleNode);
        printf("\n中序遍历: ");
        inorder(singleNode);
        printf("\n后序遍历: ");
        postorder(singleNode);
        printf("\n");
        destroyTree(singleNode);
    }
    printf("\n");
    
    // 测试3：原始测试用例
    printf("3. 测试标准二叉树：\n");
    Node* root = createNode(1);
    if (root == NULL) return -1;
    
    root->left = createNode(2);
    root->right = createNode(3);
    if (root->left) {
        root->left->left = createNode(4);
        root->left->right = createNode(5);
    }
    
    printf("树的结构：\n");
    printTreeStructure(root, 0);
    printf("\n");
    
    printf("前序遍历 (根→左→右): ");
    preorder(root);
    printf("\n中序遍历 (左→根→右): ");
    inorder(root);
    printf("\n后序遍历 (左→右→根): ");
    postorder(root);
    printf("\n\n");
    
    // 测试4：更复杂的树
    printf("4. 测试更复杂的二叉树：\n");
    Node* complexRoot = createNode(10);
    if (complexRoot) {
        complexRoot->left = createNode(5);
        complexRoot->right = createNode(15);
        if (complexRoot->left) {
            complexRoot->left->left = createNode(3);
            complexRoot->left->right = createNode(7);
        }
        if (complexRoot->right) {
            complexRoot->right->left = createNode(12);
            complexRoot->right->right = createNode(18);
        }
        
        printf("复杂树的结构：\n");
        printTreeStructure(complexRoot, 0);
        printf("\n");
        
        printf("前序遍历: ");
        preorder(complexRoot);
        printf("\n中序遍历: ");
        inorder(complexRoot);
        printf("\n后序遍历: ");
        postorder(complexRoot);
        printf("\n\n");
        
        destroyTree(complexRoot);
    }
    
    // 释放原始树的内存
    destroyTree(root);
    
    printf("所有测试完成，内存已释放。\n");
    return 0;
}
