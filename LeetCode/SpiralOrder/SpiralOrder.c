/*
 * 需求

 给定一个包含 m x n 个元素的矩阵（m 行, n 列），请按照顺时针螺旋顺序，返回矩阵中的所有元素。
 
 示例 1:
 
 输入:
 [
  [ 1, 2, 3 ],
  [ 4, 5, 6 ],
  [ 7, 8, 9 ]
 ]
 输出: [1,2,3,6,9,8,7,4,5]
 示例 2:
 
 输入:
 [
   [1, 2, 3, 4],
   [5, 6, 7, 8],
   [9,10,11,12]
 ]
 输出: [1,2,3,4,8,12,11,10,9,5,6,7]

 gcc SpiralOrder.c -g -o a.exe

 */
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>


#define DIR_RIGHT 		0
#define DIR_DOWN		1
#define DIR_LEFT		2
#define DIR_UP			3
#define DIR_MAX			4



/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* spiralOrder(int** matrix, int matrixSize, int* matrixColSize, int* returnSize){

	if(0 == matrixSize){
		*returnSize = 0;
		return NULL;
	}

	int direct = DIR_RIGHT; 	/*最开始向右走*/
	int row = matrixSize;		/*初始行长度*/
	int col = *matrixColSize;	/*初始列长度*/
	int row_start = 0;			/*从0,0出发*/
	int row_end = row - 1;		/*开始是一整行*/
	int col_start = 0;			/*从0,0出发*/
	int col_end = col - 1;		/*开始是一整列*/
	int i = 0, j = 0;
	int c = 0, total = row * col;

	int * p = (int *) malloc(row * col * sizeof(int));
	if(NULL == p){
		printf("spiralOrder malloc error\n");
		return NULL;
	}

	while(c < total){

		direct = direct % DIR_MAX;
		
		if(DIR_RIGHT == direct){
			i = row_start;
			for(j = col_start; j <= col_end; j++){
#ifdef DEBUG				
				printf("p[%d] = matrix[%d][%d] = %d\n", c, i,j,matrix[i][j]);
#endif
				p[c++] = matrix[i][j];
			}
			/*变换方向*/
			direct++;
			/*打印完一行，行起始变大*/
			row_start++;
		} else if (DIR_DOWN == direct){
			j = col_end;
			for(i = row_start; i <= row_end; i++){
#ifdef DEBUG				
				printf("p[%d] = matrix[%d][%d] = %d\n", c, i,j,matrix[i][j]);
#endif

				p[c++] = matrix[i][j];
			}
			/*变换方向*/
			direct++;
			/*打印完一列，列结尾变小*/
			col_end--;
		} else if (DIR_LEFT == direct){
			i = row_end;
			for(j = col_end; j >= col_start; j--){
#ifdef DEBUG				
				printf("p[%d] = matrix[%d][%d] = %d\n", c, i,j,matrix[i][j]);
#endif
				p[c++] = matrix[i][j];
			}
			/*变换方向*/
			direct++;
			/*打印完一行，行结尾变小*/
			row_end--;
		} else if (DIR_UP == direct){
			j = col_start;
			for(i = row_end; i >= row_start; i--){
#ifdef DEBUG				
				printf("p[%d] = matrix[%d][%d] = %d\n", c, i,j,matrix[i][j]);
#endif
				p[c++] = matrix[i][j];
			}
			/*变换方向*/
			direct++;
			/*打印完一列，列起始变大*/
			col_start++;
		} else {
			
		}
	}
	
	*returnSize = c;
	return p;
}


void testspiralOrder(void){

 	int matrix1[3][3] = {{1,2,3},{4,5,6},{7,8,9}};
	int matrix2[3][4] = {{1,2,3,4},{5,6,7,8},{9,10,11,12}};
	int returnSize = 0;
	int matrixSize = 0;
	int matrixColSize = 0;
	int * a = NULL;
	int i = 0, j =0;
	int ** matrix = NULL;
	
	printf("\n************  testspiralOrder ************ \n");

	/*testcase 1*/
	matrixSize = 3;
	matrixColSize = 3;

	/*分配一个二维数组，记录到二维指针中*/
	matrix = (int **)malloc(matrixSize * sizeof(int *));
	for(i = 0; i < matrixSize; i++){
		matrix[i] = (int *)malloc(matrixColSize * sizeof(int));
	}
	/*用二维数组初始化二维数组*/
	for(i = 0; i < matrixSize; i++){
	   for(j = 0; j < matrixColSize; j++){
		   matrix[i][j] = matrix1[i][j];
	   }
	}	
	a = spiralOrder(matrix, matrixSize, &matrixColSize, &returnSize);
	printf("\t returnSize = %d\n", returnSize);
	printf("\t [");
	for(i = 0; i < returnSize; i++){
		printf(" %d ", a[i]);
	}
	printf(" ]\n\n");
	free(a);
	a = NULL;
	free(matrix);
	matrix = NULL;

	/*testcase 2*/
	matrixSize = 3;
	matrixColSize = 4;

	/*分配一个二维数组，记录到二维指针中*/
	matrix = (int **)malloc(matrixSize * sizeof(int *));
	for(i = 0; i < matrixSize; i++){
		matrix[i] = (int *)malloc(matrixColSize * sizeof(int));
	}
	/*用二维数组初始化二维数组*/
	for(i = 0; i < matrixSize; i++){
	   for(j = 0; j < matrixColSize; j++){
		   matrix[i][j] = matrix2[i][j];
	   }
	}	
	a = spiralOrder(matrix, matrixSize, &matrixColSize, &returnSize);
	printf("\t returnSize = %d\n", returnSize);
	printf("\t [");
	for(i = 0; i < returnSize; i++){
		printf(" %d ", a[i]);
	}
	printf(" ]\n\n");
	free(a);
	a = NULL;
	free(matrix);
	matrix = NULL;

	/*testcase 2*/
	matrixSize = 0;
	matrixColSize = 0;

	/*分配一个二维数组，记录到二维指针中*/
	matrix = (int **)malloc(matrixSize * sizeof(int *));
	for(i = 0; i < matrixSize; i++){
		matrix[i] = (int *)malloc(matrixColSize * sizeof(int));
	}

	a = spiralOrder(matrix, matrixSize, &matrixColSize, &returnSize);
	printf("\t returnSize = %d\n", returnSize);
	printf("\t [");
	for(i = 0; i < returnSize; i++){
		printf(" %d ", a[i]);
	}
	printf(" ]\n\n");
	free(a);
	a = NULL;
	free(matrix);
	matrix = NULL;
 
	return; 
 
 }


 int main(int argc, char ** argv){
	testspiralOrder();
 }



