#include <stdio.h>
#include <stdlib.h>


#define SUCCESS		0
#define PARAM_ERR	-1

#define BUCKET_NUM	10		/*桶个数*/
#define BUCKET_DEEP	100		/*捅深度*/

/* 
 * 每个桶的头，包含桶元素数组和桶中有效元素的个数
 * 桶元素这里假设只有100个元素，这个限制可以通过链表处理来突破
 */
typedef struct bucket{
	int elem[BUCKET_DEEP];	/*桶元素*/
	int count;				/* 桶元素中有效的个数*/
}st_bucket;


/*
 * 正常桶深应该用链表，这里进行整数的排序，关键字10个，为 0~9
 * 默认可以排序100个数，可以通过将桶元素变为链表来突破这个限制
 */
st_bucket g_buckets[BUCKET_NUM];

/*
 * 初始化 buckets，将所有的桶的有效count设置为0，表示没有有效元素
 */
void InitBuckets(void){
	int i, j;

	for(i = 0; i < BUCKET_NUM; i++){
		g_buckets[i].count = 0;
	}
	
	return;
}


/*
 * 返回array中最大的数
 */
int getMaxNum(int * array, int size){
	
	int i = 0;
	int max = 0;

	max = array[0];
	for(i = 0; i < size; i++){
		if(array[i] > max){
			max = array[i];
		}
	}

	return max;
}


/* 根据num的位数，取得排序的轮数*/
int getRnd(int num){
	int rnd = 0;

	do {
		rnd++;
		num = num / 10;
	} while (0 != num);

	return rnd;
}


int Scatter(int * array, int size, int digit){
	if(NULL == array){
		printf("%s para error\n", __func__);
		return PARAM_ERR;
	}

	int i = 0, num = 0, d = 0;
	int key = 0;  /*桶关键字*/

	for(i = 0; i < size; i++){
		/* 取得桶关键字 */
		num  = array[i];
		for (d = 1; d <= digit; d++){
			key = num % 10;
			num = num / 10;
		}

		/*将数字插入排序桶中*/
		g_buckets[key].elem[g_buckets[key].count++] = array[i];
	}

	return SUCCESS;
}


int gather(int * array, int size) {
	if(NULL == array){
		printf("%s para error\n", __func__);
		return PARAM_ERR;
	}

	int i = 0, j = 0, k = 0;
	int count = 0;

	/*升序*/
	for(i = 0; i < BUCKET_NUM; i++){
		count = g_buckets[i].count;
		for(j = 0; j < count; j++){
			array[k++] = g_buckets[i].elem[j];
		}
	}

	return SUCCESS;
}


int BucketSort(int * array, int size){
	if(NULL == array){
		printf("%s para error\n", __func__);
		return PARAM_ERR;
	}

	int digit = 0;
	int max = 0;
	int rnd = 0;
#ifdef DEBUG
	int i = 0, j = 0, k = 0, c = 0;
#endif

	/*初始化桶*/

	/* 取得最大的数字 */
	max = getMaxNum(array, size);

	/* 取得排序的轮数 */
	rnd = getRnd(max);

	for(digit = 1; digit <= rnd; digit++){
		/* 重新初始化桶*/
		InitBuckets();

		/* 分散到桶中 */
		Scatter(array, size, digit);

		/* 从桶中聚合 */
		gather(array, size);

#ifdef DEBUG
		printf("============= Digit %d =========== \n", digit);				
		printf(" Scattered Bucket:\n");
		/*有序区域*/
		for(i =0; i < BUCKET_NUM; i++){
			printf("\t Bucket[%d]: ", i);
			c = g_buckets[i].count;
			for(j = 0; j < c; j++){
				printf(" %d ", g_buckets[i].elem[j]);
			}
			printf("\n");
		}
		printf(" --------------------------------- \n ");

		/*无序区域*/
		printf("Gattered Array: \n");
		printf("\t");
		for(k =0; k < size; k++){
			printf("  %d  ", array[k]);
		}
		printf("\n");	
		printf("================================== \n\n");
#endif
		
	}

	return SUCCESS;
	
}


int main(int argc, char ** argv){
	int array[10] = {22,32,19,53,0,47,29,116,4,6};
	int i = 0;

	printf("Before sort: \n");
	for(i = 0; i < 10; i++){
		printf("  %d  ", array[i]);
	}
	printf("\n");
	
	BucketSort(array, 10);

	printf("After sort: \n");
	for(i = 0; i < 10; i++){
		printf("  %d  ", array[i]);
	}
	printf("\n");
	
	return 0;
}



