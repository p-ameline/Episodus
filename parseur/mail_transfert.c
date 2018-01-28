#define MAIN
#include "mimelt/mimelite.h"
#include "mail_transfert.h"

// mailserver
#define MAILSERVER_HOSTNAME "localhost"
#define MAILSERVER_PORTNUMBER 110
#define MAILSERVER_USER "USER prometheus\r\n" // do not remove USER nor \r\n                                                                                                                                                         #define XML_FILE_NAME "test.xml"

#define MAILSERVER_PASS "PASS NAUTILUS\r\n" // do not remove PASS nor \r\n

// buffer : should be ok as is
#define BUFFER_INITIAL_SIZE 8192

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <sys/ioctl.h>

#include "../module0/module0/lifeline.h"
#include "./file_names.h"
#include <fstream.h>
#include <iostream.h>

bool parse_message (string message, lifeLine* pLifeLine, int connfd);

bool StockErrorMessage (char* message)
{
                ofstream outfile;
                string chaine;

                string sName = string ("ErrorParseur.msg");

                chaine =  string (message) + "\n************* fin du message non parsé *************\n \n ";

                outfile.open (sName.c_str(), ofstream::app);

                if (outfile)
                {
                        outfile << chaine + "\n";
                        outfile.close();
                        return true;
                }
                else
                        return false;
}

bool LogErrorMessage (char* message)
{
        ofstream outfile;

        // ecriture dans
        string sName = string ("Messages.log");
        outfile.open (sName.c_str(), ofstream::app);

        if (!outfile)
	        return false;

        struct tm* tp;
        long t;
        char msg[255];

        time (&t);
        tp = localtime (&t);
        strcpy (msg, asctime (tp));

        string sDebut = string(msg, 0, strlen(msg)-1) + string(" ") + string(message);

        outfile << sDebut + "\n";

        outfile.close();

        return true;
}

int connect_to_mailserver (char *hostname, int port)
{
	int connfd;
	struct sockaddr_in sin;
	struct hostent *hp;

	// resolve hostname
	hp = gethostbyname (hostname);
	if (!hp)
	{
		fprintf (stderr, "DNS error.\n", hostname);
		exit (1);
	}

	// build address data structure
	bzero ((char *) &sin, sizeof(sin));
	sin.sin_family = AF_INET;
	bcopy(hp->h_addr, (char *) &sin.sin_addr, hp->h_length);
	sin.sin_port = htons(port);

	// active open
	connfd = socket (PF_INET, SOCK_STREAM,IPPROTO_TCP);
	if (connfd < 0)
	{
		fprintf(stderr, "Socket creation failed.\n");
		exit (1);
	}

	if (connect(connfd, (struct sockaddr *) &sin, sizeof(sin)) < 0)
	{
	  // fprintf(stderr, "Connection to mail server failed.\n");
		fprintf(stderr, strerror(errno));
		exit(1);
	}

	return connfd;
}

void receive_one_line(int connfd, char *buffer, int size)
{
	char *cursor;
	int	len = 0;
	memset(buffer, 0, size);
	while ((cursor = strstr(buffer, "\r\n")) == NULL)
		len += read(connfd, buffer+len, size-len);
	*cursor = '\0';
}

void receive_multiple_lines(int connfd, char *buffer, int size)
{
	char *cursor;
	int	len = 0;
	memset(buffer, 0, size);
	while ((cursor = strstr(buffer, "\r\n.\r\n")) == NULL)
	{
		if (len == size)
		{
			fprintf(stderr, "Buffer exceeded.\n");
			exit(1);
		}
		len += read(connfd, buffer+len, size-len);
	}
	*cursor = '\0';
}

int find_match(char *buf, char *str, char *delim, char *out)
{
	char		*pos;
	pos = strstr(buf, str);

	if (pos != NULL)
	{
		pos += strlen(str); /* move pointer after the keyword */

		while (*pos == ' ' || *pos == '\t')
			pos++; /* skip any tabs or spaces between keyword and value */

		while (*pos != *delim && *pos != '\0')
		/* move along as long as we dont see a null or the delimeter */
		{
			*out = *pos++;
			out++;
		}
		*out = '\0'; /* terminate the copied value */
		return 1; /* keyword was found */
	}
	return 0; /* keyword was not found */
}

