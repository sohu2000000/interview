
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "str.h"

/*
 * ============================================================================
 * 字符串处理函数库
 * ============================================================================
 * 
 * 本文件包含常用的字符串处理函数，按功能分类：
 * 1. 基础字符串操作：复制、比较、查找、连接
 * 2. 字符串转换：整数↔字符串、进制转换
 * 3. 字符串反转：完全反转、保持单词、保留子串
 * 4. 字符串操作：删除、替换、编码
 * 5. 特殊算法：大整数运算、最长公共子串
 */

/* ============================================================================
 * 【字符串类型转换函数】
 * ============================================================================ */

/**
 * 整数转字符串 - 将整数转换为十进制字符串
 * @param output_string: 输出字符串缓冲区
 * @param input_number: 要转换的整数（支持负数）
 * @return: 成功返回SUCCESS，失败返回PARAM_ERR
 * 
 * 算法：逐位提取 + 反转
 * 1. 处理符号，取绝对值
 * 2. 逐位提取数字（个位→高位）
 * 3. 反转数字顺序
 * 4. 添加负号（如果需要）
 */
int convert_string_from_integer(char *str, int num) {
	if (str == NULL) {
		printf("Error: int2str parameter error\n");
		return PARAM_ERR;
	}

	int is_negative = 0;
	char temp_buffer[100] = {0};
	int digit_count = 0;
	int buffer_index = 0;
	int output_index = 0;
	int current_digit = 0;
	int working_number = 0;
	int total_length = 0;

	// 处理负数
	if (num < 0) {
		is_negative = 1;
		working_number = -num;
	} else {
		working_number = num;
	}

	// 逐位提取数字（从个位到高位）
	do {
		current_digit = working_number % 10;
		temp_buffer[buffer_index++] = current_digit + '0';
		working_number = working_number / 10;
	} while (working_number != 0);

	digit_count = buffer_index;
	total_length = digit_count;

	// 添加负号
	output_index = 0;
	if (is_negative == 1) {
		str[output_index++] = '-';
		total_length++;
	}

	// 反转数字顺序
	for (int i = digit_count - 1; i >= 0; i--) {
		str[output_index++] = temp_buffer[i];
	}
	
	str[total_length] = '\0';
	
	return SUCCESS;
}


/**
 * 字符串转整数 - 将十进制字符串转换为整数
 * @param input_string: 输入字符串
 * @param result_number: 输出整数指针
 * @return: 成功返回SUCCESS
 * 
 * 算法：从字符串尾部向前解析
 * 1. 从个位开始，逐位解析
 * 2. 每位乘以对应的权值（1, 10, 100, ...）
 * 3. 累加到结果
 * 4. 遇到负号则取反
 */
int convert_string_to_integer(char *str, int *rst) {
	if (str == NULL || rst == NULL) {
		printf("Error: str2int parameter error\n");
		return PARAM_ERR;
	}

	int accumulated_value = 0;
	int digit_value = 0;
	int position_weight = 0;  // 当前位的权值（个位=0, 十位=1, ...）
	int power_multiplier = 0;
	int string_length = 0;

	string_length = strlen(str);

	// 从字符串尾部向前解析（从个位到高位）
	for (int char_index = string_length - 1; char_index >= 0; char_index--, position_weight++) {
		if (str[char_index] == '-') {
			accumulated_value = -accumulated_value;
			continue;
		}

		// 将字符转为数字
		digit_value = str[char_index] - '0';

		// 计算当前位的权值（10^position_weight）
		power_multiplier = position_weight;
		while (power_multiplier > 0) {
			digit_value = digit_value * 10;
			power_multiplier--;
		}
		
		accumulated_value = accumulated_value + digit_value;
	}

	*rst = accumulated_value;
	return SUCCESS;
}


int convert_string_to_integer_lib(char *str, int *rst) {
	if(NULL == str){
		printf("int2str param error\n");
		return PARAM_ERR;
	}

	*rst = atoi(str);

	return SUCCESS;
}

int convert_string_to_float_lib(char *str, float *rst) {
	if(NULL == str){
		printf("int2str param error\n");
		return PARAM_ERR;
	}

	*rst = atof(str);

	return SUCCESS;
}


/* ============================================================================
 * 【基础字符串操作函数】
 * ============================================================================ */

/**
 * 字符串复制 - 将源字符串复制到目标缓冲区
 * @param destination: 目标缓冲区
 * @param source: 源字符串
 * @return: 成功返回SUCCESS
 */
int copy_string_to_buffer(char *dst, char *src) {
	if (dst == NULL || src == NULL) {
		printf("Error: mystrcpy parameter error\n");
		return PARAM_ERR;
	}	

	char *dest_ptr = dst;
	char *src_ptr = src;
	
	while (*src_ptr != '\0') {
		*dest_ptr++ = *src_ptr++;
	}
	*dest_ptr = '\0';

	return SUCCESS;
}

/**
 * 内存复制 - 复制指定字节数的内存
 * @param destination: 目标内存区域
 * @param source: 源内存区域
 * @param byte_count: 要复制的字节数
 * @return: 返回目标指针
 */
void *copy_memory_bytes(void *dst, void *src, int size) {
	if (dst == NULL || src == NULL) {
		printf("Error: mymemcpy parameter error\n");
		return NULL;
	}

	char *dest_ptr = (char *)dst;
	char *src_ptr = (char *)src;

	while (size > 0) {
		*dest_ptr++ = *src_ptr++;
		size--;
	}

	return dst;
}

