/*Server version by Loureen "Rin" Viloria: 
Assignment#2
PROGRAM DESCRIPTION:
This is a server-client program that allows the client to send a file to the server.
The server then will get the byte size of the file and send it back to the client.
COMMAND FOR THIS TO WORK:
SERVER: ftps <local-port>
CLIENT: ftpc <remote-IP> <remote-port> <localfiletotransfer>*/


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <libgen.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1

//gets the byte size of the file received
int getByteSize(int cli, int ifSize){
    int recCli = read(cli, &ifSize, 4);
    if (recCli > 4 || recCli != 4){
        printf("Error in filesize\n");
        exit(1);
    }
    ifSize = ntohl(ifSize);
    printf("Received file size: %d\n", ifSize);
    
    return ifSize;
}

//writes data received to a file
void copyFile(int cli, int fsize, char *buff, FILE *fp){
    int client;
    for (int i = 0; i < fsize; i++){
        client = read(cli, buff, 1);
        if (client){
            fwrite(buff, 1, 1, fp);
        }else{
            printf("FILE NOT SENT!\n");
            exit(1);
        }
    }
}

int main(int argc, char *argv[]){
    int sd, conSd; //socket desc sd = server conSd = client
    int rc; //return code
    char fn[255]; //filename
    struct sockaddr_in serverAdd;
    struct sockaddr_in clientAdd;
    char buffer[BUFFER_SIZE];
    int flags = 0;
    socklen_t fromLength;
    int filesize;
    FILE *fp;
    int checkDir;
    char *dirName = "recvd";
    int recvdBytes;
    DIR *directory;

    if (argc < 2 || argc > 2){
        printf("Wrong command!\nPlease enter: ftps <portnumber>\n");
    }

    //creating a directory
    checkDir = mkdir(dirName, 0755);
    if (!checkDir){
        printf("Directory created!\n");
        closedir(directory);
    }else{
        directory = opendir(dirName);
    }

    //for stream
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd == -1){
        printf("Could not create socket! (sorry!)\n");
    }
    else{
        puts("socket created!\n");
    }

    serverAdd.sin_family = AF_INET;
    serverAdd.sin_addr.s_addr = INADDR_ANY;
    serverAdd.sin_port = ntohs(atoi(argv[1])); // stores the port number received from command

    //binding
    bind(sd, (struct sockaddr *)&serverAdd, sizeof(serverAdd));
    
    listen(sd, 5);
        //accepting incoming connection
    puts("Waiting for incoming connections...");
    
    conSd = accept(sd, (struct sockaddr *) &clientAdd, &fromLength);
   
    if(conSd < 0){
        perror("connection failed.\n");
        return 1;
    }
    puts("connection accepted!\n");


    while (1){
        printf("Waiting on file...\n");

        //call getByteSize function
        filesize = getByteSize(conSd, filesize);
        memset(fn, 0, 255);

        //writes the filename received
        for (int i = 0; i < 255; i++){
            read(conSd, buffer, 1);
            if(buffer[0] == '\000' || buffer[0] == ' '){
                continue;
            }else{
                strncat(fn, buffer, 1);
            }
        }

        //opens file
        fp = fopen(buffer, "wb");
        printf("\nfile received:%s\n", buffer);

        if(!fp){
            printf("file not opened!\n");
        }

        //add directory
        size_t dLength = strlen(dirName);
        memmove(fn + dLength, fn, strlen(fn) + 1);
        memcpy(fn, dirName, dLength);

        copyFile(conSd, filesize, buffer, fp);

        //sending  response
        fseek(fp, 0, SEEK_END);
        recvdBytes = (ftell(fp));

        if (recvdBytes == filesize){
            recvdBytes = htonl(recvdBytes);
            write(conSd, &recvdBytes, 4);
        }else{
            printf("Complete file not received!\n");
        }

        //closing everything
        fclose(fp);
        break;
    }
    close(sd);
    close(conSd);
    return 0;
}
