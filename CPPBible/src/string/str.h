#ifndef __STR_H__
#define __STR_H__

#include <stdbool.h>

#define SUCCESS		0
#define PARAM_ERR	-1
#define ALLOC_ERR   -2


int int2str(char * str, int num);
int str2int(char * str, int * rst);
int libstr2int(char * str, int * rst);
int libstr2float(char * str, float * rst);
int mystrcpy(char * dst, char * src);
void * mymemcpy(void * dst, void * src, int size);
int mystrlen(char * str);
char * mystrstr(char * str, char * sub);
int strReverse(char * head, char * tail);
int strReversWithoutWord (char * str);
int strReversWithWord (char * str);
int strReversWithoutSubstr (char * str, char * substr);
bool isPalindromeStr(char * str);
int mystrcmp(char * str1, char * str2);
char * commonStr(const char * str1, const char * str2);
int int2HexStr(char * str, int num);
int int2BinStr(char * str, int num);
int statStr(char * str);
int encodeStr(char * str);
int addBigInt(char * str1, char * str2, char * rst);
int rightShiftStr(char * str, int n);
int removeSubString(char * str, int index, int size);
int sortHalfStr(char * str);
int strRemoveSpecChar(char * str, const char c);
char * mystrcat(char * str1, char * str2);
int gbkstrlen(char * str);
int getStrMaxSuccNum(char * str, int * pnum0, int * pnum1);
int strReplaceAll(char * str, char * sub, char * replace);



void testNum2Str(void);
void testint2str(void);
void teststr2int(void);
void testlibstr2num(void);
void testmystrcpy(void);
void testmymemcpy(void);
void testmystrlen(void);
void testmystrstr(void);
void testReverse(void);
void testisPalindromeStr(void);
void testmystrcmp(void);
void testcommonStr(void);
void testConvertNum2Str(void);
void teststatStr(void);
void testencodeStr(void);
void testaddBigInt(void);
void testrightShiftStr(void);
void testremoveSubString(void);
void testsortHalfStr(void);
void teststrRemoveSpecChar(void);
void testmystrcat(void);
void testgbkstrlen(void);
void testgetStrMaxSuccNum(void);
void teststrReplaceAll(void);




#endif
