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
SDL_Surface* gXOut = NULL;

//The title screen
SDL_Surface* gTitle = NULL;

//The window renderer
//SDL_Renderer* renderer = NULL;

TTF_Font* font = TTF_OpenFont("arial.ttf", 25);

SDL_Color foregroundColor = { 255, 255, 255 };

SDL_Color backgroundColor = { 0, 0, 255 };



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
bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
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
        //SDL_Renderer * renderer = SDL_CreateRenderer(gWindow, -1, 0);
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
	SDL_FreeSurface( gScreenSurface );
	gScreenSurface = NULL;

    //Deallocate surface
    SDL_FreeSurface( gXOut);
    gXOut = NULL;
    
    //Deallocate surface
    SDL_FreeSurface( textSurface);
    textSurface = NULL;
    
	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
    
   //SDL_DestroyTexture(texture);
    //texture = NULL;
    
    TTF_Quit();

	//Quit SDL subsystems
	SDL_Quit();
}


int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
        exit(1);
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
            
                SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Hello World", foregroundColor);
                
                //Event handler
                SDL_Event e;
            
                //Apply the image
                SDL_BlitSurface( gXOut, NULL, gScreenSurface, NULL );
            
                //Update the surface
                SDL_UpdateWindowSurface( gWindow );
            
                //Wait
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
                            SDL_FillRect(gXOut, NULL, SDL_MapRGB(gScreenSurface->format, 0, 0, 0));
                            SDL_BlitSurface(gXOut, NULL, gScreenSurface, NULL);
                            SDL_UpdateWindowSurface( gWindow );
                            //Wait
                            SDL_Delay( 1000 );
                            SDL_BlitSurface(textSurface, NULL, gScreenSurface, NULL);
                            SDL_UpdateWindowSurface( gWindow );
                            
                        }
                    }
                    
                    //Apply the image
                    SDL_BlitSurface( gXOut, NULL, gScreenSurface, NULL );
                    
                    //Update the surface
                    SDL_UpdateWindowSurface( gWindow );
                }
            
        }
    }



	//Free resources and close SDL
	close();

	return 0;
}
