#include <iostream>
#include <fstream>
#include <typeinfo>
#include <cmath>
#include "headers/encoder.h"
#include "headers/utils.h"

void Encoder::debugByte( const unsigned char & d, const char * label ){
  std::cout << "debug a Byte: " << label << "\t\t";
  char data[8] = {0};
  for( char i=0; i < 8; i++ ){ data[8 - i -1] = ( d >> i) & 1 ; }
  for( char i=0; i < 8; i++ ){ std::cout << (int)data[i] << " "; }
  std::cout << std::endl;
}

std::string Encoder::compose( const EncodedMessage & message ){
  std::string output = "";
  std::string mSize = std::to_string( message.message.size() ) ;
  output += (char) message.code;
  output += '|';
  for( int k=0; k < mSize.size(); k++ ){
    output += mSize.at( k );
  }
  output += '|';
  for( int i = 0; i < message.message.size(); i++ ){ output += (char) message.message.at( i ); }
  output += '|';
  output += '|';
  return output;
}

EncodedMessage Encoder::decomposeFromBuffer( const char * buffer, const int & bufferSize ){
  EncodedMessage m = EncodedMessage( "" );
  m.message = "";
  Bilbo b = Bilbo();
  std::string messageSize = "";
  int mSize = 0;
  int markoff = 0;
  bool foundSize = false;
  bool foundMessage = false;
  for( int i =0; i < bufferSize; i++ ){
    if( i == 0 ){
      m.code = (EncodingType) buffer[i];
    }else{
      if( i == 1 ){
        if(  buffer[i] != '|' ){
          i = bufferSize;
          m.code = Invalid;
          m.message = "";
        }
      }else{
        if( !foundSize){
          if (buffer[i] != '|' ){
            messageSize += buffer[i];
          }else{
            this->log( (std::string("Found message of size: " )+ messageSize).c_str() );
            mSize = b.fast_atoi( messageSize.c_str() );
            foundSize = true;
          }
        }else{
          if( buffer[i] == '|' && buffer[ i + 1 ] == '|' ){
            i = bufferSize;
            foundMessage = true;
            this->log( (std::string("Found message: " )+ m.message).c_str() );
          }else{
            m.message += buffer[i];
          }
        }
      }
    }
  }
  if(!foundMessage){
    m.code = Invalid;
    m.message = "";
    this->error( "Could not decode a valid message" );
  }
  return m;
}

void Encoder::decode_hamming347( const std::string &input, std::string &output ){
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

EncodedMessage Encoder::encode_hamming347( const std::string &input ){
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

unsigned char Encoder::checkfourbitHamming( const unsigned char & encoded_byte ){
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

unsigned char Encoder::reversfourBitHamming( const unsigned char & lower, const unsigned char & upper ){
  unsigned char a = (unsigned char) 0;
  a |= ( upper & 15 );
  a = a << 4;
  a |= ( lower & 15 );
  return a;
}

unsigned char Encoder::fourbitHamming( const unsigned char & nibble ){
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

void Encoder::corruptNibble( unsigned char & buffer ){
  if((buffer & 1) == 1){
    buffer = ( buffer & 254 );
  }else{
    buffer = buffer | 1;
  }
}

EncodedMessage Encoder::encode( const std::string &input, const EncodingType & encType ){
  EncodedMessage em = EncodedMessage( "" );
  em.code = Invalid;
  em.message = "";
  switch (encType)
  {
  case Hamming347:
    em = this->encode_hamming347( input );
    break;
  case ClearText:
    em.code = ClearText;
    em.message = input;
    break;
  default:
    break;
  }
  return em;
}

std::string Encoder::encodeToString( const std::string &input, const EncodingType & encType ){
  EncodedMessage em = encode( input, encType );
  const std::string c = compose( em );
  std::string logm = "Encoder Raw Input: " + input + ", Size: " + std::to_string( input.size() );
  this->log( logm.c_str() );
  logm = "Encoder Output: " + em.message + ", Size: " + std::to_string( em.message.size() );
  this->log( logm.c_str() );
  logm = "TX message: " + c + ", Size: " + std::to_string( c.size() );
  this->log( logm.c_str() );
  return c;
}

void Encoder::decode( const EncodedMessage & rec ){
  std::string message = "";
  std::string l = "Decoding(" + rec.message + ") with ";
  switch (rec.code)
  {
    case Hamming347:
      l += "Hamming (7,4) ";
      this->decode_hamming347( rec.message, message );
      break;
    case ClearText:
      l += "no encoding scheme as its clear text ";
      message = rec.message;
      break;
    default:
      l += "Unknown code";
      break;
  }
  this->log( l.c_str() );
  l = "Decoded output " + message ;
  this->log( l.c_str() );
}