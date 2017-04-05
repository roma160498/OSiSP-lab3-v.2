#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include "string.h"
#include <stdlib.h>
#include <errno.h>

char *progName;
int workingProcess=1;
int maxWorkingProcess;
  int amount=0;
  int amountOfInputFolders=0;
void CreateArgsErrorName(char* progName)
{
  strcat(progName,": Incorrect args count\n");
  return;
}
char** ParseTheString(const char *string, int *cnt)
{
    char **words = NULL, *name = NULL;
               // int cnt = 0;
                //char *temp = strdup(fullFileName);
                name = strtok(string, "/");
                //printf("%s\n",name);
                words = (char**)realloc(words,sizeof(char*)*((*cnt)+1));
                words[*cnt]=strdup(name);
                (*cnt)++;
                while (name !=NULL)
                {
                    name = strtok(NULL,"/");
                    if (name!=NULL)
                    {
                    words = (char**)realloc(words,sizeof(char*)*((*cnt)+1));
                    words[*cnt]=strdup(name);
            
                // printf("%s\n",name);
                    (*cnt)++;
                    }
                }
                //(*cnt)--;
                return words;

}
int CreateErrorMsg(const char* path)
{
  char* errorPath;
  errorPath = (char *)malloc(strlen(path)+strlen(progName)+2);
  sprintf(errorPath, "%s %s", progName, path);
  perror(errorPath);  
  free(errorPath);
  return -1;
}

void copy(char *source, char *dest)
{
    int childExitStatus;
    int status;
      char **abc = NULL, **def = NULL; 
      char *temp1 = strdup(source);
      char *temp2 = strdup(dest);
   //   abc =  ParseTheString(temp1);
    //  def = ParseTheString(temp2);
      //for 
    //vot tut kak-to peredat' rabotu odnomu processu
  //  pid_t pid;
 //   if (pid == 0) {
        execl("/bin/cp", "/bin/cp", source, dest, (char *)0);
  //  }
  //  else if (pid < 0) {
//	CreateErrorMsg(source);
  //  }
  /*  else {
        pid_t ws = waitpid( pid, &childExitStatus, WNOHANG);
        if (ws == -1)
        {
	  CreateErrorMsg(source);
        }
        if( WIFEXITED(childExitStatus))  exit code in childExitStatus
        {
            status = WEXITSTATUS(childExitStatus);
        }
        else if (WIFSIGNALED(childExitStatus))  killed 
        {
        }
        else if (WIFSTOPPED(childExitStatus))  stopped 
        {

        }
    }*/
}
int CombareByBytes(const char* firstFile, const char* secondFile)
{
    FILE *file1, *file2;
    int counter=0;
        if ((file1 = fopen(firstFile, "rb")) != NULL)
        {
            if ((file2 = fopen(secondFile, "rb")) != NULL)
            {
                int ch1, ch2;
                do
                {
                    ch1 = getc(file1);
                    ch2 = getc(file2);
                    if (ch1 != ch2)
                    {
                        counter++;
                        fclose(file1);
                        fclose(file2);
                        return 0;
                    }
                }
                while(ch1 != EOF);
                if (counter == 0)
                {
                    fclose(file1);
                    fclose(file2);
                    return 1;
                }
            }
            else
                printf("Не удается открыть второй файл!");
        }
        else
            printf("Не удается открыть первый файл!");
}

