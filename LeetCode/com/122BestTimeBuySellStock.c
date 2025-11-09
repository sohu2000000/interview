/*
You are given an integer array prices where prices[i] is the price of a given stock on the ith day.

On each day, you may decide to buy and/or sell the stock. You can only hold at most one share of the stock at any time. However, you can sell and buy the stock multiple times on the same day, ensuring you never hold more than one share of the stock.

Find and return the maximum profit you can achieve.

 

Example 1:

Input: prices = [7,1,5,3,6,4]
Output: 7
Explanation: Buy on day 2 (price = 1) and sell on day 3 (price = 5), profit = 5-1 = 4.
Then buy on day 4 (price = 3) and sell on day 5 (price = 6), profit = 6-3 = 3.
Total profit is 4 + 3 = 7.
Example 2:

Input: prices = [1,2,3,4,5]
Output: 4
Explanation: Buy on day 1 (price = 1) and sell on day 5 (price = 5), profit = 5-1 = 4.
Total profit is 4.
Example 3:

Input: prices = [7,6,4,3,1]
Output: 0
Explanation: There is no way to make a positive profit, so we never buy the stock to achieve the maximum profit of 0.
 

Constraints:

1 <= prices.length <= 3 * 104
0 <= prices[i] <= 104
 */

int maxProfit(int* prices, int pricesSize) {
    int totalProfit = 0;
    
    /* 贪心算法（Greedy）：
     * 核心思想：抓住每一次价格上涨的机会
     * 贪心策略：只要明天价格 > 今天价格，就做交易（今天买，明天卖）
     * 正确性证明：任何利润区间 [buy, sell] 都可以拆分成每日差价之和
     *   例如：[1→5] 的利润 4 = (2-1)+(3-2)+(4-3)+(5-4) = 每日上涨累加
     */
    
    for (int day = 0; day < pricesSize - 1; day++) {
        int todayPrice = prices[day];
        int tomorrowPrice = prices[day + 1];
        
        // 贪心决策：只要有利可图，立即交易
        int profitFromThisTrade = tomorrowPrice - todayPrice;
        if (profitFromThisTrade > 0) {
            totalProfit += profitFromThisTrade;
        }
    }
    
    return totalProfit;
}