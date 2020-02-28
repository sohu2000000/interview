

#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>

#include "str.h"


int int2str(char * str, int num){

	if(NULL == str){
		printf("int2str param error\n");
		return PARAM_ERR;
	}

	int negative = 0;
	char buf[100] = {0};
	int len = 0, k = 0, i = 0, tmp = 0, n = 0, lenstr = 0;


	if(num < 0){
		negative = 1;
		n = -num;
	} else {
		n = num;
	}

	do{
		tmp = n % 10;
		buf[k++] = tmp + '0';
		n = n / 10;
	} while(n != 0);

	len = k; /*len 不用在 + 1，因为 k++了*/
	lenstr = len;

	k = 0;
	if(1 == negative){
		str[k++] = '-';
		lenstr = lenstr + 1;
	}

	for(i = len - 1; i >= 0; i--){
		str[k++] = buf[i];
	}
	
	str[lenstr] = '\0';
	
	return SUCCESS;

}


int str2int(char * str, int * rst){
	if(NULL == str){
		printf("int2str param error\n");
	}

	int num = 0, n = 0, i = 0, k = 0, digit = 0, len = 0;
	int negative = 0;

	len = strlen(str);

	for(i = len - 1; i >= 0; i--, digit++){
		if('-' == str[i]){
			num = -num;
			continue;
		}

		n = str[i] - '0';

		k = digit;
		while(k > 0){
			n = n * 10;
			k--;
		}
		num = num + n;
	}

	*rst = num;	
}


int libstr2int(char * str, int * rst){
	if(NULL == str){
		printf("int2str param error\n");
		return PARAM_ERR;
	}

	*rst = atoi(str);

	return SUCCESS;
}

int libstr2float(char * str, float * rst){
	if(NULL == str){
		printf("int2str param error\n");
		return PARAM_ERR;
	}

	*rst = atof(str);

	return SUCCESS;
}


int mystrcpy(char * dst, char * src){
	if(NULL == dst || NULL == src){
		printf("mystrcpy param error\n");
		return PARAM_ERR;
	}	

	char * p = NULL, * q = NULL;

	p = dst;
	q = src;	
	while(*q != '\0'){
		*p++ = *q++;
	}
	*p = '\0';

	return SUCCESS;
}


void * mymemcpy(void * dst, void * src, int size){
	if(NULL == dst || NULL == src){
		printf("mystrcpy param error\n");
		return NULL;
	}

	char * p = dst;
	char * q = src;

	while(size != 0){
		*p++ = *q++;
		size--;
	}

	return dst;
}


int mystrlen(char * str){
	if(NULL == str){
		printf("mystrcpy param error\n");
		return 0;
	}

	char * c = NULL;
	int len = 0;

	c = str;
	while(*c++ != '\0'){
		len++;
	}

	return len;
}


char * mystrstr(char * str, char * sub){
	if(NULL == str || NULL == sub){
		printf("mystrstr param error\n");
		return NULL;
	}

	char * p = NULL, * q = NULL, * c = NULL;
	int found = 0;

	p = str;
	while(*p != '\0'){

		q = sub;
		c = p;
		while(*c == *q && *q != '\0'){
			q++;
			c++;
		}

		if('\0' == *q){
			found = 1;
			break;			
		}
		p++;
	}

	if(1 == found){
		return p;
	} else {
		return NULL;
	}
}



int strReverse(char * head, char * tail){
	if(NULL == head || NULL == tail){
		printf("strReverse param error!\n");
		return PARAM_ERR;
	}
	
	char tmp;

	while(head < tail){
		tmp = *head;
		*head = *tail;
		*tail = tmp;

		head++;
		tail--;
	}
	
	return SUCCESS;
}

int strReversWithoutWord (char * str){
	if(NULL == str){
		printf("strReverse param error!\n");
		return PARAM_ERR;
	}

	int len = 0;
	char * head = NULL, * tail = NULL;
	char * p = NULL, * q = NULL;

	/*1. 整个字符串颠倒*/
	len  = strlen(str);
	head = str;
	tail = str + len - 1;
	strReverse(head, tail);
	
	/*逐个单词颠倒回来*/
	p = str;
	while(*p != '\0'){
		head = p;
		q = p;
		while(' ' != *q && '\0' != *q){
			q++;
		}
		tail = q - 1;
		strReverse(head, tail);
		
		p = q + 1;
	}
	
}


