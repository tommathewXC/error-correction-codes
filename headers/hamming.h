#ifndef HAMMING_H
#define HAMMING_H

#include "encoder.h"
#include <iostream>
#include "log.h"


/**
 * @brief   The class that conducts error coding of string messages
 * 
 */
class HammingCoder:public iLogggable {

private:

  const std::string label = "HammingCoder";

public:

  HammingCoder( const char * name ) { this->setLabel( "HammingCoder" ); }

  /**
   * Input : 0  0  0  0  d1 d2 d3 d4
   * Output: 0 p1 p2 p3  d1 d2 d3 d4
   * 
   * Where: 
   * 
   *      p1 = d1 + d2 + d4
   *      p2 = d1 + d3 + d4
   *      p3 = d2 + d3 + d4
   * 
   * and  di, pi ∈ [0, 1]
   * 
   * @param nibble  Lower 4 bites of this byte of data is the message to encode
   * @return char   A byte of data 7 bits of encoded data + 1 padding bit, 
   *                by performing Haming (7, 4) encoding.
   */
  unsigned char fourbitHamming( const unsigned char & nibble );

  /**
   * @brief                 Checks the number of errors in a hamming coded byte. 
   *                        Hamming coded bytes contain 7 bits of information + 1 bit of padding
   * 
   * @param encoded_byte    The hamming coded byte of the form  0 p1 p2 p3 d1 d2 d3 d4,
   *                        where pi are the parity bits, and di are the data bits
   * @return unsigned char  The number of errors detected.
   */
  unsigned char checkfourbitHamming( const unsigned char & encoded_byte );

  /**
   * @brief                   Combine two bytes of hamming (7,4) coded data to create the original message byte
   * 
   * @param lower             The hamming coded byte of the lower nibble
   * @param upper             The hamming coded byte of the upper nibble
   * @return unsigned char    The reconstructed byte from the data bits of the lower and upper nibbles
   */
  unsigned char reversfourBitHamming( const unsigned char & lower, const unsigned char & upper );

  /**
   * @brief             Flip the bits in the lower nibble of the input byte reference
   * 
   * @param buffer      The byte to corrupt the bits of.
   */
  void corruptNibble( unsigned char & buffer );


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
   *                coding. Each 4-bit nibble gets exanded to a byte, so the message size
   *                doubles, when you include the parity bit. In the future, if the message 
   *                size is some multiple of eight, we might be safely discard some bits.
   * 
   * @param input             A constant string that doesn't change
   * @return EncodedMessage   The encoded message and some meta data
   */
  EncodedMessage encode_hamming347( const std::string &input );

};


#endif
