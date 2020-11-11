## bit  

컴퓨터에서는 모든 데이터가 0과 1 비트의 형태로 존재한다.
이러한 특징을 이용하여 유용한 작업을 할 수 있다.

Bit를 사용한 operation의 장점  
- 빠르다.
- 메모리 사용량이 적다.
- 간결한 표현이 가능하다.

## bit operation

|연산 종류|표현|
|:-------|:---|
|AND|a & b|
|OR|a \| b|
|XOR|a ^ b|
|NOT| ~a |
|left bit shift| a << 1 |
|right bit shift| a >> 1 |


비트 연산시 주의해야할 점

``` cpp

// 1. 우선순위
// 순차적인 연산을 기대하지만,
// bit 연산은 == 연산보다 우선 순위가 낮다.
// 아래 연산은 (6 & 1)로 변환된다. 
int result = (6 & 4 == 4);

// 연산 우선 순위는 괄호를 사용하여 명시적으로 핸들링한다.
int result = ((6 & 4) == 4);


// 2. 비트의 크기
// b번째 bit가 켜져있는지 확인하는 function
// 상수 1은 32비트 데이터로 취급되기 때문에 b가 32 이상이면 에러
bool isBitSet(unsigned long long a, int b){
	return (a & (1 << b)) > 0;
}

// 명시적으로 unsigned long long임을 알린다
bool isBitSet(unsigned long long a, int b){
	return (a & (1ull << b)) > 0;
}
```

## Bit를 사용한 집합 표현

``` cpp
// 크기가 10인 집합을 표현한다.

// 10개 bit 모두 on
// bit는 0번에서 9번으로 표현한다.
int set = (1 << 10) - 1;
int p = 3;

// p번 비트 존재 여부 확인
if(set & (1 << p)) cout << "exist";

// p번 비트 제거
set &= ~(1 << p);

// p번 비트 추가
set |= (1 << p);

// p번 비트 토글
// 켜져 있으면 끄고, 꺼져 있으면 킴
set ^= (1 << p);

// 집합의 크기 구하기
int copy = set;
int size = 0;
while(copy > 0){
	if(copy & 1 == 1) size++;
	copy /= 2;
}

// 최소 원소 찾기
// 2의 보수를 활용한 방법
// set을 다음과 같이 가정하면
// 00000000 00000000 00000011 00010100
// 11111111 11111111 11111100 11101100
int min = (set & -set);

// 최소 원소 지우기
set &= (set - 1);

// 집합 순회하기
for(int subSet = set ; subSet ; subSet = ((subSet - 1) & set)){
	...
}