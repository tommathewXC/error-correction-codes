#ifndef ENCODER_H
#define ENCODER_H

#include <iostream>
#include "log.h"

#define MESSAGE_SIZE 4096

/**
 * @brief The enum of the encoding type
 * 
 */
enum EncodingType {
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

  /**
   * Input : 0  0  0  0  d1 d2 d3 d4
   * Output: 0 p1 p2 p3  d1 d2 d3 d4
   * 
   * Where: 
   * 
   * p1 = d1 + d2 + d4
   * p2 = d1 + d3 + d4
   * p3 = d2 + d3 + d4
   * 
   * di, pi ∈ Z
   * 
   * @param nibble  Lower 4 bites of this byte of data is the message to encode
   * @return char   A byte of data 7 bits of encoded data + 1 padding bit, 
   *                by performing Haming (7, 4) encoding.
   */
  unsigned char fourbitHamming( const unsigned char & nibble );

public:

  /**
   * @brief         Print out the bits in a char
   * 
   * @param d       The character byte to analyze
   * @param label   The label to print with the input
   */
  void debugByte( const unsigned char & d, const char * label );

  /**
   * @brief         Decode Hamming 347 codes, Reads a C++ 11 string and assumes that
   *                each character has 7 bits of hamming (7, 4) and 1 padding bit. The
   *                7 bits are assume to be an encoded form of an original message
   *                which contained 4 bits.
   * 
   * @param input   A constant string that doesn't change
   * @param output  Where to write the decoded output to
   */
  void decode_hamming347( const std::string &input, std::string &output );

  /**
   * @brief         Encode Hamming 347 codes, Reads a C++ 11 string, splits each character
   *                into two pairs of 4 bits each, and performs Hamming (7, 4) error 
   *                coding.
   * 
   * @param input             A constant string that doesn't change
   * @return EncodedMessage   The encoded message and some meta data
   */
  EncodedMessage encode_hamming347( const std::string &input );

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
   * @brief               convert a message struct into a message string.
   * 
   * @param message       EncodingMessage
   * @return std::string  One byte of encoding type + The message string
   */
  std::string compose( const EncodedMessage & message );
  

  /**
   * @brief                 Construct a EncodedMessage object from the UDP message
   * 
   * @param message         The message recieved over UDP.
   * @return EncodedMessage The complete encapsulation of the recieved message
   */
  EncodedMessage decompose( const std::string &message );
};


#endif
