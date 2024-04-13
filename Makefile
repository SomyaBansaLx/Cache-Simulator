all: run

run:
	g++ do_me.cpp -o cacheSim
	./cacheSim 256 4 16 write-allocate write-back fifo < trace.txt
clean :
	rm cacheSim

# ./cacheSim 4 1 4 write-allocate write-through lru < trace.txt

