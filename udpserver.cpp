#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include "headers/udpserver.h"

#define BUFFER_SIZE 2048

bool UdpServer::serve(){
  const std::string logline = "Serving on UDP( port=" + std::to_string( this->port ) + " )";
  this->log( logline.c_str() );
  char buffer[ BUFFER_SIZE ] = {0};
  struct sockaddr_in address, client;
  int sockFd = socket( AF_INET, SOCK_DGRAM, 0 );
  socklen_t length = sizeof( client );
  int bindInd, N;
  if( sockFd < 0 ){
    this->error( "Could not create socket" );
  }else{
    memset( &address, 0, sizeof(address) );
    memset( &client, 0, sizeof(client) );
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( this->port );
    bindInd = bind( sockFd, (const struct sockaddr *)&address, sizeof(address) );
    if( bindInd < 0 ){
      this->error( "Could not bind to address");
    }else{
      this->log( "Listening..." );
      while( true ){
        memset( buffer, '\0', sizeof(buffer) );
        N = recvfrom( sockFd, ( char *) buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *)&client, &length );
        this->onMessage( buffer, BUFFER_SIZE );
        std::this_thread::sleep_for( std::chrono::microseconds( 1000 ) );
      }
    }
  }
  close( sockFd );
  return true;
}

UdpServer::UdpServer( const int port  ){
  this->port = port;
  this->setLabel( "UdpServer" );
}
