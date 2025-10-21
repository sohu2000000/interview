#include <stdio.h>
#include <stdlib.h>


#define SUCCESS		0
#define PARAM_ERR	-1

int InsertSort(int * array, int size){
	if(NULL == array){
		printf("%s para error\n", __func__);
		return PARAM_ERR;
	}

	int i = 0, j = 0;
	int insert = 0;
	int loc = 0;
#ifdef DEBUG
		int k = 0;
#endif


	for(i = 0; i < size; i++){
		insert = array[i];
		/*升序*/

		/*insert 是最大的，不改变，进入下一轮插入排序*/
		if(insert > array[i-1]){
			goto info;
		}

		/*在有序区域查找合适的插入位置*/
		loc = i-1; //初始插入位置认为在最后一个上
		for(j = i - 1; j >= 0; j--){
			if(insert >= array[j]){
#ifdef DEBUG				
				printf("find loc insert %d > array[%d] = %d\n", insert, j, array[j]);
#endif
				break;
			}
			loc = j;
		}

		/*有序区从插入位置向后移动*/
		for(j = i; j > loc; j--){
			array[j] = array[j-1];
		}
		
		array[loc] = insert;

info:			
#ifdef DEBUG		
		printf("i = %d insert = %d, loc = %d\n", i, insert, loc);				
		printf("[");
		/*有序区域*/
		for(k =0; k < i+1; k++){
			printf("  %d  ", array[k]);
		}
		printf("] , ");

		/*无序区域*/
		printf(" [");
		for(k =i+1; k < size; k++){
			printf("  %d  ", array[k]);
		}
		printf("]\n");
		printf("\n");
#endif
		continue;
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
	

	InsertSort(array, 10);

	printf("After sort: \n");
	for(i = 0; i < 10; i++){
		printf("  %d  ", array[i]);
	}
	printf("\n");
	
	return 0;
}


