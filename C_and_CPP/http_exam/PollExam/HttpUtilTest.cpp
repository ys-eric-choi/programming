#include <iostream>

#include "HttpUtil.h"

using namespace std;

int main(void) {

	HttpUtil *httputil = new HttpUtil;

	string strIP = "127.0.0.2";
	int iPort = 5000;
	string strAPI = "/get_data";
	string strHeader = "Content-Type: application/json";

	int iConnect_Timeout_MS = 2000;
	int iRecv_Timeout_MS = 2000;

	int iSockFd = -1;
	iSockFd = httputil->ConnectWithTimeout(strIP.c_str(), iPort, iConnect_Timeout_MS, iRecv_Timeout_MS);

	if(iSockFd < 0) {
		cerr << httputil->GetErrorMesg(iSockFd) << endl;
		httputil->CloseSocket(iSockFd);
		return 0;
	}

	string strHttpHeader = httputil->MakeHttpHeader(strIP, iPort, strAPI, strHeader);

	int iResult = -1;
	string strBody = "{\"in\": \"HI\"}";
	iResult = httputil->SendDataByPost(iSockFd, strHttpHeader, strBody);

	if(iResult < 0) {
		httputil->GetErrorMesg(iResult);
		httputil->CloseSocket(iSockFd);
		return 0;
	}

	string strResponse = httputil->ReceiveData(iSockFd);
	cerr << "Response: " << strResponse << endl;

	httputil->CloseSocket(iSockFd);

	delete httputil;

	return 0;
}
