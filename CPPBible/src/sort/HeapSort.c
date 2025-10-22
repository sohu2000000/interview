#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SUCCESS     0
#define PARAM_ERR  -1

// 交换宏定义
#define SWAP(a, b) do { \
    typeof(a) temp = (a); \
    (a) = (b); \
    (b) = temp; \
} while(0)

/**
 * 堆调整函数（向下调整）
 * 维护最大堆的性质，确保父节点大于子节点
 * @param array 堆数组
 * @param root 当前根节点索引
 * @param heap_size 堆的大小
 * @return SUCCESS(0) 成功，PARAM_ERR(-1) 参数错误
 */
int max_heapify(int *array, int root, int heap_size) {
    if (array == NULL) {
        printf("%s: parameter error - array is NULL\n", __func__);
        return PARAM_ERR;
    }
    
    if (root >= heap_size) {
        return SUCCESS;  // 索引超出范围，无需调整
    }

    int left_child = 2 * root + 1;   // 左子节点索引
    int right_child = 2 * root + 2;  // 右子节点索引
    int largest = root;              // 假设根节点最大

    // 检查左子节点是否存在且大于根节点
    if (left_child < heap_size && array[left_child] > array[largest]) {
        largest = left_child;
    }

    // 检查右子节点是否存在且大于当前最大值
    if (right_child < heap_size && array[right_child] > array[largest]) {
        largest = right_child;
    }

    // 如果最大值不是根节点，需要交换并继续调整
    if (largest != root) {
        SWAP(array[root], array[largest]);
        
        // 递归调整被交换的子树
        return max_heapify(array, largest, heap_size);
    }

    return SUCCESS;
}

/**
 * 构建最大堆
 * 从最后一个非叶子节点开始，自底向上调整堆
 * @param array 待建堆的数组
 * @param size 数组大小
 * @return SUCCESS(0) 成功，PARAM_ERR(-1) 参数错误
 */
int build_max_heap(int *array, int size) {
    if (array == NULL) {
        printf("%s: parameter error - array is NULL\n", __func__);
        return PARAM_ERR;
    }
    
    if (size <= 1) {
        return SUCCESS;  // 空数组或单元素数组已经是堆
    }

    /*
     * 关键思路：从最后一个非叶子节点开始，自底向上建堆
     * 最后一个非叶子节点的索引是 (size-1-1)/2 = size/2 - 1
     * 
     * 为什么要从后往前？
     * 1. 叶子节点本身就满足堆性质
     * 2. 必须先调整子树，再调整父节点
     * 3. 这样可以保证每次调整时，子树已经是合法的堆
     * 
     * 为什么从 size/2-1 开始？
     * - 索引 size/2 到 size-1 都是叶子节点 [数组后半部分都认为是叶子节点]
     * - 叶子节点本身就满足堆性质
     * - 只需要调整非叶子节点
     */
    for (int i = size / 2 - 1; i >= 0; i--) {
        int result = max_heapify(array, i, size);
        if (result != SUCCESS) {
            printf("%s: failed to heapify at index %d\n", __func__, i);
            return result;
        }
    }

#ifdef DEBUG
    printf("构建最大堆完成，堆大小: %d\n", size);
    printf("建堆结果: [");
    for (int k = 0; k < size; k++) {
        printf("%d", array[k]);
        if (k < size - 1) printf(", ");
    }
    printf("]\n\n");
#endif

    return SUCCESS;
}

/**
 * 堆排序主函数
 * 先构建最大堆，然后反复取出堆顶元素进行排序
 * @param array 待排序数组
 * @param size 数组大小
 * @return SUCCESS(0) 成功，PARAM_ERR(-1) 参数错误
 */
int heap_sort(int *array, int size) {
    if (array == NULL) {
        printf("%s: parameter error - array is NULL\n", __func__);
        return PARAM_ERR;
    }
    
    if (size <= 0) {
        printf("%s: parameter error - invalid size %d\n", __func__, size);
        return PARAM_ERR;
    }
    
    if (size == 1) {
        return SUCCESS;  // 单元素数组已经有序
    }

    printf("开始堆排序，数组大小: %d\n", size);

    // 第一阶段：构建最大堆
    printf("\n=== 第一阶段：构建最大堆 ===\n");
    int build_result = build_max_heap(array, size);
    if (build_result != SUCCESS) {
        printf("%s: failed to build max heap\n", __func__);
        return build_result;
    }

    // 第二阶段：排序过程
    printf("=== 第二阶段：堆排序过程 ===\n");
    
    /*
     * 排序思路：
     * 1. 将堆顶（最大值）与堆的最后一个元素交换
     * 2. 堆大小减1（最大值进入有序区）
     * 3. 重新调整堆，维护堆性质
     * 4. 重复上述过程直到堆为空
     */
    for (int heap_size = size - 1; heap_size > 0; heap_size--) {
        // 将堆顶（最大值）与堆的最后一个元素交换
        int max_value = array[0];
        SWAP(array[0], array[heap_size]);

#ifdef DEBUG
        printf("第%d步: 取出最大值%d，剩余堆大小=%d\n", 
               size - heap_size, max_value, heap_size);
        
        printf("堆区: [");
        for (int k = 0; k < heap_size; k++) {
            printf("%d", array[k]);
            if (k < heap_size - 1) printf(", ");
        }
        printf("] | 有序区: [");
        for (int k = heap_size; k < size; k++) {
            printf("%d", array[k]);
            if (k < size - 1) printf(", ");
        }
        printf("]\n");
#endif

        // 重新调整堆（堆大小减1）
        int heapify_result = max_heapify(array, 0, heap_size);
        if (heapify_result != SUCCESS) {
            printf("%s: failed to heapify in step %d\n", __func__, size - heap_size);
            return heapify_result;
        }

#ifdef DEBUG
        printf("调整后: [");
        for (int k = 0; k < heap_size; k++) {
            printf("%d", array[k]);
            if (k < heap_size - 1) printf(", ");
        }
        printf("] | 有序区: [");
        for (int k = heap_size; k < size; k++) {
            printf("%d", array[k]);
            if (k < size - 1) printf(", ");
        }
        printf("]\n\n");
#endif
    }

    printf("堆排序完成！\n");
    return SUCCESS;
}

