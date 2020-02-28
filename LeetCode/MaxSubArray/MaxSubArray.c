#include <stdio.h>
#include <assert.h>

/*
 * 题目描述

 给定一个整数数组 nums ，找到一个具有最大和的连续子数组（子数组最少包含一个元素），
 返回其最大和。
 
 示例:
 
 输入: [-2,1,-3,4,-1,2,1,-5,4],
 输出: 6
 解释: 连续子数组 [4,-1,2,1] 的和最大，为 6。

 进阶:
 
 如果你已经实现复杂度为 O(n) 的解法，尝试使用更为精妙的分治法求解


 gcc MaxSubArray.c -g -o a.exe


 */

int maxSubArray(int* nums, int numsSize){
 	if(NULL == nums){
		assert(0);
	}

	int i = 0, j = 0;
	int max = ~0;
	int sum = 0;


	max = nums[0];
	for(i = 0; i < numsSize; i++){
		sum = 0;
		for(j = i; j < numsSize; j++){
			sum += nums[j];
			if(sum > max){
				max = sum;
			}
		}
	}

	return max;
 	
}



void testmaxSubArray(void){

 	 int nums1[9] = {-2,1,-3,4,-1,2,1,-5,4};
	 int nums2[1] = {-1};
	 int nums3[3] = {-2147483647};
	 int max = 0;
	 
	 printf("\n************  testmaxSubArray ************ \n");

	 max = maxSubArray(nums1, 9);

	 printf("testmaxSubArray max  = %d\n", max);

	 max = maxSubArray(nums2, 1);

	 printf("testmaxSubArray max  = %d\n", max);

	 max = maxSubArray(nums3, 1);

	 printf("testmaxSubArray max  = %d\n", max);	 
	  
	 return; 
 
 }


 int main(int argc, char ** argv){
	testmaxSubArray();
 }




