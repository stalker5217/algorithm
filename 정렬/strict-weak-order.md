## Sort 기준

알고리즘 문제를 풀다보면 어떠한 기준을 가지고 데이터들을 정렬해야하는 경우가 많다.

``` cpp
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool compare(const int & a, const int & b);

int main() {
	vector<int> arr;

	arr.push_back(3);
	arr.push_back(2);
	arr.push_back(1);
	arr.push_back(5);
	arr.push_back(4);
	arr.push_back(1);

	sort(arr.begin(), arr.end(), compare);

	for (int i = 0; i < arr.size(); i++) {
		cout << arr[i] << " ";
	}

	return 0;
}

// 내림차순 정렬
bool compare(const int& a, const int& b) {
	if (a >= b) {
		return true;
	}
	else {
		return false;
	}
}

```

정수 내림차순 정렬을 위해 위와 같은 코드를 작성하고 실행하였다.
하지만 이 코드는 에러가 발생한다.

에러의 원인은 결론부터 보면 ```if(a>=b) return true;```이다.  
이는 a와 b가 같을 때, ```a의 우선순위가 b보다 크다```이면서 ```b의 우선순위가 a보다 크다```라는 모순을 발생시킨다.
그렇기에, 만약 a와 b의 값이 같다면 compare 함수는 반드시 false를 return 해야한다.


## Strict Weak Order

비교 함수를 작성할 때 수학적으로 만족시켜야 하는 조건이 있다.

1. **Irreflexivity, 비반사성**  
어떤 값 a에 대해서 R(a, a)는 거짓이다.
compare(a, a)는 false를 return 한다.

2. **Asymmetry, 비대칭성**  
어떤 값 a, b에 대해서 R(a, b)가 참이면 R(b, a)는 거짓이며,
R(a, b)가 거짓이면 R(a, b)는 참이다.

3. **Transitivity, 추이성**  
어떤 값 a, b, c에 대해서 R(a, b)가 참이고, R(b, c)가 참이면 R(a, c)도 참이다.

4. **Transitivity of imcomparability**  
어떤 값 a, b를 서로 비교할 수 없고 (neither ```a < b``` nor ```b < a```), b와 c를 비교할 수 없을 때 a와 c도 서로 비교할 수 없다.