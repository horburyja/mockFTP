#include "command.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <dirent.h>


char* makeFileList(char* path)
{
	DIR* theCWD = opendir(path);
	struct dirent buf;
	struct dirent* cur = NULL;
	readdir_r(theCWD,&buf,&cur);
	int ttlBytes = 0;
	while(cur != NULL) {
		ttlBytes += strlen(cur->d_name) + 1;
		readdir_r(theCWD,&buf,&cur);
	}
	char* txt = malloc((ttlBytes + 10)*sizeof(char));
	rewinddir(theCWD);
	readdir_r(theCWD,&buf,&cur);
	*txt = 0;
	while(cur != NULL) {
		strcat(txt,cur->d_name);
		strcat(txt,"\n");
		readdir_r(theCWD,&buf,&cur);
	}
	closedir(theCWD);
	return txt;
}

int getFileSize(char* fName)
{
	FILE* f = fopen(fName,"r");
	fseek(f,0,SEEK_END);
	long sz = ftell(f);
	fclose(f);
	return (int)sz;
}

void sendFileOverSocket(char* fName,int chatSocket)
{  
   // open file and allocate space for buffer
   FILE* fp = fopen(fName, "rw");
   char* buf = malloc(sizeof(char)*getFileSize(fName));
   
   fread(buf, sizeof(char), getFileSize(fName), fp);
   
   // send file over socket
   int rem = getFileSize(fName);
   int rec = 0;
   
   while(rem != 0) {
   	int nbsend = send(chatSocket, buf + rec, rem, 0);
   	rec += nbsend;
   	rem -= nbsend;
   }
   
   // close file and free buffer
   fclose(fp);
   free(buf);
}

void receiveFileOverSocket(int sid,char* fname,char* ext,int fSize)
{	
	// allocate buffer to store file
	char* buf = malloc(sizeof(char)*fSize);
	
	// use recv to recieve file
	int rec = 0;
    int rem = fSize;
    
    while(rem != 0) {
    	int nbrecv = recv(sid, buf + rec, rem, 0);
    	rec += nbrecv;
    	rem -= nbrecv;
    }
	
	char fn[512];
	strcpy(fn,fname);
	strcat(fn,ext);
	FILE* fd = fopen(fn,"w");
	
	// fwrite file to fd
	fwrite(buf, 1, fSize, fd);
	
	// close fd and free buffer
	fclose(fd);
	free(buf);
}
