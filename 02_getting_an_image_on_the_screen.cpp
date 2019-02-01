/*This source code copyrighted by Lazy Foo' Productions (2004-2019)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <stdio.h>
#include <string>

//Screen dimension constants
const int SCREEN_WIDTH = 720;
const int SCREEN_HEIGHT = 480;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;
	
//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The image we will load and show on the screen
SDL_Surface* gHelloWorld = NULL;

//The image we will load and show on the screen
SDL_Surface* gXOut = NULL;

//The title screen
SDL_Surface* gTitle = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

TTF_Font* font = TTF_OpenFont("ARIAL.TTF", 12);

SDL_Color foregroundColor = { 255, 255, 255 };

SDL_Color backgroundColor = { 0, 0, 255 };

SDL_Surface* textSurface = TTF_RenderText_Shaded(font, "This is my text.", foregroundColor, backgroundColor);

// Pass zero for width and height to draw the whole surface
SDL_Rect textLocation = { 100, 100, 0, 0 };


SDL_Surface* loadSurface( std::string path )
{
    //The final optimized image
    SDL_Surface* optimizedSurface = NULL;
    
    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
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
bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 || TTF_Init() < 0)
	{
		printf( "SDL or TTF could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow( "Bandersnatch", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Get window surface
			gScreenSurface = SDL_GetWindowSurface( gWindow );
		}
	}

	return success;
}

bool loadMedia(std::string path)
{
    //Loading success flag
    bool success = true;
    
    //Load splash image
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
	//Deallocate surface
	SDL_FreeSurface( gHelloWorld );
	gHelloWorld = NULL;

	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}


int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia("bandersnatch-logo.png") )
		{
			printf( "Failed to load media!\n" );
		}
		else

        {
                //Main loop flag
                bool quit = false;
            
                bool gameRunning = false;
                
                //Event handler
                SDL_Event e;
            
                //Apply the image
                SDL_BlitSurface( gXOut, NULL, gScreenSurface, NULL );
            
                //Update the surface
                SDL_UpdateWindowSurface( gWindow );
            
                //Wait two seconds
                SDL_Delay( 5000 );
            
                //Load media
                if( !loadMedia("title.jpg") )
                {
                    printf( "Failed to load media!\n" );
                }
                
                //While title screen is running
                while( !quit )
                {
                    //Handle events on queue
                    while( SDL_PollEvent( &e ) != 0 )
                    {
                        //User requests quit
                        if( e.type == SDL_QUIT )
                        {
                            quit = true;
                        }
                        else if (e.type == SDL_MOUSEBUTTONUP )
                        {
                            SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0, 0, 0));
                            SDL_UpdateWindowSurface( gWindow );
                            gameRunning = true;
                            quit = true;
                        }
                    }
                    
                    //Apply the image
                    SDL_BlitSurface( gXOut, NULL, gScreenSurface, NULL );
                    
                    //Update the surface
                    SDL_UpdateWindowSurface( gWindow );
                }
            
                int current_state = 0;
                int end_state = 1;
            
                //While normal operation of game is running
                while (gameRunning && current_state < end_state)
                {
                    SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0, 0, 0));
                    SDL_BlitSurface(textSurface, NULL, gScreenSurface, &textLocation);
                    SDL_UpdateWindowSurface( gWindow );
                    
                    //Handle events on queue
                    if( SDL_PollEvent( &e ) != 0 )
                    {
                        //User requests quit
                        if( e.type == SDL_QUIT )
                        {
                            gameRunning = false;
                        }
                    }
                    
                }
        }
    }



	//Free resources and close SDL
	close();

	return 0;
}
