#include <string>
#include <vector>

using namespace std;

int M, N, Point;
int Size = N + 2*(M -1);

vector<vector<int > > MAP(Size, vector<int>(Size, 2));

void Make_MAP(vector<vector<int> > lock)
{
    int lock_x = 0;
    int lock_y = 0;
    for (int i = M - 1; i < Size - (M-1);i++, lock_x++)
    {
        for (int j = M-1; j< Size - (M-1); j++, lock_y++)
        {
            MAP[i][j] = lock[lock_x][lock_y];
            if (MAP[i][j] == 0) Point++;

        }
        lock_y = 0 ;               
    }
}

// #include <iostream>
// int main() {
//     std::cout << "Hello" << std::endl ;
//     return 0;
// }

