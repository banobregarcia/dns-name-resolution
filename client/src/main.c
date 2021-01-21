#include "header.h"
#include "check.h"

/**
 * @brief charger les valeurs d'un file dans une structure servers_info
 *
 * @param servers struct déclaré à initialiser
 * @param fptr fichier à lire
 * @param buffer à lire
 * @result renvois la struct servers_info avec les valeurs du file dedans
 */

struct servers_info load_into_struct(struct servers_info servers, FILE *fptr, char *buffer)
{
	int i = 0;
	char *temp;
	servers.IP = malloc(sizeof(char *) * (servers.nbr_servers + 1));
	servers.PORT = malloc(sizeof(int) * (servers.nbr_servers + 1));
	while (fgets(buffer, SIZE, fptr) != NULL && i <= servers.nbr_servers) {
	        temp = strtok(buffer, "|");
		servers.IP[i] = malloc(sizeof(char) * (strlen(temp) + 1));
		strcpy(servers.IP[i], temp);
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
	servers = load_into_struct(servers, fptr, s);
	fclose(fptr);
	return (servers);
}


/**
 * @brief  prendre les valeurs d'un fichier et charger
 * les valeurs dans un struct request
 *
 * @param path chemin vers le fichier à ouvrir
 * @result renvois la struct request
 */

struct request load_commands_from_file(char *path)
{
    FILE *fptr;
    char *buffer = malloc(sizeof(char) * 1042);
    int i = 0;
    struct request rqst;
    rqst.nb_commandes = 0;
    rqst.commandes = NULL;

    fptr = fopen(path,"r");
    if (!fptr) {
		char *er_mess = "File not found, verify your path!";
		fprintf(stderr, er_mess, strlen(er_mess));
		exit (EXIT_FAILURE);
	}
    while (fgets(buffer, SIZE, fptr) != NULL) {
	       rqst.nb_commandes++;
	}
    rqst.commandes = malloc(sizeof(char *)*(rqst.nb_commandes+1));
    CHECK((fptr = fopen(path, "r")) != NULL);
    while(fgets(buffer,SIZE,fptr) != NULL && i < rqst.nb_commandes+1) {
        rqst.commandes[i] = malloc(sizeof(char)*(strlen(buffer)+1));
        strcpy(rqst.commandes[i],buffer);
        i++;
    }
    for(int i = 0; i < rqst.nb_commandes; i++) {
        printf("Les commandes sont %s", rqst.commandes[i]);
    }

    CHECK(fclose(fptr) == 0);
    return (rqst);
}



/**
 * @brief  rempli une struct serveur avec le string découpé
 *
 * @param serv à remplir avec les valeurs du char *
 * @param reponse string avec la réponse du serveurs
 * @result mets la réponse du serveur sous forme d'une struct servers_info
 */

struct servers_info prendre_serveur(struct servers_info serv, char* reponse)
{
	const char * sep = ",|";
	char *cs = NULL;
    int i = 0;
    int val;

	cs = strtok(reponse,sep);
	cs = strtok(NULL,sep);
	cs = strtok(NULL,sep);
	cs = strtok(NULL,sep);
	cs = strtok(NULL,sep);

	while (cs != NULL) {
		serv.IP[i] = strtok(NULL,sep);
		val = atoi(strtok(NULL,sep));
		serv.PORT[i] = val;
		printf("%s, %d\n", serv.IP[i],serv.PORT[i]);
		cs = strtok(NULL,sep);
		i++;
	}

	serv.nbr_servers = i;
	return serv;
}

/**
 * @brief initialiser une structure ensemble de serveur
 *
 * @result renvoie la structure ensemble de serveur initialiser

 */

 struct ensemble_serv initensemble()
 {
 	struct ensemble_serv ens;

 	ens.serveurs.IP = malloc(sizeof(char *) * (SIZE2));
 	ens.serveurs.PORT = malloc(sizeof(int) * (SIZE2));
 	ens.domaine = malloc(sizeof(char *) * (SIZE2));
 	ens.utilisable = malloc(sizeof(int)*(SIZE2));
 	ens.etape = malloc(sizeof(int)*(SIZE2));

