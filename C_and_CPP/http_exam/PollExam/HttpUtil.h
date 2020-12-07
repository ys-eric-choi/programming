#ifndef HTTP_UTIL_H
#define HTTP_UTIL_H

#include <iostream>
#include <sstream>
#include <vector>
#include <string.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>

#include <poll.h>
#include <fcntl.h>
#include <errno.h>

using namespace std;

//#define _HTTP_DEBUG

namespace HTTPUTIL {

class HttpResponse {
public:
	int	iHttpCode;
	int	iContLen;
	string	strProtocol;
	string	strRespMesg;
	string	strHttpBody;

	void	PrintResponse() {
		cerr << "Protocol: " << strProtocol << endl;
		cerr << "HttpCode: " << iHttpCode << endl;
		cerr << "Response Message: " << strRespMesg << endl;
		cerr << "Content-Length: " << iContLen << endl;
		cerr << "HttpBody: " << strHttpBody << endl;
	}

	HttpResponse() {
		iHttpCode = 0;
		iContLen = -1;
		strProtocol = "";
		strRespMesg = "";
		strHttpBody = "";
	}
	~HttpResponse() {}
};

const int HTTP_SUCCESS	= 1;

const int CONNECT_FAILED	= -101;
const int CONNECT_TIMEOUT	= -102;
const int SEND_MESG_FAILED	= -103;
const int RECV_MESG_FAILED	= -104;
const int RECV_TIMEOUT		= -105;

const int NUM_POLL_FD		= 1;
const int RECV_BUF_SIZE		= 2048;

bool CheckTimeOut(struct timeval& startTime, int iRecv_Timeout_MS) {
	struct timeval endTime, diffTime;
	gettimeofday(&endTime, NULL);

	diffTime.tv_sec = endTime.tv_sec - startTime.tv_sec;
	diffTime.tv_usec = endTime.tv_usec - startTime.tv_usec;

	if (diffTime.tv_usec < 0) {
		diffTime.tv_sec = diffTime.tv_sec - 1;
		diffTime.tv_usec = diffTime.tv_usec + 1000000;
	}

	int iDiffTime = (diffTime.tv_sec * 1000) + (diffTime.tv_usec / 1000);
#ifdef _HTTP_DEBUG
	cerr << "Receive Timeout Check: " << iDiffTime << endl;
#endif
	if(iRecv_Timeout_MS < iDiffTime) return true;
	else return false;
}

int ConnectWithTimeout(const char* strIP, const int iPort, int iConnect_Timeout_MS, int iRecv_Timeout_MS, int &iSockFd) {

	iSockFd = -1;
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
	cerr << "\nSend Message:\n" << strSendMesg << endl;
#endif
	if(send(iSockFd, strSendMesg.c_str(), strSendMesg.length(), 0) < 0) {
		return SEND_MESG_FAILED;
	}

	return HTTP_SUCCESS;
}

int SendDataByGet(const int iSockFd, const string& strPath, const string& strHttpHeader) {

	ostringstream oss;
	oss.str(""); oss.clear(); // Initialize

	oss << "GET " << strPath << " HTTP/1.1\r\n";
	oss << strHttpHeader << "\r\n";

	string strSendMesg = oss.str();
#ifdef _HTTP_DEBUG
	cerr << "Send Message:\n" << strSendMesg << endl;
#endif
	if(send(iSockFd, strSendMesg.c_str(), strSendMesg.length(), 0) < 0) {
		return SEND_MESG_FAILED;
	}

	return HTTP_SUCCESS;
}

int ReceiveData(const int iSockFd, int iRecv_Timeout_MS, HttpResponse& httpResponse) {

	int iRecvLen = 0;
	char strRecvBuf[RECV_BUF_SIZE] = {"\0",};
	string strReceiveData = "";

	struct timeval startTime;
	gettimeofday(&startTime, NULL);

	do {
		if(CheckTimeOut(startTime, iRecv_Timeout_MS)) return RECV_TIMEOUT;

		iRecvLen = recv(iSockFd, strRecvBuf, RECV_BUF_SIZE - 1, 0);

		if(iRecvLen < 1) break;

		if(iRecvLen < RECV_BUF_SIZE) strRecvBuf[iRecvLen] = '\0';
		else strRecvBuf[RECV_BUF_SIZE - 1] = '\0';

		strReceiveData += strRecvBuf;

	} while(iRecvLen > 0);
#ifdef _HTTP_DEBUG
	cout << "\nReceive Data:\n" << strReceiveData << endl;
#endif
	if(strReceiveData == "") return RECV_TIMEOUT;

	char *head = NULL, *head_buf = NULL;
	if((head = strtok_r((char*)strReceiveData.c_str(), "\r\n", &head_buf)) == NULL) return RECV_MESG_FAILED;

	char *protocol = NULL, *code = NULL, *first_buf = NULL;
	if((protocol = strtok_r(head, " ", &first_buf)) == NULL) return RECV_MESG_FAILED;
	if((code = strtok_r(NULL, " ", &first_buf)) == NULL) return RECV_MESG_FAILED;

	httpResponse.strProtocol = protocol;
	httpResponse.iHttpCode = atoi(code);
	httpResponse.strRespMesg = first_buf;

	while((head = strtok_r(NULL, "\r\n", &head_buf)) != NULL) {
		if(!strncmp(head, "Content-Length:", 15)) {
			httpResponse.iContLen = atoi(head + 16);
			break;
		}
	}

	if(httpResponse.iContLen == -1) return RECV_MESG_FAILED;

	char *strGetBody = NULL;
	if((strGetBody = strstr(head_buf, "\n\r")) != NULL) {
		strGetBody += 3;
#ifdef _HTTP_DEBUG
		cerr << "Body: " << strGetBody << endl;
#endif
		int iBodyLen = strlen(strGetBody);
		if(httpResponse.iContLen != iBodyLen) return RECV_MESG_FAILED;

		httpResponse.strHttpBody = strGetBody;
#ifdef _HTTP_DEBUG
		httpResponse.PrintResponse();
#endif
	} else {
		return RECV_MESG_FAILED;
	}

	return HTTP_SUCCESS;
}

void CloseSocket(int iSockFd) {
	close(iSockFd);
}

string GetErrorMesg(int iErrorCode) {
	switch(iErrorCode) {
		case CONNECT_FAILED:
			return "Connection Failed";
		case CONNECT_TIMEOUT:
			return "Connect Timeout";
		case SEND_MESG_FAILED:
			return "Failed to Send Message";
		case RECV_MESG_FAILED:
			return "Failed to Receive Message";
		case RECV_TIMEOUT:
			return "Receive Timeout";
		default:
			return "Unknown Error Occurred";
	}
}

} // namespace HTTPUTIL
#endif
