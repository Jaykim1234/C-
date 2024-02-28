#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PACKET_SIZE 16
#define MAX_SERVERS 10  // 최대 서버 수

typedef struct {
    double best_bid;
    double best_ask;
} ServerData;

typedef struct {
    char ip_address[INET_ADDRSTRLEN];  // IPv4 주소의 최대 길이
    ServerData server_data;
} ServerInfo;


// 서버의 초기 최고 Bid 및 Ask 값을 설정하는 함수
void initializeServerData(ServerData *server_data) {
    // 초기 값으로 설정할 범위를 정의
    const double initial_bid = 0;  
    const double initial_ask = 999999999999999;  

    // 초기화
    server_data->best_bid = initial_bid;
    server_data->best_ask = initial_ask;
}

ServerInfo handle_connection(const char *ip_address) {
    int sockfd;
    struct sockaddr_in server_addr;

    ServerInfo result; // 결과 값을 저장할 구조체 변수

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

    // 서버의 초기 최고 Bid 및 Ask 값을 설정
    initializeServerData(&result.server_data);

    // // 데이터 수신 및 출력
    // while (1) 

    int num_updates = 5;  // 수정 횟수를 설정
    while (num_updates > 0){
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
        if (side == 'B' && price > result.server_data.best_bid && quantity > 0) {
            result.server_data.best_bid = price;
        } else if (side == 'A' && price < result.server_data.best_ask && quantity > 0) {
            result.server_data.best_ask = price;
        }

        // 최고 Bid 및 Ask 출력
        // printf("%s Best_Bid=%.4f, Best_Ask=%.4f\n", result.ip_address, result.server_data.best_bid, result.server_data.best_ask);
        printf("Hello\n");
        num_updates--;
    }

    // 연결 종료 (실행되지 않음)
    close(sockfd);

    return result; // 최종 결과를 반환
}


// 스레드 함수
void* handle_connection_thread(void* arg) {
    ServerInfo* server_info = (ServerInfo*)arg;
    handle_connection(server_info->ip_address, &server_info->server_data);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > MAX_SERVERS + 1) {
        fprintf(stderr, "사용법: %s <서버 주소1> [서버 주소2] [서버 주소3] ... (최대 %d개)\n", argv[0], MAX_SERVERS);
        exit(EXIT_FAILURE);
    }

    int num_servers = argc - 1;
    ServerInfo server_info[MAX_SERVERS];
    pthread_t threads[MAX_SERVERS];

    // 각 서버에 대한 초기화 및 연결 처리
    for (int i = 0; i < num_servers; ++i) {
        server_info[i] = handle_connection(argv[i + 1]);
    }

    // 각 서버에 대해 별도의 스레드로 연결 처리를 시작
    for (int i = 0; i < num_servers; ++i) {
        pthread_create(&threads[i], NULL, handle_connection_thread, (void*)&server_info[i]);
    }

    // 모든 스레드의 종료를 기다림
    for (int i = 0; i < num_servers; ++i) {
        pthread_join(threads[i], NULL);
    }

    // 모든 서버에 대한 결과 출력
    for (int i = 0; i < num_servers; ++i) {
        printf("Server %s - Best_Bid=%.4f, Best_Ask=%.4f\n", server_info[i].ip_address, server_info[i].server_data.best_bid, server_info[i].server_data.best_ask);
    }

    return 0;
}


// int main(int argc, char *argv[]) {
//     if (argc < 2 || argc > MAX_SERVERS + 1) {
//         fprintf(stderr, "사용법: %s <서버 주소1> [서버 주소2] [서버 주소3] ... (최대 %d개)\n", argv[0], MAX_SERVERS);
//         exit(EXIT_FAILURE);
//     }

    // int num_servers = argc - 1;

    // ServerInfo server_info[MAX_SERVERS];


    // // 모든 주소에 대해 연결을 처리
    // for (int i = 0; i < num_servers; ++i) {
    //     server_info[i] = handle_connection(argv[i + 1]);
    // }

    // for (int i = 0; i < num_servers; ++i) {
    //     printf("Server %s - Best_Bid=%.4f, Best_Ask=%.4f\n", server_info[i].ip_address, server_info[i].server_data.best_bid, server_info[i].server_data.best_ask);
    // }


    // // 초기화
    // for (int i = 0; i < num_servers; ++i) {
    //     strcpy(server_info[i].ip_address, argv[i + 1]);
    //     server_info[i].server_data.best_bid = 0.0;
    //     server_info[i].server_data.best_ask = 999999999999.0;
    // }

    // // main 함수에서 server_info 배열을 활용하여 필요한 작업 수행 가능
    // // IP 주소와 ServerData를 출력
    // for (int i = 0; i < num_servers; ++i) {
    //     printf("Server %s - Best_Bid=%.4f, Best_Ask=%.4f\n", server_info[i].ip_address, server_info[i].server_data.best_bid, server_info[i].server_data.best_ask);
    // }

    // server_info[0] = handle_connection(argv[0]);
    // printf("Server %s - Best_Bid=%.4f, Best_Ask=%.4f\n", server_info[0].ip_address, server_info[0].server_data.best_bid, server_info[0].server_data.best_ask);

    //     // 모든 주소에 대해 연결을 처리
    // for (int i = 1; i < argc; ++i) {
    //     handle_connection(argv[i]);
    // }

//     return 0;
// }
