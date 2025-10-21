#include <stdio.h>
#include <stdlib.h>


#define SUCCESS		0
#define PARAM_ERR	-1

int BubbleSort(int * array, int size){
	if(NULL == array){
		printf("%s para error", __func__);
		return PARAM_ERR;
	}

	int i = 0, j = 0;
	int temp = 0;
	int xch = 0;
#ifdef DEBUG
	int k = 0;
#endif

	for(i = 0; i < size; i++){ /*逐步扩大有序区域*/

		xch = 0; /*记录是否发生过交换*/
		
		for(j = i + 1; j < size; j++){ /*逐个比较交换无序区域*/
			/* 升序，小的上浮 */
			if(array[j] < array[i]){
				temp = array[i];
				array[i] = array[j];
				array[j] = temp;
				xch = 1;
			}
		}

#ifdef DEBUG		
		printf("[");
		/*有序区域*/
		for(k =0; k < i; k++){
			printf("  %d  ", array[k]);
		}
		printf("], ");

		printf(" %d, ",array[i]);
			
		/*无序区域*/
		printf("[");
		for(k =i+1; k < size; k++){
			printf("  %d  ", array[k]);
		}
		printf("]\n");
		printf("\n");
#endif
		/*无序区域完全没有发生交换，说明已经完全排序好了，退出*/
		if(1 != xch){
			break;
		}

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
	

	BubbleSort(array, 10);

	printf("after sort: \n");
	for(i = 0; i < 10; i++){
		printf("  %d  ", array[i]);
	}
	printf("\n");
	
	return 0;
}



