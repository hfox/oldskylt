#include <stdlib.h>

int myatoi(char *s)
{
	int value = 0;
	int mult = 10;
	if(*s == '0') {
		s++;
		if(*s == 'x') {
			s++;
			mult = 16;
		} else {
			mult = 8;
		}
	}
	while(*s) {
		if(isalpha(*s)) {
			value = value * mult + (10 + toupper(*s) - 'A');
		} else {
			value = value * mult + *s - '0';
		}
		s++;
	}
	return value;
}

