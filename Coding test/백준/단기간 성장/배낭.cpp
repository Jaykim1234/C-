"""
평범한 배낭
시간 제한	메모리 제한	제출	정답	맞힌 사람	정답 비율
2 초	512 MB	57308	21103	13716	35.368%
문제
이 문제는 아주 평범한 배낭에 관한 문제이다.

한 달 후면 국가의 부름을 받게 되는 준서는 여행을 가려고 한다. 세상과의 단절을 슬퍼하며 최대한 즐기기 위한 여행이기 때문에, 가지고 다닐 배낭 또한 최대한 가치 있게 싸려고 한다.

준서가 여행에 필요하다고 생각하는 N개의 물건이 있다. 각 물건은 무게 W와 가치 V를 가지는데, 해당 물건을 배낭에 넣어서 가면 준서가 V만큼 즐길 수 있다. 아직 행군을 해본 적이 없는 준서는 최대 K만큼의 무게만을 넣을 수 있는 배낭만 들고 다닐 수 있다. 준서가 최대한 즐거운 여행을 하기 위해 배낭에 넣을 수 있는 물건들의 가치의 최댓값을 알려주자.

입력
첫 줄에 물품의 수 N(1 ≤ N ≤ 100)과 준서가 버틸 수 있는 무게 K(1 ≤ K ≤ 100,000)가 주어진다. 두 번째 줄부터 N개의 줄에 거쳐 각 물건의 무게 W(1 ≤ W ≤ 100,000)와 해당 물건의 가치 V(0 ≤ V ≤ 1,000)가 주어진다.

입력으로 주어지는 모든 수는 정수이다.

출력
한 줄에 배낭에 넣을 수 있는 물건들의 가치합의 최댓값을 출력한다.

예제 입력 1 

4 7
6 13
4 8
3 6
5 12

예제 출력 1 
14

"""

using namespace std; // std::cout 이거를 cout만 써도 되게 한다.

int N, K; // N 과 K를 int로 지정 
int DP[101][100001]; // DP[n][p] : n 번째 까지 선택/ m 만큼의 무게일 때, value의 최대값
int W[101];
int V[101];

int main()
{
    cin >> N >> K;

    for (int i = 1; i <= N; i++)
    cin >>W[i]>>V[i];

    for (int i =1; i <= N; i++)
    {
        for (int j = 1; j <=K; j++)
        {
            if (j - W[i] >= 0) DP[i][j] = max(DP[i-1][j], DP[i-1][j-W[i]] + V[i]]);
            else DP[i][j] = DP[i-1][j];
        }
    }
    cout <<DP[N][K];
}

using namespace std;
int N, K;
int dp[101][100001]; // dp[n][m] : n번째 물품까지 선택/m만큼의 무게일 때, value의 최대값
int main(){
    memset(dp, 0, sizeof(dp)); 
    cin >> N >> K;
    int w, v, res=0;
    for(int i=1; i<=N; i++){
        cin >> w >> v;  
        for(int j=1; j<=K; j++){   
            dp[i][j] = max(dp[i][j], dp[i-1][j]);
            if(j>=w) dp[i][j] = max(dp[i][j], dp[i-1][j-w] + v);
            res = max(res, dp[i][j]);
        }
    }
    cout << res;
    return 0;
}  

// memset : https://coding-factory.tistory.com/673