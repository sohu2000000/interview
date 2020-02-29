#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

/*
 * 题目描述
 
 给定一个正整数 n，生成一个包含 1 到 n2 所有元素，且元素按顺时针顺序螺旋排列的正方形矩阵。
 
 示例:
 
 输入: 3
 输出:
 [
  [ 1, 2, 3 ],
  [ 8, 9, 4 ],
  [ 7, 6, 5 ]
 ]
 
 来源：力扣（LeetCode）
 链接：https://leetcode-cn.com/problems/spiral-matrix-ii
 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。


 gcc GenerateMatrix.c -g -o a.exe
 gcc GenerateMatrix.c -g -o a.exe -DDEBUG

 */

#define DIR_RIGHT 		0
#define DIR_DOWN		1
#define DIR_LEFT		2
#define DIR_UP			3
#define DIR_MAX			4


/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */
int** generateMatrix(int n, int* returnSize, int** returnColumnSizes){

	int ** matrix = NULL;	
	int direct = DIR_RIGHT; 	/*最开始向右走*/
	int row = n;				/*初始行长度*/
	int col = n;				/*初始列长度*/
	int row_start = 0;			/*从0,0出发*/
	int row_end = row - 1;		/*开始是一整行*/
	int col_start = 0;			/*从0,0出发*/
	int col_end = col - 1;		/*开始是一整列*/
	int i = 0, j = 0;
	int c = 0, total = row * col;
	int gen = 1; 				/*产生的数字从1开始*/

	if(0 == n){
		*returnSize = 0;
		*returnColumnSizes = NULL;
		return NULL;
	}
	
	/*为二维指针分配二维数组格式内存*/
	matrix = (int **)malloc(n * sizeof(int *));
	for(i = 0; i < n; i++){
		matrix[i] = (int *)malloc(n * sizeof(int));
	}

	/*为二维指针分配一维数组格式内存(连续的)*/
	*returnColumnSizes = (int *)malloc(n * sizeof(int));	
	for(i = 0; i < n; i++){
		*(*returnColumnSizes + i) = n;
	}
	
	while(gen <= total){

		direct = direct % DIR_MAX;
		
		if(DIR_RIGHT == direct){
			i = row_start;
			for(j = col_start; j <= col_end; j++){				
#ifdef DEBUG				
				printf("matrix[%d][%d] = %d\n", i,j, gen);
#endif
				matrix[i][j] = gen++;
			}
			/*变换方向*/
			direct++;
			/*打印完一行，行起始变大*/
			row_start++;
		} else if (DIR_DOWN == direct){
			j = col_end;
			for(i = row_start; i <= row_end; i++){
#ifdef DEBUG				
				printf("matrix[%d][%d] = %d\n", i,j, gen);
#endif

				matrix[i][j] = gen++;
			}
			/*变换方向*/
			direct++;
			/*打印完一列，列结尾变小*/
			col_end--;
		} else if (DIR_LEFT == direct){
			i = row_end;
			for(j = col_end; j >= col_start; j--){
#ifdef DEBUG				
				printf("matrix[%d][%d] = %d\n", i,j, gen);
#endif
				matrix[i][j] = gen++;
			}
			/*变换方向*/
			direct++;
			/*打印完一行，行结尾变小*/
			row_end--;
		} else if (DIR_UP == direct){
			j = col_start;
			for(i = row_end; i >= row_start; i--){
#ifdef DEBUG				
				printf("matrix[%d][%d] = %d\n", i,j, gen);
#endif
				matrix[i][j] = gen++;
			}
			/*变换方向*/
			direct++;
			/*打印完一列，列起始变大*/
			col_start++;
		} else {
			
		}
	}
	

	*returnSize = n;
	return matrix;
}


void testgenerateMatrix(void){

	int returnSize = 0;
	int * returnColumnSizes = NULL;
	int ** matrix = NULL;
	int n = 0, i = 0, j = 0;
	
	printf("\n************  testgenerateMatrix ************ \n");

	/*testcase 1*/
	n = 3;
	matrix = generateMatrix(n, &returnSize, &returnColumnSizes);
	if(NULL != matrix){
		printf("\t returnSize = %d\n", returnSize);
		printf(" [ ");
		for(i = 0; i < n; i++){
			for(j = 0; j < n; j++){
				printf(" %d ", matrix[i][j]);
			}
		}
		printf(" ]\n\n");
		
		printf("\t returnColumnSizes = %p\n", returnColumnSizes);
		printf(" [ ");
		for(i = 0; i < n; i++){
			printf(" %d ", returnColumnSizes[i]);
		}
		printf(" ]\n\n");
		
		free(matrix);
		matrix = NULL;
		free(returnColumnSizes);
		returnColumnSizes = NULL;		
	}

	/*testcase 2*/
	n = 0;
	matrix = generateMatrix(n, &returnSize, &returnColumnSizes);
	if(NULL != matrix){
		printf("\t returnSize = %d\n", returnSize);
		printf(" [ ");
		for(i = 0; i < n; i++){
			for(j = 0; j < n; j++){
				printf(" %d ", matrix[i][j]);
			}
		}
		printf(" ]\n\n");
		
		printf("\t returnColumnSizes = %p\n", returnColumnSizes);
		printf(" [ ");
		for(i = 0; i < n; i++){
			printf(" %d ", returnColumnSizes[i]);
		}
		printf(" ]\n\n");
		
		free(matrix);
		matrix = NULL;
		free(returnColumnSizes);
		returnColumnSizes = NULL;		
	}


	return; 
 
 }


 int main(int argc, char ** argv){
	testgenerateMatrix();
 }




