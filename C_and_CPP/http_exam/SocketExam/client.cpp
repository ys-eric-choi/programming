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

	tv.tv_sec = 0;
	tv.tv_usec = 300;

	for(int i = 0; ; i++) {
	//cerr << i << endl;
	//while(1){

    //SOCKET sock;
    int sock;
    struct sockaddr_in addr;
    char buffer[1024];
    const char *msg = "hello! I'm Client";
    int recv_len;
 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("소켓 생성 실패\n");
        return 0;
    }

	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval));
	//setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&tv, sizeof(struct timeval));

	//while(1) {
    
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    //addr.sin_addr.s_addr = inet_addr("114.199.27.36");
    addr.sin_port = htons(PORT);
 

	//while(1) {
    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        printf("소켓 연결 실패\n");
    	close(sock);
	//continue;
        return 0;
    } //else break;
	//}

	//while(1) {
    if(send(sock, msg, strlen(msg), 0) < 0) {
        printf("소켓 전송 실패\n");
        //return 0;
    }
 
    if ((recv_len = recv(sock, buffer, 1024, 0)) < 0) {
        printf("시간 초과!!!\n");
		//continue;
        //return 0;
    } else {
		//fprintf(stderr, "%d\n", recv_len);
	}
    
    buffer[recv_len] = '\0';
 
    //printf("recived data : %s\n", buffer);
	usleep(200);
	//}

	//}
    close(sock);
 
	}
    
    return 0;
}
