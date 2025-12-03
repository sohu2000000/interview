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

## 📖 学习资源

- **[LeetCode 解题经验总结](LeetCode/com/doc/LeetCode解题经验总结.md)** - 23道题目的核心技巧、易错点和面试要点

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

### 哈希表
- [x] **49** - Group Anagrams (字母异位词分组)
- [x] **128** - Longest Consecutive Sequence (最长连续序列)

### 栈
- [x] **71** - Simplify Path (简化路径)
- [x] **150** - Evaluate Reverse Polish Notation (逆波兰表达式求值)
- [x] **155** - Min Stack (最小栈)

### 贪心算法与区间
- [x] **56** - Merge Intervals (合并区间)
- [x] **57** - Insert Interval (插入区间)
- [x] **134** - Gas Station (加油站)
- [x] **452** - Minimum Number of Arrows to Burst Balloons (用最少数量的箭引爆气球)

### 矩阵
- [x] **36** - Valid Sudoku (有效的数独)
- [x] **48** - Rotate Image (旋转图像)
- [x] **54** - Spiral Matrix (螺旋矩阵)
- [x] **73** - Set Matrix Zeroes (矩阵置零)
- [x] **289** - Game of Life (生命游戏)

### 链表
- [x] **2** - Add Two Numbers (两数相加)
- [x] **19** - Remove Nth Node From End of List (删除链表的倒数第N个节点)
- [x] **61** - Rotate List (旋转链表)
- [x] **82** - Remove Duplicates from Sorted List II (删除排序链表中的重复元素II)
- [x] **86** - Partition List (分隔链表)
- [x] **92** - Reverse Linked List II (反转链表II)
- [x] **138** - Copy List with Random Pointer (复制带随机指针的链表)

### 二叉树
- [x] **98** - Validate Binary Search Tree (验证二叉搜索树)
- [x] **102** - Binary Tree Level Order Traversal (二叉树的层序遍历)
- [x] **103** - Binary Tree Zigzag Level Order Traversal (二叉树的锯齿形层序遍历)
- [x] **105** - Construct Binary Tree from Preorder and Inorder Traversal (从前序与中序遍历构造二叉树)
- [x] **106** - Construct Binary Tree from Inorder and Postorder Traversal (从中序与后序遍历构造二叉树)
- [x] **114** - Flatten Binary Tree to Linked List (二叉树展开为链表)
- [x] **117** - Populating Next Right Pointers in Each Node II (填充每个节点的下一个右侧节点指针 II)
- [x] **129** - Sum Root to Leaf Numbers (求根节点到叶节点数字之和)
- [x] **173** - Binary Search Tree Iterator (二叉搜索树迭代器)
- [x] **199** - Binary Tree Right Side View (二叉树的右视图)
- [x] **230** - Kth Smallest Element in a BST (二叉搜索树中第K小的元素)
- [x] **236** - Lowest Common Ancestor of a Binary Tree (二叉树的最近公共祖先)

### 数据结构设计
- [x] **146** - LRU Cache (LRU缓存)
- [x] **380** - Insert Delete GetRandom O(1) (常数时间插入删除和获取随机元素)

### 图论/DFS/BFS
- [x] **130** - Surrounded Regions (被围绕的区域)
- [x] **133** - Clone Graph (克隆图)
- [x] **200** - Number of Islands (岛屿数量)
- [x] **399** - Evaluate Division (除法求值)

## 💡 核心算法与技巧

### 1. 贪心算法 (Greedy)
- Jump Game I & II - 维护最远可达位置
- Gas Station - 跳过不可能的起点
- Merge Intervals - 排序后贪心合并
- Insert Interval - 一次遍历插入合并
- Minimum Arrows - 按end排序，贪心射箭
- H-Index (排序法)

### 2. 双指针 (Two Pointers)
- Two Sum II - 对撞指针
- Three Sum - 固定一个数 + 双指针
- Container With Most Water - 贪心移动较短边
- Remove Nth Node - 快慢指针，间隔n+1
- Reverse Words - 双指针压缩空格

### 3. 滑动窗口 (Sliding Window)
- Longest Substring - 窗口 + 哈希表
- Minimum Subarray Sum - 动态窗口

### 4. 哈希表 (Hash Table)
- Group Anagrams - 排序后的字符串作为键，uthash分组
- Longest Consecutive Sequence - 哈希集合O(1)查找，只从起点计数

### 5. 栈 (Stack)
- Simplify Path - 栈处理目录，遇到".."弹栈
- Evaluate RPN - 栈计算后缀表达式，注意负数判断
- Min Stack - 双栈同步，minStack存储每层最小值

### 6. 数组技巧
- Product Except Self - 左右累积乘积
- Rotate Array - 三次反转

### 7. 数据结构设计
- LRU Cache - 双向链表 + 哈希表，O(1)所有操作
- RandomizedSet - 动态数组 + 哈希表 + 交换删除
- Copy Random List - 交织法（O(1)空间）

### 8. 矩阵操作
- Valid Sudoku - 三个哈希表标记行/列/方块
- Rotate Image - 转置 + 反转每行 = 顺时针90度
- Spiral Matrix - 四边界螺旋遍历，注意单行/单列
- Set Matrix Zeroes - 用首行首列作标记，O(1)空间
- Game of Life - 状态编码（2位表示前后状态），原地更新

### 9. 二叉树
- Validate BST (98) - 范围约束递归传递，用long避免边界问题
- Level Order Traversal (102) - BFS层序遍历标准模板，返回二维数组
- Zigzag Level Order (103) - BFS + 方向标记，逆序存储偶数层
- Construct Binary Tree (105) - 前序+中序递归构造，哈希表优化O(n)
- Construct Binary Tree (106) - 中序+后序递归构造，根节点在后序末尾
- Flatten Binary Tree (114) - Morris遍历展开，O(1)空间原地操作
- Populating Next Right Pointers (117) - 层序遍历，利用next指针连接，O(1)空间
- Sum Root to Leaf Numbers (129) - DFS路径累积，数字=父数字*10+当前值
- BST Iterator (173) - 栈模拟中序遍历，平均O(1)时间，O(h)空间
- Binary Tree Right Side View (199) - BFS层序遍历，记录每层最右节点
- Kth Smallest in BST (230) - 中序遍历计数，先++后判断，剪枝优化
- Lowest Common Ancestor (236) - 后序遍历递归，信息向上传递

### 10. 图论/DFS/BFS
- Surrounded Regions (130) - 反向思维，从边界DFS标记不被围绕的O
- Clone Graph (133) - DFS + 哈希表，先克隆再递归邻居防环
- Number of Islands (200) - DFS标记连通分量，visited数组防重复
- Evaluate Division (399) - 带权有向图+DFS路径搜索，累积边权重

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