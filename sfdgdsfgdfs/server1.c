#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>

#define SERVER_IP "192.168.1.1"  // 각 서버에 할당된 IP 주소로 변경
#define PORT 12345
#define PACKET_SIZE 16

void generate_random_packet(char *buffer) {
    time_t current_time = time(NULL);
    unsigned int seed = (unsigned int)(current_time - 1);
    srand(seed);

    // Side: 'B' (Bid) 또는 'A' (Ask) 중 랜덤 선택
    buffer[0] = (rand() % 2 == 0) ? 'B' : 'A';
        
    // Price: 랜덤 부동 소수점 숫자 생성 (4자리 정밀도)
    double price = (rand() % 10000) + (rand() % 100) / 100.0;
    sprintf(buffer + 1, "%11.4f", price);

    // Quantity: 랜덤 부동 소수점 숫자 생성 (1자리 정밀도)
    double quantity = (rand() % 100) / 10.0;
    sprintf(buffer + 12, "%4.1f", quantity);
}

int main() {
        int sockfd, new_socket;
        struct sockaddr_in server_addr, client_addr;
        socklen_t client_len = sizeof(client_addr);

        // 소켓 생성
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            perror("socket");
            exit(EXIT_FAILURE);
        }

        // 서버 주소 설정
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
        server_addr.sin_port = htons(PORT);

        // 소켓에 주소 바인딩
        if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
            perror("bind");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        printf("Server listening on %s:%d...\n", SERVER_IP, PORT);
        while (1) {

        // 연결 대기
        if (listen(sockfd, 5) == -1) {
            perror("listen");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        
            // 클라이언트 연결 수락
            if ((new_socket = accept(sockfd, (struct sockaddr *)&client_addr, &client_len)) == -1) {
                perror("accept");
                close(sockfd);
                exit(EXIT_FAILURE);
            }

            // 1초마다 데이터 전송  
        
            char buffer[PACKET_SIZE];
            generate_random_packet(buffer);
            printf("Server: %s\n", buffer);

            // 데이터 전송
            if (send(new_socket, buffer, PACKET_SIZE, 0) == -1) {
                perror("send");
                close(new_socket);
                close(sockfd);
                exit(EXIT_FAILURE);
            }

            sleep(1); // 1초 대기
    }

    // // 연결 종료
    // close(new_socket);
    // close(sockfd);

    return 0;
}