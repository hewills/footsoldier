/*
File: fs.cpp
Author: Hana Wills
Function file for main.cpp */

#include <stdio.h>
#include <string.h>
#include <allegro.h>
#include "fs.h"
#include "fsgame.h"

DATAFILE* data; //data pointer
DATAFILE* music_set; //music set pointer
DATAFILE* sound_set; //sound set pointer
DATAFILE* sound_set2; //sound set pointer

//Mucho global variables
//"Dead" Game pieces
int dead_f1=0,dead_f2=0,dead_f3=0,dead_m1=0,dead_r1=0;
int dead_f1_cpu=0,dead_f2_cpu=0,dead_f3_cpu=0,dead_m1_cpu=0,dead_r1_cpu=0;

//Keep track of player and CPU score
int total_score = 0, total_score_cpu = 0;
int total_wins = 0, total_loses = 0;

//Game wins and lose flags
int win=0, cpuwin = 0;


   //Timers ------------------------
  /*  volatile unsigned int subticks = 0;

   void subticker(void)
   {   subticks++;
   } END_OF_FUNCTION(ticker);
   */
   volatile unsigned int ticks = 0;

   void ticker(void)
   {   ticks++;
   } END_OF_FUNCTION(ticker);
   // -------------------------------

/* Square Class ---------------------------------
A square on the game board. It starts out empty (without a game piece).
The x/y location will stay the same throughout the game.
Input: x/y location on the board
Output: none
*/
Square::Square()
{
  bmp = (BITMAP*)data[FS_empty].dat;        //Empty bitmap is default
  active = FALSE;                             //default is un-highlighted
  type = 'E';            //E = Empty  M = medic  F = footsoldier R = radio engineer
  fulltype = "Empty";
  dead = FALSE;
  plans = FALSE;
  cpu = FALSE;
  x1 = 0;
  y1 = 0;
}

inline void Square::setCoords(int x_1,int y_1)
{
  x1 = x_1;
  y1 = y_1;
}

inline int Square::getX1()
{
return x1;
}

inline int Square::getY1()
{
return y1;
}

inline bool Square::isDead()
{
return dead;
}

inline void Square::setDead(bool deadchoice)
{
  dead = deadchoice;
}

inline bool Square::getCpu()
{
  return cpu;
}

inline void Square::setCpu(bool ifcpu)
{
   cpu = ifcpu;
}

inline bool Square::getPlans()
{
 return plans;
}

inline void Square::setPlans(bool gotplans)
{
  plans = gotplans;
}

inline BITMAP* Square::getBmp()
{
return bmp;
}

inline char Square::getType()
{
return type;
}

//Don't need this
/*inline void Square::setType(char pieceType)
{
  type = pieceType;
}*/

inline char* Square::getFulltype()
{
  return fulltype;
}
//Highlight the square if active
inline void Square::setActive(bool Highlight)
{
  /* Some one time initialisation code. */
  COLOR_MAP global_light_table;
  set_trans_blender(10, 10, 220, 10);   //blue light or use (220,0,0,0) for red

       if (Highlight)
          {   active = TRUE;
              /* Lite the square with a blueish light.*/
              if ( !dead )
              draw_lit_sprite(screen, bmp, x1, y1, 120);

          }
          else
          {   active = FALSE;  //Unhighlight the square
              if ( !dead )  // dead pieces keep their grey color
              masked_blit(bmp, screen, 0, 0, x1, y1, 47, 47);
          }
}

inline bool Square::getActive()
{
  return active;
}
//Set the Square to a particular game piece (F,M,R,E or H)
inline void Square::setPiece(char pieceType)
{
        if (pieceType == 'M' && !cpu){
         bmp = (BITMAP*)data[FS_medic].dat;
         fulltype = "Medic";
         type = 'M';}

        if (pieceType == 'F' && !cpu && !plans){
         bmp = (BITMAP*)data[FS_footsoldier].dat;
         fulltype = "Footsoldier";
         type = 'F';}

         if (pieceType == 'F' && !cpu && plans){
         bmp = (BITMAP*)data[FS_footsoldier_plans].dat;
         fulltype = "Footsoldier";
         type = 'F';}

        if (pieceType == 'R' && !cpu){
         bmp = (BITMAP*)data[FS_radio].dat;
         fulltype = "Radio Engineer";
         type = 'R';}

        if (pieceType == 'E'){
         bmp = (BITMAP*)data[FS_empty].dat;
         fulltype = "Empty";
         type = 'E';}

        if (pieceType == 'H' && !cpu) {
         bmp = (BITMAP*)data[FS_helipad].dat;
         fulltype = "Helipad";
         type = 'H';}

         /* *** CPU Pieces *** */
           if (pieceType == 'M' && cpu){
         bmp = (BITMAP*)data[FS_medic_cpu].dat;
         fulltype = "Medic";
         type = 'M';}

        if (pieceType == 'F'  && cpu && !plans){
         bmp = (BITMAP*)data[FS_footsoldier_cpu].dat;
         fulltype = "Footsoldier";
         type = 'F';}

          if (pieceType == 'F'  && cpu && plans){
         bmp = (BITMAP*)data[FS_footsoldier_cpu_plans].dat;
         fulltype = "Footsoldier";
         type = 'F';}

        if (pieceType == 'R' && cpu){
         bmp = (BITMAP*)data[FS_radio_cpu].dat;
         fulltype = "Radio Engineer";
         type = 'R';}

        if (pieceType == 'H' && cpu) {
         bmp = (BITMAP*)data[FS_helipad_cpu].dat;
         fulltype = "Helipad";
         type = 'H';}
}
// End of Square ---------------------------------

/* About Screen in Menu
*/
void about_screen()
{
     BITMAP *pic;
     pic = (BITMAP*)data[FS_alex].dat;

     // FONTS
      FONT *smallfont, *myfont;
      PALETTE palette;
      smallfont = load_font("fonts/smallfont.bmp",palette,NULL);
      myfont = load_font("fonts/myfont.pcx",palette,NULL);
      //-----------------------------------

     textout_ex(screen, myfont, "Programming and 'Graphics' by ",20,100,makecol(0,255,0),-1);
     textout_ex(screen, myfont, "Hana Wills",20,120,makecol(128,0,0),-1);
     textout_ex(screen, myfont, "Send comments to ",20,160,makecol(0,255,0),-1);
     textout_ex(screen, myfont, "gryphon275@yahoo.com",20,180,makecol(128,0,0),-1);
     textout_ex(screen, myfont, "Thanks for trying my first attempts at a game!",20,220,makecol(0,255,0),-1);
     textout_ex(screen, myfont, "And thanks to the Allegro Developers and Community.",20,300,makecol(128,255,0),-1);
     textout_ex(screen, myfont, "(www.allegro.cc)",150,320,makecol(128,255,0),-1);
     draw_sprite(screen,pic,330,320);     //Alleg the Allegator
     textout_ex(screen, myfont, "[ESC]",300,400,makecol(0,255,0),-1);

     readkey();

     while(!key[KEY_ESC]){}
     destroy_bitmap(pic);
     destroy_font(smallfont);
     destroy_font(myfont);
     pic = 0;
     smallfont = 0;
     myfont = 0;
}

/*
menu()
Input: none
Output: Print introduction, title, and menu to the screen
*/
int menu()
{
    int track = 0, exit = 0, enter = 0, music = 1;
    BITMAP *background,*menubar;

     //Load GAME data (bitmaps,music,sound effects...)
     data = load_datafile("fsgame.dat");
     if (!data)
     allegro_message("Problem loading fsgame.dat. Make sure it's in game folder. %s\n",allegro_error);

     // FONTS
      FONT *smallfont, *myfont;
      PALETTE palette;
      smallfont = load_font("fonts/smallfont.bmp",palette,NULL);
      myfont = load_font("fonts/myfont.pcx",palette,NULL);
      //-----------------------------------

    //Initialize backdrop and menu fonts
    background = (BITMAP*)data[FS_backdrop].dat;
    menubar = (BITMAP*)data[FS_menuplay].dat;

    //play background music
    //Load Intro MUSIC and SOUND
    music_set = load_datafile_object("fsgame.dat", "background");  //Background music
	if (!music_set)
	allegro_message("Couldn't load background music. Problem loading fsgame.dat%s\n",allegro_error);

    //play music
    play_looped_midi((MIDI*)music_set->dat,0,145);

    //set volume
    set_volume(255,100);

    //Initial screen
    menubar = (BITMAP*)data[FS_menuplay].dat;
    masked_blit(background,screen,0,0,0,0,SCREEN_W,SCREEN_H); //update backdrop picture
    draw_sprite(screen,menubar,433,321);
    textout_ex(screen, myfont, "F2 - Turn Music Off",120,450,makecol(0,0,0),-1);

    while (exit == 0)
    {
         readkey();

        //Turn off Music with F2
        if (key[KEY_F2])
           stop_midi();

        if (key[KEY_UP] && track != 0)
        track--;

        if (key[KEY_DOWN] && track != 2)
        track++;

        if (track == 0)
        {
           menubar = (BITMAP*)data[FS_menuplay].dat;
           masked_blit(background,screen,0,0,0,0,SCREEN_W,SCREEN_H); //update backdrop picture
           draw_sprite(screen,menubar,433,321);
           textout_ex(screen, myfont, "F2 - Turn Music Off",120,450,makecol(0,0,0),-1);
        }

        if (track == 1)
        {  menubar = (BITMAP*)data[FS_menuabout].dat;
           masked_blit(background,screen,0,0,0,0,SCREEN_W,SCREEN_H); //update backdrop picture
           draw_sprite(screen,menubar,446,361);
           textout_ex(screen, myfont, "F2 - Turn Music Off",120,450,makecol(0,0,0),-1);
        }

        if (track == 2)
        {
           menubar = (BITMAP*)data[FS_menuquit].dat;
           masked_blit(background,screen,0,0,0,0,SCREEN_W,SCREEN_H); //update backdrop picture
           draw_sprite(screen,menubar,429,403);
           textout_ex(screen, myfont, "F2 - Turn Music Off",120,450,makecol(0,0,0),-1);
        }

        if (key[KEY_ENTER])
        enter = 1;

      if (enter == 1 && track == 2)
     {
        clear(screen);
        destroy_bitmap(background);
        destroy_bitmap(menubar);
        destroy_font(smallfont);
        destroy_font(myfont);
        background = 0;
        menubar = 0;
        return 2;
      }

      if (enter == 1 && track == 1)
     {  clear(screen);
        about_screen();
        menubar = (BITMAP*)data[FS_menuabout].dat;
        masked_blit(background,screen,0,0,0,0,SCREEN_W,SCREEN_H); //update backdrop picture
        draw_sprite(screen,menubar,446,361);
        textout_ex(screen, myfont, "F2 - Turn Music Off",120,450,makecol(0,0,0),-1);
        enter = 0;
     }

     if (enter == 1 && track == 0)
     {  clear(screen);
        destroy_bitmap(background);
        destroy_bitmap(menubar);
        destroy_font(smallfont);
        destroy_font(myfont);
        background = 0;
        menubar = 0;
        return 0;
     }
   }
}

/* init_board
Initialize the game board squares to their locations, these should not change. This is Random.
Input: The array of game board squares
Output: none
*/
void init_board(Square theboard[])
{
   int sql_num, x=80, y=100, z, random;

   srand(time(0));  //Randomize timer

       //randomly place Players 1st footsolider
       do
       {   random = (rand()%59);
           if ( theboard[random].getType() == 'E' )
           {   theboard[random].setPiece('F');
               break;
           }

       }while (theboard[random].getType() != 'E');

       //randomly place Players 2nd footsoldier
       do
       {   random = (rand()%59);
           if ( theboard[random].getType() == 'E' )
           {   theboard[random].setPiece('F');
               break;
           }

       }while (theboard[random].getType() != 'E');

       //randomly place Players 3rd footsoldier
        do
       {   random = (rand()%59);
           if ( theboard[random].getType() == 'E' )
           {   theboard[random].setPiece('F');
               break;
           }

       }while (theboard[random].getType() != 'E');

       //Randomly place Player's Medic
       do
       {   random = (rand()%59);
           if ( theboard[random].getType() == 'E' )
           {   theboard[random].setPiece('M');
               break;
           }

       }while (theboard[random].getType() != 'E');

       //Randomly place Player's Radio Engineer
         do
       {   random = (rand()%59);
           if ( theboard[random].getType() == 'E' )
           {   theboard[random].setPiece('R');
               break;
           }

       }while (theboard[random].getType() != 'E');

       //Randomly place Player's Helipad
        do
       {   random = (rand()%59);
           if ( theboard[random].getType() == 'E' )
           {   theboard[random].setPiece('H');
               theboard[random].setPlans(TRUE);
               break;
           }

       }while (theboard[random].getType() != 'E');

       /* ********* INITIALIZE THE CPU PIECES ************* */
       //randomly place CPU's 1st footsolider
       do
       {   random = (rand()%59);
           if ( theboard[random].getType() == 'E' )
           {  theboard[random].setCpu(TRUE);
               theboard[random].setPiece('F');
               break;
           }

       }while (theboard[random].getType() != 'E');

       //randomly place CPU 2nd footsoldier
       do
       {   random = (rand()%59);
           if ( theboard[random].getType() == 'E' )
           {  theboard[random].setCpu(TRUE);
               theboard[random].setPiece('F');
               break;
           }

       }while (theboard[random].getType() != 'E');

        //randomly place CPU 3rd footsoldier
        do
       {   random = (rand()%59);
           if ( theboard[random].getType() == 'E' )
           {   theboard[random].setCpu(TRUE);
                theboard[random].setPiece('F');
               break;
           }

       }while (theboard[random].getType() != 'E');

       //Randomly place CPU's Medic
       do
       {   random = (rand()%59);
           if ( theboard[random].getType() == 'E' )
           {  theboard[random].setCpu(TRUE);
                theboard[random].setPiece('M');
               break;
           }

       }while (theboard[random].getType() != 'E');

       //Randomly place CPU's Radio Engineer
         do
       {   random = (rand()%59);
           if ( theboard[random].getType() == 'E' )
           { theboard[random].setCpu(TRUE);
               theboard[random].setPiece('R');
               break;
           }

       }while (theboard[random].getType() != 'E');

       //Randomly place CPU's Helipad
        do
       {   random = (rand()%59);
           if ( theboard[random].getType() == 'E' )
           {  theboard[random].setCpu(TRUE);
                theboard[random].setPiece('H');
                theboard[random].setPlans(TRUE);
               break;
           }

       }while (theboard[random].getType() != 'E');

    //Set the locations for the board squares
   for(sql_num = 0; sql_num < 60; sql_num++)
   {
         theboard[sql_num].setCoords(x,y); //Set coordinates. Squares are 50 pxls from each other

         y = y+50;
         if ( y > 350)
         {
            x = x+50;
            y = 100;
         }
    }
}
// ----- END of init_board ------------------------------------------

