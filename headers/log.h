#ifndef LOGGABLE_H
#define LOGGABLE_H

#include <iostream>

class iLogggable{

private:

  std::string label = "iLogggable";

public:

  /**
   * @brief Set the Label name
   * @param name The name which will be seen in the logger
   */
  void setLabel( const char * name ){ label = std::string( name ); };

  /**
   * @brief Log an error message
   * 
   * @param msg The error message to log
   */
  void error( const char * msg ){ std::cout << label.c_str() << "\tERROR:\t" << msg<< std::endl; }
  
  /**
   * @brief   Log an ordinary message
   * 
   * @param msg The message to log
   */
  void log(  const char * msg ){ std::cout << label.c_str() << "\tINFO:\t" << msg << std::endl; }

};

#endif
