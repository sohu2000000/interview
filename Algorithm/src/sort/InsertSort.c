#include <stdio.h>
#include <stdlib.h>

#define SUCCESS     0
#define PARAM_ERROR -1

/**
 * 插入排序函数 - 将数组按升序排列
 * @param numbers: 待排序的整数数组
 * @param length: 数组长度
 * @return: 成功返回SUCCESS，参数错误返回PARAM_ERROR
 */
int insertion_sort(int *numbers, int length) {
    // 参数检查
    if (numbers == NULL || length <= 0) {
        printf("Error: Invalid parameters in %s\n", __func__);
        return PARAM_ERROR;
    }

    // 从第二个元素开始，因为第一个元素本身就是有序的
    for (int current_index = 1; current_index < length; current_index++) {
        int current_value = numbers[current_index];  // 当前要插入的值
        int insert_position = current_index;         // 插入位置

        // 在已排序部分从右向左查找插入位置
        // 同时将大于current_value的元素向右移动
        while (insert_position > 0 && numbers[insert_position - 1] > current_value) {
            numbers[insert_position] = numbers[insert_position - 1];
            insert_position--;
        }

        // 将当前值插入到正确位置
        numbers[insert_position] = current_value;

#ifdef DEBUG
        // 调试信息：显示每轮排序后的数组状态
        printf("Round %d: ", current_index);
        for (int k = 0; k < length; k++) {
            if (k <= current_index) {
                printf("[%d] ", numbers[k]);  // 已排序部分
            } else {
                printf(" %d  ", numbers[k]);  // 未排序部分
            }
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

int main(int argc, char **argv) {
    // 测试已排序数组（最好情况）
    // 7 3 5 8 0 9 1 2 4 6
    int test_array[] = {7, 3, 5, 8, 0, 9, 1, 2, 4, 6};
    int array_size = sizeof(test_array) / sizeof(test_array[0]);

    // 显示排序前的数组
    print_array(test_array, array_size, "Before sorting");

    // 执行插入排序
    int result = insertion_sort(test_array, array_size);
    
    if (result == SUCCESS) {
        // 显示排序后的数组
        print_array(test_array, array_size, "After sorting");
    } else {
        printf("Sorting failed with error code: %d\n", result);
    }

    return 0;
}


