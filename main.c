#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>
#include <stdio.h>
#include <stdlib.h>

 int main(int argc, char * argv[]){
 if(SDL_Init(SDL_INIT_EVERYTHING) !=0){
 fprintf(stderr,"ErrorinSDL_Init:%s\n",SDL_GetError());
 exit(EXIT_FAILURE);
 }
 SDL_Surface *ecran=NULL;
 if((ecran=SDL_SetVideoMode(800,600,32,SDL_HWSURFACE|
 SDL_DOUBLEBUF))==NULL){
 fprintf(stderr,"ErrorinSDL_SetVideoMode:%s\n",
 SDL_GetError());
 SDL_Quit();
 exit(EXIT_FAILURE);
 }
 SDL_WM_SetCaption("Sonic The Hedgehog",NULL);
 /*Votretravailici*/
 SDL_Rect rectangle;
 rectangle.x = 50;
 rectangle.y = 50;
 rectangle.w = 200;
 rectangle.h = 100;
 SDL_FillRect(ecran, &rectangle, SDL_MapRGB(ecran->format, 64, 118, 173));
 SDL_Flip(ecran);
 SDL_Rect rectangle2;
 rectangle2.x = 200;
 rectangle2.y = 50;
 rectangle2.w = 100;
 rectangle2.h = 200;
 SDL_FillRect(ecran, &rectangle2, SDL_MapRGB(ecran->format, 64, 140, 110));
 SDL_Flip(ecran);
 /*Votretravailici*/
 int active=1;
 SDL_Event event;
 while(active){
 SDL_WaitEvent(&event);
 switch(event.type){
 case SDL_QUIT:active=0; break;
 default :break;
 }
 }
 SDL_Quit();
 exit(EXIT_SUCCESS);
 }
