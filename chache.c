#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "chache.h"
#include "inode.h"



#define ICOUNTER "icounter"

static int sock_fd;


bool get(char * key, char * value);
bool set(char * key, char * value);
void send_set_request(char * key, int size, void * data);
void send_get_request(char * key);
bool recieve_get_data(void * data);
bool stored_success();
//void make_block_key(char * path,int sector,char * key);
void make_block_key(long long hash,int sector,char * key);
void make_key(char * path, char * key);
bool remove_hash(char * key);

/*
bool mem_set_inode(char * path, struct inode_disk * inode){
    char key[20];
    make_key(path,key);
    send_set_request(key,sizeof(struct inode_disk),inode);

    return stored_success();
}*/

bool mem_set_inode(long long hash, struct inode_disk * inode){

    //printf("in chache: %d\n",(int)inode->mode);

    char key[20];
    //make_key(path,key);
    sprintf(key,"%lld",hash);
    send_set_request(key,sizeof(struct inode_disk),inode);

    return stored_success();
}
/*
bool mem_get_inode(char * path,struct inode_disk * inode){
    char key[20];
    make_key(path,key);

    send_get_request(key);

    return recieve_get_data(inode);
}*/

bool mem_get_inode(long long hash,struct inode_disk * inode){
    char key[20];
    sprintf(key,"%lld",hash);

    send_get_request(key);

    return recieve_get_data(inode);
}
/*
bool mem_block_read(char * path,int sector,void * block){

    char key[30];
    make_block_key(path,sector,key);

    get(key,block);
}*/

bool mem_block_read(long long hash,int sector,void * block){

    char key[30];
    make_block_key(hash,sector,key);

    if(get(key,block)){
        return true;
    }else{
        bzero(block,BLOCK_SECTOR_SIZE);
        return false;
    }
}
/*
bool mem_block_write(char * path, int sector,void * block){

    char key[30];
    make_block_key(path,sector,key);

    return set(key,block);
}*/

bool mem_block_write(long long hash, int sector,void * block){

    char key[30];
    make_block_key(hash,sector,key);

    return set(key,block);


}

bool mem_block_remove(long long hash, int index){
    char key[50];
    make_block_key(hash,index,key);
    remove_hash(key);
}

bool mem_inode_remove(long long hash){
    char key[30];
    //make_key(path,key);
    sprintf(key,"%lld",hash);
    return remove_hash(key);
    
}

bool remove_hash(char * key){
    char request[100];
    bzero(request,100);
    strcat(request,"delete ");
    strcat(request,key);
    strcat(request,"\r\n");

    printf("%s\n",request);

    write(sock_fd,request,strlen(request));

    char buff[10] = {0};
    read(sock_fd,buff,10);
    if(!strncmp(buff,"DELETED",7)){
        return true;
    }
    return false;
}


bool stored_success(){

    char buff[100] = {0};

    read(sock_fd, buff, 100);
    if(!strncmp(buff,"STORED",6)){
        return true;
    }
    return false;

}


bool mem_set_icounter(int counter){

    /*char snum[20];
    sprintf(snum,"%d",counter);
    int len = strlen(snum);
    char slen[5];
    sprintf(slen,"%d",len);

    char buff[1200] = "set ";
    strcat(buff,ICOUNTER);
    strcat(buff," 0 900 ");
    strcat(buff,slen);
    strcat(buff,"\r\n");
    strcat(buff,snum);
    strcat(buff,"\r\n");

    int size = strlen(buff);

    write(sock_fd, buff, size);*/



    send_set_request(ICOUNTER,sizeof(int),&counter);

    return stored_success();

}
/*
int mem_get_icounter(){

    send_get_request(ICOUNTER);

    int result;
    recieve_get_data(&result);
    
    return result;

}
*/
/*
int mem_get_next_inumber(){
    int icounter = mem_get_icounter();
    mem_set_icounter(icounter + 1);
    return icounter;

}
*/

/*
int mem_inode_add(struct inode_disk * data){
    int inumber = mem_get_next_inumber();
    
    mem_set_inode(inumber,data);


    return inumber;

}
*/


