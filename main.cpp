/*
Drift Droids Alpha
Language: C++
Uses SDL Library
And MiniAT integration
Coded by Drift Droids Team 1
Version 0.9L
*/

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>

//MiniAT Headers
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <stdint.h>
#include <inttypes.h>

#include "miniat/miniat.h"
#include "peripherals.h"
#include "ports.h"
#include "vehicle.h"

#define UINT64_MAX (18446744073709551615)
#define MAX_CYCLES UINT64_MAX


#define	EXIT_FAILURE	1

//MiniAT Global Variables
miniat *m = NULL;
miniat *n = NULL;
char *input_filename = NULL;
char *input_filename2 = NULL;
FILE *infile = NULL;
FILE *infile2 = NULL;


uint64_t cycles = 0;

//MiniAT Protocols
static void cleanup();
static void miniat_start(int argc, char *argv[]);
static void parse_options(int argc, char *argv[]);
static void print_usage(char *argv[]);
static void signal_handler(int sig);


int main_start(int argc, char *argv[]);
SDL_Texture* loadMap(char* mapPath, SDL_Renderer* render, int* w, int* h);
SDL_Texture* loadCar(char* carPath, SDL_Renderer* render);
Uint32 get_pixel(SDL_Surface *surface, int x, int y);
SDL_Surface* loadFrictionMap(char* mapPath, int MapW, int MapH);
Mix_Music* loadMusic(char* musicPath);
SDL_Rect* sizeCarImage(SDL_Rect* car1_rect,SDL_Texture* car_img);

//MiniAT Functions
static void miniat_start(int argc, char *argv[]) {

	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);

	atexit(cleanup);

	parse_options(argc, argv);

	infile = fopen(input_filename, "r+b");
	infile2 = fopen(input_filename2, "r+b");

	if(!infile) {
		fprintf(stderr, "Couldn't open \"%s\".  %s", input_filename, strerror(errno));
		exit(EXIT_FAILURE);
	}

	m = miniat_new(infile, NULL);
	n = miniat_new(infile2, NULL);
    main_start(argc, argv);

	return;
}

//Here?

static void signal_handler(int sig) {

	if(sig == SIGINT || sig == SIGTERM) {
		exit(EXIT_SUCCESS);
	}

	return;
}

static void cleanup() {

	if(m) {
		/* MiniAT also closes the binary file it was passed on miniat_new */
		miniat_free(m);
	}
	if(n)
    {
        miniat_free(n);
    }

	/*
	 * Call all the other cleanup functions
	 */
	peripherals_cleanup();
	ports_cleanup();

	if(cycles < MAX_CYCLES) {
		printf("\nMiniAT: %d cycles executed\n", cycles);
	}
	else {
		printf("%d Runtime exceeded cycles!" , MAX_CYCLES);
	}

	return;
}
static void print_usage(char *argv[]) {

	fprintf(stderr, "\n");
	fprintf(stderr, "Usage:  ");
    fprintf(stderr, "%s", argv[0]);
    fprintf(stderr, " [bin_file]\n");
	fprintf(stderr, "\n");

	return;
}


static void parse_options(int argc, char *argv[]) {


	input_filename = argv[5];
	input_filename2 = argv[7];

	return;
}

//End of MiniAT Functions

//MiniAT main
int main(int argc, char *argv[]) {

	TRY {
		miniat_start(argc, argv);
	}
		CATCHALL {
		miniat_dump_error();
	}
    FINALLY {}
    ETRY;
	return EXIT_SUCCESS;
}



