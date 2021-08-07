                                 ---About the Program---
This is the server program where it will received a file from the client. The program will check the
4 first bytes of the file to get the file size then the rest will be the filename. After reading and getting
the byte size of the file, the file will be copied and moved to another directory named "recvd" and the server
will send the file size to the client.

Program created in Visual Code and tested in csegrid.

                                ---How to Install Program in CSEGRID---
    clone repo:
            $ git clone https://github.com/CSCI3800/assignment-2-smiley.git
            $ cd assignment-2-smiley
            $ git checkout rin

                                ----How to Run Client----
    TO COMPILE:
        gcc -o ftps ftps.c

    TO RUN SERVER:
        ./ftps <port number>

    TO DELETE EXECUTABLE:
        make clean

                                ----Problems Encountered----
    Files received from the client is copied though can't be transferred to recvd directory.
                  
