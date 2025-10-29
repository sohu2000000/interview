#ifndef __STR_H__
#define __STR_H__

#include <stdbool.h>

#define SUCCESS		0
#define PARAM_ERR	-1
#define ALLOC_ERR   -2

/* ===== 字符串处理函数（统一命名规范：动词_string_名词_修饰词） ===== */

/* 类型转换函数 */
int convert_string_from_integer(char *output, int value);
int convert_string_to_integer(char *input, int *result);
int convert_string_from_integer_hex(char *output, int value);
int convert_string_from_integer_binary(char *output, int value);
int convert_string_to_integer_lib(char *input, int *result);
int convert_string_to_float_lib(char *input, float *result);

/* 基础字符串操作 */
int copy_string_to_buffer(char *dest, char *src);
void *copy_memory_bytes(void *dest, void *src, int size);
int get_string_length(char *input);
char *find_string_substring(char *main_str, char *sub_str);
char *concatenate_string_append(char *dest, char *src);

/* 字符串反转 */
int reverse_string_range(char *start, char *end);
int reverse_string_preserve_words(char *input);
int reverse_string_complete(char *input);
int reverse_string_preserve_substring(char *input, char *substring);

/* 字符串检测和比较 */
bool validate_string_is_palindrome(char *input);
int compare_string_lexical(char *str1, char *str2);

/* 字符串操作 */
char *find_string_longest_common(const char *str1, const char *str2);
int count_string_consecutive_chars(char *input);
int encode_string_caesar_cipher(char *input);
int add_string_big_integers(char *num1, char *num2, char *result);
int shift_string_right_circular(char *input, int positions);
int remove_string_substring_at(char *input, int index, int length);
int sort_string_half_descending(char *input);
int remove_string_char_all(char *input, const char target);
int replace_string_substring_all(char *input, char *old_sub, char *new_sub);
int get_string_length_gbk(char *input);
int get_string_max_successive_bits(char *input, int *max0, int *max1);

/* 兼容性宏：保持旧代码可用 */
#define int2str              convert_string_from_integer
#define str2int              convert_string_to_integer
#define int2HexStr           convert_string_from_integer_hex
#define int2BinStr           convert_string_from_integer_binary
#define libstr2int           convert_string_to_integer_lib
#define libstr2float         convert_string_to_float_lib
#define mystrcpy             copy_string_to_buffer
#define mymemcpy             copy_memory_bytes
#define mystrlen             get_string_length
#define mystrstr             find_string_substring
#define mystrcat             concatenate_string_append
#define strReverse           reverse_string_range
#define strReversWithoutWord reverse_string_preserve_words
#define strReversWithWord    reverse_string_complete
#define strReversWithoutSubstr reverse_string_preserve_substring
#define isPalindromeStr      validate_string_is_palindrome
#define mystrcmp             compare_string_lexical
#define commonStr            find_string_longest_common
#define statStr              count_string_consecutive_chars
#define encodeStr            encode_string_caesar_cipher
#define addBigInt            add_string_big_integers
#define rightShiftStr        shift_string_right_circular
#define removeSubString      remove_string_substring_at
#define sortHalfStr          sort_string_half_descending
#define strRemoveSpecChar    remove_string_char_all
#define strReplaceAll        replace_string_substring_all
#define gbkstrlen            get_string_length_gbk
#define getStrMaxSuccNum     get_string_max_successive_bits



void testNum2Str(void);
void testint2str(void);
void teststr2int(void);
void testlibstr2num(void);
void testmystrcpy(void);
void testmymemcpy(void);
void testmystrlen(void);
void testmystrstr(void);
void testReverse(void);
void testisPalindromeStr(void);
void testmystrcmp(void);
void testcommonStr(void);
void testConvertNum2Str(void);
void teststatStr(void);
void testencodeStr(void);
void testaddBigInt(void);
void testrightShiftStr(void);
void testremoveSubString(void);
void testsortHalfStr(void);
void teststrRemoveSpecChar(void);
void testmystrcat(void);
void testgbkstrlen(void);
void testgetStrMaxSuccNum(void);
void teststrReplaceAll(void);




#endif
