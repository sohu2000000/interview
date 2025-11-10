/*
You are given a 0-indexed array of integers nums of length n. You are initially positioned at index 0.

Each element nums[i] represents the maximum length of a forward jump from index i. In other words, if you are at index i, you can jump to any index (i + j) where:

0 <= j <= nums[i] and
i + j < n
Return the minimum number of jumps to reach index n - 1. The test cases are generated such that you can reach index n - 1.

 

Example 1:

Input: nums = [2,3,1,1,4]
Output: 2
Explanation: The minimum number of jumps to reach the last index is 2. Jump 1 step from index 0 to 1, then 3 steps to the last index.
Example 2:

Input: nums = [2,3,0,1,4]
Output: 2
 

Constraints:

1 <= nums.length <= 104
0 <= nums[i] <= 1000
It's guaranteed that you can reach nums[n - 1].
 */

// ========== 方案1：模拟跳跃（你的思路）==========
// 时间复杂度：O(n²)
// 优点：逻辑直观，容易理解
// 缺点：效率较低

// 找能跳得最远的位置（关键：比较 i+nums[i]，不是 nums[i]）
int get_next_ix(int * nums, int numsSize, int start, int end) {
	int maxReach, ix, i;
    
	maxReach = start + nums[start];
	ix = start;
	
	for(i = start; i <= end && i < numsSize; i++) {
	    int reach = i + nums[i];  // 关键：从位置i能跳到的最远位置
	    if (reach > maxReach) {
		maxReach = reach;
		ix = i;
	    }
	}
    
	return ix;
}
    
int jump_v1(int* nums, int numsSize) {
	int i, ix = 0, cnt = 0;
    
	if (numsSize <= 1) return 0;
    
	i = 0;
	while(i < numsSize - 1) {
	    if (i + nums[i] >= numsSize - 1) {
		return cnt + 1;
	    }
    
	    ix = get_next_ix(nums, numsSize, i + 1, i + nums[i]);
	    
	    i = ix;
	    cnt++;
	}
    
	return cnt;
}


// ========== 方案2：BFS思想（最推荐）==========
// 时间复杂度：O(n) - 只遍历一次数组
// 空间复杂度：O(1)
// 优点：最高效，代码简洁

/**
 * 核心思想：把数组看成多个"层"
 * 
 * 类比BFS：
 * - 第1跳能到达的所有位置是"第1层"
 * - 第2跳能到达的所有新位置是"第2层"
 * - 每扩展一层，steps就+1
 * 
 * 关键变量：
 * - currentEnd：当前这一跳能到达的最远位置（当前层的右边界）
 * - farthest：遍历到目前为止，下一跳能到达的最远位置（下一层的右边界）
 * 
 * 例子：[2,3,1,1,4]
 * 
 * 第0层（起点）：位置0
 * i=0: farthest=0+2=2, i==currentEnd(0), steps=1, currentEnd=2
 *      意思：第1跳能到达[1,2]范围
 * 
 * 第1层：位置[1,2]
 * i=1: farthest=max(2,1+3)=4
 * i=2: farthest=4, i==currentEnd(2), steps=2, currentEnd=4
 *      意思：第2跳能到达位置4
 *      currentEnd>=4（终点），结束
 * 
 * 返回：steps=2
 */
int jump(int* nums, int numsSize) {
    if (numsSize <= 1) return 0;
    
    int steps = 0;        // 跳跃次数
    int currentEnd = 0;   // 当前这一跳的边界（当前层的右边界）
    int farthest = 0;     // 下一跳能到达的最远位置（下一层的右边界）
    int i;
    
    // 遍历数组（不需要访问最后一个位置，因为到达倒数第二个就够了）
    for (i = 0; i < numsSize - 1; i++) {
        // 更新下一跳能到达的最远位置
        int canReach = i + nums[i];
        if (canReach > farthest) {
            farthest = canReach;
        }
        
        // 到达当前层的边界，需要跳到下一层了
        if (i == currentEnd) {
            steps++;              // 跳跃次数+1
            currentEnd = farthest; // 更新下一层的边界
            
            // 优化：如果已经能到达终点，提前结束
            if (currentEnd >= numsSize - 1) {
                break;
            }
        }
    }
    
    return steps;
}
