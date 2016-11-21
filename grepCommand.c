#include "main.h"
#include "grepCommand.h"

int parseGREPCommand(char** arguments, int size, int* iFlag) {
	int internalIFlag = 0;

    int index;
    int option;
    optind = 1;

	while ((option = getopt (size, arguments, "i")) != -1)
      switch (option)
     {
      case 'i':
        internalIFlag = 1;
		optind = 2;
        break;
      default:
        break;
      }
      if (internalIFlag == 1) {
        *iFlag = internalIFlag;
      }
     
    for (index = optind; index < size; index++)
    	printf ("Non-option argument %s\n", arguments[index]);
    return optind;

}

void executeGREPCommandForSingleFile(char* filePath, char* stringToMatch, int iFlag) {
  FILE *file = fopen ( filePath, "r" );
   if ( file != NULL )
   {
      char line [ 1024 ]; /* or other suitable maximum line size */
      while ( fgets ( line, sizeof line, file ) != NULL ) /* read a line */
      {
        if (iFlag==1) {
        	if(strstr(line , stringToMatch) !=NULL)
        	{
          		printf("%s:%s" , filePath, line);
        	}
        	else 
        	{
            	continue;
        	}
         }
		if (iFlag==0) {
			if(strcasestr(line , stringToMatch) !=NULL)
        	{
          		printf("%s:%s" , filePath, line);
        	}
        	else 
        	{
            	continue;
        	}
		}
      }
      fclose ( file );
   }
   else
   {
      perror ( filePath ); /* why didn't the file open? */
   }
}
void executeGREPCommand(char** arguments, int numberOfPaths){
  int iFlag = 0;
  int initialArgumentIndex = parseGREPCommand(arguments, numberOfPaths, &iFlag);

	if ((iFlag == 1 && numberOfPaths > 4) || numberOfPaths>3) {
    char* stringToMatch = malloc(PATH_MAX);
    strcpy(stringToMatch,arguments[initialArgumentIndex]);
    //TODO: Make changes to accept flags, validate paths and ignore invalid paths

	
	
     printf("flag value i: %d \n", iFlag);
    printf("initialArgumentIndex = %d \n",initialArgumentIndex);
    int i = initialArgumentIndex + 1;
    while(i<numberOfPaths){
    //convert to absolute path
      char * path = getFullPath(arguments[i]); 
	  printf("caminho = %s \n",path);
      int pathType = checkIfPathExists(path);
    //validating input
      if (pathType == PATH_TO_FILE) {
        executeGREPCommandForSingleFile(path, stringToMatch, iFlag);
      } else {
        if (pathType == PATH_TO_DIRECTORY) {
          printf("Invalid argument for grep command: %s is a directory \n",path);
        }
        else {
          printf("Invalid argument for grep command : path %s does not exist \n",path);
        }
      }
      i++;
    }
  }
  else {
    printf("You must provide at least 2 arguments to the grep command: the string to match and a file \n");
  } 
}