     int i = 0;
     while (i < SIZE2) {
 		ens.serveurs.IP[i] = malloc(sizeof(char)*300);
 		ens.domaine[i] = malloc(sizeof(char)*300);
 		i++;
 	}

 	ens.serveurs.nbr_servers = 0;
 	return ens;
 }


 /**
  * @brief  crée le requête à envoyer au serveur
  *
  * @param r string utilisé pour stocker le resultat
  * @param etp nombre de l'étape qui représente le domaine demandé
  * @param lreq commande à envoyer au serveur
  * @param horloge horodatage à utiliser
  * @result renvois la liste d'entier triée
  */

char* crea_result(char* r, int etp,char* lreq, int horloge)
{
	char result[1024] = {0};
	char horl[100];
	sprintf(result,"%d",etp);
	sprintf(horl,"%d",horloge);
	result[strlen(result)+1] = '\0';
	horl[strlen(horl)+1] = '\0';

	result[strlen(result)] = '|';
	result[strlen(result)+1] = '\0';

	horl[strlen(horl)] = '|';
	horl[strlen(horl)+1] = '\0';

	strcat(result,horl);
	strcat(result,lreq);
	result[strlen(result)-1] = '\0';

	r = result;

 return r;
}


/**
 * @brief  détermine le type ip d'une adresse
 *
 * @param ip adresse ip à tester
 * @result renvois 4 si l'ip est ipv4 et 6 si ipv6
 */

int ip_type(char* ip)
{
	int i;
	for (i = 0; ip[i] != '\0'; i++) {
		if(ip[i]==':')
            return 6;
	}
    return 4;
}

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
 * @brief  compte le nombre de domaine que demande la requête
 *
 * @param requ la requête à tester
 * @result renvois le nombre de domaine contenue dans la requête
 */
int compdom(char* requ)
{
	int i = 0;
	int com = 0;
	for (i = 0; requ[i] != '\0'; i++) {
		if(requ[i]=='.')
            com++;
	}

	if(requ[0] == '.')
        return 1;

	return com+1;
}

/**
 * @brief stocker les nouvelles adresses données par le serveur
 *
 * @param serv serveur pour stocker les données envoyé par le serveur
 * @param reponse reponse du serveur à utiliser
 * @result renvois une structure serveur_info  contenant les nouvelles adresses
 */

struct servers_info newadd(struct servers_info serv, char* reponse)
{

    const char * sep = ",|";
	char *cs = NULL;

	cs = strtok(reponse,sep);
	cs = strtok(NULL,sep);
	cs = strtok(NULL,sep);
	cs = strtok(NULL,sep);

    if(atoi(cs) == -1) {
        serv.PORT[0] = 0;
        return serv;
    }

	cs = strtok(NULL,sep);

	int i = 0;
	int val;

	while (cs != NULL) {
		serv.IP[i] = strtok(NULL,sep);
		val = atoi(strtok(NULL,sep));
		serv.PORT[i] = val;
		cs = strtok(NULL,sep);
		i = i+1;
	}

	//serv.PORT[i+1] = 0;
    serv.PORT[i] = 0;
	return serv;
}


/**
 * @brief initialise une copie des adresses d'un serveur
 *
 * @param serv serveur contenant les adresses à copier
 * @result renvois la copie des adresses serveur
 */

char** copieadd(struct servers_info serv)
{
	char** add = malloc(sizeof(char *)*(TAILLE));

    int i = 0;
    while (i <= serv.nbr_servers) {
		add[i] = malloc(sizeof(char)*300);
		i++;
	}

	int max = serv.nbr_servers;

	for (i= 0; i < max; i++) {
		strcpy(add[i],serv.IP[i]);
	}

	return add;
}

/**
 * @brief initialise une copie des ports d'un serveur
 *
 * @param serv serveur contenant les ports à copier
 * @result renvois la copie des ports serveur
 */
int* copieport(struct servers_info serv)
{
    int* port = malloc(sizeof(int)*(TAILLE));
	int i = 0;
	int max = serv.nbr_servers;

	for (i = 0; i< max; i++)
		port[i] = serv.PORT[i];

	return port;
}


/**
 * @brief initialise une copie des ports d'un serveur
 *
 * @param serv serveur contenant les ports à copier
 * @result renvois la copie des ports serveur
 */
int compterserv(int * ens)
{
	int i = 0;
	for (i = 0 ; ens[i] != 0;i++);
	return i;
}


/**
 * @brief initialise une copie des ports d'un serveur
 *
 * @param envoyer envoi de la requête pour le client au nième domaine
 * @param adresse adresse ip du serveur avec lequel on souhaite communiquer
 * @param port port de serveur avec lequel on souhaite se communiquer
 * @result renvois la copie des ports serveur
 */
 struct servers_info communication_serveur(char* envoyer,char* adresse ,int port)
 {
 	char * add;
 	char buf[TAILLE];
 	add = adresse;