int strReversWithWord (char * str){
	if(NULL == str){
		printf("strReverse param error!\n");
		return PARAM_ERR;
	}

	int len = 0, rst = 0;
	char * head = NULL, * tail = NULL;

	len = strlen(str);
	head = str;
	tail = str + len - 1;
	
	rst = strReverse(head, tail);

	return rst;
}

int strReversWithoutSubstr (char * str, char * substr){
	if(NULL == str || NULL == substr){
		printf("strReverse param error!\n");
		return PARAM_ERR;
	}

	int len = 0, sublen = 0;
	char * head = NULL, * tail = NULL;
	char * p = NULL, * q = NULL;

	/*1. 整个字符串颠倒*/
	len  = strlen(str);
	head = str;
	tail = str + len - 1;
	strReverse(head, tail);

	/*2. 把子串颠倒*/
	len  = strlen(substr);
	head = substr;
	tail = substr + len - 1;
	strReverse(head, tail);
	
	
	/*3. 把子串颠倒回来*/
	sublen = strlen(substr);
	head = mystrstr(str, substr);
	tail = head + sublen - 1;
	strReverse(head, tail);

}



bool isPalindromeStr(char * str){
	if(NULL == str){
		printf("isPalindromeStr param error!\n");
		return false;
	}


	bool flag = true;

	char * head = str;
	char * tail = str + strlen(str) - 1;

	while(head < tail){
		if(*head != *tail){
			flag = false;
			break;
		}
		head++;
		tail--;
	}

	return flag;
	
}


int mystrcmp(char * str1, char * str2){

	char * p = str1;
	char * q = str2;
	int len1 = strlen(str1);
	int len2 = strlen(str2);

	if(len1 > len2){
		return -1;
	} else if (len1 < len2){
		return -1;
	}
	
	while(('\0' != p) && ('\0' != *q)){
		if(*p != *q){
			if(*p > *q){
				return 1;
			} else if (*p < *q){
				return -1;
			} else {
			}
		}
		p++;
		q++;
	}

	return 0;
}


char * commonStr(const char * str1, const char * str2){
	if(NULL == str1 || NULL == str2){
		printf("commonStr param error!\n");
		return false;
	}

	const char * p = NULL;
	const char * q = NULL;
	const char * start = NULL, *p1 = NULL, *q1 = NULL;
	char * commstr = NULL;
	int len = 0, max = 0;

	p = str1;
	while('\0' != *p){
		q = str2;
		while('\0' !=  *q){
			if(*p == *q){ /*发现相等的了, 开始匹配*/
				len = 0;
				p1 = p;
				q1 = q;

				if(NULL == start){
					start = p;
				}
				
				/*内循环查找common子串*/
				while(('\0' != *p1) && ('\0' != *q1) && (*p1 == *q1)){
					len++;
					p1++;
					q1++;
				}

				if(len > max){
					max = len;
					start = p;
				}
			}
			q++;
		}		
		p++;
	}

	commstr = (char *)malloc(max + 1);
	memset(commstr, 0x0, max + 1);
	
	memcpy(commstr, start, max);
	commstr[max] = '\0';

	return commstr;
}


static char hexMetric[16] = { \
	'0', '1', '2', '3', \
	'4', '5', '6', '7', \
	'8', '9', 'A', 'B', \
	'C', 'D', 'E', 'F', \
};

int int2HexStr(char * str, int num){

	if(NULL == str){
		printf("int2HexStr param error\n");
		return PARAM_ERR;
	}

	int base = 16;
	int len  = 0, n = 0, a = 0, k = 0;
	char buf[100] = {'0'};

	n = num;
	do {
		a = n % base;
		n = n / base;
		buf[len++] = hexMetric[a];
	} while(n != 0);
	buf[len] = '\0';

	strReversWithWord(buf);

	str[0] = '0';
	str[1] = 'x';
	str = str + 2;

	memcpy(str, buf, len);
		
	return SUCCESS;

}


