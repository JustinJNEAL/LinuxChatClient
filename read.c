#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h> 

void read();
//code to test the read() function
int main()
{
   read();
   return 0;
}

//read function
void read(){
char ch, file_name[25]="chatroom.txt";
   FILE *fp;
 
   fp = fopen(file_name, "r"); // read mode
 
   if (fp == NULL)
   {
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
   }
 
   printf("WELCOME TO THE CHATROOM \n", file_name);
   printf("======================= \n");
 
   while((ch = fgetc(fp)) != EOF)
      printf("%c", ch);
 
   fclose(fp);

}
