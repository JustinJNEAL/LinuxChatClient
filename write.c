#include <stdio.h>

void writeToChatRoom();

main() {
	writeToChatRoom();
	return 0;   
}//mean function ends

void writeToChatRoom(){
	
	FILE *fp;
	fp = fopen("chatroom.txt", "a");
	fprintf(fp, "This is testing for fprintf...\n");
	fclose(fp);

}//writeToChatRoom function ends