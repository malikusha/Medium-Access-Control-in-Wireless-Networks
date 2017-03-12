all: wireless_sim

wireless_sim: main.o rand_fns.o node_fns.o thread_fns.o message_fns.o
	gcc -pthread main.o rand_fns.o node_fns.o thread_fns.o message_fns.o -o wireless_sim

main.o: main.c proj3hdr.h
	gcc -g -c main.c

rand_fns.o: rand_fns.c proj3hdr.h
	gcc -g -c rand_fns.c

node_fns.o: node_fns.c proj3hdr.h
	gcc -g -c node_fns.c

thread_fns.o: thread_fns.c proj3hdr.h
	gcc -g -c thread_fns.c

message_fns.o: message_fns.c proj3hdr.h
	gcc -g -c message_fns.c

clean:
	rm -f *.o *.txt wireless_sim
