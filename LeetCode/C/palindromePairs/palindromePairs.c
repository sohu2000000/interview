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


#define SIZE 100

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


void testpalindromePairs(void){
	
	printf("\n************  testpalindromePairs ************ \n");
	char ** words = NULL;
	int wordsSize = 0;
	int returnSize = 0;
	int* returnColumnSizes = NULL;
	int i = 0;
	int ** ret = NULL;
	
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

#endif

	return; 
 
 }


 int main(int argc, char ** argv){
	testpalindromePairs();
 }













