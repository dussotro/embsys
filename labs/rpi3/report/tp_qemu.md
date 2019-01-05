**Question 1**: À quoi sert l'option *--cap-add* lors du lancement d'un nouveau coneneur Docker?

De base Docker n'a pas les privilèges administrateurs. La commande '--cap--add' permet de donner les droits Linux à docker pour qu'il puisse lancer une commande spécifique ou encore accepder à des dossiers nécéssitants les droits administrateurs.

**Question 2**: À quoi sert la commande *chroot*?

La commande chroot permet de changer le répertoire racine vers un nouvel emplacement.

Cette opération peut être utilisée dans divers cas :

   * prison : empêche un utilisateur ou un programme de remonter dans l'arborescence et le cantonne à une nouvelle arborescence restreinte.
   * changement d'environnement : permet de basculer vers un autre système linux (autre architecture, autre distribution, autre version).

Dans notre cas, nous voulons changer d'environement pour pouvoir aller dans un environement avec un processeur type "Arm" pour pouvoir executer notre binaire qui a été compilé pour ce type de processeur.

**Question 3**: Que se passe-t-il? Pourquoi?

Nous pouvons voir que le binaire s'exécute corretement, car grâce à la commande *chroot*, nous avons pu basculer vers un système de raspberry pi et le fichier était compilé pour ce type de processeur.