/**
 * 字符串长度 - 计算字符串长度（不含'\0'）
 * @param input_string: 输入字符串
 * @return: 字符串长度
 */
int get_string_length(char *str) {
	if (str == NULL) {
		printf("Error: mystrlen parameter error\n");
		return 0;
	}

	char *char_ptr = str;
	int length = 0;

	while (*char_ptr != '\0') {
		length++;
		char_ptr++;
	}

	return length;
}

/**
 * 子串查找 - 在主字符串中查找子串
 * @param main_string: 主字符串
 * @param substring: 要查找的子串
 * @return: 找到返回位置指针，否则返回NULL
 * 
 * 算法：暴力匹配
 */
char *find_string_substring(char *str, char *sub) {
	if (str == NULL || sub == NULL) {
		printf("Error: mystrstr parameter error\n");
		return NULL;
	}

	char *main_ptr = str;
	char *sub_ptr = NULL;
	char *match_ptr = NULL;
	int is_found = 0;

	while (*main_ptr != '\0') {
		sub_ptr = sub;
		match_ptr = main_ptr;
		
		// 尝试匹配
		while (*match_ptr == *sub_ptr && *sub_ptr != '\0') {
			match_ptr++;
			sub_ptr++;
		}

		// 匹配成功
		if (*sub_ptr == '\0') {
			is_found = 1;
			break;			
		}
		main_ptr++;
	}

	return is_found ? main_ptr : NULL;
}



/* ============================================================================
 * 【字符串反转函数】
 * ============================================================================ */

/**
 * 字符串反转（基础版）- 反转指定范围内的字符
 * @param start_ptr: 起始位置指针
 * @param end_ptr: 结束位置指针
 * @return: 成功返回SUCCESS
 * 
 * 算法：双指针交换
 */
int reverse_string_range(char *head, char *tail) {
	if (head == NULL || tail == NULL) {
		printf("Error: strReverse parameter error!\n");
		return PARAM_ERR;
	}
	
	char swap_temp;

	while (head < tail) {
		swap_temp = *head;
		*head = *tail;
		*tail = swap_temp;

		head++;
		tail--;
	}
	
	return SUCCESS;
}

int reverse_string_preserve_words(char *str) {
	if(NULL == str){
		printf("strReverse param error!\n");
		return PARAM_ERR;
	}

	int len = 0;
	char * head = NULL, * tail = NULL;
	char * p = NULL, * q = NULL;

	/*1. 整个字符串颠倒*/
	len  = strlen(str);
	head = str;
	tail = str + len - 1;
	reverse_string_range(head, tail);
	
	/*逐个单词颠倒回来*/
	p = str;
	while(*p != '\0'){
		head = p;
		q = p;
		while(' ' != *q && '\0' != *q){
			q++;
		}
		tail = q - 1;
		reverse_string_range(head, tail);
		
		p = q + 1;
	}
	
	return SUCCESS;
}


int reverse_string_complete(char *str) {
	if(NULL == str){
		printf("strReverse param error!\n");
		return PARAM_ERR;
	}

	int len = 0, rst = 0;
	char * head = NULL, * tail = NULL;

	len = strlen(str);
	head = str;
	tail = str + len - 1;
	
	rst = reverse_string_range(head, tail);

	return rst;
}

/**
 * 反转字符串但保留子串不变 - 反转整个字符串，但指定子串保持原样
 * @param str: 主字符串
 * @param substr: 要保持不变的子串
 * @return: 成功返回SUCCESS
 * 
 * 算法：3步反转法
 * 1. 反转整个主字符串
 * 2. 反转子串本身（用于查找）
 * 3. 在反转后的主串中找到所有子串位置，逐个反转回来
 * 
 * 注意：当前实现只处理第一个匹配的子串
 * 如果有多个相同子串，只有第一个会被保留
 * 
 * 示例：
 * - str="Beijing from Feng", substr="from"
 *   → "gneF from si gnijieB"（整体反转，"from"不变）
 * 
 * BUG: 如果有多个"from"，只有第一个会被保留！
 */
int reverse_string_preserve_substring(char *str, char *substr) {
	if(NULL == str || NULL == substr){
		printf("Error: reverse_string_preserve_substring parameter error!\n");
		return PARAM_ERR;
	}

	int main_length = 0, sub_length = 0;
	char *range_head = NULL, *range_tail = NULL;
	char *current_pos = NULL;

	// 步骤1: 反转整个主字符串
	main_length = strlen(str);
	range_head = str;
	range_tail = str + main_length - 1;
	reverse_string_range(range_head, range_tail);

	// 步骤2: 反转子串本身（用于在反转后的主串中查找）
	sub_length = strlen(substr);
	range_head = substr;
	range_tail = substr + sub_length - 1;
	reverse_string_range(range_head, range_tail);
	
	// 步骤3: 查找所有匹配的子串并反转回来
	current_pos = str;
	while (current_pos != NULL && *current_pos != '\0') {
		current_pos = find_string_substring(current_pos, substr);
		if (current_pos == NULL) {
			break;  // 没有找到更多子串
		}
		
		// 将找到的子串反转回来
		range_head = current_pos;
		range_tail = current_pos + sub_length - 1;
		reverse_string_range(range_head, range_tail);
		
		// 移到下一个可能的位置
		current_pos = current_pos + sub_length;
	}

	return SUCCESS;
}



