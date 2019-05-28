#include <iostream>
#include <fstream>
#include <typeinfo>
#include <cmath>
#include "headers/encoder.h"
#include "headers/hamming.h"
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
  HammingCoder h2 = HammingCoder( "hammingEncoder");
  switch (encType)
  {
  case Hamming347:
    em = h2.encode_hamming347( input );
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
  HammingCoder h = HammingCoder( "hammingDecoder");
  switch (rec.code)
  {
    case Hamming347:
      l += "Hamming (7,4) ";
      h.decode_hamming347( rec.message, message );
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