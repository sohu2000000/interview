#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * Pair structure to store index pair and their sum
 */
typedef struct IndexPair {
	int idx1;      // Index in nums1
	int idx2;      // Index in nums2
	int sum;       // Sum of nums1[idx1] + nums2[idx2]
} IndexPair;

/**
 * Min heap structure for pairs
 */
typedef struct MinHeap {
	IndexPair *data;
	int size;
	int capacity;
} MinHeap;

/**
 * Create a 2D visited array for tracking
 */
char** createVisited(int rowCount, int colCount) {
	char **visited;
	int i;

	visited = (char **)malloc(rowCount * sizeof(char *));
	for (i = 0; i < rowCount; i++) { 
		visited[i] = (char *)calloc(colCount, sizeof(char));
	}

	return visited;
}

/**
 * Destroy visited array and free memory
 */
void destroyVisited(char** visited, int rowCount) {
	int i;

	for (i = 0; i < rowCount; i++) {
		free(visited[i]);
	}
	free(visited);
}

/**
 * Create a min heap with given capacity
 */
MinHeap *heapCreate(int capacity) {
	MinHeap *heap = (MinHeap *)malloc(sizeof(MinHeap));
	heap->data = (IndexPair *)malloc(sizeof(IndexPair) * capacity);
	heap->size = 0;
	heap->capacity = capacity;
	return heap;
}

/**
 * Swap two pairs
 */
void swapPair(IndexPair *a, IndexPair *b) {
	IndexPair temp = *a;
	*a = *b;
	*b = temp;
}

/**
 * Destroy heap and free memory
 */
void heapDestroy(MinHeap *heap) {
	free(heap->data);
	free(heap);
}

/**
 * Sift up: maintain min heap property after insertion
 */
void siftUp(MinHeap *heap, int index) {
	int parentIndex;

	while (index > 0) {
		parentIndex = (index - 1) / 2;
		if (heap->data[parentIndex].sum <= heap->data[index].sum)
			break;
		
		swapPair(&heap->data[parentIndex], &heap->data[index]);
		index = parentIndex;
	}
}

/**
 * Sift down: maintain min heap property after deletion
 */
void siftDown(MinHeap *heap, int index) {
	int leftChild, rightChild, smallestIndex;

	while (true) {
		leftChild = 2 * index + 1;
		rightChild = 2 * index + 2;
		smallestIndex = index;

		if (leftChild < heap->size && 
		    heap->data[leftChild].sum < heap->data[smallestIndex].sum)
			smallestIndex = leftChild;
		if (rightChild < heap->size && 
		    heap->data[rightChild].sum < heap->data[smallestIndex].sum)
			smallestIndex = rightChild;
		
		if (smallestIndex == index)
			break;
		
		swapPair(&heap->data[index], &heap->data[smallestIndex]);
		index = smallestIndex;
	}
}

/**
 * Push a new pair into the heap
 */
void heapPush(MinHeap *heap, int sumValue, int idx1, int idx2) {
	if (heap->size >= heap->capacity)
		return;
	
	heap->data[heap->size].sum = sumValue;
	heap->data[heap->size].idx1 = idx1;
	heap->data[heap->size].idx2 = idx2;
	heap->size++;
	siftUp(heap, heap->size - 1);
}

/**
 * Pop and return the minimum pair from the heap
 */
IndexPair heapPop(MinHeap *heap) {
	IndexPair minPair;

	minPair = heap->data[0];
	heap->data[0] = heap->data[heap->size - 1];
	heap->size--;
	siftDown(heap, 0);

	return minPair;
}

/**
 * Get the minimum pair without removing it
 */
IndexPair heapTop(MinHeap *heap) {
	return heap->data[0];
}

