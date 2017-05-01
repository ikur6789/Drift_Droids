/*
Drift Droids Alpa
Language: C++
Uses SDL Library
Coded by Ian Kurzrock, Brian Malsan, Matt Thauvette
Tested by: Alex Stewart, Bryan Berrent
Version 0.52
Last Edit:
3/13/2017 10:39
Last Compile:
3/14/2017 16:04
*/

#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>

SDL_Texture* loadMap(char* mapPath, SDL_Renderer* render, int* w, int* h);
SDL_Texture* loadCar(char* carPath, SDL_Renderer* render);
Uint32 get_pixel(SDL_Surface *surface, int x, int y);
SDL_Surface* loadMapSurface(char* mapPath);

const double pi = 3.1415926536;
class vehicle
{
    private:
        double xPos, yPos, xVel, yVel, vel, angle, maxVel, minVel, acc, turnSpeed, width, height;
        bool moveForward, moveBack, turnLeft, turnRight;
        Uint32 PixelTL, PixelTR, PixelBL, PixelBR;
    public:


    vehicle(double iniXPos, double iniYPos, double iniAngle, double vHeight, double vWidth, double vehicleMaxVel, double vehicleAcc, double vehicleTurn)
    {
        //Should probably take in a path to an image...
        //Initial position, angle, vehicle values, etc. could be input manually (from file) or calculated by some other function, in theory
        moveForward = false;
        moveBack = false;
        turnLeft = false;
        turnRight = false;
        vel = 0.0; // Initializing variables, as constructors typically do
        xVel = 0.0;
        yVel = 0.0;
        xPos = iniXPos;
        yPos = iniYPos;
        angle = iniAngle;
        height = vHeight;
        width = vWidth;
        maxVel = vehicleMaxVel;
        minVel = -maxVel;
        acc = vehicleAcc;
        turnSpeed = vehicleTurn;
    }

    void move(int imgW, int imgH, SDL_Surface* gameSurface, Uint32 startPixel) // Still need collision detection
    {
        // Collision detection code for track?
        PixelTL = get_pixel(gameSurface, xPos, yPos);
        PixelTR = get_pixel(gameSurface, (xPos+width), yPos);
        PixelBL = get_pixel(gameSurface, xPos, (yPos+height));
        PixelBR = get_pixel(gameSurface, (xPos+width), (yPos+height));

        if(PixelTL!=startPixel || PixelTR!=startPixel)
           {
               moveForward=false;
               vel=0;
           }

        if(PixelBL!=startPixel || PixelBR!=startPixel)
           {
               moveBack=false;
               vel=0;
           }

        if (moveForward){ // If the bools are true, turn or move forward/backward
            moveBack=false;
            vel += acc;
        }
        if (moveBack){
            moveForward=false;
            vel -= acc;
        }
        if (turnRight){
            angle += turnSpeed;
        }
        if (turnLeft){
            angle -= turnSpeed;
        }

        // There's currently no friction because it will depend on the track (color), and I haven't done any of that stuff yet

        if (vel>maxVel){ // Restrict speed to the given maxVel
            vel = maxVel;
        }

        if (vel<minVel)
            vel = minVel;

        xVel = cos(angle*(pi/180)) * vel; // Convert the angle and speed into vert/horiz components
        yVel = sin(angle*(pi/180)) * vel;
        xPos += xVel; // and then actually move
        yPos += yVel;

        // Collision detection for map/window edge; this is simple, but only partially done. There are many ways to calculate collision... this is a simple rectangular method, but other options exist. This one has a hard time with diagonal collisions, etc.; perhaps a circular method would be better.
        if ((xPos<0.0) || (xPos+width>(double)(imgW*1.0))){ // Also note that screen_width doesn't exist in this class, and will need to be pulled (with whatever its name actually is) from the class that generates the main window.
            xPos-=xVel;
        }
        if ((yPos<0.0) || (yPos+height>(double)(imgH*1.0))){
            yPos-=yVel;
        }

        if(vel<0.0)
            vel+=0.0002;
        if(vel>0.0)
            vel-=0.0002;

    }

    void handleEvent(SDL_Event e)
    {
        if (e.type==SDL_KEYDOWN && e.key.repeat==0){ // The key repeat check is to avoid the default key repeat action.
            switch(e.key.keysym.sym){
                case SDLK_UP: moveForward = true;break;
                case SDLK_DOWN: moveBack = true; break;
                case SDLK_RIGHT: turnRight = true; break;
                case SDLK_LEFT: turnLeft = true; break;
            }
        }
        else if (e.type==SDL_KEYUP && e.key.repeat==0){
            switch (e.key.keysym.sym){
                case SDLK_UP: moveForward = false; break;
                case SDLK_DOWN: moveBack = false; break;
                case SDLK_RIGHT: turnRight = false; break;
                case SDLK_LEFT: turnLeft = false; break;
            }
        }
    }

    void callEvent(int inputCase){ // This needs to be discussed in documentation
        switch (inputCase)
        {
            case 0: moveForward = true; moveBack = false; break; // Setting one bool true sets its opposite false; this is unnecessary but makes it simpler to call these
            case 1: moveBack = true; moveForward = false; break;
            case 2: turnRight = true; turnLeft = false; break;
            case 3: turnLeft = true; turnRight = false; break;
            case 4: moveForward = false; break;
            case 5: moveBack = false; break;
            case 6: turnRight = false; break;
            case 7: turnLeft = false; break;
        }
    }

