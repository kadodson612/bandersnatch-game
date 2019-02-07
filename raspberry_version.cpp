#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
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

//The image we will load and show on the screen
SDL_Surface* gStory = NULL;

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
        optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, 0 );
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
            int imgFlags = IMG_INIT_JPG|IMG_INIT_PNG;
	    if( !( IMG_Init( imgFlags ) & imgFlags ) )
            {
                printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                success = false;
            }
	    else {
            	//Get window surface
            	gScreenSurface = SDL_GetWindowSurface( gWindow );
            	if (gScreenSurface == NULL) {
                	success = false;
		}
            }
        }
    }
    
    TTF_Init();
    return success;
}

bool loadMedia(std::string path,SDL_Surface** surface )
{
    bool success = true;
    
    *surface = loadSurface( path );
    if( surface == NULL )
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
        "England. July 1984. You are a young programmer named Stefan Butler with dreams of adapting a choose your own adventure book called Bandersnatch into a video game. You demo your game to Tuckersoft, the biggest video game company, and get an offer. Do you choose to work with Tuckersoft to develop your game? Use the left and right arrow keys to choose. \n \n Yes \t \t No",
        "Ritman from Tuckersoft says you chose the wrong path. The game is released months later and critically panned as 'designed by committee', receiving a score of 2.5/5. \n \n GAME OVER",
        "While working in your bedroom, you make noises of frustration. Dad comes in and asks you to talk about mom, thinking it is because of her anniversary of passing away. Do you talk about mom? \n \n Yes \t \t No",
        "You throw tea at your computer, and it is destroyed. Bandersnatch is not published. \n \n GAME OVER",
        "No, you don\'t want to think about mom, you are already stressed out enough about the game. Why is Dad even bringing it up now? \n \n Shout at Dad \t \t Just keep quiet",
        "You shout at Dad, and you run out of the house. You needed that fresh air. You see Collin, the star game dev who made all of your favorite games, turning the corner. Follow him? \n \n Follow Collin \t \t Stay put",
        "You catch up to Collin who is on the rooftop of a tall building and see that he is snorting something. Collin offers you some. Take the drug? \n \n Why not? \t \t Uh, no",
        "You take the drug and jump off the building. Bandersnatch is not published. \n \n GAME OVER",
        "You don't take the drug, but you feel a pang of betrayal. How could the person you look up to, your hero, be dependent on such lowly substance? \n \n Push Collins \t \t Go back home",
        "There is a policeman patrolling who catches you in the act. You end up in jail immediately, and Bandersnatch is not published. \n \n GAME OVER",
        "Dad forces you to visit your therapist Dr. Haynes. She prescribes you even more medication. When you get back home, you ponder with the medication in your hands. \n \n Take the meds \t \t Throw them away",
        "You took the meds and finished Bandersnatch. It gets a rating of 0/5. \n \n GAME OVER",
        "You throw away the meds. You hack away at your computer until midnight, and the program crashes suddenly. You are fed up with this. Throw tea at the computer? \n \n Yes \t \t No",
        "You hit your desk in frustration. Dad comes in angry, asking what you think you are doing. Well, what does he think? Fiddling your thumbs? You start yelling at him, and him at you. The argument continues into the kitchen, and you just cannot take any more of this. You eye the kitchen knife. You grab it. \n \n Look at the table \t \t Look at Dad",
        "You stab the apple on the table. You go back to your room and finish the game. Bandersnatch receives a 0/5. \n \n GAME OVER",
        "You lose control and the next thing you know, you see Dad on the kitchen floor, bloody and lifeless. You don\'t feel anything. You laugh maniacally. \n \n Chop up Dad's body \t \t Bury Dad's body",
        "You quickly chop up the body into pieces and toss them into a bag. You return to your computer and finish Bandersnatch. The police don\'t even come for you until a neighbor reports a strange smell and Dad\'s office files a missing persons case after a week of missed phone calls. You\'re in jail now as the crazy programmer who killed his Dad, but nothing matters anymore. Bandersnatch is finished, and it receives the 5/5 that it deserves. That you deserve. \n \n GAME OVER",
        "You start panicking as you dig a big enough hole in the backyard. You hear footsteps and glance up - thank God, it's just Collin... But he sees the bloody body next to you and the hole. \n \n Kill Collin \t \t Ask for help"
    };
    
    
    // Initialize a Map of string & vector of int using initializer_list
    std::map<int, std::vector<int> > choiceMap =     {
        { 0, {1, 2} },
        { 1, {100, 100} },
        { 2, {3, 4} },
        { 3, {100, 100} },
        { 4, {5, 10} },
        { 5, {6, 10} },
        { 6, {7, 8}},
        { 7, {100, 100} },
        { 8, {9, 10} },
        { 9, {100, 100} },
        { 10, {11, 12} },
        { 11, {100, 100} },
        { 12, {3, 13} },
        { 13, {14, 15} },
        { 14, {100, 100} },
        { 15, {16, 17} },
        { 16, {100, 100} },
        { 17, {9, 9} }
    };
    
    SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(font, story[0].c_str(), foregroundColor, 700);
    
    //Event handler
    SDL_Event e;
    
    //Current State
    int current_state = 0;
    
    //User's choice
    int choice = 0;
    
    SDL_Rect dstrect;
    dstrect.x = 250;
    dstrect.y = 220;
    
    //Load second title image
    if( !loadMedia("title.jpg", &gXOut) )
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
                gXOut = TTF_RenderText_Blended_Wrapped(font, story[current_state].c_str(), foregroundColor, 710);
               
                //Load second title image
                if( !loadMedia("scenes/" + std::to_string(current_state)+".GIF", &gStory) )
                {
                    printf( "Failed to load media!\n" );
                    exit(1);
                }
                SDL_BlitSurface( gXOut, NULL, gScreenSurface, NULL );
                SDL_BlitSurface(gStory, NULL, gScreenSurface, &dstrect);
                SDL_UpdateWindowSurface( gWindow );
                SDL_Delay( 3000 );
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
