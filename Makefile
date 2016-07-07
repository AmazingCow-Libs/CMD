all:
	rm -rf ./bin/
	mkdir -p ./bin

	g++ -std=c++11 -Wfatal-errors  -o ./bin/test \
	    -I./include  \
	    src/*.cpp    \
	    test/*.cpp
