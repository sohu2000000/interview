
/*
 * 需求
 
 一个整型数组 nums 里除两个数字之外，其他数字都出现了两次。请写程序找出这两个只出现一次的数字。要求时间复杂度是O(n)，空间复杂度是O(1)。
    
 示例 1： 
 输入：nums = [4,1,4,6]
 输出：[1,6] 或 [6,1]

 示例 2： 
 输入：nums = [1,2,10,4,1,4,3,3]
 输出：[2,10] 或 [10,2]
  
 
 限制： 
 2 <= nums <= 10000
     
 gcc singleNumbers-I.c -g -o a.exe -DDEBUG

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



/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* singleNumbers(int* nums, int numsSize, int* returnSize){

	if(NULL == nums || 0 == numsSize){
		*returnSize = 0;
		return NULL;
	}

	int * retp = NULL;
	int featureCode = 0, seperator = 0, i = 0, x = 0, y = 0;

	retp = (int *) malloc (2 * sizeof(int));
	if(NULL == retp){
		*returnSize = 0;
		return NULL;		
	}
	memset(retp, 0, 2 * sizeof(int));
	*returnSize = 2;

	/*取得2个数得特征值，出现两次的数字通过^排除掉，所以特征值里面只有2个出现一次的数字*/
	for(i = 0; i < numsSize; i++){
		featureCode = featureCode ^ nums[i];
	}

	/*取得最右侧的1的特征值，只会含有一个数字（不同为1），作为1个的特征值*/
	seperator = featureCode & (-featureCode);

	/*提取第一个数字*/
	for(i = 0; i < numsSize; i++){
		/*出现2次的数字被^去掉了*/
		if(0 != (nums[i] & seperator)){
			x = x ^ nums[i];
		}
	}

	/*通过^，把X从featureCode里面去掉，这剩下y*/
	y = featureCode ^ x;

	retp[0] = x;
	retp[1] = y;
	

	return retp;
	
}


void testsingleNumbers(void){
	
	printf("\n************  testsingleNumbers ************ \n");
	int * retp = NULL;
	int returnSize = 0;

	int nums1[4] = {4,1,4,6};
	retp = singleNumbers(nums1, 4, &returnSize);
	printf("returnSize = %d, retp[0] = %d, retp[1] = %d\n", returnSize, retp[0], retp[1]);
	if(NULL !=  retp){
		free(retp);
		retp = NULL;
	}

	int nums2[8] = {1,2,10,4,1,4,3,3};
	retp = singleNumbers(nums2, 8, &returnSize);
	printf("returnSize = %d, retp[0] = %d, retp[1] = %d\n", returnSize, retp[0], retp[1]);
	if(NULL !=  retp){
		free(retp);
		retp = NULL;
	}
	
	return; 
 
 }


 int main(int argc, char ** argv){
	testsingleNumbers();
 }






