#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "header.h"

void initialize_nodes(){
  initialize_field();
  //ok add the nodes in
}

/*
void* test_thread(void* arg){
  printf("Hey, that's pretty test!\n");
  return NULL;
}
*/

// initialize_threads
void print_nodes_located(){
  for (int i = 0; i < 100; i++){
    for (int j = 0; j < 100; j++){
      if (field[i][j] != NULL){
        Node* me = field[i][j];
        printf("The node is at (%d, %d)!\n", me->x_pos, me->y_pos);
        pthread_create(&me->thread, NULL, threads, me);
      }
    }
  }
}

void find_neighbors(Node* node_ptr, int x_pos, int y_pos){

  int minX = 5;
  int maxX = 5;
  int minY = 5;
  int maxY = 5;

  if (x_pos < 5){
    minX = x_pos;
  }
  else if (x_pos > 94) {
    maxX = 100 - x_pos;
  }
  if (y_pos < 5) {
    minY = y_pos;
  }
  else if (y_pos > 94) {
    maxY = 100 - y_pos;
  }

  // GO through the neighobrhood og the given node and find neighbors
  for (int i = (x_pos-minX); i <= (x_pos+maxX); i++){
    for (int j = (y_pos-minY); j <= (y_pos+maxY); j++){
      // If we found that the field is holding a node at coordinates i and j
      if (field[i][j] != NULL){
        printf("The node at coordinates (%d, %d) has a neighbor at coordinates (%d, %d).\n", x_pos, y_pos, i, j);

        // Add a new neighbor to the array of neighbors of the given node
        node_ptr->nbrs[node_ptr->nbr_size] = field[i][j];
        // Update neighbor's array of neighbors, so it includes the given node too
        field[i][j]->nbrs[field[i][j]->nbr_size] = node_ptr;

        // Incerement the counters
        node_ptr->nbr_size++;
        field[i][j]->nbr_size++;
      }
    }
  }
}

// add_node
Node *initialize_node(int channel, int x_pos, int y_pos, int type){
  Node* node_ptr = (Node*) malloc(sizeof(Node));
  node_ptr->channel = channel;
  node_ptr->x_pos = x_pos;
  node_ptr->y_pos = y_pos;
  node_ptr->type = type;
  node_ptr->shield = 0;
  node_ptr->action = NEITHER;
  node_ptr->interference = CLEAR;
  node_ptr->nbr_size = 0;
  node_ptr->id = CURRENT_NUM;
  CURRENT_NUM++;

  node_ptr->MSGS_COUNT = 0;
  node_ptr->message_list = (MessageList*)malloc(sizeof(MessageList));
  initialize_message_list(node_ptr->message_list);

  node_ptr->file = NULL;

  sem_init(&node_ptr->shield_lock, 0, 1);
  sem_init(&node_ptr->action_lock, 0, 1);
  sem_init(&node_ptr->shield_unlock, 0, 1);
  sem_init(&node_ptr->action_unlock, 0, 1);
  sem_init(&node_ptr->noise_lock, 0, 1);
  sem_init(&node_ptr->message_lock, 0, 1);


  // Initialize the array of neighbors of the node
  find_neighbors(node_ptr, x_pos, y_pos);
  // The node is placed on the grid at x_pos, y_pos coordinates
  field[x_pos][y_pos] = node_ptr;
  return node_ptr;
}

// The method to initialize the grid
void initialize_field(){
  for (int i = 0; i < 100; i++){
    for (int j = 0; j < 100; j++){
      field[i][j] = NULL;
    }
  }
}
