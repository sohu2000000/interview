# Algorithm - 基础算法与数据结构

C语言实现的经典算法和数据结构，涵盖面试常见问题。

## 📂 目录结构

```
Algorithm/
└── src/
    ├── sort/       # 排序算法
    ├── string/     # 字符串操作
    └── struct/     # 数据结构
```

## 🔄 排序算法 (src/sort/)

### 已实现的排序算法

| 算法 | 时间复杂度（平均）| 空间复杂度 | 稳定性 | 文件 |
|-----|-----------------|-----------|-------|------|
| **冒泡排序** | O(n²) | O(1) | 稳定 | BubbleSort.c |
| **选择排序** | O(n²) | O(1) | 不稳定 | SelectSort.c |
| **插入排序** | O(n²) | O(1) | 稳定 | InsertSort.c |
| **希尔排序** | O(n^1.3) | O(1) | 不稳定 | ShellSort.c |
| **快速排序** | O(n log n) | O(log n) | 不稳定 | QuickSort.c |
| **归并排序** | O(n log n) | O(n) | 稳定 | MergeSort.c |
| **堆排序** | O(n log n) | O(1) | 不稳定 | HeapSort.c |
| **桶排序** | O(n+k) | O(n+k) | 稳定 | BucketSort.c |

### 排序算法选择指南

```
数据量小（<50）：插入排序
数据基本有序：插入排序、冒泡排序
需要稳定性：归并排序、冒泡排序
内存受限：堆排序、快速排序
追求性能：快速排序（平均最快）
最坏情况保证：归并排序、堆排序
```

## 📝 字符串操作 (src/string/)

### 实现的功能

- `str.h/str.c` - 字符串基本操作
  - 字符串复制
  - 字符串比较
  - 字符串查找
  - 字符串反转
  - 其他常用操作

- `test_str_basic.c` - 字符串功能测试
- `main.c` - 示例程序

## 🏗️ 数据结构 (src/struct/)

### 线性结构

| 数据结构 | 实现文件 | 测试文件 | 特点 |
|---------|---------|---------|------|
| **单链表** | list.h/list.c | test_list.c | 动态、插入删除O(1) |
| **双向链表** | doublist.h/doublist.c | test_doublist.c | 双向遍历 |
| **栈** | stack.h/stack.c | test_stack.c | LIFO（后进先出）|
| **队列** | queue.h/queue.c | test_queue.c | FIFO（先进先出）|

### 树结构

| 数据结构 | 实现文件 | 测试文件 | 特点 |
|---------|---------|---------|------|
| **二叉树** | tree.h/tree.c | test_tree.c | 基础树操作 |
| **树深度** | - | test_tree_depth.c | 递归/迭代计算深度 |

### 核心操作

**链表**：
- 插入（头插、尾插、中间插入）
- 删除（按值、按位置）
- 查找
- 反转
- 合并

**栈**：
- push（入栈）
- pop（出栈）
- peek（查看栈顶）
- isEmpty（判空）

**队列**：
- enqueue（入队）
- dequeue（出队）
- front（查看队首）
- isEmpty（判空）

**树**：
- 遍历（前序、中序、后序、层序）
- 插入、删除、查找
- 深度计算
- 节点计数

## 🔧 编译运行

### 编译排序算法

```bash
cd Algorithm/src/sort
gcc -o quicksort QuickSort.c
./quicksort
```

### 编译数据结构

```bash
cd Algorithm/src/struct

# 编译链表测试
gcc -o test_list test_list.c list.c
./test_list

# 编译栈测试
gcc -o test_stack test_stack.c stack.c
./test_stack

# 编译树测试
gcc -o test_tree test_tree.c tree.c
./test_tree
```

### 编译字符串操作

```bash
cd Algorithm/src/string
gcc -o test_str test_str_basic.c str.c
./test_str
```

## 📚 学习路径

### 基础阶段
1. **排序算法**：理解各种排序的原理和适用场景
2. **链表操作**：掌握指针操作，反转、合并等技巧
3. **栈和队列**：理解LIFO和FIFO特性

### 进阶阶段
1. **树的遍历**：递归和迭代实现
2. **复杂度分析**：时间和空间权衡
3. **算法优化**：从O(n²)到O(n log n)

### 实战阶段
1. 结合LeetCode题目练习
2. 理解算法在实际问题中的应用
3. 掌握常见优化技巧

## 💡 面试要点

### 排序算法
- 快排的partition过程
- 归并排序的分治思想
- 堆排序的堆调整

### 数据结构
- 链表的双指针技巧
- 栈的应用场景（括号匹配、表达式求值）
- 队列的应用（BFS、任务调度）
- 树的递归思维

### 常见问题
- 反转链表
- 检测环
- 合并有序链表
- 二叉树遍历
- 栈实现队列/队列实现栈

## 📖 参考资料

- 《数据结构》（严蔚敏）
- 《算法导论》
- 《剑指Offer》