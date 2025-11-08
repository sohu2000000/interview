/*
 * 需求
 
 给定一组唯一的单词， 找出所有不同 的索引对(i, j)，使得列表中的两个单词， words[i] + words[j] ，可拼接成回文串。
 
 示例 1: 
 输入: ["abcd","dcba","lls","s","sssll"]
 输出: [[0,1],[1,0],[3,2],[2,4]] 
 解释: 可拼接成的回文串为 ["dcbaabcd","abcddcba","slls","llssssll"]

 示例 2:
 输入: ["bat","tab","cat"]
 输出: [[0,1],[1,0]] 
 解释: 可拼接成的回文串为 ["battab","tabbat"]
   
 gcc palindromePairs.c -g -o a.exe -DDEBUG

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

#define SIZE 5000

/*
 * 注意：调用者需要确认拼接的串顺序为，st1 + str2, str2在后面 
 */
bool isPalindromeConbineStr(char * str1, char * str2){
	bool ret = true;
	int len1 = strlen(str1);
	int len2 = strlen(str2);
	char * p1 = NULL, *t1 = NULL;	
	char * p2 = NULL, *t2 = NULL;

	p1 = str1;
	if(0 != len1){
		t1 = p1 + len1 - 1;
	} else {
		t1 = p1;
	}

	p2 = str2;
	if(0 != len2){
		t2 = p2 + len2 - 1;
	} else {
		t2 = p2;
	}

	/*比较前缀和后缀部分*/
	while(p1 != t1 + 1 && p2 - 1 != t2 && '\0' != *p1 && '\0' != *t2) {
		if(*p1 != *t2){
			ret = false;
			goto out;
		}

		p1++;
		t2--;
	} 

	/*比较str1长出的部分*/
	while(p1 <= t1 && '\0' != *p1) {
		if(*p1 != *t1){
			ret = false;
			goto out;
		}
		p1++;
		t1--;
	}

	/*比较str2长出的部分*/
	while(p2 <= t2  && '\0' != *p2) {
		if(*p2 != *t2){
			ret = false;
			goto out;
		}
		p2++;
		t2--;
	}

out:
	return ret;
}



/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */
int** palindromePairs(char ** words, int wordsSize, int* returnSize, int** returnColumnSizes){

	if(NULL == words || 0 >= wordsSize){
		*returnColumnSizes = NULL;
		return NULL;
	}

	int ** retp = NULL;
	int i = 0, j = 0, k = 0;
	int size = 2 * wordsSize;

	retp = (int** )malloc(size * (sizeof(int *)));
	for(i = 0; i < size; i++){
		retp[i] = (int *)malloc(10 * sizeof(int)); /*因为每行2个数字进行拼接*/
	}

	*returnColumnSizes = (int *)malloc(size * sizeof(int));	
	for(i = 0; i < size; i++){
		*(*returnColumnSizes + i) = 2; /*因为是两两组合，所以列宽度都是2*/
	}

	for(i = 0; i < wordsSize; i++){
		for(j = 0; j < wordsSize; j++){
			if(i == j){
				continue;
			}

			if(isPalindromeConbineStr(words[i], words[j])){
				retp[k][0] = i;
				retp[k][1] = j;
				k++;
			}			
		}
	}

	*returnSize = k;

	return retp;

}


#if 0

bool isPalindromeStr(char * head, char * tail){
	bool ret = true;

	while(head < tail){
		if(*head != *tail){
			ret = false;
			break;
		}

		head++;
		tail--;		
	}

	return ret;
}


