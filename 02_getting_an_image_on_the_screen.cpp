#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <map>
#include <vector>

//Screen dimension constants
const int SCREEN_WIDTH = 720;
const int SCREEN_HEIGHT = 480;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Clears screens/surfaces
void clearSurfaces();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The surface contained by the window
SDL_Surface* textSurface = NULL;

//The image we will load and show on the screen
SDL_Surface* gXOut = NULL;

//The title screen
SDL_Surface* gTitle = NULL;

SDL_Color backgroundColor = { 0, 0, 255 };

// Pass zero for width and height to draw the whole surface
SDL_Rect textLocation = { 100, 100, 0, 0 };

//Creates an optimized surface with an image given image path
SDL_Surface* loadSurface( std::string path )
{
    //The final optimized image
    SDL_Surface* optimizedSurface = NULL;
    
    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
        return NULL;
    }
    else
    {
        //Convert surface to screen format
        optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, NULL );
        if( optimizedSurface == NULL )
        {
            printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        
        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }
    
    return optimizedSurface;
}

void clearSurfaces()
{
    SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0, 0, 0));
    SDL_FillRect(gXOut, NULL, SDL_MapRGB(gScreenSurface->format, 0, 0, 0));
}

bool init()
{
    bool success = true;
    
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL or TTF could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
        return success;
    }
    else
    {
        //Create window
        gWindow = SDL_CreateWindow( "Bandersnatch", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
            return success;
        }
        else
        {
            //Get window surface
            gScreenSurface = SDL_GetWindowSurface( gWindow );
            if (gScreenSurface == NULL) {
                return false;
            }
        }
    }
    TTF_Init();
    return success;
}

bool loadMedia(std::string path)
{
    bool success = true;
    
    gXOut = loadSurface( path );
    if( gXOut == NULL )
    {
        printf( "Unable to load image %s! SDL Error: %s\n", path.c_str() , SDL_GetError() );
        success = false;
    }
    
    return success;
}

void close()
{
    //Deallocate surfaces
    SDL_FreeSurface( gScreenSurface );
    gScreenSurface = NULL;
    
    SDL_FreeSurface( gXOut);
    gXOut = NULL;
    
    SDL_FreeSurface( textSurface);
    textSurface = NULL;
    
    //Destroy Window
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    
    TTF_Quit();
    
    //Quit SDL subsystems
    SDL_Quit();
}


int main( int argc, char* args[] )
{
    //Start up SDL and create window
    if( !init() )
    {
        printf( "Failed to initialize game.\n" );
        exit(1);
    }
    bool quit = false;
    
    TTF_Font* font = TTF_OpenFont("orange kid.ttf", 25);
    
    SDL_Color foregroundColor = { 255, 255, 255 };
    
    std::string story[100] = {
        "England. July 1984. You are a young programmer named Stefan Butler with dreams of adapting a choose your own adventure book called Bandersnatch into a video game. Do you choose to work with Tuckersoft to develop your game? Use the left and right arrow keys to choose. \n \n Yes \t \t No",
        "Ritman from Tuckersoft says you chose the wrong path. The game is released months later and critically panned as 'designed by committee'. \n \n GAME OVER",
        "Your dad asks you to talk about mom. Do you talk about mom? \n \n Yes \t \t No",
        "You threw tea on your computer, ruining all your work. \n \n GAME OVER",
        "You have to visit your psychiatrist, Dr. Haynes. Do you take the meds that he prescribes to you or throw them in the garbage? \n \n Take \t \t Throw Away",
        "After taking the meds, your game gets published with a bad rating, saying that it seemed like the developer was on autopilot. Dagnabbit. \n \n GAME OVER",
        "You're really mad. Do you decide to kill Dad? \n \n Yes \t \t No",
        "Your game was published at 2.5/5 stars. \n \n GAME OVER",
        "You went to jail. On the bright side, your game was rated at 5/5 stars. \n \n GAME OVER",
    };
    
    
    // Initialize a Map of string & vector of int using initializer_list
    std::map<int, std::vector<int> > choiceMap =     {
        { 0, {1, 2} },
        { 1, {100, 100} },
        { 2, {3, 4} },
        { 3, {100, 100} },
        { 4, {5, 6} },
        { 5, {100, 100} },
        { 6, {7, 8}},
        { 7, {100, 100} },
        { 8, {100, 100} }
    };
    
    SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(font, story[0].c_str(), foregroundColor, 700);
    
    //Event handler
    SDL_Event e;
    
    //Current State
    int current_state = 0;
    
    //User's choice
    int choice = 0;
    
    //Load second title image
    if( !loadMedia("title.jpg") )
    {
        printf( "Failed to load media!\n" );
    }
    
    //While title screen is running
    while( !quit && current_state < 100)
    {
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 && current_state < 100)
        {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                quit = true;
            }
            else if (e.type == SDL_MOUSEBUTTONUP && current_state == 0)
            {
                clearSurfaces();
                SDL_BlitSurface(gXOut, NULL, gScreenSurface, NULL);
                SDL_UpdateWindowSurface( gWindow );
                SDL_Delay( 2000 );
                gXOut = textSurface;
                
            }
            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_RIGHT) choice = 1;
                else if (e.key.keysym.sym == SDLK_LEFT) choice = 0;
                else continue;
                
                //Update State
                current_state = choiceMap[current_state][choice];
                if(current_state == 100) {
                    close();
                    return 0;
                }
                clearSurfaces();
                gXOut = TTF_RenderText_Blended_Wrapped(font, story[current_state].c_str(), foregroundColor, 700);
                SDL_Delay( 2000 );
                SDL_BlitSurface( gXOut, NULL, gScreenSurface, NULL );
                
                //Update the surface
                SDL_UpdateWindowSurface( gWindow );
                SDL_Delay( 2000 );
            }
        }
        //Apply the image
        SDL_BlitSurface( gXOut, NULL, gScreenSurface, NULL );
        
        //Update the surface
        SDL_UpdateWindowSurface( gWindow );
    }
    
    //Free resources and close SDL
    close();
    
    return 0;
}
