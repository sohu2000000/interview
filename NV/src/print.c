/* 设置第n位为1 */
static inline uint32_t set_bit(uint32_t num, int n) {
    return num | (1U << n);
}

/* 清除第n位为0 */
static inline uint32_t clear_bit(uint32_t num, int n) {
    return num & ~(1U << n);
}

/* 翻转第n位 */
static inline uint32_t toggle_bit(uint32_t num, int n) {
    return num ^ (1U << n);
}

/* 检测第n位是否为1 */
static inline bool test_bit(uint32_t num, int n) {
    return (num & (1U << n)) != 0;
}

static inline uint32_t get_lowest_set_bit(uint32_t num) {
    return num & (-num);
}

static inline uint32_t clear_lowest_set_bit(uint32_t num) {
    return num & (num - 1);
}

int find_lowest_set_bit_position(uint32_t num) {
    if (num == 0) return -1;  // 无设置位
    
    // 方法1：使用内置函数（最高效）
    #ifdef __GNUC__
    return __builtin_ctz(num);  // Count Trailing Zeros - 直接计算末尾零个数
    #else
    // 方法2：使用我们自己实现的count_trailing_zeros函数
    return count_trailing_zeros(num);
    #endif
}

int find_highest_set_bit_position(uint32_t num) {
    if (num == 0) return -1;  // 无设置位
    
    // 方法1：使用内置函数（最高效）
    #ifdef __GNUC__
    return 31 - __builtin_clz(num);  // 31 - Count Leading Zeros
    #else
    // 方法2：手工实现前导零计数
    return 31 - count_leading_zeros(num);
    #endif
}

static inline bool is_power_of_two(uint32_t num) {
    return num > 0 && (num & (num - 1)) == 0;
}

#define ROUND_UP(x, align) ((((x) - 1) | ((align) - 1)) + 1)
#define PAGE_SIZE 4096
#define PAGE_ALIGN(addr) ROUND_UP(addr, PAGE_SIZE)

#define ALIGN(x, a) (((x) + (a) - 1) & ~((a) - 1))
#define ALIGN_4(x) ALIGN(x, 4)

void xor_swap(int* a, int* b) {
    if (a != b) {  // 防止同一地址的情况
        *a ^= *b;  // a = a₀ ^ b₀
        *b ^= *a;  // b = b₀ ^ (a₀ ^ b₀) = a₀
        *a ^= *b;  // a = (a₀ ^ b₀) ^ a₀ = b₀
    }
}

int find_single_number(int arr[], int n) {
    int result = 0;
    for (int i = 0; i < n; i++) {
        result ^= arr[i];  // 所有偶数次数字会被抵消，只剩奇数次的
    }
    return result;
}

typedef struct xor_node {
    int data;
    struct xor_node* npx;  // XOR of next and previous pointers
} xor_node_t;

xor_node_t* xor_pointers(xor_node_t* a, xor_node_t* b) {
    return (xor_node_t*)((uintptr_t)a ^ (uintptr_t)b);
}

int count_set_bits_kernighan(uint32_t num) {
    int count = 0;
    while (num) {
        num &= (num - 1);  // 清除最右边的1
        count++;
    }
    return count;
}

int count_leading_zeros(uint32_t num) {
    if (num == 0) return 32;  // 特殊情况：全0
    
    int count = 0;
    
    if (!(num & 0xFFFF0000)) { count += 16; num <<= 16; }  // 检查高16位
    if (!(num & 0xFF000000)) { count += 8;  num <<= 8;  }  // 检查高8位
    if (!(num & 0xF0000000)) { count += 4;  num <<= 4;  }  // 检查高4位
    if (!(num & 0xC0000000)) { count += 2;  num <<= 2;  }  // 检查高2位
    if (!(num & 0x80000000)) { count += 1; }              // 检查最高位
    
    return count;
}

int count_trailing_zeros(uint32_t num) {
    if (num == 0) return 32;  // 特殊情况：全0
    
    int count = 0;
    
    // 二分查找：逐步缩小范围（从低位开始）
    if ((num & 0xFFFF) == 0) { count += 16; num >>= 16; }  // 检查低16位
    if ((num & 0xFF) == 0)   { count += 8;  num >>= 8;  }  // 检查低8位
    if ((num & 0xF) == 0)    { count += 4;  num >>= 4;  }  // 检查低4位
    if ((num & 0x3) == 0)    { count += 2;  num >>= 2;  }  // 检查低2位
    if ((num & 0x1) == 0)    { count += 1; }              // 检查最低位
    
    return count;
}

static inline int mod_power_of_2(int x, int power_of_2) {
    return x & (power_of_2 - 1);
}

int fast_log2(uint32_t x) {
    if (x == 0) return -1;  // log₂(0)未定义
    return 31 - count_leading_zeros(x);
}

typedef struct { /* 位图操作 */
    uint32_t* bits;
    int size;
} bitmap_t;

