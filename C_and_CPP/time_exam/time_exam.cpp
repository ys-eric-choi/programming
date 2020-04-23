#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

int main(void) {

	// 1. 현재 시간 구하기
	time_t t;
	time(&t);

	// thread-unsafe 한 방법
	//struct tm *start_time;
	//start_time = localtime(&t);

	// thread-safe한 방법
	struct tm start_time;
	localtime_r(&t, &start_time);

	printf("지금시간: %04d-%02d-%02d %02d:%02d:%02d\n", start_time.tm_year + 1900, start_time.tm_mon + 1, start_time.tm_mday, start_time.tm_hour, start_time.tm_min, start_time.tm_sec);


	// 2. 시간을 문자열로 변환
	char strEndTime[64];
	int result_size;

	result_size = strftime(strEndTime, sizeof(strEndTime), "Today is %A, %b %d.\nTime:  %r", &start_time);
	printf("%d characters written.\n%s\n", result_size, strEndTime);


	// 3. 시간 차이 구하는 함수
	double diff_time = 0;
	time_t start, end;

	time(&start);
	sleep(1);
	time(&end);

	diff_time = difftime(end, start);
	printf("%f\n", diff_time);


	// 4. 문자열을 시간으로 변환
	char strInTime[64];
	strcpy(strInTime, "20200423151400");

	struct tm target_time;
	strptime(strInTime, "%Y%m%d%H%M%S", &target_time);

	time_t result = mktime(&target_time);

	// 시간으로 변환한 것과 현재 시간을 다시 비교
	diff_time = difftime(result, start);
	printf("%f\n", diff_time);

	return 0;
}