static char binMetric[16] = { \
	'0', '1' \
};

int int2BinStr(char * str, int num){
	if(NULL == str){
		printf("int2BinStr param error\n");
		return PARAM_ERR;
	}

	int base = 2;
	int len  = 0, n = 0, a = 0, k = 0;
	char buf[100] = {'0'};

	n = num;
	do {
		a = n % base;
		n = n >> 1;
		buf[len++] = binMetric[a];
	} while(n != 0);
	buf[len] = '\0';

	strReversWithWord(buf);

	str[0] = '0';
	str[1] = 'b';
	str = str + 2;

	memcpy(str, buf, len);
	return SUCCESS;
}


int statStr(char * str){
	if(NULL == str){
		printf("statStr param error\n");
		return PARAM_ERR;
	}

	int len = strlen(str) + 1; /*加上 '\0'*/
	int scanlen = 0; /*扫描的长度，从1开始*/
	int stat = 1;
	char * p = NULL;
	char * q = NULL;
	char s;

	p = str;
	while('\0' != *p){
		q = p;
		stat = 0; /*至少有自己重复*/
		while('\0' != q && *q == *p){
			stat++;
			q++;
		}
		scanlen += stat;

		s = stat + '0';
		memcpy(p + stat + 1, p + stat, len - scanlen);	
		*(p + stat) = s; /*填入统计数字*/
		scanlen += 1; 	/*因为插入了一个数字*/
		len = len + 1; 	/*因为插入了一个数字*/
		p = p + stat + 1; /* + 1 因为插入了一个数字*/
	}

	return SUCCESS;	
	
}


static char lowerMap[26] = { \
	'a','b','c','d','e','f','g', \
	'h','i','j','k','l','m','n', \
	'o','p','q','r','s','t','u', \
	'v','w','x','y','z' \
};

static char upperMap[26] = { \
	'A','B','C','D','E','F','G', \
	'H','I','J','K','L','M','N', \
	'O','P','Q','R','S','T','U', \
	'V','W','X','Y','Z' \
};

int encodeStr(char * str){
	if(NULL == str){
		printf("encodeStr param error\n");
		return PARAM_ERR;
	}

	char * p = NULL;
	int dist = 0;

	p = str;
	while('\0' != *p){
		if(*p >= 'a' && *p <= 'z'){
			dist = *p - 'a';
			dist = dist + 4;
			dist = dist % 26;
			*p = lowerMap[dist];
		} else if (*p >= 'A' && *p <= 'Z'){
			dist = *p - 'A';
			dist = dist + 4;
			dist = dist % 26;
			*p = upperMap[dist];
			
		} else {
			
		}
		
		p++;
	}

	strReversWithWord(str);
	
	return SUCCESS;

}


int addBigInt(char * str1, char * str2, char * rst){
	if(NULL == str1 || NULL == str2 || NULL == rst){
		printf("addBigInt param error\n");
		return PARAM_ERR;
	}

	char * p1 = NULL, *p2 = NULL, * pr = NULL;
	char * tail1 = NULL, *tail2 = NULL, *tailr = NULL;
	int len1 = strlen(str1);
	int len2 = strlen(str2);
	int len = len1 > len2 ? len1 : len2;
	int a = 0, b = 0, r= 0, c = 0;
	
	len = len + 1; /*最后可能发生进位*/
	tail1 = str1 + len1 - 1;
	tail2 = str2 + len2 - 1;
	pr = rst;
	tailr = pr + (len - 1);
	*(pr + len) = '\0'; /*结尾*/
	

	/*
	 * 注意这里要用str-1, 因为字符串的头本身也要处理，
	 * 结束条件是头前面的一个指针地址
	 */
	while(tail1 != str1 - 1 &&  tail2 != str2 - 1){ 
		a = *tail1 - '0';	
		b = *tail2 - '0';
		r = a + b + c;
		if(r >= 10){
			r = r % 10;
			c = 1;
		} else {
			c = 0;
		}
		*tailr = '0' + r;		

		tail1--;
		tail2--;
		tailr--;
	}

	while(tail1 != str1 - 1){
		a = *tail1 - '0';
		r = a + c;
		if(r >= 10){
			r = r % 10;
			c = 1;
		} else {
			c = 0;
		}
		*tailr = '0' + r;

		tail1--;
		tailr--;
	}

	while(tail2 != str2 - 1){
		b = *tail2 - '0';
		r = b + c;
		if(r >= 10){
			r = r % 10;
			c = 1;
		} else {
			c = 0;
		}
		*tailr = '0' + r;

		tail2--;
		tailr--;
	}

	if(0 == c){
		memcpy(tailr, tailr + 1, len - 1);
		*(rst + len - 1) = '\0'; /*因为字符串迁移了，所以要提前一位写入'\0'标明结束*/
	} else if (1 == c){
		*tailr = '0' + c;
	}

	return SUCCESS;	
	
}


