#include <iostream>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;
 
#define PORT 5000
 
int main() {

	struct timeval tv;

	tv.tv_sec = 1;
	tv.tv_usec = 0;

	for(int i = 0; ; i++) {
		cerr << i << endl;

		int sock;
		struct sockaddr_in addr;
		char buffer[1024];

		char msg[128];
		strcpy(msg, "hello! I'm Client_");

		int recv_len;
 
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
			printf("소켓 생성 실패\n");
			return 0;
		}

		setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval));
	
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		addr.sin_port = htons(PORT);

		if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
			printf("소켓 연결 실패\n");
			close(sock);
			usleep(10000);
			continue;
		}

		sprintf(msg+strlen(msg), "%d", i);

		if(send(sock, msg, strlen(msg), 0) < 0) {
			printf("소켓 전송 실패\n");
		}
 
		if ((recv_len = recv(sock, buffer, 1024, 0)) < 0) {
			printf("시간 초과!!!\n");
			fprintf(stdout, "%s\n", msg);
		} else {

		}

		buffer[recv_len] = '\0';
 
		//usleep(1000);
		sleep(1);

		close(sock);
 
	}
	
	return 0;
}