/*
draw_graveyard()
Print the dead pieces to the side board for possible healing by Medic
*/
void draw_graveyard()
{
  BITMAP* thisbmp;
  int count;

     if (dead_f1 == 1)
     { thisbmp = (BITMAP*)data[FS_footsoldier_dead].dat;
       draw_sprite(screen,thisbmp,1,75);
     }
     if (dead_f2 == 1)
     { thisbmp = (BITMAP*)data[FS_footsoldier_dead].dat;
       draw_sprite(screen,thisbmp,1,105);
     }
     if (dead_f3 == 1)
     { thisbmp = (BITMAP*)data[FS_footsoldier_dead].dat;
       draw_sprite(screen,thisbmp,1,135);
     }
     if (dead_r1 == 1)
     { thisbmp = (BITMAP*)data[FS_radio_dead].dat;
       draw_sprite(screen,thisbmp,1,165);
     }
     if (dead_m1 == 1)
     { thisbmp = (BITMAP*)data[FS_medic_dead].dat;
       draw_sprite(screen,thisbmp,1,195);
     }
     if (dead_f1_cpu == 1)
     { thisbmp = (BITMAP*)data[FS_footsoldier_dead_cpu].dat;
       draw_sprite(screen,thisbmp,1,255);
     }
     if (dead_f2_cpu == 1)
     { thisbmp = (BITMAP*)data[FS_footsoldier_dead_cpu].dat;
       draw_sprite(screen,thisbmp,1,315);
     }
     if (dead_f3_cpu == 1)
     { thisbmp = (BITMAP*)data[FS_footsoldier_dead_cpu].dat;
       draw_sprite(screen,thisbmp,1,345);
     }
     if (dead_r1_cpu == 1)
     { thisbmp = (BITMAP*)data[FS_radio_dead_cpu].dat;
       draw_sprite(screen,thisbmp,1,375);
     }
     if (dead_m1_cpu == 1)
     { thisbmp = (BITMAP*)data[FS_medic_cpu_dead].dat;
       draw_sprite(screen,thisbmp,1,405);
     }

   thisbmp = 0;
}
// ------- end of draw_graveyard() ---------

/*
send_graveyard()
Input: type = type of piece,  cpu = cpu piece boolean
Flag a piece as dead (aka In the graveyard)
*/
void send_graveyard(char type, bool cpu)
{
    if ( type == 'F' && !cpu && (dead_f1 == 1 && dead_f2 == 1) )
    {   dead_f3 = 1; }
    else if ( type == 'F' && !cpu && (dead_f1 == 1 && dead_f2 == 0) )
    {   dead_f2 = 1; }
    else if ( type == 'F' && !cpu && (dead_f1 == 0) )
    {   dead_f1 = 1; }
    else if ( type == 'M' && !cpu )
    {   dead_m1 = 1; }
    else if ( type == 'R' && !cpu )
    {   dead_r1 = 1; }
    else if ( type == 'F' && cpu && (dead_f1_cpu == 1 && dead_f2_cpu == 1) )
    {   dead_f3_cpu = 1; }
    else if ( type == 'F' && cpu && (dead_f1_cpu == 1 && dead_f2_cpu == 0) )
    {   dead_f2_cpu = 1; }
    else if ( type == 'F' && cpu && (dead_f1_cpu == 0) )
    {   dead_f1_cpu = 1; }
    else if ( type == 'M' && cpu )
    {   dead_m1_cpu = 1; }
    else if ( type == 'R' && cpu )
    {   dead_r1_cpu = 1; }

}//-------- end of send_graveyard() ------------

/*
leave_graveyard()
Flag the piece as alive, and send back to the board into a random empty spot
*/
int leave_graveyard(Square board[])
{
      // FONTS -----
      FONT *smallfont;
      PALETTE palette;
      smallfont = load_font("fonts/smallfont.bmp",palette,NULL);
      //-----------------------------------

      char heal_it = 'x';
      int done = 0, random;

      srand(time(0));  //Randomize timer

      if ( (dead_f1==1 || dead_f2==1 || dead_f3==1) && dead_r1 == 1 )
      textout_ex(screen, smallfont, "<F>ootsoldier    <R>adio Engineer",140,420,makecol(150, 0, 0),-1);

      if ( (dead_f1==1 || dead_f2==1 || dead_f3==1) && dead_r1 == 0 )
      textout_ex(screen, smallfont, "<F>ootsoldier",140,420,makecol(150, 0, 0),-1);

      if ( dead_f1==0 && dead_f2==0 && dead_f3==0 && dead_r1 == 1 )
      textout_ex(screen, smallfont, "<R>adio Engineer",140,420,makecol(150, 0, 0),-1);

      if ( dead_f1==0 && dead_f2==0 && dead_f3==0 && dead_r1 == 0 )
      textout_ex(screen, smallfont, "No one is hurt.. oops.",140,420,makecol(150, 0, 0),-1);

   while ( heal_it == 'x' )
   {
      if (key[KEY_F] && (dead_f1==1 || dead_f2==1 || dead_f3==1))
      {
         heal_it = 'F';
      }

      if (key[KEY_R] && dead_r1==1)
      {
         heal_it = 'R';
      }

     if ( heal_it == 'R' || heal_it == 'F' )
     {
        if ( heal_it == 'F' && (dead_f3 == 1) )
       {   dead_f3 = 0; done = 1; }
       else if ( heal_it == 'F' && (dead_f1 == 1 && dead_f2 == 1) )
       {   dead_f2 = 0; done = 1;}
       else if ( heal_it == 'F' && (dead_f1 == 1) )
       {   dead_f1 = 0; done = 1; }
       else if ( heal_it == 'R' )
       {   dead_r1 = 0; done = 1; }
     }

       //randomly place the healed piece
      if ( done == 1)
      {
         do
         {   random = (rand()%59);
           if ( board[random].getType() == 'E' )
           {    board[random].setPiece(heal_it);
                break;
           }

         }while (board[random].getType() != 'E');

         total_score = total_score - 1; //Costs 1 pt. to heal a soldier
       }
   }

      destroy_font(smallfont);
      smallfont = 0;

}//-------- end of leave_graveyard() ------------

/* draw_board
Draw the gameboard to the screen
Input: The array of game board squares
Output: none
*/
void redraw(Square board[])
{
  int sq_num;
  COLOR_MAP global_light_table;
  set_trans_blender(130, 0, 130, 0);   //light for dead pieces

  BITMAP* tempframe;

      // FONTS
      FONT *smallfont, *myfont;
      PALETTE palette;
      smallfont = load_font("fonts/smallfont.bmp",palette,NULL);
      myfont = load_font("fonts/myfont.pcx",palette,NULL);
      //-----------------------------------

      tempframe = load_bitmap("images/boardframe.bmp",NULL);

     blit(tempframe,screen,400,400,400,400,200,80); //update score
     blit(tempframe,screen,1,1,1,1,50,450); //update sidebar graveyard

     //Draw Score
     textprintf_ex(screen, myfont, 400, 410, makecol(30, 150, 10),-1, "Player Score: %d", total_score);
     textprintf_ex(screen, myfont, 400, 430, makecol(150, 30, 10),-1, "Enemy Score: %d", total_score_cpu);

      //Game Statistics
     textprintf_ex(screen, smallfont, 580, 30, makecol(150, 30, 10),-1, "WINS: %d", total_wins);
     textprintf_ex(screen, smallfont, 580, 50, makecol(150, 30, 10),-1, "LOSES:%d", total_loses);

     for (sq_num = 0; sq_num < 60; sq_num++ )
     {
        //draw_sprite(screen,board[sq_num].getBmp(),board[sq_num].getX1(),board[sq_num].getY1());
       masked_blit(board[sq_num].getBmp(), screen, 0, 0, board[sq_num].getX1(), board[sq_num].getY1(), 47, 47);

       }

     //Draw graveyard on side board
     draw_graveyard();

  destroy_font(smallfont);
  destroy_font(myfont);
  destroy_bitmap(tempframe);
  smallfont = 0;
  myfont = 0;
  tempframe = 0;

}
// --------- END of redraw() ---------------------------------------

/*
check_board
Poll the gameboard squares
Input: mouse positions, Board squares
Output: none

void check_board(Square board[],int sq)
{

      while (subticks > 0) {
       rest(1);
      }
      subticks--;


            show_mouse(NULL);
            board[0].setActive(TRUE);
            show_mouse(screen);

      (board[0].getActive())
     {
          show_mouse(NULL);
          board[0].setActive(FALSE);
          show_mouse(screen);
     }


}
// ------- END of check_board ------------------------------------------
*/

/* isAdjacent()
Checks if two squares are adjacent to each other
Input: Locations of two squares
Output: TRUE/FALSE is adjacent */
bool isAdjacent(int b1,int b2)
{
  if ( b1 % 6 == 0 && (b2==5 || b2==11 || b2==17 || b2==23 || b2==29 || b2==35 || b2==41 || b2==47 || b2==53 || b2==59))
  return FALSE;
  if ( b2 % 6 == 0 && (b1==5 || b1==11 || b1==17 || b1==23 || b1==29 || b1==35 || b1==41 || b1==47 || b1==53 || b1==59))
  return FALSE;
  if (b1 == b2)
  return FALSE;
  if ( b1 < 0 || b2 < 0 )
  return FALSE;
  if ( b1 < 6 && (b2 == (b1+6) || b2 == (b1+1) || b2 == (b1-1)) )
  return TRUE;
   if ( b2 < 6 && (b1 == (b2+6) || b1 == (b2+1) || b1 == (b2-1)) )
  return TRUE;
   if ( b1 > 5 && b1 < 54 && (b2 == (b1+6) || b2 == (b1-6) || b2 == (b1-1) || b2 == (b1+1)) )
  return TRUE;
  if ( b2 > 5 && b2 < 54 && (b1 == (b2+6) || b1 == (b2-6) || b1 == (b2-1) || b1 == (b2+1)) )
  return TRUE;
  if ( b1 > 53 && (b2 == (b1-6) || b2 == (b1+1) || b2 == (b1-1)) )
  return TRUE;
  else
  return FALSE;
}
// ---------- end of isAdjacent --------------

/* --- get_square() --------------------------
Given x/y coordinates, determine and return the Game Square number (0-59)
-1 is returned if the coordinates are not on the Game board.
*/
int get_square(Square theboard[],int mx,int my)
{  int x = 0, y = -1;

   for (x = 0; x < 60; x++ )
   {
        if ( mx > theboard[x].getX1() && mx < theboard[x].getX1() + 50 )
        {  if ( my > theboard[x].getY1() && my < theboard[x].getY1() + 50 )
            y = x;
        }
    }
    return y;
}
// ------- end of get_square() ----------------

/* --- Update_pieces()
Given two squares the player clicks on, perform the game actions
*/
int update_pieces(Square* s1, Square* s2, Square board[])
{
     int heliloc, count = 0 , done = 0;

    //Move player piece to an empty square
     if ( done==0 && s2->getType()=='E' )
     {
            //Sound effect for empty spot placement
            sound_set = load_datafile_object("fsgame.dat","Snd50");
            play_sample((SAMPLE*)sound_set->dat,255,128,1000, 0);

            s2->setPlans( s1->getPlans() );  //set plans before piece to set right bitmap
            s2->setPiece( s1->getType() );

            s1->setPlans(FALSE);
            s1->setPiece('E');
            done = 1;
            //unload_datafile_object(sound_set);
            return 1;
     }

     //Player steals plans from the enemy Helipad
     if ( done==0 && s1->getType()=='F' && s2->getType()=='H' && s2->getPlans() && s2->getCpu() && !s1->getPlans() )
     {
          s1->setPlans(TRUE);
          s1->setPiece('F'); //update footsoldier bitmap mostly to show plans
          s2->setPlans(FALSE);
          done = 1;
          return 1;
     }

      //Player attacks enemy footsoldier holding stolen plans
     if ( done==0 && s1->getType()=='F' && s2->getType()=='F' && s2->getPlans() && s2->getCpu())
     {
          // changed to automatic win
          s2->setPlans(FALSE);    //remove plans from cpu
          s2->setPiece('F');    //update footsoldier bitmap mostly to show no plans

                //Play "death" sounds
               sound_set = load_datafile_object("fsgame.dat","Ak47");
               play_sample((SAMPLE*)sound_set->dat,255,0,1000, 0);
               sound_set2 = load_datafile_object("fsgame.dat","Agony");
               play_sample((SAMPLE*)sound_set2->dat,255,255,1000,0);

          done = 1;
          win = 1;  //If you kill the enemy with your plans, YOU WIN!

          total_score = total_score + 2;  //2 pts for stealing back plans
         // unload_datafile_object(sound_set);
         // unload_datafile_object(sound_set2);
          return 1;
     }

     //Player footsoldier attacks enemy piece
     if ( done==0 && s1->getType()=='F' && s2->getCpu() && (s2->getType()=='F' || s2->getType()=='R' || s2->getType()=='M') )
     {
          send_graveyard(s2->getType(),s2->getCpu());

          //Play "death" sounds
          sound_set = load_datafile_object("fsgame.dat","Ak47");
          play_sample((SAMPLE*)sound_set->dat,255,0,1000, 0);
          sound_set2 = load_datafile_object("fsgame.dat","Agony");
          play_sample((SAMPLE*)sound_set2->dat,255,255,1000,0);

           //Set the killed piece as the Player piece
          s2->setPlans( s1->getPlans() );
          s2->setCpu( s1->getCpu() );
          s2->setPiece( s1->getType() );
          //Set the Player piece as empty
          s1->setPlans(FALSE);
          s1->setCpu(FALSE);
          s1->setPiece('E');

          done = 1 ;

          total_score++;  //1pt. for killing enemy soldier
         // unload_datafile_object(sound_set);
         // unload_datafile_object(sound_set2);
          return 1;
     }
     //Escape with plans
     if ( done==0 && s1->getType()=='F' && !s1->isDead() && s1->getPlans() && !s2->getCpu() && s2->getType()=='H')
     {
         win = 1;

         total_score = total_score + 2; //2pts for stealing plans successfully.

         return 1;
     }

     return 0; //No updates done
}