int rightShiftStr(char * str, int n){
	if(NULL == str){
		printf("addBigInt param error\n");
		return PARAM_ERR;
	}

	int len = strlen(str);
	if(n > len){
		printf("addBigInt param error2\n");
		return PARAM_ERR;
	}

	char * p = NULL;
	char * t = str + len - 1;
	char buf[100] = {'\0'};
	int k = 0;

	p = t - n + 1;
	for(k = 0; k < n; k++){
		buf[k] = *p;
		p++;
	}

	p = str + n;
	memcpy(p, str, len - n);

	p = str;
	for(k = 0; k < n; k++){
		*p = buf[k];
		p++;
	}
	
	return SUCCESS;
	
}


int removeSubString(char * str, int index, int size){
	if(NULL == str || index < 0 || size < 0){
		printf("addBigInt param error\n");
		return PARAM_ERR;
	}

	char * p = NULL, *t = NULL;
	int len  = strlen(str);

	p = str + index;
	t = p + size;

	memcpy(p, t, len - index - size);	
	str[len - size] = '\0';

	return SUCCESS;
}


int sortHalfStr(char * str){
	if(NULL == str){
		printf("strSortSwap param error\n");
		return PARAM_ERR;
	}

	int len  = strlen(str);	
	int i = 0, j = 0, sz = len / 2;
	char * p = str;
	char * t = str + sz - 1;
	char tmp;

	for(i = 0; i < sz; i++){
		for(j = i + 1; j < sz; j++){
			if(p[i] < p[j]){
				tmp = p[i];
				p[i] = p[j];
				p[j] = tmp;
			}
		}
	}

	return SUCCESS;

}


int strRemoveSpecChar(char * str, const char c){
	if(NULL == str){
		printf("strRemoveSpecChar param error\n");
		return PARAM_ERR;
	}	
	
	int len = strlen(str);	
	int k = 0;
	char * p = str;	
	char * t = str + len - 1;

	while('\0' != *p){
		if(c == *p){
			memcpy(p, p + 1, (t - p));
			k++;
			/*p 被覆盖掉了，p的位置是新字符，不用p++*/
			continue;
		}
		p++;
	}

	str[len - k] = '\0';

	return SUCCESS;
}



char * mystrcat(char * str1, char * str2){
	if(NULL == str1 || NULL == str2){
		printf("mystrcat param error\n");
		return NULL;
	}	
	
	int len1  = strlen(str1);
	int len2  = strlen(str2);
	char * p1 = str1;
	char * p2 = str2;
	char * t1 = p1 + len1 - 1;

	t1++; /*复制到第一个字符串的'\0'的地方*/
	memcpy(t1, p2, len2);
	str1[len1 + len2] = '\0';

	return str1;
}


int gbkstrlen(char * str){
	if(NULL == str){
		printf("mystrcat param error\n");
		return PARAM_ERR;
	}		

	int len = 0;
	char * p = str;

	while('\0' != *p){
		/*中文字符直接跳过2个字符*/
		if(*p < 0 && (*(p + 1) < 0 || *(p + 1) > 63)){ /*中文字符*/
			p = p + 3;
		} else {
			p++;
		}		
		len++;
	}

	return len;
}


