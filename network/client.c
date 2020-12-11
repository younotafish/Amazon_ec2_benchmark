#include "common.h"

struct timeval t0, t1, t2, t3, t4;
int fileSize;

char buff[BUFFSIZE];

int time_diff(struct timeval a, struct timeval b) {
    return (b.tv_sec - a.tv_sec) * 1000 * 1000 + (b.tv_usec - a.tv_usec);
}

void report_time() {
    printf("file size: %d\n", fileSize);
    printf("connect: %d us\n", time_diff(t0, t1));
    printf("roundtrip: %d us\n", time_diff(t1, t2));
    printf("transmit: %d us\n", time_diff(t2, t3));
    printf("teardown: %d us\n", time_diff(t3, t4));
}

int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0;
    char flag;

    struct sockaddr_in serv_addr;

    if (argc != 3) {
        printf("\n Usage: %s <ip of server> <file size> \n", argv[0]);
        return 1;
    }

    gettimeofday(&t0, NULL);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);

    connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    gettimeofday(&t1, NULL);

    sscanf(argv[2], "%d", &fileSize);

    send(sockfd, &fileSize, sizeof(int), 0);
    recv(sockfd, &flag, 1, 0); // receive the comma
    assert(flag == ',');

    gettimeofday(&t2, NULL);

    int sent = 0;
    while (sent < fileSize) {
        int remaining = MIN(BUFFSIZE, fileSize - sent);
        int ret = send(sockfd, buff, remaining, 0);
        if (ret < 0) {
            exit(-1);
        }
        sent += ret;
    }
    recv(sockfd, &flag, 1, 0); // receive the comma
    assert(flag == '.');

    gettimeofday(&t3, NULL);

    close(sockfd);
    connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    gettimeofday(&t4, NULL);

    close(sockfd);

    report_time();

    return 0;
}
