## union-find  

상호 배타적 집합, 그러니까 특정 집합을 공통 원소가 없는 부분 집합들로 나눈 것을 disjoint set이라고 한다. 
그리고 이를 표현하는 자료구조는 **union-find**가 가장 대표적이다.  

이름처럼 이 자료구조는 아래와 같은 연산을 지닌다.
- union : 두 원소 a, b가 주어질 때 이들이 속한 두 집합을 하나로 합친다.
- find : 어떤 원소 a가 주어질 때 이 원소가 속한 집합을 반환한다.

union-find는 보통 트리로 구성된다. 즉 이는 하나의 집합은 하나의 트리가 되는 것을 의미한다. 

![union_find](/images/union_find.png)  

두 원소가 같은 집합에 속해 있는지 확인하는 가장 직관적인 방법은 각 원소가 포함된 트리의 루트가 같은지 비교하는 것이다. 
이러한 연산을 구현하기 위해서는 각 노드는 자신의 부모 노드에 대한 포인터를 가지고 있어야 한다. 
예제에서 화살표의 방향을 표기한 이유이다.  

union 연산도 간단하다. 두 개의 루트를 찾고 하나를 다른 하나 밑에 집어 넣으면 그만이다.

``` cpp
struct NaiveDisjointSet{
	vector<int> parent;
	NaiveDisjointSet(int n)
	: parent(n)
	{
		for(int i = 0 ; i < n ; i++){
			parent[i] = i;
		}
	}

  int find(int u) const {
    if(u == parent[u]) return u;
    return find(parent[u]);
  }

  // u, v가 속해 있는 각각의 트리를 합친다
  void merge(int u, int v){
    u = find(u);
    v = find(v);

    if(u == v) return;
    parent[u] = v;
  }
};
```

## union-find 최적화  

위의 naive한 구현의 문제점은 트리가 한 쪽으로 쏠리는 현상이 발생할 수 있다. 
이를 해결하기 위한 방법으로는 두 트리를 합칠 때 항상 높이가 더 작은 트리를 높은 트리 밑에 집어 넣어 
트리의 높이가 높아지는 상황을 방지하는 것이다.  

또한 가능한 최적화 중 경로 압축이 있다. 
```find(u)```를 통해 일단 u를 조회하여 트리의 루트를 찾았다고 가정하면, 
parent[u]를 루트로 바꿔버린다면 다음에 동일한 호출에 있어서는 경로를 따라 갈 필요 없이 
바로 루트에 도달하게 된다. 

``` cpp
struct OptimizedDisjointSet{
  vector<int> parent;
  vector<int> rank;

  OptimizedDisjointSet(int n)
  : parent(n), rank(n, 1)
  {
    for(int i = 0 ; i < n ; i++){
      parent[i] = i;
    }
  }

  // 경로 압축 최적화
  int find(int u) {
    if(u == parent[u]) return u;
    else return parent[u] = find(parent[u]);
  }

  // u, v가 속해 있는 각각의 트리를 합친다
  void merge(int u, int v){
    u = find(u);
    v = find(v);

    if(u == v) return;
    if(rank[u] > rank[v]) swap(u, v);
    parent[u] = v;
    if(rank[u] == rank[v]) ++rank[v];
  }
};
```