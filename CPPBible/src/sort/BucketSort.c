#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SUCCESS         0
#define PARAM_ERR      -1

#define BUCKET_COUNT   10      // 桶的数量（对应数字0-9）
#define MAX_BUCKET_SIZE 100    // 每个桶的最大容量

/**
 * 桶结构定义
 * 用于基数排序中存储特定数位的元素
 */
typedef struct radix_bucket {
    int elements[MAX_BUCKET_SIZE];  // 桶中的元素数组
    int count;                      // 桶中当前元素数量
} radix_bucket_t;

/**
 * 全局桶数组
 * 10个桶分别对应数字0-9，用于按位数分类
 */
radix_bucket_t g_buckets[BUCKET_COUNT];

/**
 * 初始化所有桶
 * 将每个桶的元素计数重置为0，准备新一轮的分配
 */
void initialize_buckets(void) {
    for (int i = 0; i < BUCKET_COUNT; i++) {
        g_buckets[i].count = 0;
    }
}


/**
 * 找到数组中的最大值
 * @param array 输入数组
 * @param size 数组大小
 * @return 数组中的最大值，如果数组为空返回0
 */
int find_maximum_value(int *array, int size) {
    if (array == NULL || size <= 0) {
        printf("%s: parameter error - invalid array or size\n", __func__);
        return 0;
    }

    int max_value = array[0];
    for (int i = 1; i < size; i++) {
        if (array[i] > max_value) {
            max_value = array[i];
        }
    }

    return max_value;
}


/**
 * 计算数字的位数
 * @param number 要计算位数的数字
 * @return 数字的位数（例如：123返回3，5返回1）
 */
int get_digit_count(int number) {
    if (number == 0) {
        return 1;  // 0是1位数
    }
    
    int digit_count = 0;
    int temp = number;
    
    // 处理负数（如果需要支持）
    if (temp < 0) {
        temp = -temp;
    }
    
    while (temp > 0) {
        digit_count++;
        temp /= 10;
    }

    return digit_count;
}


/**
 * 将数组元素按指定位数分散到对应的桶中
 * @param array 待排序数组
 * @param size 数组大小
 * @param digit_position 当前处理的位数（1=个位，2=十位，3=百位...）
 * @return SUCCESS(0) 成功，PARAM_ERR(-1) 参数错误
 */
int distribute_to_buckets(int *array, int size, int digit_position) {
    if (array == NULL) {
        printf("%s: parameter error - array is NULL\n", __func__);
        return PARAM_ERR;
    }
    
    if (size <= 0) {
        printf("%s: parameter error - invalid size %d\n", __func__, size);
        return PARAM_ERR;
    }

    // 遍历数组中的每个元素
    for (int i = 0; i < size; i++) {
        // 提取指定位数的数字作为桶索引
        int number = array[i];
        int bucket_index = 0;
        
        // 提取第digit_position位的数字
        for (int d = 1; d <= digit_position; d++) {
            bucket_index = number % 10;
            number /= 10;
        }

        // 检查桶是否已满（安全检查）
        if (g_buckets[bucket_index].count >= MAX_BUCKET_SIZE) {
            printf("%s: bucket %d is full, cannot add more elements\n", 
                   __func__, bucket_index);
            return PARAM_ERR;
        }

        // 将元素放入对应的桶中
        g_buckets[bucket_index].elements[g_buckets[bucket_index].count] = array[i];
        g_buckets[bucket_index].count++;
    }

    return SUCCESS;
}


/**
 * 从桶中按顺序收集元素回到原数组
 * @param array 目标数组（用于存放排序结果）
 * @param size 数组大小
 * @return SUCCESS(0) 成功，PARAM_ERR(-1) 参数错误
 */
int collect_from_buckets(int *array, int size) {
    if (array == NULL) {
        printf("%s: parameter error - array is NULL\n", __func__);
        return PARAM_ERR;
    }
    
    if (size <= 0) {
        printf("%s: parameter error - invalid size %d\n", __func__, size);
        return PARAM_ERR;
    }

    int array_index = 0;

    // 按桶的顺序（0-9）收集元素，实现升序排列
    for (int bucket_id = 0; bucket_id < BUCKET_COUNT; bucket_id++) {
        int bucket_count = g_buckets[bucket_id].count;
        
        // 将当前桶中的所有元素复制回原数组
        for (int j = 0; j < bucket_count; j++) {
            if (array_index >= size) {
                printf("%s: array overflow detected\n", __func__);
                return PARAM_ERR;
            }
            array[array_index++] = g_buckets[bucket_id].elements[j];
        }
    }

    return SUCCESS;
}


