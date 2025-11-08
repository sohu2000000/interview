/*
 * 需求
 
 给定两个字符串 text1 和 text2，返回这两个字符串的最长公共子序列。
 
 一个字符串的 子序列 是指这样一个新的字符串：它是由原字符串在不改变字符的相对顺序的情况下删除某些字符（也可以不删除任何字符）后组成的新字符串。
 例如，"ace" 是 "abcde" 的子序列，但 "aec" 不是 "abcde" 的子序列。两个字符串的「公共子序列」是这两个字符串所共同拥有的子序列。
 
 若这两个字符串没有公共子序列，则返回 0。
 
  
 示例 1: 
 输入：text1 = "abcde", text2 = "ace" 
 输出：3  
 解释：最长公共子序列是 "ace"，它的长度为 3。

 示例 2: 
 输入：text1 = "abc", text2 = "abc"
 输出：3
 解释：最长公共子序列是 "abc"，它的长度为 3。

 示例 3: 
 输入：text1 = "abc", text2 = "def"
 输出：0
 解释：两个字符串没有公共子序列，返回 0。
   
 提示: 
 1 <= text1.length <= 1000
 1 <= text2.length <= 1000
 输入的字符串只含有小写英文字符。
      
 gcc longestCommonSubsequence.c -g -o a.exe -DDEBUG

 */
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifdef DEBUG
#define LOG(fmt, args...) fprintf(stdout, fmt, ##args)
#else
#define LOG(fmt,...)
#endif

#define TRUE        1
#define FALSE       0

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (b) : (a))


int longestCommonSubsequence(char * text1, char * text2){

	if(NULL == text1 || NULL == text2){
		return 0;
	}

	int len1 = 0, len2 = 0, i = 0, j = 0, ret = 0;
	int ** dp = NULL;
	
	len1 = strlen(text1);
	len2 = strlen(text2);

	/*构建 dp*/
	dp = (int **)malloc((len1 + 1) * sizeof(int *));
	for(i = 0; i <= len1; i++){
		dp[i] = (int *) malloc((len2 + 1) * sizeof(int));
	}
	
	/*初始化dp*/
	for(i = 0; i <= len1; i++){
		for(j = 0; j <= len2; j++){
			dp[i][j] = 0;
		}
	}

	/*
	 * 0的那列和那行，代表一个字符串是“”，公共子串为为0，
	 * dp[i][j] 表示 s1[0....i-1]和s[0...j-1]的公共子串大小
	 * 注意，下标是从0开始标记的，所以这里应该变为 i - 1 和 j - 1
	 */
	for(i = 1; i <=len1; i++){
		for(j = 1; j <= len2; j++){
			if(text1[i - 1] == text2[j - 1]){
				/* 又发现一个相同的字符，在原有基础上加1*/
				dp[i][j] = dp[i - 1][j - 1] + 1; 
			} else {
				/* 两个新的字符不同，从他们最长的两个子串中，选出LCS较大的数量*/
				dp[i][j] = MAX(dp[i - 1][j], dp[i][j - 1]);
			}
		}
	}

	ret = dp[len1][len2];

	/*释放已用完的dp*/
	for(i = 0; i <= len1; i++){
		free(dp[i]);
		dp[i] = NULL;
	}
	free(dp);
	dp = NULL;

	return ret;
	
}


void testlongestCommonSubsequence(void){
	
	printf("\n************  testlongestCommonSubsequence ************ \n");
	int ret = 0;
	
	char * str1 = "abcde";
	char * str2 = "ace";
	ret = longestCommonSubsequence(str1, str2);
	printf("The Longest commen subseq of \"%s\" and  \"%s\" is %d\n", str1, str2, ret);

	char * str3 = "abc";
	char * str4 = "abc";
	ret = longestCommonSubsequence(str3, str4);
	printf("The Longest commen subseq of  \"%s\" and  \"%s\" is %d\n", str3, str4, ret);


	char * str5 = "abc";
	char * str6 = "def";
	ret = longestCommonSubsequence(str5, str6);
	printf("The Longest commen subseq of \"%s\" and \"%s\" is %d\n", str5, str6, ret);

	return; 
 
 }


 int main(int argc, char ** argv){
	testlongestCommonSubsequence();
 }






