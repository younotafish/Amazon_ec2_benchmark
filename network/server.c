#include "common.h"

char buff[BUFFSIZE];

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;
    int fileSize;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORT);

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(listenfd, 5);

    while (1) {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

        if (recv(connfd, &fileSize, sizeof(int), 0) < 0) {
            exit(-1);
        }
        send(connfd, ",", 1, 0);

        int received = 0;
        while (received < fileSize) {
            int remaining = MIN(BUFFSIZE, fileSize - received);
            int ret = recv(connfd, buff, remaining, 0);
            if (ret < 0) {
                exit(-1);
            }
            received += ret;
        }

        send(connfd, ".", 1, 0);
    }

}
