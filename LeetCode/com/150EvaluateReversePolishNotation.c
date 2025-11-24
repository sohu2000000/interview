#define MAX_SIZE 10000

int operate(int a, int b, char *op) {
	switch (*op) {
		case '+': return a + b;
		case '-': return a - b;
		case '*': return a * b;
		case '/': return a / b;
		default:  return 0;  // 理论上不会到达
	}
}

bool isOperator(char *op) {
	// 修复：操作符只有一个字符，负数有多个字符
	return strlen(op) == 1 && (*op == '+' || *op == '-' || *op == '*' || *op == '/');
}

int evalRPN(char** tokens, int tokensSize) {
	int stack[MAX_SIZE];
	int top = -1, i, left, right;

	for (i = 0; i < tokensSize; i++) {
		if (!isOperator(tokens[i])) {
			// 数字：入栈
			stack[++top] = atoi(tokens[i]);
		} else {
			// 操作符：弹出两个操作数，计算后入栈
			right = stack[top--];
			left = stack[top--];
			stack[++top] = operate(left, right, tokens[i]); 
		}
	}

	return stack[top];

}