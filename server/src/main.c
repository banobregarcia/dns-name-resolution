#include "header.h"
#include "check.h"

/**
 * @brief  free la mémoire
 *
 * @param options char ** à libérér
 * @result void
 */
void free_fields(char ** options)
{
    for (int i = 0; i < (int)sizeof(options); i++)
        free(options[i]);
    free(options);
}

/**
 * @brief charger les valeurs d'un file dans une structure servers_info
 *
 * @param servers struct déclaré à initialiser
 * @param fptr fichier à lire
 * @param buffer à lire
 * @result renvois la struct servers_info avec les valeurs du file dedans
 */

struct servers_info load_into_struct(struct servers_info servers, FILE * fptr, char *buffer)
{
    int i = 0;
    char *temp,*temp2;

    servers.domain = malloc(sizeof(char *)*(servers.nbr_servers + 1));
    servers.IP = malloc(sizeof(char *)*(servers.nbr_servers + 1));
    servers.PORT = malloc(sizeof(int)*(servers.nbr_servers + 1));

    while(fgets(buffer,SIZE,fptr) != NULL && i < servers.nbr_servers+1) {
        temp = strtok(buffer, "|");
        servers.domain[i] = malloc(sizeof(char)*(strlen(temp)+1));
        strcpy(servers.domain[i],temp);
        temp2 = strtok(NULL, "|");
        servers.IP[i] = malloc(sizeof(char)*(strlen(temp2)+1));
        strcpy(servers.IP[i],temp2);
        servers.PORT[i] = atoi(strtok(NULL, "|"));
        i++;
    }
    return (servers);
}


/**
 * @brief initialiser une structure servers_info et charge du file grâce au path
 *
 * @param servers serveurs à initialiser
 * @param path chemin vers le fichier à ouvrir
 * @result renvois la struct servers_info
 */

struct servers_info open_load_config_file(struct servers_info servers, char *path)
{
    FILE *fptr;
    char *s = malloc(sizeof(char) * 1042);
	servers.nbr_servers = 0;

    fptr = fopen(path, "r");

	if (!fptr) {
		char *er_mess = "File not found, verify your path!";
		fprintf(stderr, er_mess, strlen(er_mess));
		exit (EXIT_FAILURE);
	}
    while (fgets(s, SIZE, fptr) != NULL) {
        servers.nbr_servers++;
    }
    CHECK((fptr = fopen(path, "r")) != NULL);
    servers = load_into_struct(servers,fptr,s);
    fclose(fptr);
    return (servers);
}

/**
 * @brief  Concatène les différentes valeurs dans un string
 *
 * @param client struct avec les valeurs à mettre dans le string
 * @param code1ou0 code si ça marche ou pas
 * @param servers struct avec les infos du server
 * @result renvois la réponse du serveur
 */

char* answer_server (struct client_info client, struct servers_info servers)
{

    int code1ou0 = 1;
    int j = 0;
    for (j = 0; (j < servers.nbr_servers) && strcmp(client.domaine,servers.domain[j]) != 0; j++);

    if (j == servers.nbr_servers)
        code1ou0 = -1;

    char* id = malloc(sizeof(char));
    sprintf(id,"%d",client.id);

    char* horl = malloc(sizeof(char));
    sprintf(horl,"%d",client.horloge);

    char* code = malloc(sizeof(char));
    sprintf(code,"%d",code1ou0);

    int i = 0;
    char * port = malloc(sizeof(char));

    char* result = malloc(sizeof(char *)*2000);

    strcat(result,id);
    strcat(result,"|");
    strcat(result,horl);
    strcat(result,"|");
    strcat(result,client.nom);
    strcat(result,"|");
    strcat(result,code);

    if(code1ou0 == -1)
 	  strcat(result,"|");

     if(code1ou0 == 1) {
        while (i < servers.nbr_servers) {
            if(strcmp(client.domaine,servers.domain[i]) == 0) {
                strcat(result,"|");
                strcat(result,servers.domain[i]);
                strcat(result,",");
                strcat(result,servers.IP[i]);
                strcat(result,",");
                sprintf(port,"%d",servers.PORT[i]);
                strcat(result,port);
            }
          	i++;
    	}
    }

	free(id);
	free(horl);
	free(code);
	free(port);

    return result;
}

/**
 * @brief  compte le nombre de domaine que demande la requête
 *
 * @param requ la requête à tester
 * @result renvois le nombre de domaine contenue dans la requête
 */

int compdom(char* requ)
{
	int i = 0;
	int com = 0;
	for(i = 0; requ[i] != '\0'; i++) {
		if(requ[i]=='.')
			com++;
	}
	return com+1;
}

/**
  * @brief  stocke les données utilise envoyé par le client
  *
  * @param lreq requête envoyée par le client
  * @param  c structure client_info pour stocker les données du client
  * @result renvois une structure contenant les données utile pour répondre au client
 */
 struct client_info creer_client(char* lreq, struct client_info c)
 {
 	char* sep= "|";
 	char* domaine = NULL;
 	c.id = atoi(strtok(lreq,sep));
 	c.horloge = atoi(strtok(NULL,sep));
 	domaine = strtok(NULL,sep);
 	c.nom = domaine;

 	char* copy;
 	char sep2 = '.';
 	int nbdom = compdom(domaine)-c.id;

 	if (nbdom == 0) {
 		c.domaine =  c.nom;
 		return c;
 	}

 	int i;
 	for (i = 0; i < nbdom; i++) {
 		if(i == 0) {
 			copy = strpbrk(domaine,&sep2);
 		}else{
 			copy = strpbrk(copy+1,&sep2);
 		 }
 	}

 	if (nbdom == compdom(domaine) - 1)
 		 c.domaine = copy;
 	else
 		 c.domaine = copy+1;

 	return c;
 }


