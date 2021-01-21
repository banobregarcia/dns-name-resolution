#ifndef HEADER_CLIENT
#define HEADER_CLIENT

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
#include <ctype.h>
#include <curses.h>
#include <sys/time.h>
#include <time.h>

#define TAILLE 2048
#define SIZE 5000
#define MAXSERV 2000
#define SIZE2 1024

struct servers_info
{
	int nbr_servers;
	char** IP;
	int* PORT;
};

struct ensemble_serv
{
    char** domaine;
    struct servers_info serveurs;
    int* utilisable;
    int* etape;
};

struct request {
    int nb_commandes;
    char ** commandes;
};

#endif
