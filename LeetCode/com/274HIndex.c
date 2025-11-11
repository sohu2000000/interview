/*
Given an array of integers citations where citations[i] is the number of citations a researcher received for their ith paper, return the researcher's h-index.

According to the definition of h-index on Wikipedia: The h-index is defined as the maximum value of h such that the given researcher has published at least h papers that have each been cited at least h times.

Example 1:

Input: citations = [3,0,6,1,5]
Output: 3
Explanation: [3,0,6,1,5] means the researcher has 5 papers in total and each of them had received 3, 0, 6, 1, 5 citations respectively.
Since the researcher has 3 papers with at least 3 citations each and the remaining two with no more than 3 citations each, their h-index is 3.
Example 2:

Input: citations = [1,3,1]
Output: 1
Explanation: Since the researcher has 1 paper with at least 1 citation and the remaining two with no more than 1 citation each, their h-index is 1.
 */

// ========== 方案0：你的方法（暴力枚举）==========
// 时间复杂度：O(n²)
// 空间复杂度：O(1)
// 优点：逻辑简单，容易理解
// 缺点：效率低
int hIndex_bruteforce(int* citations, int citationsSize) {
	int i, h, cnt;
    
	// 从最大可能的h值往下找
	for (h = citationsSize; h >= 0; h--) {
	    cnt = 0;
	    // 统计有多少篇论文引用次数 >= h
	    for (i = 0; i < citationsSize; i++) {
		if (citations[i] >= h)
		    cnt++;
	    }
	    // 如果至少有h篇论文被引用>=h次，就找到了
	    if (cnt >= h)  return h;
	}
    
	return h;
}


// ========== 方案1：排序法（推荐）==========
// 时间复杂度：O(n log n) - 排序
// 空间复杂度：O(1) - 原地排序
// 优点：代码简洁，效率高
// 最推荐的方法！

// 比较函数（降序）
int compare_desc(const void* a, const void* b) {
    return *(int*)b - *(int*)a;
}

/**
 * 核心思想：排序后，从大到小遍历
 * 
 * 关键观察：
 * 排序后 citations[i] 是第 i+1 大的引用数
 * 如果 citations[i] >= i+1，说明至少有 i+1 篇论文被引用 >= i+1 次
 * 
 * 例子：[3,0,6,1,5] → 排序后 [6,5,3,1,0]
 * 
 * i=0: citations[0]=6 >= 1? 是，至少有1篇论文引用>=1次
 * i=1: citations[1]=5 >= 2? 是，至少有2篇论文引用>=2次
 * i=2: citations[2]=3 >= 3? 是，至少有3篇论文引用>=3次
 * i=3: citations[3]=1 >= 4? 否，只有3篇论文引用>=4次
 * 
 * 答案：h=3
 */
int hIndex(int* citations, int citationsSize) {
    if (citationsSize == 0) return 0;
    
    // 降序排序
    qsort(citations, citationsSize, sizeof(int), compare_desc);
    
    int h = 0;
    // 遍历排序后的数组
    for (int i = 0; i < citationsSize; i++) {
        // citations[i] 是第 i+1 大的引用数
        // 如果 citations[i] >= i+1，说明至少有 i+1 篇论文被引用 >= i+1 次
        if (citations[i] >= i + 1) {
            h = i + 1;  // 更新h值
        } else {
            break;  // 后面的都更小，不可能满足条件了
        }
    }
    
    return h;
}


// ========== 方案2：计数排序（最优）==========
// 时间复杂度：O(n)
// 空间复杂度：O(n)
// 优点：最快
// 缺点：需要额外空间

/**
 * 核心思想：用计数数组统计
 * 
 * count[i] = 引用次数为i的论文数量
 * 
 * 关键技巧：引用次数超过n的，统一算作n
 * （因为h-index最大就是n，超过n没意义）
 * 
 * 从大到小累加，找到第一个满足"累计论文数>=h"的h值
 */
int hIndex_counting(int* citations, int citationsSize) {
    if (citationsSize == 0) return 0;
    
    int n = citationsSize;
    // count[i] = 引用次数为i的论文数量
    // count[n] 存储所有引用次数>=n的论文
    int* count = (int*)calloc(n + 1, sizeof(int));
    
    // 统计每个引用次数的论文数量
    for (int i = 0; i < n; i++) {
        if (citations[i] >= n) {
            count[n]++;  // 超过n的统一算作n
        } else {
            count[citations[i]]++;
        }
    }
    
    // 从大到小累加，找h-index
    int total = 0;  // 引用次数>=h的论文总数
    for (int h = n; h >= 0; h--) {
        total += count[h];  // 累加引用次数>=h的论文数
        if (total >= h) {
            free(count);
            return h;
        }
    }
    
    free(count);
    return 0;
}