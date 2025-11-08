/*
 * 需求
 
 给定一个字符串 S，找出 S 中不同的非空回文子序列个数，并返回该数字与 10^9 + 7 的模。 
 通过从 S 中删除 0 个或多个字符来获得子字符序列。 
 如果一个字符序列与它反转后的字符序列一致，那么它是回文字符序列。 
 如果对于某个  i，A_i != B_i，那么 A_1, A_2, ... 和 B_1, B_2, ... 这两个字符序列是不同的。
    
 示例 1：
 输入：
 S = 'bccb'
 输出：6
 解释：
 6 个不同的非空回文子字符序列分别为：'b', 'c', 'bb', 'cc', 'bcb', 'bccb'。
 注意：'bcb' 虽然出现两次但仅计数一次。

 示例 2： 
 输入：
 S = 'abcdabcdabcdabcdabcdabcdabcdabcddcbadcbadcbadcbadcbadcbadcbadcba'
 输出：104860361
 解释：
 共有 3104860382 个不同的非空回文子字符序列，对 10^9 + 7 取模为 104860361。
  
 
 提示： 
 字符串 S 的长度将在[1, 1000]范围内。
 每个字符 S[i] 将会是集合 {'a', 'b', 'c', 'd'} 中的某一个。
     
 gcc countPalindromicSubsequences.c -g -o a.exe -DDEBUG

 */
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifdef DEBUG
#define LOG(fmt, args...) fprintf(stdout, fmt, ##args)
#define BREAKER(a, b, c) breaker(a, b, c)
#else
#define LOG(fmt,...)
#define BREAKER(a, b, c)
#endif

#define TRUE        1
#define FALSE       0

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (b) : (a))


int countPalindromicSubsequences(char * S){

	if(NULL == S){
		return 0;
	}

	int ** dp = NULL;
	int i = 0, j = 0;
	int size = strlen(S);
	int left = 0, right = 0;
	int ret = 0;
    int M = 1e9 + 7;

	dp = (int ** )malloc(size * (sizeof(int *)));
	for(i = 0; i < size; i++){
		dp[i] = (int *)malloc(size * sizeof(int)); 
	}	

    /*数组初始化都是0，这步骤很重要，因为dp算法，后面的计算要依赖前面的值和初始值*/
    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            dp[i][j] = 0;
        }
    }

	for(i = 0; i < size; i++){
		dp[i][i] = 1;
	}

	for(i = size - 2; i >= 0; i--){
		for(j = i + 1; j < size; j++){
			if(S[i] == S[j]) {
				left = i + 1;
				right = j - 1;
				while(left <= right && S[left] != S[i]){
					left++;
				}
				while(left <= right && S[right] != S[i]){
					right--;
				}

				if(left > right) { /*不包含s[i]*/
                    dp[i][j] = dp[i + 1][j - 1] * 2 + 2;
				} else if (left == right){ /*包含1个s[i]*/
                    dp[i][j] = dp[i + 1][j - 1] * 2 + 1;
				} else { /*包含2个以上s[i]*/
                    dp[i][j] = dp[i + 1][j - 1] * 2 - dp[left + 1][right - 1];
				}
			} else {
                dp[i][j] = dp[i][j - 1] + dp[i + 1][j] - dp[i + 1][j - 1];
			}
            dp[i][j] = (dp[i][j] < 0) ? dp[i][j] + M : dp[i][j] % M;
		}
	}
	
	ret = dp[0][size - 1];
	free(dp);
	dp = NULL;
	
	return ret;
}



void testcountPalindromicSubsequences(void){
	
	printf("\n************  testcountPalindromicSubsequences ************ \n");
	int ret = 0;
	
#if 1

	char * str1 = "bccb";
	ret = countPalindromicSubsequences(str1);
	printf("The Palindromic Subsequences of str %s is %d\n", str1, ret);

	char * str2 = "abcdabcdabcdabcdabcdabcdabcdabcddcbadcbadcbadcbadcbadcbadcbadcba";
	ret = countPalindromicSubsequences(str2);
	printf("The Palindromic Subsequences of str %s is %d\n", str2, ret);

#endif

	return; 
 
 }


 int main(int argc, char ** argv){
	testcountPalindromicSubsequences();
 }