    strcpy(buf,envoyer);

    struct servers_info servers;
 	servers.nbr_servers = TAILLE;
 	servers.IP = malloc(sizeof(char *)*(servers.nbr_servers + 1));
    servers.PORT = malloc(sizeof(int)*(servers.nbr_servers + 1));
    int i = 0;
    while (i <= servers.nbr_servers) {
 		servers.IP[i] = malloc(sizeof(char)*300);
 		i++;
 	}
 	servers.nbr_servers = 0;

 	struct timeval timeOut;
    timeOut.tv_sec = 5;
    timeOut.tv_usec = 0;

    int type = ip_type(adresse);

 	if (type == 4) {

        int sockfd;
        socklen_t addrlen;
        struct sockaddr_in client;
        struct sockaddr_in dest;

        add = "127.0.0.1";

        CHECK((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) != -1);

        dest.sin_family = AF_INET;
        dest.sin_port   = htons(port);
        addrlen         = sizeof(struct sockaddr_in);

        CHECK((inet_pton(AF_INET,add,&dest.sin_addr)) == 1);

        addrlen = sizeof(struct sockaddr_in);
        fd_set ensemble;
        int nbtime = 0; //nombre de timeout
        int ret = 0; //valeur de retour du select

        FD_ZERO(&ensemble);
        FD_SET(sockfd, &ensemble);

        while (nbtime <= 3) {

            if (sendto(sockfd,buf,sizeof(buf),0,(struct sockaddr *) &dest,addrlen) == -1) {
            	perror("sendto");
            	close(sockfd);
            	exit(EXIT_FAILURE);
            }

            if ((ret = select(sockfd+1, &ensemble, NULL, NULL, &timeOut)) == -1) {
                perror("select");
                close(sockfd);
                exit(EXIT_FAILURE);
            }

            if (ret == 0) {
                nbtime++;
            }
 			else if (FD_ISSET(sockfd,&ensemble)) {
     		    if (recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr *) &client, &addrlen) == -1) {
    				perror("recvfrom");
    				close(sockfd);
    				exit(EXIT_FAILURE);
    			}

    			printf("%s \n", buf);

    			newadd(servers,buf);
    			servers.nbr_servers = compterserv(servers.PORT);

    		    CHECK(close(sockfd) != -1);
    			nbtime = 10;
 			}
 		}
 		if (nbtime == 4) {
 			servers.PORT[0] = 0;
 		}
 	}

 	if(type == 6) {
        int sockfd;
 		socklen_t addrlen;
 		struct sockaddr_in6 client;
 		struct sockaddr_in6 dest;

 		printf("%s et %d \n",add,port);

 		add = "::1";

 		CHECK((sockfd = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP)) != -1);

        dest.sin6_family = AF_INET6;
        dest.sin6_port   = htons(port);
        addrlen         = sizeof(struct sockaddr_in6);

 		CHECK((inet_pton(AF_INET6,add,&dest.sin6_addr)) == 1);

 		addrlen = sizeof(struct sockaddr_in6);

        fd_set ensemble;
        int nbtime = 0;
        int ret = 0;

 		FD_ZERO(&ensemble);
 		FD_SET(sockfd, &ensemble);

 		while (nbtime <= 3) {
 			if (sendto(sockfd,buf,sizeof(buf),0,(struct sockaddr *) &dest,addrlen) == -1) {
	            perror("sendto");
 				close(sockfd);
                exit(EXIT_FAILURE);
 			}
 			if ((ret = select(sockfd+1, &ensemble, NULL, NULL, &timeOut)) == -1) {
                perror("select");
 				close(sockfd);
 				exit(EXIT_FAILURE);
 			}
 			if (ret == 0) {
                nbtime++;
            } else if (FD_ISSET(sockfd,&ensemble)) {
 				if (recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr  *) &client, &addrlen) == -1)
                {
            		perror("recvfrom");
            		close(sockfd);
            		exit(EXIT_FAILURE);
                }

                printf("%s \n", buf);

                newadd(servers,buf);
                servers.nbr_servers = compterserv(servers.PORT);

                CHECK(close(sockfd) != -1);
                nbtime = 10;
     		}
 	   }
	   if(nbtime == 4) {
           servers.PORT[0] = 0;
	   }
 	}
 	return servers;
 }



