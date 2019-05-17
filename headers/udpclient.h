#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include "log.h"

class UdpClient: public iLogggable {

private:
  short port = 9000;
  const std::string label = "UdpClient";

public:

  /**
   * @brief Construct a new Udp Client object
   * 
   * @param PORT The port to send the message to
   */
  UdpClient( const int & PORT ){ port = PORT; };

  /**
   * @brief         Send a message over a UDP socket
   * 
   * @param input   std::string
   * @return true   If the UDP transmission worked
   * @return false  If the UDP transmission failed
   */
  bool send( const std::string & input ) ;
};

#endif
