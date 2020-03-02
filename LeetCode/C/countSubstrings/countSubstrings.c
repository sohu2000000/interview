/*
 * 需求
 
 给定一个字符串，你的任务是计算这个字符串中有多少个回文子串。
 
 具有不同开始位置或结束位置的子串，即使是由相同的字符组成，也会被计为是不同的子串。
 
 示例 1:
 输入: "abc"
 输出: 3
 解释: 三个回文子串: "a", "b", "c".

 示例 2:
 输入: "aaa"
 输出: 6
 说明: 6个回文子串: "a", "a", "a", "aa", "aa", "aaa".

 注意: 
 输入的字符串长度不会超过1000。
  
 gcc countSubstrings.c -g -o a.exe -DDEBUG

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

#define SIZE	1000

int expend(char * s, int left, int right){
	int ret = 0;
	int len = strlen(s);

	/* 注意是 < len, 不是 <=, 因为数组从0开始*/
	while(left >= 0 && right < len && s[left] == s[right]){
		right++;
		left--;
		ret++;
	}

	return ret;	
}

int countSubstrings(char * s){

	int cnt = 0;
	int i = 0;
	int ret1 = 0, ret2 = 0;
	int len = 0;
	
	if(NULL == s){
		goto out;
	}	

	len = strlen(s);
	/*[i,i],[i,i+1]两种中心方式进行扩展*/
	for(i = 0; i < len; i++){ /* 注意是 < len, 不是 <=, 因为数组从0开始*/
		ret1 = expend(s, i, i); 	//（i）作为中心位置向两边扩散
		ret2 = expend(s, i, i + 1);	//（i,i+1）向两边扩散
		cnt = cnt + ret1 + ret2;
	}

out:
	return cnt;
}


void testcountSubstrings(void){
	
	printf("\n************  testcountSubstrings ************ \n");
	char str[100] = "abc";
	char str1[100] = "aaa";
	int n = 0;

	/*testcase 1*/
	n = countSubstrings(str);	
	printf("The Palindrome strings in %s is %d\n", str,n);

	/*testcase 2*/
	n = countSubstrings(str1);	
	printf("The Palindrome strings in %s is %d\n", str1,n);

	return; 
 }

 int main(int argc, char ** argv){
	testcountSubstrings();
 }