int parse_mails (lifeLine* pLifeLine)
{
        int connfd;
        char *buff, *buff2;
        int buffsize=BUFFER_INITIAL_SIZE;
        int nbmessages;
        int messagesize;
        int indexmessage;
        char *message_header, *message_body;
        // MYSQL mysql;
        int len;
        bool bGoodMessage;
        string sErreur;

        // connection mysql
        // mysql_init(&mysql);
        // if (!mysql_real_connect(&mysql,MYSQLSERVER_NAME,MYSQLSERVER_LOGIN,MYSQLSERVER_PASSWORD,MYSQLSERVER_DATABASE,0,NULL,0))
        // {
                // fprintf(stderr, "Failed to connect to database: Error: %s\n", mysql_error(&mysql));
                // exit(1);
        // }

        // buffer
        buff = (char *) malloc (buffsize);
        if (!buff)
        {
                fprintf(stderr, "Not enough memory\n");
                exit(1);
        }
        buff2 = (char *) malloc (buffsize*2);
        if (!buff2)
        {
                fprintf(stderr, "Not enough memory\n");
                exit(1);
        }

        // connection
        connfd = connect_to_mailserver (MAILSERVER_HOSTNAME, MAILSERVER_PORTNUMBER);
        receive_one_line(connfd, buff, buffsize);
        fprintf(stderr, "Connexion mailserveur, file descriptor %i\n", connfd);

        // user
        write(connfd, MAILSERVER_USER, strlen(MAILSERVER_USER));
        receive_one_line(connfd, buff, buffsize);

        // password
        write(connfd, MAILSERVER_PASS, strlen(MAILSERVER_PASS));
        receive_one_line(connfd, buff, buffsize);
        if (strncmp(buff, "-ERR", 4) == 0)
        {
                fprintf(stderr, "Error logging into the mail server\n");
                exit(1);
        }

        // nb messages
        write(connfd, "STAT\r\n", 6);
        receive_one_line(connfd, buff, buffsize);
        strtok(buff, " ");
        nbmessages =  atoi(strtok(NULL, " "));

        // Récupération message par message
        for (indexmessage=1; indexmessage<=nbmessages; indexmessage++)
        {
                // message size
                fprintf(stderr, "Appel message %i\n", indexmessage);
                sprintf (buff, "LIST %i\n", indexmessage);
                write(connfd, buff, strlen (buff));
                receive_one_line(connfd, buff, buffsize);
                fprintf(stderr, "Message %i, LIST :\n%s\n", indexmessage, buff);
                strtok(buff, " ");
                strtok(NULL, " ");
                messagesize = atoi(strtok(NULL, " "));
                fprintf(stderr, "Message %i, messagesize :\n%i\n", indexmessage, messagesize);

                // resize buffer
                if (buffsize<messagesize+BUFFER_INITIAL_SIZE)
                {
                        buffsize=messagesize+BUFFER_INITIAL_SIZE;
                        buff = (char *) realloc (buff, buffsize);
                        if (!buff)
                        {
                                fprintf(stderr, "Not enough memory to download message %i\n", indexmessage);
                                exit(1);
                        }
                        buff2 = (char *) realloc (buff2, buffsize*2);
                        if (!buff2)
                        {
                                fprintf(stderr, "Not enough memory to download message %i\n", indexmessage);
                                exit(1);
                        }
                }

                // message
                sprintf (buff, "RETR %i\n", indexmessage);
                write(connfd, buff, strlen (buff));
                receive_multiple_lines(connfd, buff, buffsize);
                fprintf(stderr, "Message %i, RETR done\n", indexmessage);

                message_header = strstr(buff, "\r\n");
                message_header += 2;

                // message_body = strstr(buff, "\r\n\r\n");
                // *message_body = '\0';
                // message_body += 4;

                // sprintf (buff2, "insert into %s (%s, %s, %s, %s, %s, %s) values ('", MYSQLSERVER_TABLE, MYSQLSERVER_HEADER, MYSQLSERVER_BODY, MYSQLSERVER_FROM, MYSQLSERVER_DATE, MYSQLSERVER_SUBJECT, MYSQLSERVER_TO);
                // len=mysql_real_escape_string(&mysql, &buff2[strlen(buff2)], message_header, strlen(message_header));
                // sprintf (&buff2[strlen(buff2)], "','");
                // len=mysql_real_escape_string(&mysql, &buff2[strlen(buff2)], message_body, strlen(message_body));
                // sprintf (&buff2[strlen(buff2)], "','");
                //find_match(message_header, "From:", "\n", &buff2[strlen(buff2)]);
                //sprintf (&buff2[strlen(buff2)], "','");
                //find_match(message_header, "Date:", "\n", &buff2[strlen(buff2)]);
                //sprintf (&buff2[strlen(buff2)], "','");
                //find_match(message_header, "Subject:", "\n", &buff2[strlen(buff2)]);
                //sprintf (&buff2[strlen(buff2)], "','");
                //find_match(message_header, "To:", "\n", &buff2[strlen(buff2)]);
                //sprintf (&buff2[strlen(buff2)], "','");
                //find_match(message_header, "Content-Type:", "\n", &buff2[strlen(buff2)]);
                //sprintf (&buff2[strlen(buff2)], "')");
                //fprintf(stderr, "Message %i, header : %s\n", indexmessage, buff2);

                // if(mysql_query(&mysql, buff2))
                // {
                        // fprintf(stderr, "Error: %s\nmessage %i is probably too large\nother messages should have been received\n", mysql_error(&mysql), indexmessage);
                // }
                // else

                // mime decoding

                bGoodMessage = true;
                // Le message a-t-il un content type
                if (find_match(message_header, "Content-Type:", "\n", &buff2[0]))
                {
                        fprintf(stderr, "Message %i, content-type : %s\n", indexmessage, buff2);
                        // Si ce n'est pas text/plain, on le supprime
                        if (strstr(buff2, "text/plain") == NULL)
                        {
                                sErreur = string("Type de message invalide : ") + string(buff2);
                                buff2[0] = '\0';
                                if (find_match(message_header, "From:", "\n", &buff2[0]))
                                        sErreur += string(" (emetteur : ") + string(buff2) + string(")");

                                LogErrorMessage ((char*) sErreur.c_str());

                                sprintf (buff, "DELE %i\n", indexmessage);
                                write(connfd, buff, strlen (buff));
                                receive_one_line(connfd, buff, buffsize);

                                bGoodMessage = false;
                        }
                }

                if (bGoodMessage)
                {
                        fprintf(stderr, "Message %i, message bon\n", indexmessage);

                        unsigned char *ss;
                        int chset1, chset2, xx, yy, state, endheader;

                        /*
                        | Set up translation parameters for folding.  The input character set
                        | for the body is to be set later (by parsing the MIME header, but news
                        | sometimes contains ISO Latin 1 headers, so that is what we set iset
                        | to for headers processing.  The target character set should be set to
                        | the default native set of the computer it is running on (e.g. the code
                        | page the machine is set up to use).  I want this demonstrator to be
                        | easy portable, so I ask the user about this at runtime.
                        */
                        if (!ml_foldinit(CS_ISOL1, CS_ISOL1)) {
                                fputs("*** bad o/p charset\n", stderr);
                        }
                        else
                        {
                                /*
                                | Set up for parsing header and unmiming a new message.
                                */
                                ml_unmimsetup(CS_ISOL1, CE_PRSHEAD);

                                fputs("============================================================================\n",  stderr);
                                /*
                                | And feed entire message to ml_unmimeline, one line at the time.
                                */
                                ss = (unsigned char *)message_header;
                                xx = 0;
                                yy = 0;
                                endheader=0;
                                while (*ss) {
                                        buff2[xx++] = *ss;
                                        if (xx == buffsize*2-1)
                                        {
                                                fprintf(stderr, "Error: buffer overflow\nmessage %i is probably too large\nother messages should have been received\n", indexmessage);
                                        }
                                        if ((*ss == '\n')) {
                                                buff2[xx++] = '\0';
                                                //fputs(&buff2[yy], stderr);
                                                state = ml_unmimeline((unsigned char *)(&buff2[yy]));
                                                //fputs(&buff2[yy], stderr);
                                                if (state == 1) {
                                                        endheader=1;
                                                } /* if (end of header */
                                                if (endheader == 1) {
                                                        // Attention, après ml_unmimeline, le bout de chaine peut avoir rétréci ; il faut donc rechercher le '\0'
                                                        //xx--;
                                                        //yy=xx;
                                                        while (buff2[yy])
                                                            yy++;
                                                        xx = yy;
                                                } /* if (end of header */
                                                else {
                                                        xx = 0;
                                                }
                                        } /* eoln */
                                        ss++;
                                } /* while */
                                if (xx) {
                                        buff2[xx++] = '\0';

                                        if (yy && (yy < xx - 1))
                                        {
                                            fputs(&buff2[yy], stderr);
                                            fputs("\n",  stderr);
                                            state = ml_unmimeline((unsigned char *)(&buff2[yy]));
                                            fputs(&buff2[yy], stderr);
                                            fputs("\n",  stderr);
                                            fputs(buff2, stderr);
                                        }

                                        /* fputs(buff2, stderr); */
                                } /* if */

                                xx = strlen(buff2);
                                if (xx) xx--;
                                /*if (buff2[xx] != '\n') fputc('\n', stderr);*/
                                fputs("============================================================================\n",  stderr);

                                // /mime decoding

                                bool bTuerMessage = true;
                                if (!parse_message (buff2, pLifeLine, connfd))
                                {
                                        if (!StockErrorMessage (buff2))
                                                bTuerMessage = false;
                                }
                                if (bTuerMessage)
                                {
                                    fprintf(stderr, "Destruction du message %i\n", indexmessage);
                                    sprintf (buff, "DELE %i\n", indexmessage);
                                    write(connfd, buff, strlen (buff));
                                    receive_one_line(connfd, buff, buffsize);
                                    fprintf(stderr, "Message %i détruit\n", indexmessage);
                                }
                        }
                }

                // indication au superviseur que le module est toujours vivant
                pLifeLine->heartBeat();

        }

        // quit
        write(connfd, "QUIT\n", 5);
        receive_one_line(connfd, buff, buffsize);

        // disconnect
        close (connfd);

        // free buffer
        free(buff);
        free(buff2);

        // disconnect mysql
        // mysql_close(&mysql);

        // exit (0);
        return 1;
}
