## Prim Algorithm  

프림 알고리즘은 최소 비용 스패닝 트리를 만드는 알고리즘 중 하나이다.  
트리를 구성하는데 있어 크루스칼 알고리즘과 똑같이 작은 가중치를 가지는 간선들을 선택해가는 것이 유사하다. 
다른 점은 크루스칼 알고리즘은 전체 그래프에서 가중치가 낮은 간선들을 선택하며 산발적으로 구성해 가는 반면, 
프림 알고리즘은 특정한 정점 하나를 잡고 인접한 간선 중에서 후보를 고려하여 스패닝 트리를 만들어 나간다.

> 크루스칼 알고리즘과 같은 논리로 알고리즘의 정당성을 설명할 수 있다.

### 동작 과정 확인 

![minimum_spanning_tree_prim](/images/minimum_spanning_tree_prim.png)

<br/>

하나의 정점에서 알고리즘이 시작하며 이미 선택된 정점과 간선은 굵은 실선, 현재 후보가 되는 간선들은 점선으로 표현했다. 
알고리즘이 동작하면서 이미 선택된 정점을 연결하는 간선들은 후보에서 제외된다.

### 코드 구현  

``` cpp
const int MAX_V = 100;
const int INF = 987654321;

// 정점의 개수
int V;

// 인접 리스트
vector<pair<int, int> > adj[MAX_V];

int prim(vector<pair<int, int> > & selected){
    selected.clear();

    // 정점이 Spanning Tree에 포함되어 있는지 여부
    vector<bool> added(V, false);

    // 트리에 인접한 간선 중 해당 정점에 닿는 가중치가 가장 작은 간선의 정보를 저장
    vector<int> minWeight(V, INF);
    vector<int> parent(V, -1);

    int ret = 0;

    minWeight[0] = 0;
    parent[0] = 0;

    for(int iter = 0 ; iter < V ; iter++){
        // 다음 추가할 정점 u를 찾는다.
        int u = -1;
        for(int v = 0 ; v < V ; v++){
            if(!added[v] && (u == -1 || minWeight[u] > minWeight[v])) u = v;
        }

        if(parent[u] != u) selected.push_back({parent[u], u});

        ret += minWeight[u];
        added[u] = true;

        // u에 인접한 간선을 검사한다.
        for(int i = 0 ; i < adj[u].size() ; i++){
            int v = adj[u][i].first;
            int weight = adj[u][i].second;

            if(!added[v] && minWeight[v] > weight){
                parent[v] = u;
                minWeight[v] = weight;
            }
        }
    }

    return ret;
}   
```

> Time Complexity는 $ O(\|V\|^2 + \|E\|) $ 이며, 모든 정점들 사이에 간선이 있는 경우 $ \|V\|^2 \approx \|E\| $ 로 구성되므로 크루스칼 보다 빠르게 동작할 수 있다.