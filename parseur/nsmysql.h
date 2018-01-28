#include <stdlib.h>
#include <stdio.h>

#if !defined _OWLDLL

#include <mysql/mysql.h>

int nsmysql_connect();
int nsmysql_query(const char *query);
int nsmysql_query(const char *query, char *&result);
int nsmysql_query(const char *query, char ***&result, int &nb_rows);
char *nsmysql_insert(const char *table, const char *id, const char *fields, const char *values);
int nsmysql_close();

#endif
