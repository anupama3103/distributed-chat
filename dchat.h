#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <stdint.h>

//#include "dchat.h"
//#include "holdback_queue.h"
#include "queue.h"

#define QUEUE_SIZE 128
#define INITIAL_CLIENT_COUNT 8
#define MSGBUFSIZE 256


//Message types
enum bool_t {TRUE=1,FALSE=0};
enum packettype_t { CHAT = 0, SEQUENCE = 1, CHECKUP = 2, ELECTION = 3, VOTE = 4, VICTORY = 5, JOIN_REQUEST = 6, LEADER_INFO = 7, JOIN = 8};
//

// Function Declarations
void print(clist *); // print client list
void getLocalIp(char*);
void holdElection();

int isSequencer = 0;
int alloc_clients_size;

clist *clientlist;

const int LOCALPORT = 2886;
cname userdata;
char buf[BUFLEN];

const int MAXSENDERLEN = 64;
const int MAXUIDLEN = 128;
const int MAXPACKETLEN = 1024;
const int MAXPACKETBODYLEN = MAXPACKETLEN-MAXSENDERLEN-MAXUIDLEN-(2*sizeof(int))-sizeof(packettype_t)-(5*sizeof(char));
const int MESSAGEMULTIPLIER = 10;
const int MAXCHATMESSAGELEN = MAXPACKETBODYLEN*MESSAGEMULTIPLIER;

static llist_t* UNSEQ_CHAT_MSGS;


typedef struct packet_t {
  char sender[MAXSENDERLEN];
  char uid[MAXUIDLEN];
  packettype_t packettype;
  int packetnum;
  int totalpackets;
  int packetbody[MAXPACKETBODYLEN];
} packet_t;

typedef struct chatmessage_t {
  int seqnum;
  int numpacketsexpected;
  bool_t iscomplete;
  bool_t packetsreceived[MESSAGEMULTIPLIER];//indicates which packets have been received
  char sender[MAXSENDERLEN];
  char uid[MAXUIDLEN];
  char messagebody[MAXCHATMESSAGELEN];
} chatmessage_t;

// rpc part start

const MAX_MSG_LEN = 512;        /* 64 Bytes */
const MAX_USR_LEN = 32;         /* 4 Bytes  */
const MAX_IP_LEN =  32;		    /* 4 Bytes  */
const BUFLEN = 556;             /* WTF MATE */

typedef string msg_send<MAX_MSG_LEN>;
typedef string uname<MAX_USR_LEN>;
typedef string hoststr<MAX_IP_LEN>;

enum msg_type_t {TEXT = 0, NEWUSER = 1, USEREXIT = 2, ELECTION = 3};
enum status_code {JSUCCESS = 0, JFAILURE = 1, UNAMEINUSE = 2, UNAMEINVALID = 3};


struct cname {
    
    uname userName;
    hoststr hostname;
    int lport;
    int leader_flag;
    
};

typedef struct cname cname;

struct clist {
    cname clientlist<>;
    
};

struct msg_recv {
    msg_send msg_sent;
    uname user_sent;
    unsigned int seq_num;
    msg_type_t msg_type;
};

//rpc part end


clist *clients;
msg_recv *msg_buffer;

char buf[BUFLEN];

int initialized = FALSE;
int alloc_client_size;
int seq_num = 0;

void error(char*);

bool_t check_chatmessage_completeness(chatmessage_t*);

//create a new chatmessgae given a packet
chatmessage_t* create_chatmessage(packet_t*);

//add this CHAT packet's contents to the appropriate chat message
//returns whether or not this message is now complete
bool_t append_to_chatmessage(chatmessage_t*, packet_t*);

// comparing sequence number of messages to print it acc to total ordering
int message_compare(const void* varname, const void*);

// chack if input is of-> enum msg_type_t;TEXT = 0, NEWUSER = 1, USEREXIT = 2, ELECTION = 3};
packet* parsePacket(char*);

chatmessage_t* find_chatmessage(char[])

void receive_UDP_packet();

// incomplete
// discover IP address using name
void getLocalIp(char*);

void print_client_list(clist*);

//incomplete
void holdElection();

// add some way to check if client is alive
int initialize_data_structures();

void destroy_data_structures();

void send_UDP_packet( msg_type_t, int, uname, msg_send);

void multicast_clients(uname, hoststr, int, int);

void multicast_exit(uname*);

//add to client list
int add(cname *);

void send_UDP_packet(msg_recv *);

// retry getting message
msg_recv* retry(int*);

// exit of a client
void exit(uname*);

// keep checking if sequencer is alive
int check();

void shutdown();