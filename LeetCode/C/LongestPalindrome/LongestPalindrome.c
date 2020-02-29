/*
 * 需求
 
 给定一个字符串 s，找到 s 中最长的回文子串。你可以假设 s 的最大长度为 1000。
 
 示例 1：
 
 输入: "babad"
 输出: "bab"
 注意: "aba" 也是一个有效答案。
 示例 2：
 
 输入: "cbbd"
 输出: "bb"
  
 gcc LongestPalindrome.c -g -o a.exe -DDEBUG

 */
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


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

char * longestPalindrome(char * s){
	if(NULL == s){
		return NULL;
	}

	char * pLongest = NULL, *qLongest = NULL;
	char * p = NULL, *q = NULL, *t = NULL, *r = NULL;
	int npLongest = 0, n = 0, len = 0;
	bool ret;

	len = strlen(s);
	p = s;
	t = s + len - 1;

	if(1 == len){
		pLongest = s;
		qLongest = t;
		goto out;
	} else if (0 == len) {
		pLongest = s;
		qLongest = s;
		goto out;
	}

	
	while('\0' != *p){
		
		/*因为找最长的，所以q从尾部开始向前*/
		q = t;
		/*
		 * 这里用do..while，因为q = p 也要处理，
		 * 因为一个单独的字符"a"也算回文串
		 */
		do {
			if(*q == *p){
				ret = isPalindromeStr(p, q);
				if(ret){
					n = q - p + 1;
					if(n > npLongest){
						npLongest = n;
						pLongest = p;
						qLongest = q;
					}
					break;
				}								
			}
			q--;
		}while(q != p);
		p++;
	}


	/*没有找到回文串*/
	if(NULL == pLongest && NULL == qLongest){
		return NULL;
	}


	len = qLongest - pLongest + 1;
	if(0 == len){
		return NULL;
	}

out:
	r = (char * )malloc((len + 1) * sizeof(char));
	memcpy(r, pLongest, len);
	r[len] = '\0';

	return r;
}


void testlongestPalindrome(void){
	
	printf("\n************  testlongestPalindrome ************ \n");
	char str[100] = "babad";
	char str1[100] = "cbbd";
	char str2[100] = "a";
	char str3[100] = "";
	char str4[100] = "ac";
	char * s = NULL;

	/*testcase 1*/
	s = longestPalindrome(str);
	if(NULL != s){
		printf("The longest Palindrome of %s is %s\n", str,s);
	}

	/*testcase 2*/
	s = longestPalindrome(str1);
	if(NULL != s){
		printf("The longest Palindrome of %s is %s\n", str1,s);
	}

	/*testcase 3*/
	s = longestPalindrome(str2);
	if(NULL != s){
		printf("The longest Palindrome of %s is %s\n", str2,s);
	}

	/*testcase 4*/
	s = longestPalindrome(str3);
	if(NULL != s){
		printf("The longest Palindrome of %s is %s\n", str3,s);
	}

	/*testcase 5*/
	s = longestPalindrome(str4);
	if(NULL != s){
		printf("The longest Palindrome of %s is %s\n", str4,s);
	}

	return; 
 
 }


 int main(int argc, char ** argv){
	testlongestPalindrome();
 }








