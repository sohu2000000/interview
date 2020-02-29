
/*
 * 需求

 给定一个整数数组 nums，其中恰好有两个元素只出现一次，其余所有元素均出现两次。 找出只出现一次的那两个元素。
 
 示例 :
 
 输入: [1,2,1,3,2,5]
 输出: [3,5]
 
 gcc SingleNumber.c -g -o a.exe -DDEBUG

 */
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* singleNumber(int* nums, int numsSize, int* returnSize){

	if(NULL == nums || numsSize == 0){
		printf("singleNumber param error\n");
		*returnSize = 0;
		return NULL;
	}

	int * p = NULL;
	int i = 0, j = 0, k = 0;
	int found = 0;

	p = (int *)malloc(2 * sizeof(int));
	if(NULL == p){
		printf("singleNumber alloc error\n");
		*returnSize = 0;
		return NULL;
	}


	for(i = 0; i < numsSize; i++){
		found = 0;
		for(j = 0; j < numsSize; j++){
			if(nums[i] == nums[j] && i != j){
				found = 1;
				break;
			}
		}
		if(0 == found){
			p[k++] = nums[i];
		}

		if(2 == k){
			goto out;
		}		
	}

out:
	*returnSize = k;
	return p;
}


int* singleNumber2(int* nums, int numsSize, int* returnSize){
	if(NULL == nums || numsSize == 0){
		printf("singleNumber param error\n");
		*returnSize = 0;
		return NULL;
	}

	int * p = NULL;
	int i = 0, k = 0;
	int bitmask = 0;
	int diff = 0;
	int x = 0;
		
	p = (int *)malloc(2 * sizeof(int));
	if(NULL == p){
		printf("singleNumber alloc error\n");
		*returnSize = 0;
		return NULL;
	}


	/*
	 * 出现两次的数，被从bitmask中去掉了，出现一次的保存到了bitmask中
	 * 其实bitmask是数字的特征值，出现两个数字的会通过异或把特征值去除，
	 * 剩下的就是单独出现的两个数字的特征值。
	 * 如果单独出现的两个数字在某位上都是1的怎么处理呢？那不是情况了吗？
	 * 这就对了，相同为的特征值被抵消掉了，但一定会有不同的为，有特征值被保留下来，
	 * 否则就成相同的两个数字了
	 */
	for(i = 0; i < numsSize; i++){
		bitmask ^= nums[i];
	}

	/*取得最右侧的1，到diff中*/
	diff = bitmask & (-bitmask);
	/* 
	 * 这是的diff只有第一个出现一次的数 X 对应的bit位置
	 * 但是可能有很多的数，在改bit位上为1;
	 * 但是 x 通过不断的异或，把其他出现两次的数字去掉，
	 * 最后就剩下了只出现一次的且该位为1的 X
	 */
	for(i = 0; i < numsSize; i++){
		if(0 != (diff & nums[i])){
			x ^= nums[i];
		}
	}
	
	p[k++] = x;

	/*从bitmask中分离出x，就剩下了y*/
	p[k++] = bitmask ^ x;
	
out:
	*returnSize = k;
	return p;
}



void testsingleNumber(void){
	
	printf("\n************  testsingleNumber ************ \n");
	int nums[] = {1,2,1,3,2,5};
	int numsSize = sizeof(nums) / sizeof(int);
	int returnSize = 0;
	int * p= NULL;
	int i = 0;

	/*testcase 1*/
	p = singleNumber(nums, numsSize, &returnSize);
	if(NULL != p){
		printf("\t returnSize = %d \n",returnSize);
		printf("\t [ ");
		for(i = 0; i < returnSize; i++){
			printf(" %d ", p[i]);
		}
		printf(" ] \n");
		free(p);
		p = NULL;
	}

	/*testcase 2*/
	p = singleNumber2(nums, numsSize, &returnSize);
	if(NULL != p){
		printf("\t returnSize = %d \n",returnSize);
		printf("\t [ ");
		for(i = 0; i < returnSize; i++){
			printf(" %d ", p[i]);
		}
		printf(" ] \n");
		free(p);
		p = NULL;
	}
	
	return; 
 
 }


 int main(int argc, char ** argv){
	testsingleNumber();
 }






