#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

int main() {
	int a, b;
	char result;

	scanf("%d %d", &a, &b);

	// ����
	if (a == 1) {
		if (b == 3)
			result = 'A';
		else
			result = 'B';
	}

	// ����
	else if (a == 2) {
		if (b == 1)
			result = 'A';
		else
			result = 'B';
	}

	// ��
	else if (a == 3) {
		if (b == 2)
			result = 'A';
		else
			result = 'B';
	}

	printf("%c", result);

	return 0;
}