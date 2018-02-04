S = src
H = hdr
O = obj
B = bin
T = tst

$B/maze-test: $O/maze-test.o $O/maze.o
	g++ $O/maze-test.o $O/maze.o -o $B/maze-test
$O/maze-test.o: $T/maze-test.cpp
	g++ -c -I$H/ $T/maze-test.cpp -o $O/maze-test.o
$O/maze.o: $S/maze.cpp
	g++ -c -I$H/ $S/maze.cpp -o $O/maze.o
