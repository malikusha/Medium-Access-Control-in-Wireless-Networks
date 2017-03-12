#ifndef HEADER
#define HEADER

#include <pthread.h>
#include <semaphore.h>

//redefine our channel numbers so that they arent arbitrary
#define ONE 1
#define THREE 2
#define ELEVEN 3

#define LEGITIMATE 0
#define NOISEMAKER 1

#define TRANSMITTING 0
#define RECIEVING 1
#define NEITHER -1

#define CLEAR 0;
#define BLOCKED 1;

#define ON 1
#define OFF 0

//add a "broadcasting" condition variable

typedef struct Node Node;
typedef struct Message Message;
typedef struct MessageList MessageList;


struct Message {
  int id;
  Message *next; // the head of the message list
};

struct MessageList {
  struct Message *front, *rear;
};

int MESSAGES[10];

//a node that wants to broadcast scans tries to lock each of its links with a corresponding value

struct Node {
  int channel; //one of the three defined above
  int x_pos;
  int y_pos;
  int type; //can be legit or a NOISEMAKER
  int action;
  int shield;
  int interference;
  Node* nbrs [100];
  int nbr_size;
  int id;
  // The messages that have already been broadcasted
  int seen_msgs[10];
  int MSGS_COUNT;
  MessageList* message_list;
  FILE* file;
  pthread_t thread;
  sem_t shield_lock; //most useful when two neighbors both want to talk to you
  sem_t shield_unlock;
  sem_t action_lock;
  sem_t action_unlock;
  sem_t noise_lock;
  sem_t message_lock;
  //Messages* message_list;

};

Node* field [100][100];

int CURRENT_NUM;

int num_nodes;
int num_noisemakers;
int dwell_duration; //time in milliseconds
int dwell_probability; //an int out of 100
int transmission_time;
int talk_window_time;
int talk_probability;
int block_probability;

Node* initialize_node(int frequency, int x_pos, int y_pos, int type);
void find_neighbors(Node* node_ptr, int x_pos, int y_pos);
void initialize_field();
void print_nodes_located();
void add_rand_nodes();
void* node_thread(void* node_void_ptr);
void check_channel_change(struct timeval* start, struct timeval* end, Node* me);
void transmission_wait(struct timeval* msg_start, struct timeval* msg_end, Node* me);
int try_message(struct timeval* think_start, struct timeval* think_end);
void* threads(void* me_p);
void noisemaker_wait(struct timeval* start, struct timeval* end);
void noisemaker_thread(Node* me);
struct Message *new_message(int id);
void initialize_message_list(MessageList *msg_list);
int add_rebroadcast(MessageList *messages, FILE *file, int new_id, int sender_id, int channel, time_t time, int x_pos, int y_pos, int receiver_id, Node* me);
int add_transmit(MessageList *messages, FILE *file, int node_id, time_t time_get, int x_pos, int y_pos);
struct Message *retrieve(struct MessageList *messages);

#endif
