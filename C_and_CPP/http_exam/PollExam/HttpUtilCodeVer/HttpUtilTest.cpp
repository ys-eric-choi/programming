#include <iostream>
#include <vector>

#include "HttpUtil.h"

using namespace std;

int main(void) {

	int iConnect_Timeout_MS = 2000;
	int iRecv_Timeout_MS = 2000;
	int iMaxRetry = 3;

	string strIP = "127.0.0.1";
	int iPort = 5000;
	string strProtocol = "HTTP/1.1";
	vector<string> vecHeaders;
	vecHeaders.clear();
	vecHeaders.push_back("Host: 127.0.0.1:5000");
	vecHeaders.push_back("User-Agent: HTTP_UTIL_TESTER");

	// GET
	string strPath = "/get_data?A=1&B=2";

	int iSockFd = -1;
	iSockFd = HTTPUTIL::ConnectWithTimeout(strIP.c_str(), iPort, iConnect_Timeout_MS, iRecv_Timeout_MS);

	if(iSockFd < 0) {
		cerr << HTTPUTIL::GetErrorMesg(iSockFd) << endl;
		HTTPUTIL::CloseSocket(iSockFd);
		return 0;
	}

	string strHttpHeader = HTTPUTIL::MakeHttpHeader(vecHeaders);

	int iResult = -1;
	iResult = HTTPUTIL::SendDataByGet(iSockFd, strPath, strHttpHeader);

	if(iResult == HTTPUTIL::SEND_MESG_FAILED) {
		HTTPUTIL::GetErrorMesg(iResult);
		HTTPUTIL::CloseSocket(iSockFd);
		return 0;
	}

	string strResponse = HTTPUTIL::ReceiveData(iSockFd, iMaxRetry);
	cerr << "Response: " << strResponse << endl;

	HTTPUTIL::CloseSocket(iSockFd);


	// POST
	strPath = "/get_data";
	vecHeaders.push_back("Content-Type: application/json;");
	strHttpHeader = HTTPUTIL::MakeHttpHeader(vecHeaders);

	iSockFd = -1;
	iSockFd = HTTPUTIL::ConnectWithTimeout(strIP.c_str(), iPort, iConnect_Timeout_MS, iRecv_Timeout_MS);

	if(iSockFd < 0) {
		cerr << HTTPUTIL::GetErrorMesg(iSockFd) << endl;
		HTTPUTIL::CloseSocket(iSockFd);
		return 0;
	}

	string strBody = "{\"A\":1, \"B\":2}\n";

	iResult = -1;
	iResult = HTTPUTIL::SendDataByPost(iSockFd, strPath, strHttpHeader, strBody);

	if(iResult == HTTPUTIL::SEND_MESG_FAILED) {
		HTTPUTIL::GetErrorMesg(iResult);
		HTTPUTIL::CloseSocket(iSockFd);
		return 0;
	}

	strResponse = HTTPUTIL::ReceiveData(iSockFd, iMaxRetry);
	cerr << "Response: " << strResponse << endl;

	HTTPUTIL::CloseSocket(iSockFd);

	return 0;
}
