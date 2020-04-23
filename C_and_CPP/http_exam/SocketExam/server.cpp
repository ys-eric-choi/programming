#include<stdio.h>
#include<string.h>
 
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
 
#define PORT 5000
 
int main() {
    //SOCKET sock, client_sock;
    int sock, client_sock;
    struct sockaddr_in addr, client_addr;
    char buffer[1024];
    int len, addr_len, recv_len;
 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("소켓 생성 실패\n");
        return 0;
    }
 
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORT);
 
    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        printf("소켓 바인드 실패\n");
        return 1;
    }
 
    if (listen(sock, 5) < 0) {
        printf("소켓 리슨 실패\n");
        return 1;
    }
 
    addr_len = sizeof(client_addr);
 
    printf("waiting for client..\n");
 
    while ((client_sock = accept(sock, (struct sockaddr*)&client_addr, (socklen_t *)&addr_len)) > 0) {
        printf("client ip : %s\n", inet_ntoa(client_addr.sin_addr));
 
        if ((recv_len = recv(client_sock, buffer, 1024, 0)) < 0) {
            printf("recv 실패\n");
            break;
        }
 
        buffer[recv_len] = '\0';
 
        printf("received data: %s\n", buffer);

		//usleep(100);
 
        send(client_sock, buffer, strlen(buffer), 0);
 
        close(client_sock);
    }
 
    close(sock);

    return 0;
}
