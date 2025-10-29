#include <stdio.h>
#include <stdlib.h>

#define SUCCESS       0
#define PARAM_ERROR  -1

/**
 * 交换宏 - 使用临时变量交换两个整数的值
 * @param a: 第一个变量
 * @param b: 第二个变量
 * 
 * 注意：
 * 1. 使用do-while(0)包裹，确保宏在任何地方都能正确使用
 * 2. 临时变量使用独特的名字避免命名冲突
 * 3. 适用于基本类型（int, char等）
 */
#define SWAP(a, b) do { \
    typeof(a) _swap_temp_ = (a); \
    (a) = (b); \
    (b) = _swap_temp_; \
} while(0)

/**
 * 在无序区查找最小元素的位置 - 选择排序的核心操作
 * @param source_array: 源数组
 * @param unsorted_region_start: 无序区域起始位置
 * @param unsorted_region_end: 无序区域结束位置
 * @return: 最小元素的索引位置，错误返回PARAM_ERROR
 */
int find_minimum_element_index(int *source_array, 
                               int unsorted_region_start, 
                               int unsorted_region_end) {
    // 参数有效性检查
    if (source_array == NULL || unsorted_region_start > unsorted_region_end) {
        printf("Error: Invalid parameters in %s\n", __func__);
        return PARAM_ERROR;
    }
    
    int minimum_value_index = unsorted_region_start;  // 假设第一个元素最小
    int minimum_value = source_array[unsorted_region_start];  // 记录最小值
    
    // 遍历无序区间[unsorted_region_start, unsorted_region_end]
    // 注意：必须是 <= unsorted_region_end，因为end是有效索引（n-1）
    for (int scan_index = unsorted_region_start; scan_index <= unsorted_region_end; scan_index++) {
        if (source_array[scan_index] < minimum_value) {
            minimum_value_index = scan_index;  // 更新最小值位置
            minimum_value = source_array[scan_index];  // 更新最小值
        }
    }
    
    return minimum_value_index;
}

/**
 * 选择排序主函数 - 逐步选择最小值构建有序区
 * @param array_to_sort: 待排序的整数数组
 * @param array_length: 数组元素个数
 * @return: 成功返回SUCCESS，失败返回PARAM_ERROR
 * 
 * 算法思想：
 * 将数组分为有序区和无序区两部分
 * 每次从无序区选择最小元素，放到有序区的末尾
 * 重复n-1次后完成排序
 */
int selection_sort_main_entry(int *array_to_sort, int array_length) {
    // 参数有效性检查
    if (array_to_sort == NULL || array_length <= 0) {
        printf("Error: Invalid parameters in %s\n", __func__);
        return PARAM_ERROR;
    }

    int sorted_boundary_index = 0;          // 有序区和无序区的分界点
    int minimum_element_position = 0;       // 无序区中最小元素的位置

#ifdef DEBUG
    int display_index = 0;  // DEBUG模式下用于显示数组的索引
#endif

    // 外层循环：逐步扩大有序区，缩小无序区
    // sorted_boundary_index 是有序区的右边界（下一个待填充位置）
    for (sorted_boundary_index = 0; sorted_boundary_index < array_length; sorted_boundary_index++) {
        
        // 在无序区 [sorted_boundary_index, array_length-1] 中查找最小值位置
        minimum_element_position = find_minimum_element_index(array_to_sort, 
                                                              sorted_boundary_index, 
                                                              array_length - 1);

        // 将找到的最小值交换到有序区的末尾（sorted_boundary_index位置）
        // 优化：只在需要时才交换（最小值不在正确位置时）
        if (minimum_element_position != sorted_boundary_index) {
            SWAP(array_to_sort[minimum_element_position], array_to_sort[sorted_boundary_index]);
        }

#ifdef DEBUG
        printf("第%d轮选择：最小值=%d，位置=%d\n", 
               sorted_boundary_index + 1, array_to_sort[sorted_boundary_index], minimum_element_position);
        
        printf("有序区 [");
        // 显示有序区域：[0, sorted_boundary_index]
        for (display_index = 0; display_index <= sorted_boundary_index; display_index++) {
            printf("%d", array_to_sort[display_index]);
            if (display_index < sorted_boundary_index) printf(", ");
        }
        printf("]  ");

        printf("无序区 [");
        // 显示无序区域：[sorted_boundary_index+1, array_length-1]
        for (display_index = sorted_boundary_index + 1; display_index < array_length; display_index++) {
            printf("%d", array_to_sort[display_index]);
            if (display_index < array_length - 1) printf(", ");
        }
        printf("]\n\n");
#endif
    }

    return SUCCESS;
}


/**
 * 格式化输出数组内容的辅助函数
 * @param array_to_display: 要显示的数组
 * @param element_count: 数组元素个数
 * @param display_title: 显示标题
 */
void display_array_contents(int *array_to_display, int element_count, const char *display_title) {
    printf("%s: ", display_title);
    for (int element_index = 0; element_index < element_count; element_index++) {
        printf("%d", array_to_display[element_index]);
        if (element_index < element_count - 1) printf(", ");
    }
    printf("\n");
}

/**
 * 程序主入口 - 选择排序测试演示
 */
int main(int argc, char **argv) {
    // 定义测试数据集
    int demonstration_array[] = {7, 3, 5, 8, 0, 9, 1, 2, 4, 6};
    int total_elements = sizeof(demonstration_array) / sizeof(demonstration_array[0]);

    // 显示排序前的数组状态
    display_array_contents(demonstration_array, total_elements, "排序前的数组");

    // 执行选择排序算法
    int operation_result = selection_sort_main_entry(demonstration_array, total_elements);
    
    // 根据排序结果显示相应信息
    if (operation_result == SUCCESS) {
        // 显示排序后的数组状态
        display_array_contents(demonstration_array, total_elements, "排序后的数组");
    } else {
        printf("排序操作失败，错误代码: %d\n", operation_result);
    }

    return 0;
}