void bitmap_set(bitmap_t* bmp, int index) {
    int word_index = index / 32;
    int bit_index = index % 32;
    bmp->bits[word_index] |= (1U << bit_index);
}

void bitmap_clear(bitmap_t* bmp, int index) {
    int word_index = index / 32;
    int bit_index = index % 32;
    bmp->bits[word_index] &= ~(1U << bit_index);
}

bool bitmap_test(bitmap_t* bmp, int index) {
    int word_index = index / 32;
    int bit_index = index % 32;
    return (bmp->bits[word_index] & (1U << bit_index)) != 0;
}

uint32_t round_up_to_power_of_two(uint32_t num) {
	if (num == 0) return 1;
	
	num--;                // 处理已经是2的幂的情况
	num |= num >> 1;      // 填充右边1位
	num |= num >> 2;      // 填充右边2位
	num |= num >> 4;      // 填充右边4位
	num |= num >> 8;      // 填充右边8位
	num |= num >> 16;     // 填充右边16位
	num++;                // 得到2的幂
	
	return num;
}

void find_two_single_numbers(int arr[], int n, int* num1, int* num2) {
	int xor_all = 0;
	
	for (int i = 0; i < n; i++) { // 步骤1：获取两个目标数字的XOR结果
	    xor_all ^= arr[i];
	}
	
	int rightmost_set_bit = xor_all & (-xor_all); // 步骤2：找到最右边的设置位（分组依据）
	
	*num1 = 0;
	*num2 = 0;
       
	for (int i = 0; i < n; i++) {  // 步骤3：根据该位分组，每组内使用单数字XOR算法
	    if (arr[i] & rightmost_set_bit) {
		*num1 ^= arr[i];  // 该位为1的数字分到第一组
	    } else {
		*num2 ^= arr[i];  // 该位为0的数字分到第二组
	    }
	}
}

int abs_no_branch(int x) {
	int mask = x >> 31;        // 创建符号掩码
	return (x + mask) ^ mask;  // 条件绝对值计算
    }
    
    int max_no_branch(int a, int b) {
	int diff = a - b;
	int mask = (diff >> 31) & 1;  // 如果a < b，mask为1；否则为0
	return a - (diff & mask);
    }
    
    bool same_sign_no_branch(int a, int b) {
	return ((a ^ b) & 0x80000000) == 0;
}

int insertion_sort(int *numbers, int length) {
	for (int current_index = 1; current_index < length; current_index++) {
	    int current_value = numbers[current_index];  // 当前要插入的值
	    int insert_position = current_index;         // 插入位置
    
	    while (insert_position > 0 && numbers[insert_position - 1] > current_value) {
		numbers[insert_position] = numbers[insert_position - 1];
		insert_position--;
	    }
    
	    numbers[insert_position] = current_value;
    
	return SUCCESS;
    }
}

int shell_sort(int *numbers, int array_size) {
	for (int gap = array_size / 2; gap > 0; gap--) {
	    for (int group_start = 0; group_start < gap; group_start++) {
		for (int current_index = group_start + gap; current_index < array_size; current_index += gap) {
		    int current_value = numbers[current_index];
		    int insert_position = current_index;
		    while (insert_position >= gap && numbers[insert_position - gap] > current_value) {
			numbers[insert_position] = numbers[insert_position - gap];
			insert_position -= gap;
		    }
		    numbers[insert_position] = current_value;
		}
	    }
	}
	return SUCCESS;
}

int BubbleSort(int *array, int size) {
    int sorted_boundary = 0;  // 有序区域边界
    int current_pos = 0;      // 当前比较位置
    bool swapped = false;     // 是否发生过交换
    
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

        if (!swapped) {
            printf("排序提前完成于第%d轮\n", sorted_boundary + 1);
            break;
        }
    }

    return SUCCESS;
}

int quick_sort(int *numbers, int start_index, int end_index) {
	if (start_index >= end_index)
	    return SUCCESS;
    
	int left_pointer = start_index;    // 左指针
	int right_pointer = end_index;     // 右指针
	int pivot_value = numbers[start_index];  // 选择第一个元素作为基准值
    
	while (left_pointer < right_pointer) {
	    
	    while (left_pointer < right_pointer && numbers[right_pointer] >= pivot_value) {
		right_pointer--;
	    }
	    if (left_pointer < right_pointer) {
		numbers[left_pointer] = numbers[right_pointer];
		left_pointer++;
	    }
    
	    while (left_pointer < right_pointer && numbers[left_pointer] <= pivot_value) {
		left_pointer++;
	    }
	    if (left_pointer < right_pointer) {
		numbers[right_pointer] = numbers[left_pointer];
		right_pointer--;
	    }
	}
    
	numbers[left_pointer] = pivot_value;
	int pivot_position = left_pointer;
    
	quick_sort(numbers, start_index, pivot_position - 1);
	quick_sort(numbers, pivot_position + 1, end_index);
    
	return SUCCESS;
}