//Player selects 'Move Helipad' option. b1 = radio engineer location
void update_helipad(Square board[], int b1)
{
      BITMAP *tmpFrame;

     // FONTS
      FONT *smallfont, *myfont;
      PALETTE palette;
      smallfont = load_font("fonts/smallfont.bmp",palette,NULL);
      myfont = load_font("fonts/myfont.pcx",palette,NULL);
      //-----------------------------------

  int wait = 0, b2 = -1, oldheli = -1, count = 0;

  tmpFrame = load_bitmap("images/boardframe.bmp",NULL);

  textout_ex(screen, smallfont, "'Copy.' <squack>",140,415,makecol(150,0,0),-1);
  textout_ex(screen, smallfont, "'Select new Helipad coordinates.'",140,425,makecol(150,0,0),-1);

  show_mouse(screen); //Show mouse

   while (wait == 0 )
  {
      if (mouse_b & 1)
      {
          b2 = get_square(board,mouse_x,mouse_y);

          while (mouse_b & 1){}; //do nothing, just wait till button is lifted

           if ( b2 > -1 ) //If the square clicked on is a valid game square
           {
              //and the valid square is empty and adjacent to the Radio Engineer
             if (isAdjacent(b1,b2) && board[b2].getType() == 'E')
             {
                   //Set old Helipad to an empty square
                   for ( count = 0; count < 60; count++ )
                   {   if ( board[count].getType() == 'H' && !board[count].getCpu())
                          oldheli = count;
                   }
                   board[b2].setPlans(board[oldheli].getPlans());
                   board[b2].setCpu(FALSE);
                   board[oldheli].setPlans(FALSE);
                   board[oldheli].setCpu(FALSE);
                   board[oldheli].setPiece('E');

                //Set empty square clicked on as the new Helipad
                board[b2].setPiece('H');
                wait = 1;
              }
            }
        }
      //Cancel action and go back to polling board
      if ( key[KEY_ESC] )
      break;
  }
   blit(tmpFrame,screen,80,400,80,400,250,120); //update message box

   //deallocate
   destroy_bitmap(tmpFrame);
   destroy_font(smallfont);
   destroy_font(myfont);
   tmpFrame = 0;
   smallfont = 0;
   myfont = 0;
}


/*
update_adjacent_cpu()
Update the actions between the CPU piece and the Player piece.
*/
void update_adjacent_cpu(Square board[],int cpubox, int pbox)
{
    int done = 0;

    //CPU footsoldier attacks enemy piece
    if ( done==0 && board[cpubox].getType()=='F')
     {
          if ( board[pbox].getPlans() ) //If catching the Player with plans, they lose
         { cpuwin = 1;
           total_score_cpu = total_score_cpu + 2;
         }

               //Play "death" sounds
               sound_set = load_datafile_object("fsgame.dat","50calmg");
               play_sample((SAMPLE*)sound_set->dat,255,0,1000, 0);
                sound_set2 = load_datafile_object("fsgame.dat","Agony");
               play_sample((SAMPLE*)sound_set2->dat,255,255,1000,0);

          //Send dead piece to the graveyard side board
          send_graveyard(board[pbox].getType(),board[pbox].getCpu());

          //Set new square as the CPU piece
          board[pbox].setPlans( board[cpubox].getPlans() );
          board[pbox].setCpu( board[cpubox].getCpu());
          board[pbox].setPiece( board[cpubox].getType() );
          //Set the CPU piece as empty
          board[cpubox].setPlans(FALSE);
          board[cpubox].setCpu(FALSE);
          board[cpubox].setPiece('E');

          if (!board[pbox].getPlans() )
          total_score_cpu++;  //1pt. for killing soldier

          done = 1 ;
     }
     //unload_datafile_object(sound_set);
     //unload_datafile_object(sound_set2);
}
// ----------------- end of update_adjacent_cpu() ------------------

/* end_game()
End of game function. Print winner message to the screen.
*/
void end_game()
{
      // FONTS
      FONT *smallfont, *myfont;
      PALETTE palette;
      smallfont = load_font("fonts/smallfont.bmp",palette,NULL);
      myfont = load_font("fonts/myfont.pcx",palette,NULL);
      //-----------------------------------

    if ( total_score > total_score_cpu )
    {
         sound_set = load_datafile_object("fsgame.dat","mission_suc");
         play_sample((SAMPLE*)sound_set->dat,255,50,1000,0);

        textout_ex(screen, myfont, "*** MISSION SUCCESS ***",100,420,makecol(0, 150, 0),-1);
        //unload_datafile_object(sound_set);
        total_wins = total_wins + 1;
    }

    if ( total_score <= total_score_cpu )
    {   sound_set2 = load_datafile_object("fsgame.dat","mission_fail");
        play_sample((SAMPLE*)sound_set2->dat,255,50,1000,0);

        textout_ex(screen, myfont, "*** MISSION FAILURE ***",100,420,makecol(150, 30, 30),-1);
       // unload_datafile_object(sound_set2);
        total_loses = total_loses + 1;

    }

     textout_ex(screen, smallfont, "[ESC]",210,460,makecol(150, 0, 0),-1);
     while (!key[KEY_ESC]) {};

     destroy_font(myfont);
     destroy_font(smallfont);
     myfont = 0;
     smallfont = 0;
}
// -------------------- end of end_game() -------------------

/* same_column()
Given two locations from the game board, returns
TRUE if the boxes lie in the same column
*/
bool same_column(int b1,int b2)
{
  if ( b1 >= 0 && b1 <= 5 && b2 >=0 && b2 <= 5)
  return TRUE;
    if ( b1 >= 6 && b1 <= 11 && b2 >=6 && b2 <= 11)
  return TRUE;
   if ( b1 >= 12 && b1 <= 17 && b2 >=12 && b2 <= 17)
  return TRUE;
   if ( b1 >= 18 && b1 <= 23 && b2 >=18 && b2 <= 23)
  return TRUE;
   if ( b1 >= 24 && b1 <= 29 && b2 >=24 && b2 <= 29)
  return TRUE;
   if ( b1 >= 30 && b1 <= 35 && b2 >=30 && b2 <= 35)
  return TRUE;
   if ( b1 >= 36 && b1 <= 41 && b2 >=36 && b2 <= 41)
  return TRUE;
   if ( b1 >= 42 && b1 <= 47 && b2 >=42 && b2 <= 47)
  return TRUE;
   if ( b1 >= 48 && b1 <= 53 && b2 >=48 && b2 <= 53)
  return TRUE;
  if ( b1 >= 54 && b1 <= 59 && b2 >=54 && b2 <= 59)
  return TRUE;

  return FALSE;  //Not in the same column
}
// ---------- End of same_column() ---------------

