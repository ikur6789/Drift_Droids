#ifndef VEHICLE_H
#define VEHICLE_H
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>


class Vehicle
{
    private:
        double xPos, yPos, xVel, yVel, vel, angle, maxVel, minVel, acc, turnSpeed, width, height;
        bool moveForward, moveBack, turnLeft, turnRight;
        Uint32 PixelTL, PixelTR, PixelBL, PixelBR;
    public:
<<<<<<< HEAD
        int a;

        Vehicle(double iniXPos, double iniYPos, double iniAngle, double vHeight, double vWidth, double vehicleMaxVel, double vehicleAcc, double vehicleTurn);

         void move(int imgW, int imgH, SDL_Surface* gameSurface, Uint32 startPixel);

         void handleEvent(SDL_Event e);

         void callEvent(int inputCase);

         double xPos_f();

         double yPos_f();

         double angle_f();

         double spaceUp_f();

         double spaceDown_f();

         double spaceRight_f();

         double spaceLeft_f();

         //Vehicle the_car(car1_rect.x, car1_rect.y, 270, car1_rect.h, car1_rect.w, 0.20, 0.05, 0.25);



=======
int a;

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

>>>>>>> 5794a32bc88dcc654d74f114c82d48c9c05cacb1
};

#endif // VEHICLE_H
