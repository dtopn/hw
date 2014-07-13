#ifndef _TOKEN_H_
#define _TOKEN_H_

#define MAX_TOKEN_MSG_LEN 80
/* token structre */
struct Token {
	int id;
	long long in_stamp;
	long long out_stamp;
	char msg[MAX_TOKEN_MSG_LEN + 1];
};

#endif //_TOKEN_H_
