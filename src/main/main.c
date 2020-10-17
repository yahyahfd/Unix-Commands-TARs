#include <string.h>
#include <stdlib.h>

int isTar (char c[]){
   int i = strlen (c) - 3;
   if ((c[i] == 't') && (c[i + 1] == 'a') && (c[i + 2] == 'r')){
      return 1;
   }
   return 0;
}

int main (int argc, char *argv[]){
    if (isTar (argv[1]) != 1){
       //ajouter ligne pour gerer erreur en cas d'extension autre que .tar
       exit (EXIT_FAILURE);
    }
   return 0;
}
