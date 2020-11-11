## suffix array  

suffix array(접미사 배열)은 다양한 문자열 문제를 푸는데 사용될 수 있다.
suffix array는 말 그대로 어떤 문자열 S에서 나올 수 있는 모든 접미사를 사전 순으로 정리해 놓은 것이며,
정렬되어 있기 때문에 이분탐색을 통한 문자열 검색이 가능하다.

ex) S = "algorithm"

|array_index|start_index|S[start_idndex...]|
|:--|:--|:--|
|0|0|algorithm|
|1|2|gorithm|
|2|7|hm|
|3|5|ithm|
|4|1|lgorithm|
|5|8|m|
|6|3|orithm|
|7|4|rithm|
|8|6|thm|

## 맨버-마이어스 알고리즘

접미사배열을 구할 때, 접미사를 모두 배열에 넣고 각 언어의 내장 sort 함수를 사용하여 구할 수도 있지만,
O(n) 안으로 동작하는 맨버-마이어스 알고리즘을 알아본다.

``` cpp

vector<int> getSuffixArray(const string & s){
    int n = s.size();
    int t = 1;

    // group 배열
    // 각 접미사들을 [0:t]를 기준으로 정렬했을 때 속하는 그룹을 나타냄
    vector<int> group(n+1);
    // 초기 세팅에서는 그냥 문자열 character를 그대로 사용한다.
    // 0부터 시작하는 값은 아니지만 대소 관계 비교에는 문제가 없다.
    for(int i = 0 ; i < n ; ++i) group[i] = s[i];
    // group은 길이가 0인 접미사도 포함하며 -1로 세팅한다.
    group[n] = -1;

    // return array
    vector<int> ret(n);
    for(int i = 0 ; i < n ; ++i) ret[i] = i;

    
    while(t < n){
        // compare 함수
        auto compare = [&group, t](int a, int b){
            // 첫 t글자가 다르면 판단 가능
            if(group[a] != group[b]) return group[a] < group[b];
            // 아니면 s[a+t:]와 s[b+t:]를 비교한다.
            // group[a] == group[b]이므로 두 길이는 t 이상임을 알 수 있다.
            // a + t, b + t 모두 n보다 작으므로 out of index는 없다.
            return group[a+t] < group[b+t];
        };
        sort(ret.begin(), ret.end(), compare);

        t *= 2;
        if(t >= n) break;

        vector<int> newGroup(n+1);
        newGroup[n] = -1;
        newGroup[ret[0]] = 0;
        for(int i = 1 ; i < n ; ++i){
            if(compare(ret[i-1], ret[i])) newGroup[ret[i]] = newGroup[ret[i-1]] + 1;
            else newGroup[ret[i]] = newGroup[ret[i-1]];
        }
        group = newGroup;
    }

    return ret;
}

```

해당 알고리즘은 S[0:1], S[0:2], S[0:4], S[0:8], ... 을 기준으로 logn번 정렬을 한다.

Example) S = "banana"

**한 글자 기준**

|0|1|2|
|:--:|:--:|:--:|
|anana <br/> ana <br/> a|banana|nana <br/> na|

<br/>

**두 글자 기준** 

|0|1|2|3|
|:--:|:--:|:--:|:--:|
|a|anana <br/> ana|banana|nana <br/> na|

<br/>

**네 글자 기준**  

|0|1|2|3|4|5|
|:--:|:--:|:--:|:--:|:--:|:--:|
|a|ana|anana|banana|na|nana|

정렬 과정에서 위의 표 처럼 각 문자열은 특정 t까지의 접미사를 기준으로 특정 group에 속하게 된다.
이 정보를 유지하고 있다면, 두 접두사의 대소비교를 상수 시간에 할 수 있다.

S[a:]와 S[b:]를 비교할 때, group[a] != group[b]라면 이미 결정된 상태이고,
group[a] == group[b]라면 group[a + t]와 group[b + t]를 비교하면 된다.


## LCP(Longest Common Prefix) array

suffix array와 함께 문제 해결에 사용되는 배열이다.
suffix array를 기반으로 구현을 할 수 있으며 [i-1]과 [i]의 가장 긴 접두사 길이를 저장한다.

Example) banana

|i|sa[i]|suffix|lcp[i]|
|:--:|:--:|:--:|:--:|
|0|5|a|x|
|1|3|ana|1|
|2|1|anana|3|
|3|0|banana|0|
|4|4|na|0|
|5|2|nana|2|


``` cpp
vector<int> getLCPArray(const string& s, vector<int> sa){
    int n = s.size();

    vector<int> lcp(n);

    // 접미사 시작 인덱스 : suffix array 인덱스
    vector<int> rank(n);
    for(int i = 0 ; i < n ; i++) rank[sa[i]] = i;

    int matched = 0;
    for(int i = 0 ; i < n ; i++){
        int k = rank[i];
        if (k){
            for(int j = sa[k-1] ; s[i + matched] == s[j + matched] ; matched++);
            lcp[k] = matched;
 
            if (matched) --matched;
        }
    }
    return lcp;
}
```