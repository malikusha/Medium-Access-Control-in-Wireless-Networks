#include <stdio.h>
#include <stdlib.h>
#include "header.h"

void initialize_values(){
  num_nodes = 200;
  num_noisemakers = 50;
  dwell_duration = 100;
  dwell_probability = 25;
  transmission_time = 100;
  talk_window_time = 100;
  talk_probability = 75;
  block_probability = 75;
  CURRENT_NUM = 0;

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

void test_add_nodes(){
  initialize_node(ONE, 10, 10, LEGITIMATE);
  initialize_node(ONE, 12, 10, LEGITIMATE);
  initialize_node(ONE, 8, 10, LEGITIMATE);
  initialize_node(ONE, 4, 10, LEGITIMATE);
  initialize_node(THREE, 13, 13, LEGITIMATE);
  initialize_node(THREE, 15, 11, LEGITIMATE);
  initialize_node(ELEVEN, 15, 8, LEGITIMATE);
  initialize_node(ELEVEN, 11, 5, LEGITIMATE);
  initialize_node(ELEVEN, 10, 11, NOISEMAKER);

  /*
  printf("The node at (10, 10) has %d neighbors.\n", field[10][10]->nbr_size);
  printf("The first neighbor is at (%d, %d).\n", (field[10][10]->nbrs)[0]->x_pos, (field[10][10]->nbrs)[0]->y_pos);
  printf("The second neighbor is at (%d, %d).\n", (field[10][10]->nbrs)[1]->x_pos, (field[10][10]->nbrs)[1]->y_pos);
  */
  print_nodes_located();
}

void test_add_rand_nodes(){
  add_rand_nodes();
}

int main(){

  initialize_values();
  printf("Welcome to Project 3B! We are running the main file...\n");
  //test_add_nodes();

  Node* n1 = initialize_node(ONE, 10, 10, LEGITIMATE);
  Node* n2 = initialize_node(ONE, 12, 10, LEGITIMATE);
  Node* n3 = initialize_node(ONE, 8, 10, LEGITIMATE);
  Node* n4 = initialize_node(ONE, 4, 10, LEGITIMATE);
  Node* n5 = initialize_node(THREE, 13, 13, LEGITIMATE);
  Node* n6 = initialize_node(THREE, 15, 11, LEGITIMATE);
  Node* n7 = initialize_node(ELEVEN, 15, 8, LEGITIMATE);
  Node* n8 = initialize_node(ELEVEN, 11, 5, LEGITIMATE);
  Node* n9 = initialize_node(ELEVEN, 10, 11, NOISEMAKER);
  print_nodes_located();

  threads(n1);
  threads(n2);
  threads(n3);
  threads(n4);
  threads(n5);
  threads(n6);
  threads(n7);
  threads(n8);
  threads(n9);



  /*
  int retTime = time(0) + 10;   // Get finishing time.
  while (time(0) < retTime){
  }
  */
  //test_add_rand_nodes();
}
