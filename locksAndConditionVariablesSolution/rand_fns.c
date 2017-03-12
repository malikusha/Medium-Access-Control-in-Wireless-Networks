#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#include "proj3hdr.h"

void add_rand_nodes(){

  //set our randomizer
  srand(time(NULL));

  //we add nodes in types based on the number of each node we still need to add
  int remaining_nodes, remaining_noise, total_nodes, type, x_pos, y_pos, type_pick, frequency;
  remaining_nodes = num_nodes;
  remaining_noise = num_noisemakers;

  total_nodes = remaining_nodes + remaining_noise;
  Node fill_out;

  //loop through until we run out of any node to add
  while (total_nodes > 0){
    x_pos = rand() % 100;
    y_pos = rand() % 100;
    if (field[x_pos][y_pos] == NULL){ //place each node in an unoccupied array space
      type_pick = rand() % total_nodes + 1;
      if(remaining_nodes-type_pick >= 0){
        type = LEGITIMATE; //adds a real node
        remaining_nodes--;
      }
      else{
        type = NOISEMAKER; //adds a noisemaker
        remaining_noise--;
      }
      total_nodes--;
      frequency = rand() % 3; //pick a ranoom frequency to start
      add_node(frequency, x_pos, y_pos, type);
      printf("A new node of type %d was added at the position (%d, %d).\n", type, x_pos, y_pos);
    }
  }
}

//changes the channel of a node
void check_channel_change(struct timeval* start, struct timeval* end, Node* me){

  //grab our shield to make sure we an undisturbed
  pthread_mutex_lock(&me->shield_lock);
  printf("The node at (%d, %d) is changing its channel!\n", me->x_pos, me->y_pos);
  //wait until we get our shield
  while (me->shield == 1) {
    pthread_cond_wait(&me->cond, &me->shield_lock);
  }
  me->shield = 1;
  pthread_mutex_unlock(&me->shield_lock);

  //wait until nobody is still transmitting to our node
  pthread_mutex_lock(&me->action_lock);
  while (me->action != NEITHER){
    pthread_cond_wait(&me->cond, &me->action_lock);
  }
  pthread_mutex_unlock(&me->action_lock);

  gettimeofday(end, NULL);
  int elapsed_time = ((end->tv_sec * 1000 + end->tv_usec / 1000) - (start->tv_sec * 1000 + start->tv_usec / 1000));
  //if we have waited past dwell_duration, we decide if we will change channels
  if (elapsed_time >= dwell_duration){
    int channel_stay = rand() % 100 + 1;
    //if we decide to change (using the dwell probability)
    if (channel_stay > dwell_probability){
      gettimeofday(start, NULL);
    }
    else {
      gettimeofday(start, NULL);
      //we change based on what we previously were
      int new_channel = rand() % 2;
      if (new_channel && me->channel == ONE){ me->channel = THREE; }
      else if (!new_channel && me->channel == ONE){ me->channel = ELEVEN; }
      else if (new_channel && me->channel == THREE){ me->channel = ELEVEN; }
      else if (!new_channel && me->channel == THREE){ me->channel = ONE; }
      else if (new_channel && me->channel == ELEVEN){ me->channel = THREE; }
      else if (!new_channel && me->channel == ELEVEN){ me->channel = ONE; }
      pthread_mutex_lock(&me->noise_lock); //we reset the noisemakers bothering us (were on a different channel now)
      me->interference = CLEAR;//only temporary
      pthread_mutex_unlock(&me->noise_lock);
    }
  }
  //put away our shield because we are done
  pthread_mutex_lock(&me->shield_lock);
  me->shield = 0;
  pthread_cond_broadcast(&me->cond);
  pthread_mutex_unlock(&me->shield_lock);

  //pthread_mutex_unlock(&me->channel_lock);
}

//changes the noisemaker channel
void check_channel_change_noisemaker(struct timeval* start, struct timeval* end, Node* me){
//same as changing the node channel but we never have to shield oursleves or reset our data
  gettimeofday(end, NULL);
  int elapsed_time = ((end->tv_sec * 1000 + end->tv_usec / 1000) - (start->tv_sec * 1000 + start->tv_usec / 1000));
  if (elapsed_time >= dwell_noisemakers){
    int channel_stay = rand() % 100 + 1;
    if (channel_stay < dwell_probability_noisemakers){
      gettimeofday(start, NULL);
    }
    else {
      gettimeofday(start, NULL);
      int new_channel = rand() % 2;
      if (new_channel && me->channel == ONE){ me->channel = THREE; }
      else if (!new_channel && me->channel == ONE){ me->channel = ELEVEN; }
      else if (new_channel && me->channel == THREE){ me->channel = ELEVEN; }
      else if (!new_channel && me->channel == THREE){ me->channel = ONE; }
      else if (new_channel && me->channel == ELEVEN){ me->channel = THREE; }
      else if (!new_channel && me->channel == ELEVEN){ me->channel = ONE; }
    }
  }
}

//this function is used to wait while we transmit a message
void transmission_wait(struct timeval* msg_start, struct timeval* msg_end, Node* me){
  time_t now;
  int elapsed_time = 0;
  gettimeofday(msg_start, NULL);
  if (me->interference > 0){ //we cant send a message if a noisemaker is bothering us
    printf("I'm being blocked!\n");
  }
  else {
    Node* neighbor;
    for(int i = 0; i < me->nbr_size; i++){ //pick up all the neighbors we are connected to
      neighbor = me->nbrs[i];
      if (neighbor->channel == me->channel && neighbor->type == LEGITIMATE){
        now = time(0); //set up the time we send our message
        Message* msg_to_send = retrieve(me->message_list); //send the message and report on it
        //our reciever will add it into the messages he needs to rebroadcast
        add_rebroadcast(neighbor->message_list, neighbor->file, msg_to_send->id, me->id, neighbor->channel, now, me->x_pos, me->y_pos, neighbor->id, neighbor);
      }
    }
  }
  //wait out until the transmission time is over
  while(elapsed_time <= transmission_time){
    gettimeofday(msg_end, NULL);
    elapsed_time = ((msg_end->tv_sec * 1000 + msg_end->tv_usec / 1000) - (msg_start->tv_sec * 1000 + msg_start->tv_usec / 1000));
  }
}

//this function determines whether a noisemaker will start interfering and for how long
void noisemaker_wait(struct timeval* start, struct timeval* end){
  int elapsed_time = 0;
  gettimeofday(start, NULL);
  int block_chance = rand() % 101; //check if we can block or not
  if (block_chance <= block_probability){
    int block_time = rand() % 1001; //you can block for up to 1 second
    while(elapsed_time <= block_time){
      gettimeofday(end, NULL);
      elapsed_time =  ((end->tv_sec * 1000 + end->tv_usec / 1000) - (start->tv_sec * 1000 + start->tv_usec / 1000));
    }
  }

}

//this function indicates the time required for the node to decide whether to transfer
int try_message(struct timeval* think_start, struct timeval* think_end){
  int elapsed_time = 0;
  gettimeofday(think_start, NULL);
  while(elapsed_time <= talk_window_time){
    gettimeofday(think_end, NULL);
    elapsed_time =  ((think_end->tv_sec * 1000 + think_end->tv_usec / 1000) - (think_start->tv_sec * 1000 + think_start->tv_usec / 1000));
  } //decide based on the probability whether to send a message this time
  int run_message = rand() % 100 + 1;
  if (run_message < talk_probability){
    return 0;
  }
  else{
    return 1;
  }
}