int getStrMaxSuccNum(char * str, int * pnum0, int * pnum1){
	if(NULL == str){
		printf("mystrcat param error\n");
		return PARAM_ERR;
	}

	char * p = str;
	char * q = NULL;
	int n = 0, num0 = 0, num1 = 0;

	while('\0' != *p){	
		q = p;
		n = 0;
		if('1' == *p){
			while('\0' != *q && '1' == *q){
				n++;
				q++;
			}
			if(n > num1){
				num1 = n;
			}
			p = q; /*不用加1，因为退出循环之前，已经q++*/
			continue;
		} else if('0' == *p){
			while('\0' != *q && '0' == *q){
				n++;
				q++;
			}
			if(n > num0){
				num0 = n;
			}
			p = q; /*不用加1，因为退出循环之前，已经q++*/
			continue;
		} else {
		
		}
		
		p = p + 1;		
	}

	*pnum0 = num0;
	*pnum1 = num1;

	return SUCCESS;
}


int strReplaceAll(char * str, char * sub, char * replace){

	if(NULL == str || NULL == sub || NULL == replace){
		printf("mystrcat param error\n");
		return PARAM_ERR;
	}	

	char * p = NULL;
	char * t = NULL;
	char * q = NULL;
	char * dst = NULL;
	char * src = NULL;
	

	int len = strlen(str);
	int len1 = strlen(sub);
	int len2 = strlen(replace);

	p = str;
	while('\0' != *p){
		t = str + len;
		q = strstr(str, sub);
		if(NULL == q) /*没有子串了，那么直接返回吧*/{
			break;
		}

		src = q + len1; /*源头, 原有sub后的一个字符*/
		dst = q + len2; /*目的，放完replace后的一个字符*/
		memcpy(dst, src, t - src); /*原有字符串后移，放出空间*/
		memcpy(q, replace, len2); /*将replace字符拷贝进来*/
		len = len + len2 - len1;
		
		p = q + len2; /* p 下一轮replace后的一个字符 */
	}
	
	str[len] = '\0'; /*通过'\0'表示结尾*/

}


void teststrReplaceAll(void){
	char str[100] = "abcdefcdg";
	char sub[100] = "cd";
	char replace[100] = "123";
	//char replace[100] = "2";
	int num0  = 0, num1 = 0;

	printf("\n************  teststrReplaceAll ************ \n");
	
	strReplaceAll(str, sub, replace);
	
	printf("After replace all, str is : %s\n", str);

	return;	
}



void testgetStrMaxSuccNum(void){
	char str[100] = "00001110110000001100110101101001010101011111010";
	int num0  = 0, num1 = 0;

	printf("\n************  testgetStrMaxSuccNum ************ \n");
	
	getStrMaxSuccNum(str, &num0, &num1);
	
	printf("Max successive 0 num is : %d \n Max successive 1 num is : %d\n", num0, num1);

	return;	
}

void testgbkstrlen(void){
	char str[100] = "abc你好123中国456";
	int len  = 0;

	printf("\n************  testgbkstrlen ************ \n");
	
	len  = gbkstrlen(str);
	
	printf("gbk string lenght is : %d\n", len);

	return;	

}


void testmystrcat(void){
	char str1[100] = "Hello";
	char str2[100] = " Feng!";

	printf("\n************  testmystrcat ************ \n");
	
	mystrcat(str1, str2);

	printf("after strcat is : %s\n", str1);

	return;	

}


void teststrRemoveSpecChar(void){
	char str[100] = "cabcdefcgchci";

	printf("\n************  teststrRemoveSpecChar ************ \n");
	
	strRemoveSpecChar(str, 'c');

	printf("after remove char 'c' is : %s\n", str);

	return;	
} 



void testsortHalfStr(void){
	char str[100] = "ADZDDJKJFIEJHGI";

	printf("\n************  testsortHalfStr ************ \n");
	
	sortHalfStr(str);

	printf("after sort half stirng is : %s\n", str);

	return;	
} 