/**
 * @brief ajoute les requêtes données par l'utilisateur dans l'entrée standard
 *
 * @param req ensemble de requête auquel on va ajouter la requête lreq
 * @param lreq requêtes donnée par l'utilisateur dans l'entrée standard
 * @result req l'ensemble des requêtes
 */

struct request ajouterreq(struct request req, char* lreq) {

	char* cs = NULL;
	char* sep = "|";

	cs = strtok(lreq,sep);
	int i = req.nb_commandes;
    printf("Teste %s\n",cs);
	strcpy(req.commandes[i],cs);
    req.nb_commandes++;
    i++;

	return req;
}


/**
 * @brief compare deux chaînes de cractères, donne 0 si elles sont identiques 1 sinon
 *
 * @param a chaîne à comparer
 * @param b chaîne à comparer
 * @result 0 ou 1
 */

int memechaine(char* a, char* b)
{

	int i;
	int size = strlen(a);

	if (strcmp(a,b) == 0) {
        for (i = 0; i < size-1; i++) {
			if(a[i] != b[i])
				return 1;
		}
		return 0;
	}
	return 1;
}


/**
 * @brief donne l'indice du serveur dans le tableau d'ensemble de serveur si l'adresse peut être utiliser sinon -1
 *
 * @param ens ensemble de serveur à tester
 * @param add adresse ip à tester
 * @param port port à tester
 * @param dom domaine à tester
 * @result return l'indice du serveur dans l'ensemble ou -1
 */

 int estUtilisable(struct ensemble_serv ens, char* add, int port, char* dom)
 {
 	int i;
 	for (i = 0; i < ens.serveurs.nbr_servers; i++) {

 		if ((memechaine(ens.serveurs.IP[i],add)==0) && (ens.serveurs.PORT[i] == port) && ((memechaine(ens.domaine[i],dom)) == 0)) {
 			if (ens.utilisable[i] < 0)
 					return -1;

 			if (ens.utilisable[i] == 1)
 					return -1;

 			if (ens.utilisable[i] == 0) {
 					ens.utilisable[i] = 1;
 					return i;
 			}
 		}
 	}
 	strcpy(ens.domaine[i],dom);
 	ens.serveurs.IP[i] = add;
 	ens.utilisable[i] = 1;
 	ens.serveurs.PORT[i] = port;

 	return i;
}


/**
 * @brief remet à zero toute les valeurs à utilisé et incrémente le compteur des serveurs problématiques(performance,timeout)
 *
 * @param ens ensemble de serveur à tester
 * @param dom domaine à tester
 * @result renvois la structure modifié
 */

struct ensemble_serv MiseZero(struct ensemble_serv ens, char* dom)
{
	int i;

	for (i = 0; i < ens.serveurs.nbr_servers; i++) {
		if (memechaine(ens.domaine[i],dom) == 0) {
			if (ens.utilisable[i] < 0)
				ens.utilisable[i] = ens.utilisable[i]+1;
		  	else
				ens.utilisable[i] = 0;
		}
	}
	return ens;
}


/**
 * @brief renvois le domaine actuellement recherché dans l'étape
 *
 * @param ens ensemble de serveur à tester
 * @param dom domaine à tester
 * @result renvois la structure modifié
 */

char* domaineActuel(char* requete, int nbetape) {

	char* copy;
	char sep2 = '.';
	char buf[SIZE2];

	strcpy(buf,requete);
	int nbdom = compdom(requete)-nbetape;

	if (nbdom == 0)
		return requete;

	int i;

	for (i = 0; i < nbdom; i++) {
		if (i == 0) {
			copy = strpbrk(buf,&sep2);
		} else {
			copy = strpbrk(copy+1,&sep2);
		}
	}

	if (nbdom == compdom(requete) - 1)
		  return  copy;
	else
		 return copy+1;

}

