#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PACKET_SIZE 16

void process_packet(const char *packet) {
    // 패킷을 처리하고 Best Bid 또는 Best Ask 값이 변경되면 출력하는 로직을 여기에 추가
    printf("Processing packet: %s\n", packet);
}

void handle_connection(const char *ip_address) {
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

    // 더미 데이터 전송 (테스트 목적)
    const char *dummy_data = "BID1234567891000QUAN";
    if (send(sockfd, dummy_data, PACKET_SIZE, 0) == -1) {
        perror("send");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // 데이터 수신 및 처리 루프
    while (1) {
        ssize_t bytes_received = recv(sockfd, buffer, PACKET_SIZE, 0);

        if (bytes_received == -1) {
            perror("recv");
            close(sockfd);
            exit(EXIT_FAILURE);
        } else if (bytes_received == 0) {
            printf("Connection closed by server %s\n", ip_address);
            break;
        } else {
            buffer[bytes_received] = '\0'; // Null terminate the received data
            process_packet(buffer);
        }
    }

    // 연결 종료
    close(sockfd);
}

int main() {
    // 로컬 루프백 주소를 사용하여 테스트
    handle_connection("127.0.0.1");

    return 0;
}
