#include<stdio.h>
#include<string.h>
#include <stdlib.h>
 
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#include <pthread.h>
 
#define PORT 5000
#define MAX 10

static int sock;

void* ServerThread(void *data) {
	int idx = atoi((char*)data);
	int i = 0;

    //int sock, client_sock;
    int client_sock;
    struct sockaddr_in addr, client_addr;
    char buffer[1024];
    int len, addr_len, recv_len;
 
/*
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("소켓 생성 실패\n");
        //return 0;
    }
 
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORT);
 
    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        printf("소켓 바인드 실패\n");
        //return 1;
    }
 
    if (listen(sock, 5) < 0) {
        printf("소켓 리슨 실패\n");
        //return 1;
    }
*/
 
    addr_len = sizeof(client_addr);
 
    printf("waiting for client..\n");
 
    while ((client_sock = accept(sock, (struct sockaddr*)&client_addr, (socklen_t *)&addr_len)) > 0) {
        printf("client ip : %s\n", inet_ntoa(client_addr.sin_addr));
 
        if ((recv_len = recv(client_sock, buffer, 1024, 0)) < 0) {
            printf("recv 실패\n");
            break;
        }
 
        buffer[recv_len] = '\0';
 
        //printf("received data: %s\n", buffer);
        fprintf(stdout, "received data: %s\n", buffer);

		//usleep(100);
 
        if(send(client_sock, buffer, strlen(buffer), 0) < 0) {
			fprintf(stdout, "send fail\n");
		}
 
        close(client_sock);
    }
 
    close(sock);
}
 
int main() {
    //int sock, client_sock;
    struct sockaddr_in addr, client_addr;
    char buffer[1024];
    int len, addr_len, recv_len;
 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("소켓 생성 실패\n");
        //return 0;
    }
 
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORT);
 
    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        printf("소켓 바인드 실패\n");
        //return 1;
    }
 
    if (listen(sock, 5) < 0) {
        printf("소켓 리슨 실패\n");
        //return 1;
    }

	pthread_t p_thread[MAX];
	int thr_id, status;

	for(int i = 0; i < MAX; i++) {
		char buf[16];
		sprintf(buf, "%d", i);
		thr_id = pthread_create(&(p_thread[i]), NULL, ServerThread, (void *)buf);
	}

	for(int i = 0; i < MAX; i++) {
		pthread_join(p_thread[i], (void **)&status);
	}

    return 0;
}
