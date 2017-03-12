#ifndef HEADER
#define HEADER

#include <pthread.h>

//redefine our channel numbers so that they arent arbitrary
#define ONE 1
#define THREE 2
#define ELEVEN 3

//are we a node or noisemaker
#define LEGITIMATE 0
#define NOISEMAKER 1

//what is a node doing
#define TRANSMITTING 0
#define RECIEVING 1
#define NEITHER -1

//is a noisemaker messing with us
#define CLEAR 0;
#define BLOCKED 1;

#define ON 1
#define OFF 0

//add a "broadcasting" condition variable

typedef struct Node Node;
typedef struct Message Message;
typedef struct MessageList MessageList;

//holds data about messages the node wants to send
struct Message {
  int id;
  Message *next; // the head of the message list
};

//the messages are put in a linked list
struct MessageList {
  struct Message *front, *rear;
};

//all the messages we can send
int MESSAGES[10];

//a node that wants to broadcast scans tries to lock each of its links with a corresponding value

//everything we need to know about a node
struct Node {
  int channel; //one of the three defined above
  int x_pos; //in the array
  int y_pos;
  int type; //can be legit or a NOISEMAKER
  int action;//are we in a network right now
  int shield;//is someone checking us right now
  int interference; //is a noisemaker messing with us
  Node* nbrs [100];
  int nbr_size; //keeps track of our neighbors to the node
  int id;
  // The messages that have already been broadcasted
  int seen_msgs[10]; //we keep track of the messages we have seen
  int MSGS_COUNT;
  MessageList* message_list; //a list of messages to send
  FILE* file;
  pthread_t thread; //each node has a thread

  pthread_mutex_t shield_lock; //most useful when two neighbors both want to talk to you
  pthread_mutexattr_t shield_attr; //we modify the attribute of each lock
  pthread_mutex_t action_lock;
  pthread_mutexattr_t action_attr;
  pthread_mutex_t noise_lock;
  pthread_mutexattr_t noise_attr;
  pthread_mutex_t msgcount_lock;
  pthread_mutexattr_t msgcount_attr;

  //pthread_cond_t lock_cond;
  pthread_cond_t cond; //we only need one per node to talk to the other nodes

  //Messages* message_list;
};

Node* field [100][100];

int CURRENT_NUM;

//all the values we are required to tweak
int num_nodes;
int num_noisemakers;
int dwell_duration; //time in milliseconds
int dwell_probability; //an int out of 100
int dwell_noisemakers;
int dwell_probability_noisemakers;
int transmission_time; //in ms
int talk_window_time;
int talk_probability; //out of 100
int block_probability; //out of 100

//lots of prototypes
void add_node(int frequency, int x_pos, int y_pos, int type);
void find_neighbors(Node* node_ptr, int x_pos, int y_pos);
void initialize_field();
void initialize_threads();
void add_rand_nodes();
void* node_thread(void* node_void_ptr);
void check_channel_change(struct timeval* start, struct timeval* end, Node* me);
void check_channel_change_noisemaker(struct timeval* start, struct timeval* end, Node* me);
void transmission_wait(struct timeval* msg_start, struct timeval* msg_end, Node* me);
int try_message(struct timeval* think_start, struct timeval* think_end);
void noisemaker_wait(struct timeval* start, struct timeval* end);
void noisemaker_thread(Node* me);
struct Message *new_message(int id);
void initialize_message_list(MessageList *msg_list);
int add_rebroadcast(MessageList *messages, FILE *file, int new_id, int sender_id, int channel, time_t time, int x_pos, int y_pos, int receiver_id, Node* me);
int add_transmit(MessageList *messages, FILE *file, int node_id, time_t time_get, int x_pos, int y_pos);
struct Message *retrieve(struct MessageList *messages);

#endif
