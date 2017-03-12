#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "proj3hdr.h"

void initialize_nodes(){
  initialize_field();
}

//this function starts a thread for each node
void initialize_threads(){
  for (int i = 0; i < 100; i++){
    for (int j = 0; j < 100; j++){
      if (field[i][j] != NULL){
        Node* me = field[i][j]; //set up a node in the position of the array
        printf("Hey now, we found a node at (%d, %d)!\n", me->x_pos, me->y_pos);
        //pthread_t p;
        pthread_create(&me->thread, NULL, node_thread, me); //start up its thread
        //pthread_create(&p, NULL, test_thread, NULL);
      }
    }
  }
}

//whenever a new node is added, this function gives it pointers to all its nodes
void find_neighbors(Node* node_ptr, int x_pos, int y_pos){

  //adjust our search bounds if we are near the array edge
  int minX = 5;
  int maxX = 5;
  int minY = 5;
  int maxY = 5;
  if (x_pos < 5){ minX = x_pos; }
  else if (x_pos > 94) { maxX = 99 - x_pos; }
  if (y_pos < 5){ minY = y_pos; }
  else if (y_pos > 94) { maxY = 99 - y_pos; }

  for (int i = (x_pos-minX); i <= (x_pos+maxX); i++){
    for (int j = (y_pos-minY); j <= (y_pos+maxY); j++){
      //find and link the two neighbors
      if (field[i][j] != NULL){
        printf("We found that the node at (%d, %d) has a neighbor at (%d, %d).\n", x_pos, y_pos, i, j);
        node_ptr->nbrs[node_ptr->nbr_size] = field[i][j];
        field[i][j]->nbrs[field[i][j]->nbr_size] = node_ptr;
        node_ptr->nbr_size++;
        field[i][j]->nbr_size++;
      }
    }
  }
}

//this initializes all the data for a node
void add_node(int frequency, int x_pos, int y_pos, int type){
  Node* node_ptr = (Node*) malloc(sizeof(Node)); //reserve it memory
  node_ptr->channel = frequency;
  node_ptr->x_pos = x_pos;
  node_ptr->y_pos = y_pos;
  node_ptr->type = type;
  node_ptr->shield = 0; //sheilds always start a 0
  node_ptr->action = NEITHER;
  node_ptr->interference = CLEAR; //start with nobody bothering us
  node_ptr->nbr_size = 0;
  node_ptr->id = CURRENT_NUM; //each node has a unique "id"
  CURRENT_NUM++;

  node_ptr->MSGS_COUNT = 0;
  node_ptr->message_list = (MessageList*)malloc(sizeof(MessageList)); //we malloc our list of messages
  initialize_message_list(node_ptr->message_list); //we set up the head and tail pointers within
  node_ptr->file = NULL;

  //reassign the attributes of our locks
  pthread_mutexattr_init(&node_ptr->shield_attr);
  pthread_mutexattr_settype(&node_ptr->shield_attr, PTHREAD_MUTEX_RECURSIVE);
  pthread_mutex_init(&node_ptr->shield_lock, &node_ptr->shield_attr);
  pthread_mutexattr_init(&node_ptr->action_attr);
  pthread_mutexattr_settype(&node_ptr->action_attr, PTHREAD_MUTEX_RECURSIVE);
  pthread_mutex_init(&node_ptr->action_lock, &node_ptr->action_attr);
  pthread_mutexattr_init(&node_ptr->noise_attr);
  pthread_mutexattr_settype(&node_ptr->noise_attr, PTHREAD_MUTEX_RECURSIVE);
  pthread_mutex_init(&node_ptr->noise_lock, &node_ptr->noise_attr);
  pthread_mutexattr_init(&node_ptr->msgcount_attr);
  pthread_mutexattr_settype(&node_ptr->msgcount_attr, PTHREAD_MUTEX_RECURSIVE);
  pthread_mutex_init(&node_ptr->msgcount_lock, &node_ptr->msgcount_attr);

  //initialize the condition bariable
  pthread_cond_init(&node_ptr->cond, NULL);

  //look for our neighbors and put the node in the array
  find_neighbors(node_ptr, x_pos, y_pos);
  field[x_pos][y_pos] = node_ptr;
}

void initialize_field(){
  //the field starts with nulls
  for (int i = 0; i < 100; i++){
    for (int j = 0; j < 100; j++){
      field[i][j] = NULL;
    }
  }
}
