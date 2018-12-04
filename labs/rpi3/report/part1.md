## Partie 1 : Construction d'OS avec Buildroot et chaine de cross-compilation

### Decouverte de Buildroot

*Question 1* : 

Fichier *config/embsys_defconfig*:

	Utilité : configurer le kernel

Fichier *busybox.config*

	Utilité : configure la busybox (qui permet elle-même de gagner de l'espace mémoire grâce à des liens symboliques)

Fichier *user.table* 

        Syntaxe : configure les droits des différents utilisateurs


                # <option> is not set # non pris en compte
                <option>=""           # indiquer une valeur spécifique
                <option>=0            # indiquer une durée
                <option>=y            # yes donc compilé obligatoirement
                <option>=m            # module donc compilé si spécifiquement de$
 


*Question 2* : 
Il faut utiliser le fichier de configuration *Config.in.arm* dans *arch/*. 
La raspberryPi3 a un processeur ARM.

*Question 3* :
On peut trouver des librairies ajustables selon l'OS qui va être utilisé.

*Question 4* :
.patch   : fichier permettant de choisir un patch selon les besoins, donc selon l'OS Cible
.mk      : Automatique, fichier make pour compiler
.in      : Automatique, fichier de configuration
S50sshd  : script de contrôle su service sshd
.hash    : clé permettant de vérifier l'intégrité de nos fichiers après téléchargements, décompression, etc.
.service : permet de lancer un service daemon 

*Question 5* :
Permet de configurer la version de la raspberry grace à des fichiers executable bash


*Question 6* : 

````
# make embsys_defconfig
````
````
# Override the BR2_DEFCONFIG from COMMON_CONFIG_ENV with the new defconfig
%_defconfig: $(BUILD_DIR)/buildroot-config/conf $(1)/configs/%_defconfig prepare-kconfig
	@$$(COMMON_CONFIG_ENV) BR2_DEFCONFIG=$(1)/configs/$$@ \
		$$< --defconfig=$(1)/configs/$$@ $$(CONFIG_CONFIG_IN)
endef
````

Permet de réécrire BR2_DEFCONFIG avec une configuration choisie.


*Question 7* : 
menuconfig: $(BUILD_DIR)/buildroot-config/mconf prepare-kconfig
	@$(COMMON_CONFIG_ENV) $< $(CONFIG_CONFIG_IN)

architecture cible : ARM (little endian)
cpu cible          : cortex A53
ABI                : EABIhf (décrit une interface de bas niveau entre les applications et l'OS)
	(c'est l'ABI qui définit le rôle précis des registres généraux et la reponsabilité de leur intégrité)
	(hf pour Hard Float, encodage des flottants qui permet de une bonne vitesse de fonctionnement)
lib C utils        : uClibc-ng
version Cross-comp : ELF
version Kernel     : 4.9

*Question 8* :

 *From the buildroot*
````
Symbol: BR2_PACKAGE_OPENSSH [=y]
````
OpenSSH sera compilé et disponible (=y)

 *From the config file*
Avec la recherche "/" sous vim
````
BR2_PACKAGE_OPENSSH=y
````

*Question 9* :
Busybox est un moyen de regrouper plusieurs centaines de fichiers executables sous un même fichier avec des liens symboliques 
pour un gain de places et de performances de l'OS.
Avec *make busybox-menuconfig*
On obtient un menu de configuration qui nous permet de gérer les paquets qui seront installé grâce à busybox.

### Chaîne de cross-compilation

*Question 10* :
arm-buildroot-linux-uclibcgnueabihf  doc  include  libexec  sbin   usr
bin				     etc  lib	   man	    share

	Cela ressemble à la racine de l'image du kernel.
arm-linux-gcc est le binaire du compilateur.
(On trouve aussi arm-buildroot-...-gcc)

arm-linux-gcc est un binaire qui permet de pointer sur toolchain-wrapper qui semble contenir beaucoup de fonctions selon comment on l'appelle.

*Question 11* :

On crée le fichier *helloworld.c*
On compile *gcc helloworld.c -o hw.o*.

#file hw.o 
hw.o: ELF 64-bit LSB shared object, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, for GNU/Linux 2.6.32, BuildID[sha1]=49120e7a1c1e801b0bad6acd4d22e284e98f5d46, not stripped
Le binaire est formaté pour l'architecture locale et non celle cible
Si l'on execute, on affiche comme demandé "Hello World !".
````
# ./hw.o
Hello World !
````
Si l'on compare avec l'autre
````
# file hw
hw: ELF 32-bit LSB executable, ARM, EABI5 version 1 (SYSV), dynamically linked, interpreter /lib/ld-uClibc.so.0, not stripped
````
On a un fichier sous un format différent, compilé pour un compilateur qui n'est pas notre ordinateur, le fichier compilé est compatible raspberry

````
# ./hw
bash: ./hw: cannot execute binary file: Exec format error
````
Ici le processeur de mon ordinateur ne peut executer le binaire.

### Images

*Question 13* :
````
# ls output/images/
bcm2710-rpi-3-b.dtb  boot.vfat	  rootfs.ext4  rpi-firmware  zImage
bcm2710-rpi-cm3.dtb  rootfs.ext2  rootfs.tar   sdcard.img
````

Si l'on regarde dans le fichier genimage_raspberry.cfg
On voit que ce fichier permet de créer une partie bootable boot.vfat et une partie stockage de données en ext4.

le fichier rootfs.tar est un fichier compressé contenant les données à mettre dans rootfs.vfat.
zImage est le binaire du Kernel Linux
sdcard.img est l'image de la carte sd


*Question 14* :

````
# file zImage 
zImage: Linux kernel ARM boot executable zImage (little-endian)
````

Décrit le contenu du fichier et son format, on voit donc qu'il s'agit du kernel compilé.

````
# file sdcard.img  
sdcard.img: DOS/MBR boot sector; partition 1 : ID=0xc, active, start-CHS (0x0,0,2), end-CHS (0x4,20,17), startsector 1, 65536 sectors; partition 2 : ID=0x83, start-CHS (0x4,20,18), end-CHS (0x1d,146,54), startsector 65537, 409600 sectors
root@25191e8b026b:~/buildroot-precom
````
On boit donc les différentes partitions de l'image disque avec la mémoire déstiné à chaque partition

*Question 15* :

````
# ls /tmp/rootfs/
bin  dev  etc  home  lib  lib32  linuxrc  media  mnt  opt  proc  root  run  sbin  sys  tmp  usr  var
````

C'est la racine de l'image que nous sommes en train de paramétrer, similaire à une image linux.
