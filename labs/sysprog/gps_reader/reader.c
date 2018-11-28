#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <syslog.h>

#include <util.h>

//-----------------------------------------------------------------------------
int main(int argc, char *argv [])
{

    openlog("--->", LOG_PID|LOG_CONS, LOG_CRON);

    char * port = NULL;
    char * portbis = NULL;
    int nb_fd;
    // parse comand line
    if ((argc != 3) && (argc!=5))
    {
        fprintf(stderr, "Invalid usage: \nreader -p port_name\nreader -p port_name -q port_name\n");
        exit(EXIT_FAILURE);
    }
    if (argc==3) { nb_fd = 1; }
    if (argc==5) { nb_fd = 2; }

    fprintf(stderr, "Nombre de ports ouverts : %d\n", nb_fd);

    char * options = "p:q:";
    int option;

    while((option = getopt(argc, argv, options)) != -1)
    {
        switch(option)
        {
            case 'p':
                port = optarg;
                printf("port open %s\n", port);
                break;

            case 'q':
                portbis = optarg;
                printf("port open %s\n", portbis);
                break;

            case '?':
                printf(stderr, "Invalid option %c\n", optopt);
                exit(EXIT_FAILURE);
        }
    }

    // open serial port
    int fd = open(port, O_RDWR | O_NOCTTY);
    int fdbis=0;
    if (argc>2) {
        int fdbis = open(portbis, O_RDWR | O_NOCTTY);
    }
    //if (nb_fd == 2) {
    //    int fdbis = open(portbis, O_RDWR | O_NOCTTY);
    //}

    int fd_lim;

    if (fd > fdbis){
        fd_lim = fd+1;
    }
    else {
        fd_lim = fdbis+1;
    }

    if ((fd == -1) || (fdbis == -1))
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    tcflush(fd   , TCIOFLUSH);
    tcflush(fdbis, TCIOFLUSH);

    // read port
    char buff[50];
    fd_set fdset;

    while(1)
    {
        bzero(buff, sizeof(buff));

        FD_ZERO(&fdset);
        FD_SET(fd, &fdset);
        FD_SET(fdbis, &fdset);
        
        select(fd_lim, &fdset, NULL, NULL, NULL);

        if (FD_ISSET(fd, &fdset))
        {
            int bytes = read (fd, buff, sizeof(buff));
            if (bytes > 0)
            {
                fprintf(stderr, "GPS1\n");
                syslog(LOG_NOTICE,"%s\n", buff);
                fflush(stdout);
            }
        }
        if (FD_ISSET(fdbis, &fdset))
        {
            int bytes = read (fdbis, buff, sizeof(buff));
            if (bytes > 0)
            {
                fprintf(stderr, "GPS2\n");
                syslog(LOG_NOTICE,"%s\n", buff);
                fflush(stdout);
            }
        }
    }

    // close serial port
    close(fd);
    if (nb_fd == 2) {
        close(fdbis);
    }

    closelog();
    exit(EXIT_SUCCESS);
}
