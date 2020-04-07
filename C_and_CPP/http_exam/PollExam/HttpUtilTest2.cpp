#include <iostream>
#include <vector>

#include "HttpUtil.h"

using namespace std;

int main(void) {

	string strMethod = "POST";
	string strIP = "127.0.0.1";
	int iPort = 5000;
	string strPath = "/get_data";
	string strProtocol = "HTTP/1.1";
	vector<string> vecHeaders;
	vecHeaders.clear();
	vecHeaders.push_back("Host: 127.0.0.1:5000");
	vecHeaders.push_back("Content-Type: application/json;");

	int iConnect_Timeout_MS = 2000;
	int iRecv_Timeout_MS = 2000;

	int iSockFd = -1;
	iSockFd = HTTPUTIL::ConnectWithTimeout(strIP.c_str(), iPort, iConnect_Timeout_MS, iRecv_Timeout_MS);

	if(iSockFd < 0) {
		cerr << HTTPUTIL::GetErrorMesg(iSockFd) << endl;
		HTTPUTIL::CloseSocket(iSockFd);
		return 0;
	}

	string strHttpHeader = HTTPUTIL::MakeHttpHeader(vecHeaders);

	int iResult = -1;
	string strBody = "{\"in\":1, \"out\":2}\n";
	iResult = HTTPUTIL::SendDataByPost(iSockFd, strPath, strHttpHeader, strBody);

	if(iResult == HTTPUTIL::SEND_MESG_FAILED) {
		HTTPUTIL::GetErrorMesg(iResult);
		HTTPUTIL::CloseSocket(iSockFd);
		return 0;
	}

	string strResponse = HTTPUTIL::ReceiveData(iSockFd);
	cerr << "Response: " << strResponse << endl;

	HTTPUTIL::CloseSocket(iSockFd);

	return 0;
}
