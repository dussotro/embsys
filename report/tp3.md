## Exercice 1 : Multiplexage

**Question 1** : Selon vous, à quoi correspond le champs indiqué par
                *PTTY*?


Ouverture du port 19
````
PTTY: /dev/pts/19
````

**Question 2** : En regardant le code de *reader.c*, y a-t-il quelque chose qui
                 vous chagrine?

Il n'y a pas de gestionnaire de signaux.

**Question 3** : Grâce à des recherches Internet (ou en fouinant dans le code
                 du simulateur), déterminez dans quelle trame et dans quel champs
                 l'heure est définie.

````
    // date
    char date_str[NMEA_DATE_SIZE];
    current_date_str(date_str);
    memcpy(&gll->frame[28], date_str, NMEA_DATE_SIZE);
````

La date est définie dans la trame GLL au champ 28.
````
$GPGLL,4836.60,N,00741.00,E,102007,A
````
Dans cette trame, il est 10h20m07s.

**Question 4** : Quelles fonctions sont utilisées dans *reader.c* pour
                 ouvrir/écouter/lire/fermer le port virtuel du simulateur?
                 Comment s'appelle ce type de programmation?


````
    // Ouverture du port virtuel donne en argument
    int fd = open(port, O_RDWR | O_NOCTTY);

    //Dans la boucle while(1)
    FD_ZERO(&fdset); 
    FD_SET(fd, &fdset);
    //attente passive
    select(fd+1, &fdset, NULL, NULL, NULL); //lire l'ensemble des file descriptors

    if (FD_ISSET(fd, &fdset))
    {
        int bytes = read (fd, buff, sizeof(buff)); //lecture de fd

        if (bytes > 0)
        {
            printf("%s\n", buff); //Affichage du buffer
            fflush(stdout);
        }
    }

````

On est dans un cas d'attente passive avec la fonction select.

**Question 5** : Modifiez le code de *reader.c* afin qu'il puisse écouter les
                 trames provenant de deux simulateurs GPS différents (ports
                 paramétrables au lancement). Vérifiez le bon fonctionnement en
                 lançant deux instances du simulateur GPS.

**Question 6** : Utilisez *syslog* pour afficher l'heure dans la console ainsi
                 que le PID du père.

On utilise pour ouvrir le processus de log:

````
    //ouverture du processus de log
    // LOG_PID  affiche le pid du programme qui appelle syslog
    // LOG_CONS affichage console
    // LOG_CRON affichage de l'heure
    openlog("--->", LOG_PID|LOG_CONS, LOG_CRON);

    //appel de la fonction d'affichage/écriture du log 
    syslog(LOG_NOTICE,"%s\n", buff);
    syslog(LOG_NOTICE,"%s\n", buff);
    
    //fermeture du log
    closelog();
````

## Exercice 2 : Mémoire partagée et sémaphore

**Question 1** : Selon vous, à quoi correspond le paramètre *myshm* indiquée via
                 l'option *-s* de *shm_writer*? Et *lock*?


*-s* pour share memory
*-l* pour le sémaphore

*myshm* nom de la mémoire partagée qui va être utilisée
*lock* est le nom du sémaphore

**Question 2** : Où peut-on trouver la représentation du segment de mémoire
                 partagée sur le système de fichiers?

On peut retrouver ce segment de mémoire partagée dans /dev/shm. On y retrouve donc *myshm* et *sem.lock*
Si l'on met la commane :
````
./shm_writer -p /dev/pts/19 -s myshm86 -l lock4
````
On retrouve les fichiers *myshm86* et *sem.lock4*

**Question 3** : Faites un schéma bloc des différents éléments mis en jeu.

````

------------------------------------------------
|                                              |
|                                              |
|                                              |
|                                              |
|           Shared Memory SHM                  |
|                                              |
|                                              |
|                                              |
------------------------------------------------
    ^                              ^ 
    |                              |
    |                              |
    |                              |
    | Création /w                  | attachement
    | attachement                  | 
    |                              | Lecture /
    | Lecture /                    | Ecriture
    | Ecriture                     |
    |                              |    
    | Création Clef                | Besoin Clef   
    |                              |    
    |                              |    
    |                              |    
    |                              |    
    |                              |    
    v                              v 
--------------              --------------
|            |              |            |
|  Processus |              | Processus  |
|      1     |              |     2      |
|            |              |            |
--------------              --------------

````


**Question 4** : En étudiant la fonction *hndopen* implémentée dans le fichier
                 *handler.c*, décrivez les fonctions utilisées pour gérer le
                 segment de mémoire partagée.

````
handlers->shmfd = shm_open(opts.shm, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
````
Permet d'ouvrir le segment spécifié en lecture/écriture *O_RDWR|O_CREAT* modifiable par le propiétaire de la mémoire *S_IRUSR|S_IWUSR*.

````
ftruncate(handlers->shmfd, sizeof(handlers->shdata)) != 0)
````
Fixe la taille de la mémoire partagée.

````
handlers->shdata = mmap(NULL, sizeof(handlers->shdata), PROT_READ|PROT_WRITE, MAP_SHARED, handlers->shmfd, 0);

````
Permet de segmenter la mémoire.


**Question 5** : Quelle fonction utilise le paramètre *myshm* passé en ligne de
                 commande?

Dans *util.c*
```` 
while((option = getopt(argc, argv, options)) != -1)
````
Dans *handler.c*
```` 
handlers->shm = opts.shm;
````
On utilise la fonction getopt pour retrouver les différents paramètres précisés. Ceux-ci sont mis dans une structure. Enfin, la structure OPTS permet d'initialiser le nom du segment. C'est pour cela que l'on trouve un fichier appelé *myshm* dans */dev/shm/* .

**Question 6** : Quel flag en particulier indique une *création* de segment
                 et pas seulement une ouveture en lecture/écriture?

*O_CREAT* permet la création en cas de non-existence.


















