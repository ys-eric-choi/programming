#include <iostream>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <poll.h>
#include <fcntl.h>
#include <errno.h>

using namespace std;

const int NUM_POLL_FD		= 1;

const int CONNECT_FAIL		= -1;
const int CONNECT_TIMEOUT	= -2;

const string ERR_MESG[] = { \
	"Connect Failed", \
	"Connect Timeout" \
};

int ConnectHttp(const char* strIP, int iPort, int iConnect_Timeout_MS, int iRecv_Timeout_MS) {

	int iSockFd = -1;
	if((iSockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		return CONNECT_FAIL;
	}

	struct timeval tv;
	tv.tv_sec = iRecv_Timeout_MS / 1000;
	tv.tv_usec = (iRecv_Timeout_MS % 1000) * 1000;

	setsockopt(iSockFd, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(struct timeval));

	struct sockaddr_in addr;

	memset(&addr, 0x00, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(strIP);
	addr.sin_port = htons(iPort);

	int iFlags, iStat = 0, iErrorNum = 0;
	socklen_t iLen;

	iFlags = fcntl(iSockFd, F_GETFL, 0);
	if(fcntl(iSockFd, F_SETFL, iFlags | O_NONBLOCK) < 0) {
		return CONNECT_FAIL;
	}

	if((iStat = connect(iSockFd, (struct sockaddr*)&addr, sizeof(addr))) < 0) {
		if(errno != EINPROGRESS) {
			return CONNECT_FAIL;
		}
	}

	if(iStat == 0) goto CONNECT_DONE;

	pollfd sockPoll[NUM_POLL_FD];
	sockPoll[0].fd = iSockFd;
	sockPoll[0].events = POLLIN | POLLOUT;
	sockPoll[0].revents = 0;

	iStat = poll(sockPoll, NUM_POLL_FD, iConnect_Timeout_MS);
	
	if(iStat < 0) {
		return CONNECT_FAIL;
	} else if(iStat == 0) { // TIME-OUT
		return CONNECT_TIMEOUT;
	}

	if(sockPoll[0].revents & (POLLIN | POLLOUT)) {
		iLen = sizeof(iErrorNum);
		if((iStat = getsockopt(iSockFd, SOL_SOCKET, SO_ERROR, &iErrorNum, &iLen)) < 0) {
			return CONNECT_FAIL;
		}
	}

CONNECT_DONE:
	fcntl(iSockFd, F_SETFL, iFlags);

	if(iErrorNum) {
		errno = iErrorNum;
		return CONNECT_FAIL;
	}

	return iSockFd;
}

int main(void) {

	cerr << ERR_MESG[1] << endl;

	int iSockFd = -1;

	iSockFd = ConnectHttp("127.0.0.1", 5000, 700, 700);

	char body[1024];
	strcpy(body, "{\"TEST_MSG\": \"안녕하세요\"}");
	
	char msg[1024];
	strcpy(msg, "POST /GPR HTTP/1.1\r\nHost: 127.0.0.1:5000\r\nAccept: */*\r\nContent-Type: application/json\r\nContent-Length: ");
	sprintf(msg + strlen(msg), "%d\r\n\r\n", strlen(body));
	strcat(msg, body);

	if(send(iSockFd, msg, strlen(msg), 0) < 0) {
		return 0;
	}
	
	int k = 0, recv_len;
	char b;
	char tmpBuff[2048];
	char tmpBuff2[2048] = {'\0',};
	tmpBuff[0] = '\0';
	
	while(1) {
		if ((recv_len = recv(iSockFd, &b, 1, 0)) > 0) {
			tmpBuff[k++] = b;
		} else {
			break;
		}
	}
	tmpBuff[k] = '\0';
	
	char *c = NULL;
	if((c = strstr(tmpBuff, "\n\r")) != NULL) {
		fprintf(stderr, "Response: %s\n", c + 3);
	}
	
	close(iSockFd);

	return 0;
}
