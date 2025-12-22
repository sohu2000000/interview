#include <stdlib.h>
#include <stdbool.h>

/**
 * Min Heap data structure
 */
typedef struct MinHeap {
	int *data;       // Heap array
	int capacity;    // Maximum capacity
	int size;        // Current number of elements
} MinHeap;

/**
 * Create a new min heap with given capacity
 */
MinHeap *heapCreate(int capacity) {
	MinHeap *heap = (MinHeap *)malloc(sizeof(MinHeap));
	heap->data = (int *)malloc(sizeof(int) * capacity);
	heap->capacity = capacity;
	heap->size = 0;
	return heap;
}

/**
 * Destroy heap and free memory
 */
void heapDestroy(MinHeap *heap) {
	free(heap->data);
	free(heap);
}

/**
 * Swap two integers
 */
void swap(int *a, int *b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

/**
 * Sift up: maintain heap property by moving element up
 * Used after inserting a new element at the end
 */
void siftUp(MinHeap *heap, int index) {
	int parentIndex;

	while (index > 0) {
		parentIndex = (index - 1) / 2;
		
		// If parent is smaller or equal, heap property satisfied
		if (heap->data[parentIndex] <= heap->data[index])
			break;

		// Swap with parent and continue
		swap(&heap->data[parentIndex], &heap->data[index]);
		index = parentIndex;
	}
}

/**
 * Sift down: maintain heap property by moving element down
 * Used after removing the root element
 */
void siftDown(MinHeap *heap, int index) {
	int leftChild, rightChild, smallestIndex;

	while (true) {
		leftChild = 2 * index + 1;
		rightChild = 2 * index + 2;
		smallestIndex = index;
		
		// Find the smallest among parent, left child, and right child
		if (leftChild < heap->size && heap->data[leftChild] < heap->data[smallestIndex])
			smallestIndex = leftChild;
		if (rightChild < heap->size && heap->data[rightChild] < heap->data[smallestIndex])
			smallestIndex = rightChild;
		
		// If current node is smallest, heap property satisfied
		if (smallestIndex == index)
			break;
		
		// Swap with smallest child and continue
		swap(&heap->data[index], &heap->data[smallestIndex]);
		index = smallestIndex;
	}
}

/**
 * Remove and return the minimum element (root)
 */
int heapPop(MinHeap *heap) {
	int minValue;

	if (heap->size == 0)
		return -1;
	
	// Save root value
	minValue = heap->data[0];
	
	// Move last element to root
	heap->data[0] = heap->data[heap->size - 1];
	heap->size--;
	
	// Restore heap property
	siftDown(heap, 0);
	
	return minValue;
}

/**
 * Insert a new element into the heap
 */
void heapPush(MinHeap *heap, int value) {
	if (heap->size >= heap->capacity)
		return;

	// Add new element at the end
	heap->data[heap->size] = value;
	heap->size++;
	
	// Restore heap property
	siftUp(heap, heap->size - 1);
}

/**
 * Get the minimum element without removing it
 */
int heapTop(MinHeap *heap) {
	return heap->data[0];
}

/**
 * LeetCode 215: Kth Largest Element in an Array
 * 
 * Find the kth largest element in an unsorted array.
 * Note that it is the kth largest element in sorted order, not the kth distinct element.
 * 
 * Algorithm:
 * Use a min heap of size k to maintain the k largest elements seen so far.
 * 
 * Strategy:
 * 1. Build a min heap of size k
 * 2. For each element:
 *    - If heap size < k: push element
 *    - If element > heap top: pop smallest, push element
 * 3. The heap top is the kth largest element
 * 
 * Why min heap?
 * - Min heap's root is the smallest among k largest elements
 * - This smallest element is exactly the kth largest in the array
 * 
 * Time Complexity: O(n log k) - n elements, each operation O(log k)
 * Space Complexity: O(k) - heap size is k
 * 
 * Example:
 * Input: nums = [3,2,1,5,6,4], k = 2
 * Output: 5
 */
int findKthLargest(int* nums, int numsSize, int k) {
	int result, i;
	MinHeap *minHeap;

	// Create a min heap with capacity k
	minHeap = heapCreate(numsSize);
	
	// Process each element
	for (i = 0; i < numsSize; i++) {
		if (minHeap->size < k) {
			// Heap not full: add element
			heapPush(minHeap, nums[i]);
		} else if (nums[i] > heapTop(minHeap)) {
			// Element larger than heap minimum: replace
			heapPop(minHeap);
			heapPush(minHeap, nums[i]);
		}
	}

	// The root of min heap is the kth largest element
	result = heapTop(minHeap);
	heapDestroy(minHeap);

	return result;
}