/* ============================================================================
 * 【字符串检测和比较函数】
 * ============================================================================ */

/**
 * 回文检测 - 判断字符串是否为回文
 * @param input_string: 输入字符串
 * @return: 是回文返回true，否则返回false
 * 
 * 算法：双指针对比
 */
bool validate_string_is_palindrome(char *str) {
	if (str == NULL) {
		printf("Error: isPalindromeStr parameter error!\n");
		return false;
	}

	char *head_ptr = str;
	char *tail_ptr = str + strlen(str) - 1;

	while (head_ptr < tail_ptr) {
		if (*head_ptr != *tail_ptr) {
			return false;
		}
		head_ptr++;
		tail_ptr--;
	}

	return true;
}

/**
 * 字符串比较 - 按字典序比较两个字符串
 * @param first_string: 第一个字符串
 * @param second_string: 第二个字符串
 * @return: >0表示str1>str2, <0表示str1<str2, 0表示相等
 * 
 * 修复：原版本有BUG（长度不同时都返回-1）
 */
int compare_string_lexical(char *str1, char *str2) {
	if (str1 == NULL || str2 == NULL) {
		return 0;
	}

	char *ptr1 = str1;
	char *ptr2 = str2;

	// 逐字符比较
	while (*ptr1 != '\0' && *ptr2 != '\0') {
		if (*ptr1 != *ptr2) {
			return (*ptr1 > *ptr2) ? 1 : -1;
		}
		ptr1++;
		ptr2++;
	}

	// 处理长度差异
	if (*ptr1 == '\0' && *ptr2 == '\0') {
		return 0;   // 完全相同
	} else if (*ptr1 == '\0') {
		return -1;  // str1更短
	} else {
		return 1;   // str2更短
	}
}


/* ============================================================================
 * 【字符串操作函数】
 * ============================================================================ */

/**
 * 查找最长公共子串 - 找到两个字符串中最长的公共连续子串
 * @param first_string: 第一个字符串
 * @param second_string: 第二个字符串
 * @return: 返回最长公共子串（需调用者释放内存）
 * 
 * ============================================================================
 * 算法思路（简单易懂版）
 * ============================================================================
 * 
 * 核心思想：双重循环暴力匹配
 * 
 * 外层循环：遍历str1的每个字符（作为起点）
 * 内层循环：遍历str2的每个字符（寻找匹配点）
 * 匹配时：  从匹配点开始，看能连续匹配多少个字符
 * 
 * ============================================================================
 * 执行过程示例：str1="aocdfe", str2="pmcdfa"
 * ============================================================================
 * 
 * str1的第0个字符'a'：
 *   - 在str2中找'a'：在str2[5]找到
 *   - 从这里开始匹配：'a'匹配，但下一个不匹配
 *   - 匹配长度：1
 * 
 * str1的第1个字符'o'：
 *   - 在str2中找不到'o'
 *   - 匹配长度：0
 * 
 * str1的第2个字符'c'：
 *   - 在str2中找'c'：在str2[2]找到
 *   - 从这里开始匹配：
 *     str1: c d f
 *     str2: c d f
 *     全都匹配！
 *   - 匹配长度：3 ← 最长！
 * 
 * 继续遍历str1剩余字符...
 * 
 * 最终：最长公共子串 = "cdf" (长度3)
 * 
 * ============================================================================
 * 简化版代码逻辑
 * ============================================================================
 */
char *find_string_longest_common(const char *str1, const char *str2) {
	if (str1 == NULL || str2 == NULL) {
		printf("Error: find_string_longest_common parameter error!\n");
		return NULL;
	}

	int max_length = 0;              // 最长匹配长度
	const char *max_start = NULL;    // 最长匹配的起始位置
	char *result = NULL;

	// 外层循环：遍历str1的每个起始位置
	for (int i = 0; str1[i] != '\0'; i++) {
		
		// 内层循环：遍历str2的每个起始位置
		for (int j = 0; str2[j] != '\0'; j++) {
			
			// 从(i,j)开始，看能匹配多长
			int match_length = 0;
			while (str1[i + match_length] != '\0' && 
			       str2[j + match_length] != '\0' &&
			       str1[i + match_length] == str2[j + match_length]) {
				match_length++;
			}
			
			// 更新最长记录
			if (match_length > max_length) {
				max_length = match_length;
				max_start = &str1[i];
			}
		}
	}

	// 分配内存并复制结果
	result = (char *)malloc(max_length + 1);
	if (result == NULL) {
		return NULL;
	}
	
	if (max_length > 0) {
		memcpy(result, max_start, max_length);
	}
	result[max_length] = '\0';

	return result;
}


