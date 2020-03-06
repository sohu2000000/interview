/*
 * 需求
 
 给定一个字符串 s，你可以通过在字符串前面添加字符将其转换为回文串。找到并返回可以用这种方式转换的最短回文串。
 
 示例 1:
 输入: "aacecaaa"
 输出: "aaacecaaa"

 示例 2: 
 输入: "abcd"
 输出: "dcbabcd"
    
 gcc shortestPalindrome.c -g -o a.exe -DDEBUG

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


int longestPalindromeSubstrFromHead(char * s, char ** pdrear){
	int len = 0, pdlen = 0;
	char * rear = NULL;

	len = strlen(s);
	rear = s + len - 1;

	if('\0' == *s){
		*pdrear = s;
		return pdlen;
	}
	
	do {
		if(isPalindromeStr(s, rear)){ 
			/*找到了从0开始最长的回文串*/
			break;
		}
		rear--;
	} while(rear !=  s);

	*pdrear = rear;
	
	pdlen = rear - s + 1;
	
	return pdlen;
}



char * shortestPalindrome(char * s){
	if(NULL == s){
		return NULL;
	}

	char * rear = NULL, * pdrear = NULL;
	char * substr = NULL, *p = NULL;
	int pdlen = 0, len = 0, total = 0;

	
	len = strlen(s);
	if(0 == len){ /*空字符串*/
		substr = (char *)malloc(1 * sizeof(char));
		*substr = '\0';
		goto out;
	}

	
	rear = s + len - 1;

	pdlen = longestPalindromeSubstrFromHead(s, &pdrear);
	total = len - pdlen + len + 1; /*后缀长度 + 原有字符串长度 + '\0' */
	LOG("s = %s, pdlen = %d, pdrear = %c, total= %d\n", s, pdlen, *pdrear, total);
	
	substr = (char *)malloc(total * sizeof(char));
	if(NULL == substr){
		return NULL;
	}
	
	p = substr;
	while(rear != pdrear){
		*p = *rear;
		p++;
		rear--;
	}

	memcpy(p, s, len);
	substr[total - 1] = '\0';
	LOG("substr = %s\n", substr);

out:
	return substr;
}



void testshortestPalindrome(void){
	
	printf("\n************  testshortestPalindrome ************ \n");
	char * pdstr = NULL;
	
#if 1
	int pdlen = 0;
	char astr1[100] = "aacecaaa";
	char * rear = NULL;
	
	pdlen = longestPalindromeSubstrFromHead(astr1, &rear);
	*(rear + 1) = '\0';
	printf("The longest PalindromeSubstr From Head is \"%s\", long %d \n", astr1, pdlen);

	char astr2[100] = "abcd";
	rear = NULL;
	pdlen = longestPalindromeSubstrFromHead(astr2, &rear);
	*(rear + 1) = '\0';
	printf("The longest PalindromeSubstr From Head is \"%s\" long %d \n", astr2, pdlen);

	char astr3[100] = "";
	rear = NULL;
	pdlen = longestPalindromeSubstrFromHead(astr3, &rear);
	*(rear + 1) = '\0';
	printf("The longest PalindromeSubstr From Head is \"%s\" long %d \n", astr3, pdlen);
#endif

#if 1
	/*testcase 1*/
	char * str1 = "aacecaaa";
	pdstr = shortestPalindrome(str1);
	printf("The shortest pdstr is \"%s\" \n", pdstr);
	if(NULL != pdstr){
		free(pdstr);
		pdstr = NULL;
	}

	/*testcase 2*/
	char * str2 = "abcd";
	pdstr = shortestPalindrome(str2);
	printf("The shortest pdstr is \"%s\" \n", pdstr);
	if(NULL != pdstr){
		free(pdstr);
		pdstr = NULL;
	}

	/*testcase 2*/
	char * str3 = "";
	pdstr = shortestPalindrome(str3);
	printf("The shortest pdstr is \"%s\" \n", pdstr);
	if(NULL != pdstr){
		free(pdstr);
		pdstr = NULL;
	}

	
#endif

	return; 
 
 }


 int main(int argc, char ** argv){
	testshortestPalindrome();
 }