//Our old main
 int main_start(int argc, char *argv[])
 {

    bool testbool = false;
    bool testbool2 = true;

    int laptest = -1;

    SDL_Window    *gameWindow = NULL;       //Pointer for Window
    SDL_Renderer  *gameRender = NULL;       //Pointer for Renderer
    SDL_Texture   *gameMap    = NULL;       //Pointer for Map Texture
    SDL_Texture   *car1       = NULL;       //Pointer for first Car
    Mix_Music     *gMusic     = NULL;       //Pointer for Music
    SDL_Texture   *car2       = NULL;       //Pointer for Second Car
    SDL_Surface   *gameSurface= NULL;       //Pointer for Map-Surface(Used later for friction map)

    //Creates window and Renderer
    gameWindow = SDL_CreateWindow("Drift Droids", 100, 100, 50, 50, 0);

    gameRender = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED);


    char frictionPath[255];                 //String for friction map pathname
    char mapPath[255];                     //String for map pathname
    char carPath[255];                      //String for first car pathname
    char musicPath[255];
    char car2Path[255];                   //String for second car pathname


    if(argv[1] == NULL || argv[2] == NULL || argv[3] == NULL || argv[4] == NULL /*|| argv[5] == NULL || argv[6] == NULL*/)
      {
        fprintf(stderr, "ERROR: No friction map or map or car path arguments passed");
        exit(4);                               //Error 4 means one or both arguments were not given
      }
                 //Initialize SDL_mixer

    /*Copying filenames from command line arguments to strings for opening*/
    strcpy(frictionPath,argv[1]); //friction map file path
    strcpy(mapPath, argv[2]);     //map file path
    strcpy(carPath, argv[3]);     //car1 file path
    strcpy(musicPath, argv[4]);  //music file path
    strcpy(car2Path, argv[6]);  //car2 file path


    int   imgW,                            //Contains map width value for window resizing
          imgH;                            //Contains map height value for window resizing

    gameMap = loadMap(mapPath, gameRender, &imgW, &imgH);
    gameSurface = loadFrictionMap(frictionPath, imgW,imgH);
    car1    = loadCar(carPath, gameRender);
    gMusic  = loadMusic(musicPath);
    car2    = loadCar(car2Path,gameRender);

    /*Error Handling Block*/
    if(gameMap == NULL)
    {
      fprintf(stderr, "ERROR: Map was unable to be loaded\n");
      if(car1 == NULL /*|| car2 == NULL || car3 == NULL || car4 == NULL*/)
        {
            fprintf(stderr, "ERROR: Car was unable to be loaded\n");
            exit(3);                           //Error 3 means both car and map could not be loaded
        }
        if(gMusic == NULL){
            fprintf(stderr, "ERROR: Music was unable to be loaded\n");
            exit(3);
        }
        else
            {
                exit(1);                       //Error 1 means Map could not be loaded, Car untested
            }
    }
    else
    if(car1 == NULL || car2 == NULL)
    {
      fprintf(stderr, "ERROR: Car was unable to be loaded\n");
      exit(2);                                 //Error 2 means Car could not be loaded, but Map was fine
    }

    /*Changes values of window size integers*/
    SDL_QueryTexture(gameMap, NULL, NULL, &imgW, &imgH);

    SDL_SetWindowSize(gameWindow, imgW, imgH);

    SDL_Rect map_rect;
    map_rect.x = 0;
    map_rect.y = 0;
    /*Reuses imgW and imgH for rectangle*/
    map_rect.w = imgW;
    map_rect.h = imgH;

    SDL_Rect car1_rect;
    car1_rect.x = 105;
    car1_rect.y = 300;

    SDL_Rect car2_rect;
    car2_rect.x = 125;
    car2_rect.y = 300;

    sizeCarImage(&car1_rect,car1);
    sizeCarImage(&car2_rect,car2);

    Uint32 startPixel1 = get_pixel(gameSurface,car1_rect.x ,car1_rect.y);

    Uint32 Pixel1TL ;
    Uint32 Pixel1TR ;
    Uint32 Pixel1BL ;
    Uint32 Pixel1BR ;


    Uint32 startPixel2 = get_pixel(gameSurface,car2_rect.x ,car2_rect.y);

    Uint32 Pixel2TL ;
    Uint32 Pixel2TR ;
    Uint32 Pixel2BL ;
    Uint32 Pixel2BR ;

    double car1_angle =0;
    double car2_angle =0;

    vehicle the_car1(car1_rect.x, car1_rect.y, 270, car1_rect.h, car1_rect.w, 0.5, 0.1, 0.25);
    vehicle the_car2(car2_rect.x, car2_rect.y, 270, car2_rect.h, car2_rect.w, 0.5, 0.1, 0.25);

    SDL_Event user_clicked;
        int continue_game =1;

    Mix_PlayMusic( gMusic, -1 );

    /*Loads map into memory*/
    SDL_RenderCopyEx(gameRender, gameMap, NULL, &map_rect, 0, NULL, SDL_FLIP_NONE);

    SDL_RenderPresent(gameRender);

    /*Prints map to screen*/
    SDL_RenderPresent(gameRender);

    /*Loads car into memory*/
    SDL_RenderCopyEx(gameRender, car1, NULL, &car1_rect, car1_angle, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyEx(gameRender, car2, NULL, &car2_rect, car2_angle, NULL, SDL_FLIP_NONE);

    SDL_RenderPresent(gameRender);

    /*Keeps Window Open until Escape is pressed or Close Signal*/
    /*Window Can become moveable*/
    int car1laptest = -1;
            int car2laptest = -1;
    do
      {

        SDL_RenderClear(gameRender);
        SDL_RenderCopy(gameRender, gameMap, NULL, &map_rect);
        car1_rect.x=the_car1.xPos_f();
        car1_rect.y=the_car1.yPos_f();
        car1_angle =the_car1.angle_f();

        car2_rect.x=the_car2.xPos_f();
        car2_rect.y=the_car2.yPos_f();
        car2_angle =the_car2.angle_f();

        the_car1.move(imgW, imgH, gameSurface, startPixel1);
        the_car2.move(imgW, imgH, gameSurface, startPixel2);

        SDL_RenderCopyEx(gameRender, car1, NULL, &car1_rect, car1_angle, NULL, SDL_FLIP_NONE);
        SDL_RenderCopyEx(gameRender, car2, NULL, &car2_rect, car2_angle, NULL, SDL_FLIP_NONE);

        SDL_RenderPresent(gameRender);

        SDL_PollEvent(&user_clicked);

        /*If escape key pressed, close game*/
        if(user_clicked.key.keysym.sym == SDLK_ESCAPE)
            continue_game=0;

        the_car1.handleEvent(user_clicked);
        the_car2.handleEvent(user_clicked);

        //MiniAT Update
        if(cycles < MAX_CYCLES) {
            cycles++;
        }
        miniat_clock(m);
        peripherals_clock(m, the_car1);
        miniat_clock(n);
        peripherals_clock(n, the_car2);
        //ports_clock(m);
        //the_car1.updatelaps(cycles);
        //the_car2.updatelaps(cycles);
            bool car1bestlap;
            bool car2bestlap;



        if(car1laptest!=the_car1.laps_f())
        {
        if(the_car1.laps_f()>0)
        {
            if(car1laptest!=the_car1.laps_f())
        {
            if(the_car1.lastcycles_f()>cycles-the_car1.lastcycles_f())
                car1bestlap = true;

            the_car1.set_lastcycles(cycles-the_car1.lastcycles_f());
            printf("\nCar1:\n");
            printf("\nLap: %d Cycles: %d\n Total Cycles: %u", the_car1.laps_f(), the_car1.lastcycles_f(), cycles);
            if(car1bestlap)
            {
                printf("-New best lap!\n");
                car1bestlap=false;
            }
            car1laptest=the_car1.laps_f();
        }
        }
        }

      if(car2laptest!=the_car2.laps_f())
      {

        if(the_car2.laps_f()>0)
        {
            if(car2laptest!=the_car2.laps_f())
        {
            if(the_car2.lastcycles_f()>cycles-the_car2.lastcycles_f())
                car2bestlap = true;

            the_car2.set_lastcycles(cycles-the_car2.lastcycles_f());
            printf("\nCar2:\n");
            printf("\nLap: %d Cycles: %d\n Total Cycles: %u", the_car2.laps_f(), the_car2.lastcycles_f(), cycles);
            if(car2bestlap)
            {
                printf("-New best lap!\n");
                car2bestlap=false;
            }
            car2laptest=the_car2.laps_f();
        }
        }
      }
        //End MiniAT Update

        /*Frames per second on game screen*/
        SDL_Delay(1000.0/180);

      }while((continue_game) && (user_clicked.type != SDL_QUIT) && ((the_car1.laps_f()<3)||the_car2.laps_f()<3));
    /*Detects if car 1 or 2 won game*/
      if(the_car1.laps_f()==3)
      {
          printf("\nCar 1 Won!\n");
      }
    else if (the_car2.laps_f()==3)
      {
          printf("\nCar2 Won!\n");
      }
    else
        printf("\nGame closed before race finished\n");
    //Destroy Window
   SDL_DestroyWindow(gameWindow);
   //Free the music
    Mix_FreeMusic( gMusic );
    gMusic = NULL;
    Mix_Quit();

   return 0;
   //Goodbye
 }

