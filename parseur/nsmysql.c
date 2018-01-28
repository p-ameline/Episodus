#include <string.h>
#include "nsmysql.h"

#define NSMYSQL_BUFFER_SIZE 512
#define NSMYSQL_ID_SIZE 10

MYSQL nsmysql;

int nsmysql_connect()
{
	mysql_init(&nsmysql);
	if (!mysql_real_connect(&nsmysql,"localhost","root","","prometheus",0,NULL,0))
	{
		fprintf(stderr, "Failed to connect to database\nError: %s\n", mysql_error(&nsmysql));
		exit(1);
	}
	return 1;
}

int nsmysql_query(const char *query)
{
//	fprintf(stderr, "%s\n", query);

	if(mysql_query(&nsmysql, query))
	{
		fprintf(stderr, "Invalid query: %s\nError: %s\n", query, mysql_error(&nsmysql));
		exit(1);
	}

	return 1;
}

int nsmysql_query(const char *query, char *&result)
{
	MYSQL_RES *myresult;
	MYSQL_ROW myrow;

	nsmysql_query(query);
	myresult = mysql_store_result(&nsmysql);

	if (myresult)
	{
		myrow = mysql_fetch_row (myresult);
		if (myrow[0] != NULL)
		{
			result=new char[strlen (myrow[0])+1];
			strcpy(result,myrow[0]);
			mysql_free_result(myresult);
//			fprintf (stderr, "-> %s\n",result);
			return 1;
		}
	}
	result=new char[2];
	strcpy(result,"1");
	return 0;
}

int nsmysql_query(const char *query, char ***&result, int &nb_rows)
{
	MYSQL_RES *myresult;
	MYSQL_FIELD *myfields;
	MYSQL_ROW myrow;
	int i, j;

	nsmysql_query(query);
	myresult = mysql_store_result(&nsmysql);

	if (myresult)
	{
		nb_rows = mysql_num_rows (myresult);
		result = new char ** [nb_rows+1];

// renvoie le nom des colonnes en ligne 0
		myfields = mysql_fetch_fields(myresult);
		result [0] = new char * [mysql_num_fields (myresult)];
		for (j=0; j<mysql_num_fields (myresult); j++)
		{
			result [0][j] = new char [strlen (myfields[j].name)+1];
			strcpy(result[0][j],myfields[j].name);
		}

		for (i=1; i<=nb_rows; i++)
		{
			myrow = mysql_fetch_row (myresult);
			result [i] = new char * [mysql_num_fields (myresult)];
//			fprintf (stderr, "->");
			for (j=0; j<mysql_num_fields (myresult); j++)
			{
				if (myrow[j] == NULL)
					result [i][j] = NULL;
				else
				{
					result [i][j] = new char [strlen (myrow[j])+1];
					strcpy(result[i][j],myrow[j]);
				}
//				fprintf (stderr, " %s", result[i][j]);
			}
//			fprintf (stderr, "\n");
		}
		mysql_free_result(myresult);
	}

	return nb_rows;
}

int nsmysql_query(const char *query, char **&result)
{
	return 1;
}

char *nsmysql_insert(const char *table, const char *id, const char *fields, const char *values)
{
	char buffer[NSMYSQL_BUFFER_SIZE];
	char *result;
	int myid;

	sprintf (buffer,"lock tables %s write",table);
	nsmysql_query(buffer);
	sprintf (buffer,"select max(abs(%s))+1 from %s",id,table);
	nsmysql_query(buffer,result);
	sprintf (buffer,"insert into %s (%s,%s) values ('%s',%s)",table,id,fields,result,values);
	nsmysql_query(buffer);
	sprintf (buffer,"unlock tables",table);
	nsmysql_query(buffer);

	return result;
}

int nsmysql_close()
{
	mysql_close(&nsmysql);
	return 1;
}
