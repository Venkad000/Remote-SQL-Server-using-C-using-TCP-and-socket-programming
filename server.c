#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <mysql.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[1024];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
     void finish_with_error(MYSQL *con){
	     fprintf(stderr,"%s\n",mysql_error(con));
	     mysql_close(con);
	     exit(1);
	}
     MYSQL *con = mysql_init(NULL);
     if (con == NULL)
  {
      fprintf(stderr, "%s\n", mysql_error(con));
      exit(1);
  }
      if (mysql_real_connect(con, "localhost", "user12", "34klq*",
          "testdb", 0, NULL, 0) == NULL)
  {
      finish_with_error(con);
  }

     while(1)
     {
           bzero(buffer,1023);
           n = read(newsockfd,buffer,1023);
           if (n < 0) error("ERROR reading from socket");
           printf("Client: %s\n",buffer);
	   if (mysql_query(con,buffer)){
		   finish_with_error(con);
		}

	   FILE *fp;
           fp = fopen("glad.txt","a+");
	   MYSQL_RES *result = mysql_store_result(con);

 	 if (result == NULL)
 	 {
     	 finish_with_error(con);
        	}

 	 int num_fields = mysql_num_fields(result);

 	 MYSQL_ROW row;

 	 while ((row = mysql_fetch_row(result)))
  	{
      	for(int i = 0; i < num_fields; i++)
     	 {
          printf("%s ", row[i] ? row[i] : "NULL");
          fputs(row[i] ? row[i] : NULL ,fp);
     		 }

     		 fputs("\n",fp);
	  }

	  fclose(fp);


          bzero(buffer,1023);
          strcpy(buffer,"Task done");
          write(newsockfd,buffer,strlen(buffer));
           int i=strncmp("Bye" , buffer, 3);
           if(i == 0)
               break;
     }
//     mysql_free_result(result);
      mysql_close(con);
     close(newsockfd);
     close(sockfd);
     return 0; 
}
