## 균형 잡힌 이진 검색 트리

기본적인 이진 검색 트리에는 아래와 같은 문제점이 있다.  

만약, 숫자의 크기를 우선 순위로 갖는 트리에서 삽입이 1부터 N까지 순차적으로 삽입됐다고 가정하자.
루트 노드는 1이고, 오른쪽 아래로만 노드가 추가되며 결국에서는 높이가 N-1인 트리가 만들어진다.

이러면 트리가 ***기울어졌다(skewed)***라고 한다. 
이렇게 되면 트리 구조를 사용하는 의미가 없고,
단순 배열보다 못한 퍼포먼스를 가진다.

이 점을 해결하기 위해서, 노드들이 한 쪽으로 몰리지 않게 조정을 해주는 자료구조인 
AVL Tree, Red-Black Tree 등의 **Balanced Tree**가 존재하며, 
대부분 표준 라이브러리의 이진 검색 트리는 레드 블랙 트리를 사용해서 구현되어 있다.


## 트립  

교과서적인 자료구조인 AVL Tree, Red-Black Tree 등은 구현이 상당히 힘들다.
PS를 진행하며 구현하기에는 공수가 너무 많아 적합하지 않고 다른 대안이 사용되는데, 
많이 사용하는 것 중 대표적인 것이 Tree + Heap을 의미하는 **Treap**이다.  

트립에서는 트리의 형태가 입력 순서에 따라 결정되지 않고 난수에 의해 임의로 결정이 된다. 
그래서 노드가 한 쪽으로 쏠리지 않고 어느 정도 일정한 높이를 가지게 된다.  

트립은 새 노드가 추가될 때 마다 각 노드에 난수 생성을 통해 우선 순위를 부여하고 
이 우선 순위에 따라 트리가 생성된다.  

입력이 {1, 2, 3, 4, 5, 6, 7}이 들어오고, 
난수를 통해 임의로 생성한 우선 순위가 {37, 49, 13, 31, 65, 14, 25}라고 하자.

트립의 생성은 우선 순위가 높은 것부터 차례대로 삽입한 것이라고 생각하면 된다.

![treap](/images/treap.png)  


## 트립의 구현

``` cpp
// 노드의 구현
struct Node{
  KeyType key;
  int priority, size;
  Node *left, *right;

  // 난수를 통한 우선순위 생성
  Node(const KeyType & _key)
  : key(_key), priority(rand()), size(1), left(NULL), right(NULL)
  {}

  void setLeft(Node * newLeft){
    left = newLeft;
    calcSize();
  }

  void setRight(Node * newRight){
    right = newRight;
    calcSize();
  }

  // 사이즈 갱신
  // left나 right가 바뀔 때 갱신되며, k번째 원소나 X보다 작은 원소를 세는 연산을 쉽게 구현 가능하게 한다.
  void calcSize(){
    size = 1;
    if(left) size += left->size;
    if(right) size += right->size;
  }
};
```

이제 노드를 추가하는 연산을 구현한다. 
만약 root의 우선순위가 더 높다면 node는 root 아래로 들어가야 하며 적절한 자리를 찾아가면 된다. 
하지만 문제는 node의 우선순위가 root보다 더 높은 경우이다. 
이때는 node가 기존에 있던 root를 밀어내고 트리의 새로운 root가 되어야 한다. 
이 때 이를 구현하는 것은 기존의 트리를 node가 가진 원소를 기준으로 쪼개는 것이다.  

예를 들어, root의 원소가 삽입되는 node보다 작은 값이라고 가정하자. 
그러면 root 포함 왼쪽 자식 트리의 모든 값은 항상 node보다 작다. 
그리고 오른쪽 자식 트리를 재귀적으로 구성하여 다시 쪼개면 된다.

``` cpp
typedef pair<struct Node*, struct Node*> NodePair;

NodePair split(Node * root, KeyType key){
  if(root == NULL) return NodePair(NULL, NULL);
  
  // 루트가 더 작으면 오른쪽 서브 트리를 쪼갠다
  if(root->key < key){
    NodePair rs = split(root->right, key);
    root->setRight(rs.first);

    return NodePair(root, rs.second);
  }
  else{
    // 루트가 key 이상이면 왼쪽 서브 트리를 쪼갠다
    NodePair ls = split(root->left, key);
    root->setLeft(ls.first);

    return NodePair(ls.first, root);
  }
}

Node * insert(Node * root, Node * node){
  if(root == NULL) return node;

  if(root->priority < node->priority){
    NodePair splitted = split(root, node->key);
    node->setLeft(splitted.first);
    node->setRight(splitted.second);

    return node;
  }
  else if(node->key < root->key) root->setLeft(insert(root->left, node));
  else root->setRight(insert(root->right, node));
}
```

``` cpp
// max(a) < min(b) 일 때 합친다
Node * merge(Node * a, Node * b){
  if(a == NULL) return b;
  if(b == NULL) return a;

  if(a->priority < b->priority){
    b->setLeft(merge(a, b->left));
    return b;
  }
  else{
    a->setRight(merge(a->right, b));
    return a;
  }
}

// key에 해당하는 노드 삭제 후 root 반환
Node * erase(Node * root, KeyType key){
  if(root == NULL) return root;

  if(root->key == key){
    Node * ret = merge(root->left, root->right);
    delete root;
    return ret;
  }

  if(key < root->key) root->setLeft(erase(root->left, key));
  else root->setRight(erase(root->right, key));

  return root;
}
```

## 응용 가능 문제 

이렇게 균형 잡힌 이진 트리를 트립으로 구현하였을 때 해결할 수 있는 문제는 다음과 같다.

1. k번째 원소 찾기  

    각 Node에서 서브 트리의 크기인 size를 저장해 두기 때문에 쉽게 구현 가능 하다. 
    왼쪽 서브 트리의 크기를 l이라고 했을 때 아래 성질을 만족한다.

    - $ k \leq l $ : k번째 노드는 왼쪽 서브 트리에 속해 있다.
    - $ k = l + 1 $ : 루트가 k번째 노드이다.
    - $ k > l + 1 $ : k번째 노드는 오른쪽 서브트리에서 k-l-1번째 노드가 된다.

    ``` cpp
    Node * kth(Node * root, int k){
      int leftSize = 0;
      if(root->left != NULL) leftSize = root->left->size;
      if(k <= leftSize) return kth(root->left, k);
      if(k == leftSize + 1) return root;

      return kth(root->right, k - leftSize - 1);
    }
    ```

2. X보다 작은 원소 세기  

    특정 범위 [a, b)가 주어질 때 범위 안의 원소 숫자를 계산할 수 있다. 
    만약 root의 원소가 X보다 같거나 더 크면 root와 오른쪽 서브 트리에 있는 원소들은 모두 X 이상이므로 
    왼쪽 서브 트리의 원소들만 재귀적으로 세서 반환하면 된다.

    ``` cpp
    int countLessThan(Node * root, KeyType key){
      if(root == NULL) return 0;
      if(root->key >= key) return countLessThan(root->left, key);
      int ls = (root->left ? root->left->size : 0);
      return ls + 1 + countLessThan(root->right, key);
    }
    ```