/**
 * 基数排序主函数（LSD - 最低位优先）
 * 从最低位开始，逐位进行桶排序，直到处理完所有位
 * @param array 待排序数组
 * @param size 数组大小
 * @return SUCCESS(0) 成功，PARAM_ERR(-1) 参数错误
 */
int radix_sort(int *array, int size) {
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

    // 找到数组中的最大值
    int max_value = find_maximum_value(array, size);
    if (max_value < 0) {
        printf("%s: error finding maximum value\n", __func__);
        return PARAM_ERR;
    }

    // 计算最大值的位数，决定排序轮数
    int total_rounds = get_digit_count(max_value);
    
    printf("开始基数排序：最大值=%d，需要%d轮排序\n", max_value, total_rounds);

    // 从个位开始，逐位进行桶排序
    for (int current_digit = 1; current_digit <= total_rounds; current_digit++) {
        // 重新初始化所有桶
        initialize_buckets();

        // 将元素按当前位数分散到桶中
        int scatter_result = distribute_to_buckets(array, size, current_digit);
        if (scatter_result != SUCCESS) {
            printf("%s: failed to distribute elements in round %d\n", 
                   __func__, current_digit);
            return scatter_result;
        }

        // 从桶中按顺序收集元素
        int gather_result = collect_from_buckets(array, size);
        if (gather_result != SUCCESS) {
            printf("%s: failed to collect elements in round %d\n", 
                   __func__, current_digit);
            return gather_result;
        }

#ifdef DEBUG
        printf("============= 第%d轮排序（%s位） =========== \n", 
               current_digit, 
               current_digit == 1 ? "个" : 
               current_digit == 2 ? "十" : 
               current_digit == 3 ? "百" : "高");
               
        printf("按第%d位分桶结果:\n", current_digit);
        
        // 显示每个桶的内容
        for (int bucket_id = 0; bucket_id < BUCKET_COUNT; bucket_id++) {
            printf("\t桶[%d]: [", bucket_id);
            int bucket_count = g_buckets[bucket_id].count;
            for (int j = 0; j < bucket_count; j++) {
                printf("%d", g_buckets[bucket_id].elements[j]);
                if (j < bucket_count - 1) printf(", ");
            }
            printf("]\n");
        }
        
        printf("收集后的数组: [");
        for (int k = 0; k < size; k++) {
            printf("%d", array[k]);
            if (k < size - 1) printf(", ");
        }
        printf("]\n");
        printf("========================================\n\n");
#endif
    }

    printf("基数排序完成，共进行了%d轮\n", total_rounds);
    return SUCCESS;
}

// 保持向后兼容的别名
int BucketSort(int *array, int size) {
    return radix_sort(array, size);
}


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

int main(int argc, char **argv) {
    printf("🔢 基数排序测试程序\n");
    printf("==================\n\n");
    
    // 测试数组1：原始测试用例
    printf("【测试1: 原始测试数据】\n");
    int array1[10] = {22, 32, 19, 53, 0, 47, 29, 116, 4, 6};
    print_array("排序前", array1, 10);
    
    int result1 = BucketSort(array1, 10);  // 使用兼容的函数名
    if (result1 == SUCCESS) {
        print_array("排序后", array1, 10);
        printf("排序验证: %s\n", is_array_sorted(array1, 10) ? "✅ 正确" : "❌ 错误");
    }
    
    // 测试数组2：包含重复元素
    printf("\n【测试2: 重复元素测试】\n");
    int array2[8] = {123, 45, 67, 45, 89, 12, 123, 1};
    print_array("排序前", array2, 8);
    
    int result2 = radix_sort(array2, 8);
    if (result2 == SUCCESS) {
        print_array("排序后", array2, 8);
        printf("排序验证: %s\n", is_array_sorted(array2, 8) ? "✅ 正确" : "❌ 错误");
    }
    
    // 测试数组3：边界情况
    printf("\n【测试3: 边界情况】\n");
    
    // 单元素数组
    int single[1] = {42};
    printf("单元素数组测试: ");
    int single_result = radix_sort(single, 1);
    printf("返回值=%d %s\n", single_result, single_result == SUCCESS ? "✅" : "❌");
    
    // 空指针测试
    printf("空指针测试: ");
    int null_result = radix_sort(NULL, 5);
    printf("返回值=%d %s\n", null_result, null_result == PARAM_ERR ? "✅" : "❌");
    
    // 包含0的数组
    printf("包含0的数组: ");
    int with_zero[5] = {0, 5, 0, 3, 0};
    print_array("", with_zero, 5);
    int zero_result = radix_sort(with_zero, 5);
    if (zero_result == SUCCESS) {
        print_array("排序后", with_zero, 5);
    }
    
    printf("\n🎉 所有测试完成！\n");
    return 0;
}



