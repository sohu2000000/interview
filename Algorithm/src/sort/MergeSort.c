#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define SUCCESS		0
#define PARAM_ERR	-1
#define ALLOC_ERR	-2

/*
 * 合并两个子数组 [lstart, lend] [rstart, rend]
 */
int MergeImpl(int * array, int * temp,  int lstart, int lend, int rstart, int rend){
	if(NULL == array || NULL == temp){
		printf("%s para error\n", __func__);
		return PARAM_ERR;
	}

#if 1
	int size = 0; /* 两个待合并的素组总长度 */
	int i = 0, j = 0, k = 0;

	size = (lend - lstart +  1) + (rend - rstart + 1);
	i = lstart;
	j = rstart;
	k = 0;
	
#ifdef DEBUG
	int m = 0;
	printf("Befor %s: size = %d \n", __func__, size);	
	for(m = lstart; m <= rend ; m++){
		printf("array[%d] = %d ", m, array[m]); 	
	}
	printf("\n");
#endif

	/*清空待合并数组*/
	memset(temp, 0x0, size * sizeof(int));

	/* 升序 */
	while ( (i <= lend) && (j <= rend)){
		if(array[i] <= array[j]){
			temp[k++] = array[i++];
		} else {
			temp[k++] = array[j++];
		}
	}

	/* 左子组还有 */
	while (i <= lend){
		temp[k++] = array[i++];
	}
	
	/* 右子组还有 */
	while (j <= rend){
		temp[k++] = array[j++];
	}	

	/*
	 * 合并后的数组，复制回array 
	 * 因为左右两个子组是相邻的，所以从lstart顺序复制如array
	 */
	i = lstart;
	for(k = 0; k < size; k++){
		array[i++] = temp[k];		
	}

#ifdef DEBUG
	printf("After: %s \n", __func__);
	for(m = 0; m < size ; m++){
		printf("temp[%d] = %d ", m, temp[m]);		
	}
	printf("\n");

	for(m = lstart; m <= rend ; m++){
		printf("array[%d] = %d ", m, array[m]);		
	}

	printf("\n\n");
#endif


#else
	int size = 0; /* 两个待合并的素组总长度 */
	int lpos = 0, rpos = 0, tpos = 0, k = 0;

	size = rend - lstart + 1;
	lpos = lstart;
	rpos = rstart;
	tpos = lstart;

	/* 升序 */
	while ( (lpos <= lend) && (rpos <= rend)){
		if(array[lpos] <= array[rpos]){
			temp[tpos++] = array[lpos++];
		} else {
			temp[tpos++] = array[rpos++];
		}
	}


	/* 左子组还有 */
	while (lpos <= lend){
		temp[tpos++] = array[lpos++];
	}
	
	/* 右子组还有 */
	while (rpos <= rend){
		temp[tpos++] = array[rpos++];
	}	

	/*
	 * 合并后的数组，复制回array 
	 * 因为左右两个子组是相邻的，所以从lstart顺序复制如array
	 */
	for(k = 0; k < size; k++, rend--){
		array[rend] = temp[rend];	
	}

#endif
	return SUCCESS;
	
}

/*
 * 本函数真正的合并排序实现
 */
int MergeSortImpl(int * array, int * temp,  int low, int high){
	if(NULL == array || NULL == temp){
		printf("%s para error\n", __func__);
		return PARAM_ERR;
	}

	int i = 0, j = 0;
	int size = 0; /* 排序分组大小 */
	int mid = 0;
	
	size =  high - low + 1;
	/* 递归终止条件，数组中只有一个元素*/
	if(size <= 1){
		goto out;		
	}

	/*
	 * 两路合并排序，自定向下，取得中间点 
	 * 注意 low 和 high 的中间的不是 size /2 而是 (low + high) / 2 ！ 千万注意
	 */
	mid = (low + high) / 2; /*左子组：[low, mid], 右子组：[mid+1, high]*/
	
	/* 对左子组两路合并排序*/
	MergeSortImpl(array, temp, low, mid);

	/* 对右子组两路合并排序*/
	MergeSortImpl(array, temp, mid + 1 , high);

	/* 对已排序的两个子组进行合并 */
	MergeImpl(array, temp, low, mid, mid + 1, high);

out:
	return SUCCESS;
}


int MergeSort(int * array, int size){
	if(NULL == array){
		printf("%s para error\n", __func__);
		return PARAM_ERR;
	}

	int * temp = NULL;

	/*这里是合并排序的接口，完成准备工作*/
	temp = (int *) malloc (size * sizeof(int));
	if(NULL == temp){
		printf("%s alloc memory error\n",__func__);
		return ALLOC_ERR;
	}

	MergeSortImpl(array, temp, 0, size - 1);

	/* 删除已分配空间 */
	free(temp);
	temp = NULL;

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

	MergeSort(array, 10);

	printf("after sort: \n");
	for(i = 0; i < 10; i++){
		printf("  %d  ", array[i]);
	}
	printf("\n");
	
	return 0;
}





