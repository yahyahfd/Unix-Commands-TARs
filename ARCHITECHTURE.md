ARCHITECHTURE


Notre dossier de projet se divise en deux dossiers principaux accompagnés d'un Makefile, le premier dossier contenant le code, et le second les fichiers sur lesquels sont fait les tests, comme les noms l'indiquent. Le répertoire "Code" contient à son tour 2 dossiers ainsi que le shell "tsh.c". Nous nous intéresseront donc à ces 2 sous-dossiers situés dans "Code".
Nous avons choisi une architecture qui nous permet dans un premier temps de lancer chaque commande indépendamment du shell, pour faire les tests nécéssaires (etc...). Ces commandes sont présentes dans le dossier "C files" sous la forme de fichiers ".c " accompagnés d'un makefile.
Le fichier "headers" quant à lui contient une version ".h "des mêmes commandes qui seront utilisable directement à travers le shell "tsh.c".


STRUCTURES DE DONNÉES


struct posix_header: Cette structure est présente dans presque toutes les commandes, et nous a servi à manipuler les archives tar et leur contenu.
struct group: Cette structure a été utilisé dans la commande cp et a servi à renvoyer des informations sur un groupe.
struct stat: Cette structure est renvoyé par la méthode stat et permet d'obtenir l'état d'un fichier, notamment la taille, l'utilisateur/groupe d'utilisateurs, le numéro d'i-noeud, etc...
struct dirent: Cette structure est renvoyée par la méthode readdir(), et contient le nom du fichier, son type, son numéro d'i-noeud, etc...
struct tm: Cette structure a été utilisée dans la commande ls afin de renvoyer une date correcte pour les fichiers dans un tarballs, concernant la date de dernière modification ou de création. Cette structure permet de manipuler une date de manière plus simple.
struct passwd: Cette structure contient les identifiants d'un utilisateur.


ALGORITHMES IMPLÉMENTÉS


Pour la majorité des commandes, nous avons utilisé des fonctions communes que l'on retrouve dans "headers/functions.h", comme par exemple la fonction "split" qui permet de séparer la partie tar et la partie dossier dans le path d'un tarball, la fonction "cmp" qui nous permet de comparer des chaines de caractères, la fonction "isTar" utilisé bien que très rarement afin de déterminer si un path donné n'est qu'un fichier tar, ou encore la fonction "samepath" qui sert à éviter toute confusion pour les fichiers tar et signaler qu'un répertoire "r1" et "r1/" sont identiques.
Dans le dossier headers, chaque version ".h" des commandes contient une fonction sous la forme commandname_tsh qui sera appelé dans le main de "tsh.c" et regroupe donc la totalité des fonctions de la commande "commandname". Comme souligné précédemment, une version des commandes destinée aux tests est présente sous la forme de fichiers c dans le répertoire "C files".
