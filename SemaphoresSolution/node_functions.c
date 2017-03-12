#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
#include "header.h"

void add_rand_nodes(){
  // Set the random seed
  srand(time(NULL));

  int remaining_nodes, remaining_noise, total_nodes, type, x_pos, y_pos, type_pick, channel;

  // Store the values of the global variable as counters to keep track how many nodes we left to initialize yet
  remaining_nodes = num_nodes;
  remaining_noise = num_noisemakers;
  total_nodes = remaining_nodes + remaining_noise;

  // Loop until there are still nodes that we need to initialize
  while (total_nodes > 0){

    // Pick the random position of the node on the grid
    x_pos = rand() % 100;
    y_pos = rand() % 100;

    // If the grid at randomly picked position is free
    if (field[x_pos][y_pos] == NULL){
      // Choose if the node is going to be noisemaker or the node
      // First, pick randomly from number of total nodes that should be initialized
      type_pick = rand() % total_nodes + 1;

      // Set the type based on the need of more nodes
      if(remaining_nodes-type_pick >= 0){
        type = LEGITIMATE;
        remaining_nodes--;
      }
      else{
        type = NOISEMAKER;
        remaining_noise--;
      }

      total_nodes--;

      // Generate the random channel
      channel = rand() % 3;

      // Finally, initialize the node with the random data, we got
      initialize_node(channel, x_pos, y_pos, type);
      printf("A new node of type %d was added at the position (%d, %d).\n", type, x_pos, y_pos);
    }
  }
}

void check_channel_change(struct timeval* start, struct timeval* end, Node* me){

  // The node starts to change the channel
  sem_wait(&me->shield_lock);
  me->shield = 1;
  sem_post(&me->shield_lock);

  printf("The node at (%d, %d) is changing its channel!\n", me->x_pos, me->y_pos);

  if (me->action == NEITHER) {
    gettimeofday(start, NULL);
    int elapsed_time = ((end->tv_sec * 1000 + end->tv_usec / 1000) - (start->tv_sec * 1000 + start->tv_usec / 1000));
    if (elapsed_time >= dwell_duration){
      int channel_stay = rand() % 101;
      if (channel_stay <= dwell_probability){
        gettimeofday(end, NULL);
      }
      else {
        gettimeofday(end, NULL);
        int new_channel = rand() % 2;
        if (new_channel && me->channel == ONE){ me->channel = THREE; }
        else if (!new_channel && me->channel == ONE){ me->channel = ELEVEN; }
        else if (new_channel && me->channel == THREE){ me->channel = ELEVEN; }
        else if (!new_channel && me->channel == THREE){ me->channel = ONE; }
        else if (new_channel && me->channel == ELEVEN){ me->channel = THREE; }
        else if (!new_channel && me->channel == ELEVEN){ me->channel = ONE; }
        sem_wait(&me->noise_lock);
        me->interference = CLEAR;//only temporary
        sem_post(&me->noise_lock);
      }
    }
  }
  sem_wait(&me->shield_unlock);
  me->shield = 0;
  sem_post(&me->shield_unlock);

}

// If the neighbor is ready to receive the message, the message broadcasts
// The transmission time is checked after each node is checked, the elapsed time is calculated at that time too
void transmission_wait(struct timeval* msg_start, struct timeval* msg_end, Node* me){
  time_t now;
  int elapsed_time = 0;

  gettimeofday(msg_start, NULL);

  while(elapsed_time <= transmission_time){
    // If the node has an interference with noisemaker, then it is blocked
    if (me->interference > 0){
      printf("%d IS BLOCKED!\n", me->id);
    }

    else {
      Node* neighbor;

      for(int i = 0; i < me->nbr_size; i++){
        neighbor = me->nbrs[i];
        if(elapsed_time > transmission_time) {
          return;
        }
        else {
          // If neighbor is ready to receive the message, the message is rebroadcasted, otherwise do not send the message to that neighbor and check the next one
          if (neighbor->channel == me->channel && neighbor->type == LEGITIMATE && neighbor->action == NEITHER){
            sem_wait(&me->message_lock);
            now = time(0);
            Message* msg_to_send = retrieve(me->message_list);
            add_rebroadcast(neighbor->message_list, neighbor->file, 0, me->id, neighbor->channel, now, me->x_pos, me->y_pos, neighbor->id, neighbor);
            sem_post(&me->message_lock);
          }
          gettimeofday(msg_end, NULL);
          elapsed_time = ((msg_end->tv_sec * 1000 + msg_end->tv_usec / 1000) - (msg_start->tv_sec * 1000 + msg_start->tv_usec / 1000));
        }
      }
    }
    gettimeofday(msg_end, NULL);
    elapsed_time = ((msg_end->tv_sec * 1000 + msg_end->tv_usec / 1000) - (msg_start->tv_sec * 1000 + msg_start->tv_usec / 1000));
  }
}

void noisemaker_wait(struct timeval* start, struct timeval* end){
  int elapsed_time = 0;

  gettimeofday(start, NULL);

  int block_chance = rand() % 101;
  if (block_chance <= block_probability){

    int block_time = rand() % 1001;

    while(elapsed_time <= block_time){
      gettimeofday(end, NULL);
      elapsed_time =  ((end->tv_sec * 1000 + end->tv_usec / 1000) - (start->tv_sec * 1000 + start->tv_usec / 1000));
    }
  }

}

int try_message(struct timeval* think_start, struct timeval* think_end){
  int elapsed_time = 0;
  gettimeofday(think_start, NULL);

  while(elapsed_time <= talk_window_time){
    gettimeofday(think_end, NULL);
    elapsed_time =  ((think_end->tv_sec * 1000 + think_end->tv_usec / 1000) - (think_start->tv_sec * 1000 + think_start->tv_usec / 1000));
  }
  int message_probability = rand() % 101;
  if (talk_probability <= message_probability){
    return 1;
  }
  else{
    return 0;
  }
}
