#include <stdio.h>
#include <stdlib.h>


#define SUCCESS		0
#define PARAM_ERR	-1

int getMinPoS(int * array, int low, int high){
	if(NULL == array || low > high){
		printf("%s para error", __func__);
		return PARAM_ERR;
	}	
	
	int i = 0;
	int minPoS = low;
	int min = array[low];

	/*千万注意这里是 <= high, 因为数组的最后一个是high (n-1)*/
	for(i = low; i <= high; i++){
		if(array[i] < min ){
			minPoS = i;
			min = array[i];
		}
	}

	return minPoS;
}

int SelectSort(int * array, int size){
	if(NULL == array){
		printf("%s para error", __func__);
		return PARAM_ERR;
	}

	int i = 0, j = 0;
	int minPoS = 0; /*当前无序区最小值的位置*/
	int temp = 0;
#ifdef DEBUG
		int k = 0;
#endif

	for(i = 0; i < size; i++){ /* 无序区域起始位置逐步向后 */
		
		/*无序区处理，此时 i 还属于无序区，无序区为 [i,.... n-1], 找到无序区最小位置minPoS*/
		/*升序*/
		minPoS = getMinPoS(array, i, size - 1);

		/* 交换最小值到i，通过i++, 将i并入有序区域 */
		if(minPoS != i){
			temp = array[minPoS];
			array[minPoS] = array[i];
			array[i] = temp;
		}

#ifdef DEBUG		
		printf("i = %d min = %d\n", i, temp);				
		printf("[");
		/*有序区域*/
		for(k =0; k < i+1; k++){
			printf("  %d  ", array[k]);
		}
		printf("] ");

		/*无序区域*/
		printf("[");
		for(k = i+1; k < size; k++){
			printf("  %d  ", array[k]);
		}
		printf("]\n");
		printf("\n");
#endif		
	}

	return SUCCESS;
	
}


int main(int argc, char ** argv){
	int array[10] = {7,3,5,8,0,9,1,2,4,6};
	int i = 0;

	printf("Before sort: \n");
	for(i = 0; i < 10; i++){
		printf("  %d  ", array[i]);
	}
	printf("\n");
	

	SelectSort(array, 10);

	printf("after sort: \n");
	for(i = 0; i < 10; i++){
		printf("  %d  ", array[i]);
	}
	printf("\n");
	
	return 0;
}



