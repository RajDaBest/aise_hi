#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[BUFFER_SIZE];
    socklen_t len = sizeof(servaddr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("103.144.93.188");

    while (1) {
        printf("You: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        sendto(sockfd, buffer, strlen(buffer), 0, (const struct sockaddr *)&servaddr, len);

        recvfrom(sockfd, buffer, BUFFER_SIZE, 0, NULL, NULL);
        printf("Server: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}
