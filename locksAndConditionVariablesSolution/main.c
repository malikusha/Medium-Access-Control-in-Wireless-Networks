#include <stdio.h>
#include <stdlib.h>
#include "proj3hdr.h"

void initialize_values(){
  //please set your values here
  num_nodes = 100;
  num_noisemakers = 50;
  dwell_duration = 100;
  dwell_probability = 0;
  transmission_time = 100;
  talk_window_time = 100;
  talk_probability = 25;
  dwell_noisemakers = 100;
  dwell_probability_noisemakers = 10;
  block_probability = 75;

  //the messages we could send
  MESSAGES[0] = 12;
  MESSAGES[1] = 34;
  MESSAGES[2] = 45;
  MESSAGES[3] = 52;
  MESSAGES[4] = 78;
  MESSAGES[5] = 90;
  MESSAGES[6] = 18;
  MESSAGES[7] = 62;
  MESSAGES[8] = 59;
  MESSAGES[9] = 20;
}

//a cool stub to see how a single network runs in our system
void test_add_nodes(){
  add_node(ONE, 10, 10, LEGITIMATE);
  add_node(ONE, 12, 10, LEGITIMATE);
  add_node(ONE, 8, 10, LEGITIMATE);
  add_node(ONE, 4, 10, LEGITIMATE);
  add_node(THREE, 13, 13, LEGITIMATE);
  add_node(THREE, 15, 11, LEGITIMATE);
  add_node(ELEVEN, 15, 8, LEGITIMATE);
  add_node(ELEVEN, 11, 5, LEGITIMATE);
  add_node(ELEVEN, 10, 11, NOISEMAKER);

  initialize_threads();
}

//to be used when n nodes and noisemakers are desired randmly
void test_add_rand_nodes(){
  add_rand_nodes();
  initialize_threads();
}

int main(){
  //get our nodes set up
  initialize_values();
  printf("Welcome to Project 3B! We are running the main file...\n");
  //add some random nodes as we desire
  test_add_rand_nodes();
  //let the threads do their thing.
  while(1){};
}
