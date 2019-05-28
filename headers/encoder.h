#ifndef ENCODER_H
#define ENCODER_H

#include <iostream>
#include "log.h"

#define MESSAGE_SIZE 4096

const int CodesSupported = 3;

/**
 * @brief The enum of the encoding type
 * 
 */
enum EncodingType {
  Invalid,
  ClearText,
  Hamming347
};

/**
 * @brief The struct that contains data that encapsulates the encoding 
 *        type and the encoded message
 */
struct EncodedMessage{
  EncodingType code;
  std::string message;
  EncodedMessage( const char * m ){ message = m; code = ClearText; }
};

/**
 * @brief   The class that conducts error coding of string messages
 * 
 */
class Encoder:public iLogggable {

private:

  const std::string label = "Encoder";

public:

  Encoder( const char * name ) { this->setLabel( "Encoder" ); }

  /**
   * @brief             Flip the bits in the lower nibble of the input byte reference
   * 
   * @param buffer      The byte to corrupt the bits of.
   */
  void corruptNibble( unsigned char & buffer );

  /**
   * @brief Get the distinct number Encodings supported
   * 
   * @return int 
   */
  int getEncodingCount() { return CodesSupported; };

  /**
   * @brief         Print out the bits in a char
   * 
   * @param d       The character byte to analyze
   * @param label   The label to print with the input
   */
  void debugByte( const unsigned char & d, const char * label );

  /**
   * @brief Encode a given message with the chosen error coding scheme
   * 
   * @param input           A const refrence to a C++ 11 string with the message to encode
   * @param encType         An EncodingType enum indicationg what error coding to use
   * @return EncodedMessage The struct that contains the error coded message
   *                        and any and all meta data
   */
  EncodedMessage encode( const std::string & input, const EncodingType & encType );

  /**
   * @brief Call the base encoder, composed.
   * 
   * @param input           A const refrence to a C++ 11 string with the message to encode
   * @param encType         An EncodingType enum indicationg what error coding to use
   * @return EncodedMessage The struct that contains the error coded message
   *                        and any and all meta data
   */
  std::string encodeToString( const std::string & input, const EncodingType & encType );


  /**
   * @brief     Detect the form of error coding the in the recieved messsage and perform
   *            error checking and correction
   * 
   * @param rec message reciegved over the socket.
   */
  void decode( const EncodedMessage & rec );

  /**
   * @brief               convert a message struct into a message string.
   * 
   * @param message       EncodingMessage
   * @return std::string  One byte of encoding type + The message string
   */
  std::string compose( const EncodedMessage & message );
  
  /**
   * @brief              Extract the data struct from a char buffer of a known size
   * 
   * @param buffer       The UDP message buffer 
   * @param bufferSize   The UDP message buffer size
   * @return EncodedMessage The struct with the encoded message 
   */
  EncodedMessage decomposeFromBuffer( const char * buffer, const int & bufferSize  );
};


#endif
