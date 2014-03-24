#ifndef __HTTP_PROTOCOL_H
#define __HTTP_PROTOCOL_H

#define STR_OK_200 "HTTP/1.0 200 OK"

#define SUCCESS 0
#define FAIL -1

/*
typedef enum{
  OK_200,
  NOT_EXIST_404
  
}HtmlResponseHeader ; 
/*
/*
 * HTTP ERROR MESSAGE
 * HTTP/1.0 200 OK 
 * HTTP/1.0 404 
 * ...
 */

typedef enum{   
    SOCKET_Error_NO_ERROR,
    SOCKET_Error_SOCKET,
    SOCKET_Error_CONNECT,
    SOCKET_Error_SEND,
    SOCKET_Error_READ,
    SOCKET_Error_WRITE
    
}SocketError;

#endif