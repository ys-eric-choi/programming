#include <iostream>
#include <sstream>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <poll.h>
#include <fcntl.h>
#include <errno.h>

#include "HttpUtil.h"

namespace HTTPUTIL {

const int NUM_POLL_FD		= 1;
const int RECV_BUF_SIZE		= 1024;

int ConnectWithTimeout(const char* strIP, const int iPort, int iConnect_Timeout_MS, int iRecv_Timeout_MS) {

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

string MakeHttpHeader(const vector<string>& vecHeaders) {
	ostringstream oss;
	oss.str(""); oss.clear(); // Initialize

	int iSize = vecHeaders.size();
	for(int i = 0; i < iSize; i++) {
		oss << vecHeaders.at(i) << "\r\n";
	}

	return oss.str();
}

int SendDataByPost(const int iSockFd, const string& strPath, const string& strHttpHeader, const string& strBody) {

	ostringstream oss;
	oss.str(""); oss.clear(); // Initialize

	oss << "POST " << strPath << " HTTP/1.1\r\n";
	oss << strHttpHeader;
	oss << "Content-Length: " << strBody.length() << "\r\n";
	oss << "\r\n" << strBody;

	string strSendMesg = oss.str();
#ifdef _HTTP_DEBUG
	cerr << "Send Message:\n" << strSendMesg << endl;
#endif
	if(send(iSockFd, strSendMesg.c_str(), strSendMesg.length(), 0) < 0) {
		return SEND_MESG_FAILED;
	}

	return HTTP_SUCCESS;
}

string ReceiveData(const int iSockFd, int iMaxRetry) {
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

void CloseSocket(int iSockFd) {
	close(iSockFd);
}

string GetErrorMesg(int iErrorCode) {
	switch(iErrorCode) {
		case CONNECT_FAILED:
			return "[ERROR] Connect Failed";
		case CONNECT_TIMEOUT:
			return "[ERROR] Connect Timeout";
		case SEND_MESG_FAILED:
			return "[ERROR] Send Message Failed";
		case RECV_MESG_FAILED:
			return "[ERROR] Receive Message Failed";
		default:
			return "[ERROR] Unknow Error Occurred";
	}
}

} // namespace HTTPUTIL
