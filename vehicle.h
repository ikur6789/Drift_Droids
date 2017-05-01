#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdint.h>
#include <iostream>

#ifndef VEHICLE_H
#define VEHICLE_H


const double pi = 3.1415926536;

const Uint32 blue = 4291577919;
const Uint32 green = 4283216162;
const Uint32 yellow = 4278252287;
const Uint32 red = 4280556781;

class vehicle
{
    private:

        double xPos, yPos, xVel, yVel, vel, angle, hitAngle, maxVel, minVel, acc, turnSpeed, width, height, throttle, turnThrottle;
        Uint32 startPix, PixelTL, PixelTR, PixelBL, PixelBR, TL_line, TR_line, BR_line, BL_line;
        bool offroad, drifting, finishing, braking;
        int i, j, k, l, imageH, imageW, laps, laptest, lastcycles;
        SDL_Surface* gameSur;

    public:

    Uint32 get_pixel( SDL_Surface *surface, int x, int y )
    {
        SDL_LockSurface(surface);
        //Convert the pixels to 32 bit
        Uint32 *pixels = (Uint32 *)surface->pixels;

        SDL_UnlockSurface(surface);
        //Get the requested pixel
        return pixels[ ( y * surface->w ) + x ];
    }

    vehicle(double iniXPos, double iniYPos, double iniAngle, double vHeight, double vWidth, double vehicleMaxVel, double vehicleAcc, double vehicleTurn)
    {
        //Should probably take in a path to an image...
        //Initial position, angle, vehicle values, etc. could be input manually (from file) or calculated by some other function, in theory
        throttle = 0;
        turnThrottle = 0;
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
        offroad = false;
        drifting = false;
        finishing = false;
        braking = false;
        hitAngle = 0.0;
        laps = -1;
        laptest=-1;
        lastcycles=0;
    }

    void move(int imgW, int imgH, SDL_Surface* gameSurface, Uint32 startPixel)
    {
        // Collision detection code for track?
        gameSur = gameSurface;
        imageW = imgW;
        imageH = imgH;
        startPix = startPixel;
        PixelTL = get_pixel(gameSurface, xPos, yPos);
        PixelTR = get_pixel(gameSurface, (xPos+width), yPos);
        PixelBL = get_pixel(gameSurface, xPos, (yPos+height));
        PixelBR = get_pixel(gameSurface, (xPos+width), (yPos+height));

        if((PixelTL!=yellow && PixelTL!=red) || ( PixelTR!=yellow && PixelTR!=red))
           {
               if (offroad) {
                    maxVel = maxVel * 2;
                    minVel = minVel * 2;
               }
               offroad = false;
           }

        if (PixelTL==green && PixelTR==green && PixelBR==green && PixelBL==green)
            finishing=false;

        if(PixelTL==yellow || PixelTR==yellow)
           {
               if (!offroad) {
                    maxVel = maxVel / 2;
                    minVel = minVel / 2;
               }
               offroad = true;
               finishing = false;
           }

        if(PixelTL==red || PixelTR==red || PixelBL==red || PixelBR==red)
            {
                xPos-=xVel;
                yPos-=yVel;

                xVel = cos((angle)*(pi/180)) * vel;
                yVel = sin((angle)*(pi/180)) * vel;
            }

        if(PixelTL==blue && PixelTR==blue)
        {
            if (!finishing && vel>0) {
                laps += 1;
                finishing = true;
            }
            else if (!finishing && vel<0) {
                laps -=1;
                finishing = true;
            }
            offroad = false;
        }

            if (vel/maxVel < throttle && (throttle != 0 && !braking))
                vel += acc;
            if (vel/maxVel > throttle && (throttle != 0 && !braking))
                vel -= acc;
            if (vel/maxVel < 0 && braking)
                vel += acc;
            if (vel/maxVel > 0.002 && braking)
                vel -= acc;
            //if (turnThrottle!=0 && vel>0.002 || vel<-0.002) //Restrict car turning if not in movement
                angle += turnSpeed*turnThrottle;

        if (vel>maxVel) // Restrict speed to the given maxVel
            vel = maxVel;
        else if (vel<minVel)
            vel = minVel;

        if ((turnThrottle > .5 || turnThrottle < -.5) && (vel > .0002 || vel < -.0002) && !drifting) {
            drifting = true;
        }
        else if (drifting) {
            if (throttle > 0) {
                xVel += cos(angle*(pi/180)) * acc; // Convert the angle and acceleration into vert/horiz components
                yVel += sin(angle*(pi/180)) * acc;
            }
            else if (throttle < 0) {
                xVel -= cos(angle*(pi/180)) * acc; // Convert the angle and acceleration into vert/horiz components
                yVel -= sin(angle*(pi/180)) * acc;
            }
            if (vel < .0002 && vel > -.0002)
                drifting = false;
        }
        else{
            xVel = cos(angle*(pi/180)) * vel;
            yVel = sin(angle*(pi/180)) * vel;
        }

        if (xVel>maxVel) // Restrict velocities to the given maxVel
            xVel = maxVel;
        else if (xVel<minVel)
            xVel = minVel;
        if (yVel>maxVel)
            yVel = maxVel;
        else if (yVel<minVel)
            yVel = minVel;

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
            vel += 0.0002;
       else if(vel>0.0)
            vel -= 0.0002;

        if(xVel<0.0)
            xVel += 0.0002;
       else if(xVel>0.0)
           xVel -= 0.0002;
        if(yVel<0.0)
            yVel += 0.0002;
       else if(yVel>0.0)
            yVel -= 0.0002;

    }

