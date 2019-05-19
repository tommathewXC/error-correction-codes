#include <iostream>
#include <unordered_map>
#include "headers/utils.h"

Bilbo g = Bilbo();

int main( const int argc, const char ** arguments ){ 
  int port = -1;
  int mode = 0;
  int coding = 1;
  std::unordered_map< std::string, std::string> kv;
  kv["--p"] = "8000";
  g.cli( argc, arguments, kv );
  port = g.fast_atoi( kv["--p"].c_str() );
  std::cout << "Port: " << port << std::endl; 
  return 0;
}
