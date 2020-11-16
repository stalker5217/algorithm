## 무식하게 풀기  

전산학에서 brute force란 가능한 방법을 전부 만들어 보는 알고리즘들을 가리켜서 
흔히 **완전 탐색(Exhaustive Search)**라고 부른다. 그리고 이 완전 탐색의 소요 시간은 만들어지는 경우의 수에 비례한다.  

자료 구조에 따라 구분을 하자면 선형 자료 구조를 대상으로는 흔히 Permutation, Combination이 대표적이며, 
그래프를 대상으로는 DFS, BFS가 대표적인 완전 탐색의 유형이다.

### Permutation  

**순열**을 의미하며 서로 다른 N개의 원소를 일렬로 줄 세운 경우의 수(N!)를 나타낸다. 

``` cpp
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void permutation(vector<int> & arr, const int curPos){
	if(curPos == arr.size()){
		for(int el : arr)
 cout << el << " ";
		cout << "\n";
		return;
	}

	for(int i = curPos ; i < arr.size() ; i++){
		swap(arr[i], arr[curPos]);
		permutation(arr, curPos + 1);
		swap(arr[i], arr[curPos]);
	}
}

int main(){
	vector<int> arr = {1, 2, 3, 4, 5};
	permutation(arr, 0); // 5! 개의 출력
	
	return 0;
}
```

### Combination  

**조합**을 의미하며 서로 다른 N개의 원소에서 R개의 원소를 순서 없이 골라낸 경우(nCk)의 수를 나타낸다.

``` cpp
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void combination(vector<int> & arr, vector<int> & picked, const int k, const int curPos){
	if(k == 0){
		for(int el : picked) cout << el << " ";
		cout << "\n";
		return;
	}

	for(int i = curPos ; i <= arr.size() - k ; i++){
		picked.push_back(arr[i]);
		combination(arr, picked, k - 1, i + 1);
		picked.pop_back();
	}
}

int main(){
	vector<int> arr = {1, 2, 3, 4, 5};
	vector<int> picked; 
	combination(arr, picked, 2, 0); // 5C2개 출력
	
	return 0;
}
```