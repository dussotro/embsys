## TP1 : Utilisation de simulateur GPS

**Question 1**: Qu'est ce qu'un Makefile? À quoi sert make?

Un Makefile est un fichier permettant l'utilisation des commandes de compilation 

"make". Cela permet de fixer les paramètres de compilation dans des mots clefs définis.
make permet donc de lancer des commandes bash paramétrer.

**Question 2**: Quel compilateur est utilisé ici?

gcc est un compilateur 

**Question 3**: Qu'est ce qu'une librairie partagée?

Une librairie partagée est une librairie que des programmes puissent utilisés sans copier le contenu de la librairie.

**Question 4**:
Affiche hello_world

---------------------

#include <stdio.h>
int main()
{
   // printf() displays the string inside quotation
   printf("Hello, World!");
   return 0;
}

---------------------

#Commande terminal
gcc -g -Wall hello_world.c -o hello_world.o 

**Question 5**

Sortie de Makefile de labs/sysprog/gps/
---------------------

GCC = gcc
BIN = gps
TOP = ../..

all:
	$(GCC) $(BIN).c -g -o $(BIN) -I $(TOP)/include -L $(TOP)/lib/ -lptmx -lnmea
	mv $(BIN) $(TOP)/bin

---------------------
-lptmx et -lnmea font le lien entre le programme et les librairies partagées