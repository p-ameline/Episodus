#ifndef _MAIL_TRANSFERT
#define _MAIL_TRANSFERT

#include "../module0/module0/lifeline.h"

int parse_mails (lifeLine* pLifeLine);
void receive_one_line(int connfd, char *buffer, int size);

#endif