/*
go_towards()
Moves one piece towards a another piece on the board
Input: board[] - game board
       piece1  - 1st game piece
       piece2   - 2nd game piece
       pf1     - footsoldier1
       pf2     - footsoldier2
       pf3     - footsoldier3
       sacrifice  - 0 = won't allow sacrificing itself
                    1 = will allow sacrificing itself

Function will move the 1st game piece towards the 2nd game piece.
If sacrifice is TRUE, 1st game piece won't consider the 3 footsoldiers when moving
If sarifice is FALSE, 1st game piece WILL consider the 3 footsoldiers when moving
*/
int go_towards(Square board[], int piece1, int piece2, int pf1, int pf2, int pf3, int sacrifice )
{
    int count, done = 0;

    if ( sacrifice == 0 )
    {
       //Piece2 is below footsoldier in column
       if (done==0 && same_column(piece1,piece2) && piece2 > piece1 && isAdjacent(piece1+1,piece1))
       {
           if (board[piece1+1].getType() == 'E' && (!isAdjacent(piece1+1,pf1) && !isAdjacent(piece1+1,pf2) && !isAdjacent(piece1+1,pf3))) //If square below is empty, move down
          { board[piece1+1].setPlans( board[piece1].getPlans() );  //set plans before piece to set right bitmap
            board[piece1+1].setCpu(TRUE);
            board[piece1+1].setPiece( board[piece1].getType() );
            board[piece1].setPlans(FALSE);
            board[piece1].setCpu(FALSE);
            board[piece1].setPiece('E');
            done = 1;
            return 1;
          }
       }

         //Piece2 is above footsoldier in same column
       if (done == 0 && same_column(piece1,piece2) && piece2 < piece1 && isAdjacent(piece1-1,piece1))
       {
           if (board[piece1-1].getType() == 'E' && (!isAdjacent(piece1-1,pf1) && !isAdjacent(piece1-1,pf2) && !isAdjacent(piece1-1,pf3))) //If square above is empty, move up
          { board[piece1-1].setPlans( board[piece1].getPlans() );  //set plans before piece to set right bitmap
            board[piece1-1].setCpu(TRUE);
            board[piece1-1].setPiece( board[piece1].getType() );
            board[piece1].setPlans(FALSE);
            board[piece1].setCpu(FALSE);
            board[piece1].setPiece('E');
            done = 1;
            return 1;
          }
        }

      for (count = 6; count < 60; count+=6)
      {
         if (done==0 && same_column(piece1,piece2+count) && piece2+count == piece1 && isAdjacent(piece1-6,piece1))
         {
           if (board[piece1-6].getType() == 'E' && (!isAdjacent(piece1-6,pf1) && !isAdjacent(piece1-6,pf2) && !isAdjacent(piece1-6,pf3))) //If square below is empty, move down
          { board[piece1-6].setPlans( board[piece1].getPlans() );  //set plans before piece to set right bitmap
            board[piece1-6].setCpu(TRUE);
            board[piece1-6].setPiece( board[piece1].getType() );
            board[piece1].setPlans(FALSE);
            board[piece1].setCpu(FALSE);
            board[piece1].setPiece('E');
            done = 1;
            return 1;
          }
         }

         if (done==0 && same_column(piece1,piece2+count) && piece2+count > piece1 && isAdjacent(piece1+1,piece1))
         {
           if (board[piece1+1].getType() == 'E' && (!isAdjacent(piece1+1,pf1) && !isAdjacent(piece1+1,pf2) && !isAdjacent(piece1+1,pf3))) //If square below is empty, move down
          { board[piece1+1].setPlans( board[piece1].getPlans() );  //set plans before piece to set right bitmap
            board[piece1+1].setCpu(TRUE);
            board[piece1+1].setPiece( board[piece1].getType() );
            board[piece1].setPlans(FALSE);
            board[piece1].setCpu(FALSE);
            board[piece1].setPiece('E');
            done = 1;
            return 1;
          }
          else
          if (board[piece1+6].getType() == 'E' && (!isAdjacent(piece1+6,pf1) && !isAdjacent(piece1+6,pf2) && !isAdjacent(piece1+6,pf3))) //If square below is empty, move down
          { board[piece1+6].setPlans( board[piece1].getPlans() );  //set plans before piece to set right bitmap
            board[piece1+6].setCpu(TRUE);
            board[piece1+6].setPiece( board[piece1].getType() );
            board[piece1].setPlans(FALSE);
            board[piece1].setCpu(FALSE);
            board[piece1].setPiece('E');
            done = 1;
            return 1;
          }
         }

         if (done==0 && same_column(piece1,piece2+count) && piece2+count < piece1 && isAdjacent(piece1-1,piece1))
         {
           if (board[piece1-1].getType() == 'E' && (!isAdjacent(piece1-1,pf1) && !isAdjacent(piece1-1,pf2) && !isAdjacent(piece1-1,pf3))) //If square below is empty, move down
          { board[piece1-1].setPlans( board[piece1].getPlans() );  //set plans before piece to set right bitmap
            board[piece1-1].setCpu(TRUE);
            board[piece1-1].setPiece( board[piece1].getType() );
            board[piece1].setPlans(FALSE);
            board[piece1].setCpu(FALSE);
            board[piece1].setPiece('E');
            done = 1;
            return 1;
          }
          else
          if (board[piece1-6].getType() == 'E' && (!isAdjacent(piece1-6,pf1) && !isAdjacent(piece1-6,pf2) && !isAdjacent(piece1-6,pf3))) //If square below is empty, move down
          { board[piece1-6].setPlans( board[piece1].getPlans() );  //set plans before piece to set right bitmap
            board[piece1-6].setCpu(TRUE);
            board[piece1-6].setPiece( board[piece1].getType() );
            board[piece1].setPlans(FALSE);
            board[piece1].setCpu(FALSE);
            board[piece1].setPiece('E');
            done = 1;
            return 1;
          }
         }

         if (done==0 && same_column(piece1,piece2-count) && piece2-count == piece1 && isAdjacent(piece1+6,piece1))
         {
           if (board[piece1+6].getType() == 'E' && (!isAdjacent(piece1+6,pf1) && !isAdjacent(piece1+6,pf2) && !isAdjacent(piece1+6,pf3))) //If square below is empty, move down
          { board[piece1+6].setPlans( board[piece1].getPlans() );  //set plans before piece to set right bitmap
            board[piece1+6].setCpu(TRUE);
            board[piece1+6].setPiece( board[piece1].getType() );
            board[piece1].setPlans(FALSE);
            board[piece1].setCpu(FALSE);
            board[piece1].setPiece('E');
            done = 1;
            return 1;
          }
         }

         if (done==0 && same_column(piece1,piece2-count) && piece2-count > piece1 && isAdjacent(piece1+1,piece1))
         {
           if (board[piece1+1].getType() == 'E' && (!isAdjacent(piece1+1,pf1) && !isAdjacent(piece1+1,pf2) && !isAdjacent(piece1+1,pf3))) //If square below is empty, move down
          { board[piece1+1].setPlans( board[piece1].getPlans() );  //set plans before piece to set right bitmap
            board[piece1+1].setCpu(TRUE);
            board[piece1+1].setPiece( board[piece1].getType() );
            board[piece1].setPlans(FALSE);
            board[piece1].setCpu(FALSE);
            board[piece1].setPiece('E');
            done = 1;
            return 1;
          }
          else
          if (board[piece1+6].getType() == 'E' && (!isAdjacent(piece1+6,pf1) && !isAdjacent(piece1+6,pf2) && !isAdjacent(piece1+6,pf3))) //If square below is empty, move down
          { board[piece1+6].setPlans( board[piece1].getPlans() );  //set plans before piece to set right bitmap
            board[piece1+6].setCpu(TRUE);
            board[piece1+6].setPiece( board[piece1].getType() );
            board[piece1].setPlans(FALSE);
            board[piece1].setCpu(FALSE);
            board[piece1].setPiece('E');
            done = 1;
            return 1;
          }
         }

         if (done==0 && same_column(piece1,piece2-count) && piece2-count < piece1 && isAdjacent(piece1-1,piece1))
         {
           if (board[piece1-1].getType() == 'E' && (!isAdjacent(piece1-1,pf1) && !isAdjacent(piece1-1,pf2) && !isAdjacent(piece1-1,pf3))) //If square below is empty, move down
          { board[piece1-1].setPlans( board[piece1].getPlans() );  //set plans before piece to set right bitmap
            board[piece1-1].setCpu(TRUE);
            board[piece1-1].setPiece( board[piece1].getType() );
            board[piece1].setPlans(FALSE);
            board[piece1].setCpu(FALSE);
            board[piece1].setPiece('E');
            done = 1;
            return 1;
          }
          else
          if (board[piece1+6].getType() == 'E' && (!isAdjacent(piece1+6,pf1) && !isAdjacent(piece1+6,pf2) && !isAdjacent(piece1+6,pf3))) //If square below is empty, move down
          { board[piece1+6].setPlans( board[piece1].getPlans() );  //set plans before piece to set right bitmap
            board[piece1+6].setCpu(TRUE);
            board[piece1+6].setPiece( board[piece1].getType() );
            board[piece1].setPlans(FALSE);
            board[piece1].setCpu(FALSE);
            board[piece1].setPiece('E');
            done = 1;
            return 1;
          }
         }
      } //End of 1st FOR loop



    } // End of Non-Sacrifice loops
   /* -------------------------------------------------------------------- */

    // Go bravely good soldier, don't be such a woose!
    //Don't care if footsoldiers move into danger, they will sacrifice themselves if necessary
    if ( sacrifice == 1 )
    {
       //Piece2 is below soldier in column
       if (done==0 && same_column(piece1,piece2) && piece2 > piece1 && isAdjacent(piece1+1,piece1))
       {
           if (board[piece1+1].getType() == 'E') //If square below is empty, move down
          { board[piece1+1].setPlans( board[piece1].getPlans() );  //set plans before piece to set right bitmap
            board[piece1+1].setCpu(TRUE);
            board[piece1+1].setPiece( board[piece1].getType() );
            board[piece1].setPlans(FALSE);
            board[piece1].setCpu(FALSE);
            board[piece1].setPiece('E');
            done = 1;
            return 1;
          }
       }
         //Piece2 is above soldier in same column
       if (done == 0 && same_column(piece1,piece2) && piece2 < piece1 && isAdjacent(piece1-1,piece1))
       {
           if (board[piece1-1].getType() == 'E' ) //If square above is empty, move up
          { board[piece1-1].setPlans( board[piece1].getPlans() );  //set plans before piece to set right bitmap
            board[piece1-1].setCpu(TRUE);
            board[piece1-1].setPiece( board[piece1].getType() );
            board[piece1].setPlans(FALSE);
            board[piece1].setCpu(FALSE);
            board[piece1].setPiece('E');
            done = 1;
            return 1;
          }
       }

        for (count = 6; count < 60; count+=6)
       {
         if (done==0 && same_column(piece1,piece2+count) && piece2+count == piece1 && isAdjacent(piece1-6,piece1))
         {
           if (board[piece1-6].getType() == 'E' ) //If square below is empty, move down
          { board[piece1-6].setPlans( board[piece1].getPlans() );  //set plans before piece to set right bitmap
            board[piece1-6].setCpu(TRUE);
            board[piece1-6].setPiece( board[piece1].getType() );
            board[piece1].setPlans(FALSE);
            board[piece1].setCpu(FALSE);
            board[piece1].setPiece('E');
            done = 1;
            return 1;
          }
         }

         if (done==0 && same_column(piece1,piece2+count) && piece2+count > piece1 && isAdjacent(piece1+1,piece1))
         {
           if (board[piece1+1].getType() == 'E' ) //If square below is empty, move down
          { board[piece1+1].setPlans( board[piece1].getPlans() );  //set plans before piece to set right bitmap
            board[piece1+1].setCpu(TRUE);
            board[piece1+1].setPiece( board[piece1].getType() );
            board[piece1].setPlans(FALSE);
            board[piece1].setCpu(FALSE);
            board[piece1].setPiece('E');
            done = 1;
            return 1;
          }
         }
         if (done==0 && same_column(piece1,piece2+count) && piece2+count < piece1 && isAdjacent(piece1-1,piece1))
         {
           if (board[piece1-1].getType() == 'E' ) //If square below is empty, move down
          { board[piece1-1].setPlans( board[piece1].getPlans() );  //set plans before piece to set right bitmap
            board[piece1-1].setCpu(TRUE);
            board[piece1-1].setPiece( board[piece1].getType() );
            board[piece1].setPlans(FALSE);
            board[piece1].setCpu(FALSE);
            board[piece1].setPiece('E');
            done = 1;
            return 1;
          }
         }

         if (done==0 && same_column(piece1,piece2-count) && piece2-count == piece1 && isAdjacent(piece1+6,piece1))
         {
           if (board[piece1+6].getType() == 'E' ) //If square below is empty, move down
          { board[piece1+6].setPlans( board[piece1].getPlans() );  //set plans before piece to set right bitmap
            board[piece1+6].setCpu(TRUE);
            board[piece1+6].setPiece( board[piece1].getType() );
            board[piece1].setPlans(FALSE);
            board[piece1].setCpu(FALSE);
            board[piece1].setPiece('E');
            done = 1;
            return 1;
          }
         }

         if (done==0 && same_column(piece1,piece2-count) && piece2-count > piece1 && isAdjacent(piece1+1,piece1))
         {
           if (board[piece1+1].getType() == 'E' ) //If square below is empty, move down
          { board[piece1+1].setPlans( board[piece1].getPlans() );  //set plans before piece to set right bitmap
            board[piece1+1].setCpu(TRUE);
            board[piece1+1].setPiece( board[piece1].getType() );
            board[piece1].setPlans(FALSE);
            board[piece1].setCpu(FALSE);
            board[piece1].setPiece('E');
            done = 1;
            return 1;
          }
         }
         if (done==0 && same_column(piece1,piece2-count) && piece2-count < piece1 && isAdjacent(piece1-1,piece1))
         {
           if (board[piece1-1].getType() == 'E' ) //If square below is empty, move down
          { board[piece1-1].setPlans( board[piece1].getPlans() );  //set plans before piece to set right bitmap
            board[piece1-1].setCpu(TRUE);
            board[piece1-1].setPiece( board[piece1].getType() );
            board[piece1].setPlans(FALSE);
            board[piece1].setCpu(FALSE);
            board[piece1].setPiece('E');
            done = 1;
            return 1;
          }
         }
       } //End of 1st FOR loop

       for (count = 6; count < 60; count+=6)
       {
          if (done==0 && same_column(piece1,piece2+count) && isAdjacent(piece1-6,piece1))
         {
           if (board[piece1-6].getType() == 'E' ) //If square below is empty, move down
          { board[piece1-6].setPlans( board[piece1].getPlans() );  //set plans before piece to set right bitmap
            board[piece1-6].setCpu(TRUE);
            board[piece1-6].setPiece( board[piece1].getType() );
            board[piece1].setPlans(FALSE);
            board[piece1].setCpu(FALSE);
            board[piece1].setPiece('E');
            done = 1;
            return 1;
          }
         }

         if (done==0 && same_column(piece1,piece2-count) && isAdjacent(piece1+6,piece1))
         {
           if (board[piece1+6].getType() == 'E' ) //If square below is empty, move down
          { board[piece1+6].setPlans( board[piece1].getPlans() );  //set plans before piece to set right bitmap
            board[piece1+6].setCpu(TRUE);
            board[piece1+6].setPiece( board[piece1].getType() );
            board[piece1].setPlans(FALSE);
            board[piece1].setCpu(FALSE);
            board[piece1].setPiece('E');
            done = 1;
            return 1;
          }
        }

       } //End of 2nd FOR loop

    } // End of Sacrifice loops

      return 0;
}// --------------- End of go_towards() ---------------------

/* count_steps()
Counts how many steps it takes to get from one piece to an adjacent box of another piece
(Intially written to find which CPU footsoldier is closest to the Player's Helipad)
Input:
      board[] - gameboard
      f1      - 1st piece
      p_h1    - 2nd piece
      steps   - flag

Function is recursive
*/
int count_steps(Square board[],int f1, int p_h1, int steps)
{
    int done = 0, check = 0, col = 0;

    if (p_h1 < 0) //(aka. don't bother counting if the 2nd piece is dead)
    return -1;

    if (f1 < 0) //(aka. don't bother counting if the 1st piece is dead)
    return -1;

    if ( same_column(f1,p_h1) )
    {
         if ( p_h1 > f1 )
         {
            if (isAdjacent(f1,f1+1))
            { f1++; steps++;             }
            else if (isAdjacent(f1,f1+6))
            { f1=f1+6; steps++;                 }
            else if (isAdjacent(f1,f1-6))
            { f1=f1-6; steps++;                 }
            else if (isAdjacent(f1,f1-1))
            { f1=f1-1; steps++;                 }

            done = 1;
         }
         if ( p_h1 < f1 && done == 0 )
         {
            if (isAdjacent(f1,f1-1))
            { f1--; steps++;             }
            else if (isAdjacent(f1,f1+6))
            { f1=f1+6; steps++;                 }
            else if (isAdjacent(f1,f1-6))
            { f1=f1-6; steps++;                 }
            else if (isAdjacent(f1,f1+1))
            { f1=f1+1; steps++;                 }

            done = 1;
         }
     }

    if ( !same_column(f1,p_h1) && done == 0 )
    {
       //Find how far the Helipad column is from the Player's column
       for ( check = 6; check < 60; check=check+6)
       {
           if ( same_column(f1+check,p_h1) )  //Check right columns
           col = check;

           if ( same_column(f1-check,p_h1) )  //Check left columns
              col = check;
       }

       // ------------- end of column find code ----------------

        if ( p_h1 > f1 )
         {
            if (isAdjacent(f1,f1+6))
            { f1=f1+6; steps++;             }
            else if (p_h1 > f1+(col) && isAdjacent(f1,f1+1))
            { f1=f1+1; steps++;                 }
            else if (isAdjacent(f1,f1-1))
            { f1=f1-1; steps++;                 }
             else if (isAdjacent(f1,f1-6))
            { f1=f1-6; steps++;                 }

            done = 1;
         }
         if ( p_h1 < f1 && done == 0 )
         {
            if (isAdjacent(f1,f1-6))
            { f1=f1-6; steps++;             }
            else if (p_h1 > f1+(col) && isAdjacent(f1,f1+1))
            { f1=f1+1; steps++;              }
            else if (isAdjacent(f1,f1-1))
            { f1=f1-1; steps++;                 }
            else if (isAdjacent(f1,f1+6))
            { f1=f1+6; steps++;                 }

            done = 1;
         }
    }

   //Recursive function. Returns steps when an adjacent square to the
   //square is found
    if ( isAdjacent(f1,p_h1) || done == 0 )
       return steps;
    else
       steps = count_steps(board,f1,p_h1,steps);

    return steps;

}//-------- End of count_steps() ----------------------------

