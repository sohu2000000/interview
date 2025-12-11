/*
 * LeetCode 427: 建立四叉树
 * 
 * 问题描述：
 * 给定一个 n×n 的二维网格（grid），每个值为 0 或 1
 * 用四叉树表示这个网格
 * 
 * 四叉树定义：
 * - 叶子节点：区域内所有值相同
 * - 非叶子节点：区域内有不同值，分成4个子区域
 * 
 * 示例：
 * grid = [[1,1,0,0],
 *         [1,1,0,0],
 *         [0,0,1,1],
 *         [0,0,1,1]]
 * 
 * 分割成4个2×2区域，每个区域值相同，都是叶子节点
 * 
 * 核心思路：
 * - 分治递归算法
 * - 检查区域是否值全相同（叶子节点）
 * - 否则分成4个子区域，递归构建
 */

/*
// Definition for a QuadTree node.
class Node {
public:
    bool val;
    bool isLeaf;
    Node* topLeft;
    Node* topRight;
    Node* bottomLeft;
    Node* bottomRight;
    
    Node() {
        val = false;
        isLeaf = false;
        topLeft = NULL;
        topRight = NULL;
        bottomLeft = NULL;
        bottomRight = NULL;
    }
    
    Node(bool _val, bool _isLeaf) {
        val = _val;
        isLeaf = _isLeaf;
        topLeft = NULL;
        topRight = NULL;
        bottomLeft = NULL;
        bottomRight = NULL;
    }
    
    Node(bool _val, bool _isLeaf, Node* _topLeft, Node* _topRight, Node* _bottomLeft, Node* _bottomRight) {
        val = _val;
        isLeaf = _isLeaf;
        topLeft = _topLeft;
        topRight = _topRight;
        bottomLeft = _bottomLeft;
        bottomRight = _bottomRight;
    }
};
*/

class Solution {
public:
	/*
	 * 检查区域是否为叶子节点（所有值相同）
	 * @param grid: 网格
	 * @param startRow: 起始行
	 * @param startCol: 起始列
	 * @param regionSize: 区域大小
	 * @return: 所有值相同返回true
	 */
	bool isUniformRegion(vector<vector<int>>& grid, int startRow, int startCol, int regionSize) {
		int targetValue = grid[startRow][startCol];
		for (int i = startRow; i < startRow + regionSize; i++) {
			for (int j = startCol; j < startCol + regionSize; j++) {
				if (grid[i][j] != targetValue) {
					return false;
				}
			}
		}
		return true;
	}

	/*
	 * 递归构建四叉树
	 * @param grid: 网格
	 * @param startRow: 当前区域起始行
	 * @param startCol: 当前区域起始列
	 * @param regionSize: 当前区域大小
	 * @return: 四叉树节点
	 * 
	 * 算法步骤：
	 * 1. 终止条件：size == 0
	 * 2. 检查是否叶子节点（所有值相同）
	 * 3. 如果是叶子：创建叶子节点
	 * 4. 如果不是：分成4个子区域，递归构建
	 */
	Node* buildQuadTreeRecursive(vector<vector<int>>& grid, int startRow, int startCol, int regionSize) {
		Node *topLeftChild = NULL, *topRightChild = NULL;
		Node *bottomLeftChild = NULL, *bottomRightChild = NULL;
		int middleRow = 0, middleCol = 0;

		/* 终止条件 */
		if (regionSize == 0) {
			return NULL;
		}

		/* 检查是否所有值相同 */
		bool isLeaf = isUniformRegion(grid, startRow, startCol, regionSize);
		if (isLeaf) {
			/* 叶子节点：所有值相同 */
			return new Node(grid[startRow][startCol], true, topLeftChild, topRightChild, bottomLeftChild, bottomRightChild);
		}

		/* 非叶子节点：分成4个子区域 */
		int subRegionSize = regionSize / 2;
		middleRow = startRow + subRegionSize;
		middleCol = startCol + subRegionSize;

		/* 递归构建4个子区域 */
		topLeftChild = buildQuadTreeRecursive(grid, startRow, startCol, subRegionSize);
		topRightChild = buildQuadTreeRecursive(grid, startRow, middleCol, subRegionSize);
		bottomLeftChild = buildQuadTreeRecursive(grid, middleRow, startCol, subRegionSize);
		bottomRightChild = buildQuadTreeRecursive(grid, middleRow, middleCol, subRegionSize);

		return new Node(false, false, topLeftChild, topRightChild, bottomLeftChild, bottomRightChild);
	}

	/*
	 * 主函数：构建四叉树
	 * @param grid: 网格
	 * @return: 四叉树根节点
	 * 
	 * 时间复杂度: O(n² × logn)
	 * 空间复杂度: O(logn)，递归栈
	 */
	Node* construct(vector<vector<int>>& grid) {
		return buildQuadTreeRecursive(grid, 0, 0, grid.size());
	}
};

/*
 * ==================== 算法详解 ====================
 * 
 * 核心思想：分治递归
 * 
 * 四叉树结构：
 * - 叶子节点：区域内值全相同
 * - 非叶子节点：有4个子节点
 * 
 * 分割方式（2×2示例）：
 * +-------+-------+
 * | TL    | TR    |  TL: topLeft
 * |       |       |  TR: topRight
 * +-------+-------+  BL: bottomLeft
 * | BL    | BR    |  BR: bottomRight
 * +-------+-------+
 * 
 * 递归过程：
 * 1. 检查整个区域
 * 2. 如果值全相同 → 叶子节点
 * 3. 否则分成4个子区域，递归构建
 * 
 * 示例：4×4网格全为1
 * - 检查：所有值相同
 * - 返回：叶子节点(val=1, isLeaf=true)
 * 
 * 示例：4×4网格，左上1，其他0
 * - 检查：值不同
 * - 分割成4个2×2
 * - 递归构建每个子区域
 */