    //Returner Functions

    double xPos_f(){
        return xPos;
    }
    double yPos_f(){
        return yPos;
    }
    double angle_f(){
        return angle;
    }
        // The following is all CLEARLY WiP: they're just placeholder functions for calling the space between the car and track edge
    double spaceUp_f(){
    return 0.0;
    }
    double spaceDown_f(){
        return 0.0;
    }
    double spaceRight_f(){
        return 0.0;
    }
    double spaceLeft_f(){
        return 0.0;
    }

};

 int main(int argc, char *argv[])
 {

     bool testbool = false;
     bool testbool2 = true;

    SDL_Window    *gameWindow = NULL;       //Pointer for Window
    SDL_Renderer  *gameRender = NULL;       //Pointer for Renderer
    SDL_Texture   *gameMap    = NULL;       //Pointer for Map Texture
    SDL_Texture   *car1       = NULL;       //Pointer for First Car
    SDL_Surface   *gameSurface= NULL;       //Pointer for Map-Surface(Used later for friction map)

    //Creates window and Renderer
    gameWindow = SDL_CreateWindow("Drift Droids", 100, 100, 50, 50, 0);

    gameRender = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED);


    char mapPath[255];                     //String for map pathname
    char carPath[255];                     //String for car pathname

    if(argv[1] == NULL || argv[2] == NULL)
      {
        fprintf(stderr, "ERROR: No map or car path arguments passed");
        return 4;                               //Error 4 means one or both arguments were not given
      }

    /*Copying filenames from command line arguments to strings for opening*/
    strcpy(mapPath, argv[1]);
    strcpy(carPath, argv[2]);

    int   imgW,                            //Contains map width value for window resizing
          imgH;                            //Contains map height value for window resizing

    gameMap = loadMap(mapPath, gameRender, &imgW, &imgH);
    gameSurface = loadMapSurface(mapPath);
    car1    = loadCar(carPath, gameRender);


    /*Error Handling Block*/
    if(gameMap == NULL)
    {
      fprintf(stderr, "ERROR: Map was unable to be loaded\n");
      if(car1 == NULL)
        {
            fprintf(stderr, "ERROR: Car was unable to be loaded\n");
            exit(3);                           //Error 3 means both car and map could not be loaded
        }
        else
            {
                exit(1);                       //Error 1 means Map could not be loaded, Car untested
            }
    }
    else
    if(car1 == NULL)
    {
      fprintf(stderr, "ERROR: Car was unable to be loaded\n");
      exit(2);                                 //Error 2 means Car could not be loaded, but Map was fine
    }


    /*Changes values of window size intergers*/
    SDL_QueryTexture(gameMap, NULL, NULL, &imgW, &imgH);



    SDL_SetWindowSize(gameWindow, imgW, imgH);

    SDL_Rect map_rect;
    map_rect.x = 0;
    map_rect.y = 0;
    /*Reuses imgW and imgH for rectangle*/
    map_rect.w = imgW;
    map_rect.h = imgH;

    SDL_Rect car1_rect;
    car1_rect.x = 125;
    car1_rect.y = 300;
    /*Aribitrary divide by 32 before a size comparision*/
    car1_rect.w = 25;
    car1_rect.h = 25;

    Uint32 startPixel = get_pixel(gameSurface,car1_rect.x ,car1_rect.y);

    Uint32 PixelTL ;
    Uint32 PixelTR ;
    Uint32 PixelBL ;
    Uint32 PixelBR ;


    double car1_angle =0;

    vehicle the_car(car1_rect.x, car1_rect.y, 270, car1_rect.h, car1_rect.w, 0.20, 0.05, 0.25);

    /*Loads map into memory*/
    SDL_RenderCopyEx(gameRender, gameMap, NULL, &map_rect, 0, NULL, SDL_FLIP_NONE);

    /*Prints map to screen*/
    SDL_RenderPresent(gameRender);

    /*Loads car into memory*/
    SDL_RenderCopyEx(gameRender, car1, NULL, &car1_rect, car1_angle, NULL, SDL_FLIP_NONE);

    SDL_RenderPresent(gameRender);

   /*Keeps Window Open until Escape is pressed or Close Signal*/
    /*Window Can become moveable*/
    SDL_Event user_clicked;
    int continue_game;
    //SDL_DestroyWindow(win2);

    do
      {

        SDL_RenderClear(gameRender);
        SDL_RenderCopy(gameRender, gameMap, NULL, &map_rect);
        car1_rect.x=the_car.xPos_f();
        car1_rect.y=the_car.yPos_f();
        car1_angle =the_car.angle_f();

        the_car.move(imgW, imgH, gameSurface, startPixel);

        SDL_RenderCopyEx(gameRender, car1, NULL, &car1_rect, car1_angle+90, NULL, SDL_FLIP_NONE);
        SDL_RenderPresent(gameRender);
        SDL_PollEvent(&user_clicked);
        the_car.handleEvent(user_clicked);

      }while(user_clicked.type != SDL_QUIT);

   SDL_DestroyWindow(gameWindow);

   return 0;
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


    if((carW>190) || (carH>203))
    {
        fprintf(stderr, "Car is too big, maybe you loaded map first, please switch");
        exit(6);                        //Error 6: Your car image was too large.
    }

    return car;
  }

 //load in base surface to get pixels
  SDL_Surface* loadMapSurface(char* mapPath){

    SDL_Surface *mapSurface = NULL;

    mapSurface = IMG_Load(mapPath);

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
