# Error Codes in C++ 11

This project implements error codes in C++ 11 as an exercise. 


## Error Detection and Correction

Error codes are algorithms that ensure the integrity of binary data. When data travels through
arbitrary media through space (like in communication systems), or through time (like in storage/database systems), there is always the possibility that some of the information bits could get corrupted and flipped. With the use of error correction algorithms, it is possible to add 
some extra bits of information to the raw data so that if some corruptions were to occur, they can be detected. Using these extra error correction bits, it can also be possible to correct any errors that may have occured, and retrieve the uncorrupted raw data.


### Implementation:

In this implementation, the communication medium is a UDP channel. The product of this project is one executable - error_coder. This executable can be used either as a client
or blocking server. The udp client takes string inputs from the command line, encodes them according to a user-specified error_coding algorithm, and transmits them to the UDP server.

The UDP server, if alive, will recieve the coded message, detect the chosen error coding scheme, and then decode the original message. This decoded message is written to the console.

The following are the implemented error detection/correction codes

* [Hamming (7, 3)](https://en.wikipedia.org/wiki/Hamming_code#[7,4]_Hamming_code)
 
  Currently only detecting but not correcting errors.


## Getting Started

The core error coding services come from the executable "error_coder". There is a python script "kickoff" that delegates some CLI calls. You can start and stop the server with kickoff. But sending messages to the UDP server has to be done through the executble.

### Installing

 make pre - Install all the needed dependancies
 
 make clean all - Build the project
 
 sudo make install - install error_coder to /usr/local/bin

### Starting the UDP listener server on 8888 (background + logfile)

./kickoff --start --p 8888

Note that even though error_coder 's UDP server is blocking, calling it through kickoff runs it in the background and 
traps the logs to a configurable path (Defaults to /tmp/error_correction_server.log)

### Starting the UDP listener server on 8888 (foreground + console logs)

error_coder --p 8888 --m 2

### Ending the UDP listener server (non blocking)

./kickoff --stop

### Send an error encoded message to the UDP server - Clear (blocking)

This is an example of sending the phrase "Hola" to the UDP server 
where each byte is sent without encoding.

error_coder --p 8888 --m 1 --c 1 --i Hola

### Send an error encoded message to the UDP server - Hamming (blocking)

This is an example of sending the phrase "Hello" to the UDP server 
where each byte is encoded with Hamming (7, 4) coding

error_coder --p 8888 --m 1 --c 2 --i Hello

### Clear the log files (blocking)

./kickoff --clear

### Tail the log files

tail -f /tmp/error_correction_server.log
