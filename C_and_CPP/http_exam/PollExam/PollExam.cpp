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
const int RECV_BUF_SIZE		= 1024;

const int SEND_MESG_SUCCESS	= 0;
const int CONNECT_FAILED	= -1;
const int CONNECT_TIMEOUT	= -2;
const int SEND_MESG_FAILED	= -3;

const string ERR_MESG[] = { \
	"Sucess", \
	"[ERROR] Connect Failed", \
	"[ERROR] Connect Timeout" \
	"[ERROR] Send Message Failed" \
};

int ConnectWithTimeout(const char* strIP, int iPort, int iConnect_Timeout_MS, int iRecv_Timeout_MS) {

	int iSockFd = -1;
	if((iSockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		return CONNECT_FAILED;
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
		return CONNECT_FAILED;
	}

	if((iStat = connect(iSockFd, (struct sockaddr*)&addr, sizeof(addr))) < 0) {
		if(errno != EINPROGRESS) {
			return CONNECT_FAILED;
		}
	}

	if(iStat == 0) goto CONNECT_DONE;

	pollfd sockPoll[NUM_POLL_FD];
	sockPoll[0].fd = iSockFd;
	sockPoll[0].events = POLLIN | POLLOUT;
	sockPoll[0].revents = 0;

	iStat = poll(sockPoll, NUM_POLL_FD, iConnect_Timeout_MS);
	
	if(iStat < 0) {
		return CONNECT_FAILED;
	} else if(iStat == 0) { // TIME-OUT
#ifdef _HTTP_DEBUG
		cerr << "Connet Timeout" << endl;
#endif
		return CONNECT_TIMEOUT;
	}

	if(sockPoll[0].revents & (POLLIN | POLLOUT)) {
		iLen = sizeof(iErrorNum);
		if((iStat = getsockopt(iSockFd, SOL_SOCKET, SO_ERROR, &iErrorNum, &iLen)) < 0) {
			return CONNECT_FAILED;
		}
	}

CONNECT_DONE:
	fcntl(iSockFd, F_SETFL, iFlags);

	if(iErrorNum) {
		errno = iErrorNum;
		return CONNECT_FAILED;
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

	string strSendMesg = oss.str();
#ifdef _HTTP_DEBUG
	cerr << "Send Message:\n" << strSendMesg << endl;
#endif
	if(send(iSockFd, strSendMesg.c_str(), strSendMesg.length(), 0) < 0) {
		return SEND_MESG_FAILED;
	}

	return SEND_MESG_SUCCESS;
}

string ReceiveData(int iSockFd, int iMaxRetry = 1) {
	string strRecvMesg = "";
	string strBodyMesg = "";

	for(int iRecvLen = -1, iRetry = 0; (iRecvLen != 0) && (iRetry < iMaxRetry); ) {
		char strRecvBuf[RECV_BUF_SIZE] = {"\0",};
		iRecvLen = recv(iSockFd, strRecvBuf, RECV_BUF_SIZE - 1, 0);
		if(iRecvLen < 0) {
			iRetry++;
		} else {
			if(iRecvLen < RECV_BUF_SIZE) {
				strRecvBuf[iRecvLen] = '\0';
			}
			strRecvMesg += strRecvBuf;
			iRetry = 0;
		}
	}
#ifdef _HTTP_DEBUG
	cerr << "Receive Message:\n" << strRecvMesg << endl;
#endif

	char* strGetBody = NULL;
	if((strGetBody = strstr((char*)strRecvMesg.c_str(), "\n\r")) != NULL) {
#ifdef _HTTP_DEBUG
		cerr << "Body: " << strGetBody + 3 << endl;
#endif
		strBodyMesg = strGetBody + 3;
	}

	return strBodyMesg;
}

int main(void) {

	string strIP = "127.0.0.1";
	int iPort = 5000;
	string strAPI = "/get_data";
	string strHeader = "Content-Type: application/json";

	int iConnect_Timeout_MS = 2000;
	int iRecv_Timeout_MS = 2000;

	int iSockFd = -1;
	iSockFd = ConnectWithTimeout(strIP.c_str(), iPort, iConnect_Timeout_MS, iRecv_Timeout_MS);

	if(iSockFd < 0) {
		cerr << ERR_MESG[(iSockFd * -1)] << endl;
		close(iSockFd);
		return 0;
	}

	int iResult = -1;
	string strBody = "{\"in\": \"HI\"}";
	iResult = SendDataByPost(iSockFd, strIP, iPort, strAPI, strHeader, strBody);

	if(iResult < 0) {
		cerr << ERR_MESG[(iResult * -1)] << endl;
		close(iSockFd);
		return 0;
	}

	string strResponse = ReceiveData(iSockFd);
	cerr << "Response: " << strResponse << endl;

	close(iSockFd);

	return 0;
}
