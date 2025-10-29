#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUCCESS       0
#define PARAM_ERROR  -1
#define MEMORY_ERROR -2

/**
 * 合并两个已排序的子数组 - 归并排序的核心操作
 * @param source_array: 原数组
 * @param auxiliary_array: 辅助数组用于合并
 * @param left_subarray_start: 左子数组起始位置
 * @param left_subarray_end: 左子数组结束位置
 * @param right_subarray_start: 右子数组起始位置
 * @param right_subarray_end: 右子数组结束位置
 * @return: 成功返回SUCCESS，失败返回错误码
 */
int merge_two_sorted_subarrays(int *source_array, int *auxiliary_array, 
                               int left_subarray_start, int left_subarray_end, 
                               int right_subarray_start, int right_subarray_end) {
    // 参数有效性检查
    if (source_array == NULL || auxiliary_array == NULL) {
        printf("Error: Invalid parameters in %s\n", __func__);
        return PARAM_ERROR;
    }

    int left_current_index = left_subarray_start;      // 左子数组当前处理位置
    int right_current_index = right_subarray_start;    // 右子数组当前处理位置
    int auxiliary_write_index = 0;                     // 辅助数组写入位置
    int merged_elements_count = (left_subarray_end - left_subarray_start + 1) + 
                               (right_subarray_end - right_subarray_start + 1);

#ifdef DEBUG
    printf("合并子数组: [%d-%d] 和 [%d-%d], 总元素数: %d\n", 
           left_subarray_start, left_subarray_end, right_subarray_start, right_subarray_end, merged_elements_count);
    printf("合并前: ");
    for (int i = left_subarray_start; i <= right_subarray_end; i++) {
        printf("%d ", source_array[i]);
    }
    printf("\n");
#endif

    // 初始化辅助数组（可选，确保数据清洁）
    memset(auxiliary_array, 0, merged_elements_count * sizeof(int));

    // 双指针合并：比较两个子数组的当前元素，将较小者放入辅助数组
    while (left_current_index <= left_subarray_end && right_current_index <= right_subarray_end) {
        if (source_array[left_current_index] <= source_array[right_current_index]) {
            auxiliary_array[auxiliary_write_index++] = source_array[left_current_index++];
        } else {
            auxiliary_array[auxiliary_write_index++] = source_array[right_current_index++];
        }
    }

    // 处理左子数组剩余元素：如果左子数组还有未处理的元素，全部复制
    while (left_current_index <= left_subarray_end) {
        auxiliary_array[auxiliary_write_index++] = source_array[left_current_index++];
    }

    // 处理右子数组剩余元素：如果右子数组还有未处理的元素，全部复制
    while (right_current_index <= right_subarray_end) {
        auxiliary_array[auxiliary_write_index++] = source_array[right_current_index++];
    }

    // 将合并结果从辅助数组复制回原数组的对应位置
    for (int copy_index = 0; copy_index < merged_elements_count; copy_index++) {
        source_array[left_subarray_start + copy_index] = auxiliary_array[copy_index];
    }

#ifdef DEBUG
    printf("合并后: ");
    for (int i = left_subarray_start; i <= right_subarray_end; i++) {
        printf("%d ", source_array[i]);
    }
    printf("\n\n");
#endif

    return SUCCESS;
}

/**
 * 归并排序的分治递归实现 - 分而治之的核心逻辑
 * @param array_to_sort: 待排序数组
 * @param auxiliary_workspace: 辅助工作空间数组
 * @param range_start: 当前排序区间的起始位置
 * @param range_end: 当前排序区间的结束位置
 * @return: 成功返回SUCCESS，失败返回错误码
 */
int merge_sort_divide_and_conquer(int *array_to_sort, int *auxiliary_workspace, 
                                  int range_start, int range_end) {
    // 参数有效性检查
    if (array_to_sort == NULL || auxiliary_workspace == NULL) {
        printf("Error: Invalid parameters in %s\n", __func__);
        return PARAM_ERROR;
    }

    // 递归终止条件：子数组长度小于等于1（已经有序）
    if (range_start >= range_end) {
        return SUCCESS;
    }

    // 计算分割中点，使用安全的中点计算方式避免整数溢出
    int division_midpoint = range_start + (range_end - range_start) / 2;

#ifdef DEBUG
    printf("分割数组 [%d-%d] 为左半部分 [%d-%d] 和右半部分 [%d-%d]\n", 
           range_start, range_end, range_start, division_midpoint, division_midpoint + 1, range_end);
#endif

    // 递归处理左半部分：分而治之的"分"
    merge_sort_divide_and_conquer(array_to_sort, auxiliary_workspace, range_start, division_midpoint);

    // 递归处理右半部分：分而治之的"分"
    merge_sort_divide_and_conquer(array_to_sort, auxiliary_workspace, division_midpoint + 1, range_end);

    // 合并两个已排序的子数组：分而治之的"治"
    merge_two_sorted_subarrays(array_to_sort, auxiliary_workspace, 
                               range_start, division_midpoint, 
                               division_midpoint + 1, range_end);

    return SUCCESS;
}


/**
 * 归并排序主入口函数 - 对外公开接口
 * @param input_array: 待排序的整数数组
 * @param array_length: 数组元素个数
 * @return: 成功返回SUCCESS，失败返回错误码
 */
int merge_sort_main_entry(int *input_array, int array_length) {
    // 输入参数有效性检查
    if (input_array == NULL || array_length <= 0) {
        printf("Error: Invalid parameters in %s\n", __func__);
        return PARAM_ERROR;
    }

    // 为合并操作分配辅助内存空间
    int *auxiliary_memory_space = (int *)malloc(array_length * sizeof(int));
    if (auxiliary_memory_space == NULL) {
        printf("Error: Failed to allocate auxiliary memory in %s\n", __func__);
        return MEMORY_ERROR;
    }

    // 启动分治递归排序过程
    int sorting_result = merge_sort_divide_and_conquer(input_array, auxiliary_memory_space, 
                                                      0, array_length - 1);

    // 清理辅助内存空间，防止内存泄漏
    free(auxiliary_memory_space);
    auxiliary_memory_space = NULL;

    return sorting_result;
}


/**
 * 格式化输出数组内容的辅助函数
 * @param array_to_display: 要显示的数组指针
 * @param element_count: 数组元素个数
 * @param display_title: 显示标题说明
 */
void display_array_contents(int *array_to_display, int element_count, const char *display_title) {
    printf("%s: ", display_title);
    for (int element_index = 0; element_index < element_count; element_index++) {
        printf("%d ", array_to_display[element_index]);
    }
    printf("\n");
}

/**
 * 程序主入口 - 归并排序测试演示
 */
int main(int argc, char **argv) {
    // 定义测试数据集
    int demonstration_array[] = {7, 3, 5, 8, 0, 9, 1, 2, 4, 6};
    int total_elements = sizeof(demonstration_array) / sizeof(demonstration_array[0]);

    // 显示排序前的数组状态
    display_array_contents(demonstration_array, total_elements, "排序前的数组");

    // 执行归并排序算法
    int operation_result = merge_sort_main_entry(demonstration_array, total_elements);
    
    // 根据排序结果显示相应信息
    if (operation_result == SUCCESS) {
        // 显示排序后的数组状态
        display_array_contents(demonstration_array, total_elements, "排序后的数组");
    } else {
        printf("排序操作失败，错误代码: %d\n", operation_result);
    }

    return 0;
}