/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */
int** palindromePairs2(char ** words, int wordsSize, int* returnSize, int** returnColumnSizes){

	if(NULL == words || 0 >= wordsSize){
		*returnColumnSizes = NULL;
		return NULL;
	}

	int ** retp = NULL;
	char *p = NULL, *t = NULL;
	int row = 0, col = 0;
	int len1 = 0, len2 = 0, len = 0;
	int i = 0, j = 0, k = 0;
	char tester[100] = {'\0'};

	retp = (int** )malloc(SIZE * (sizeof(int *)));
	for(i = 0; i < SIZE; i++){
		retp[i] = (int *)malloc(SIZE * sizeof(int));
	}

	*returnColumnSizes = (int *)malloc(SIZE * sizeof(int));	
	for(i = 0; i < SIZE; i++){
		*(*returnColumnSizes + i) = 2; /*因为是凉凉组合，所以列宽度都是2*/
	}

	for(i = 0; i < wordsSize; i++){
		for(j = 0; j < wordsSize; j++){
			if(i == j){
				continue;
			}

			len1 = strlen(words[i]);
			len2 = strlen(words[j]);
			len = len1 + len2;
			
			/*验证字符串 [i + j]*/
			memset(tester, 0x0, 100);
			strcpy(tester, words[i]);
			tester[len1] = '\0';
			strcat(tester, words[j]);
			tester[len] = '\0';

			p = tester;
			t = tester + len - 1;

			if(isPalindromeStr(p,t)){
				retp[k][0] = i;
				retp[k][1] = j;
				k++;
			}			
		}
	}

	*returnSize = k;

	return retp;

}
#endif

