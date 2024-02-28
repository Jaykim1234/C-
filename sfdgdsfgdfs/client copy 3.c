#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <fcntl.h>

#define PACKET_SIZE 16

double best_bid = 0.0;
double best_ask = 999999999999.0;

void handle_connection(const char *ip_address, int pipe_fd[], double *best_bid, double *best_ask) {
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

    char buffer[PACKET_SIZE];
    ssize_t bytes_received = recv(sockfd, buffer, PACKET_SIZE, 0);
    if (bytes_received == -1) {
        perror("recv");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    buffer[bytes_received] = '\0'; // Null terminate the received data


    // 수신된 데이터에서 Side, Price, Quantity 추출
    char side;
    double price, quantity;
    sscanf(buffer, " %c %lf %lf", &side, &price, &quantity);

    // 최고 가격 갱신
    if ((side == 'B' && price > *best_bid) || (side == 'A' && price < *best_ask)) {
        if (quantity > 0) {
            if (side == 'B') {
                *best_bid = price;
            } else {
                *best_ask = price;
            }

            // printf("%s Best_Bid=%.4f, Best_Ask=%.4f\n", ip_address, *best_bid, *best_ask);
            // 데이터를 부모 프로세스로 전송
            write(pipe_fd[1], buffer, PACKET_SIZE);


            // // 데이터를 부모 프로세스로 전송
            // write(pipe_fd[1], buffer, strlen(buffer));

            // 데이터를 부모 프로세스로 전송
            // write(pipe_fd[1], buffer, strlen(buffer));

        }
    }
    //printf("%s Best_Bid=%.4f, Best_Ask=%.4f\n", ip_address, *best_bid, *best_ask);
    // printf("\n");
    // 연결 종료 
    close(sockfd);
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <server_address1> [server_address2] [server_address3] ...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int pipes[argc - 1][2];
    // 부모 프로세스에서는 모든 자식 프로세스의 종료를 기다림
    for (int i = 1; i < argc; ++i) {
        // 새로운 프로세스 생성
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { // 자식 프로세스
            // 자식 프로세스에서 서버 처리 함수 호출
            handle_connection(argv[i], pipes[i - 1], &best_bid, &best_ask);
            // 자식 프로세스 종료
            exit(EXIT_SUCCESS);
        }
    }

    // 모든 자식 프로세스의 종료를 기다림
    for (int i = 1; i < argc; ++i) {
        int status;
        waitpid(-1, &status, 0);
    }

    return 0;
}