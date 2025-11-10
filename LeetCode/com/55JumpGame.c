/*
You are given an integer array nums. You are initially positioned at the array's first index, and each element in the array represents your maximum jump length at that position.

Return true if you can reach the last index, or false otherwise.

 

Example 1:

Input: nums = [2,3,1,1,4]
Output: true
Explanation: Jump 1 step from index 0 to 1, then 3 steps to the last index.
Example 2:

Input: nums = [3,2,1,0,4]
Output: false
Explanation: You will always arrive at index 3 no matter what. Its maximum jump length is 0, which makes it impossible to reach the last index.
 

Constraints:

1 <= nums.length <= 104
0 <= nums[i] <= 105
*/

/**
 * 贪心算法：维护能到达的最远位置
 * 时间复杂度：O(n) - 只遍历一次数组
 * 空间复杂度：O(1) - 只用了常量额外空间
 * 
 * 核心思想：
 * 不需要模拟真实的跳跃过程，只需要知道"当前能到达的最远位置"
 * 遍历数组时，不断更新这个最远位置
 * 如果某个位置超出了最远位置，说明到不了，返回false
 */
bool canJump(int* nums, int numsSize) {
    int farthest = 0;  // 当前能到达的最远位置
    int i;
    
    // 遍历数组中的每个位置
    for (i = 0; i < numsSize; i++) {
        // 如果当前位置超出了能到达的最远位置，说明到不了这里
        if (i > farthest) {
            return false;
        }
        
        // 从位置i可以跳到的最远位置是：i + nums[i]
        // 更新全局的最远可达位置
        int canReach = i + nums[i];
        if (canReach > farthest) {
            farthest = canReach;
        }
        
        // 优化：如果已经能到达或超过最后一个位置，直接返回true
        if (farthest >= numsSize - 1) {
            return true;
        }
    }
    
    // 遍历结束，检查能否到达最后位置
    return farthest >= numsSize - 1;
}