void testpalindromePairs(void){
	
	printf("\n************  testpalindromePairs ************ \n");
	char ** words = NULL;
	int wordsSize = 0;
	int returnSize = 0;
	int* returnColumnSizes = NULL;
	int i = 0;
	int ** ret = NULL;
	
#if 1
	bool bret = false;
	char * str1 = "lls";
	char * str2 = "sssll";
	bret = isPalindromeConbineStr(str1, str2);
	if(bret){
		printf("%s + %s is a Palindrome\n", str1, str2);
	} else {
		printf("%s + %s not a Palindrome\n", str1, str2);
	}

	char * str3 = "abcd";
	char * str4 = "dcba";
	bret = isPalindromeConbineStr(str1, str2);
	if(bret){
		printf("%s + %s is a Palindrome\n", str3, str4);
	} else {
		printf("%s + %s not a Palindrome\n", str3, str4);
	}

	char * str5 = "a";
	char * str6 = "dcba";
	bret = isPalindromeConbineStr(str5, str6);
	if(bret){
		printf("%s + %s is a Palindrome\n", str5, str6);
	} else {
		printf("%s + %s not a Palindrome\n", str5, str6);
	}

	char * str7 = "";
	char * str8 = "llsll";
	bret = isPalindromeConbineStr(str7, str8);
	if(bret){
		printf("%s + %s is a Palindrome\n", str7, str8);
	} else {
		printf("%s + %s not a Palindrome\n", str7, str8);
	}

	bret = isPalindromeConbineStr(str8, str7);
	if(bret){
		printf("%s + %s is a Palindrome\n", str8, str7);
	} else {
		printf("%s + %s not a Palindrome\n", str8, str7);
	}


	char * str9 = "";
	char * str10 = "";
	bret = isPalindromeConbineStr(str9, str10);
	if(bret){
		printf("%s + %s is a Palindrome\n", str9, str10);
	} else {
		printf("%s + %s not a Palindrome\n", str9, str10);
	}
#endif

#if 1
	/*testcase 1*/
	words = (char** )malloc(SIZE * (sizeof(char *)));
	for(i = 0; i < SIZE; i++){
		words[i] = (char *)malloc(SIZE * sizeof(char));
		memset(words[i],'\0', SIZE);
	}
	
	wordsSize = 5;
	memcpy(words[0], "abcd", 5);
	memcpy(words[1], "dcba", 5);
	memcpy(words[2], "lls", 4);
	memcpy(words[3], "s", 2);
	memcpy(words[4], "sssll", 6);

	ret = palindromePairs(words, wordsSize, &returnSize, &returnColumnSizes);
	printf("-------------  returnSize = %d -------------\n", returnSize);
	printf("\t[ ");
	for(i = 0; i < returnSize; i++){
		printf(" [ ");
		printf("%d,", ret[i][0]);
		printf("%d ], ", ret[i][1]);
	}
	printf(" ] \n");
	free(ret);
	ret = NULL;
	free(returnColumnSizes);
	returnColumnSizes = NULL;
	for(i = 0; i < SIZE; i++){
		free(words[i]);
		words[i] = NULL;
	}	
	free(words);
	words = NULL;

	/*testcase 2*/
	words = (char** )malloc(SIZE * (sizeof(char *)));
	for(i = 0; i < SIZE; i++){
		words[i] = (char *)malloc(SIZE * sizeof(char));
		memset(words[i],'\0', SIZE);
	}
	
	wordsSize = 2;
	memcpy(words[0], "", 0);
	memcpy(words[1], "a", 1);

	ret = palindromePairs(words, wordsSize, &returnSize, &returnColumnSizes);
	printf("-------------  returnSize = %d -------------\n", returnSize);
	printf("\t[ ");
	for(i = 0; i < returnSize; i++){
		printf(" [ ");
		printf("%d,", ret[i][0]);
		printf("%d ], ", ret[i][1]);
	}
	printf(" ] \n");
	free(ret);
	ret = NULL;
	free(returnColumnSizes);
	returnColumnSizes = NULL;
	for(i = 0; i < SIZE; i++){
		free(words[i]);
		words[i] = NULL;
	}	
	free(words);
	words = NULL;


	/*testcase 3*/
	words = (char** )malloc(SIZE * (sizeof(char *)));
	for(i = 0; i < SIZE; i++){
		words[i] = (char *)malloc(SIZE * sizeof(char));
		memset(words[i],'\0', SIZE);
	}
	
	wordsSize = 6;
	memcpy(words[0], "a", 1);
	memcpy(words[1], "b", 1);
	memcpy(words[2], "c", 1);
	memcpy(words[3], "ab", 2);
	memcpy(words[4], "ac", 2);
	memcpy(words[5], "aa", 2);
	
	ret = palindromePairs(words, wordsSize, &returnSize, &returnColumnSizes);
	printf("-------------  returnSize = %d -------------\n", returnSize);
	printf("\t[ ");
	for(i = 0; i < returnSize; i++){
		printf(" [ ");
		printf("%d,", ret[i][0]);
		printf("%d ], ", ret[i][1]);
	}
	printf(" ] \n");
	free(ret);
	ret = NULL;
	free(returnColumnSizes);
	returnColumnSizes = NULL;
	for(i = 0; i < SIZE; i++){
		free(words[i]);
		words[i] = NULL;
	}	
	free(words);
	words = NULL;


	/*testcase 4*/
	words = (char** )malloc(SIZE * (sizeof(char *)));
	for(i = 0; i < SIZE; i++){
		words[i] = (char *)malloc(SIZE * sizeof(char));
		memset(words[i],'\0', SIZE);
	}
	
	wordsSize = 3;
	memcpy(words[0], "a", 1);
	memcpy(words[1], "aa", 2);
	memcpy(words[2], "aaa", 3);
	
	ret = palindromePairs(words, wordsSize, &returnSize, &returnColumnSizes);
	printf("-------------  returnSize = %d -------------\n", returnSize);
	printf("\t[ ");
	for(i = 0; i < returnSize; i++){
		printf(" [ ");
		printf("%d,", ret[i][0]);
		printf("%d ], ", ret[i][1]);
	}
	printf(" ] \n");
	free(ret);
	ret = NULL;
	free(returnColumnSizes);
	returnColumnSizes = NULL;
	for(i = 0; i < SIZE; i++){
		free(words[i]);
		words[i] = NULL;
	}	
	free(words);
	words = NULL;

#endif

	return; 
 
 }


 int main(int argc, char ** argv){
	testpalindromePairs();
 }