int ComapareFiles(char **words,int length,const char* fileName, const char* directory2)
{
    char *newPath;
    DIR *dir = opendir(directory2);
    char *fullFileName;
    struct tm  *timeInf;
    char timeString[30];
    if (dir == NULL)
    {
        char* errorPath;
        errorPath = (char*)malloc(strlen(progName)+1);
        sprintf(errorPath,"%s",progName);
        perror(errorPath);
        return -1;
    }
    else
    {
        struct dirent *de;
        struct stat statBuff;
        struct stat inputBuff;

        while ((de = readdir(dir)) != NULL)
        {
            if (de->d_type == DT_REG)
            {
                fullFileName = (char*)malloc(strlen(directory2)+strlen(de->d_name)+2);
                sprintf(fullFileName,"%s/%s",directory2,de->d_name);

                if (!strcmp(de->d_name,words[length]))
                {
                    if (stat(fullFileName,&statBuff) != 0 || stat(fileName,&inputBuff)!=0)
                    {
                        char* errorPath;
                        errorPath = (char*)malloc(strlen(progName)+1);
                        sprintf(errorPath,"%s",progName);
                        perror(errorPath);
                        return -1;      
                    }
                    else
                    {
                        if (statBuff.st_size==inputBuff.st_size)
                        {
                            if (CombareByBytes(fileName,fullFileName)==1)
                                return 1;
                        }   
                    }
                }
                free(fullFileName);
            }
            else
                if (de->d_type == DT_DIR && strcmp(de->d_name,".") && strcmp(de->d_name,".."))
                {
                    newPath = (char*)malloc((strlen(directory2)+strlen(de->d_name)+2)*sizeof(char));
                    sprintf(newPath, "%s/%s", directory2, de->d_name);
                    if(ComapareFiles(words,length,fileName,newPath)==1){
                        return 1;
                    free(newPath);
                }
        }
    }
    if (closedir(dir) == -1)
    {
        char* errorPath;
        errorPath = (char*)malloc(strlen(progName)+1);
        sprintf(errorPath,"%s",progName);
        perror(errorPath);
        return -1;
        
    }
    }
        return 0;
}
void print_results(int pid, char *full_path)
{
	printf("%d %s\n\n\n", pid, full_path);
}

