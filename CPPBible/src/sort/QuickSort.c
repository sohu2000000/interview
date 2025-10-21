#include <stdio.h>
#include <stdlib.h>


#define SUCCESS		0
#define PARAM_ERR	-1

int QuickSort(int * array, int low, int high){
	if(NULL == array){
		printf("%s para error", __func__);
		return PARAM_ERR;
	}

	int i = 0, j = 0;	/*左右下游标*/
	int pivot = 0; 		/*分治数*/
#ifdef DEBUG
		int k = 0;
#endif

	/*递归结束条件*/
	if(low >= high){
		goto out;
	}

	/*初始化*/
	pivot = array[low]; /* i = low 的值为默认分治值，保存到pivot中，此时i的位置已经空出*/
	i = low;
	j = high;

	/*
	 * 每次循环，左右对找一次，
	 * 保证本次第一个小于pivot的放左边，大于pivot的放右边
	 * 每次结束的时候，i位置空出
	 */
	while(i < j){
		
		/* 从后面找到第一个小于pivot的值，
		 * 回填到已经空出的i的位置，
		 * i位置填充后向后移动
		 * 此时 j 的位置相当于已经空出
		 */
		while(i < j && array[j] >= pivot) j--;
		if(i < j){ /* 只有游标正常的情况下发生填写交换，游标不正常说明找到了pivotpos，在while外处理填入pivot*/
			array[i] = array[j];
			i++;
		}

		/* 从前面找到第一个大于pivot的值，
		 * 回填到已经空出的j的位置，
		 * j位置填充后前后移动
		 * 此时 i 的位置相当于已经又空出
		 */
		while(i < j && array[i] <= pivot) i++;
		if(i < j){ /* 只有游标正常的情况下发生填写交换，游标不正常说明找到了pivotpos，在while外处理填入pivot*/
			array[j] = array[i];
			j--;
		}
	}

	/*最后的i位置，就是pivotpos*/
	array[i] = pivot;

#ifdef DEBUG
	printf("low = %d, high = %d, pivot = %d\n",low, high, pivot);
	printf("[");
	/*左边的小于区域*/
	for(k =low; k < i; k++){
		printf("  %d  ", array[k]);
	}
	printf("], ");

	/*pivot的值*/
	printf("  %d,  ",array[i]);
		
	/*右边的大于区域*/
	printf("[");
	for(k =i+1; k < high + 1; k++){
		printf("  %d  ", array[k]);
	}
	printf("]\n");
	printf("\n");

#endif

	/*左边分区递归*/
	QuickSort(array, low, i - 1);
	
	/*右边分区递归*/
	QuickSort(array, i + 1, high);

out:
	return SUCCESS;
	
}


int main(int argc, char ** argv){
	int array[10] = {54,38,96,23,0,15,72,60,45,83};
	int i = 0;

	printf("Before sort: \n");
	for(i = 0; i < 10; i++){
		printf("  %d  ", array[i]);
	}
	printf("\n");
	

	QuickSort(array, 0, 9);

	printf("after sort: \n");
	for(i = 0; i < 10; i++){
		printf("  %d  ", array[i]);
	}
	printf("\n");
	
	return 0;
}



