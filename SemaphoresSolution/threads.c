#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "header.h"

void* threads(void* me_p){
  struct timeval start, end, msg_start, msg_end, think_start, think_end;
  time_t now;
  Node* me = (Node*)me_p;

  if (me->type == NOISEMAKER){
    noisemaker_thread(me);
  }

  printf("We are starting! This node is at (%d, %d)\n", me->x_pos, me->y_pos);

  while(1){
    int can_talk = try_message(&think_start, &think_end);
    if (!can_talk){
      continue;
    }

    sem_wait(&me->message_lock);
    now = time(0);
    add_transmit(me->message_list, me->file, me->id, now, me->x_pos, me->y_pos);
    sem_post(&me->message_lock);

    check_channel_change(&start, &end, me);
    printf("\nThe node at (%d, %d) has a channel of: %d!\n", me->x_pos, me->y_pos, me->channel);

    Node* neighbor;
    printf("The node at (%d, %d) is trying to set up its shields!\n", me->x_pos, me->y_pos);

    for (int i = 0; i < me->nbr_size; i++){
      printf("    The neighbor at (%d, %d) has this current shield value: %d and the current channel value: %d\n", me->nbrs[i]->x_pos, me->nbrs[i]->y_pos, me->nbrs[i]->shield,  me->nbrs[i]->channel);
    }
    //Put the shield on myself, so no other node can access me while I'm putting shields on my neighbors
    sem_wait(&me->shield_lock);
    me->shield = 1;
    printf("HERE");
    printf("The node at (%d, %d) has set up its own shield!\n", me->x_pos, me->y_pos);
    sem_post(&me->shield_lock);

    //try to put up our shields on every neighbor
    for (int i = 0; i < me->nbr_size; i++){
      neighbor = me->nbrs[i];
      //if someone else has aleady shielded this node:
      if (neighbor->shield == 0 && neighbor->channel == me->channel && neighbor->type == LEGITIMATE) {
        sem_wait(&neighbor->shield_lock);
        neighbor->shield = 1;
        sem_post(&neighbor->shield_lock);

        if (neighbor->action = NEITHER && neighbor->channel == me->channel && neighbor->type == LEGITIMATE) {
          sem_wait(&neighbor->action_lock);
          me->action = TRANSMITTING;
          neighbor->action = RECIEVING;
          sem_post(&neighbor->action_lock);

          transmission_wait(&msg_start, &msg_end, me);

          sem_wait(&neighbor->action_unlock);
          neighbor->action = NEITHER;
          sem_post(&neighbor->action_unlock);

          sem_wait(&neighbor->shield_unlock);
          neighbor->shield = 0;
          sem_post(&neighbor->shield_unlock);
        }
      }
      if (neighbor->shield == 1 && neighbor->channel == me->channel && neighbor->type == LEGITIMATE) {
        sem_wait(&me->shield_unlock);
        me->shield = 0;
        sem_post(&me->shield_unlock);
      }
    }
  }

}


void noisemaker_thread(Node* me){

  struct timeval start, end;
  Node* neighbor;

  while (1){
    for(int i = 0; i < me->nbr_size; i++){
      neighbor = me->nbrs[i];
      if (me->channel == neighbor->channel){
        sem_wait(&neighbor->noise_lock);
        neighbor->interference++;
        sem_post(&neighbor->noise_lock);
      }
    }

    noisemaker_wait(&start, &end);

    for(int i = 0; i < me->nbr_size; i++){
      if (me->channel == neighbor->channel){
        sem_wait(&neighbor->noise_lock);
        neighbor->interference--;
        sem_post(&neighbor->noise_lock);
      }
    }
  }

}
