#ifndef UTILS_H
#define UTILS_H

  #include <iostream>
  #include <unordered_map>
  #include <vector>

  /**
   * @brief General utilties
   * 
   */
  class Bilbo {

    private:

      /**
       * @brief Log an error message
       * 
       * @param msg The error message to log
       */
      void error( const char * msg ){ std::cout << "Bilbo\tERROR:\t" << msg<< std::endl; }
      
      /**
       * @brief   Log an ordinary message
       * 
       * @param msg The message to log
       */
      void log(  const char * msg ){ std::cout << "Bilbo\tINFO:\t" << msg << std::endl; }


    public:
    
      /**
       * Reads the data from the text file specified in the path and puts it inside
       * the string
       * @param  path      Path to the text file
       * @param  inputData The std:string into which the file content will be written
       * @return           The truth whether the read worked correctly.
       */
      bool readTextFile( const char * path, std::string &inputData );

      /**
       * @brief String to integer conversion
       * @param str A const reference to the begeining of the string
       * @return int The string converted to integer
       */
      int fast_atoi( const char * str );

      /**
        * @brief Get the Key Value Pairs
        * @param input       A const string to split
        * @param splitToken  The line separator
        * @param equality    The separator between keys and values
        * @return std::unordered_map< std::string, std::string> 
        * 
      */
      std::unordered_map< std::string, std::string> getKeyValuePairs( const std::string & input, const char splitToken, const char equality );
      
      
      /**
       * @brief Split a string by a delimiter
       * @param input  A const string to split
       * @param delimiter  The const character to split the string by
       * @return std::vector<std::string>
       */
      std::vector<std::string> split( const std::string &input, const char delimiter );

      /**
       * @brief           Extract key value pairs from the command line and pass it into a string map
       * 
       * @param argc 
       * @param arguments 
       * @param kv 
       */
      void cli( const int argc, const char ** arguments, std::unordered_map< std::string, std::string> &kv );
  };

#endif