void testremoveSubString(void){
	char str[100] = "abcdefg";

	printf("\n************  testremoveSubString ************ \n");
	
	removeSubString(str, 1, 2);

	printf("after remove sub stirng is : %s\n", str);

	return;	
} 



void testrightShiftStr(void){
	char str[100] = "123456789";

	printf("\n************  testrightShiftStr ************ \n");
	
	rightShiftStr(str, 6);

	printf("right shift 6 is : %s\n", str);

	return;	
} 


void testaddBigInt(void){
	char str1[100] = "123456789323";
	char str2[100] = "45671254563123";
	char str3[100] = "523456789323";
	char str4[100] = "99671254563123";	
	char rst[100] = {'\0'};

	printf("\n************  testaddBigInt ************ \n");
	
	addBigInt(str1, str2, rst);

	printf("%s + %s = %s\n", str1, str2, rst);

	memset(rst, 0x0, 100);

	addBigInt(str3, str4, rst);

	printf("%s + %s = %s\n", str3, str4, rst);


	return;
}



void testencodeStr(void){
	char str[100] = "hasdxz11HYZ";

	printf("\n************  testencodeStr ************ \n");	

	encodeStr(str);

	printf("Encoded str is: %s\n", str);

	return;		
}



void teststatStr(void){
	char str[100] = "aaabc";

	printf("\n************  teststatStr ************ \n");

	statStr(str);

	printf("Stat str is: %s\n", str);

	return;	
}



void testConvertNum2Str(void){
	char str[100] = {'0'};
	int num = 456789;

	printf("\n************  testConvertNum2Str ************ \n");

	int2HexStr(str,num);
	printf("The hex str is %s\n", str);

	int2BinStr(str,num);
	printf("The bin str is %s\n", str);

	return;
}

void testcommonStr(void){
	char * str1 = "aocdfe";
	char * str2 = "pmcdfa";
	char * commstr = NULL;

	printf("\n************  testcommonStr ************ \n");

	commstr = commonStr(str1, str2);
	printf("The common str is %s\n", commstr);

	free(commstr);
	commstr = NULL;

	return;
}


void testmystrcmp(void){
	char * str = "1234567";
	char * str1 = "1234567";
	char * str2 = "12345678";
	char * str3 = "1234566";
	int ret = 0;

	printf("\n************  testmystrcmp ************ \n");

	ret = mystrcmp(str, str1);
	if(0 == ret){
		printf("%s and %s is the same\n", str,str1);
	} else if (1 == ret) {
		printf("%s big than %s \n", str,str1);
	}else if (-1 == ret) {
		printf("%s less than %s \n", str,str1);
	} else {
	}


	ret = mystrcmp(str, str2);
	if(0 == ret){
		printf("%s and %s is the same\n", str,str2);
	} else if (1 == ret) {
		printf("%s big than %s \n", str,str2);
	}else if (-1 == ret) {
		printf("%s less than %s \n", str,str2);
	} else {
	}

	ret = mystrcmp(str, str3);
	if(0 == ret){
		printf("%s and %s is the same\n", str,str3);
	} else if (1 == ret) {
		printf("%s big than %s \n", str,str3);
	}else if (-1 == ret) {
		printf("%s less than %s \n", str,str3);
	} else {
	}

	return;
}


void testisPalindromeStr(void){
	char * str1 = "Feng is from Beijing!";
	char * str2 = "level level";
	bool flag;

	printf("\n************  testisPalindromeStr ************ \n");

	flag = isPalindromeStr(str1);
	if(flag){
		printf("%s is a Palindrome string \n", str1);
	} else {
		printf("%s is not a Palindrome string \n", str1);
	}

	flag = isPalindromeStr(str2);
	if(flag){
		printf("%s is a Palindrome string \n", str2);
	} else {
		printf("%s is not a Palindrome string \n", str2);
	}


}


