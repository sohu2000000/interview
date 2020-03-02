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


int expend(char * s, int left, int right){
	int n = strlen(s);
	int len = 0;

	while(left >=0 && right < n && s[left] == s[right]){
		left--;
		right++;
	}

	/*
	 * 退出的时候，应该不满足条件，应该去掉首尾两个字符
	 * right - left，已经去掉了首字符，-1去掉尾字符
	 */
	len = right - left - 1;
	return len;
}


char * longestPalindrome2(char * s){

	if(NULL == s){
		return NULL;
	}

	int len = 0;
	int len1 = 0, len2 = 0;
	int subStart = 0, subEnd = 0;
	int size = 0;
	int i = 0;
	char * sub = NULL;

	size  = strlen(s);
	for(i = 0; i < size; i++){
		len1 = expend(s, i, i); /*[i, i] 为中心*/
		len2 = expend(s, i, i+1); /*[i, i] 为中心*/
		len = MAX(len1, len2);

		if(len > subEnd - subStart + 1){
			/*
			 * 这里start多减去一个1，是因为在双中心的情况下，
			 * 向前的距离要少去掉一个，因为双中心的头占了一个
		     */
			subStart = i - (len - 1)/ 2; 
			subEnd = i + len / 2;
		}	
	}

	len = subEnd - subStart + 1 + 1; /*减去start 补充1，'\0'再补充1*/
	sub = (char *)malloc(len * sizeof(char));
	memcpy(sub, s + subStart, len - 1); /*最后一个是'\0', 不用拷贝，单独赋值*/
	sub[len - 1] = '\0';	/*字符串结尾*/

	return sub;
}



void testlongestPalindrome(void){
	
	printf("\n************  testlongestPalindrome ************ \n");
	char str[100] = "babad";
	char str1[100] = "cbbd";
	char str2[100] = "a";
	char str3[100] = "";
	char str4[100] = "ac";
	char * s = NULL;
#if 1
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


	/*testcase 1*/
	s = longestPalindrome2(str);
	if(NULL != s){
		printf("The longest Palindrome2 of %s is %s\n", str,s);
	}


	/*testcase 2*/
	s = longestPalindrome2(str1);
	if(NULL != s){
		printf("The longest Palindrome2 of %s is %s\n", str1,s);
	}

	/*testcase 3*/
	s = longestPalindrome2(str2);
	if(NULL != s){
		printf("The longest Palindrome2 of %s is %s\n", str2,s);
	}

	/*testcase 4*/
	s = longestPalindrome2(str3);
	if(NULL != s){
		printf("The longest Palindrome2 of %s is %s\n", str3,s);
	}

	/*testcase 5*/
	s = longestPalindrome2(str4);
	if(NULL != s){
		printf("The longest Palindrome2 of %s is %s\n", str4,s);
	}
#endif

	return; 
 
 }


 int main(int argc, char ** argv){
	testlongestPalindrome();
 }