int CatalogSearching(const char* directory, const char* directory2)
{
    char *newPath;
    DIR *dir = opendir(directory);
    char *fullFileName;
    struct tm  *timeInf;
    char timeString[30];
    
    if (dir == NULL)
    {
        char* errorPath;
        errorPath = (char*)malloc(strlen(progName)+1);
        sprintf(errorPath,"%s",progName);
        perror(errorPath);
        return -1;
    }
    else
    {
        struct dirent *de;
        struct stat statBuff;

        while ((de = readdir(dir)) != NULL)
        {
            if (de->d_type == DT_REG)
            {
                fullFileName = (char*)malloc(strlen(directory)+strlen(de->d_name)+2);
                sprintf(fullFileName,"%s/%s",directory,de->d_name);
                char **words = NULL, *name = NULL;
                int cnt = 0;
                char *temp = strdup(fullFileName);
            // words = ParseTheString(temp)
                name = strtok(temp, "/");
                //printf("%s\n",name);
                words = (char**)realloc(words,sizeof(char*)*(cnt+1));
                words[cnt]=strdup(name);
                cnt++;
                while (name !=NULL)
                {
                    name = strtok(NULL,"/");
                    if (name!=NULL)
                    {
                    words = (char**)realloc(words,sizeof(char*)*(cnt+1));
                    words[cnt]=strdup(name);
            
                // printf("%s\n",name);
                    cnt++;
                    }
                }
                cnt--;
                if (ComapareFiles(words,cnt,fullFileName,directory2)==0)
                {
                    if (workingProcess >= maxWorkingProcess)
       		        {
       			        wait(NULL);
       			        workingProcess -= 1;
       		        }
                    pid_t pid = fork();
                    if (pid == 0)
                    {
                        print_results(getpid(),fullFileName);
                        copy(fullFileName,directory2); 
                        exit(0);
                    }
                    else if (pid < 0)
                    {
                	    exit(1);
                    }
                    workingProcess += 1;
                   }
                    for (int i = 0; i < cnt-1; ++i ){
                 if ( words[i] != NULL ){
                          free(words[i]);
                      words[i] = NULL;
                    }
               }
                   free(words);
                   words = NULL;
               /* if (!strcmp(de->d_name,file))
                {
                    if (stat(fullFileName,&statBuff) != 0)
                    {
                        char* errorPath;
                        errorPath = (char*)malloc(strlen(progName)+1);
                        sprintf(errorPath,"%s",progName);
                        perror(errorPath);
                        return -1;
                    }
                    else
                    {
                        timeInf = localtime(&(statBuff.st_mtime));
                        strftime(timeString,30,"%d %b %H:%M",timeInf);
                        printf("%s %jd %s ",fullFileName, (statBuff.st_size), timeString);
                        printf((statBuff.st_mode & S_IRUSR) ? "r" : "-");
                        printf( (statBuff.st_mode & S_IWUSR) ? "w" : "-");
    	                printf( (statBuff.st_mode & S_IXUSR) ? "x" : "-");
    	                printf( (statBuff.st_mode & S_IRGRP) ? "r" : "-");
    	                printf( (statBuff.st_mode & S_IWGRP) ? "w" : "-");
    	                printf( (statBuff.st_mode & S_IXGRP) ? "x" : "-");
    	                printf( (statBuff.st_mode & S_IROTH) ? "r" : "-");
    	                printf( (statBuff.st_mode & S_IWOTH) ? "w" : "-");
    	                printf( (statBuff.st_mode & S_IXOTH) ? "x " : "- ");
                        printf("%ju \n",statBuff.st_ino);
                    }
                }*/
                free(fullFileName);
            }
            else
                if (de->d_type == DT_DIR && strcmp(de->d_name,".") && strcmp(de->d_name,".."))
                {
                   // amount++;
                    newPath = (char*)malloc((strlen(directory)+strlen(de->d_name)+2)*sizeof(char));
                    sprintf(newPath, "%s/%s", directory, de->d_name);
                    CatalogSearching(newPath,directory2);
                    free(newPath);

                }
        }
        if (errno != 0)
        {
            int errSave = errno;
            char *errorPath;
            errorPath = (char *)malloc(strlen(progName)+1);
            sprintf(errorPath, "%s", progName);
	        char *errorInfo; 
	        errorInfo = (char *)malloc(strlen(directory)+80);	
            if (errSave == EOVERFLOW)
	        {
	            sprintf(errorInfo, "%s: Value too large for defined data type.\n", progName);
	            perror(errorInfo);
	        }
	        else if (errSave == EBADF)
	        {
	            sprintf(errorInfo, "%s: Bad file number.\n", progName);
	            perror(errorInfo);
	        }   
	        else if (errSave == ENOENT)
	        {
	            sprintf(errorInfo, "%s: No such file or directory.\n", progName);
	            perror(errorInfo);	
	        }
            else if (errSave == EFAULT)
	        {
	            sprintf(errorInfo, "%s: Bad address.\n", progName);
	            perror(errorInfo);	
	        }
            else if (errSave == ENOENT)
	        {
	            sprintf(errorInfo, "%s: Invalid argument.\n", progName);
	            perror(errorInfo);	
	        }
            return -1;
        }
    }
    if (closedir(dir) == -1)
    {
        char* errorPath;
        errorPath = (char*)malloc(strlen(progName)+1);
        sprintf(errorPath,"%s",progName);
        perror(errorPath);
        return -1;
    }
    return 0;
}
void main(int argc, char** argv)
{
  progName = argv[0]+2;
  int globalPid = getpid();
  char **folders = NULL;
  char *tempArg = argv[1];
  folders = ParseTheString(tempArg,&amountOfInputFolders);
 // amountOfInputFolders = sizeof(folders)/sizeof(char*);
  if (argc == 4)
  {    
      maxWorkingProcess = argv[3];
      CatalogSearching(argv[1], argv[2]);    
  }
  else
  {
    char* errName = progName;
    CreateArgsErrorName(errName);
    fprintf(stderr,"%s",errName);
  }  
}