/**
 * @brief  Créer une socket de type Ipv4 pour un port
 *
 * @param port port utiliser pour créer la socket
 * @result renvois l'identifiant de la socket
 */

int createSocketIPv4 (int port)
{
    int sock;
    struct sockaddr_in my_addr;

    CHECK((sock = socket(AF_INET, SOCK_DGRAM, 0)) != -1);

    my_addr.sin_family    = AF_INET;
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    CHECK(bind(sock, (const struct sockaddr *)&my_addr, sizeof(struct sockaddr_in)) != -1);

    return sock;
}

/**
 * @brief  Créer une socket de type Ipv6 pour un port
 *
 * @param port port utiliser pour créer la socket
 * @result renvois l'identifiant de la socket
 */

int createSocketIPv6(int port) {
    int sock;
    struct sockaddr_in6 my_addr;

    CHECK((sock = socket(AF_INET6, SOCK_DGRAM, 0)) != -1);

    my_addr.sin6_family = AF_INET6;
    my_addr.sin6_port = htons(port);
    my_addr.sin6_addr = in6addr_any;

    CHECK(bind(sock,(const struct sockaddr *) &my_addr,sizeof(struct sockaddr_in6)) != -1);

    return sock;
}


/**
 * @brief créer un communication avec tout les clients pendants un temps donnée
 *
 * @param server serveur contenant les fichiers serveurs du domaines
 * @param portv4 port d'écoute pour les adresses ipv4
 * @param portv6 port découte pour les adresses ipv6
 * @param dormir entier correspond on temps de sleep du serveur pour tester les performances
 * @result rien
 */
void communication_client(struct servers_info server,int portv4,int portv6, int dormir)
{
	struct client_info info;
	int sockfd4,sockfd6;
	socklen_t addrlen;
	socklen_t addrlen6;
	char buf[SIZE];
	char* resultat_serv;
	memset(buf,'\0',SIZE);
	struct sockaddr_in client;

	addrlen = sizeof(struct sockaddr_in);
	addrlen6 = sizeof(struct sockaddr_in6);

	sockfd4 = createSocketIPv4(portv4);
	sockfd6 = createSocketIPv6(portv6);

	fd_set ensemble;


	while (1) {
		memset(buf,'\0',SIZE);
		FD_ZERO(&ensemble);
		FD_SET(sockfd4, &ensemble);
		FD_SET(sockfd6, &ensemble);

		CHECK(select(sockfd6+1, &ensemble, NULL, NULL, NULL) != -1);

		if (FD_ISSET(sockfd4,&ensemble)) {
			addrlen = sizeof(struct sockaddr_in);

			if (recvfrom(sockfd4,buf,SIZE,0,(struct sockaddr  *) &client, &addrlen) == -1) {
					perror("recvfrom");
					close(sockfd4);
					exit(EXIT_FAILURE);
			}

			//printf("client est %s \n",buf);

			info = creer_client(buf,info);

			//printf("le domaine est %s \n",info.domaine);

			resultat_serv = answer_server(info,server);

			//printf("%s \n",resultat_serv);

			strcpy(buf,resultat_serv);

			sleep(dormir);

			if (sendto(sockfd4,buf,sizeof(buf),0,(const struct sockaddr *) &client,addrlen) == -1) {
				perror("sendto");
				close(sockfd4);
				exit(EXIT_FAILURE);
			}

		}

		if (FD_ISSET(sockfd6,&ensemble)) {


			if (recvfrom(sockfd6,buf,SIZE,0,(struct sockaddr  *) &client, &addrlen6) == -1) {
				perror("recvfrom");
				close(sockfd6);
				exit(EXIT_FAILURE);
			}

			//printf("client est %s \n",buf);

			info = creer_client(buf,info);

			//printf("le domaine est %s \n",info.domaine);

			resultat_serv = answer_server(info,server);

			//printf("%s \n",resultat_serv);

			strcpy(buf,resultat_serv);

			sleep(dormir);

			if (sendto(sockfd6,buf,sizeof(buf),0,(const struct sockaddr *) &client,addrlen6) == -1) {
				perror("sendto");
				close(sockfd6);
				exit(EXIT_FAILURE);
			}

		}
	}

	CHECK(close(sockfd6) != -1);
	CHECK(close(sockfd4) != -1);
}



int main(int argc , char * argv[])
{
	if (argc != 5) {
		fprintf(stderr, "Usage: %s fichier_liste_serveurs portsipv4 portsipv6 sleep\n",argv[0]);
		return EXIT_FAILURE;
	};


    struct servers_info servers;
    servers = open_load_config_file(servers,argv[1]);

	communication_client(servers,atoi(argv[2]),atoi(argv[3]), atoi(argv[4]));

    return 0;

}
