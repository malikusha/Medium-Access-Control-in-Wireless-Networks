all: wireless_sim

wireless_sim: main.o initialize_nodes.o node_functions.o messages.o threads.o
	gcc -pthread  main.o initialize_nodes.o node_functions.o messages.o threads.o -o wireless_sim

main.o: main.c header.h
	gcc -g -c main.c

threads.o: threads.c header.h
	gcc -g -c threads.c

node_functions.o: node_functions.c header.h
	gcc -g -c node_functions.c

initialize_nodes.o: initialize_nodes.c header.h
	gcc -g -c initialize_nodes.c

messages.o: messages.c header.h
	gcc -g -c messages.c

clean:
	rm -f *.o wireless_sim
