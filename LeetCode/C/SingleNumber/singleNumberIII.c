
/*
 * 需求


给定一个非空整数数组，除了某个元素只出现一次以外，其余每个元素均出现了三次。找出那个只出现了一次的元素。

说明：
你的算法应该具有线性时间复杂度。 你可以不使用额外空间来实现吗？

示例 1:
输入: [2,2,3,2]
输出: 3

示例 2:
输入: [0,1,0,1,0,1,99]
输出: 99


 gcc singleNumberIII.c -g -o a.exe -DDEBUG

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

int singleNumber(int* nums, int numsSize){
	int ones = 0; /*某位上出现1次1*/
	int twos = 0; /*某位上出现2次1*/
	int threes = 0; /*某位上出现3次1*/
	int i = 0;

	for(i = 0; i < numsSize; i++){
		/* 看ones中那些已经置1的位，在nums[i]里面又出现了，记录下来 */
		twos |= ones & nums[i];
	
		/*将nums[i]记录到ones里，出现了就应该记录*/
		ones = ones ^ nums[i];

		/*ones 和 twos 里面都出现的位，说明出现了3次*/
		threes = ones & twos;

		/*出现3次发生进位，ones和twos对应的位清零*/
		ones &= ~threes;
		twos &= ~threes;
	}

	/*返回只出现1次的数*/
	return ones;	
	
}


void testsingleNumber(void){
	
	printf("\n************  testsingleNumber ************ \n");
	int ret = 0;
	int numsSize = 0;

	int nums1[4] = {2,2,2,3};
	numsSize = 4;
	ret = singleNumber(nums1, numsSize);
	printf(" %d appear once \n", ret);

	int nums2[7] = {0,1,0,1,0,1,99};
	numsSize = 7;
	ret = singleNumber(nums2, numsSize);
	printf(" %d appear once \n", ret);
	
	return; 
 
 }


 int main(int argc, char ** argv){
	testsingleNumber();
 }











