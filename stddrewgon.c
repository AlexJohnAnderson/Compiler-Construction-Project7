#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>

/*
Technically, this only creates a random 32-bit value,
but that's good enough for the mayhem semantics
*/
int64_t mayhem(){
	static int init = 0;
	if (init == 0){
		srand(time(NULL));
		init = 1;
	}
	return rand();
}

void printBool(int64_t c){
	if (c == 0){
		fprintf(stdout, "false");
	} else{
		fprintf(stdout, "true");
	}
	fflush(stdout);
}

void printInt(long int num){
	fprintf(stdout, "%ld", num);
	fflush(stdout);
}

void printString(const char * str){
	fprintf(stdout, "%s", str);
	fflush(stdout);
}

int64_t getBool(){
	char c;
	scanf("%c", &c);
	getchar(); // Consume trailing newline
	if (c == '0'){
		return 0;
	} else {
		return 1;
	}
}

int64_t getInt(){
	char buffer[32];
	fgets(buffer, 32, stdin);
	long int res = atol(buffer);
	return res;
}
