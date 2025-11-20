/*
You are given an absolute path for a Unix-style file system, which always begins with a slash '/'. Your task is to transform this absolute path into its simplified canonical path.

The rules of a Unix-style file system are as follows:

A single period '.' represents the current directory.
A double period '..' represents the previous/parent directory.
Multiple consecutive slashes such as '//' and '///' are treated as a single slash '/'.
Any sequence of periods that does not match the rules above should be treated as a valid directory or file name. For example, '...' and '....' are valid directory or file names.
The simplified canonical path should follow these rules:

The path must start with a single slash '/'.
Directories within the path must be separated by exactly one slash '/'.
The path must not end with a slash '/', unless it is the root directory.
The path must not have any single or double periods ('.' and '..') used to denote current or parent directories.
Return the simplified canonical path.

 

Example 1:

Input: path = "/home/"

Output: "/home"

Explanation:

The trailing slash should be removed.

Example 2:

Input: path = "/home//foo/"

Output: "/home/foo"

Explanation:

Multiple consecutive slashes are replaced by a single one.

Example 3:

Input: path = "/home/user/Documents/../Pictures"

Output: "/home/user/Pictures"

Explanation:

A double period ".." refers to the directory up a level (the parent directory).

Example 4:

Input: path = "/../"

Output: "/"

Explanation:

Going one level up from the root directory is not possible.

Example 5:

Input: path = "/.../a/../b/c/../d/./"

Output: "/.../b/d"

Explanation:

"..." is a valid name for a directory in this problem.

 

Constraints:

1 <= path.length <= 3000
path consists of English letters, digits, period '.', slash '/' or '_'.
path is a valid absolute Unix path.
 */

/**
 * 简化路径 - 栈
 * 
 * 核心思想：
 * - 用栈存储有效的目录名
 * - 遇到".."弹栈（返回上级）
 * - 遇到"."或空跳过
 * - 其他目录名入栈
 * - 最后从栈底到栈顶拼接路径
 * 
 * 时间复杂度：O(n)
 * 空间复杂度：O(n)
 */

void pushDirectory(char **dirStack, int *stackSize, char *dir) {
	dirStack[*stackSize] = dir;
	(*stackSize)++;
}

void popDirectory(char **dirStack, int *stackSize) {
	if (*stackSize > 0) {
		(*stackSize)--;
	}
}

char* simplifyPath(char* path) {
	char *pathCopy, *result, *token, *writePos;
	char **dirStack = (char **)malloc(3000 * sizeof(char *));  // 栈
	int i, resultLen, stackSize = 0;

	// 复制路径用于分割（strtok会修改原字符串）
	pathCopy = strdup(path);
	
	// ===== 步骤1：用栈处理路径组件 =====
	token = strtok(pathCopy, "/");
	while (token) {
		if (strcmp(token, "..") == 0) {
			// ".." 返回上级目录
			popDirectory(dirStack, &stackSize);
		} else if (strcmp(token, ".") != 0 && strlen(token) > 0) {
			// 有效目录名（不是"."且非空）
			pushDirectory(dirStack, &stackSize, token);
		}
		// "." 和空字符串跳过
		token = strtok(NULL, "/");
	}

	// ===== 步骤2：计算结果长度 =====
	resultLen = 1;  // 至少有根目录 "/"
	for (i = 0; i < stackSize; i++) {
		resultLen += strlen(dirStack[i]) + 1;  // 目录名 + '/'
	}
	
	// ===== 步骤3：构建结果字符串 =====
	result = (char *)malloc((resultLen + 1) * sizeof(char));
	result[0] = '/';
	writePos = result + 1;

	for (i = 0; i < stackSize; i++) {
		int dirLen = strlen(dirStack[i]);
		memcpy(writePos, dirStack[i], dirLen);
		writePos += dirLen;
		
		// 在目录间添加 '/'（最后一个目录后不加）
		if (i != stackSize - 1) {
			*writePos = '/';
			writePos++;
		}
	}

	*writePos = '\0';
	
	// 释放内存
	free(pathCopy);
	free(dirStack);
	
	return result;
}