/*
There are n gas stations along a circular route, where the amount of gas at the ith station is gas[i].

You have a car with an unlimited gas tank and it costs cost[i] of gas to travel from the ith station to its next (i + 1)th station. You begin the journey with an empty tank at one of the gas stations.

Given two integer arrays gas and cost, return the starting gas station's index if you can travel around the circuit once in the clockwise direction, otherwise return -1. If there exists a solution, it is guaranteed to be unique.

 

Example 1:

Input: gas = [1,2,3,4,5], cost = [3,4,5,1,2]
Output: 3
Explanation:
Start at station 3 (index 3) and fill up with 4 unit of gas. Your tank = 0 + 4 = 4
Travel to station 4. Your tank = 4 - 1 + 5 = 8
Travel to station 0. Your tank = 8 - 2 + 1 = 7
Travel to station 1. Your tank = 7 - 3 + 2 = 6
Travel to station 2. Your tank = 6 - 4 + 3 = 5
Travel to station 3. The cost is 5. Your gas is just enough to travel back to station 3.
Therefore, return 3 as the starting index.
Example 2:

Input: gas = [2,3,4], cost = [3,4,3]
Output: -1
Explanation:
You can't start at station 0 or 1, as there is not enough gas to travel to the next station.
Let's start at station 2 and fill up with 4 unit of gas. Your tank = 0 + 4 = 4
Travel to station 0. Your tank = 4 - 3 + 2 = 3
Travel to station 1. Your tank = 3 - 3 + 3 = 3
You cannot travel back to station 2, as it requires 4 unit of gas but you only have 3.
Therefore, you can't travel around the circuit once no matter where you start.
 

Constraints:

n == gas.length == cost.length
1 <= n <= 105
0 <= gas[i], cost[i] <= 104
The input is generated such that the answer is unique.
 
 */
/**
 * 贪心算法：一次遍历找起点
 * 
 * 核心思想：
 * 1. total_tank：记录总的油量差（gas总和 - cost总和）
 *    - 如果 total_tank < 0，说明总油量不够，不可能完成
 *    - 如果 total_tank >= 0，说明一定存在解
 * 
 * 2. current_tank：从当前起点start出发，累积的油量
 *    - 如果 current_tank < 0，说明从start到i走不通
 *    - 此时更新起点为 i+1，重新开始累积
 * 
 * 关键定理：
 * 如果从A到B走不通（current_tank < 0），
 * 那么从A和B之间的任何点出发，也到不了B
 * 所以直接把起点设为B+1
 */
int canCompleteCircuit(int* gas, int gasSize, int* cost, int costSize) {
	int i, start, total_tank, current_tank; 
    
	total_tank = 0;
	current_tank = 0;
	start = 0;
	
	for (i = 0; i < gasSize; i++) {
	    int diff = gas[i] - cost[i];
	    current_tank += diff;
	    total_tank += diff;
    
	    // 如果从start到i走不通，更新起点为i+1
	    if (current_tank < 0) {
		start = i + 1;
		current_tank = 0;
	    }
	}
    
	// 修复：在循环结束后检查total_tank
	// 如果总油量不够，不可能完成
	if (total_tank < 0) {
	    return -1;
	}
	
	return start;
}