#include <stdio.h>
#include <stdlib.h>

#define SUCCESS      0
#define PARAM_ERROR -1

/**
 * 希尔排序函数 - 改进的插入排序
 * 
 * 核心思想：
 * 通过逐渐减小的间隔对数组进行多次"间隔插入排序"
 * 最后一次是标准插入排序（间隔=1），确保完全有序
 * 
 * @param numbers: 待排序的整数数组
 * @param array_size: 数组长度
 * @return: 成功返回SUCCESS，参数错误返回PARAM_ERROR
 */
int shell_sort(int *numbers, int array_size) {
    if (numbers == NULL) {
        printf("Error: Invalid parameters in %s\n", __func__);
        return PARAM_ERROR;
    }

    /*
     * 希尔排序的分组机制：
     * 
     * 重要理解：分组是按间隔跳跃分组的！
     * 例如间隔=3时：
     * 第0组：位置 0, 3, 6, 9, ...
     * 第1组：位置 1, 4, 7, 10, ...  
     * 第2组：位置 2, 5, 8, 11, ...
     * 
     * 不是连续分组[0-2], [3-5], [6-8]...
     */

    // 间隔从数组长度的一半开始，逐步减小到1
    for (int gap = array_size / 2; gap > 0; gap--) {
        
#ifdef DEBUG
        printf("\n========== 当前间隔 = %d ==========\n", gap);
#endif

        // 对每个间隔组进行插入排序
        for (int group_start = 0; group_start < gap; group_start++) {
            
#ifdef DEBUG
            printf("\n------ 处理第%d组（起始位置%d，间隔%d）------\n", 
                   group_start, group_start, gap);
#endif

            /*
             * 对当前组进行改进的插入排序
             * 
             * 改进方式：边比较边移动，类似标准插入排序的优化写法
             * 相比原来的"查找位置→移动元素→插入"三步式，
             * 改为"边比较边移动"的一体化处理，更简洁高效
             * 
             * 组内元素的位置：group_start, group_start+gap, group_start+2*gap, ...
             */
            for (int current_index = group_start + gap; current_index < array_size; current_index += gap) {
                int current_value = numbers[current_index];  // 保存当前要插入的值
                int insert_position = current_index;         // 从当前位置开始向前查找

                /*
                 * 改进的插入逻辑：
                 * 1. 边比较边移动，将大于current_value的元素直接后移
                 * 2. 一旦找到合适位置或到达边界，立即停止
                 * 3. 减少了额外的位置记录和重复赋值
                 */
                while (insert_position >= gap && numbers[insert_position - gap] > current_value) {
                    // 直接将前面的大元素后移gap个位置
                    numbers[insert_position] = numbers[insert_position - gap];
                    insert_position -= gap;
                }

                // 将当前值插入到最终找到的正确位置
                numbers[insert_position] = current_value;
            }

#ifdef DEBUG
            // 显示当前组排序后的状态
            printf("第%d组排序后: [", group_start);
            for (int k = group_start; k < array_size; k += gap) {
                printf(" %d ", numbers[k]);
            }
            printf("]\n");
#endif
        }

#ifdef DEBUG
        // 显示当前间隔完成后的整个数组状态
        printf("\n间隔%d完成后的数组: ", gap);
        for (int k = 0; k < array_size; k++) {
            printf("%d ", numbers[k]);
        }
        printf("\n");
#endif
    }

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
 * 主函数 - 测试希尔排序
 */
int main(int argc, char **argv) {
    int test_array[] = {7, 3, 5, 8, 0, 9, 1, 2, 4, 6};
    int array_size = sizeof(test_array) / sizeof(test_array[0]);

    // 显示排序前的数组
    print_array(test_array, array_size, "希尔排序前");

    // 执行希尔排序
    int result = shell_sort(test_array, array_size);
    
    if (result == SUCCESS) {
        // 显示排序后的数组
        print_array(test_array, array_size, "希尔排序后");
    } else {
        printf("排序失败，错误代码: %d\n", result);
    }

    return 0;
}
