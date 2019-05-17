#include <iostream>
#include <fstream>
#include <typeinfo>
#include <cmath>
#include "headers/encoder.h"

void Encoder::debugByte( const unsigned char & d, const char * label ){
  std::cout << "debugByte: " << label << "\t\t";
  char data[8] = {0};
  for( char i=0; i < 8; i++ ){ data[8 - i -1] = ( d >> i) & 1 ; }
  for( char i=0; i < 8; i++ ){
    std::cout << (int)data[i] << " ";
  }
  std::cout << std::endl;
}

std::string Encoder::compose( const EncodedMessage & message ){
  std::string output = "";
  output += (char) message.code;
  for( int i = 0; i < message.message.size(); i++ ){
    output += message.message.at( i );
  }
  return output;
}

void Encoder::decode_hamming347( const std::string &input, std::string &output ){
  output.clear();
  output.resize(0);
  const std::string logline = "Decoding message with Hamming 3-4-7";
  this->log( logline.c_str() );
  this->log( input.c_str() );
}

unsigned char Encoder::fourbitHamming( const unsigned char & nibble ){
  unsigned char buffer = 0;
  buffer = buffer | (((nibble >> 3) | (nibble >> 2) | nibble ) & 1);
  buffer = buffer << 1;
  buffer = buffer | (((nibble >> 3) | (nibble >> 1) | nibble ) & 1);
  buffer = buffer << 1;
  buffer = buffer | (((nibble >> 2) | (nibble >> 1) | nibble ) & 1);
  buffer = (buffer << 4);
  buffer = buffer | ( nibble & 15 );
  return buffer;
}

EncodedMessage Encoder::encode_hamming347( const std::string &input ){
  this->log( "Encoding message with Hamming 3-4-7. " );
  char lowerByte = '\0';
  char upperByte = '\0';
  EncodedMessage em = EncodedMessage( input.c_str() );
  em.message = "";
  for( int i=0; i < input.size(); i++ ){
    em.message += fourbitHamming( input.at(i) & 15 );           // lower 4 bits
    em.message += fourbitHamming( (input.at(i) >> 4) & 15 );    // upper 4 bits
  }
  return em;
}

EncodedMessage Encoder::encode( const std::string &input, const EncodingType & encType ){
  EncodedMessage em = EncodedMessage( "" );
  em.code = ClearText;
  if( encType == Hamming347 ){
    em = this->encode_hamming347( input );
  }
  return em;
}

std::string Encoder::encodeToString( const std::string &input, const EncodingType & encType ){
  EncodedMessage em = EncodedMessage( "" );
  em.code = ClearText;
  if( encType == Hamming347 ){
    em = this->encode_hamming347( input );
  }
  return compose( em );
}

EncodedMessage Encoder::decompose( const std::string &message ){
  EncodedMessage m = EncodedMessage( "" );
  std::cout << "decomposing " << message << std::endl;
  for( int i=0; i < message.size(); i++ ){
    if( i == 0){
      m.code = (EncodingType) message.at( i );
    }else{
      m.message += (char) message.at(i);
    }
  }
  return m;
}