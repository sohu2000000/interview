#include <stdio.h>
#include <stdlib.h>

#define SUCCESS      0
#define PARAM_ERROR -1

/**
 * 快速排序主函数 - 分治算法实现
 * @param numbers: 待排序的整数数组
 * @param start_index: 排序区间的起始索引
 * @param end_index: 排序区间的结束索引
 * @return: 成功返回SUCCESS，参数错误返回PARAM_ERROR
 */
int quick_sort(int *numbers, int start_index, int end_index) {
    if (numbers == NULL) {
        printf("Error: Invalid parameters in %s\n", __func__);
        return PARAM_ERROR;
    }

    // 递归结束条件：子数组长度小于等于1
    if (start_index >= end_index) {
        return SUCCESS;
    }

    int left_pointer = start_index;    // 左指针
    int right_pointer = end_index;     // 右指针
    int pivot_value = numbers[start_index];  // 选择第一个元素作为基准值

#ifdef DEBUG
    printf("分区前：范围[%d, %d]，基准值=%d\n", start_index, end_index, pivot_value);
#endif

    /*
     * 分区过程：将数组分为三部分
     * [小于pivot的元素] [pivot] [大于pivot的元素]
     * 
     * "挖坑填坑"算法详解：
     * 
     * 核心思想：
     * 通过交替挖坑和填坑的方式，避免传统的三变量交换，实现高效的元素重排
     * 
     * 具体过程（以[54,38,96,23,0,15,72,60,45,83]为例）：
     * 
     * 初始状态：
     * [54, 38, 96, 23, 0, 15, 72, 60, 45, 83]
     *  ↑                                   ↑
     * left=0                            right=9
     * pivot_value = 54，left位置变成第一个"坑"
     * 
     * 第1轮挖坑填坑：
     * 步骤1 - 从右挖坑：
     *   从right向左找 < 54的元素 → 找到45
     *   用45填left位置的坑 → [45, 38, 96, 23, 0, 15, 72, 60, ?, 83]
     *   现在right=8位置变成新坑，left++
     * 
     * 步骤2 - 从左挖坑：  
     *   从left向右找 > 54的元素 → 找到96
     *   用96填right位置的坑 → [45, 38, ?, 23, 0, 15, 72, 60, 96, 83]
     *   现在left=2位置变成新坑，right--
     * 
     * 第2轮挖坑填坑：
     * 步骤3 - 从右挖坑：
     *   从right向左找 < 54的元素 → 找到15
     *   用15填left位置的坑 → [45, 38, 15, 23, 0, ?, 72, 60, 96, 83]
     *   现在right=5位置变成新坑，left++
     * 
     * 步骤4 - 从左挖坑：
     *   从left向右找 > 54的元素 → left已经到达right=5
     *   指针相遇，循环结束
     * 
     * 最终填坑：
     * 用基准值54填最后的坑(位置5)：
     * [45, 38, 15, 23, 0, 54, 72, 60, 96, 83]
     *                    ↑
     *                pivot的最终正确位置
     * 
     * "挖坑填坑"的巧妙之处：
     * 
     * 1. 避免三变量交换：
     *    传统交换：temp=a; a=b; b=temp; (3次赋值)
     *    挖坑填坑：a=b; (1次赋值)
     * 
     * 2. 减少内存访问：
     *    每次只有一次写操作，没有读-改-写的开销
     * 
     * 3. 逻辑清晰：
     *    总是有一个"坑"等待填充
     *    找到合适元素就填坑，同时产生新坑
     *    直到指针相遇，用基准值填最后的坑
     * 
     * 4. 坑的状态转移：
     *    初始: left位置是坑(保存了pivot)
     *    → 从right找元素填left坑 → right位置变成坑
     *    → 从left找元素填right坑 → left位置变成坑
     *    → 重复直到相遇 → 用pivot填最后的坑
     * 
     * 5. 分区不变式：
     *    过程中始终保持：[start...left-1] ≤ pivot ≤ [right+1...end]
     *    最终：pivot在正确位置，左边都≤pivot，右边都≥pivot
     */
    while (left_pointer < right_pointer) {
        
        // 从右向左找第一个小于基准值的元素
        while (left_pointer < right_pointer && numbers[right_pointer] >= pivot_value) {
            right_pointer--;
        }
        if (left_pointer < right_pointer) {
            numbers[left_pointer] = numbers[right_pointer];
            left_pointer++;
        }

        // 从左向右找第一个大于基准值的元素
        while (left_pointer < right_pointer && numbers[left_pointer] <= pivot_value) {
            left_pointer++;
        }
        if (left_pointer < right_pointer) {
            numbers[right_pointer] = numbers[left_pointer];
            right_pointer--;
        }
    }

    // 将基准值放到最终位置
    numbers[left_pointer] = pivot_value;
    int pivot_position = left_pointer;

#ifdef DEBUG
    printf("分区后：基准值%d的最终位置=%d\n", pivot_value, pivot_position);
    printf("小于区域: [");
    for (int k = start_index; k < pivot_position; k++) {
        printf(" %d ", numbers[k]);
    }
    printf("], 基准值: %d, 大于区域: [", numbers[pivot_position]);
    for (int k = pivot_position + 1; k <= end_index; k++) {
        printf(" %d ", numbers[k]);
    }
    printf("]\n\n");
#endif

    // 递归排序左半部分（小于基准值的元素）
    quick_sort(numbers, start_index, pivot_position - 1);
    
    // 递归排序右半部分（大于基准值的元素）
    quick_sort(numbers, pivot_position + 1, end_index);

    return SUCCESS;
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
 * 主函数 - 测试快速排序
 */
int main(int argc, char **argv) {
    int test_array[] = {54, 38, 96, 23, 0, 15, 72, 60, 45, 83};
    int array_size = sizeof(test_array) / sizeof(test_array[0]);

    // 显示排序前的数组
    print_array(test_array, array_size, "排序前");

    // 执行快速排序
    int result = quick_sort(test_array, 0, array_size - 1);
    
    if (result == SUCCESS) {
        // 显示排序后的数组
        print_array(test_array, array_size, "排序后");
    } else {
        printf("排序失败，错误代码: %d\n", result);
    }

    return 0;
}



