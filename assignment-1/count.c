#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define BUFFER_SIZE 20

//function to get the byte size of the file
void getByteSize(FILE *ifile, FILE *ofile){ 
    fseek(ifile, 0L, SEEK_END);
    long int size = ftell(ifile);
    fseek(ifile, 0, SEEK_SET);
    if (size != -1){
        printf("\nSize of the file is %ld\n", size);
        fprintf(ofile, "\nSize of the file is %ld\n", size);
    }

}

//function to get the number of times the inputted word was found.
void searchWords(FILE *ifile, FILE *ofile, char *w){
    char string[BUFFER_SIZE];
    char *p;

    int index;
    int count = 0;
    
    while((fgets(string, BUFFER_SIZE, ifile)) != NULL){
        index = 0;
        while((p = strstr(string + index, w)) != NULL){
            index = (p - string) + 1;
            count++;
        }
    }
    fprintf(ofile, "\nNumber of matches = %d\n", count);
    printf("\nNumber of matches = %d\n", count);
}

//TODO: CREATE AN OUTPUT FILE FUNCTION
int main(int argc, char *argv[]){
    
    //variable declarations
    FILE *infile;
    FILE *outputfile;
    char *word;

    //if statement to see if there is a complete inputted argument
    if (argc == 4){
        infile = fopen(argv[1], "rb");
        word = argv[2];
        
        if (infile == NULL){
            printf("\n%s not found!\n", argv[1]);
        }
        else{
            //if the number of command entered is correct, program will display an ouput the the file is opened.
            //and will run the other 2 functions
            printf("\n%s opened!", argv[1]);
            
            outputfile = fopen(argv[3], "wb+");

            //function calls
            getByteSize(infile, outputfile);
            searchWords(infile, outputfile, word);
        }

    }
    if (argc > 4){
        printf("\nEnter complete command\n");
        exit(1);
    }
    //closes the files
    fclose(infile);
    fclose(outputfile);
    return 0;
}



