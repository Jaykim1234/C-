// #include <iostream>
// #include <stdio.h>
// #include <vector>
// #include <utility>
// #include <algorithm>
// #include <math.h>
// #include <string.h>
// #include <queue>

// using namespace std;

// //사과의 위치 1이면 사과가 존재
// int board[101][101];

// //뱀 몸통이 있는가?
// bool Is_There_Snake[101][101];

// //방향 저장
// int dy[4] = { 0,1,0,-1 };
// int dx[4] = { 1,0,-1,0 };

// // i 시간에 방향전환을 함 
// char swift[10001];

// int main()
// {
// 	ios_base::sync_with_stdio(false);
// 	cin.tie(NULL);
// 	cout.tie(NULL);
	
// 	int board_Size, Apple_Cnt; cin >> board_Size >> Apple_Cnt;

// 	for (int i = 0; i < Apple_Cnt; i++)
// 	{
// 		int col, row; cin >> col >> row;
// 		board[col][row] = 1;
// 	}

// 	int swift_Cnt; cin >> swift_Cnt;

// 	for (int i = 0; i < swift_Cnt; i++)
// 	{
// 		int swift_Time;
// 		char Direction;
// 		cin >> swift_Time >> Direction;
// 		swift[swift_Time] = Direction;
// 	}

// 	//뱀의 몸통이 어디에 위치하고 있는가
// 	queue<pair<int,int>> snake;

// 	//처음위치 초기화
// 	snake.push({ 1,1 });
// 	Is_There_Snake[1][1] = true;

// 	//시간
// 	int answer_Time = 0;

// 	//지금 방향
// 	int now_Direction = 0;

// 	while (1)
// 	{
// 		answer_Time++;

// 		int next_col = snake.back().first + dy[now_Direction];
// 		int next_row = snake.back().second + dx[now_Direction];

// 		//벽을 부딪히면 종료
// 		if (next_col < 1 || next_col > board_Size || next_row < 1 || next_row > board_Size) break;

// 		snake.push({ next_col,next_row });

// 		//방향 변경
// 		if (swift[answer_Time] == 'D')
// 		{
// 			now_Direction += 1;
// 			if (now_Direction == 4) now_Direction = 0;
// 		}

// 		else if (swift[answer_Time] == 'L')
// 		{
// 			now_Direction -= 1;
// 			if (now_Direction == -1) now_Direction = 3;
// 		}

// 		//사과가 있으면? 사과 없어지고 꼬리 움직이지 않음
// 		if (board[next_col][next_row] == 1)
// 		{
// 			board[next_col][next_row] = 0;
// 		}
		
// 		//사과가 없으면? 꼬리 없어짐
// 		else
// 		{
// 			if (Is_There_Snake[next_col][next_row] == true) break;
// 			Is_There_Snake[snake.front().first][snake.front().second] = false;
// 			snake.pop();
// 		}

// 		//만약 원래 몸통이 있었다면? break
// 		if (Is_There_Snake[next_col][next_row] == true) break;

// 		else Is_There_Snake[next_col][next_row] = true;
	
// 	}

// 	cout << answer_Time << endl;

// 	return 0;
// }

#include <iostream>
#include <vector>
#include <queue>

#define X _FirstImpl
#define Y _SecondImpl

using namespace std;

int dx[4] = {1, -1, 0 , 0};
int dy[4] = {0, 0, 1, -1};

int N, K, L;

int map[100][100];;
char direction[10001];

queue <pair<int, int>> q;