SDL_Texture* loadMap(char* mapPath, SDL_Renderer* render, int* w, int* h)
  {
    SDL_Texture *bgmap = NULL;                     //Pointer for map image

    bgmap = IMG_LoadTexture(render, mapPath);

    SDL_QueryTexture(bgmap, NULL, NULL, w, h);

    if((*w<=190) || (*h<=203))
    {
        fprintf(stderr, "\nERROR: Map is too small, maybe you loaded car first, please switch");
        exit(5);                        //Error 5: Your map image was too small.
    }

    return bgmap;
  }

SDL_Texture* loadCar(char* carPath, SDL_Renderer* render)
  {
    SDL_Texture *car = NULL;                     //Pointer for car image

    car = IMG_LoadTexture(render, carPath);

    int carW, carH;

    SDL_QueryTexture(car, NULL, NULL, &carW, &carH);


    if((carW>300) || (carH>300))
    {
        fprintf(stderr, "ERROR: Car is too big, remember [map] then [car]");
        exit(6);                        //Error 6: Your car image was too large.
    }

    return car;
  }
Mix_Music* loadMusic(char* musicPath){

Mix_Music *gMusic = NULL;
    //Music

//If mixer isn't available, error shows up
      if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        exit(4);
    }

//Loads the music as requested by the coder. If it isn't found, displays an error
    gMusic = Mix_LoadMUS( musicPath );
    if( gMusic == NULL )
    {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
       exit(4);
    }
     Mix_PlayMusic( gMusic, -1 );


 return gMusic;
 }
 //load in base surface to get pixels
  SDL_Surface* loadFrictionMap(char* mapPath ,int MapW, int MapH){

    SDL_Surface *mapSurface = NULL;

    mapSurface = IMG_Load(mapPath);

    if(mapSurface == NULL){
        fprintf(stderr, "ERROR: Friction map was unable to be loaded");
        exit(7);                   //Error 7: The friction map was unable to be loaded
    }

    if(mapSurface->w != MapW || mapSurface->h != MapH){
        fprintf(stderr, "ERROR: Friction map and Display map are not the same size. Please check your friction map to be same size");
        exit(8);                   //Error 8: The maps are not the same size
    }



    return mapSurface;

  }

  //returns the pixel color to be used for testing
  Uint32 get_pixel( SDL_Surface *surface, int x, int y )
{
    SDL_LockSurface(surface);
    //Convert the pixels to 32 bit
    Uint32 *pixels = (Uint32 *)surface->pixels;

    SDL_UnlockSurface(surface);
    //Get the requested pixel
    return pixels[ ( y * surface->w ) + x ];
}

SDL_Rect* sizeCarImage(SDL_Rect *car_rect, SDL_Texture *car_img)
{
    int carW,carH;

    SDL_QueryTexture(car_img, NULL, NULL, &carW, &carH);

    while(carW>20 && carH>20)
    {
        carW= carW/1.5;
        carH= carH/1.5;
    }
    car_rect->w = carW;
    car_rect->h = carH;

    return car_rect;
}
