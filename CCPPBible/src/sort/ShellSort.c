#include <stdio.h>
#include <stdlib.h>


#define SUCCESS		0
#define PARAM_ERR	-1

int ShellSort(int * array, int size){
	if(NULL == array){
		printf("%s para error", __func__);
		return PARAM_ERR;
	}

	int i = 0, j =0;   /*插入排序使用*/ 
	int m = 0, n = 0;  /*按dist分组使用*/
	int insert = 0;
	int dist = 0; /*希尔步距*/
#ifdef DEBUG
	int k = 0; /*debug 输出*/
#endif

	/*
	 * 注意：分组是以D为步长，跳着分组的
	 * 【0，D-1，2D-1, 3D-1】 为一组
	 *  不是连续的【0~D】为一组！
     */

	for(dist = size/2; dist > 0; dist--){ /*步距逐步减小*/		
#ifdef DEBUG		
		printf("\n ========  distance = %d =============\n", dist);
#endif		
		for(m =0; m < dist; m++) { //当前dist下，分别插入排序 dist -1 组

			/*对每组进行插入排序
			 * i 以后是无序区，步长为dist
		     */
			for(i = m; i < size ; i = i + dist) {
				insert = array[i];

				/*i前面是有序区, 没有找到合适的位置，向后移动步长dist*/
				for(j = i - dist; j >= 0; j = j - dist){
					if(array[j] >= insert){
						array[j + dist] = array[j];
					} else {
						break;
					}
				}
				
				/*插入到合适的位置*/
				array[j + dist] = insert;					
			}
#ifdef DEBUG			
			printf("\n ------ another group -----------\n");
			printf("[");
			for(k =m; k < size; k = k + dist){
				printf("  %d  ", array[k]);
			}				
			printf("]\n");
			printf("\n");
#endif			
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
	

	ShellSort(array, 10);

	printf("after sort: \n");
	for(i = 0; i < 10; i++){
		printf("  %d  ", array[i]);
	}
	printf("\n");
	
	return 0;
}



