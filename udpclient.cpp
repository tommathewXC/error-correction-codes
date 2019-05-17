#include "headers/udpclient.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

bool UdpClient::send( const std::string & input ){
  std::string logline = "Writing to UDP(port=" + std::to_string( this->port ) + " )";
  this->log( logline.c_str() );
  struct sockaddr_in address;
  char buffer[1024] = {0};
  int socketFd = socket( AF_INET, SOCK_DGRAM, 0 );
  if( socketFd < 0 ){
    this->error( "Couldn't open socket" );
    return false;
  }
  memset( &address, '\0', sizeof( address ) );
  address.sin_family = AF_INET;
  address.sin_port = htons( this->port );
  address.sin_addr.s_addr = INADDR_ANY;
  std::cout << "seeefefwefwef" << input << std::endl;
  char * buff = (char *) malloc( input.size() );
  for( int j = 0; j < input.size(); j++ ){ 
    buff[j] = (char) input.at( j ); 
    std::cout << "A " << buff[j]  << std::endl;
    }
  const short _ss = sendto( socketFd, (const char *) buff, input.size(), MSG_CONFIRM, (const struct sockaddr *)&address, sizeof(address) );
  if( _ss < 0 ){
    logline = "Could not send udp message ";
    this->error( logline.c_str() );
  }else{
    logline = "Sent udp message. Length: " + std::to_string( _ss );
    this->log( logline.c_str() );
  }
  free( buff );
  close( socketFd );
  return true;
}
