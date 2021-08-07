                    ---ABOUT THE PROGRAM---
A program to read a file, find the size of the file and find the number of times the word given from the given command.
The output of the program will be inputted in an output file.

Before starting the program, enter "make count" to compile. 
Once the program is compiled, enter "./count <input-filename> <search-string> <output-filename>" to start the program.
This command will be stored in argv:
    -argv[0]: program
    -argv[1]: input file
    -argv[2]: searched word
    -argv[3]: output file

Once the program verifies that there are 4 argument entered, the program will open the given input file,
assign argv[2] to char *word variable and create an output file based on argv[3].
Then the program will call two functions: getByteSize(FILE, FILE) and searchWords(FILE, FILE, char).
When the program is complete it will display an output about the inputted command.

program example:
    INPUT COMMAND: 
        ./count example2 tomato example2out
    PROGRAM OUTPUT:
        example2 opened!
        Size of the file is 49

        Number of matches = 6


                    ---HOW TO RUN THE PROGRAM---

TO COMPILE:
    make count

TO CLEAN:
rm count

COMMAND TO RUN THE PROGRAM:

    ./count <input-filename> <search-string> <output-filename>


                    ---PROBLEM ENCOUNTERED and OTHER ISSUES TO KNOW---
Program doesn't provide correct word match output for example2, and if the search word is "the".


                    ---Contents---
count.c         example1        example4
makefile        example2
Readme          example3

