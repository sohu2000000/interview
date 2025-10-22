#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUCCESS       0
#define PARAM_ERROR  -1
#define MEMORY_ERROR -2

/**
 * 合并两个已排序的子数组
 * @param numbers: 原数组
 * @param temp_array: 临时数组用于合并
 * @param left_start: 左子数组起始位置
 * @param left_end: 左子数组结束位置
 * @param right_start: 右子数组起始位置
 * @param right_end: 右子数组结束位置
 * @return: 成功返回SUCCESS，失败返回错误码
 */
int merge_two_sorted_arrays(int *numbers, int *temp_array, 
                           int left_start, int left_end, 
                           int right_start, int right_end) {
    // 参数检查
    if (numbers == NULL || temp_array == NULL) {
        printf("Error: Invalid parameters in %s\n", __func__);
        return PARAM_ERROR;
    }

    int left_index = left_start;    // 左子数组当前索引
    int right_index = right_start;  // 右子数组当前索引
    int temp_index = 0;             // 临时数组索引
    int total_size = (left_end - left_start + 1) + (right_end - right_start + 1);

#ifdef DEBUG
    printf("Merging arrays: [%d-%d] and [%d-%d], total size: %d\n", 
           left_start, left_end, right_start, right_end, total_size);
    printf("Before merge: ");
    for (int i = left_start; i <= right_end; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
#endif

    // 清空临时数组
    memset(temp_array, 0, total_size * sizeof(int));

    // 比较两个子数组的元素，将较小的放入临时数组
    while (left_index <= left_end && right_index <= right_end) {
        if (numbers[left_index] <= numbers[right_index]) {
            temp_array[temp_index++] = numbers[left_index++];
        } else {
            temp_array[temp_index++] = numbers[right_index++];
        }
    }

    // 复制左子数组剩余元素
    while (left_index <= left_end) {
        temp_array[temp_index++] = numbers[left_index++];
    }

    // 复制右子数组剩余元素
    while (right_index <= right_end) {
        temp_array[temp_index++] = numbers[right_index++];
    }

    // 将合并结果复制回原数组
    for (int i = 0; i < total_size; i++) {
        numbers[left_start + i] = temp_array[i];
    }

#ifdef DEBUG
    printf("After merge: ");
    for (int i = left_start; i <= right_end; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n\n");
#endif

    return SUCCESS;
}

/**
 * 归并排序的递归实现
 * @param numbers: 待排序数组
 * @param temp_array: 临时数组
 * @param start: 排序区间起始位置
 * @param end: 排序区间结束位置
 * @return: 成功返回SUCCESS，失败返回错误码
 */
int merge_sort_recursive(int *numbers, int *temp_array, int start, int end) {
    // 参数检查
    if (numbers == NULL || temp_array == NULL) {
        printf("Error: Invalid parameters in %s\n", __func__);
        return PARAM_ERROR;
    }

    // 递归终止条件：只有一个元素或没有元素
    if (start >= end) {
        return SUCCESS;
    }

    // 计算中间位置，避免整数溢出
    int middle = start + (end - start) / 2;

#ifdef DEBUG
    printf("Dividing array [%d-%d] into [%d-%d] and [%d-%d]\n", 
           start, end, start, middle, middle + 1, end);
#endif

    // 递归排序左半部分
    merge_sort_recursive(numbers, temp_array, start, middle);

    // 递归排序右半部分
    merge_sort_recursive(numbers, temp_array, middle + 1, end);

    // 合并两个已排序的部分
    merge_two_sorted_arrays(numbers, temp_array, start, middle, middle + 1, end);

    return SUCCESS;
}


/**
 * 归并排序主函数 - 对外接口
 * @param numbers: 待排序的整数数组
 * @param length: 数组长度
 * @return: 成功返回SUCCESS，失败返回错误码
 */
int merge_sort(int *numbers, int length) {
    // 参数检查
    if (numbers == NULL || length <= 0) {
        printf("Error: Invalid parameters in %s\n", __func__);
        return PARAM_ERROR;
    }

    // 分配临时数组用于合并操作
    int *temp_array = (int *)malloc(length * sizeof(int));
    if (temp_array == NULL) {
        printf("Error: Failed to allocate memory in %s\n", __func__);
        return MEMORY_ERROR;
    }

    // 执行归并排序
    int result = merge_sort_recursive(numbers, temp_array, 0, length - 1);

    // 释放临时数组内存
    free(temp_array);
    temp_array = NULL;

    return result;
}


/**
 * 打印数组内容
 * @param numbers: 数组指针
 * @param length: 数组长度
 * @param title: 打印标题
 */
void print_array(int *numbers, int length, const char *title) {
    printf("%s: ", title);
    for (int i = 0; i < length; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
}

/**
 * 主函数 - 测试归并排序
 */
int main(int argc, char **argv) {
    int test_array[] = {7, 3, 5, 8, 0, 9, 1, 2, 4, 6};
    int array_size = sizeof(test_array) / sizeof(test_array[0]);

    // 显示排序前的数组
    print_array(test_array, array_size, "Before sorting");

    // 执行归并排序
    int result = merge_sort(test_array, array_size);
    
    if (result == SUCCESS) {
        // 显示排序后的数组
        print_array(test_array, array_size, "After sorting");
    } else {
        printf("Sorting failed with error code: %d\n", result);
    }

    return 0;
}
