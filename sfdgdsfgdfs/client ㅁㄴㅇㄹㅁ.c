#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define PACKET_SIZE 16


double best_bid = 0.0;
double best_ask = 999999999999.0;

void handle_connection(const char *ip_address , int pipe_fd[]) {
    int sockfd;
    struct sockaddr_in server_addr;

    // 소켓 생성
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 서버 주소 설정
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345); // 사용할 포트 번호 지정
    if (inet_pton(AF_INET, ip_address, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // 서버에 연결
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // 데이터 수신 및 출력
    while (1) {
        char buffer[PACKET_SIZE];

        ssize_t bytes_received = recv(sockfd, buffer, PACKET_SIZE, 0);
        if (bytes_received == -1) {
            perror("recv");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        buffer[bytes_received] = '\0'; // Null terminate the received data
        // printf("Received from server: %s\n", buffer);

        // 수신된 데이터에서 Side, Price, Quantity 추출
        char side;
        double price, quantity;
        sscanf(buffer, " %c %lf %lf", &side, &price, &quantity);

        // 최고 가격 갱신
        if (side == 'B' && price > best_bid && quantity>0) {
            best_bid = price;
        }
        else if (side == 'A' && price < best_ask && quantity>0){
            best_ask = price;
        }
//
        // 최고 Bid 및 Ask 출력
        //printf("%s Best_Bid=%.4f, Best_Ask=%.4f\n",ip_address, best_bid, best_ask);
        
        // 데이터를 부모 프로세스로 전송
        write(pipe_fd[1], buffer, PACKET_SIZE);
    }

    // 연결 종료 (실행되지 않음)
    close(sockfd);
}


 int main(int argc, char *argv[]) {
    // 모든 주소에 대해 연결을 처리
    for (int i = 1; i < argc; ++i) {
        handle_connection(argv[i]);
    }
    return 0;

// int main(int argc, char *argv[]) {
//     // 명령행 인수로 최소한 하나의 IP 주소를 받아옴
//     if (argc < 2) {
//         fprintf(stderr, "사용법: %s <서버 주소1> [서버 주소2] [서버 주소3] ...\n", argv[0]);
//         exit(EXIT_FAILURE);
//     }

//     // 파이프 생성
//     int pipe_fd[2];
//     if (pipe(pipe_fd) == -1) {
//         perror("pipe");
//         exit(EXIT_FAILURE);
//     } 
    
//     // 모든 주소에 대해 연결을 처리
//     for (int i = 1; i < argc; ++i) {
//         // 새로운 프로세스 생성
//         pid_t pid = fork();

//         if (pid == -1) {
//             perror("fork");
//             exit(EXIT_FAILURE);
//         } else if (pid == 0) { // 자식 프로세스
//             // 파이프의 쓰기 디스크립터를 닫음
//             close(pipe_fd[1]);

//             // 자식 프로세스에서 서버 처리 함수 호출
//             handle_connection(argv[i], pipe_fd);

//             // 자식 프로세스 종료
//             exit(EXIT_SUCCESS);
//         }
//     }
    
//     // 부모 프로세스에서는 파이프의 읽기 디스크립터를 닫음
//     close(pipe_fd[0]);

//     // 부모 프로세스에서는 파이프로부터 데이터를 읽어 출력
//     while (1) {
//         char buffer[PACKET_SIZE];

//         ssize_t bytes_read = read(pipe_fd[0], buffer, PACKET_SIZE);
//         if (bytes_read == -1) {
//             perror("read");
//             exit(EXIT_FAILURE);
//             }
//         }
//     }

    
    // // 모든 주소에 대해 연결을 처리
    // for (int i = 1; i < argc; ++i) {
    //     handle_connection(argv[i]);
    // }
    // return 0;