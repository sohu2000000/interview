/*
The string "PAYPALISHIRING" is written in a zigzag pattern on a given number of rows like this: (you may want to display this pattern in a fixed font for better legibility)

P   A   H   N
A P L S I I G
Y   I   R
And then read line by line: "PAHNAPLSIIGYIR"

Write the code that will take a string and make this conversion given a number of rows:

string convert(string s, int numRows);
 

Example 1:

Input: s = "PAYPALISHIRING", numRows = 3
Output: "PAHNAPLSIIGYIR"
Example 2:

Input: s = "PAYPALISHIRING", numRows = 4
Output: "PINALSIGYAHRPI"
Explanation:
P     I    N
A   L S  I G
Y A   H R
P     I
Example 3:

Input: s = "A", numRows = 1
Output: "A"
 

Constraints:

1 <= s.length <= 1000
s consists of English letters (lower-case and upper-case), ',' and '.'.
1 <= numRows <= 1000
 */

char* convert(char* s, int numRows) {
	char *result, *rows[numRows];
	int len, i, row, direct, *rowPos;

	if (numRows == 1) return s;
	len = strlen(s);

	// 为每一行分配内存（+1为\0留空间）
	for (i = 0; i < numRows; i++) {
		rows[i] = (char *)malloc((len + 1) * sizeof(char));
		rows[i][0] = '\0';  // 初始化为空字符串
	}

	// 为结果分配内存（+1为\0留空间）
	result = (char *)malloc((len + 1) * sizeof(char));
	result[0] = '\0';  // 初始化为空字符串

	// 为每一行的写入位置分配内存
	rowPos = (int *)calloc(numRows, sizeof(int));

	// 遍历字符串，按Z字形分配到各行
	row = 0; 
	direct = 1;  // 修复：初始方向向下
	
	for (i = 0; i < len; i++) {
		// 将字符添加到当前行
		rows[row][rowPos[row]++] = s[i];
		
		// 在顶部或底部时改变方向
		if (row == 0) {
			direct = 1;  // 到顶部，向下
		} else if (row == numRows - 1) {
			direct = -1;  // 到底部，向上
		}
		
		row += direct;
	}

	// 为每行添加字符串结束符
	for (i = 0; i < numRows; i++) {
		rows[i][rowPos[i]] = '\0';
	}

	// 将每一行拼接成结果
	for (i = 0; i < numRows; i++) {
		strcat(result, rows[i]);
	}

	// 释放内存
	free(rowPos);
	for (i = 0; i < numRows; i++) {
		free(rows[i]);
	}
	// 修复：不要free(rows)，因为rows是VLA（栈上分配）

	return result;
}