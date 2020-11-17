#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

#define ENCODE_LEN 2

bool isAllDigit(const char *strIn, size_t iLen) {
	for(size_t i = 0; i < iLen; i++) {
		if(('0' <= strIn[i]) && (strIn[i] <= '9')) continue;
		else return false;
	}

	return true;
}

long long add_integer(long long currentValue, long long newValue) {
	if(currentValue == 0) return newValue;
	else return currentValue + newValue;
}

long long get_integer_from_hangle(const char *strIn) {
	if(!strncmp(strIn, "일", ENCODE_LEN)) {
		return 1;
	} else if(!strncmp(strIn, "이", ENCODE_LEN)) {
		return 2;
	} else if(!strncmp(strIn, "삼", ENCODE_LEN)) {
		return 3;
	} else if(!strncmp(strIn, "사", ENCODE_LEN)) {
		return 4;
	} else if(!strncmp(strIn, "오", ENCODE_LEN)) {
		return 5;
	} else if(!strncmp(strIn, "육", ENCODE_LEN)) {
		return 6;
	} else if(!strncmp(strIn, "칠", ENCODE_LEN)) {
		return 7;
	} else if(!strncmp(strIn, "팔", ENCODE_LEN)) {
		return 8;
	} else if(!strncmp(strIn, "구", ENCODE_LEN)) {
		return 9;
	} else return -1;

}

long long get_integer_from_hangle_2(const char *strIn) {
	if(!strncmp(strIn, "만", ENCODE_LEN)) {
		return 10000;
	} else if(!strncmp(strIn, "억", ENCODE_LEN)) {
		return 100000000;
	} else if(!strncmp(strIn, "조", ENCODE_LEN)) {
		return 1000000000000;
	} else if(!strncmp(strIn, "경", ENCODE_LEN)) {
		return 10000000000000000;
	} else return -1;
}

long long get_integer_from_hangle_3(const char *strIn) {
	if(!strncmp(strIn, "십", ENCODE_LEN)) {
		return 10;
	} else if(!strncmp(strIn, "백", ENCODE_LEN)) {
		return 100;
	} else if(!strncmp(strIn, "천", ENCODE_LEN)) {
		return 1000;
	} else return -1;
}

long long convert_currency(const char *strIn) {

	long long current_integer = 1, now_int = 0, result = 0, last_result = 0;
	size_t i, si, iLen = strlen(strIn);
	bool isNum = true, isRealNum = false; 
	char buf[64];

	for(i = 0; i < iLen; i++) {
		if(strIn[i] == ' ') {
			continue;
		}

		// 남은 숫자가 모두 정수형일 때
		if(isAllDigit(strIn + i, iLen - i)) {
			result = add_integer(result, atoll(strIn + i));
			return last_result + result;
		}
		// 정수형 처리
		else if(('0' <= strIn[i]) && (strIn[i] <= '9')) {
			if(isRealNum) {
				strncpy(buf, strIn + si, i - si + 1);
				buf[i - si + 1] = '\0';
				current_integer = atoll(buf);
			} else {
				current_integer = strIn[i] - '0';
				si = i;
			}

			if(i == iLen - 1) { // 문자열의 끝일 때
				result = add_integer(result, current_integer);
			}
			isNum = true; 
			isRealNum = true;
		}
		// 일, 이, 삼, 사, 오, 육, 칠, 팔 구 처리
		else if((now_int = get_integer_from_hangle(strIn + i)) != -1) {
			current_integer = now_int;
			i += ENCODE_LEN - 1;
			if(i == iLen - 1) { // 문자열의 끝일 때
				result = add_integer(result, current_integer);
			}
			isNum = true; 
			isRealNum = false;
		}
		// 만, 억 조 단위 처리
		else if((now_int = get_integer_from_hangle_2(strIn + i)) != -1) {
			if(isNum) result += current_integer;

			result *= now_int;
			last_result += result;
			result = 0;
			isNum = false;
			isRealNum = false;
			i += ENCODE_LEN - 1;
		}
		// 십, 백, 천 단위 처리
		else if((now_int = get_integer_from_hangle_3(strIn + i)) != -1) {
			if(isNum) {
				current_integer *= now_int;
				result = add_integer(result, current_integer);
			} else {
				result = add_integer(result, now_int);
			}
			isNum = false;
			isRealNum = false;
			i += ENCODE_LEN - 1;
		} else break;
	}

	return last_result + result;
}

int main(void) {

	string strIn = "";
	
	FILE *fp = stdin;

	char in_buf[1024];
	long long result = 0;
	char out_buf[64];
	char last_out_buf[64];

	while(fgets(in_buf, 1024, fp) != NULL) {
		in_buf[strlen(in_buf) - 1] = '\0';
		if(!strcmp(in_buf, "QUIT")) break;

		result = convert_currency(in_buf);
		//cerr << result << endl;
		sprintf(out_buf, "%lld", result);
		//cerr << out_buf << endl;

		int i, j = 0, k = 0, x, y, len;
		len = strlen(out_buf);
		x = len / 3;
		y = len % 3;

		if(y > 0) {
			for(i = 0; i < y; i++) {
				last_out_buf[j++] = out_buf[k++];
			}
			last_out_buf[j++] = ',';
		}
		for(i = 0; i < x; i++) {
			last_out_buf[j++] = out_buf[k++];
			last_out_buf[j++] = out_buf[k++];
			last_out_buf[j++] = out_buf[k++];
			last_out_buf[j++] = ',';
		}
		last_out_buf[j - 1] = '\0';

		cerr << last_out_buf << endl;
	}

	return 0;
}
