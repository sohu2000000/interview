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
	int max = ~0; /* 注意，没有说nums是正数，所以不能初始化为0 */
	int sum = 0;

	/* max 默认最小就是数组的第一个元素作为初始化值，
	 * 其他的值都不合适，因为后面的sum都可能小于任何值，所以用第一个元素作为初始值
	 * 特别针对只有一个元素且是很小负数的数组
	 */
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


#define MAX(x,y) (x) > (y) ? (x) : (y)

int getSubMaxSum(int* nums, int left, int right){

	int leftMax = 0, rightMax = 0, cross = 0;
	int leftsum = 0, rightsum = 0, sum = 0;
	int max = ~0;
	int mid = (left + right) / 2;
	int i = 0;
	
	/*递归结束条件*/
	if(left == right){
		return nums[left];
	}

	/* 左半边的子数组最大和*/
	leftMax = getSubMaxSum(nums, left, mid);

	/* 右半边的子数组最大和*/
	rightMax = getSubMaxSum(nums, mid + 1, right);

	/*
	 * 当前数组的子数组最大和
	 * 注意因为是连续的子数组，所以从中间向两边发散，
	 * 可以得到从中间向两边发散的连续子数组
	 * 然后左右两边max相加，得到子数组最大值
	 * 从中间想两边发散寻找子数组是非常巧妙的方式
	 */
	leftsum = nums[mid];
	sum = 0;
	for(i = mid; i >= left; i--){
		sum += nums[i];
		leftsum = MAX(leftsum, sum);
	}
	
	rightsum = nums[mid+1];
	sum = 0;
	for(i = mid + 1; i <= right; i++){
		sum += nums[i];
		rightsum = MAX(rightsum, sum);
	}

	cross = leftsum + rightsum;

	max = MAX(leftMax, rightMax);
	max = MAX(max, cross);

	return max;

}


int maxSubArray2(int* nums, int numsSize){
	int max = getSubMaxSum(nums, 0, numsSize - 1);

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


	 max = maxSubArray2(nums1, 9);

	 printf("testmaxSubArray max  = %d\n", max);

	 max = maxSubArray2(nums2, 1);

	 printf("testmaxSubArray max  = %d\n", max);

	 max = maxSubArray2(nums3, 1);

	 printf("testmaxSubArray max  = %d\n", max);	 

	  
	 return; 
 
 }


 int main(int argc, char ** argv){
	testmaxSubArray();
 }




