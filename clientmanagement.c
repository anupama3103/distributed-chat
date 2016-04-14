#include "clientmanagement.h"

void print_client_list() {
  int numclients = CLIENTS->numnodes;
    
  printf("Total # of Clients:\t%d\n", numclients);

  node_t* curr = CLIENTS->head;
  while(curr != NULL)
  {
    printf("%s %s: %d",((client_t*)curr->elem)->username,
	   ((client_t*)curr->elem)->hostname,
	   ((client_t*)curr->elem)->portnum);

    if(((client_t*)curr->elem)->isleader == TRUE)
            printf(" I...am...LEADER!!!");
    printf("\n");
    curr = curr->next;
  }
}

client_t* create_client(char username[], char hostname[], int portnum, bool isleader)
{
  client_t* newclient = (client_t*)malloc(sizeof(client_t));
  strcpy(newclient->username,username);
  strcpy(newclient->hostname,hostname);
  newclient->portnum = portnum;
  newclient->isleader = isleader;
  return newclient;
}

client_t* add_client(char username[], char hostname[], int portnum, bool isleader)
{
  client_t* newclient = create_client(username,hostname,portnum,isleader);
  if(portnum==LOCALPORT && strcmp(hostname,LOCALHOSTNAME) == 0)
  {
    me = newclient;
    uihostname = me->hostname;
    uiport = me->portnum;
  }
  add_elem(CLIENTS,(void*)newclient);
  return newclient;
}

void remove_client(char hostname[], int portnum)
{
  client_t* client;
  node_t* curr = CLIENTS->head;
  bool found = FALSE;
  while(curr != NULL)
  {
    client = ((client_t*)curr->elem);
    if(strcmp(hostname,client->hostname) == 0 && portnum == client->portnum)
    {
      found = TRUE;
      break;
    }
    curr = curr->next;
  }
  if(found)
  {
    remove_node(CLIENTS,curr);
  }
  free(client);
  client = NULL;
}

client_t* find_first_client_by_username(char username[])
{
  client_t* client;
  node_t* curr = CLIENTS->head;
  while(curr != NULL)
  {
    client = ((client_t*)curr->elem);
    if(strcmp(username,client->username) == 0)
      return client;
    curr = curr->next;
  }
  return NULL;
}

void holdElection() {
    //Elect a new sequencer
}
