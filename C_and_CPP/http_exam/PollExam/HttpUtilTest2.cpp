#include <iostream>

#include "HttpUtil.h"

using namespace std;

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
		cerr << GetErrorMesg(iSockFd) << endl;
		CloseSocket(iSockFd);
		return 0;
	}

	string strHttpHeader = MakeHttpHeader(strIP, iPort, strAPI, strHeader);

	int iResult = -1;
	string strBody = "{\"in\": \"HI\"}";
	iResult = SendDataByPost(iSockFd, strHttpHeader, strBody);

	if(iResult < 0) {
		GetErrorMesg(iResult);
		CloseSocket(iSockFd);
		return 0;
	}

	string strResponse = ReceiveData(iSockFd);
	cerr << "Response: " << strResponse << endl;

	CloseSocket(iSockFd);

	return 0;
}