// 注意：原 HeapSort 函数已重构为 heap_sort
// 新接口：heap_sort(array, size)


/**
 * 打印数组内容的辅助函数
 * @param label 数组标签
 * @param array 要打印的数组
 * @param size 数组大小
 */
void print_array(const char *label, int *array, int size) {
    if (!array || size <= 0) {
        printf("%s: 无效数组\n", label);
        return;
    }
    
    printf("%s: [", label);
    for (int i = 0; i < size; i++) {
        printf("%d", array[i]);
        if (i < size - 1) printf(", ");
    }
    printf("]\n");
}

/**
 * 验证数组是否已正确排序
 * @param array 待验证的数组
 * @param size 数组大小
 * @return true 已排序，false 未排序
 */
bool is_array_sorted(int *array, int size) {
    if (!array || size <= 1) return true;
    
    for (int i = 1; i < size; i++) {
        if (array[i] < array[i-1]) {
            return false;
        }
    }
    return true;
}

/**
 * 打印堆的树状结构（简化版）
 * @param array 堆数组
 * @param size 堆大小
 */
void print_heap_structure(int *array, int size) {
    if (!array || size <= 0) return;
    
    printf("堆结构可视化:\n");
    printf("        %d\n", array[0]);  // 根节点
    
    if (size > 1) {
        printf("      ");
        if (size > 1) printf("%d", array[1]);  // 左子节点
        if (size > 2) printf("  %d", array[2]); // 右子节点
        printf("\n");
    }
    
    if (size > 3) {
        printf("    ");
        for (int i = 3; i < 7 && i < size; i++) {
            printf("%d  ", array[i]);
        }
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char **argv) {
    printf("🏔️ 堆排序测试程序\n");
    printf("================\n\n");
    
    // 测试1：原始测试数据
    printf("【测试1: 原始测试数据】\n");
    int array1[10] = {7, 3, 5, 8, 0, 9, 1, 2, 4, 6};
    print_array("排序前", array1, 10);
    
    int result1 = heap_sort(array1, 10);  // 使用新的函数名
    if (result1 == SUCCESS) {
        print_array("排序后", array1, 10);
        printf("排序验证: %s\n", is_array_sorted(array1, 10) ? "✅ 正确" : "❌ 错误");
    }
    
    // 测试2：已排序数组
    printf("\n【测试2: 已排序数组】\n");
    int array2[6] = {1, 2, 3, 4, 5, 6};
    print_array("排序前", array2, 6);
    
    int result2 = heap_sort(array2, 6);
    if (result2 == SUCCESS) {
        print_array("排序后", array2, 6);
        printf("排序验证: %s\n", is_array_sorted(array2, 6) ? "✅ 正确" : "❌ 错误");
    }
    
    // 测试3：逆序数组
    printf("\n【测试3: 逆序数组】\n");
    int array3[5] = {5, 4, 3, 2, 1};
    print_array("排序前", array3, 5);
    
    int result3 = heap_sort(array3, 5);
    if (result3 == SUCCESS) {
        print_array("排序后", array3, 5);
        printf("排序验证: %s\n", is_array_sorted(array3, 5) ? "✅ 正确" : "❌ 错误");
    }
    
    // 测试4：边界情况
    printf("\n【测试4: 边界情况】\n");
    
    // 单元素数组
    int single[1] = {42};
    printf("单元素数组测试: ");
    int single_result = heap_sort(single, 1);
    printf("返回值=%d %s\n", single_result, single_result == SUCCESS ? "✅" : "❌");
    
    // 空指针测试
    printf("空指针测试: ");
    int null_result = heap_sort(NULL, 5);
    printf("返回值=%d %s\n", null_result, null_result == PARAM_ERR ? "✅" : "❌");
    
    // 重复元素测试
    printf("重复元素测试: ");
    int duplicates[6] = {3, 1, 4, 1, 5, 3};
    print_array("", duplicates, 6);
    int dup_result = heap_sort(duplicates, 6);
    if (dup_result == SUCCESS) {
        print_array("排序后", duplicates, 6);
        printf("稳定性验证: %s\n", is_array_sorted(duplicates, 6) ? "✅ 正确" : "❌ 错误");
    }
    
    printf("\n🎉 所有测试完成！\n");
    return 0;
}




