#include <string.h>
#include <stdlib.h>

#define max(a, b) ((a) > (b) ? (a) : (b))

/**
 * Reverse a string in place between start and end indices
 */
void reverseString(char* str, int start, int end) {
	char temp;

	while (start < end) {
		temp = str[start];
		str[start] = str[end];
		str[end] = temp;
		start++;
		end--;
	}
}

/**
 * LeetCode 67: Add Binary
 * 
 * Given two binary strings a and b, return their sum as a binary string.
 * 
 * Algorithm:
 * 1. Process from right to left (lowest bit to highest bit)
 * 2. Add corresponding bits with carry, similar to decimal addition
 * 3. Store result from lowest to highest bit, then reverse
 * 4. Handle edge case: remove leading zeros (e.g., "0" + "0" = "0", not "00")
 * 
 * Time Complexity: O(max(n, m)) where n, m are lengths of a and b
 * Space Complexity: O(max(n, m)) for result string
 * 
 * Example:
 * Input: a = "11", b = "1"
 * Output: "100"
 */
char* addBinary(char* a, char* b) {
	int digitA, digitB, sum, carry;
	int lenA, lenB, maxLen, i;
	char *result;

	lenA = strlen(a);
	lenB = strlen(b);
	maxLen = max(lenA, lenB);
	maxLen = maxLen + 1;  // Reserve space for potential carry

	result = (char *)malloc((maxLen + 1) * sizeof(char));
	memset(result, 0, (maxLen + 1) * sizeof(char));

	carry = 0;
	
	// Process from right to left (lowest bit to highest bit)
	for (i = 0; i < maxLen; i++) {
		// Read from right side of strings
		if (i < lenA)
			digitA = a[lenA - 1 - i] - '0';
		else
			digitA = 0;
		
		if (i < lenB)
			digitB = b[lenB - 1 - i] - '0';
		else
			digitB = 0;

		// Binary addition
		sum = digitA + digitB + carry;
		carry = sum / 2;
		sum = sum % 2;

		result[i] = sum + '0';
	}

	// Handle final carry
	if (carry > 0)
		result[maxLen - 1] = carry + '0';

	result[maxLen] = '\0';

	// Reverse result (we stored from low to high bit)
	reverseString(result, 0, strlen(result) - 1);
	
	// Remove leading zeros (handle "0" + "0" = "0" case)
	if (result[0] == '0' && result[1] != '\0') {
		// Shift left to remove leading zero
		i = 0;
		while (result[i + 1] != '\0') {
			result[i] = result[i + 1];
			i++;
		}
		result[i] = '\0';
	}
	
	return result;
}