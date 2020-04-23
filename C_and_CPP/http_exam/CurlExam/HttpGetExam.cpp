#include <string>
#include <iostream>
#include <curl/curl.h>

using namespace std;

size_t callBackFunk(char* ptr, size_t size, size_t nmemb, string* stream)
{
    int realsize = size * nmemb;
    stream->append(ptr, realsize);
    return realsize;
}

string url_get_proc(const char url[])
{
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    string chunk;

    if (curl)
        {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callBackFunk);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (string*)&chunk);
        curl_easy_setopt(curl, CURLOPT_PROXY, "");
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        }
    if (res != CURLE_OK) {
        cout << "curl error" << endl;
        //exit(1);
    }

    return chunk;
}

int main (int argc,char *argv[])
{
    string url_target = "http://127.0.0.1:5000/_get_data/?in=";
    string strIn = "Hi";

    url_target += strIn;

    string str_out = url_get_proc(url_target.c_str());

    cout << str_out << "\n";

    return 0;
}

