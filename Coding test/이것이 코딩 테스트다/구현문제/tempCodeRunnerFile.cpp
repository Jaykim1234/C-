#include <iostream>
#include <vector>
 
#define M 3
#define N 4
 
int main()
{
    // Vector 요소를 채울 기본값을 지정합니다.
    int default_value = 1;
 
    // 채우기 생성자를 사용하여 2차원 Vector 초기화
    // 주어진 기본값으로
    std::vector<std::vector<int>> matrix(M, std::vector<int>(N, default_value));
 
    // 2차원 Vector를 출력
 
    return 0;
}