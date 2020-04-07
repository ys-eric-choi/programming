#ifndef HTTP_UTIL_H
#define HTTP_UTIL_H
#include <iostream>

using namespace std;

class HttpUtil {
public:

	HttpUtil();
	~HttpUtil();

	int ConnectWithTimeout(const char* strIP, const int iPort, int iConnect_Timeout_MS, int iRecv_Timeout_MS);
	string MakeHttpHeader(const string& strIP, const int iPort, const string& strAPI, const string& strHeader);
	int SendDataByPost(const int iSockFd, const string& strHttpHeader, const string& strBody);
	string ReceiveData(const int iSockFd, int iMaxRetry = 1);
	void CloseSocket(int iSockFd);
	string GetErrorMesg(int iErrorCode);

};
#endif
