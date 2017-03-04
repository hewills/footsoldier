/*
File: main.cpp
Author: Hana Wills
Last Mod: 04/5/09

First ALLEGRO game.

Thanks to:
Sarah, Samantha, and Willy
Pete for posting the Frame rate demo
*/
#include <stdio.h>
#include <string.h>
#include <allegro.h>
#include "fs.h"

//Fade IN bitmap
void highcolor_fade_in(BITMAP *bmp_orig, int speed)
{
   BITMAP *bmp_buff;

    if ((bmp_buff = create_bitmap(SCREEN_W, SCREEN_H)))
    {
        int a;
        if (speed <= 0) speed = 16;
        
        for (a = 0; a < 256; a+=speed)
        {
            clear(bmp_buff);
            set_trans_blender(0,0,0,a);
            draw_trans_sprite(bmp_buff, bmp_orig, 0, 0);
            vsync();
            blit(bmp_buff, screen, 0,0, 0,0, SCREEN_W, SCREEN_H);
        }
        destroy_bitmap(bmp_buff);
    } 

    blit(bmp_orig, screen, 0,0, 0,0, SCREEN_W, SCREEN_H);
}

//Fade OUT a Bitmap
void highcolor_fade_out(int speed)
{
    BITMAP *bmp_orig, *bmp_buff;

    if ((bmp_orig = create_bitmap(SCREEN_W, SCREEN_H)))
    {
        if ((bmp_buff = create_bitmap(SCREEN_W, SCREEN_H)))
        {
            int a;
            blit(screen, bmp_orig, 0,0, 0,0, SCREEN_W, SCREEN_H);
            if (speed <= 0) speed = 16;
        
            for (a = 255-speed; a > 0; a-=speed)
            {
                clear(bmp_buff);
                set_trans_blender(0,0,0,a);
                draw_trans_sprite(bmp_buff, bmp_orig, 0, 0);
                vsync();
                blit(bmp_buff, screen, 0,0, 0,0, SCREEN_W, SCREEN_H);
            }
            destroy_bitmap(bmp_buff);
        }
        destroy_bitmap(bmp_orig);
    }

    rectfill(screen, 0,0, SCREEN_W,SCREEN_H, makecol(0,0,0));
}


int main( void )
{
    int menucheck = -1;
    BITMAP *tmp;  //tmp bitmap for fading in/out

    allegro_init();
    install_keyboard();
    install_timer();    //Timer functions

    //Install SOUND drivers
   install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,NULL);

   //Install MOUSE drivers
   if (install_mouse() == -1) {
    allegro_message("Unable to load Mouse, check mouse installation: %s\n", allegro_error);
    exit(1);}

    //set_palette(desktop_palette);
    set_color_depth(16);

    //SET GFX MODE
    if(set_gfx_mode (GFX_AUTODETECT,640,480,0,0)) {
    allegro_message("Couldn't set Graphics mode: %s\n", allegro_error);
	exit(1); }

    //fade.c Author: Matthew Leverton
    /* set up a temp bitmap to so we can fade back later ------ */
    if (!(tmp = create_bitmap(SCREEN_W, SCREEN_H))) exit(1);
    tmp = load_bitmap("images/LOGOb.bmp",NULL);

    highcolor_fade_in(tmp, 16);
    readkey();
    highcolor_fade_out(22);
    destroy_bitmap(tmp); //done with this bitmap
    // End of fade bitmap -------------------------------------

    //Play!
    do
    {
      menucheck = menu();

    }while (menucheck == -1);

    if (menucheck == 0)

    game_play();

 return 0;

}
END_OF_MAIN()