/* cpu_turn()
Logic for CPU taking its turn, Let's give this stupid arse computer a brain.
Forget the heart and courage... not that great, but oh well.
Input: Board square array
Output: Move CPU piece and update board
*/
void cpu_turn (Square board[])
{
     // FONTS
     FONT *smallfont, *myfont;
     PALETTE palette;
     smallfont = load_font("fonts/smallfont.bmp",palette,NULL);
     myfont = load_font("fonts/myfont.pcx",palette,NULL);
     // -----------------------------------

     int choice = 0, count = 0, done = 0;

     //Location markers for cpu pieces and player pieces
     int f1=-1,f2=-1,f3=-1,r1=-1,m1=-1,h1=-1,p_f1=-1,p_f2=-1,p_f3=-1,p_r1=-1,p_m1=-1,p_h1=-1;

     //Counting "steps" between pieces
     int f1_steps = -1, f2_steps = -1, f3_steps = -1;

     int heli_steps = -1, smallest = -1, direction = 0;

     //Counting "steps" between pieces
     int radio_steps_right=-1,radio_steps_left=-1,radio_steps_up=-1,radio_steps_down=-1;

     int priority = 0, random = -1, footsoldier = -1, num = 0;
     char soldier;

     srand(time(0));  //Randomize timer
     textout_ex(screen, smallfont, "Enemy prepares to move..",80,420,makecol(150, 0, 0),-1);
     rest(800);

     //Flag the dead pieces
    if (dead_f1_cpu == 1)    f1 = -2;
    if (dead_f2_cpu == 1)    f2 = -2;
    if (dead_f3_cpu == 1)    f3 = -2;
    if (dead_r1_cpu == 1)    r1 = -2;
    if (dead_m1_cpu == 1)    m1 = -2;
    if (dead_f1 == 1)        p_f1 = -2;
    if (dead_f2 == 1)        p_f2 = -2;
    if (dead_f3 == 1)        p_f3 = -2;
    if (dead_r1 == 1)        p_r1 = -2;
    if (dead_m1 == 1)        p_m1 = -2;
    //--------------------

    //Find locations for all the ALIVE pieces (dead pieces = -2)
    // CPU pieces- f1,f2,f3,m1,r1,h1   Player pieces- p_f1,p_f2,p_f3,p_m1,p_r1,p_h1
   for ( count = 0; count < 60; count++ )
   {
         if ( board[count].getType() == 'F')
         {
            if ( board[count].getCpu() )
            {
                if (f1 != -1 && f2 != -1 && f3 == -1)
                f3 = count;

                if (f1 != -1 && f2 == -1)
                f2 = count;

                if (f1 == -1)
                f1 = count;
            }
            if ( !board[count].getCpu() )
            {
                if (p_f1 != -1 && p_f2 != -1 && p_f3 == -1)
                p_f3 = count;

                if (p_f1 != -1 && p_f2 == -1)
                p_f2 = count;

                if (p_f1 == -1)
                p_f1 = count;
            }
        }

        if ( board[count].getType() == 'M')
        {
           if ( board[count].getCpu() )
              m1 = count;
           else
              p_m1 = count;
        }

        if ( board[count].getType() == 'R')
        {
           if ( board[count].getCpu() )
              r1 = count;
           else
              p_r1 = count;
        }

         if ( board[count].getType() == 'H')
        {
           if ( board[count].getCpu() )
              h1 = count;
           else
              p_h1 = count;
        }
     }// ---- end of find piece locations ------

    // CPU Footsoldier next to CPU helipad with plans, wins the game!
    if ( done==0 && board[f1].getPlans() && isAdjacent(f1,h1))
    {
       cpuwin = 1;
       total_score_cpu = total_score_cpu + 2;
       done = 1;    }

    if ( done==0 && board[f2].getPlans() && isAdjacent(f2,h1))
    {
       cpuwin = 1;
       total_score_cpu = total_score_cpu + 2;
       done = 1;    }

    if ( done==0 && board[f3].getPlans() && isAdjacent(f3,h1))
    {
       cpuwin = 1;
       total_score_cpu = total_score_cpu + 2;
       done = 1;    }
    // ------------- end of Footsoldier wins via Helipad -----------------------

     //Footsoldier next to the Player piece
    if ( done == 0 && isAdjacent(f1,p_f1) )
    {
     update_adjacent_cpu(board,f1,p_f1);
     done = 1;
    }
    if ( done == 0 && isAdjacent(f1,p_f2) )
    {
     update_adjacent_cpu(board,f1,p_f2);
     done = 1;
    }
    if ( done == 0 && isAdjacent(f1,p_f3) )
    {
     update_adjacent_cpu(board,f1,p_f3);
     done = 1;
    }
    if ( done == 0 && isAdjacent(f1,p_m1) && !isAdjacent(p_f3,p_m1) && !isAdjacent(p_f1,p_m1) && !isAdjacent(p_f2,p_m1))
    {
     update_adjacent_cpu(board,f1,p_m1);
     done = 1;
    }
    if ( done == 0 && isAdjacent(f1,p_r1) && !isAdjacent(p_f3,p_r1) && !isAdjacent(p_f1,p_r1) && !isAdjacent(p_f2,p_r1))
    {
     update_adjacent_cpu(board,f1,p_r1);
     done = 1;
    }
    if ( done == 0 && isAdjacent(f2,p_f1) )
    {
     update_adjacent_cpu(board,f2,p_f1);
     done = 1;
    }
    if ( done == 0 && isAdjacent(f2,p_f2) )
    {
     update_adjacent_cpu(board,f2,p_f2);
     done = 1;
    }
    if ( done == 0 && isAdjacent(f2,p_f3) )
    {
     update_adjacent_cpu(board,f2,p_f3);
     done = 1;
    }
    if ( done == 0 && isAdjacent(f2,p_m1) && !isAdjacent(p_f3,p_m1) && !isAdjacent(p_f1,p_m1) && !isAdjacent(p_f2,p_m1))
    {
     update_adjacent_cpu(board,f2,p_m1);
     done = 1;
    }
    if ( done == 0 && isAdjacent(f2,p_r1) && !isAdjacent(p_f3,p_r1) && !isAdjacent(p_f1,p_r1) && !isAdjacent(p_f2,p_r1))
    {
     update_adjacent_cpu(board,f2,p_r1);
     done = 1;
    }
    if ( done == 0 && isAdjacent(f3,p_f1) )
    {
     update_adjacent_cpu(board,f3,p_f1);
     done = 1;
    }
    if ( done == 0 && isAdjacent(f3,p_f2) )
    {
     update_adjacent_cpu(board,f3,p_f2);
     done = 1;
    }
    if ( done == 0 && isAdjacent(f3,p_f3) )
    {
     update_adjacent_cpu(board,f3,p_f3);
     done = 1;
    }
    if ( done == 0 && isAdjacent(f3,p_m1) && !isAdjacent(p_f3,p_m1) && !isAdjacent(p_f1,p_m1) && !isAdjacent(p_f2,p_m1))
    {
     update_adjacent_cpu(board,f3,p_m1);
     done = 1;
    }
    if ( done == 0 && isAdjacent(f3,p_r1) && !isAdjacent(p_f3,p_r1) && !isAdjacent(p_f1,p_r1) && !isAdjacent(p_f2,p_r1))
    {
     update_adjacent_cpu(board,f3,p_r1);
     done = 1;
    }
    // ---------- End of Footsoldier attacks Piece ---------------

    //Footsoldier next to Player helipad without plans
    if ( done==0 && isAdjacent(f1,p_h1) && board[p_h1].getPlans())
     {
          board[f1].setPlans(TRUE);
          board[f1].setPiece('F'); //redraw/update footsoldier bitmap to show plans
          board[p_h1].setPlans(FALSE);    //No more plans for Player helipad heh heh
          done = 1;
     }
    if ( done==0 && isAdjacent(f2,p_h1) && board[p_h1].getPlans())
     {
          board[f2].setPlans(TRUE);
          board[f2].setPiece('F'); //redraw/update footsoldier bitmap to show plans
          board[p_h1].setPlans(FALSE);    //No more plans for Player helipad heh heh
          done = 1;
     }
    if ( done==0 && isAdjacent(f3,p_h1) && board[p_h1].getPlans())
     {
          board[f3].setPlans(TRUE);
          board[f3].setPiece('F'); //redraw/update footsoldier bitmap to show plans
          board[p_h1].setPlans(FALSE);    //No more plans for Player helipad heh heh
          done = 1;
     } // ------------------- End of Footsoldier steals plans from helipad -------------


    //If CPU already has the plans get moving towards the CPU Helipad
    //But first check if the Radio Engineer should move the Helipad so its closer
    if ( done == 0 && !board[p_h1].getPlans() )
    {
        //If Radio Engineer is still alive, check if moving Helipad to footsoldier would be closer
        if ( dead_r1_cpu == 0 )
        {
           if (dead_f1_cpu == 0 && board[f1].getPlans())
           {footsoldier = f1; heli_steps = count_steps(board,f1,h1,0);}
           else if (dead_f2_cpu == 0 && board[f2].getPlans())
           {footsoldier = f2; heli_steps = count_steps(board,f2,h1,0);}
           else if (dead_f3_cpu == 0 && board[f3].getPlans())
           {footsoldier = f3; heli_steps = count_steps(board,f3,h1,0);}

           if (board[r1+6].getType() == 'E' && isAdjacent(r1,r1+6) )
           radio_steps_right = count_steps(board,footsoldier,r1+6,0);

           if (board[r1-6].getType() == 'E' && isAdjacent(r1,r1-6) )
           radio_steps_left = count_steps(board,footsoldier,r1-6,0);

           if (board[r1-1].getType() == 'E' && isAdjacent(r1,r1-1 ) )
           radio_steps_up = count_steps(board,footsoldier,r1-1,0);

           if (board[r1+1].getType() == 'E' && isAdjacent(r1,r1+1) )
           radio_steps_down = count_steps(board,footsoldier,r1+1,0);

           if (radio_steps_right != -1 && (radio_steps_right <= radio_steps_left || radio_steps_left == -1) && (radio_steps_right <= radio_steps_up || radio_steps_up==-1)  && (radio_steps_right <= radio_steps_down || radio_steps_down==-1))
           {smallest = radio_steps_right; direction = 6;}
           else if (radio_steps_left != -1 && (radio_steps_left <= radio_steps_right || radio_steps_right==-1) && (radio_steps_left <= radio_steps_up || radio_steps_up==-1) && (radio_steps_left <= radio_steps_down || radio_steps_down==-1))
           {smallest = radio_steps_left; direction = -6;}
           else if (radio_steps_up != -1 && (radio_steps_up <= radio_steps_left || radio_steps_left==-1) && (radio_steps_up <= radio_steps_right || radio_steps_right==-1) && (radio_steps_up <= radio_steps_down || radio_steps_down==-1))
           {smallest = radio_steps_up; direction = -1;}
           else if (radio_steps_down != -1 && (radio_steps_down <= radio_steps_left || radio_steps_left==-1) && (radio_steps_down <= radio_steps_up || radio_steps_up==-1) && (radio_steps_down <= radio_steps_right || radio_steps_right==-1))
           {smallest = radio_steps_down; direction = 1;}

           //If radio engineer is closer to helipad
           if (heli_steps > smallest && direction != 0 )
           {  //Play helicopter sound effect
               sound_set = load_datafile_object("fsgame.dat","Copter1");
               play_sample((SAMPLE*)sound_set->dat,255,128,1000,0);
             board[r1+direction].setPlans(board[h1].getPlans());
             board[h1].setPlans(FALSE);
             board[h1].setCpu(FALSE);
             board[h1].setPiece('E');
            //Set empty square as the new Helipad
             board[r1+direction].setCpu(TRUE);
             board[r1+direction].setPiece('H');

             done = 1;
             //unload_datafile_object(sound_set);
           }
          }

       //If not moving the Helipad this turn, start moving the stolen plans towards the Helipad
       if (done == 0)
       { if (dead_f1_cpu == 0 && board[f1].getPlans())
        {done = go_towards(board,f1,h1,p_f1,p_f2,p_f3,0);
        //debug
        // textprintf_ex(screen, smallfont,80,400, makecol(200, 200, 100),-1, "F1 plans-done: %d", done);
        }
        else if (dead_f2_cpu == 0 && board[f2].getPlans())
        {done = go_towards(board,f2,h1,p_f1,p_f2,p_f3,0);
        //debug
       // textprintf_ex(screen, smallfont,80,410, makecol(200, 200, 100),-1, "F2 plans-done: %d", done);
         }
        else if (dead_f3_cpu == 0 && board[f3].getPlans())
        {done = go_towards(board,f3,h1,p_f1,p_f2,p_f3,0);
        //debug
       // textprintf_ex(screen, smallfont,80,420, makecol(200, 200, 100),-1, "F3 plans-done: %d", done);
        }
       }
    }
   // --------------- End of Footsoldier with plans going to Helipad

    //Player footsoldier is next to CPU helipad, Radio Eng. is alive, move da thing!
    if ( done==0 && board[h1].getPlans() && dead_r1_cpu == 0 && ( isAdjacent(p_f1,h1) || isAdjacent(p_f2,h1) || isAdjacent(p_f3,h1)))
    {
         int newheli = -1;

         //Find empty space next to CPU radio engineer (if exists)
         if ( board[r1+1].getType() == 'E' && isAdjacent(r1+1,r1) && (!isAdjacent(r1+1,p_f1) && !isAdjacent(r1+1,p_f2) && !isAdjacent(r1+1,p_f3)))
             newheli = r1+1;
         else if (board[r1-1].getType() == 'E' && isAdjacent(r1-1,r1) && (!isAdjacent(r1-1,p_f1) && !isAdjacent(r1-1,p_f2) && !isAdjacent(r1-1,p_f3)))
             newheli = r1-1;
         else if (board[r1-6].getType() == 'E' && isAdjacent(r1-6,r1) && (!isAdjacent(r1-6,p_f1) && !isAdjacent(r1-6,p_f2) && !isAdjacent(r1-6,p_f3)))
             newheli = r1-6;
         else if (board[r1+6].getType() == 'E' && isAdjacent(r1+6,r1) && (!isAdjacent(r1+6,p_f1) && !isAdjacent(r1+6,p_f2) && !isAdjacent(r1+6,p_f3)))
             newheli = r1+6;

         if ( newheli != -1 )
         {   //Play helicopter sound effect
             sound_set = load_datafile_object("fsgame.dat","Copter1");
             play_sample((SAMPLE*)sound_set->dat,255,128,1000,0);
             board[newheli].setPlans(board[h1].getPlans());
             board[h1].setPlans(FALSE);
             board[h1].setCpu(FALSE);
             board[h1].setPiece('E');
            //Set empty square as the new Helipad
             board[newheli].setCpu(TRUE);
             board[newheli].setPiece('H');

             done = 1;
            // unload_datafile_object(sound_set);
          }

    }// -------- end of Move the CPU Helipad ------------

     //If CPU still hasn't stolen the plans -------------
    //Move the closest footsoldier towards the Player's Helipad
    if ( done == 0 && board[p_h1].getPlans() )
    {
        //Count steps between footsoldiers and the Player Helipad
        if (!isAdjacent(f1,p_h1) && dead_f1_cpu == 0)
        f1_steps = count_steps(board,f1,p_h1,0);

        if (!isAdjacent(f2,p_h1) && dead_f2_cpu == 0)
        f2_steps = count_steps(board,f2,p_h1,0);

        if (!isAdjacent(f3,p_h1) && dead_f3_cpu == 0)
        f3_steps = count_steps(board,f3,p_h1,0);

        if (done==0 && f1_steps > 0 && (f1_steps <= f2_steps || f2_steps==-1) && (f1_steps <= f3_steps || f3_steps==-1))
        {
           done = go_towards(board,f1,p_h1,p_f1,p_f2,p_f3,0);

           if (done==0 && f2_steps > 0 && (f2_steps <= f3_steps || f3_steps == -1))
          {
            done = go_towards(board,f2,p_h1,p_f1,p_f2,p_f3,0);

          }
          if (done == 0 && dead_f3_cpu == 0)
            done = go_towards(board,f3,p_h1,p_f1,p_f2,p_f3,0);
        }
        if (done==0 && f2_steps > 0 && (f2_steps <= f1_steps || f1_steps == -1) && (f2_steps <= f3_steps || f3_steps == -1))
        {
           done = go_towards(board,f2,p_h1,p_f1,p_f2,p_f3,0);

            if (done==0 && f1_steps > 0 && (f1_steps <= f3_steps || f3_steps == -1))
          {
            done = go_towards(board,f1,p_h1,p_f1,p_f2,p_f3,0);
          }
          if (done == 0 && dead_f3_cpu == 0)
            done = go_towards(board,f3,p_h1,p_f1,p_f2,p_f3,0);
        }
        if (done==0 && f3_steps > 0 && (f3_steps <= f1_steps || f1_steps == -1) && (f3_steps <= f2_steps || f2_steps == -1))
       {
            done = go_towards(board,f3,p_h1,p_f1,p_f2,p_f3,0);

            if (done==0 && f1_steps > 0 && (f1_steps <= f2_steps || f2_steps == -1))
          {
            done = go_towards(board,f1,p_h1,p_f1,p_f2,p_f3,0);
          }
          if (done == 0 && dead_f2_cpu == 0)
            done = go_towards(board,f2,p_h1,p_f1,p_f2,p_f3,0);
       }
        //Debug - show steps between soldiers and Helipad
       // textprintf_ex(screen,smallfont,80,430,255,-1, "F1: %d  F2: %d  F3: %d",f1_steps,f2_steps,f3_steps);
      //  while ( !key[KEY_SPACE] ) {}
    }

    //CPU Medic heals a soldier if a soldier is dead and the Medic is alive
    if ( done == 0 && (dead_f1_cpu == 1 || dead_f2_cpu == 1 || dead_f3_cpu == 1 || dead_r1_cpu == 1) && dead_m1_cpu == 0)
    {
       //Don't let CPU heal if there score is zero, or the Player doesn't have a high score
       if ( total_score_cpu > 0 || total_score >= 4 )
       {
          //Play Happy Heal Sigh
          sound_set = load_datafile_object("fsgame.dat","Sigh");
          play_sample((SAMPLE*)sound_set->dat,255,128,1000,0);

        if (dead_f1_cpu == 1)
           {  dead_f1_cpu = 0; soldier = 'F'; }
           else if (dead_f2_cpu == 1)
           {  dead_f2_cpu = 0; soldier = 'F'; }
           else if (dead_f3_cpu == 1)
           {  dead_f3_cpu = 0; soldier = 'F'; }
           else if (dead_r1_cpu == 1)
           {  dead_r1_cpu = 0; soldier = 'R'; }

         //randomly place the healed piece
         do
         {   random = (rand()%59);
           if ( board[random].getType() == 'E' )
           {
                board[random].setCpu(TRUE);
                board[random].setPiece(soldier);
                break;
           }

         }while (board[random].getType() != 'E');

         total_score_cpu-- ;  //Subtract 1pt to heal

        done = 1;
       }

     }// ----- end of CPU Medic healing --------------


    //Footsoldier with stolen plans is stuck (can't move for some reason)
    //Get some help (if available) by sending other footsoldiers towards friend
    if ( done == 0 && !board[p_h1].getPlans() )
    {
        f1_steps = -1;
        f2_steps = -1;
        f3_steps = -1;

          if (board[f1].getPlans() && dead_f2_cpu == 0)
          {
              f1_steps = count_steps(board,f1,p_f1,0);
               f2_steps = count_steps(board,f1,p_f2,0);
                f3_steps = count_steps(board,f1,p_f3,0);

             if (f1_steps > -1 && (f1_steps <= f2_steps || f2_steps==-1) && (f1_steps <= f3_steps || f3_steps==-1) )
             done = go_towards(board,f2,p_f1,p_f1,p_f2,p_f3,1);
             else if (f2_steps > -1 && (f2_steps <= f1_steps || f1_steps==-1) && (f2_steps <= f3_steps || f3_steps==-1))
             done = go_towards(board,f2,p_f2,p_f1,p_f2,p_f3,1);
             else if (f3_steps > -1 && (f3_steps <= f2_steps || f2_steps==-1) && (f3_steps <= f1_steps || f1_steps==-1))
             done = go_towards(board,f2,p_f3,p_f1,p_f2,p_f3,1);
          }
          if ( done == 0 && board[f1].getPlans() && dead_f3_cpu == 0 )
          {
              f1_steps = count_steps(board,f1,p_f1,0);
               f2_steps = count_steps(board,f1,p_f2,0);
                f3_steps = count_steps(board,f1,p_f3,0);

             if (f1_steps > -1 && (f1_steps <= f2_steps || f2_steps==-1) && (f1_steps <= f3_steps || f3_steps==-1))
             done = go_towards(board,f3,p_f1,p_f1,p_f2,p_f3,1);
             else if (f2_steps > -1 && (f2_steps <= f1_steps || f1_steps==-1) && (f2_steps <= f3_steps || f3_steps==-1))
             done = go_towards(board,f3,p_f2,p_f1,p_f2,p_f3,1);
             else if (f3_steps > -1 && (f3_steps <= f2_steps || f2_steps==-1) && (f3_steps <= f1_steps || f1_steps==-1))
             done = go_towards(board,f3,p_f3,p_f1,p_f2,p_f3,1);

          }
          if ( done == 0 && board[f2].getPlans() && dead_f1_cpu == 0 )
          {
                f1_steps = count_steps(board,f2,p_f1,0);
               f2_steps = count_steps(board,f2,p_f2,0);
                f3_steps = count_steps(board,f2,p_f3,0);

             if (f1_steps > -1 && (f1_steps <= f2_steps || f2_steps==-1) && (f1_steps <= f3_steps || f3_steps==-1))
             done = go_towards(board,f1,p_f1,p_f1,p_f2,p_f3,1);
             else if (f2_steps > -1 && (f2_steps <= f1_steps || f1_steps==-1) && (f2_steps <= f3_steps || f3_steps==-1))
             done = go_towards(board,f1,p_f2,p_f1,p_f2,p_f3,1);
             else if (f3_steps > -1 && (f3_steps <= f2_steps || f2_steps==-1)  && (f3_steps <= f1_steps || f1_steps==-1))
             done = go_towards(board,f1,p_f3,p_f1,p_f2,p_f3,1);

          }
          if ( done == 0 && board[f3].getPlans() && dead_f1_cpu == 0)
          {
                f1_steps = count_steps(board,f3,p_f1,0);
               f2_steps = count_steps(board,f3,p_f2,0);
                f3_steps = count_steps(board,f3,p_f3,0);

             if (f1_steps > -1 && (f1_steps <= f2_steps || f2_steps==-1) && (f1_steps <= f3_steps || f3_steps==-1))
             done = go_towards(board,f1,p_f1,p_f1,p_f2,p_f3,1);
             else if (f2_steps > -1 && (f2_steps <= f1_steps || f1_steps==-1) && (f2_steps <= f3_steps || f3_steps==-1))
             done = go_towards(board,f1,p_f2,p_f1,p_f2,p_f3,1);
             else if (f3_steps > -1 && (f3_steps <= f2_steps || f2_steps==-1) && (f3_steps <= f1_steps || f1_steps==-1))
             done = go_towards(board,f1,p_f3,p_f1,p_f2,p_f3,1);
          }
          if ( done == 0 && board[f3].getPlans() && dead_f2_cpu == 0 )
          {
              f1_steps = count_steps(board,f3,p_f1,0);
               f2_steps = count_steps(board,f3,p_f2,0);
                f3_steps = count_steps(board,f3,p_f3,0);

             if (f1_steps > -1 && (f1_steps <= f2_steps || f2_steps==-1) && (f1_steps <= f3_steps || f3_steps==-1))
             done = go_towards(board,f2,p_f1,p_f1,p_f2,p_f3,1);
             else if (f2_steps > -1 && (f2_steps <= f1_steps || f1_steps==-1) && (f2_steps <= f3_steps || f3_steps==-1))
             done = go_towards(board,f2,p_f2,p_f1,p_f2,p_f3,1);
             else if (f3_steps > -1 && (f3_steps <= f2_steps || f2_steps==-1) && (f3_steps <= f1_steps || f1_steps==-1))
             done = go_towards(board,f2,p_f3,p_f1,p_f2,p_f3,1);
         }
          if ( done == 0 && board[f2].getPlans() && dead_f3_cpu == 0 )
          {
               f1_steps = count_steps(board,f2,p_f1,0);
               f2_steps = count_steps(board,f2,p_f2,0);
                f3_steps = count_steps(board,f2,p_f3,0);

             if (f1_steps > -1 && (f1_steps <= f2_steps || f2_steps==-1) && (f1_steps <= f3_steps || f3_steps==-1))
             done = go_towards(board,f3,p_f1,p_f1,p_f2,p_f3,1);
             else if (f2_steps > -1 && (f2_steps <= f1_steps || f1_steps==-1) && (f2_steps <= f3_steps || f3_steps==-1))
             done = go_towards(board,f3,p_f2,p_f1,p_f2,p_f3,1);
             else if (f3_steps > -1 && (f3_steps <= f2_steps || f2_steps==-1) && (f3_steps <= f1_steps || f1_steps==-1))
             done = go_towards(board,f3,p_f3,p_f1,p_f2,p_f3,1);
          }

     }
   // --------------- end of footsoldier help --------------

   //If Player has stolen plans. Move closest footsoldier (that isn't holding stolen plans)
   //towards the Player in hopes of catching them
   if ( done == 0 && (board[p_f1].getPlans() || board[p_f2].getPlans() || board[p_f3].getPlans()) )
   {
        footsoldier = -1;
        f1_steps = -1;
        f2_steps = -1;
        f3_steps = -1;

        if (dead_f1_cpu == 0 && board[p_f1].getPlans())
           footsoldier = p_f1;
        else if ( dead_f2_cpu == 0 && board[p_f2].getPlans())
           footsoldier = p_f2;
        else if (dead_f3_cpu == 0 && board[p_f3].getPlans())
           footsoldier = p_f3;

       if (footsoldier != -1 )
       {
           if ( dead_f1_cpu == 0 && !board[f1].getPlans() )
           f1_steps = count_steps(board,f1,footsoldier,0);
           if ( dead_f2_cpu == 0 && !board[f2].getPlans() )
           f2_steps = count_steps(board,f2,footsoldier,0);
           if ( dead_f3_cpu == 0 && !board[f3].getPlans() )
           f3_steps = count_steps(board,f3,footsoldier,0);

          if ( f1_steps > -1 && (f1_steps <= f2_steps || f2_steps ==-1) && (f1_steps <= f3_steps || f3_steps == -1)  )
          done = go_towards(board,f1,footsoldier,p_f1,p_f2,p_f3,1);
          if ( done == 0 && f2_steps > -1 && (f2_steps <= f1_steps || f1_steps ==-1) && (f2_steps <= f3_steps || f3_steps == -1)  )
          done = go_towards(board,f2,footsoldier,p_f1,p_f2,p_f3,1);
          if ( done == 0 && f3_steps > -1 && (f3_steps <= f2_steps || f2_steps ==-1) && (f3_steps <= f1_steps || f1_steps == -1)  )
          done = go_towards(board,f3,footsoldier,p_f1,p_f2,p_f3,1);
       }

    } // --- END of Go After Player with Plans -------


     //Player footsoldier is next to CPU Medic, move the poor dumb soldier
    if ( done==0 && ( isAdjacent(p_f1,m1) || isAdjacent(p_f2,m1) || isAdjacent(p_f3,m1) ) )
    {
         int newheli = -1;

         //Find empty space next to CPU radio engineer (if exists)
         if ( board[m1+1].getType() == 'E' && isAdjacent(m1+1,m1) && (!isAdjacent(m1+1,p_f1) && !isAdjacent(m1+1,p_f2) && !isAdjacent(m1+1,p_f3)))
             newheli = m1+1;
         else if (board[m1-1].getType() == 'E' && isAdjacent(m1-1,m1) && (!isAdjacent(m1-1,p_f1) && !isAdjacent(m1-1,p_f2) && !isAdjacent(m1-1,p_f3)))
             newheli = m1-1;
         else if (board[m1-6].getType() == 'E' && isAdjacent(m1-6,m1) && (!isAdjacent(m1-6,p_f1) && !isAdjacent(m1-6,p_f2) && !isAdjacent(m1-6,p_f3)))
             newheli = m1-6;
         else if (board[m1+6].getType() == 'E' && isAdjacent(m1+6,m1) && (!isAdjacent(m1+6,p_f1) && !isAdjacent(m1+6,p_f2) && !isAdjacent(m1+6,p_f3)))
             newheli = m1+6;

         if ( newheli != -1 )
         {
             //Set empty square as the new Medic
             board[newheli].setCpu(TRUE);
             board[newheli].setPiece('M');
             board[newheli].setPlans(board[m1].getPlans());
             board[m1].setPlans(FALSE);
             board[m1].setCpu(FALSE);
             board[m1].setPiece('E');


             done = 1;
          }
    }// -------- end of Move the Medic when in trouble  -------------------

     //Player footsoldier is next to CPU Radio Engineer, move the  dumb soldier -----------------
    if ( done==0 && ( isAdjacent(p_f1,r1) || isAdjacent(p_f2,r1) || isAdjacent(p_f3,r1) ) )
    {
         int newheli = -1;

         //Find empty space next to CPU radio engineer (if exists)
         if ( board[r1+1].getType() == 'E' && isAdjacent(r1+1,m1) && (!isAdjacent(r1+1,p_f1) && !isAdjacent(r1+1,p_f2) && !isAdjacent(r1+1,p_f3)))
             newheli = r1+1;
         else if (board[r1-1].getType() == 'E' && isAdjacent(r1-1,r1) && (!isAdjacent(r1-1,p_f1) && !isAdjacent(r1-1,p_f2) && !isAdjacent(r1-1,p_f3)))
             newheli = r1-1;
         else if (board[r1-6].getType() == 'E' && isAdjacent(r1-6,r1) && (!isAdjacent(r1-6,p_f1) && !isAdjacent(r1-6,p_f2) && !isAdjacent(r1-6,p_f3)))
             newheli = r1-6;
         else if (board[r1+6].getType() == 'E' && isAdjacent(r1+6,r1) && (!isAdjacent(r1+6,p_f1) && !isAdjacent(r1+6,p_f2) && !isAdjacent(r1+6,p_f3)))
             newheli = r1+6;

         if ( newheli != -1 )
         {
             board[newheli].setPlans(board[r1].getPlans());
             board[r1].setPlans(FALSE);
             board[r1].setCpu(FALSE);
             board[r1].setPiece('E');
            //Set empty square as the new Medic
             board[newheli].setCpu(TRUE);
             board[newheli].setPiece('R');

             done = 1;
          }
    }// -------- end of Move the Radio Engineer when in trouble  -------------------

   //Move the Radio Engineer in a random direction that isn't next to a footsoldier of course
   if (done == 0 && dead_r1_cpu == 0)
   {
      num = (rand()%20);

      if ( num < 5 )
      {
         if (board[r1+6].getType() == 'E' && isAdjacent(r1+6,r1) && (!isAdjacent(r1+6,p_f1) && !isAdjacent(r1+6,p_f2) && !isAdjacent(r1+6,p_f3))) //If square below is empty, move down
         { board[r1+6].setPlans( board[r1].getPlans() );  //set plans before piece to set right bitmap
           board[r1+6].setCpu(TRUE);
           board[r1+6].setPiece( board[r1].getType() );
           board[r1].setPlans(FALSE);
           board[r1].setCpu(FALSE);
           board[r1].setPiece('E');
           done = 1;
          }
          else if (board[r1-6].getType() == 'E' && isAdjacent(r1-6,r1) && (!isAdjacent(r1-6,p_f1) && !isAdjacent(r1-6,p_f2) && !isAdjacent(r1-6,p_f3))) //If square below is empty, move down
         { board[r1-6].setPlans( board[r1].getPlans() );  //set plans before piece to set right bitmap
           board[r1-6].setCpu(TRUE);
           board[r1-6].setPiece( board[r1].getType() );
           board[r1].setPlans(FALSE);
           board[r1].setCpu(FALSE);
           board[r1].setPiece('E');
           done = 1;
          }
      }

      if ( num >=5 && num < 10 )
      {
         if (board[r1-6].getType() == 'E' && isAdjacent(r1-6,r1) && (!isAdjacent(r1-6,p_f1) && !isAdjacent(r1-6,p_f2) && !isAdjacent(r1-6,p_f3))) //If square below is empty, move down
         { board[r1-6].setPlans( board[r1].getPlans() );  //set plans before piece to set right bitmap
           board[r1-6].setCpu(TRUE);
           board[r1-6].setPiece( board[r1].getType() );
           board[r1].setPlans(FALSE);
           board[r1].setCpu(FALSE);
           board[r1].setPiece('E');
           done = 1;
          }
      }

      if ( num >= 10 && num < 15  )
      {  if (board[r1+1].getType() == 'E' && isAdjacent(r1+1,r1) && (!isAdjacent(r1+1,p_f1) && !isAdjacent(r1+1,p_f2) && !isAdjacent(r1+1,p_f3))) //If square below is empty, move down
        { board[r1+1].setPlans( board[r1].getPlans() );  //set plans before piece to set right bitmap
          board[r1+1].setCpu(TRUE);
          board[r1+1].setPiece( board[r1].getType() );
          board[r1].setPlans(FALSE);
          board[r1].setCpu(FALSE);
          board[r1].setPiece('E');
          done = 1;
        }

       }

        if ( num >= 15  )
      {  if (board[r1-1].getType() == 'E' && isAdjacent(r1-1,r1) && (!isAdjacent(r1-1,p_f1) && !isAdjacent(r1-1,p_f2) && !isAdjacent(r1-1,p_f3))) //If square below is empty, move down
        { board[r1-1].setPlans( board[r1].getPlans() );  //set plans before piece to set right bitmap
          board[r1-1].setCpu(TRUE);
          board[r1-1].setPiece( board[r1].getType() );
          board[r1].setPlans(FALSE);
          board[r1].setCpu(FALSE);
          board[r1].setPiece('E');
          done = 1;
        }
        else if (board[r1+1].getType() == 'E' && isAdjacent(r1+1,r1) && (!isAdjacent(r1+1,p_f1) && !isAdjacent(r1+1,p_f2) && !isAdjacent(r1+1,p_f3))) //If square below is empty, move down
        { board[r1+1].setPlans( board[r1].getPlans() );  //set plans before piece to set right bitmap
          board[r1+1].setCpu(TRUE);
          board[r1+1].setPiece( board[r1].getType() );
          board[r1].setPlans(FALSE);
          board[r1].setCpu(FALSE);
          board[r1].setPiece('E');
          done = 1;
        }
       }
    } //--------- End of random Radio Engineer CPU move

    //Random radio engineer move -------------
    if ( done==0 && dead_r1_cpu == 0 )
    {
        if (isAdjacent(r1,r1+1) && board[r1+1].getType() == 'E')
        {
          board[r1+1].setPlans( board[r1].getPlans() );  //set plans before piece to set right bitmap
          board[r1+1].setCpu(TRUE);
          board[r1+1].setPiece( board[r1].getType() );
          board[r1].setPlans(FALSE);
          board[r1].setCpu(FALSE);
          board[r1].setPiece('E');
          done = 1;
        }
        else if (isAdjacent(r1,r1-1) && board[r1-1].getType() == 'E')
        {
          board[r1-1].setPlans( board[r1].getPlans() );  //set plans before piece to set right bitmap
          board[r1-1].setCpu(TRUE);
          board[r1-1].setPiece( board[r1].getType() );
          board[r1].setPlans(FALSE);
          board[r1].setCpu(FALSE);
          board[r1].setPiece('E');
          done = 1;
        }
        else if (isAdjacent(r1,r1-6) && board[r1-6].getType() == 'E')
        {
          board[r1-6].setPlans( board[r1].getPlans() );  //set plans before piece to set right bitmap
          board[r1-6].setCpu(TRUE);
          board[r1-6].setPiece( board[r1].getType() );
          board[r1].setPlans(FALSE);
          board[r1].setCpu(FALSE);
          board[r1].setPiece('E');
          done = 1;
        }
        else if (isAdjacent(r1,r1+6) && board[r1+6].getType() == 'E')
        {
          board[r1+6].setPlans( board[r1].getPlans() );  //set plans before piece to set right bitmap
          board[r1+6].setCpu(TRUE);
          board[r1+6].setPiece( board[r1].getType() );
          board[r1].setPlans(FALSE);
          board[r1].setCpu(FALSE);
          board[r1].setPiece('E');
          done = 1;
        }
    }// -------- end of RANDOM Random Engineer move -----------

    // Random medic move ---------------
    if ( done==0 && dead_m1_cpu == 0 )
    {
        if (isAdjacent(m1,m1+1) && board[m1+1].getType() == 'E')
        {
          board[m1+1].setPlans( board[m1].getPlans() );  //set plans before piece to set right bitmap
          board[m1+1].setCpu(TRUE);
          board[m1+1].setPiece( board[m1].getType() );
          board[m1].setPlans(FALSE);
          board[m1].setCpu(FALSE);
          board[m1].setPiece('E');
          done = 1;
        }
        else if (isAdjacent(m1,m1-1) && board[m1-1].getType() == 'E')
        {
          board[m1-1].setPlans( board[m1].getPlans() );  //set plans before piece to set right bitmap
          board[m1-1].setCpu(TRUE);
          board[m1-1].setPiece( board[m1].getType() );
          board[m1].setPlans(FALSE);
          board[m1].setCpu(FALSE);
          board[m1].setPiece('E');
          done = 1;
        }
        else if (isAdjacent(m1,m1-6) && board[m1-6].getType() == 'E')
        {
          board[m1-6].setPlans( board[m1].getPlans() );  //set plans before piece to set right bitmap
          board[m1-6].setCpu(TRUE);
          board[m1-6].setPiece( board[m1].getType() );
          board[m1].setPlans(FALSE);
          board[m1].setCpu(FALSE);
          board[m1].setPiece('E');
          done = 1;
        }
        else if (isAdjacent(m1,m1+6) && board[m1+6].getType() == 'E')
        {
          board[m1+6].setPlans( board[m1].getPlans() );  //set plans before piece to set right bitmap
          board[m1+6].setCpu(TRUE);
          board[m1+6].setPiece( board[m1].getType() );
          board[m1].setPlans(FALSE);
          board[m1].setCpu(FALSE);
          board[m1].setPiece('E');
          done = 1;
        }
    }// ------- END of Medic -------------

    //Random footsoldier move, getting desperate here
    if ( done==0 && !board[f1].getPlans() && dead_f1_cpu == 0 )
    {
        if (isAdjacent(f1,f1+1) && board[f1+1].getType() == 'E')
        {
          board[f1+1].setPlans( board[f1].getPlans() );  //set plans before piece to set right bitmap
          board[f1+1].setCpu(TRUE);
          board[f1+1].setPiece( board[f1].getType() );
          board[f1].setPlans(FALSE);
          board[f1].setCpu(FALSE);
          board[f1].setPiece('E');
          done = 1;
        }
        else if (isAdjacent(f1,f1-1) && board[f1-1].getType() == 'E')
        {
          board[f1-1].setPlans( board[f1].getPlans() );  //set plans before piece to set right bitmap
          board[f1-1].setCpu(TRUE);
          board[f1-1].setPiece( board[f1].getType() );
          board[f1].setPlans(FALSE);
          board[f1].setCpu(FALSE);
          board[f1].setPiece('E');
          done = 1;
        }
        else if (isAdjacent(f1,f1-6) && board[f1-6].getType() == 'E')
        {
          board[f1-6].setPlans( board[f1].getPlans() );  //set plans before piece to set right bitmap
          board[f1-6].setCpu(TRUE);
          board[f1-6].setPiece( board[f1].getType() );
          board[f1].setPlans(FALSE);
          board[f1].setCpu(FALSE);
          board[f1].setPiece('E');
          done = 1;
        }
        else if (isAdjacent(f1,f1+6) && board[f1+6].getType() == 'E')
        {
          board[f1+6].setPlans( board[f1].getPlans() );  //set plans before piece to set right bitmap
          board[f1+6].setCpu(TRUE);
          board[f1+6].setPiece( board[f1].getType() );
          board[f1].setPlans(FALSE);
          board[f1].setCpu(FALSE);
          board[f1].setPiece('E');
          done = 1;
        }
    }

    if ( done==0 && !board[f2].getPlans() && dead_f2_cpu == 0 )
    {
        if (isAdjacent(f2,f2+1) && board[f2+1].getType() == 'E')
        {
          board[f2+1].setPlans( board[f2].getPlans() );  //set plans before piece to set right bitmap
          board[f2+1].setCpu(TRUE);
          board[f2+1].setPiece( board[f2].getType() );
          board[f2].setPlans(FALSE);
          board[f2].setCpu(FALSE);
          board[f2].setPiece('E');
          done = 1;
        }
        else if (isAdjacent(f2,f2-1) && board[f2-1].getType() == 'E')
        {
          board[f2-1].setPlans( board[f2].getPlans() );  //set plans before piece to set right bitmap
          board[f2-1].setCpu(TRUE);
          board[f2-1].setPiece( board[f2].getType() );
          board[f2].setPlans(FALSE);
          board[f2].setCpu(FALSE);
          board[f2].setPiece('E');
          done = 1;
        }
        else if (isAdjacent(f2,f2-6) && board[f2-6].getType() == 'E')
        {
          board[f2-6].setPlans( board[f2].getPlans() );  //set plans before piece to set right bitmap
          board[f2-6].setCpu(TRUE);
          board[f2-6].setPiece( board[f2].getType() );
          board[f2].setPlans(FALSE);
          board[f2].setCpu(FALSE);
          board[f2].setPiece('E');
          done = 1;
        }
        else if (isAdjacent(f2,f2+6) && board[f2+6].getType() == 'E')
        {
          board[f2+6].setPlans( board[f2].getPlans() );  //set plans before piece to set right bitmap
          board[f2+6].setCpu(TRUE);
          board[f2+6].setPiece( board[f2].getType() );
          board[f2].setPlans(FALSE);
          board[f2].setCpu(FALSE);
          board[f2].setPiece('E');
          done = 1;
        }
    }

    if ( done==0 && !board[f3].getPlans() && dead_f3_cpu == 0 )
    {
        if (isAdjacent(f3,f3+1) && board[f3+1].getType() == 'E')
        {
          board[f3+1].setPlans( board[f3].getPlans() );  //set plans before piece to set right bitmap
          board[f3+1].setCpu(TRUE);
          board[f3+1].setPiece( board[f3].getType() );
          board[f3].setPlans(FALSE);
          board[f3].setCpu(FALSE);
          board[f3].setPiece('E');
          done = 1;
        }
        else if (isAdjacent(f3,f3-1) && board[f3-1].getType() == 'E')
        {
          board[f3-1].setPlans( board[f3].getPlans() );  //set plans before piece to set right bitmap
          board[f3-1].setCpu(TRUE);
          board[f3-1].setPiece( board[f3].getType() );
          board[f3].setPlans(FALSE);
          board[f3].setCpu(FALSE);
          board[f3].setPiece('E');
          done = 1;
        }
        else if (isAdjacent(f3,f3-6) && board[f3-6].getType() == 'E')
        {
          board[f3-6].setPlans( board[f3].getPlans() );  //set plans before piece to set right bitmap
          board[f3-6].setCpu(TRUE);
          board[f3-6].setPiece( board[f3].getType() );
          board[f3].setPlans(FALSE);
          board[f3].setCpu(FALSE);
          board[f3].setPiece('E');
          done = 1;
        }
        else if (isAdjacent(f3,f3+6) && board[f3+6].getType() == 'E')
        {
          board[f3+6].setPlans( board[f3].getPlans() );  //set plans before piece to set right bitmap
          board[f3+6].setCpu(TRUE);
          board[f3+6].setPiece( board[f3].getType() );
          board[f3].setPlans(FALSE);
          board[f3].setCpu(FALSE);
          board[f3].setPiece('E');
          done = 1;
        }
    }

    //One more random move -- Footsoldiers with plans
    //Random footsoldier move, getting desperate here
    if ( done==0 && dead_f1_cpu == 0 )
    {
        if (isAdjacent(f1,f1+1) && board[f1+1].getType() == 'E')
        {
          board[f1+1].setPlans( board[f1].getPlans() );  //set plans before piece to set right bitmap
          board[f1+1].setCpu(TRUE);
          board[f1+1].setPiece( board[f1].getType() );
          board[f1].setPlans(FALSE);
          board[f1].setCpu(FALSE);
          board[f1].setPiece('E');
          done = 1;
        }
        else if (isAdjacent(f1,f1-1) && board[f1-1].getType() == 'E')
        {
          board[f1-1].setPlans( board[f1].getPlans() );  //set plans before piece to set right bitmap
          board[f1-1].setCpu(TRUE);
          board[f1-1].setPiece( board[f1].getType() );
          board[f1].setPlans(FALSE);
          board[f1].setCpu(FALSE);
          board[f1].setPiece('E');
          done = 1;
        }
        else if (isAdjacent(f1,f1-6) && board[f1-6].getType() == 'E')
        {
          board[f1-6].setPlans( board[f1].getPlans() );  //set plans before piece to set right bitmap
          board[f1-6].setCpu(TRUE);
          board[f1-6].setPiece( board[f1].getType() );
          board[f1].setPlans(FALSE);
          board[f1].setCpu(FALSE);
          board[f1].setPiece('E');
          done = 1;
        }
        else if (isAdjacent(f1,f1+6) && board[f1+6].getType() == 'E')
        {
          board[f1+6].setPlans( board[f1].getPlans() );  //set plans before piece to set right bitmap
          board[f1+6].setCpu(TRUE);
          board[f1+6].setPiece( board[f1].getType() );
          board[f1].setPlans(FALSE);
          board[f1].setCpu(FALSE);
          board[f1].setPiece('E');
          done = 1;
        }
    }

    if ( done==0 && dead_f2_cpu == 0 )
    {
        if (isAdjacent(f2,f2+1) && board[f2+1].getType() == 'E')
        {
          board[f2+1].setPlans( board[f2].getPlans() );  //set plans before piece to set right bitmap
          board[f2+1].setCpu(TRUE);
          board[f2+1].setPiece( board[f2].getType() );
          board[f2].setPlans(FALSE);
          board[f2].setCpu(FALSE);
          board[f2].setPiece('E');
          done = 1;
        }
        else if (isAdjacent(f2,f2-1) && board[f2-1].getType() == 'E')
        {
          board[f2-1].setPlans( board[f2].getPlans() );  //set plans before piece to set right bitmap
          board[f2-1].setCpu(TRUE);
          board[f2-1].setPiece( board[f2].getType() );
          board[f2].setPlans(FALSE);
          board[f2].setCpu(FALSE);
          board[f2].setPiece('E');
          done = 1;
        }
        else if (isAdjacent(f2,f2-6) && board[f2-6].getType() == 'E')
        {
          board[f2-6].setPlans( board[f2].getPlans() );  //set plans before piece to set right bitmap
          board[f2-6].setCpu(TRUE);
          board[f2-6].setPiece( board[f2].getType() );
          board[f2].setPlans(FALSE);
          board[f2].setCpu(FALSE);
          board[f2].setPiece('E');
          done = 1;
        }
        else if (isAdjacent(f2,f2+6) && board[f2+6].getType() == 'E')
        {
          board[f2+6].setPlans( board[f2].getPlans() );  //set plans before piece to set right bitmap
          board[f2+6].setCpu(TRUE);
          board[f2+6].setPiece( board[f2].getType() );
          board[f2].setPlans(FALSE);
          board[f2].setCpu(FALSE);
          board[f2].setPiece('E');
          done = 1;
        }
    }

    if ( done==0 && dead_f3_cpu == 0 )
    {
        if (isAdjacent(f3,f3+1) && board[f3+1].getType() == 'E')
        {
          board[f3+1].setPlans( board[f3].getPlans() );  //set plans before piece to set right bitmap
          board[f3+1].setCpu(TRUE);
          board[f3+1].setPiece( board[f3].getType() );
          board[f3].setPlans(FALSE);
          board[f3].setCpu(FALSE);
          board[f3].setPiece('E');
          done = 1;
        }
        else if (isAdjacent(f3,f3-1) && board[f3-1].getType() == 'E')
        {
          board[f3-1].setPlans( board[f3].getPlans() );  //set plans before piece to set right bitmap
          board[f3-1].setCpu(TRUE);
          board[f3-1].setPiece( board[f3].getType() );
          board[f3].setPlans(FALSE);
          board[f3].setCpu(FALSE);
          board[f3].setPiece('E');
          done = 1;
        }
        else if (isAdjacent(f3,f3-6) && board[f3-6].getType() == 'E')
        {
          board[f3-6].setPlans( board[f3].getPlans() );  //set plans before piece to set right bitmap
          board[f3-6].setCpu(TRUE);
          board[f3-6].setPiece( board[f3].getType() );
          board[f3].setPlans(FALSE);
          board[f3].setCpu(FALSE);
          board[f3].setPiece('E');
          done = 1;
        }
        else if (isAdjacent(f3,f3+6) && board[f3+6].getType() == 'E')
        {
          board[f3+6].setPlans( board[f3].getPlans() );  //set plans before piece to set right bitmap
          board[f3+6].setCpu(TRUE);
          board[f3+6].setPiece( board[f3].getType() );
          board[f3].setPlans(FALSE);
          board[f3].setCpu(FALSE);
          board[f3].setPiece('E');
          done = 1;
        }
    }

  //oops, the CPU has nothing to do, this shouldn't happen I know, but hey....
    if (done == 0)
   {
      textout_ex(screen, smallfont, "Enemy is confused...",80,430,makecol(150, 0, 0),-1);
      rest(700);
   }

      destroy_font(smallfont);
      destroy_font(myfont);
      smallfont = 0;
      myfont = 0;

}
// --------  END of cpu_turn() ----------------


