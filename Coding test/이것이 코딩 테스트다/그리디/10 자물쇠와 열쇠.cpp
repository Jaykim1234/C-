#include <vector>
using namespace std;

int N, M;
vector <vector<int >> board; //???

// 잠금 해제 여부 체크
bool check(vector<vector<int>> & key, int y, int x){
    bool ret = true;

    // 보드판에 열쇠 값 적용
    for(int i=y; i<y+M;i++)
        for(int j=x; j<x+M; j++)
            board[i][j] += key[i-y][j-x];

    // 좌물쇠의 모든 좌표 확인
    for (int i=M;i<M+N;i++){
        for(int j=M;j<M+N;j++){
            if(board[i][j] != 1){
                ret=false;
                break;
            }
        }
        if(!ret) break;
    }
    for (int i=y; i<y+M; i++)
        for(int j=x; j<x+M;j++)
            board[i][j] -= key[i-y][j-x];
    return ret;
}
// 열쇠 시계방향 회전
void rotate(vector<vector<int>>& key){
    vector<vector<int>> temp(M, vector<int>(M));
    for(int i=0; i<M;i++)
        for(int j=0;j<M;j++)
            temp[i][j] = key[j][M-i-j];
    for(int i=0; i<M;i++)
        for(int j=0; j<M; j++)
            key[i][j] = temp[i][j];
}

// https://yjyoon-dev.github.io/kakao/2020/12/18/kakao-lockerkey/
