#ifndef HTTP_UTIL_H
#define HTTP_UTIL_H

#include <vector>

using namespace std;

namespace HTTPUTIL {

const int HTTP_SUCCESS	= 0;
const int CONNECT_FAILED	= -101;
const int CONNECT_TIMEOUT	= -102;
const int SEND_MESG_FAILED	= -103;
const int RECV_MESG_FAILED	= -104;

int ConnectWithTimeout(const char* strIP, const int iPort, int iConnect_Timeout_MS, int iRecv_Timeout_MS);
string MakeHttpHeader(const vector<string>& vecHeaders);
int SendDataByPost(const int iSockFd, const string& strPath, const string& strHttpHeader, const string& strBody);
string ReceiveData(const int iSockFd, int iMaxRetry = 1);
void CloseSocket(int iSockFd);
string GetErrorMesg(int iErrorCode);

} // namespace HTTPUTIL
#endif
