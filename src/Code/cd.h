#define MAX_LEN 250

char *tar_name(char *filepath){
	int nb=0;char *lus;
	char *tmp=strdup(split(filepath,1));
	char *tmp2=strdup(split(filepath,1));
	lus=strtok(tmp,"/");
	if(lus==NULL){
		nb=0;
	}
	else{
		nb++;
		while( strtok(NULL,"/")!=NULL){
			nb++;
			}
		}
	if(nb==1){
		return filepath;
		}
	char *name=strtok(tmp2,"/");
	for(int i=0;i<nb-1;i++){
		name=strtok(NULL,"/");

		}
		return name;
	}
/**retourne le chemin avant le tar**/
char *before_tar(char *filepath){
	int j=0;
	char *res=strdup(split(filepath,1));
	for(int i=0;i<strlen(res)-2;i++){
		if(filepath[i]=='.' && filepath[i+1]=='t' && filepath[i+2]=='a' &&filepath[i+3]=='r' ){
				j=i;
			}
				}
	for(j;j>0;j--){
		if(res[j]=='/'){
			res[j]='\0';
			return res;
			}
	}
	return res;
	}
/***retourne dans le dossier d'avant**/	
char *path_behind(char *filepath){
	char *res=strdup(filepath);
	for(int i=strlen(res)-2;i>0;i--){
		if(res[i]=='/'){
			res[i+1]='\0';
			return res;
			}
	}
	return filepath;
}	
/********************************/	
char *cd(char *filepath,char *wd){
	char *pwd;
	struct stat st;
	struct stat *buf = &st;
	struct posix_header p;
	const char* env = getenv("HOME");
	char tmp[MAX_LEN];

	//si on fait cd .. pour retourner en dossier pere
	if(cmp(filepath,"..")==0){
		pwd=strdup(cd(path_behind(wd),path_behind(wd)));
		return pwd;
				}
				
	if(cmp(filepath,"/")==0){
		chdir("/");
		pwd=strdup("/");
		return pwd;
				}
	//si le chemin ne contient pas un tar 
	if(cmp(split(filepath,1),"echec")==0){
		//si on est pas deja dans un tar
		if(cmp(split(wd,1),"echec")==0){
			if(chdir(filepath)==-1){
				//le dossier n'existe pas
				if(stat(filepath,buf)<0){
					write(1,"cd : No such file or directory1\n",strlen("cd : No such file or directory1\n"));
					return wd;
				}
				//le fichier n'est pas un dossier
				else{
					
					if((buf->st_mode & S_IFMT)!=S_IFDIR){
						write(1,"cd : Not a directory2\n",strlen("cd : Not a directory2\n"));
						return wd;
							}
					}
			}		
			getcwd(tmp,MAX_LEN);
			pwd=strdup(tmp);	
			return pwd;	
		}
		if(cmp(split(wd,1),"echec")==-1){
			//si on est deja dans un tar et le path du fichier commence par /
			if(filepath[0]=='/'){
				pwd=strdup(cd(filepath,pwd));
				return pwd;
			}
			
			if(filepath[0]=='.' && filepath[1]=='.' && filepath[2]=='/'){
				//si on est deja dans un tar et le path du fichier commence par ../
				char *n=strdup("");;
				strcat(n,path_behind(wd));
				strcat(n,filepath+3);
				pwd=strdup(cd(n,pwd));
				return pwd;
				}
				
			else{
				//si on est deja dans un tar et on saisie le nom d'un dossier dans le tar(sans reecrire t.tar/....)
				chdir(before_tar(wd));
				getcwd(tmp,MAX_LEN);
				char *n=strdup(tmp);
				strcat(n,"/");
				strcat(n,tar_name(wd));
				strcat(n,"/");
				strcat(n,filepath);
				pwd=strdup(cd(n,pwd));
				}
			}
	return pwd;
}
	/****tar part****/
	else{
		//si on fait cd dans un tar
		if(cmp(split(filepath,2),"")==0){
			int fd = open(split(filepath,1),O_RDONLY);
			if(fd==-1){
				write(1,"cd : No such file or directory3\n",strlen("cd : No such file or directory3\n"));
				return wd;
				}
			chdir(before_tar(filepath));
			getcwd(tmp,MAX_LEN);
			pwd=strdup(tmp);
			strcat(pwd,"/");
			strcat(pwd,tar_name(filepath));
			return pwd;
		}
		else{
			//si on fait cd dans un dossier dans un tar
			int fd =open(split(filepath,1),O_RDONLY);
			if(fd==-1 ){
				write(1,"cd: error while opening the file4\n",strlen("cd: error while opening the file4\n"));
				return wd;
			}
			int b=0;
			while(read(fd,&p,512)>0){
				if(cmp(split(filepath,2),p.name)==0){
					//le fichier n'est pas un dossier
					if(p.typeflag!='5'){
						write(1,"cd : Not a directory5\n",strlen("cd : Not a directory5\n"));	
						chdir(before_tar(filepath));
						getcwd(tmp,MAX_LEN);
						pwd=strdup(tmp);
						strcat(pwd,"/");
						strcat(pwd,tar_name(filepath));
						return pwd;
						}
					b=1;
					}
				}
			//le dossier n'existe pas dans le tar
			if(b==0){
				write(1,"cd : No such file or directory6\n",strlen("cd : No such file or directory6\n"));
				chdir(before_tar(filepath));
				getcwd(tmp,MAX_LEN);
				pwd=strdup(tmp);
				strcat(pwd,"/");
				strcat(pwd,tar_name(filepath));
				return pwd;
			}
			//le dossier existe dans le tar
			chdir(before_tar(filepath));
			getcwd(tmp,MAX_LEN);
			pwd=strdup(tmp);
			strcat(pwd,"/");
			strcat(pwd,tar_name(filepath));	
			/*strcat(pwd,"/");
			strcat(pwd,split(filepath,2));	*/
			return pwd;		
				}
			}
	
	return pwd;
} 
