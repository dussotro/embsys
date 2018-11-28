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