/**
 * LeetCode 373: Find K Pairs with Smallest Sums
 * 
 * Given two sorted arrays and an integer k, find k pairs (u,v) with smallest sums,
 * where u is from nums1 and v is from nums2.
 * 
 * Algorithm:
 * 1. Use a min heap to explore pairs in ascending order of their sums
 * 2. Start from (0,0), expand down and right using BFS-like approach
 * 3. Use visited array to avoid duplicate pairs
 * 4. Optimization: only allocate visited for k×k area to save memory
 * 
 * Why heap capacity > k?
 * - Each iteration: pop 1 element, push up to 2 elements
 * - Heap size can temporarily exceed k during exploration
 * 
 * Time Complexity: O(k log k) - k iterations, each with heap operations
 * Space Complexity: O(k) - heap and visited array both O(k×k) = O(k²) for k<<n
 * 
 * Example:
 * Input: nums1 = [1,7,11], nums2 = [2,4,6], k = 3
 * Output: [[1,2], [1,4], [1,6]]
 */
int** kSmallestPairs(int* nums1, int nums1Size, int* nums2, int nums2Size, int k, 
                     int* returnSize, int** returnColumnSizes) {
	int currentIdx1, currentIdx2, resultIndex, i, heapCapacity, visitedRows, visitedCols;
	char **visited = NULL;
	int **result = NULL;
	IndexPair currentPair;
	MinHeap *minHeap;

	// Handle edge cases
	if (nums1Size == 0 || nums2Size == 0 || k == 0) {
		*returnSize = 0;
		*returnColumnSizes = NULL;
		return NULL;
	}

	// Allocate result array
	result = (int **)malloc(k * sizeof(int *));
	for (i = 0; i < k; i++) {
		result[i] = (int *)malloc(2 * sizeof(int));
	}

	// Memory optimization: only allocate visited for k×k area
	// Since we only need k pairs, we won't explore beyond k rows/columns
	visitedRows = (nums1Size < k) ? nums1Size : k;
	visitedCols = (nums2Size < k) ? nums2Size : k;
	visited = createVisited(visitedRows, visitedCols);
	
	// Heap capacity must be larger than k to handle temporary growth
	// During BFS: pop 1, push 2 → net +1 per iteration
	heapCapacity = (nums1Size + nums2Size > k) ? (nums1Size + nums2Size) : k;
	minHeap = heapCreate(heapCapacity);

	// Initialize: start from (0,0)
	currentIdx1 = 0;
	currentIdx2 = 0;
	resultIndex = 0;
	
	if (currentIdx1 < visitedRows) {
		heapPush(minHeap, nums1[currentIdx1] + nums2[currentIdx2], 
		        currentIdx1, currentIdx2);
		visited[currentIdx1][currentIdx2] = 1;
	}

	// BFS-like exploration: pop smallest, expand down and right
	while (resultIndex < k && minHeap->size > 0) {
		currentPair = heapPop(minHeap);
		result[resultIndex][0] = nums1[currentPair.idx1];
		result[resultIndex][1] = nums2[currentPair.idx2];
		resultIndex++;

		currentIdx1 = currentPair.idx1;
		currentIdx2 = currentPair.idx2;

		// Expand down: (i+1, j)
		if (currentIdx1 + 1 < visitedRows && currentIdx1 + 1 < nums1Size && 
		    currentIdx2 < visitedCols && visited[currentIdx1 + 1][currentIdx2] == 0) {
			heapPush(minHeap, nums1[currentIdx1 + 1] + nums2[currentIdx2], 
			        currentIdx1 + 1, currentIdx2);
			visited[currentIdx1 + 1][currentIdx2] = 1;
		}

		// Expand right: (i, j+1)
		if (currentIdx2 + 1 < visitedCols && currentIdx2 + 1 < nums2Size && 
		    currentIdx1 < visitedRows && visited[currentIdx1][currentIdx2 + 1] == 0) {
			heapPush(minHeap, nums1[currentIdx1] + nums2[currentIdx2 + 1], 
			        currentIdx1, currentIdx2 + 1);
			visited[currentIdx1][currentIdx2 + 1] = 1;
		}
	}

	// Clean up
	destroyVisited(visited, visitedRows);
	heapDestroy(minHeap);

	// Set return values
	*returnSize = resultIndex;
	*returnColumnSizes = (int *)malloc(resultIndex * sizeof(int));
	for (i = 0; i < resultIndex; i++) {
		(*returnColumnSizes)[i] = 2;
	}

	return result;
}