/* 十六进制字符映射表 */
static char hex_digit_map[16] = {
	'0', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

/**
 * 整数转十六进制字符串 - 将整数转换为十六进制表示
 * @param output: 输出字符串（带0x前缀）
 * @param value: 要转换的整数
 * @return: 成功返回SUCCESS
 * 
 * 示例：255 → "0xFF", 16 → "0x10"
 */
int convert_string_from_integer_hex(char *str, int num) {
	if (str == NULL) {
		printf("Error: convert_string_from_integer_hex parameter error\n");
		return PARAM_ERR;
	}

	const int hex_base = 16;
	int digit_count = 0;
	int working_number = num;
	int remainder = 0;
	char temp_buffer[100] = {'0'};

	// 逐位提取十六进制数字
	do {
		remainder = working_number % hex_base;
		working_number = working_number / hex_base;
		temp_buffer[digit_count++] = hex_digit_map[remainder];
	} while (working_number != 0);
	temp_buffer[digit_count] = '\0';

	// 反转（因为提取是从低位到高位）
	reverse_string_complete(temp_buffer);

	// 添加"0x"前缀
	str[0] = '0';
	str[1] = 'x';
	memcpy(str + 2, temp_buffer, digit_count);
	str[2 + digit_count] = '\0';
		
	return SUCCESS;
}


/* 二进制字符映射表 */
static char binary_digit_map[2] = {'0', '1'};

/**
 * 整数转二进制字符串 - 将整数转换为二进制表示
 * @param output: 输出字符串（带0b前缀）
 * @param value: 要转换的整数
 * @return: 成功返回SUCCESS
 * 
 * 示例：5 → "0b101", 8 → "0b1000"
 */
int convert_string_from_integer_binary(char *str, int num) {
	if (str == NULL) {
		printf("Error: convert_string_from_integer_binary parameter error\n");
		return PARAM_ERR;
	}

	const int binary_base = 2;
	int digit_count = 0;
	int working_number = num;
	int current_bit = 0;
	char temp_buffer[100] = {'0'};

	// 逐位提取二进制数字
	do {
		current_bit = working_number % binary_base;
		working_number = working_number >> 1;  // 右移 = 除以2
		temp_buffer[digit_count++] = binary_digit_map[current_bit];
	} while (working_number != 0);
	temp_buffer[digit_count] = '\0';

	// 反转
	reverse_string_complete(temp_buffer);

	// 添加"0b"前缀
	str[0] = '0';
	str[1] = 'b';
	memcpy(str + 2, temp_buffer, digit_count);
	str[2 + digit_count] = '\0';
	
	return SUCCESS;
}


/**
 * 统计连续字符 - 在每组连续相同字符后插入出现次数
 * @param input: 输入字符串
 * @return: 成功返回SUCCESS
 * 
 * 示例："aaabc" → "aaa3b1c1"
 */
int count_string_consecutive_chars(char *str) {
	if (str == NULL) {
		printf("Error: count_string_consecutive_chars parameter error\n");
		return PARAM_ERR;
	}

	int total_length = strlen(str) + 1;
	int processed_length = 0;
	int consecutive_count = 1;
	char *current_ptr = NULL;
	char *scan_ptr = NULL;
	char count_char;

	current_ptr = str;
	while (*current_ptr != '\0') {
		scan_ptr = current_ptr;
		consecutive_count = 0;
		
		// 统计连续相同字符的个数
		while (*scan_ptr != '\0' && *scan_ptr == *current_ptr) {
			consecutive_count++;
			scan_ptr++;
		}
		processed_length += consecutive_count;

		// 将统计数字转为字符
		count_char = consecutive_count + '0';
		
		// 为插入数字腾出空间
		memcpy(current_ptr + consecutive_count + 1, 
		       current_ptr + consecutive_count, 
		       total_length - processed_length);
		
		// 插入统计数字
		*(current_ptr + consecutive_count) = count_char;
		
		// 更新长度和指针
		processed_length += 1;
		total_length += 1;
		current_ptr = current_ptr + consecutive_count + 1;
	}

	return SUCCESS;
}


/* 字母映射表 */
static char lowercase_alphabet[26] = {
	'a','b','c','d','e','f','g',
	'h','i','j','k','l','m','n',
	'o','p','q','r','s','t','u',
	'v','w','x','y','z'
};

static char uppercase_alphabet[26] = {
	'A','B','C','D','E','F','G',
	'H','I','J','K','L','M','N',
	'O','P','Q','R','S','T','U',
	'V','W','X','Y','Z'
};

/**
 * 凯撒密码编码 - 对字符串进行凯撒密码加密（偏移4位）并反转
 * @param input: 输入字符串
 * @return: 成功返回SUCCESS
 * 
 * 算法：字母偏移 + 反转
 * 1. 每个字母向后偏移4位（a→e, b→f, ...）
 * 2. 使用模26运算处理循环（w→a, x→b, ...）
 * 3. 最后反转整个字符串
 * 
 * 示例："abc" → "efg" → "gfe"
 */
int encode_string_caesar_cipher(char *str) {
	if (str == NULL) {
		printf("Error: encode_string_caesar_cipher parameter error\n");
		return PARAM_ERR;
	}

	char *char_ptr = str;
	int alphabet_offset = 0;
	const int caesar_shift = 4;  // 偏移量

	while (*char_ptr != '\0') {
		if (*char_ptr >= 'a' && *char_ptr <= 'z') {
			// 小写字母编码
			alphabet_offset = *char_ptr - 'a';
			alphabet_offset = (alphabet_offset + caesar_shift) % 26;
			*char_ptr = lowercase_alphabet[alphabet_offset];
		} else if (*char_ptr >= 'A' && *char_ptr <= 'Z') {
			// 大写字母编码
			alphabet_offset = *char_ptr - 'A';
			alphabet_offset = (alphabet_offset + caesar_shift) % 26;
			*char_ptr = uppercase_alphabet[alphabet_offset];
		}
		// 非字母字符保持不变
		
		char_ptr++;
	}

	// 反转整个字符串（增加编码复杂度）
	reverse_string_complete(str);
	
	return SUCCESS;
}


/**
 * 大整数加法 - 计算两个以字符串表示的大整数之和
 * @param num1_string: 第一个大整数字符串
 * @param num2_string: 第二个大整数字符串  
 * @param result_string: 结果字符串缓冲区
 * @return: 成功返回SUCCESS
 * 
 * 算法：模拟竖式加法
 * 1. 从两个数的个位开始相加
 * 2. 处理进位
 * 3. 处理长度不同的情况
 * 4. 处理最高位进位
 * 
 * 示例："123" + "456" = "579"
 */
int add_string_big_integers(char *str1, char *str2, char *rst) {
	if (str1 == NULL || str2 == NULL || rst == NULL) {
		printf("Error: add_string_big_integers parameter error\n");
		return PARAM_ERR;
	}

	char *num1_tail = NULL, *num2_tail = NULL, *result_tail = NULL;
	int length1 = strlen(str1);
	int length2 = strlen(str2);
	int max_length = (length1 > length2) ? length1 : length2;
	int digit1 = 0, digit2 = 0, sum = 0, carry = 0;
	
	max_length = max_length + 1;  // 预留最高位进位
	
	// 设置尾指针（从个位开始）
	num1_tail = str1 + length1 - 1;
	num2_tail = str2 + length2 - 1;
	result_tail = rst + (max_length - 1);
	*(rst + max_length) = '\0';

	// 两数都还有位时，逐位相加
	while (num1_tail >= str1 && num2_tail >= str2) { 
		digit1 = *num1_tail - '0';
		digit2 = *num2_tail - '0';
		sum = digit1 + digit2 + carry;
		
		if (sum >= 10) {
			sum = sum % 10;
			carry = 1;
		} else {
			carry = 0;
		}
		*result_tail = '0' + sum;

		num1_tail--;
		num2_tail--;
		result_tail--;
	}

	// 处理num1的剩余位
	while (num1_tail >= str1) {
		digit1 = *num1_tail - '0';
		sum = digit1 + carry;
		
		if (sum >= 10) {
			sum = sum % 10;
			carry = 1;
		} else {
			carry = 0;
		}
		*result_tail = '0' + sum;

		num1_tail--;
		result_tail--;
	}

	// 处理num2的剩余位
	while (num2_tail >= str2) {
		digit2 = *num2_tail - '0';
		sum = digit2 + carry;
		
		if (sum >= 10) {
			sum = sum % 10;
			carry = 1;
		} else {
			carry = 0;
		}
		*result_tail = '0' + sum;

		num2_tail--;
		result_tail--;
	}

	// 处理最高位进位
	if (carry == 0) {
		// 没有进位，去掉最高位的占位符
		memcpy(result_tail, result_tail + 1, max_length - 1);
		*(rst + max_length - 1) = '\0';
	} else {
		// 有进位，填入最高位
		*result_tail = '0' + carry;
	}

	return SUCCESS;
}


/**
 * 字符串循环右移 - 将字符串末尾n个字符移到开头
 * @param input: 输入字符串
 * @param shift_count: 右移位数
 * @return: 成功返回SUCCESS
 * 
 * 示例："123456789" 右移3位 → "789123456"
 */
int shift_string_right_circular(char *str, int n) {
	if (str == NULL) {
		printf("Error: shift_string_right_circular parameter error\n");
		return PARAM_ERR;
	}

	int string_length = strlen(str);
	if (n > string_length) {
		printf("Error: shift count exceeds string length\n");
		return PARAM_ERR;
	}

	char temp_buffer[100] = {'\0'};
	char *tail_start = str + string_length - n;

	// 步骤1: 保存末尾n个字符
	for (int i = 0; i < n; i++) {
		temp_buffer[i] = tail_start[i];
	}

	// 步骤2: 前面的字符后移n位
	memcpy(str + n, str, string_length - n);

	// 步骤3: 将保存的字符放到开头
	for (int i = 0; i < n; i++) {
		str[i] = temp_buffer[i];
	}
	
	return SUCCESS;
}


/**
 * 删除子串 - 从字符串中删除指定位置和长度的子串
 * @param input: 输入字符串
 * @param start_index: 删除起始位置
 * @param delete_length: 要删除的长度
 * @return: 成功返回SUCCESS
 */
int remove_string_substring_at(char *str, int index, int size) {
	if (str == NULL || index < 0 || size < 0) {
		printf("Error: remove_string_substring_at parameter error\n");
		return PARAM_ERR;
	}

	int string_length = strlen(str);
	char *delete_start = str + index;
	char *content_after = delete_start + size;

	// 将删除位置后的内容前移
	memcpy(delete_start, content_after, string_length - index - size);
	str[string_length - size] = '\0';

	return SUCCESS;
}

/**
 * 前半部分降序排序 - 对字符串前半部分进行降序排序
 * @param input: 输入字符串
 * @return: 成功返回SUCCESS
 */
int sort_string_half_descending(char *str) {
	if (str == NULL) {
		printf("Error: sort_string_half_descending parameter error\n");
		return PARAM_ERR;
	}

	int total_length = strlen(str);
	int half_length = total_length / 2;
	char swap_temp;

	// 选择排序（降序）
	for (int i = 0; i < half_length; i++) {
		for (int j = i + 1; j < half_length; j++) {
			if (str[i] < str[j]) {
				swap_temp = str[i];
				str[i] = str[j];
				str[j] = swap_temp;
			}
		}
	}

	return SUCCESS;
}

/**
 * 删除指定字符 - 删除字符串中所有指定的字符
 * @param input: 输入字符串
 * @param target_char: 要删除的字符
 * @return: 成功返回SUCCESS
 */
int remove_string_char_all(char *str, const char c) {
	if (str == NULL) {
		printf("Error: remove_string_char_all parameter error\n");
		return PARAM_ERR;
	}	
	
	int original_length = strlen(str);
	int removed_count = 0;
	char *scan_ptr = str;
	char *string_end = str + original_length - 1;

	while (*scan_ptr != '\0') {
		if (c == *scan_ptr) {
			// 找到目标字符，用后续内容覆盖
			memcpy(scan_ptr, scan_ptr + 1, (string_end - scan_ptr));
			removed_count++;
			// 不移动指针，因为当前位置已被新字符覆盖
			continue;
		}
		scan_ptr++;
	}

	str[original_length - removed_count] = '\0';

	return SUCCESS;
}

/**
 * 字符串连接 - 将第二个字符串连接到第一个字符串末尾
 * @param dest: 目标字符串（需有足够空间）
 * @param src: 源字符串
 * @return: 返回目标字符串指针
 */
char *concatenate_string_append(char *str1, char *str2) {
	if (str1 == NULL || str2 == NULL) {
		printf("Error: concatenate_string_append parameter error\n");
		return NULL;
	}	
	
	int dest_length = strlen(str1);
	int src_length = strlen(str2);
	char *append_position = str1 + dest_length;

	// 复制到第一个字符串的'\0'位置
	memcpy(append_position, str2, src_length);
	str1[dest_length + src_length] = '\0';

	return str1;
}


/**
 * GBK字符串长度 - 计算包含中文字符的字符串长度
 * @param input: 输入字符串（GBK编码）
 * @return: 字符数（中文算1个字符）
 */
int get_string_length_gbk(char *str) {
	if (str == NULL) {
		printf("Error: get_string_length_gbk parameter error\n");
		return PARAM_ERR;
	}

	int char_count = 0;
	char *char_ptr = str;

	while (*char_ptr != '\0') {
		// 中文字符判断（GBK编码）
		if (*char_ptr < 0 && (*(char_ptr + 1) < 0 || *(char_ptr + 1) > 63)) {
			char_ptr = char_ptr + 3;  // GBK中文占2-3字节
		} else {
			char_ptr++;
		}
		char_count++;
	}

	return char_count;
}

/**
 * 统计最大连续0/1 - 在二进制字符串中找最大连续0和1的个数
 * @param input: 输入字符串（只包含'0'和'1'）
 * @param max_zero: 输出最大连续0的个数
 * @param max_one: 输出最大连续1的个数
 * @return: 成功返回SUCCESS
 * 
 * ============================================================================
 * 算法思路（简单易懂版）
 * ============================================================================
 * 
 * 核心思想：遍历一次，边走边统计
 * 
 * 1. 当前字符是什么？（'0'还是'1'）
 * 2. 从这里开始，能连续多少个相同字符？
 * 3. 如果比之前记录的最大值还大，更新记录
 * 4. 跳到下一组不同的字符，继续
 * 
 * 示例："00011101100"
 *        ←3→←3→←2→  (连续的0)
 *           ←4→      (连续的1)
 * 
 * 最大连续0 = 3
 * 最大连续1 = 4
 * 
 * ============================================================================
 */
int get_string_max_successive_bits(char *str, int *pnum0, int *pnum1) {
	if (str == NULL || pnum0 == NULL || pnum1 == NULL) {
		printf("Error: get_string_max_successive_bits parameter error\n");
		return PARAM_ERR;
	}

	int max_zero_count = 0;
	int max_one_count = 0;
	int i = 0;

	while (str[i] != '\0') {
		char current_char = str[i];
		int consecutive_count = 0;
		
		// 统计从当前位置开始的连续相同字符数
		while (str[i] == current_char && str[i] != '\0') {
			consecutive_count++;
			i++;
		}
		
		// 更新最大记录
		if (current_char == '0' && consecutive_count > max_zero_count) {
			max_zero_count = consecutive_count;
		} else if (current_char == '1' && consecutive_count > max_one_count) {
			max_one_count = consecutive_count;
		}
	}

	*pnum0 = max_zero_count;
	*pnum1 = max_one_count;

	return SUCCESS;
}

/**
 * 替换所有子串 - 将字符串中所有指定子串替换为新字符串
 * @param input: 输入字符串
 * @param old_substring: 要被替换的子串
 * @param new_substring: 替换成的新字符串
 * @return: 成功返回SUCCESS
 */
int replace_string_substring_all(char *str, char *sub, char *replace) {
	if (str == NULL || sub == NULL || replace == NULL) {
		printf("Error: replace_string_substring_all parameter error\n");
		return PARAM_ERR;
	}

	char *current_pos = str;
	char *string_end = NULL;
	char *found_pos = NULL;
	char *copy_src = NULL;
	char *copy_dest = NULL;

	int current_length = strlen(str);
	int old_length = strlen(sub);
	int new_length = strlen(replace);

	while (*current_pos != '\0') {
		string_end = str + current_length;
		found_pos = strstr(str, sub);
		
		if (found_pos == NULL) {
			break;  // 没有更多子串
		}

		// 为新字符串腾出空间
		copy_src = found_pos + old_length;
		copy_dest = found_pos + new_length;
		memcpy(copy_dest, copy_src, string_end - copy_src);
		
		// 复制新字符串
		memcpy(found_pos, replace, new_length);
		
		// 更新长度
		current_length = current_length + new_length - old_length;
		
		// 移到下一个可能的位置
		current_pos = found_pos + new_length;
	}
	
	str[current_length] = '\0';

	return SUCCESS;
}


void teststrReplaceAll(void){
	char str[100] = "abcdefcdg";
	char sub[100] = "cd";
	char replace[100] = "123";
	//char replace[100] = "2";
	int num0  = 0, num1 = 0;

	printf("\n************  teststrReplaceAll ************ \n");
	
	strReplaceAll(str, sub, replace);
	
	printf("After replace all, str is : %s\n", str);

	return;	
}



void testgetStrMaxSuccNum(void){
	char str[100] = "00001110110000001100110101101001010101011111010";
	int num0  = 0, num1 = 0;

	printf("\n************  testgetStrMaxSuccNum ************ \n");
	
	getStrMaxSuccNum(str, &num0, &num1);
	
	printf("Max successive 0 num is : %d \n Max successive 1 num is : %d\n", num0, num1);

	return;	
}

void testgbkstrlen(void){
	char str[100] = "abc你好123中国456";
	int len  = 0;

	printf("\n************  testgbkstrlen ************ \n");
	
	len  = gbkstrlen(str);
	
	printf("gbk string lenght is : %d\n", len);

	return;	

}


void testmystrcat(void){
	char str1[100] = "Hello";
	char str2[100] = " Feng!";

	printf("\n************  testmystrcat ************ \n");
	
	mystrcat(str1, str2);

	printf("after strcat is : %s\n", str1);

	return;	

}


void teststrRemoveSpecChar(void){
	char str[100] = "cabcdefcgchci";

	printf("\n************  teststrRemoveSpecChar ************ \n");
	
	strRemoveSpecChar(str, 'c');

	printf("after remove char 'c' is : %s\n", str);

	return;	
} 



void testsortHalfStr(void){
	char str[100] = "ADZDDJKJFIEJHGI";

	printf("\n************  testsortHalfStr ************ \n");
	
	sortHalfStr(str);

	printf("after sort half stirng is : %s\n", str);

	return;	
} 



void testremoveSubString(void){
	char str[100] = "abcdefg";

	printf("\n************  testremoveSubString ************ \n");
	
	removeSubString(str, 1, 2);

	printf("after remove sub stirng is : %s\n", str);

	return;	
} 



void testrightShiftStr(void){
	char str[100] = "123456789";

	printf("\n************  testrightShiftStr ************ \n");
	
	rightShiftStr(str, 6);

	printf("right shift 6 is : %s\n", str);

	return;	
} 


void testaddBigInt(void){
	char str1[100] = "123456789323";
	char str2[100] = "45671254563123";
	char str3[100] = "523456789323";
	char str4[100] = "99671254563123";	
	char rst[100] = {'\0'};

	printf("\n************  testaddBigInt ************ \n");
	
	addBigInt(str1, str2, rst);

	printf("%s + %s = %s\n", str1, str2, rst);

	memset(rst, 0x0, 100);

	addBigInt(str3, str4, rst);

	printf("%s + %s = %s\n", str3, str4, rst);


	return;
}



void testencodeStr(void){
	char str[100] = "hasdxz11HYZ";

	printf("\n************  testencodeStr ************ \n");	

	encodeStr(str);

	printf("Encoded str is: %s\n", str);

	return;		
}



void teststatStr(void){
	char str[100] = "aaabc";

	printf("\n************  teststatStr ************ \n");

	statStr(str);

	printf("Stat str is: %s\n", str);

	return;	
}



void testConvertNum2Str(void){
	char str[100] = {'0'};
	int num = 456789;

	printf("\n************  testConvertNum2Str ************ \n");

	int2HexStr(str,num);
	printf("The hex str is %s\n", str);

	int2BinStr(str,num);
	printf("The bin str is %s\n", str);

	return;
}

void testcommonStr(void){
	char * str1 = "aocdfe";
	char * str2 = "pmcdfa";
	char * commstr = NULL;

	printf("\n************  testcommonStr ************ \n");

	commstr = commonStr(str1, str2);
	printf("The common str is %s\n", commstr);

	free(commstr);
	commstr = NULL;

	return;
}


void testmystrcmp(void){
	char * str = "1234567";
	char * str1 = "1234567";
	char * str2 = "12345678";
	char * str3 = "1234566";
	int ret = 0;

	printf("\n************  testmystrcmp ************ \n");

	ret = mystrcmp(str, str1);
	if(0 == ret){
		printf("%s and %s is the same\n", str,str1);
	} else if (1 == ret) {
		printf("%s big than %s \n", str,str1);
	}else if (-1 == ret) {
		printf("%s less than %s \n", str,str1);
	} else {
	}


	ret = mystrcmp(str, str2);
	if(0 == ret){
		printf("%s and %s is the same\n", str,str2);
	} else if (1 == ret) {
		printf("%s big than %s \n", str,str2);
	}else if (-1 == ret) {
		printf("%s less than %s \n", str,str2);
	} else {
	}

	ret = mystrcmp(str, str3);
	if(0 == ret){
		printf("%s and %s is the same\n", str,str3);
	} else if (1 == ret) {
		printf("%s big than %s \n", str,str3);
	}else if (-1 == ret) {
		printf("%s less than %s \n", str,str3);
	} else {
	}

	return;
}


void testisPalindromeStr(void){
	char * str1 = "Feng is from Beijing!";
	char * str2 = "level level";
	bool flag;

	printf("\n************  testisPalindromeStr ************ \n");

	flag = isPalindromeStr(str1);
	if(flag){
		printf("%s is a Palindrome string \n", str1);
	} else {
		printf("%s is not a Palindrome string \n", str1);
	}

	flag = isPalindromeStr(str2);
	if(flag){
		printf("%s is a Palindrome string \n", str2);
	} else {
		printf("%s is not a Palindrome string \n", str2);
	}


}


void testReverse(void){
	char str[100] = "Feng is from Beijing!";
	char sub[100] = "from";
	char * head = NULL, * tail = NULL;
	int len  = 0;

	printf("\n************  testReverse ************ \n");

	strReversWithWord(str);
	printf("Reverse string is : %s\n\n", str);

	/*在反转回来*/
	strReversWithWord(str);
	printf("Reverse string is : %s\n\n", str);


	strReversWithoutWord(str);
	printf("Reverse string without word is : %s\n\n", str);

	/*在反转回来*/
	strReversWithoutWord(str);
	printf("Reverse string without word is : %s\n\n", str);

	
	strReversWithoutSubstr(str, sub);
	printf("Reverse string without substr is : %s\n\n", str);


	return;

}

void testmystrstr(void){
	char * str1 = "12345";
	char * str2 = "45";
	char * str3 = "23";
	char * str4 = "25";
	char * r = NULL;

	printf("\n************  testmystrstr ************ \n");

	r = mystrstr(str1, str2);
	if(NULL != r){
		printf("substr: %s\n", r);
	} else {
		printf("can not find sub: %s\n", str2);
	}
	

	r = mystrstr(str1, str3);
	if(NULL != r){
		printf("substr: %s\n", r);
	} else {
		printf("can not find sub: %s\n", str3);
	}

	r = mystrstr(str1, str4);
	if(NULL != r){
		printf("substr: %s\n", r);
	} else {
		printf("can not find sub: %s\n", str4);
	}	

	return;
}



void testmystrlen(void){
	int len = 0;
	char * src = "Hello World!";

	printf("\n************  testmystrlen ************ \n");
	len = mystrlen(src);
	printf("len = %d\n", len);

	return;

}


void testmymemcpy(void){
	char dst[100] = {0};
	char * src = "Feng is the best !";

	printf("\n************  testmymemcpy ************ \n");
	mymemcpy(dst, src, 6);
	printf("dst = %s\n", dst);

	return;

}


void testmystrcpy(void){
	char dst[100] = {0};
	char * src = "Feng is the best !";

	printf("\n************  testmystrcpy ************ \n");
	mystrcpy(dst, src);
	printf("dst = %s\n", dst);

	memset(dst, 0x0, 100);
	mystrcpy(dst, "Hello Feng!");
	printf("dst = %s\n", dst);

	return;

}


void testlibstr2num(void){
	char dst[100] = {0};
	int num = 0;
	float num2 = 0;

	printf("\n************  testlibstr2num ************ \n");

	strcpy(dst, "1234");
	libstr2int(dst, &num);
	printf("num = %d\n", num);

	num = 0;
	memset(dst, 0x0, 100);
	strcpy(dst, "-1234");
	libstr2int(dst, &num);
	printf("num = %d\n", num);


	strcpy(dst, "436.55");
	libstr2float(dst, &num2);
	printf("num = %f\n", num2);

	num = 0;
	memset(dst, 0x0, 100);
	strcpy(dst, "-436.55");
	libstr2float(dst, &num2);
	printf("num = %f\n", num2);

	return;

}


void teststr2int(void){
	char dst[100] = {0};
	int num = 0;

	printf("\n************  teststr2int ************ \n");

	strcpy(dst, "12345678");
	str2int(dst, &num);
	printf("num = %d\n", num);

	num = 0;
	memset(dst, 0x0, 100);
	strcpy(dst, "-1234");
	str2int(dst, &num);
	printf("num = %d\n", num);

	return;
}


void testint2str(void){
	char dst[100] = {0};

	printf("\n************  testint2str ************ \n");

	int2str(dst, 1234);
	printf("num = %s\n", dst);

	memset(dst, 0x0, 100);
	int2str(dst, -1234);
	printf("num = %s\n", dst);
	
	return;
}

void testNum2Str(void){
	int num = -50;
	int sig = 8;
	float num2 = -50.783657821234;
	char dst[100] = {0};

	printf("\n************  testNum2Str ************ \n");

	//itoa(num, dst, 10);
	//Linux 没有 itoa
	
	sprintf(dst, "%d", num);
	printf("num = %s\n", dst);

	memset(dst, 0x0, 100);
	//gcvt(num2, sig, dst);
	sprintf(dst, "%.6f", num2);
	printf("num2 = %s\n", dst);

	
	return;
}
