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


#include <string>
#include <vector>

using namespace std;
int n, m;

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


vector <vector<int> > rotateKey(vector<vector<int> > key){
    vector <vector<int> > rotatedKey(m, vector<int>(m));
    
    for(int i=0; i<m; i++){
        for(int j =0; j<m; j++){
            rotatedKey[i][j] = key[m-j-1][i];
        }
    }
    
    return rotatedKey;
}


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
