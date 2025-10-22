#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define SUCCESS     0
#define PARAM_ERR   -1

// 交换宏定义
#define SWAP(a, b) do { \
    typeof(a) temp = (a); \
    (a) = (b); \
    (b) = temp; \
} while(0)

// 优化后的冒泡排序函数
int BubbleSort(int *array, int size) {
    // 参数验证
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
    
    int sorted_boundary = 0;  // 有序区域边界
    int current_pos = 0;      // 当前比较位置
    bool swapped = false;     // 是否发生过交换
    
#ifdef DEBUG
    int debug_idx = 0;
    printf("=== 冒泡排序过程 ===\n");
#endif

    for (sorted_boundary = 0; sorted_boundary < size; sorted_boundary++) {
        swapped = false;  // 重置交换标志
        
        // 在无序区域中寻找最小元素并上浮
        for (current_pos = sorted_boundary + 1; current_pos < size; current_pos++) {
            // 升序排序：小的元素上浮
            if (array[current_pos] < array[sorted_boundary]) {
                SWAP(array[sorted_boundary], array[current_pos]);
                swapped = true;
            }
        }

#ifdef DEBUG
        // 显示当前排序状态
        printf("第%d轮: ", sorted_boundary + 1);
        printf("有序区[");
        for (debug_idx = 0; debug_idx <= sorted_boundary; debug_idx++) {
            printf("%d", array[debug_idx]);
            if (debug_idx < sorted_boundary) printf(", ");
        }
        printf("] 无序区[");
        for (debug_idx = sorted_boundary + 1; debug_idx < size; debug_idx++) {
            printf("%d", array[debug_idx]);
            if (debug_idx < size - 1) printf(", ");
        }
        printf("]");
        printf(" %s\n", swapped ? "有交换" : "无交换");
#endif

        // 提前终止优化：如果没有发生交换，说明已经完全有序
        if (!swapped) {
            printf("排序提前完成于第%d轮\n", sorted_boundary + 1);
            break;
        }
    }

    return SUCCESS;
}


// 打印数组的辅助函数
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

// 验证数组是否已排序
bool is_sorted(int *array, int size) {
    if (!array || size <= 1) return true;
    
    for (int i = 1; i < size; i++) {
        if (array[i] < array[i-1]) {
            return false;
        }
    }
    return true;
}

// 性能测试函数
void performance_test(void) {
    const int test_sizes[] = {10, 100, 1000, 5000};
    const int num_tests = sizeof(test_sizes) / sizeof(test_sizes[0]);
    
    printf("\n=== 性能测试 ===\n");
    
    for (int t = 0; t < num_tests; t++) {
        int size = test_sizes[t];
        int *test_array = malloc(size * sizeof(int));
        
        if (!test_array) {
            printf("内存分配失败 (size=%d)\n", size);
            continue;
        }
        
        // 生成随机数组
        for (int i = 0; i < size; i++) {
            test_array[i] = rand() % 1000;
        }
        
        printf("数组大小: %d, ", size);
        
        // 记录开始时间（简单计时）
        clock_t start = clock();
        
        int result = BubbleSort(test_array, size);
        
        clock_t end = clock();
        double time_spent = ((double)(end - start)) / CLOCKS_PER_SEC;
        
        if (result == SUCCESS && is_sorted(test_array, size)) {
            printf("排序成功, 耗时: %.4f秒\n", time_spent);
        } else {
            printf("排序失败!\n");
        }
        
        free(test_array);
    }
}

int main(int argc, char **argv) {
    // 测试数组1：一般情况
    int array1[10] = {7, 3, 5, 8, 0, 9, 1, 2, 4, 6};
    printf("=== 测试1: 一般数组 ===\n");
    print_array("排序前", array1, 10);
    
    int result1 = BubbleSort(array1, 10);
    if (result1 == SUCCESS) {
        print_array("排序后", array1, 10);
        printf("排序验证: %s\n", is_sorted(array1, 10) ? "✓ 正确" : "✗ 错误");
    }
    
    // 测试数组2：已排序数组（最好情况）
    printf("\n=== 测试2: 已排序数组 ===\n");
    int array2[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    print_array("排序前", array2, 8);
    
    int result2 = BubbleSort(array2, 8);
    if (result2 == SUCCESS) {
        print_array("排序后", array2, 8);
    }
    
    // 测试数组3：逆序数组（最坏情况）
    printf("\n=== 测试3: 逆序数组 ===\n");
    int array3[6] = {6, 5, 4, 3, 2, 1};
    print_array("排序前", array3, 6);
    
    int result3 = BubbleSort(array3, 6);
    if (result3 == SUCCESS) {
        print_array("排序后", array3, 6);
    }
    
    // 测试数组4：重复元素
    printf("\n=== 测试4: 重复元素 ===\n");
    int array4[7] = {3, 1, 4, 1, 5, 9, 2};
    print_array("排序前", array4, 7);
    
    int result4 = BubbleSort(array4, 7);
    if (result4 == SUCCESS) {
        print_array("排序后", array4, 7);
    }
    
    // 测试边界情况
    printf("\n=== 测试5: 边界情况 ===\n");
    
    // 空指针测试
    printf("空指针测试: ");
    int null_result = BubbleSort(NULL, 5);
    printf("返回值=%d %s\n", null_result, null_result == PARAM_ERR ? "✓" : "✗");
    
    // 无效大小测试
    printf("无效大小测试: ");
    int invalid_result = BubbleSort(array1, 0);
    printf("返回值=%d %s\n", invalid_result, invalid_result == PARAM_ERR ? "✓" : "✗");
    
    // 单元素数组测试
    printf("单元素数组测试: ");
    int single[1] = {42};
    int single_result = BubbleSort(single, 1);
    printf("返回值=%d %s\n", single_result, single_result == SUCCESS ? "✓" : "✗");
    
    // 性能测试
//     performance_test();
    
    printf("\n=== 所有测试完成 ===\n");
    return 0;
}