void testReverse(void){
	char str[100] = "Feng is from Beijing!";
	char sub[100] = "from";
	char * head = NULL, * tail = NULL;
	int len  = 0;

	printf("\n************  testReverse ************ \n");

	strReversWithWord(str);
	printf("Reverse string is : %s\n\n", str);

	/*在反转回来*/
	strReversWithWord(str);
	printf("Reverse string is : %s\n\n", str);


	strReversWithoutWord(str);
	printf("Reverse string without word is : %s\n\n", str);

	/*在反转回来*/
	strReversWithoutWord(str);
	printf("Reverse string without word is : %s\n\n", str);

	
	strReversWithoutSubstr(str, sub);
	printf("Reverse string without substr is : %s\n\n", str);


	return;

}

void testmystrstr(void){
	char * str1 = "12345";
	char * str2 = "45";
	char * str3 = "23";
	char * str4 = "25";
	char * r = NULL;

	printf("\n************  testmystrstr ************ \n");

	r = mystrstr(str1, str2);
	if(NULL != r){
		printf("substr: %s\n", r);
	} else {
		printf("can not find sub: %s\n", str2);
	}
	

	r = mystrstr(str1, str3);
	if(NULL != r){
		printf("substr: %s\n", r);
	} else {
		printf("can not find sub: %s\n", str3);
	}

	r = mystrstr(str1, str4);
	if(NULL != r){
		printf("substr: %s\n", r);
	} else {
		printf("can not find sub: %s\n", str4);
	}	

	return;
}



void testmystrlen(void){
	int len = 0;
	char * src = "Hello World!";

	printf("\n************  testmystrlen ************ \n");
	len = mystrlen(src);
	printf("len = %d\n", len);

	return;

}


void testmymemcpy(void){
	char dst[100] = {0};
	char * src = "Feng is the best !";

	printf("\n************  testmymemcpy ************ \n");
	mymemcpy(dst, src, 6);
	printf("dst = %s\n", dst);

	return;

}


void testmystrcpy(void){
	char dst[100] = {0};
	char * src = "Feng is the best !";

	printf("\n************  testmystrcpy ************ \n");
	mystrcpy(dst, src);
	printf("dst = %s\n", dst);

	memset(dst, 0x0, 100);
	mystrcpy(dst, "Hello Feng!");
	printf("dst = %s\n", dst);

	return;

}


void testlibstr2num(void){
	char dst[100] = {0};
	int num = 0;
	float num2 = 0;

	printf("\n************  testlibstr2num ************ \n");

	strcpy(dst, "1234");
	libstr2int(dst, &num);
	printf("num = %d\n", num);

	num = 0;
	memset(dst, 0x0, 100);
	strcpy(dst, "-1234");
	libstr2int(dst, &num);
	printf("num = %d\n", num);


	strcpy(dst, "436.55");
	libstr2float(dst, &num2);
	printf("num = %f\n", num2);

	num = 0;
	memset(dst, 0x0, 100);
	strcpy(dst, "-436.55");
	libstr2float(dst, &num2);
	printf("num = %f\n", num2);

	return;

}


void teststr2int(void){
	char dst[100] = {0};
	int num = 0;

	printf("\n************  teststr2int ************ \n");

	strcpy(dst, "12345678");
	str2int(dst, &num);
	printf("num = %d\n", num);

	num = 0;
	memset(dst, 0x0, 100);
	strcpy(dst, "-1234");
	str2int(dst, &num);
	printf("num = %d\n", num);

	return;
}


void testint2str(void){
	char dst[100] = {0};

	printf("\n************  testint2str ************ \n");

	int2str(dst, 1234);
	printf("num = %s\n", dst);

	memset(dst, 0x0, 100);
	int2str(dst, -1234);
	printf("num = %s\n", dst);
	
	return;
}

void testNum2Str(void){
	int num = -50;
	int sig = 8;
	float num2 = -50.783657821234;
	char dst[100] = {0};

	printf("\n************  testNum2Str ************ \n");

	//itoa(num, dst, 10);
	//Linux 没有 itoa
	
	sprintf(dst, "%d", num);
	printf("num = %s\n", dst);

	memset(dst, 0x0, 100);
	//gcvt(num2, sig, dst);
	sprintf(dst, "%.6f", num2);
	printf("num2 = %s\n", dst);

	
	return;
}



