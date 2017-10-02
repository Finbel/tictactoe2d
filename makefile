CXX = g++
CXXFLAGS = -std=c++11 -Wall

make:
	$(CXX) *.cpp $(CXXFLAGS) -o TTT

run: TTT
	mkfifo pipe
	./TTT init verbose < pipe | ./TTT > pipe
	rm pipe