#ifndef HEADER_SERVER
#define HEADER_SERVER

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>

#define SIZE 1024

struct servers_info
{
	int nbr_servers;
    char **domain;
	char **IP;
	int *PORT;
};

struct client_info
{
	int id;
	int horloge;
	char* nom;
    char* domaine;
};

struct request {
    int nb_commandes;
    char ** commandes;
};

#endif