int main(int argc, char **argv)
{
    if (argc != 3) {
		fprintf(stderr, "Usage: %s fichier_liste_serveurs fichier_liste_requête\n",argv[0]);
		return EXIT_FAILURE;
	};

    struct request requete;
    struct servers_info servers;
    struct ensemble_serv ens;

    servers = open_load_config_file(servers,argv[1]);
    requete =  load_commands_from_file(argv[2]);
    ens = initensemble();

    char lreq[SIZE];
	char* result;
	int nbdomaine = 0;
	fprintf(stdout,"Rajouter une commande si vous le souhaitez de la forme 'www.example.fr' \nsinon retour chariot\n");
	fgets(lreq,SIZE,stdin);

	if(strlen(lreq) != 1)
		requete = ajouterreq(requete,lreq);

	time_t t;
	int nbetp;
    t = time(NULL);
    int horloge = (int)t;
    struct servers_info serv1;
    char* domaine;

    char** add; //stocker les adresses pour une requête
    int* port; //stocker addresse de port
    int nbserv; //nombre de serveur pour le parcours
    int i = 0;
    int indServ;
    int indEns;
    int bcharge;
    int nbtestreq;
    int performance;

    for (i = 0; i < requete.nb_commandes;i++) {
        strcpy(lreq,requete.commandes[i]);
        nbetp = 1;
        indServ = 0;
        indEns = -1;
        nbtestreq = 0;

        add = copieadd(servers);
        port = copieport(servers);
        nbserv = servers.nbr_servers;
        nbdomaine = compdom(lreq);

		while (nbetp <= nbdomaine && nbtestreq < nbserv) {
		    domaine = domaineActuel(lreq,nbetp);

			for (bcharge = indServ; (indEns < 0) && (bcharge < nbserv); bcharge++) {
	            indEns = estUtilisable(ens,add[bcharge],port[bcharge],domaine);

			  	if (indEns == ens.serveurs.nbr_servers)
					ens.serveurs.nbr_servers++;
			}

			if (bcharge == nbserv && indEns == -1) {
				ens = MiseZero(ens,domaine);
    			indServ = 0;
				for (bcharge = indServ; (indEns < 0) && (bcharge < nbserv); bcharge++) {
				  	indEns = estUtilisable(ens,add[bcharge],port[bcharge],domaine);
				}
			}

			indServ = bcharge - 1;
			t = time(NULL);
			horloge = (int)t;

			result = crea_result(result,nbetp,lreq, horloge);
			serv1 = communication_serveur(result,add[indServ],port[indServ]);

			if (serv1.PORT[0] == 0) {
					indServ = indServ+1;
					ens.utilisable[indEns] = -nbserv;
					nbtestreq = nbtestreq +1;

			} else {
				// ICI free_fields(add);
                free(add);
				free(port);
				add = copieadd(serv1);
				port = copieport(serv1);
				nbserv = serv1.nbr_servers;
				nbetp++;
				indServ = 0;
				nbtestreq = 0;
				performance = (int)time(NULL)- horloge;

				if (performance >= 2 )
					ens.utilisable[indEns] = -nbserv;

                free(serv1.IP);
				free(serv1.PORT);
			}
			indEns = -1;
		}
		printf("Fin requete \n");

	}

    // ICI
    // free_fields(requete.commandes);
    // free_fields(servers.IP);
    // free(servers.PORT);
    // free_fields(ens.domaine);
    // free_fields(ens.serveurs.IP);
    // free(ens.serveurs.PORT);
    // free(ens.utilisable);
    // free(ens.etape);
	// free(result);
    // free_fields(serv1.IP);
    // free(serv1.PORT);
    // free(domaine);
    // free_fields(add);
	// free(port);

    free(ens.utilisable);
    free(ens.serveurs.IP);
    free(ens.serveurs.PORT);
    free(ens.domaine);
    free(ens.etape);

    free(add);
    free(port);
    free(servers.IP);
    free(servers.PORT);

    return 0;
}
