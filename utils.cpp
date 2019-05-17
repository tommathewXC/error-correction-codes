#include "headers/utils.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_map>

std::vector<std::string> Bilbo::split( const std::string &input, const char delimiter ){
  std::stringstream check1( input );
  std::vector< std::string> tokens;
  std::string intermediate;
  while(getline(check1, intermediate, delimiter )){ tokens.push_back(intermediate); }
  return tokens;
}

bool Bilbo::readTextFile( const char * path, std::string &inputData ){
  std::ifstream myFile( path );
  if( myFile.is_open() ){
    try{
      myFile.seekg( 0, std::ios::end );
      const size_t size = myFile.tellg();
      myFile.seekg( 0, std::ios::beg );
      inputData.resize( size );
      myFile.read( &inputData[0], size );
      myFile.close();
      return true;
    }catch(...){
      myFile.close();
      return false;
    }
  }
  return false;;
}

int Bilbo::fast_atoi( const char * str ){
  int val = 0;
  while(*str){
    val = val*10 + (*str++ - '0');
  }
  return val;
}

std::unordered_map< std::string, std::string> Bilbo::getKeyValuePairs( const std::string &input, const char splitToken, const char equality ){
  std::unordered_map<std::string, std::string> kv;
  std::string val;
  std::vector<std::string> intermediate;
  auto v = split( input, splitToken );
  for( auto i = v.begin(); i != v.end(); ++i ){
    val = (*i).c_str();
    intermediate.clear();
    intermediate.resize(0);
    intermediate = split( val, equality );
    if( intermediate.size() == 2 ){
      kv[ intermediate.at(0) ] = intermediate.at( 1 );
    }
  }
  return kv;
}
