#include <iostream>
#include <sstream>
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

const int SEND_MESG_SUCCESS	= 0;
const int CONNECT_FAILED	= -1;
const int CONNECT_TIMEOUT	= -2;
const int SEND_MESG_FAILED	= -3;

const string ERR_MESG[] = { \
	"Connect Failed", \
	"Connect Timeout" \
};

int ConnectWithTimeout(const char* strIP, int iPort, int iConnect_Timeout_MS, int iRecv_Timeout_MS) {

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

	// Set Non-blocking
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
		cerr << "Connet Time-out" << endl;
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

int SendDataByPost(int iSockFd, const string& strIP, int iPort, const string& strAPI, const string& strHeader, const string& strBody) {

	ostringstream oss;
	oss.str(""); oss.clear(); // Initialize
	oss << "POST " << strAPI << " HTTP/1.1\r\n";
	oss << " Host: " << strIP << ":" << iPort << "\r\n";
	oss << "Accept: */*\r\n";
	oss << strHeader << "\r\n";
	oss << "Content-Length: " << strBody.length() << "\r\n";
	oss << "\r\n" << strBody;

	string strMesg = oss.str();

	if(send(iSockFd, strMesg.c_str(), strMesg.length(), 0) < 0) {
		return SEND_MESG_FAIL;
	}

	return -1;
}

int main(void) {

	string strIP = "127.0.0.1";
	int iPort = 5000;
	string strAPI = "/get_data";
	string strHeader = "Content-Type: application/json";

	int iConnect_Timeout_MS = 500;
	int iRecv_Timeout_MS = 500;

	int iSockFd = -1;
	iSockFd = ConnectWithTimeout(strIP.c_str(), iPort, iConnect_Timeout_MS, iRecv_Timeout_MS);

	if(iSockFd < 0) return 0;

	int iResult = -1;
	string strBody = "{\"in\": \"HI\"}";
	iResult = SendDataByPost(iSockFd, strIP, iPort, strAPI, strHeader, strBody);

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
