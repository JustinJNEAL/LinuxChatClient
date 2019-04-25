#include <stdio.h>

void write(char str[50]);

main() {
	//code to test write() function
	write("my name is kelvin");
	return 0;   
}//mean function ends

void write(char str[50]){//function opens and write string to 
//passed as and argument to a fileS
	
	FILE *fp;
	fp = fopen("chatroom.txt", "a");
	fprintf(fp, str);
	fprintf(fp,"\n");
	fclose(fp);

}//write function ends
