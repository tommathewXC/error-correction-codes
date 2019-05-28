#include <iostream>
#include <fstream>
#include <typeinfo>
#include <cmath>
#include "headers/encoder.h"
#include "headers/hamming.h"
#include "headers/utils.h"

void HammingCoder::decode_hamming347( const std::string &input, std::string &output ){
  output.clear();
  output.resize(0);
  unsigned char errorCount = 0;
  for( int i =0; i < input.size(); i+=2 ){
    errorCount += checkfourbitHamming( input.at(i) );
    errorCount += checkfourbitHamming( input.at(i+1) );
    output += reversfourBitHamming( input.at(i), input.at(i + 1) );
  }
  std::string l = "\tTotal errors: " + std::to_string( errorCount );
  this->log( l.c_str() );
}

EncodedMessage HammingCoder::encode_hamming347( const std::string &input ){
  this->log( "Encoding message with Hamming 3-4-7. " );
  char lowerByte = '\0';
  char upperByte = '\0';
  EncodedMessage em = EncodedMessage( input.c_str() );
  em.code = Hamming347;
  em.message = "";
  for( int i=0; i < input.size(); i++ ){
    em.message += fourbitHamming( input.at(i) & 15 );           // lower 4 bits
    em.message += fourbitHamming( (input.at(i) >> 4) & 15 );    // upper 4 bits
  }
  return em;
}

unsigned char HammingCoder::checkfourbitHamming( const unsigned char & encoded_byte ){
  unsigned char buffer = 0;
  unsigned char output = 0;
  buffer |= (((encoded_byte >> 3) | (encoded_byte >> 2) | encoded_byte ) & 1);
  buffer = buffer << 1;
  buffer |= (((encoded_byte >> 3) | (encoded_byte >> 1) | encoded_byte ) & 1);
  buffer = buffer << 1;
  buffer |= (((encoded_byte >> 2) | (encoded_byte >> 1) | encoded_byte ) & 1);
  if( ( (encoded_byte >> 4) & 1) + ( buffer & 1 ) == 1 ){ output += 1; }
  if( ( (encoded_byte >> 5) & 1) + ( (buffer >> 1) & 1 ) == 1 ){ output += 1; }
  if( ( (encoded_byte >> 6) & 1) + ( (buffer >> 2) & 1 ) == 1 ){ output += 1; }
  return output;
}

unsigned char HammingCoder::reversfourBitHamming( const unsigned char & lower, const unsigned char & upper ){
  unsigned char a = (unsigned char) 0;
  a |= ( upper & 15 );
  a = a << 4;
  a |= ( lower & 15 );
  return a;
}

unsigned char HammingCoder::fourbitHamming( const unsigned char & nibble ){
  unsigned char buffer = 0;
  buffer |= (((nibble >> 3) | (nibble >> 2) | nibble ) & 1);
  buffer = buffer << 1;
  buffer |= (((nibble >> 3) | (nibble >> 1) | nibble ) & 1);
  buffer = buffer << 1;
  buffer |= (((nibble >> 2) | (nibble >> 1) | nibble ) & 1);
  buffer = (buffer << 4);
  buffer |= ( nibble & 15 );
  // corruptNibble( buffer );
  return buffer;
}

void HammingCoder::corruptNibble( unsigned char & buffer ){
  if((buffer & 1) == 1){
    buffer = ( buffer & 254 );
  }else{
    buffer = buffer | 1;
  }
}