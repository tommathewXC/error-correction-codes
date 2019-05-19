#ifndef UDPSERVER_H
#define UDPSERVER_H

#include "log.h"

class UdpServer: public iLogggable {

private:
  short port = 9000;

public:

  /**
   * @brief Construct a new Udp Server object
   * 
   * @param port The port to start the server on
   */
  UdpServer( const int port );


  /**
   * @brief Start the UDP server
   * 
   * @return true If the UDP is live and amd exited gracefully (somehow)
   * @return false If the UDP server could not be started
   */
  bool serve();

  /**
   * @brief   The message handler when a UDP message is recieved on the UDP server
   * 
   * @param message The Message recieved on the UDP socket
   */
  void onMessage( const char * buffer, const int & length );
};

#endif
