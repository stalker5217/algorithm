문자열 더미(Haystack)속에, 특정 문자열(Needle)이 존재하는지 찾는 알고리즘을 알아본다.  
만약, 'avava'에서 'ava'를 찾는다면 시작 인덱스인 0과 2를 반환한다.

## Naive algorithm  

먼저 문자열을 찾는 가장 쉬운 방법은 모든 인덱스에서 직접 비교해보며 탐색하는 방법이다.

``` cpp
vector<int> naiveSearch(const string& H, const string& N){
    vector<int> ret;

    for(int begin = 0 ; begin + N.size() <= H.size() ; ++begin){
        bool matched = true;
        for(int i = 0 ; i < N.size() ; i++){
            if(N[i] != H[begin + i]){
                matched = false;
                break;
            }
        }

        if(matched) ret.push_back(begin);
    }

    return ret;
}
```

이 방법은 특정 입력에 대해 비효율적으로 동작한다.
만약, H와 N이 하나의 알파벳으로 구성된 긴 문자열이라면 가능한 모든 위치가 답이 되고
수행 시간은 O(|N| * |H|)가 된다.  
C++의 find, JAVA의 indexOf 등이 이 알고리즘을 사용한다.


## KMP 알고리즘

|Index|0|1|2|3|4|5|6|7|8|9|10|
|N|a|b|b|a|a|a|b|a|b|a|
|H|a|b|a||||||||

만약 이와 같은 비교를 한다고 하면, 순차적으로
H[0 ~ 1]과 N[0 ~ 1]가 일치하는 것을 확인하였으나 H[2]와 N[2]가 다르기 때문에 다음 탐색으로 넘어가게 된다.

하지만 H의 시작이 'a'이라는 것을 알고, N[1]이 'b'라는 것을 탐색한 적이 있는데 굳이 1번 인덱스부터 탐색을 해야될까? 
이처럼 KMP 알고리즘은 문자열 비교 과정에서 얻은 정보로 불필요한 부분의 탐색을 줄인다.  


## 다음 탐색 위치 찾기

|Index|...|i|i+1|i+2|i+3|i+4|i+5|
|H|...|a|b|c|a|b|d|a|b|c|
|N, begin = i|...|a|b|c|a|b|c|
|N, begin = i + 3|...||||a|b|c|a|b|c|

index i에서 비교를 했을 때 N은 N[0:4]까지 비교를 하고 N[5]에서 불일치를 확인했을 때,
i+1이 아닌 i+3으로 이동을 해서 비교를 한다.

KMP에서는 N에서 각 N[0:i]에 대해 접두사도 되고, 접미사도 될 수 있는 문자열의 최대 길이 정보를 유지하는데 이를 실패 함수라고 한다.
위 경우 처럼 N[0:4]이라면, N[0:1]은 'ab'이고, N[3:4]도 'ab'이기에 길이 정보는 2이다.

Example) N = "aabaabac"

|i|N[0:i]|접두사 이면서 접미사인 최대 문자열|pi[i]|
|:--|:--|:--|:--|
|0|a|-|0|
|1|aa|a|1|
|2|aab|-|0|
|3|aaba|a|1|
|4|aabaa|aa|2|
|5|aabaab|aab|3|
|6|aabaaba|aaba|4|
|7|aabaabac|-|0|

이 정보를 활용하여 다음 promising한 위치를 정한다.

<br/>

``` cpp

vector<int> kmpSearch(const string & H, const string & N){
  int n = H.size();
  int m = N.size();

  // pi 정보
  vector<int> ret;
  vector<int> pi = getPartialMatch(N);

  int begin = 0;
  int matched = 0;

  while(begin <= n - m){
    // matched index에서 서로 일치하는 경우
    if(matched < m && H[begin+matched] == N[matched]){
      ++matched;

      // 전체 탐색에 성공한 경우
      if(matched == m){
        ret.push_back(begin);
      }
    }
    else{
      // matched가 0인 경우에는 예외로 다음 칸에서 탐색
      if(matched == 0) ++begin;
      else{
        // pi 정보로 다음 promising한 위치 잡기
        begin += matched - pi[matched-1];
        // 시작 위치를 옮겨도 이미 pi[matched-1]만큼은 일치함. 
        matched = pi[matched-1];
      }
    }
  }

  return ret;
}


```

## pi 테이블 구하기

위 코드에서 배열 pi를 구하는 것은 아직 다루지 않았다.  
먼저 naive하게 구현을 해본다면 다음과 같은 코드를 구현할 수 있다.

``` cpp
// O(N^3)
vector<int> getPartialMatchNaive1(const string & N){
  int m = N.size();
  vector<int> pi(m, 0);

  for(int i = 0 ; i < m ; i++){
    string prefix = N.substr(0, i+1);
    for(int j = prefix.size() - 1 ; j >= 1 ; j--){
        string pre_suf = prefix.substr(0, j);
        bool isMatched = true;
        for(int k = 0 ; k < pre_suf.size() ; k++){
            if(!(pre_suf[k] == prefix[k] && pre_suf[k] == prefix[prefix.size() - pre_suf.size() + k])){
                isMatched = false;
                break;
            }
        }
        if(isMatched){
            pi[i] = j;
            break;
        }
    }
  }

  return pi;
}

// O(N^2)
vector<int> getPartialMatchNaive2(const string & N){
  int m = N.size();
  vector<int> pi(m, 0);

  for(int begin = 1 ; begin < m ; begin++){
    for(int i = 0 ; i + begin < m ; i++){
      if(N[begin+i] != N[i]) break;

      // break을 타지 않았다는 것은 (i+1)만큼 문자열이 일치함.
      pi[begin + i] = max(pi[begin + i], i + 1);
    }
  }

  return pi;
}
```


위 코드에서 시간 복잡도를 O(N)으로 줄일수 있는 방법이 존재한다.
실패 함수 pi를 구하는 것을 포스트의 뒤에 두는 이유는 이 pi를 구할 때도 KMP 알고리즘을 활용할 수 있기 때문이다. 


``` cpp
vector<int> getPartialMatch(const string & N){
  int m = N.size();
  vector<int> pi(m, 0);

  // KMP algorithm으로 index 1부터 자기 자신 탐색 시작
  int begin = 1, matched = 0;
  while(begin + matched < m){
    if(N[begin + matched] == N[matched]){
      ++matched;
      pi[begin + matched - 1] = matched;
    }
    else{
      if(matched == 0) ++begin;
      else{
        // 이 시점에서 matched - 1는 항상 계산되어 있음이 보장된다.
        begin += matched - pi[matched - 1];
        matched = pi[matched - 1];
      }
    }
  }

  return pi;
}

```


## 다른 형태의 KMP 알고리즘

좀 더 깔끔한 방식으로 KMP 알고리즘을 구현한 것이며, 위의 코드와 로직은 일치한다.

``` cpp
vector<int> kmpSearch2(const string & H, const string & N){
  int n = H.size();
  int m = N.size();

  vector<int> ret;
  vector<int> pi = getPartialMatch(N);

  int matched = 0;

  // 탐색 시작
  // 여기서 i는 begin + matched와 일치한다.
  for(int i = 0 ; i < n ; i++){

    // 불일치할 경우, matched를 pi[matched - 1]로 줄인다.
    while(matched > 0 && H[i] != N[matched]) matched = pi[matched - 1];

    // 일치하는 경우
    if(H[i] == N[matched]){
      ++matched;
      if(matched == m){
        ret.push_back(i - m + 1);
        matched = pi[matched - 1];
      }
    }
  }

  return ret;
}
```