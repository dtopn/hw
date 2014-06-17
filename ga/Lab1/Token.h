#ifndef _TOKEN_H_
#define _TOKEN_H_

/* token structre */
struct Token {
	int id;
	int in_stamp;
	int out_stamp;
	char msg[81];
};

#endif //_TOKEN_H_