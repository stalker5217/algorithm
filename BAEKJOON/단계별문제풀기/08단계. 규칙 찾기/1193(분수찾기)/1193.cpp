#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>

using namespace std;


int main(int argc, char** argv)
{
	int n;
	cin >> n;

	int sum = 1;
	int index = 1;
	while (true) {
		if (sum < n) {
			index++;
			sum += index;
		}
		else {
			// �и� ���� ���� sum
			sum -= index;
			n -= sum;
			if (index % 2 == 0) {
				cout << n << "/" << index + 1 - n;
			}
			
			else {
				cout << index + 1 - n << "/" << n;
			}
			break;
		}
	}

	return 0;
}