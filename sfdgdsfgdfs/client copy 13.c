#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <fcntl.h>
#include <float.h>

#define MAX_SERVERS 10
#define PACKET_SIZE 16
#define RETRY_INTERVAL 1

double best_bid = 0.0;
double best_ask = 999999999999.0;



void handle_connection(const char *ip_address, int pipe_fd[], double *best_bid, double *best_ask) {
    // int sockfd;
    struct sockaddr_in server_addr;
    int sockfd = -1;

    while (sockfd == -1) {
            // 소켓 생성
            if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
                perror("socket");
                exit(EXIT_FAILURE);
            }

            // 서버 주소 설정
            memset(&server_addr, 0, sizeof(server_addr));
            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(12345);  // 사용할 포트 번호 지정
            if (inet_pton(AF_INET, ip_address, &server_addr.sin_addr) <= 0) {
                perror("inet_pton");
                close(sockfd);
                sockfd = -1;
                sleep(RETRY_INTERVAL);  // 재시도 간격을 기다린 후 다시 시도
                continue;
            }

            // 소켓 옵션 설정 (TIME_WAIT 상태의 소켓을 빨리 재사용하기 위함)
            int enable_reuse = 1;
            if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &enable_reuse, sizeof(int)) == -1) {
                perror("setsockopt");
                close(sockfd);
                exit(EXIT_FAILURE);
            }

            // 서버에 연결 시도
            if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
                perror("connect");
                close(sockfd);
                sockfd = -1;
                sleep(RETRY_INTERVAL);  // 재시도 간격을 기다린 후 다시 시도
                continue;
            }
        }

        close(pipe_fd[1]);  // 파이프의 쓰기 디스크립터를 닫음

        char buffer[PACKET_SIZE];
        ssize_t bytes_received = recv(sockfd, buffer, PACKET_SIZE, 0);
        if (bytes_received == -1) {
            perror("recv");
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

                
                // 데이터를 부모 프로세스로 전송
                char combined_str[100];
                sprintf(combined_str, "%.4f,%.4f", *best_bid, *best_ask); // best_bid와 best_ask를 하나의 문자열로 합침
                write(pipe_fd[1], combined_str, strlen(combined_str));

            }
        } 
        printf("%s Best_Bid=%.4f, Best_Ask=%.4f\n", ip_address, *best_bid, *best_ask);

        // 연결 종료 
        close(sockfd);
        sleep(RETRY_INTERVAL);
    }



int main(int argc, char *argv[]) {

    while (1) {  // Infinite loop
        if (argc < 2) {
            fprintf(stderr, "Usage: %s <server_address1> [server_address2] [server_address3] ...\n", argv[0]);
            exit(EXIT_FAILURE);
        }

        // // 파이프 배열
        // int pipes[argc - 1][2];

        // 파이프 배열 동적으로 생성
        int (*pipes)[2] = malloc((argc - 1) * sizeof(*pipes));
        if (pipes == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        // 추가: 데이터를 저장할 배열
        char received_data[MAX_SERVERS][PACKET_SIZE];


        // 부모 프로세스에서는 모든 자식 프로세스의 종료를 기다림
        for (int i = 1; i < argc; ++i) {
            // 파이프 생성
            if (pipe(pipes[i - 1]) == -1) {
                perror("pipe");
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
                // 자식 프로세스에서 서버 처리 함수 호출
                handle_connection(argv[i], pipes[i - 1], &best_bid, &best_ask);
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

        // 부모 프로세스에서 자식 프로세스로부터 받은 데이터를 비교
        double max_bid = 0.0;
        double min_ask = DBL_MAX;  // Initialize to a large value

        for (int i = 1; i < argc; ++i) {
            // 파이프의 읽기 디스크립터를 통해 데이터를 받음
            ssize_t bytes_received = read(pipes[i - 1][0], received_data[i - 1], PACKET_SIZE);

            // 데이터를 콤마로 분리하여 best_bid와 best_ask 값을 얻음
            received_data[i - 1][bytes_received] = '\0'; // Null terminate the received data

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

        // 동적으로 할당한 파이프 배열 메모리 해제
        free(pipes);

        // 딜레이 추가: 5초 동안 재시도하지 않음
        sleep(1);
    }

    return 0;

}
