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
        break;
      default:
        break;
      }
      if (internalIFlag == 1) {
        *iFlag = internalIFlag;
      }
     
    return optind;

}

void executeGREPCommandForSingleFile(char* filePath, char* stringToMatch, int iFlag) {
  FILE *file = fopen ( filePath, "r" );
   if ( file != NULL )
   {
      char line [ 1024 ]; 
      while ( fgets ( line, sizeof line, file ) != NULL ) 
      {
        if (iFlag==0) {
        	if(strstr(line , stringToMatch) !=NULL)
        	{
          		printf("%s:%s\n" , filePath, line);
        	}
        	else 
        	{
            	continue;
        	}
         }
		if (iFlag==1) {
			if(strcasestr(line , stringToMatch) !=NULL)
        	{
          		printf("%s:%s\n" , filePath, line);
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
      perror ( filePath ); 
   }
}
void executeGREPCommand(char** arguments, int numberOfPaths){
  int iFlag = 0;
  int initialArgumentIndex = parseGREPCommand(arguments, numberOfPaths, &iFlag);
	if ((iFlag == 1 && numberOfPaths >= 4) || numberOfPaths>=3) {
    		char* stringToMatch = malloc(PATH_MAX);
    		strcpy(stringToMatch,arguments[initialArgumentIndex]);

    		int i = initialArgumentIndex + 1;
    		while(i<numberOfPaths){
	    		//convert to absolute path
		 	char * path = getFullPath(arguments[i]); 
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
