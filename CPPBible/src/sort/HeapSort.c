#include <stdio.h>
#include <stdlib.h>


#define SUCCESS		0
#define PARAM_ERR	-1

int BuildMaxHeap(int * array, int low, int high){
	if(NULL == array){
		printf("%s para error", __func__);
		return PARAM_ERR;
	}

//	printf("Enter BuildMaxHeap low = %d, high = %d\n", low, high);
	
	int left = 0, right = 0; 	/*子树的索引*/
	int max = low; 				/*最大子节点的索引，默认[low]是堆的根 */
	int temp = 0;

	/* 
	 * low 是根堆，默认[low] 是最大的点做在的位置
	 * 不用 2 * low 和 2 * low + 1, 是因为对于0为根的对来说，子树就都在右边了，树就不平生了
	 * left = 2 * low;
	 * right = 2 * low + 1;	 
	 * 当然，这么用也没什么错误，就是树不太平衡，像个瘸子 ;-D
	 */
	left = 2 * low + 1;
	right = 2 * low + 2;

	/*
	 * 有左子节点 且 [low] 不是最大节点，max取得左子树
	 * 注意比较前，需要保证左子节点存在：left <= high
	 */
	if((left <= high) && (array[left] > array[low])){
		max = left;
	} else {
		max = low;
	}

	/*
	 * 有右子节点 且 [low] 不是最大节点，max取得左子树
	 * 注意比较前，需要保证左子节点存在：right <= high
	 */
	if((right <= high) && (array[right] > array[max])){
		max = right;
	}

	if(max != low){
		temp = array[max];
		array[max] = array[low];
		array[low] = temp;

		/* 对交换的子树进行重新建堆*/
		BuildMaxHeap(array, max, high);		
	}

//	printf("Left BuildMaxHeap low = %d, high = %d\n", low, high);
	
	return SUCCESS;

}

int initMaxHeap(int * array, int size){
	if(NULL == array){
		printf("%s para error", __func__);
		return PARAM_ERR;
	}

//	printf("Enter initMaxHeap\n");
	
	int i = 0;
#ifdef DEBUG
	int k = 0;
#endif	

	/*
	 * 只是到 size /2 而不是 size，因为后半部分通过 2*i +1  和  2 * i + 2 子树方式完成了递归的构建
	 * 前半部分在初始化的时候，要逐个建堆
	 * for(i = size - 1; i >= 0; i--) 整个建堆也没有错，就是效率会差一些，随着数组越大，效率越差
	 * 注意要包含 0 和 size / 2 ， 是个闭区间，不然少一个初次的建堆
	 * 还有一点非常重要，需要从后向前初始化，相当于需要先初始化好子树，再初始化父一级的树，这样才可以
	 * 反过来的初始化顺序是错误的，父一级错误，再初始化子一级也错误，二者就更加错误了
     */
	for(i = size / 2; i >= 0; i--){
		BuildMaxHeap(array, i, size - 1);
	}
	
#ifdef DEBUG		
	printf("%s: size = %d\n", __func__, size);				
	printf("init Heap [");
	for(k =0; k < size; k++){
		printf("  %d  ", array[k]);
	}
	printf("]  \n");
	printf("\n");
#endif


//	printf("Left initMaxHeap\n");
	
	return SUCCESS;
}

int HeapSort(int * array, int size){
	if(NULL == array){
		printf("%s para error", __func__);
		return PARAM_ERR;
	}

	int i = 0, j = 0;
	int temp = 0;
#ifdef DEBUG
		int k = 0;
#endif

//	printf("Enter HeapSort\n");
	
	/*初始化构建大根堆*/
	initMaxHeap(array, size);

	/* 每轮提取构建有序区，然后对新的无序区的堆再次平衡 */
	for(i = size - 1 ; i > 0; i--){ /* 无序区逐步往前缩减，最后一个直接认为在有序区 */
		/* 交换 [0] 和 [i], i 进入有序区*/
		temp = array[0];
		array[0] = array[i];
		array[i] = temp;		

#ifdef DEBUG		
		printf("heapsize = %d, [max] = %d, 已经swap([0], [%d]) \n", i + 1, temp, i);				
		printf("[");
		/*有序区域*/
		for(k =0; k <= i; k++){
			printf("  %d  ", array[k]);
		}
		printf("]  , ");
		
		/*无序区域*/
		printf(" [");
			for(k = i + 1; k < size; k++){
				printf("  %d  ", array[k]);
		}
		printf("]\n");
		printf("\n");
#endif

		/* 构建堆 [0, i-1] */
		BuildMaxHeap(array, 0, i - 1);		
	}		

//	printf("Left HeapSort");
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
	

	HeapSort(array, 10);

	printf("after sort: \n");
	for(i = 0; i < 10; i++){
		printf("  %d  ", array[i]);
	}
	printf("\n");
	
	return 0;
}




