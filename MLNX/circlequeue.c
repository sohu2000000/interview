#include <stdio.h>
#include <stdlib.h>
#define SIZE 5

typedef struct {
    int arr[SIZE];
    int front, rear, count;
} CircularQueue;

// 初始化队列
void initQueue(CircularQueue* q) {
    q->front = 0;
    q->rear = -1;
    q->count = 0;
}

// 判空
int is_empty(CircularQueue* q) {
    return q->count == 0;
}

// 判满
int is_full(CircularQueue* q) {
    return q->count == SIZE;
}

// 入队
void enqueue(CircularQueue* q, int value) {
    if (is_full(q)) {
        printf("Queue is full\n");
        return;
    }
    q->rear = (q->rear + 1) % SIZE;
    q->arr[q->rear] = value;
    q->count++;
    printf("Inserted %d\n", value);
}

// 出队
void dequeue(CircularQueue* q) {
    if (is_empty(q)) {
        printf("Queue is empty\n");
        return;
    }
    printf("Deleted %d\n", q->arr[q->front]);
    q->front = (q->front + 1) % SIZE;
    q->count--;
}

// 显示队列
void display(CircularQueue* q) {
    if (is_empty(q)) {
        printf("Queue is empty\n");
        return;
    }
    printf("Queue elements: ");
    for (int i = 0; i < q->count; i++) {
        int idx = (q->front + i) % SIZE;
        printf("%d ", q->arr[idx]);
    }
    printf("\n");
}

// 打印队列内部状态（用于调试循环）
void print_internal_state(CircularQueue* q) {
    printf("Internal state: front=%d, rear=%d, count=%d\n", q->front, q->rear, q->count);
    printf("Array: [");
    for (int i = 0; i < SIZE; i++) {
        printf("%d", q->arr[i]);
        if (i < SIZE - 1) printf(", ");
    }
    printf("]\n\n");
}

int main() {
    CircularQueue q;
    initQueue(&q);
    
    printf("=== 循环队列测试 - 包含充分的循环测试 ===\n\n");

    // 原始测试
    printf("1. 基本测试 - 填满队列:\n");
    enqueue(&q, 10);
    enqueue(&q, 20);
    enqueue(&q, 30);
    enqueue(&q, 40);
    enqueue(&q, 50);
    display(&q);
    print_internal_state(&q);

    printf("2. 出队操作:\n");
    dequeue(&q);
    dequeue(&q);
    display(&q);
    print_internal_state(&q);

    printf("3. 测试rear指针循环回绕:\n");
    enqueue(&q, 60);
    printf("rear应该从4回绕到0\n");
    display(&q);
    print_internal_state(&q);
    
    // 增强的循环测试
    printf("4. 继续测试rear指针循环:\n");
    enqueue(&q, 70);
    printf("rear应该继续到1\n");
    display(&q);
    print_internal_state(&q);
    
    printf("5. 测试front指针循环回绕:\n");
    // 出队直到front也需要回绕
    dequeue(&q); // front: 2->3
    dequeue(&q); // front: 3->4  
    dequeue(&q); // front: 4->0 (回绕!)
    printf("front应该从4回绕到0\n");
    display(&q);
    print_internal_state(&q);
    
    printf("6. 验证front回绕后的正确性:\n");
    dequeue(&q); // front: 0->1
    printf("front应该继续到1\n");
    display(&q);
    print_internal_state(&q);
    
    printf("7. 测试多次循环 - 连续入队出队:\n");
    for (int i = 0; i < 10; i++) {
        enqueue(&q, 80 + i);
        if (i % 2 == 1) { // 每两次入队后出队一次
            dequeue(&q);
        }
        if (i == 4 || i == 9) {
            printf("第%d轮后的状态:\n", i + 1);
            display(&q);
            print_internal_state(&q);
        }
    }
    
    printf("8. 最终清空队列验证:\n");
    while (!is_empty(&q)) {
        dequeue(&q);
    }
    printf("队列已清空\n");
    print_internal_state(&q);

    return 0;
}