bool get(char * key,char * value){

    /*//sending querry to memcache server
    char buff[1200] = "get ";
    strcat(buff,key);
    strcat(buff,"\r\n");

    int size = strlen(buff);

    write(sock_fd, buff, size);*/

    send_get_request(key);


    //reading answer

    /*read(sock_fd, buff, 1200); 

    if(!strncmp(buff,"END",3)){
        return false;
    }

    char * keyword= strtok(buff," "); 
    char * flags = strtok(NULL," "); 
    char * time = strtok(NULL," "); 
    char * valuee = strtok(NULL,"\r\n"); 


    //calculating header size
    int keyword_size = strlen(keyword) + 1; 
    int flags_size = strlen(flags) + 1; 
    int time_size = strlen(time) + 1; 
    int value_size = strlen(valuee) + 2; 

    int total_header_size = keyword_size + flags_size + time_size + value_size;

    //getting data in value buffer
    memcpy(value,buff + total_header_size,BLOCK_SECTOR_SIZE);
    char * end_message = buff + total_header_size + BLOCK_SECTOR_SIZE + 2;
    
   

    //checing if request was sucessful
    ASSERT(!strcmp(end_message,"END\r\n"));*/

    return recieve_get_data(value);
    

}


bool set(char * key, char * value){

    
    /*char buff[1200] = "set ";
    strcat(buff,key);
    strcat(buff," 0 900 1024\r\n");

    int pos = strlen(buff);

    memcpy(buff+pos,value,BLOCK_SECTOR_SIZE);
    memcpy(buff+pos+BLOCK_SECTOR_SIZE,"\r\n",2);

    write(sock_fd, buff, pos + BLOCK_SECTOR_SIZE + 2);*/

    send_set_request(key,BLOCK_SECTOR_SIZE,value);

    return stored_success();

}


void mem_init(){

    int sockfd; 
    struct sockaddr_in servaddr; 
  
    // socket create and varification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
  
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(PORT); 
  
    // connect the client socket to server socket 
    if (connect(sockfd, (struct sockaddr * )&servaddr, sizeof(servaddr)) != 0) { 
        printf("connection with the server failed...\n"); 
        exit(0); 
    } 
    else
        printf("connected to the server..\n");

    sock_fd = sockfd;
}

void mem_clear(){

    write(sock_fd, "flush_all\r\n", 11);
    
    char result[10];
    bzero(result,10);
    read(sock_fd, result, 10);

    ASSERT(!strcmp(result,"OK\r\n"));

}

void mem_destroy(){

    close(sock_fd);

}


void send_set_request(char * key, int size, void * data){

    char buff[1200] = "set ";
    strcat(buff,key);
    strcat(buff," 0 0 ");

    char ssize[5];
    sprintf(ssize,"%d",size);

    strcat(buff, ssize);
    strcat(buff,"\r\n");

    int pos = strlen(buff);

    memcpy(buff+pos,data,size);
    strcat(buff+pos+size,"\r\n");

    write(sock_fd,buff,pos + size+2);

}

void send_get_request(char * key){

    char buff[120] = "get ";
    strcat(buff,key);
    strcat(buff,"\r\n");


    int size = strlen(buff);
    write(sock_fd, buff, size);

}

bool recieve_get_data(void * data){
    
    char buff[1200];

    read(sock_fd, buff, 1200); 

    if(strncmp(buff, "VALUE", 5)){
        return false;
    }

    char * keyword = strtok(buff," "); 
    char * key = strtok(NULL," "); 
    char * flags = strtok(NULL," "); 
    char * c_size = strtok(NULL,"\r\n");

    int header_size = strlen(keyword) + strlen(key) + strlen(flags) + strlen(c_size) + 5; 
    int data_size = strtol(c_size,NULL,10);
    
    if(strncmp(buff + header_size + data_size + 2,"END",3)){
        return false;
    }

    memcpy(data,buff+header_size,data_size);

    return true;
}
/*
bool mem_block_add(char * path,int sector){

    char block[BLOCK_SECTOR_SIZE];
    mem_block_write(path,sector,block);
}
*/

void make_key(char * path, char * key){

    

    sprintf(key,"%lld",get_hash(path));
    

}

long long get_hash(const char * path){

    long long hash = 0;
    long long prime = 1610612741;

    for(int i=0;path[i]!='\0';i++){
        hash = hash * 257 + (long long)path[i];
        hash = hash % prime;
    }

    return hash;
}
/*
void make_block_key(char * path, int sector, char * key){
    
    make_key(path,key);
    strcat(key,"A");
    sprintf(key+strlen(key),"%d",sector);
}*/

void make_block_key(long long hash, int sector, char * key){
    
    sprintf(key,"%lld",hash);
    strcat(key,"A");
    sprintf(key+strlen(key),"%d",sector);
}

/*
int main(){

	
    mem_init(); 

    char buff[1024];
    //memset(buff,1,1024);

   


    mem_clear();

    char buff2[1024] = {0};
    
    //printf("%s\n",buff2);


    
    close(sock_fd);

	return 0;
}*/
