# Partie 2: Compilation, debug et gestionnaire de signaux

## Exercice 1 : GDB et fichier core

**Question 1** : Que se passe-t-il au bout de quelques secondes?

````
PTTY: /dev/pts/19
Segmentation fault (core dumped)
````

Segmentation fault (core dumped) renvoie à un problème de mémoire.

**Question 2** : Quel signal a reçu le processus pour se terminer ainsi? Comment vérifiez vous le numéro du signal reçu?

````
$ sh run.sh
PTTY: /dev/pts/19
Segmentation fault (core dumped)
$ echo $?  
139
$ kill -l 139
SEGV
````
On a reçu le signal SEGV.

**Question 3** : Grâce à GDB et au fichier *core* généré, analysez la source du
                 problème du binaire *gps*. Quelle partie du code est fausse?
                 Pourquoi?

````
(gdb) bt
#0  strlen () at ../sysdeps/x86_64/strlen.S:106
#1  0x00007ffff767769c in _IO_puts (str=0x0) at ioputs.c:35
#2  0x00007ffff79d2b41 in knot_to_kmh_str (not=5.51000023, size=6, format=0x7ffff79d300f "%05.1f", 
    kmh_str=0x7fffffffdb50 "010.2") at nmea.c:23
#3  0x00007ffff79d2f98 in nmea_vtg (vtg=0x7fffffffdb90) at nmea.c:178
#4  0x0000000000400bc4 in write_vtg (fd=3) at gps.c:40
#5  0x0000000000400e3c in main () at gps.c:109
````
Après analyse, l'erreur serait sur la fonction strlen()

En effet, on fonctionne avec un pile d'appel, il faut donc regarder l'étape #0 pour savoir laquelle a été lancé en dernier. C'est donc pour cela que la fonction qui ne marche pas serait strlen().


**Question 4** : Que se passe-t-il quand vous lancez GDB en mode interactif sur le binaire *gps*?

On peut voir toutes les étapes une à une du programme, gdb essaye d'aller chercher des problèmes supplémentaires. 

RQ : 
sh permet d'executer un script bash sans conserver les variables d'environnement
. permet de les garder dans le terminal après execution

**Question 5** : À quoi sert la commande *ldd*? Quelle information supplémentaire cela vous apporte-t-il?

$ ldd bin/gps
	linux-vdso.so.1 =>  (0x00007ffc54961000)
	libptmx.so => not found
	libnmea.so => not found
	libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f0a0f1d7000)
	/lib64/ld-linux-x86-64.so.2 (0x00007f0a0f5a1000)

Grâce à ldd, on peut le problème lié aux librairies, ici le programme ne les trouve pas.

**Question 6** : Comment résoudre ce problème en tant qu'utilisateur? N'hésitez
                 pas à regarder le fichier *gps/run.sh*.

Ici, plusieurs solutions fonctionnent:
 - ajouter les librairies sous *~/local*
  par exemple et la commande 

````
export LD_LIBRARY_PATH=~/local/
````
 - faire l'export à la main dans le terminal utilisé
 - utiliser . ./run.sh pour appliquer run.sh sur le terminal en cours d'exécution

**Question 7** : Quelle est la différence entre les commandes *s* et *n* dans
                 le prompt gdb suite à un breakpoint?

"s" permet d'aller à la ligne suivante alors que "n" permet d'aller au prochain breakpoint.

 **Question 8** : Dans quel contexte ce type d'outils peut être intéressant?

Utilisation sur un serveur distant, comme en système embarqué ou l'on peut débloquer sur l'ordinateur avec de la puissance de calcul.

## Exercice 2 : LD_PRELOAD et sigaction

