// #include <string>
// #include <vector>

// using namespace std;

// int M, N, Point;
// int Size = N + 2*(M -1);

// vector<vector<int>> MAP(Size, vector<int>(Size, 2));

// void Make_MAP(vector<vector<int> > lock)
// {
//     int lock_x = 0;
//     int lock_y = 0;
//     for (int i = M - 1; i < Size - (M-1);i++, lock_x++)
//     {
//         for (int j = M-1; j< Size - (M-1); j++, lock_y++)
//         {
//             MAP[i][j] = lock[lock_x][lock_y];
//             if (MAP[i][j] == 0) Point++;

//         }
//         lock_y = 0 ;               
//     }
// }


// #include <iostream>
// #include <vector>

// using namespace std;
// int n, m;

// vector<vector<int>> expandLock(vector<vector<int>> lock){
//     vector<vector<int>> newLock(n*3, vector<int>(n*3));
    
//     for(int i=0; i<n; i++){
//         for(int j =0; j<n; j++){
//             newLock[i][j] = lock[i][j];
//             newLock[n+i][j] = lock[i][j];
//             newLock[n*2+i][j] = lock[i][j];

//             newLock[i][n+j] = lock[i][j];
//             newLock[n+i][n+j] = lock[i][j];
//             newLock[n*2+i][n+j] = lock[i][j];
            
//             newLock[i][2*n+j] = lock[i][j];
//             newLock[n+i][2*n+j] = lock[i][j];
//             newLock[n*2+i][2*n+j] = lock[i][j];
//         }
//     }
    
//     return newLock;
// }

// #include <iostream>
// #include <vector>
 
// #define M 3
// #define N 4
 
// int main()
// {
//     // Vector 요소를 채울 기본값을 지정합니다.
//     int default_value = 1;
 
//     // 채우기 생성자를 사용하여 2차원 Vector 초기화
//     // 주어진 기본값으로
//     std::vector<std::vector<int>> matrix(M, std::vector<int>(N, default_value));
 
//     // 2차원 Vector를 출력
 
//     return 0;
// }
// resizing vector

// #include <iostream>
// #include <vector>

// int main ()
// {
//   std::vector<int> myvector;

//   // set some initial content:
//   for (int i=1;i<10;i++) myvector.push_back(i);

//   myvector.resize(5);
//   myvector.resize(8,100);
//   myvector.resize(12);

//   std::cout << "myvector contains:";
//   for (int i=0;i<myvector.size();i++)
//     std::cout << ' ' << myvector[i];
//   std::cout << '\n';

//   return 0;
// }


// vector <vector<int> > rotateKey(vector<vector<int> > key){
//     vector <vector<int> > rotatedKey(m, vector<int>(m));
    
//     for(int i=0; i<m; i++){
//         for(int j =0; j<m; j++){
//             rotatedKey[i][j] = key[m-j-1][i];
//         }
//     }
    
//     return rotatedKey;
// }


// #include <iostream>

// int main() {
//     std::cout << "Hello" << std::endl ;
//     return 0;
// }

// int Sum_Key_And_Lock(int Sx, int Sy, vector<vector<int>> key)
// {
//     int x_Idx = 0;
//     int y_Idx = 0;
//     int Check = 0;
//     for (int x = Sx; x < Sx + M; x++, x_Idx++)
//     {
//         for (int y = Sy; y < Sy + M; y++, y_Idx++)
//         {
//             if (MAP[x][y] == 1 && key[x_Idx][y_Idx] == 1) return -1;
//             if (MAP[x][y] == 0 && key[x_Idx][y_Idx] == 0) return -1;
//             if (MAP[x][y] == 0 && key[x_Idx][y_Idx] == 1) Check++;
//         } 
//         y_Idx = 0;
//     }
//     return Check;
// }

// #include <iostream>
// using std::cout; // cout으로 범위를 좁혀 네임스페이스 지정
 
// int main()
// {
//     int i = 0;
//     cin >> i; // 컴파일 오류 발생
//     cout << i; 
// }

// class vector<bool>::reference {
//   friend class vector;
//   reference();  // public 생성자가 아니다
//  public:
//   ~reference();
//   operator bool() const;                     // bool 로 캐스팅 한다.
//   reference& operator=(const bool x);        // bool 을 대입
//   reference& operator=(const reference& x);  // 비트로 대입
//   void flip();  // 비트값 반전 (0 -> 1, 1 -> 0)
// // }

ue;
}
// #include <iostream>
// #include <vector>
// using namespace std;
// int main() {
//   unsigned int i;  // 위에서 설명한 생성자들을 차례대로 오버로딩함 vector<int>
//                    // first;                                // int 를 보관할 빈
//                    // 벡터 생성    vector<int> second (4,100); // 값이 100 인
//                    // int 원소 4 개 보관    vector<int> third
//                    // (second.begin(),second.end());  // second 의 처음 ~ 끝으로
//                    // 생성    vector<int> fourth (third); // 세 번째 벡터 복사본
//   // 배열을 통해서도 생성 가능하다.

//   int myints[] = {16, 2, 77, 29};
//   vector<int> fifth(myints, myints + sizeof(myints) / sizeof(int));
//   cout << "The contents of fifth are:";
//   for (i = 0; i < fifth.size(); i++) cout << " " << fifth[i];
//   cout << endl;
//   return 0;
// }

// #include <iostream>
// #include <vector>

// using namespace std;

// int main()
// {
// 	vector<vector<int>> v(6, vector<int>);
// 	for (int i = 0; i < v.size()-2; i++)
// 		cout << v[0][i] << '/';
		
// 	return 1;
// }

#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<vector<int>> lotate(vector<vector<int>> key) {
	int temp = 0;
	int size = key.size();
	for (int x = 0; x < size/2; x++) {
		for (int y = x; y < size - 1 - x; y++) {
			temp = key[x][y];
			key[x][y] = key[size - 1 - y][x];
			key[size - 1 - y][x] = key[size - 1 - x][size - 1 - y];
			key[size - 1 - x][size - 1 - y] = key[y][size - 1 - x];
			key[y][size - 1 - x] = temp;
		}
	}
	return key;
}

bool sum(int x, int y, vector<vector<int>> key, vector<vector<int>> board) {
	int ks = key.size(); //3
	int bs = board.size(); // 7
	for (int i = 0; i < ks; i++) {
		for (int j = 0; j < ks; j++) {
			board[x+i][y+j] += key[i][j];
		}
	}

	for (int i = ks-1; i < bs-ks+1; i++) {
		for (int j = ks-1; j < bs-ks+1; j++) {
			if (board[i][j] != 1) {
				return false;
			}
		}
	}

	return true;
}

bool solution(vector<vector<int>> key, vector<vector<int>> lock) {
	bool answer = false;

	int ks = key.size();
	int ls = lock.size();
	int bs = ls + ks + ks - 2;
	vector<vector<int>>board(bs,vector<int>(bs,0));
	for (int i = 0; i < ls; i++) {
		for (int j = 0; j < ls; j++) {
			board[i + ks - 1][j + ks - 1] = lock[i][j];
		}
	}
	
	for (int l = 0; l < 4; l++) {
		for (int i = 0; i < ks + ls - 1; i++) {
			for (int j = 0; j < ks + ls - 1; j++) {
				if (sum(i, j, key, board)) {
					answer = true;
					return true;
				}
			}
		}
		key = lotate(key);
	}
	return answer;
}