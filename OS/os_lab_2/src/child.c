#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_DIGIT_IN_STIRING	100

int main(int argc, const char* argv[]) {
	char c;
	char buf[MAX_DIGIT_IN_STIRING];
	int count_digit = 0;
	int number = 0;
	int result = 0;
	int f_not_left_minus = 0;
	int f_ch = 1;

	memset(buf, '\0', MAX_DIGIT_IN_STIRING);

	while (true) {
		f_ch = read(STDIN_FILENO, &c, sizeof(c));
		if (f_ch == 0){
			return 0;
		}
		if (isdigit(c) || c == '-') {
			if (c == '-' && count_digit  > 0){	//обработка ситуаций типа "1-1"
				f_not_left_minus = 1;
			}
			buf[count_digit] = c;
			count_digit++;
		}
		if (c == ' ' || c == '\n') {
			if (f_not_left_minus == 1){
				memset(buf, '\0', count_digit);
				count_digit = 0;
				f_not_left_minus = 0;
			}else
				if (count_digit > 0 || count_digit >= MAX_DIGIT_IN_STIRING) {
					number = atoi(buf);
					result += number;
					memset(buf, '\0', count_digit);
					count_digit = 0;
				}
		}
		if(c == '\n') {
            write(STDOUT_FILENO, &result, sizeof(result));
			result = 0;
		}
	}
}
