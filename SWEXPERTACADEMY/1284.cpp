#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string.h>

using namespace std;

int main() {
	int testCase;
	cin >> testCase;

	for (int t = 0; t < testCase; t++) {
		int P; // A ȸ�� ���� �� ���
		int Q, R, S; // B ȸ�� �⺻ ���, �ʰ� ����, �ʰ��� ���� �� ���
		int W; // ������ ��뷮

		cin >> P >> Q >> R >> S >> W;

		int pay1 = P * W;

		int pay2;

		if (W <= R)
			pay2 = Q;
		else
			pay2 = Q + S * (W - R);

		int result = (pay1 > pay2) ? pay2 : pay1;

		cout << "#" << t + 1 << " " << result << endl;
	}
}