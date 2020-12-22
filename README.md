# Remote-SQL-Server-using-C-using-TCP-and-socket-programming
Remote SQL Server using C using TCP that allows the clients to send queries and to a server and executes the sql commands.

Type out the sql commands on the client side and gets it executed on the server side. For queries that print out something, it will be written to glad.txt and from that, a file transer server and client can be used which send it to the sever from the client.

arguments that are supposed to be given for the server is the port number.
arguments that are supposed to be given for the client are the ip address and the port number.


Eg. ./server 9000 and 
./client 127.0.0.1 9000

