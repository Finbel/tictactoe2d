CXX = g++
CXXFLAGS = -std=c++11 -Wall


TTT: player.cpp
	$(CXX) *.cpp $(CXXFLAGS) -o TTT.out

run:
	mkfifo pipe
	./TTT.out init verbose < pipe | ./TTT.out > pipe
	rm pipe