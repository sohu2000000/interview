/*
Design a stack that supports push, pop, top, and retrieving the minimum element in constant time.

Implement the MinStack class:

MinStack() initializes the stack object.
void push(int val) pushes the element val onto the stack.
void pop() removes the element on the top of the stack.
int top() gets the top element of the stack.
int getMin() retrieves the minimum element in the stack.
You must implement a solution with O(1) time complexity for each function.

 

Example 1:

Input
["MinStack","push","push","push","getMin","pop","top","getMin"]
[[],[-2],[0],[-3],[],[],[],[]]

Output
[null,null,null,null,-3,null,0,-2]

Explanation
MinStack minStack = new MinStack();
minStack.push(-2);
minStack.push(0);
minStack.push(-3);
minStack.getMin(); // return -3
minStack.pop();
minStack.top();    // return 0
minStack.getMin(); // return -2
 

Constraints:

-231 <= val <= 231 - 1
Methods pop, top and getMin operations will always be called on non-empty stacks.
At most 3 * 104 calls will be made to push, pop, top, and getMin.
 

 */

#define MAX_SIZE 10000

typedef struct {
   int *stack;      // 主栈：存储所有元素
   int *minStack;   // 最小值栈：存储每个状态的最小值
   int top;         // 栈顶索引
} MinStack;
 
 
 MinStack* minStackCreate() {
    MinStack *obj = (MinStack *)malloc(sizeof(MinStack));
    obj->stack = (int *)malloc(MAX_SIZE * sizeof(int));
    obj->minStack = (int *)malloc(MAX_SIZE * sizeof(int));
    obj->top = -1;
    return obj;
}
 
 void minStackPush(MinStack* obj, int val) {
	// 入主栈
	obj->top++;
	obj->stack[obj->top] = val;
	
	// 更新最小值栈
	if (obj->top == 0) {
		// 第一个元素
		obj->minStack[obj->top] = val;
	} else {
		// 取当前最小值和新值的较小者
		int currentMin = obj->minStack[obj->top - 1];
		obj->minStack[obj->top] = (val < currentMin) ? val : currentMin;
	}
 
}
 
 void minStackPop(MinStack* obj) {
	if (obj->top >= 0) {
		obj->top--;
	}
}
 
 int minStackTop(MinStack* obj) {
	return obj->stack[obj->top];
}
 
 int minStackGetMin(MinStack* obj) {
	return obj->minStack[obj->top];  // 修复：直接用top索引
}
 
 void minStackFree(MinStack* obj) {
    free(obj->stack);
    free(obj->minStack);
    free(obj);
 }
 
 /**
  * Your MinStack struct will be instantiated and called as such:
  * MinStack* obj = minStackCreate();
  * minStackPush(obj, val);
  
  * minStackPop(obj);
  
  * int param_3 = minStackTop(obj);
  
  * int param_4 = minStackGetMin(obj);
  
  * minStackFree(obj);
 */