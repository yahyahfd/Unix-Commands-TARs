# Commandes Unix: Réimplémentation pour des TARs

Ce projet doit être amélioré et mis à jour.

**Paquets à installer**

* sudo apt update
* git: sudo apt install git
* build essential package : sudo apt-get install build-essential
* libreadline package : sudo apt-get install libreadline-dev

**Lancement du Shell**

Pour lancer le shell il suffit de se déplacer vers le dossier contenant le makefile et l'ensembles des fichiers, taper make puis ./tsh

**Les fonctionnalités implémentées :**

*  cd
*  exit
*  cat
*  cp et cp -r
*  rmdir
*  rm et rm -r
*  ls et ls -l
*  pwd
*  les redirections de sortie (y compris sur des fichiers d'un tarball)
*  toutes les commandes externes fonctionnent normalement si leur déroulement n'implique pas l'utilisation d'un fichier dans un tarball
