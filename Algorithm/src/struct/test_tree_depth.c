#include <stdio.h>
#include "tree.h"
#include "stack.h"
#include "queue.h"

int main(void) {
    printf("========================================\n");
    printf("  测试树的深度计算\n");
    printf("========================================\n\n");

    st_tree *test_tree = NULL;
    st_trNode *root = NULL;
    st_trNode *new_node = NULL;
    int tree_depth = 0;

    // 创建树
    createTree(&test_tree);
    root = test_tree->root;

    printf("构建测试树：\n");
    printf("       5\n");
    printf("      / \\\n");
    printf("     3   7\n");
    printf("    / \\\n");
    printf("   2   4\n");
    printf("  /\n");
    printf(" 1\n\n");

    // 插入节点构建树
    new_node = createTreeNode(5);
    insertBSTNode(&root, new_node);

    new_node = createTreeNode(3);
    insertBSTNode(&root, new_node);

    new_node = createTreeNode(7);
    insertBSTNode(&root, new_node);

    new_node = createTreeNode(2);
    insertBSTNode(&root, new_node);

    new_node = createTreeNode(4);
    insertBSTNode(&root, new_node);

    new_node = createTreeNode(1);
    insertBSTNode(&root, new_node);

    test_tree->root = root;

    // 测试深度计算
    tree_depth = get_tree_depth(test_tree->root);
    printf("树的深度 = %d\n", tree_depth);
    printf("预期深度 = 4（根节点5算第1层）\n\n");

    // 验证：中序遍历看结构
    printf("中序遍历验证：");
    traverse_tree_inorder_recursive(test_tree->root);
    printf("\n");

    printf("\n========================================\n");
    printf("  测试完成！\n");
    printf("========================================\n");

    return SUCCESS;
}

