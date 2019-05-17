all: clean build

pre:
	sudo apt-get install doxygen
	sudo apt-get install graphviz
	sudo apt-get install doxygen-gui

clean:
	@rm -rf exe/*

build:
	@echo "Building error_coder..."
	@g++  main.cpp encoder.cpp udpclient.cpp udpserver.cpp utils.cpp -std=c++11 -o exe/error_coder

install:
	@cp exe/error_coder /usr/local/bin/error_coder

uninstall:
	@pkill -9 -f error_coder
	@rm -rf /usr/local/bin/error_coder

doc:
	@echo "Making documentation"
	@rm -rf docs/html
	@rm -rf docs/latex
	@doxygen doxy.config
	@mv html/ docs/
	@mv latex/ docs/