/*
File: fs.h
Author: Hana Wills
Description:
Function headers for fs.cpp
*/

//Class Declarations
class Square {
	public:
        Square();  //constructor

        BITMAP* getBmp();       //return the game bitmap

        void setActive(bool);  //highlighting square
        bool getActive();

        void setPiece(char); //set the Game piece parameters to this square
        char getType();
        //void setType(char);

        void setFulltype(char);
        char* getFulltype();

        void setPlans(bool);  //Set whether piece has stolen plans
        bool getPlans();

        void setDead(bool);   //Set whether piece is "dead"
        bool isDead();

        bool getCpu();       //Set whether its a player piece or a CPU piece
        void setCpu(bool);

        void setCoords(int,int);  //Set piece location on the screen
        int getX1();              //Return game pieces screen location (x,y)
        int getY1();

 	private:
        BITMAP*   bmp;     //Bitmap assigned
        bool active;       //square highlighted
        int x1,y1;         //location on the board
        char      type;    //E = Empty  M = medic  F = footsoldier R = radio engineer
        bool      dead;    //Dead Piece
        bool      plans;   //Piece has plans
        bool      cpu;    //Piece is player or CPU
        char      *fulltype; //Full text of the piece description

};
// END OF DECLARATIONS ------------------------------

 int menu();
 void game_play();
