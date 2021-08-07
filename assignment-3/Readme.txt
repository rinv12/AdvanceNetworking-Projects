By: Loureen Viloria
Student ID:109068508

					===Tictactoe-Client side===

					===ABOUT THE PROGRAM===
This is the updated version of Tictactoe program by Prof. Dave Ogle. This version of the code
serves as a client side playing as player 2. The program will start once the client is connected
to the server. The program will continue to run until a player wins or a draw. If the player entered
a wrong value (ex.: <1->9, or a non-integer value).

The program is tested in VS Code and csegrid.
					
					===How to Install in CSEGRID===
	clone repo:
		$git clone https://github.com/CSCI3800/davescodeforlab3-eric-rin.git
		$cd davescodeforlab3-eric-rin
		$git checkout rin

					===To compile===

		gcc -o tictactoeOriginal tictactoeClient.c
		
	To run:
		./tictactoeOriginal <ip address> <port> 2

	
	To delete executable:
		make clean		
