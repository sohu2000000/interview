# Interview - 面试算法题集

C语言实现的算法和数据结构题目集合，用于技术面试准备。

## 📚 项目结构

```
interview/
├── Algorithm/          # 基础算法实现
│   └── src/
│       ├── sort/      # 排序算法（冒泡、快排、归并等）
│       ├── string/    # 字符串操作
│       └── struct/    # 数据结构（链表、栈、队列、树）
├── LeetCode/          # LeetCode 题目解答
│   ├── cn/           # 中国站题目
│   └── com/          # 国际站题目
└── NV/               # 面试题目集
    ├── doc/          # 面试问答文档
    └── src/          # 实现代码
```

## 🎯 LeetCode 题目列表

### 数组与双指针
- [x] **11** - Container With Most Water (盛最多水的容器)
- [x] **15** - Three Sum (三数之和)
- [x] **55** - Jump Game (跳跃游戏)
- [x] **45** - Jump Game II (跳跃游戏 II)
- [x] **80** - Remove Duplicates from Sorted Array II
- [x] **122** - Best Time to Buy and Sell Stock II
- [x] **167** - Two Sum II (两数之和 II)
- [x] **189** - Rotate Array (旋转数组)
- [x] **238** - Product of Array Except Self (除自身以外数组的乘积)
- [x] **274** - H-Index (H指数)

### 滑动窗口
- [x] **3** - Longest Substring Without Repeating Characters (无重复字符的最长子串)
- [x] **209** - Minimum Size Subarray Sum (长度最小的子数组)

### 字符串
- [x] **6** - Zigzag Conversion (Z字形变换)
- [x] **12** - Integer to Roman (整数转罗马数字)
- [x] **151** - Reverse Words in a String (反转字符串中的单词)

### 贪心算法
- [x] **134** - Gas Station (加油站)

### 矩阵
- [x] **36** - Valid Sudoku (有效的数独)
- [x] **54** - Spiral Matrix (螺旋矩阵)

### 链表
- [x] **138** - Copy List with Random Pointer (复制带随机指针的链表)

### 数据结构设计
- [x] **380** - Insert Delete GetRandom O(1) (常数时间插入删除和获取随机元素)

## 💡 核心算法与技巧

### 1. 贪心算法 (Greedy)
- Jump Game I & II - 维护最远可达位置
- Gas Station - 跳过不可能的起点
- H-Index (排序法)

### 2. 双指针 (Two Pointers)
- Two Sum II - 对撞指针
- Three Sum - 固定一个数 + 双指针
- Container With Most Water - 贪心移动较短边
- Reverse Words - 双指针压缩空格

### 3. 滑动窗口 (Sliding Window)
- Longest Substring - 窗口 + 哈希表
- Minimum Subarray Sum - 动态窗口

### 4. 数组技巧
- Product Except Self - 左右累积乘积
- Rotate Array - 三次反转

### 5. 数据结构设计
- RandomizedSet - 动态数组 + 哈希表 + 交换删除
- Copy Random List - 交织法（O(1)空间）

### 6. 矩阵操作
- Valid Sudoku - 三个哈希表标记行/列/方块
- Spiral Matrix - 四边界螺旋遍历，注意单行/单列

## 🔧 编译与运行

```bash
# 编译单个文件
gcc -o output LeetCode/com/55JumpGame.c -std=c99

# 编译算法库
cd Algorithm/src
gcc -o test sort/QuickSort.c
```

## 📖 学习笔记

### 时间复杂度分析
- **摊还分析**：动态数组扩容虽然单次O(n)，但摊还O(1)
- **BFS思想**：Jump Game II 的分层遍历
- **贪心正确性**：Gas Station 的数学证明

### C语言技巧
- VLA（可变长度数组）：栈上分配，不能free
- 哈希表实现：链地址法 + 头插法
- 字符串操作：避免多次strcat（O(n²)），用pos维护

## 🎓 面试重点

1. **算法思想**：能清楚解释为什么这样做
2. **时间空间分析**：准确分析复杂度
3. **边界处理**：空数组、单元素、重复值
4. **代码质量**：变量命名清晰、注释充分
5. **优化思路**：能提出并实现优化方案

## 📝 License

学习和面试使用