#include <iostream>
#include <fstream>
#include <unordered_map>
#include "headers/udpclient.h"
#include "headers/udpserver.h"
#include "headers/encoder.h"
#include "headers/utils.h"
#include <stdio.h>
#include <cstring>

Bilbo g = Bilbo();
Encoder enc = Encoder();

void UdpServer::onMessage( const std::string & message ){

}

int main( const int argc, const char ** arguments ){ 
  int port = -1;
  int mode = 0;
  std::string inputMessage = "default-message";
  for( int i=0; i < argc; i++ ){
    if( i < argc - 1 ){
      const std::string key = arguments[i];
      const std::string val = arguments[i+1];
      if( key == "--p" ){
        port = g.fast_atoi( val.c_str() );
      }
      if( key == "--m" ){
        mode = g.fast_atoi( val.c_str() );
      }
      if( key == "--i" ){
        inputMessage = "";
        inputMessage = std::string( val.c_str() );
      }
    }
  }
  if( port == -1 ){
    std::cout << "No port selected. ex. --p 8888" << std::endl;
    return 1;
  }
  if( port < 5000 || port > 10000 ){
    std::cout << "5000 < port < 10000" << std::endl;
    return 1;
  }
  else if( mode == 0 ){
    std::cout << "No mode selected. ex --m 1 Where 1 = UDP writer, 2 = UDP reader" << std::endl;
    return 1;
  }
  if( mode == 1  ){
    UdpClient uClient = UdpClient( port );
    std::cout << "inputMessage " << inputMessage << std::endl;
    std::string outp = enc.encodeToString( inputMessage, Hamming347 );
    std::cout << "Sending " << outp << std::endl;
    if( uClient.send( outp ) ){
      std::cout << "UDP transmission worked" << std::endl;
    }else{
      std::cout << "UDP transmission failed" << std::endl;
    }
  }else if( mode == 2 ){
    UdpServer uServer = UdpServer( port );
    uServer.serve();
  }
  return 0;
}