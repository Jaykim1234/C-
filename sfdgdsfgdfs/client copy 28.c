#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <fcntl.h>
#include <float.h>
#include <sys/wait.h>
#include <stdbool.h> 

// #define PACKET_SIZE 256
#define MAX_SERVERS 10
#define PACKET_SIZE 16
#define RECONNECT_DELAY_SEC 5

typedef struct {
    double max_bid;
    double min_ask;
} ServerInfo;

void handle_connection(const char *ip_address, int pipe_fd[], ServerInfo *server_infos, int server_index) {
    int sockfd;
    while (1){
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
            sleep(RECONNECT_DELAY_SEC);
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
        if ((side == 'B' && price > server_infos[server_index].max_bid) || (side == 'A' && price < server_infos[server_index].min_ask)) { 
            if (quantity > 0) {
                if (side == 'B') {
                    server_infos[server_index].max_bid = price;
                } 
                else {
                    server_infos[server_index].min_ask = price;
                }

                // 데이터를 부모 프로세스로 전송
                char combined_str[2 * sizeof(double) + 2];  // 최대 크기로 지정
                // sprintf(combined_str, "%.4f,%.4f", *input_best_bid, *input_best_ask);
                sprintf(combined_str, "%.4f,%.4f", server_infos[server_index].max_bid, server_infos[server_index].min_ask);
                write(pipe_fd[1], combined_str, strlen(combined_str) + 1);  // Null 문자까지 포함
                printf("%s Best_Bid=%.4f, Best_Ask=%.4f\n", ip_address, server_infos[server_index].max_bid, server_infos[server_index].min_ask);
            }
        }
        close(sockfd); // 연결 종료
        break;
    }
}

int main(int argc, char *argv[]) {
    bool RUN_FIRST = true;
    double P_INFINITY = DBL_MAX;
    double N_INFINITY = DBL_MIN;

    // // 초기값 설정
    // for (int i = 0; i < MAX_SERVERS; ++i) {
    //     server_infos[i] = (ServerInfo){0.0, DBL_MAX};
    // }

    // for (int i = 1; i < argc; ++i) {
    //         server_infos[i - 1].max_bid = P_INFINITY;
    //         server_infos[i - 1].min_ask = N_INFINITY;
    // }

    while (1) {
        if (argc < 2) {
            fprintf(stderr, "Usage: %s <server_address1> [server_address2] [server_address3] ...\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        // 파이프 배열
        int pipes[argc - 1][2];
        // 추가: 데이터를 저장할 배열
        char received_data[MAX_SERVERS][PACKET_SIZE];

        // 부모 프로세스
        ServerInfo server_infos[MAX_SERVERS]; // 서버 정보를 배열로 저장


        for (int i = 1; i < argc; ++i) {
            // 파이프 생성
            if (pipe(pipes[i - 1]) == -1) {
                perror("pipe error");
                exit(EXIT_FAILURE);
            }

            // 새로운 프로세스 생성
            pid_t pid = fork();

            if (pid == -1) {
                perror("fork");
                exit(EXIT_FAILURE);
            } 
            else if (pid == 0) { // 자식 프로세스
                // 파이프의 쓰기 디스크립터를 닫음
                close(pipes[i - 1][0]);

                if (RUN_FIRST) {
                    printf("Run first \n");
                    // 자식 프로세스에서 서버 처리 함수 호출
                    printf("ServerInfo[%d]: max_bid=%.4f, min_ask=%.4f\n", i - 1, server_infos[i - 1].max_bid, server_infos[i - 1].min_ask);
                    handle_connection(argv[i], pipes[i - 1], server_infos, i - 1);
                }
                else {
                    double bid, ask;
                    ssize_t bytes_received = read(pipes[i - 1][0], received_data[i - 1], PACKET_SIZE); // 파이프의 읽기 디스크립터를 통해 데이터를 받음
                    received_data[i - 1][bytes_received] = '\0'; // Null terminate the received data
                    sscanf(received_data[i - 1], "%lf,%lf", &bid, &ask); // 데이터를 콤마로 분리하여 best_bid와 best_ask 값을 얻음
                    server_infos[i - 1].max_bid = bid;
                    server_infos[i - 1].min_ask = ask;
                    handle_connection(argv[i], pipes[i - 1], server_infos, i - 1);
                }

                // 자식 프로세스 종료
                exit(EXIT_SUCCESS);
                
            } else { // 부모 프로세스
                // 파이프의 읽기 디스크립터를 닫음
                close(pipes[i - 1][1]);
            }
        }

        // 모든 자식 프로세스의 종료를 기다림
        for (int i = 1; i < argc; ++i) {
            int status;
            waitpid(-1, &status, 0);
        }

        // 통합 서버
        double max_bid = DBL_MIN;
        double min_ask = DBL_MAX;

        printf("For loop started \n");
        for (int i = 1; i < argc; ++i) {
            // 파이프의 읽기 디스크립터를 통해 데이터를 받음
            ssize_t bytes_received = read(pipes[i - 1][0], received_data[i - 1], PACKET_SIZE);

            // 데이터를 콤마로 분리하여 best_bid와 best_ask 값을 얻음
            received_data[i - 1][bytes_received] = '\0'; // Null terminate the received data

            // 디버그용으로 received_data 출력
            printf("Received Data[%d]: %s\n", i, received_data[i - 1]);

            double bid, ask;
            sscanf(received_data[i - 1], "%lf,%lf", &bid, &ask);
            
            // 비교하여 최댓값과 최솟값 갱신
            if (bid > max_bid) {
                max_bid = bid;
            }
            if (ask < min_ask) {
                min_ask = ask;
            }
        }
        
        printf("Best Bid=%.4f, Best Ask=%.4f\n", max_bid, min_ask);
        printf("-------------------------------------\n");
        RUN_FIRST = false;
    }

    return 0;
}
