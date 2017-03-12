#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "proj3hdr.h"

/** The function to initialize the messages list
* @param msg The message to be initialized
* @return void The zero-th message is initialized
*/
struct Message *new_message(int id) {
  struct Message *msg = (struct Message*)malloc(sizeof(struct Message));
  msg->id = id;
  msg->next = NULL;
  return msg;
}

void initialize_message_list(MessageList *msg_list) {
    msg_list->front = (struct Message*)malloc(sizeof(struct Message));
    msg_list->rear = (struct Message*)malloc(sizeof(struct Message));
}

int add_rebroadcast(MessageList *messages, FILE *file, int new_id, int sender_id, int channel, time_t time, int x_pos, int y_pos, int receiver_id, Node* me) {
  // new_id is the id of the message we are passing

  struct Message *msg = new_message(new_id);

  if (messages->front == NULL || messages->rear == NULL) {
    messages->front = messages->rear = msg;
  }
  else {
    messages->rear->next = msg;
  }

  char buf_receiver_id[9];
  sprintf(buf_receiver_id, "%d", receiver_id);

  // Concatenate two characters
  const char* filename = strcat(buf_receiver_id, ".txt");

  // Open the file
  file = fopen(filename, "a");

  // The id of the message is 5 digit number, create the buffer to store this int as character string
  char buf_id[6];

  // Write into buf
  sprintf(buf_id, "%d", new_id);

  if (file == NULL) {
    printf("Error! cant open log file!");
    return -1;
  }
  // Check if file already contains the message, if so ignore the message
  pthread_mutex_lock(&me->msgcount_lock);
  for (int i = 0; i < me->MSGS_COUNT; i++){
    if (me->seen_msgs[i] == new_id){
      pthread_mutex_unlock(&me->msgcount_lock);
      return 0;
    }
  }

    //if (!(strstr((char*)file, buf_id))) {
      fprintf(file, "MessageID: %d\n", new_id);
      fprintf(file, "SenderID: %d\n", sender_id);
      fprintf(file, "Channel: %d\n", channel);
      fprintf(file, "Time: %ld\n", (long)time);
      fprintf(file, "x-pos: %d\n", x_pos);
      fprintf(file, "y-pos: %d\n", y_pos);
      fclose(file);

      me->seen_msgs[me->MSGS_COUNT] = new_id;
      me->MSGS_COUNT++;
      pthread_mutex_unlock(&me->msgcount_lock);

      return 0;
    //}

}

// Add the message to random
int add_transmit(MessageList *messages, FILE *file, int node_id, time_t time_get, int x_pos, int y_pos) {
  srand(time(NULL));
  // Generate a random index from 0 to 9 of the global array of message id's
  int rand_index = rand()%10;
  int new_id = MESSAGES[rand_index];

  int channel = rand()%3;
  if (channel == 0) channel = 1;
  if (channel == 1) channel = 6;
  if (channel == 2) channel = 11;

  struct Message *msg = new_message(new_id);

  if (messages->front == NULL || messages->rear == NULL) {
    messages->front = messages->rear = msg;
  }
  else {
    struct Message *temp = messages->front;
    messages->front = msg;
    messages->front->next = temp;
  }

  char buf_receiver_id[9];
  sprintf(buf_receiver_id, "%d", node_id);

  // Concatenate two characters
  const char* filename = strcat(buf_receiver_id, ".txt");

  //Open the file
  file = fopen(filename, "a");

  // The id of the message is 5 digit number, create the buffer to store this int as character string
  char buf_id[6];

  // Write into buf
  sprintf(buf_id, "%d", new_id);

  char *line_to_read;

  if (file == NULL) {
    printf("Error! cant open log file!");
    return -1;
  }
  if (file) {
    //while(line_to_read = read_line(file)) {
      // Check if file already contains the message, if so ignore the message
      if (!(strstr((char*)line_to_read, buf_id))) {
        fprintf(file, "We are transmitting a new message!\n");
        fprintf(file, "MessageID: %d\n", new_id);
        fprintf(file, "SenderID: %d\n", node_id);
        fprintf(file, "Channel: %d\n", channel);
        fprintf(file, "Time: %ld\n", (long)time_get);
        fprintf(file, "x-pos: %d\n", x_pos);
        fprintf(file, "y-pos: %d\n", y_pos);
        fclose(file);
        return 0;
      }
  }

}

// Enqueue the first message form message list
struct Message *retrieve(struct MessageList *messages) {
  // If the list is empty
  int rand_index = rand()%10;
  if (messages->front == NULL) return new_message(MESSAGES[rand_index]);

  struct Message *temp = messages->front;
  messages->front = messages->front->next;

  // Check if front is null after removal
  if (messages->front == NULL) return new_message(MESSAGES[rand_index]);

  return temp;
}