    void handleEvent(SDL_Event e)
    {
        if (e.type==SDL_KEYDOWN && e.key.repeat==0){ // The key repeat check is to avoid the default key repeat action.
            switch(e.key.keysym.sym){
                case SDLK_UP: throttle = 1; break;
                case SDLK_DOWN: throttle = -1; break;
                case SDLK_RIGHT: turnThrottle = 0.49; break;
                case SDLK_LEFT: turnThrottle = -0.49; break;
                case SDLK_l: braking=true; break;
            }
        }
        else if (e.type==SDL_KEYUP && e.key.repeat==0){
            switch (e.key.keysym.sym){
                case SDLK_UP: throttle = 0;break;
                case SDLK_DOWN: throttle = 0; break;
                case SDLK_RIGHT: turnThrottle = 0; break;
                case SDLK_LEFT: turnThrottle = 0; break;
                case SDLK_l: braking=false; break;
            }
        }
    }

    //Setter Functions
    void set_vel(double innum)
    {
        if(innum<=100 && innum>=-100)
        {
            throttle=innum/100;
        }
        else
        {
            fprintf(stderr, "warning: Invalid velocity value sent during run\n");
        }
    }
    void set_angle(double innum)
    {
        if(innum < -100 )
            turnThrottle=-1;

        else if(innum > 100)
            turnThrottle=1;
        else
            turnThrottle=innum/100;

    }

    void set_laps(int innum)
    {
        laps=innum;
    }

    void set_lastcycles(int innum)
    {
        lastcycles = innum;
    }

    //Return Functions

    double vel_f(){
        return vel;
    }
    double xPos_f(){
        return xPos;
    }
    double yPos_f(){
        return yPos;
    }
    double angle_f(){
        return angle;
    }
    int laps_f(){
        return laps;
    }
    int lastcycles_f() {
        return lastcycles;
    }
        // The following is all CLEARLY WiP: they're just placeholder functions for calling the space between the car and track edge
    int spaceUp_f(){ // Space in front of the car, from the top left corner
        TL_line = PixelTL;
        i=0;
        angle = fmod(angle, 360);
        if (angle < 315 && angle >= 225){
            while (i<=imageH && TL_line == startPix){
                i++;
                TL_line = get_pixel(gameSur, xPos, yPos+i);
            }
        }
        else if (angle < 45 && angle >= 315){
            while (i<=imageH && TL_line == startPix){
                i++;
                TL_line = get_pixel(gameSur, xPos, yPos+i);
            }
        }
            //std::cout << "i is "<< i;
    return i;
    }
    int spaceDown_f(){ // Space behind the car, from the bottom right corner
        BR_line = PixelBR;
        j=0;
        angle = fmod(angle, 360);
        if (angle < 45 && angle >= -45){
            while (j<=imageH && BR_line == startPix){
                j++;
                BR_line = get_pixel(gameSur, xPos, yPos-j);
            }
        }
           // std::cout << "j is "<< j;
    return j;
    }
    int spaceRight_f(){ // Space to the right of the car, from the top right corner
        TR_line = PixelTR;
        k=0;
        angle = fmod(angle, 360);
        if (angle < 45 && angle >= -45){
            while (k<=imageH && TR_line == startPix){
                k++;
                TR_line = get_pixel(gameSur, xPos+k, yPos);
            }
        }
            //std::cout << "k is "<< k;
    return k;
    }
    int spaceLeft_f(){ // Space to the left of the car, from the bottom left corner
        BL_line = PixelBL;
        l=0;
        angle = fmod(angle, 360);
        if (angle < 45 && angle >= -45){
            while (i<=imageH && BL_line == startPix){
                l++;
                BL_line = get_pixel(gameSur, xPos-l, yPos);
            }
        }
    //std::cout << "l is "<< l;
    return l;
    }

    void updatelaps(unsigned incycles)
    {
        bool bestlap;
        if(laps>0)
        {
            if(laptest!=laps)
        {
            if(lastcycles>(incycles-lastcycles))
                bestlap = true;

            lastcycles=incycles-lastcycles;

            printf("\n Lap: %d Cycles: %d\n Total Cycles: %u", laps, lastcycles, incycles);
            if(bestlap)
            {
                printf("-New best lap!\n");
                bestlap=false;
            }
            laptest=laps;
        }
        }
    }
};

#endif // VEHICLE_H
