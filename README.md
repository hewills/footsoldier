			------------------------------------------
                                 F O O T S O L D I E R  v.1.1
			------------------------------------------
				    by Hana
                                     May 6, 2009
                       
    
I created this boardgame quickly off the top of my head, so the gameplay and AI aren't exactly
great and/or balanced. I coded it to learn basic C++ and Allegro. Each round will probaby only
last between 60 seconds to 5 minutes tops.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
INTRO
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
You control the Green "army" of 3 footsoldiers, a medic, and a radio engineer. You play
against the evil Red "army" (the computer).

	* Footsoldiers are the only ones who can kill.

	* Medics can bring hospitalized soldiers back to the battlefield.

	* Radio Engineers can radio Home Base and have 
	  them move your Helipad to your Engineer's location.

To Complete your Mission you must end the game with the most points.
You get points by killing enemy soldiers and by stealing the secret plans from the enemy Helipad.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HOW TO PLAY
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The game board will place all the pieces down randomly. 

You may move a soldier 1 adjacent square (up/down/left/right)
or
have a soldier take 1 action on your turn.

Footsoldiers can only attack adjacent squares (up/down/left/right)
Medics can only heal "dead" soldiers (duh) (see left column of the gameboard)
Radio Engineers can only move Helipads to an adjacent EMPTY square

You GAIN 1pt. for each enemy you kill, and 2 pts. for stealing the enemies plans.
You LOSE 1pt. for each soldier your medic heals.

To steal the enemy plans, you must have a footsoldier grab the enemy Helipad,
then return to your own Helipad. 
The game play ends when the Plans are stolen, or if a footsoldier holding the plans gets killed.


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FILES
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
This game was written in C++ and uses the Allegro game library.
To play the game just run footsoldier.exe. I used Dev-C++/Mingw to 
compile the code. You'll need the DLL, the image folder, the font folder, and
the fsgame.dat file for the game to work.

README.txt		- You're looking at it
footsoldier.exe		- Game executable
allegro.h 		- allegro source file
fs.h			- source file
fs.cpp			- source file
main.cpp		- source file
fsgame.h		- data source file
fsgame.dat		- Game data (sounds, images, ect...)
alleg42.dll		- Allegro DLL file (required) v4.2.2
fs.ico 			- Icon
/images/ 		- image files
/fonts/  		- fonts

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CONTACT
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Feel free to contact with questions/comments-

Allegro member: hanaMephitis
Email:          gryphon275@yahoo.com


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LEGAL JUNK
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
I don't accept responsibility for any bad stuff
that this code may have on your person, your computer, or
anything else. Basically, use it at your own risk. 
If it makes you feel better, I haven't had any problems and 
neither have any of the testers.
    
This game is totally 100% free. You can play it, copy it and
give it to whomever. But you CANNOT sell it or make money off it. 
Property of Hana.
    
	
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
THANKS TO:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Sarah for testing
Willy for endless purring
Matthew Leverton for his fade.c code
Allegro Community


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CREDIT
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
This program uses the Allegro Game Programming Library 
by Shawn Hargreaves
http://alleg.sourceforge.net/

I've never been able to get a game off the ground... but 
then I met Allegro. It kicks arse!



****    Goodnight!      ****












