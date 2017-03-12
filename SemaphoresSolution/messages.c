#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "header.h"

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
/** The function to initialize a message list
* @param MessageList  The list to be initialized
* @return void The message initialized
*/
void initialize_message_list(MessageList *msg_list) {
    msg_list->front = (struct Message*)malloc(sizeof(struct Message));
    msg_list->rear = (struct Message*)malloc(sizeof(struct Message));
}

/** The function to add new message to the end of the message queue
* @param messages Messages of the node
* @param file File of the node
* @param new_id The new message id
* @param sender_id The id of the sender node
* @param channel The channel of broadcasted
* @param time The time when the node is received
* @param x_pos The x coordinate of the sender node
* @param y_pos The y coordinate of the sender node
* @param receiver_id The receiver id
* @param me The receiver node
* @return 0 if the rebroadcast was added successfully
*/
int add_rebroadcast(MessageList *messages, FILE *file, int new_id, int sender_id, int channel, time_t time, int x_pos, int y_pos, int receiver_id, Node* me) {
  // new_id is the id of the message we are passing

  struct Message *msg = new_message(new_id);

  if (messages->front == NULL || messages->rear == NULL) {
    messages->front = messages->rear->next = msg;
  }
  else {
    messages->rear = msg;
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
  for (int i = 0; i < me->MSGS_COUNT; i++){
    if (me->seen_msgs[i] == new_id){
      return 0;
    }
  }
      fprintf(file, "MessageID: %d\n", new_id);
      fprintf(file, "SenderID: %d\n", sender_id);
      fprintf(file, "Channel: %d\n", channel);
      fprintf(file, "Time: %ld\n", (long)time);
      fprintf(file, "x-pos: %d\n", x_pos);
      fprintf(file, "y-pos: %d\n", y_pos);
      fclose(file);


      me->seen_msgs[me->MSGS_COUNT] = new_id;
      me->MSGS_COUNT++;
      return 0;


}

/** The function adds the message to the begining of the message queue
* @param messages The messages of the node
* @param file The file of the node
* @param node_id The id of the node
* @param time_get The time of the node generation
* @param x_pos, y_pos X and Y coordinates of the node
*/
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

/** The function to retrieve the messages from the begining of the message queue with replacement
* @param messages The message list from where the messages should be retrieved
* @return Message The message that is removed from the message queue
*/
struct Message *retrieve(struct MessageList *messages) {

  if (messages->front == NULL) {
    printf("No messages generated to be sent right now");
  }

  if (messages->front != messages->rear) {
    struct Message *temp = messages->front->next;
    messages->front = temp;
    return temp;
  }

  // Check if front is null after removal
  if (messages->front == NULL) {
    printf("No messages generated to be sent right now");
  }

}
