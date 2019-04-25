#include <stdio.h>

void write(char str[50]);

main() {
	//code to test write() function
	write("my name is kelvin");
	return 0;   
}//mean function ends

void write(char str[50]){//function opens and write string to 
//passed as and argument to a fileS
	
	FILE *fp;// create a pointer object of the type File
	fp = fopen("chatroom.txt", "a");//The File oject contains a fopen() function that accepts two argument: filename and mode
	fprintf(fp, str);//fprintf() function to write to the file: accepts the File pointer object and the string to be added
	fprintf(fp,"\n");//prints a new line to the file for next insertion
	fclose(fp);// closes up the file

}//write function ends
