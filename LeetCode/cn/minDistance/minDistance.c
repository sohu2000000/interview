/*
 * 需求
 
 给定两个单词 word1 和 word2，计算出将 word1 转换成 word2 所使用的最少操作数 。 
 你可以对一个单词进行如下三种操作：
 
 插入一个字符
 删除一个字符
 替换一个字符

 示例 1:
 输入: word1 = "horse", word2 = "ros"
 输出: 3
 解释: 
 horse -> rorse (将 'h' 替换为 'r')
 rorse -> rose (删除 'r')
 rose -> ros (删除 'e')

 示例 2: 
 输入: word1 = "intention", word2 = "execution"
 输出: 5
 解释: 
 intention -> inention (删除 't')
 inention -> enention (将 'i' 替换为 'e')
 enention -> exention (将 'n' 替换为 'x')
 exention -> exection (将 'n' 替换为 'c')
 exection -> execution (插入 'u')
       
 gcc minDistance.c -g -o a.exe -DDEBUG

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




int minDistance(char * word1, char * word2){
	if(NULL == word1 || NULL == word2){
		return 0;
	}	

	int ** dp = NULL;
	int i = 0, j = 0;
	int len1 = 0, len2 = 0;
	int ret = 0;

	if(NULL == word1 ||  NULL == word2){
		return 0;
	}
	
	len1 = strlen(word1);
	len2 = strlen(word2);

	if(0 == len1){
		return len2;
	} else if(0 == len2){
		return len1;
	} else {
	}

	/*
	 * 0. dp[i][j] 定义：
	 * dp[i][j] 表示把 s1[1...j-1] 改成 s2[1 ... i-1] 所需要的步骤
	 * dp[xxx][0] 表示把 s1 改成 "" 所需要的步骤
	 * dp[0][XXX] 表示把 s2 改成 "" 所需要的步骤
	 */
	/* 1. 分配 */
	dp = (int **) malloc (sizeof(int *) * (len1 + 1));
	for(i = 0; i <= len1; i++){
		dp[i] = (int*) malloc(sizeof(int) * (len2 + 1));
	}

	/* 2. 初始化 */
	for(i = 0; i < len1 + 1; i++){
		for(j = 0; j < len2 + 1; j++){
			dp[i][j] = 0;
		}
	}

	/* dp[xxx][0] 表示把 s1 改成 "" 所需要的步骤*/
	for(i = 1; i < len1 + 1; i++){
		dp[i][0] = i;
	}

	/* dp[0][XXX] 表示把 s2 改成 "" 所需要的步骤 */
	for(j = 1; j < len2 + 1; j++){
		dp[0][j] = j;
	}
	
	/*3. 动态规划*/
	for(i = 1; i < len1 + 1; i++){
		for(j = 1; j < len2 + 1; j++){
			/*注意比较的时候要 -1，因为dp比 S下标大1,0的位置被空串占据了*/
			if(word1[i - 1] == word2[j - 1]){ /*边上的两个相同，就不需要改变字符串了，距离不变*/
				dp[i][j] = dp[i - 1][j - 1];
			} else {
				/* 不相等，从子串中找到距离最小的 */
				ret = MIN(dp[i - 1][j - 1], dp[i][j - 1]);
				dp[i][j] = MIN(dp[i - 1][j], ret);
				/* 因为不相等， 当前字符也要改，所以加1*/
				dp[i][j] = dp[i][j] + 1;
			}
		}
	}

	/*4. 取得结果*/
	ret = dp[len1][len2];

	/*5. 释放动态规划表 */
	for(i = 0; i <= len1; i++){
		free(dp[i]);
	}	
	free(dp);
	dp = NULL;
	
	
	return ret;
}



void testminDistance(void){
	
	printf("\n************  testminDistance ************ \n");
	int ret = 0;
	
	char * str1 = "horse";
	char * str2 = "ros";
	ret = minDistance(str1, str2);
	printf("The min change distance of \"%s\" and  \"%s\" is %d\n", str1, str2, ret);

	char * str3 = "intention";
	char * str4 = "execution";
	ret = minDistance(str3, str4);
	printf("The min change distance of \"%s\" and  \"%s\" is %d\n", str3, str4, ret);

	return; 
 
 }


 int main(int argc, char ** argv){
	testminDistance();
 }