/* ---------- game_play() ----------------------
MAIN GAME LOOP
------------------------------------------------ */
void game_play()
{
   BITMAP* boardframe;
   int squares[531][351];

   // FONTS ---------------------------
   FONT *smallfont, *myfont;
   PALETTE palette;
   smallfont = load_font("fonts/smallfont.bmp",palette,NULL);
   myfont = load_font("fonts/myfont.pcx",palette,NULL);
   //-----------------------------------

   //----- Timer Stuff --------------------
   /* LOCK_VARIABLE(subticks);
    LOCK_FUNCTION(subticker);
    install_int_ex(subticker,BPS_TO_TIMER(60));
   */
    LOCK_VARIABLE(ticks);
    LOCK_FUNCTION(ticker);
    install_int_ex(ticker,BPS_TO_TIMER(60));
   //---------------------------------------

    int x, y, m1=0,m2=0, count=0, click = 0, click2 = 0, exit = 0, turn = 0, again = -1;
    int box = -1, box2 = -1;
    int radio = 0, medic = 0;

    Square* square1st;
    Square* square2nd; //game square pointers to the first and second square clicked

    show_mouse(NULL); //Hide mouse

    //Initialize squares
    for (m1 = 0; m1 <= 530; m1++)
   {
 		 for (m2 = 0; m2 <= 350; m2++)
 		{
            squares[m1][m2] = 0;
        }
   }

   //Set squares to the board locations
   for (m1 = 80; m1 <= 530; m1=m1+50)
   {
 		 for (m2 = 100; m2 <= 350; m2=m2+50)
 		{
            squares[m1][m2] = count;
            count++;
        }
   }

    //Game Board
    boardframe = load_bitmap("images/boardframe.bmp",NULL);
    masked_blit(boardframe,screen,0,0,0,0,SCREEN_W,SCREEN_H); //update board

    //The gameboard squares (0-59)
    Square board[60];

    // Initialize empty game board squares to their default locations
    init_board(board);

    // Draw all the board squares to the screen
    redraw(board);
    // LET's DO THIS THING!


do
{
  again = -1;

 do
 {
    //Poll game board until left-click or Q key is pressed to exit game
    while ( click == 0 && exit == 0 )
   {
         show_mouse(screen);

         while (ticks < 1)
         {
            rest(0);
         }
         ticks = 0;

          //Old code for polling the board that I could't get working without mouse flicker
          //Because I didn't actually plan my code before writing it :P
          /*  for (m1 = 80; m1 <= 530; m1=m1+50)
			{
                 for (m2 = 100; m2 <= 350; m2=m2+50)
 				  {
      			         if (mouse_x > m1  && mouse_x < m1+50 && mouse_y > m2 && mouse_y < m2+50)
 				         {
                               show_mouse(NULL);
                               board[squares[m1][m2]].setActive(TRUE);
                               show_mouse(screen);
                               if (mouse_b & 1) break;
                          }
                         else
                        {   show_mouse(NULL);
                            board[squares[m1][m2]].setActive(FALSE);
                            show_mouse(screen);
                        }
                   }
              } */

       if (mouse_b & 1) //left-click
       {
         box = get_square(board,mouse_x,mouse_y); //capture Game board square left-click
         if (box > -1) square1st = &board[box];   //assign 1st click if square is valid

         /* - is in a box (aka. Not(-1)),
            - is not a CPU piece,
            - is not a Helipad,
            - is not an empty square,
            - is not a dead piece
         */
         if (box > -1 && !(square1st->getCpu()) && square1st->getType() != 'H' && square1st->getType() != 'E' && !(square1st->isDead()))
         {
                   show_mouse(NULL);
                   square1st->setActive(TRUE); //Highlight the leftclick box
                   show_mouse(screen);
                   click = 1; //Trip flag, left-click is VALID

                   while (mouse_b & 1){}; //do nothing, just wait until mouse button is lifted

                   //Click sound
                   sound_set = load_datafile_object("fsgame.dat","Snd49");
                   play_sample((SAMPLE*)sound_set->dat,255,128,1000,0);
                  // unload_datafile_object(sound_set);

                   //If first piece selected is a Radio Engineer, give the option to call
                   //in a new Helipad, placed adjacent to piece
                   if (square1st->getType() == 'R')
                   {
                      textout_ex(screen, smallfont, "<R>adio in New Helipad",140,420,makecol(150, 0, 0),-1);
                   }

                    //If first piece selected is a Medic, give the option to Heal
                   //a piece in the graveyard and bring back to the board
                   if (square1st->getType() == 'M' && (dead_f1==1 || dead_f2==1 || dead_f3==1 || dead_r1==1) )
                   {
                      textout_ex(screen, smallfont, "<H>eal a Soldier",140,420,makecol(150, 0, 0),-1);
                   }
          }
       }

      if (key[KEY_Q])  //Trip exit game flag
       {exit = 1; show_mouse(NULL);}
   }

   /* First click was good, now waiting for 2nd left-click*/
   while ( click == 1 )
   {
       if (key[KEY_ESC]) //If ESC is pressed, cancel 'select' and go back to polling board
       break;

       //If Radio Engineer requests new Helipad
       if (key[KEY_R] && square1st->getType() == 'R')
       {
          //Play radio sound fx
         sound_set = load_datafile_object("fsgame.dat","Morse");
         play_sample((SAMPLE*)sound_set->dat,255,128,1000,0);

         //remove prompt from screen, no longer needed
         blit(boardframe,screen,80,400,80,400,250,100);

         click = 0;
         update_helipad(board,box); //Place new Helipad
         unload_datafile_object(sound_set);

         //Play helicopter sound effect
         sound_set2 = load_datafile_object("fsgame.dat","Copter1");
         play_sample((SAMPLE*)sound_set2->dat,255,128,1000,0);

         radio = 1; // Radio option done, no need to wait for 2nd click below
         turn = 1; // CPU can take turn now
         show_mouse(NULL); //Hide mouse
         //unload_datafile_object(sound_set2);
       }

       //If Medic requests to Heal a dead Piece
       if (key[KEY_H] && square1st->getType() == 'M' && (dead_f1==1 || dead_f2==1 || dead_f3==1 || dead_r1==1) )
       {
         show_mouse(NULL); //Hide mouse
         //remove prompt from screen, no longer needed
         blit(boardframe,screen,80,400,80,400,285,100); //update gameboard
         click = 0;

         leave_graveyard(board); //Heal piece

         //Play Happy Heal Sigh
         sound_set = load_datafile_object("fsgame.dat","Sigh");
         play_sample((SAMPLE*)sound_set->dat,255,128,1000,0);

         medic = 1; // Medic option done, no need to wait for 2nd click below
         turn = 1; // CPU can take turn now
         //unload_datafile_object(sound_set);
       }

       if (mouse_b & 1 && radio == 0 && medic == 0) //2nd left-click and helipad or medic option not selected
       {
         box2 = get_square(board,mouse_x,mouse_y); //capture 2nd gameboard square 'select'

        //(Box2 = -1 means they didn't click on the game board)
         if (box2 > -1) square2nd = &board[box2];

         /* Criteria for 2nd box
          If 2nd click is:
                        in a square
                        in an adjacent square (diagonal doesn't count)
                        and not same square
          then check boxes for game action
          (Box2 = -1 means they didn't click on the game board)
         */
          if ( box2 > -1 && isAdjacent(box,box2))
          {
                   click2 = 1;                 //Trip flag when 2nd 'select' is "good"
                   show_mouse(NULL); //Hide mouse
                   square2nd->setActive(TRUE); //Highlight the leftclick box

                   while (mouse_b & 1){};      //do nothing, just wait till button is lifted

                  //Do what the pieces do here (if turn =1, cpu can play)
                   turn = update_pieces(&board[box],&board[box2],board);
                   square1st->setActive(FALSE);
                   square2nd->setActive(FALSE); //unhighlight boxes when done
                   click = 0;
          }
           else click = 0; //default exit loop if click was no good whatsoever
       }



   } //end of "First click, waiting for second click" loop

    show_mouse(NULL);
    //redraw the gameboard now that pieces may have changed
    redraw(board);

    //Initialize the gameboard for next turn
    blit(boardframe,screen,80,400,80,400,300,100);

    //If Player took a valid turn and user hasn't quit
    //CPU can take a turn now
    if (!key[KEY_Q] && turn == 1 && win == 0)
    {
       cpu_turn(board);
    }

    //Now that CPU has gone, update the game board
    blit(boardframe,screen,80,400,80,400,300,100); //game board
    redraw(board);

    //If someone has win, end the game
    if (cpuwin == 1 || win == 1)
    {
       end_game();
       exit = 1;
       blit(boardframe,screen,80,400,80,400,300,100); //update game board
       blit(boardframe,screen,580,10,580,10,50,50);
    }

    //initialize for next round
    square1st = 0;
    square2nd = 0;
    click     = 0;
    click2    = 0;
    box       = -1;
    box2      = -1;
    radio     = 0;
    medic     = 0;
    turn      = 0;

 } while (exit == 0);  //game loop until Q is pressed to exit game

   clear_keybuf();
   rest(1);

 //Play again prompt
 textout_ex(screen, smallfont, "BATTLE AGAIN? [y/n]",140,420,makecol(150, 0, 0),-1);
  again = -1;

  while (again == -1)
  {
     if (key[KEY_Y])
     again = 0;
     if (key[KEY_N])
     again = 1;
  }

  //Initialize Globals and other Variables
 dead_f1=0;dead_f2=0;dead_f3=0;dead_m1=0;dead_r1=0;
 win=0; cpuwin = 0;
 dead_f1_cpu=0; dead_f2_cpu=0; dead_f3_cpu=0; dead_m1_cpu=0; dead_r1_cpu=0;
 total_score = 0; total_score_cpu = 0;

  click = 0; click2 = 0; exit = 0; turn = 0;
  box = -1; box2 = -1;
  radio = 0; medic = 0;

 sound_set = 0;
 sound_set2 = 0;

 for (x = 0; x < 60; x++)
 {
   board[x].setCpu(FALSE);
   board[x].setPlans(FALSE);
   board[x].setDead(FALSE);
   board[x].setPiece('E');
 }
 // End of Initialization ----------

  //Initialize the game board and other stuff
  //remove prompt from screen, no longer needed
  blit(boardframe,screen,80,400,80,400,250,100);
  init_board(board);
  redraw(board);
  show_mouse(screen);


} while (again == 0);

 destroy_bitmap(boardframe);
 unload_datafile_object(data);
 unload_datafile_object(sound_set);
 unload_datafile_object(sound_set2);
 unload_datafile_object(music_set);
 destroy_font(myfont);
 destroy_font(smallfont);
 data = 0;
 sound_set = 0;
 sound_set2 = 0;

}
// -------------- END OF PROGRAM ----------------------------------------------------


