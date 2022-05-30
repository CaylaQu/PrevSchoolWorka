#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "Missile.h" 
#define FPS 60
#define WIDTH 1024
#define HEIGHT 768
#define SPEED 5
using namespace std;

bool g_running = false;
Uint32 g_start, g_end, g_delta, g_fps;
const Uint8* g_keystates;
SDL_Window* g_pWindow;
SDL_Renderer* g_pRenderer;
SDL_Texture* g_pTankTexture; // Every source image needs a SDL_Texture*
SDL_Texture* g_pBGTexture;

SDL_Rect g_player; // For primitive rectangle.
SDL_Rect g_src, g_dst; // For the ship sprite.
// Background doesn't need any SDL_Rects because we want the WHOLE image.
bool firing = false;
bool bullet = false; 
SDL_Point g_mousePos; // SDL_Point is a struct that contains two ints: x and Y

// Init function. Sets up SDL and all subsystems and other (dynamic) memory allocation.
int Init(const char* title, int xPos, int yPos, int width, int height, int flags)
{
	cout << "Initializing game..." << endl;
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) // If initialization is okay.
	{
		// Try to create the SDL_Window.
		g_pWindow = SDL_CreateWindow(title, xPos, yPos, width, height, flags);
		if (g_pWindow != nullptr) // If window creation passes.
		{
			// Try to create the SDL_Renderer. (Back buffer)
			g_pRenderer = SDL_CreateRenderer(g_pWindow, -1, 0);
			if (g_pRenderer != nullptr)
			{
				if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != 0)
				{
					g_pTankTexture = IMG_LoadTexture(g_pRenderer, "WHeavy Tank.png");
					g_pBGTexture = IMG_LoadTexture(g_pRenderer, "Ground.png");
					// Add check of texture pointer later...
				}
				else return false; // Image init failed.
			}
			else return false; // Renderer creation failed.
		}
		else return false; // Window creation failed.
	}
	else return false; // Initialization has failed.
	// If everything is okay, we are here...
	g_fps = (Uint32)round(1 / (double)FPS * 1000);
	g_keystates = SDL_GetKeyboardState(nullptr);
	g_player = { WIDTH / 2, HEIGHT / 2, 35, 55 };

	g_src = { 0, 0, 154, 221 }; // Clips out entire image.
	g_dst = { WIDTH / 2, HEIGHT / 2, 113, 117 }; // On screen location/appearance.

	cout << "Initialization successful!" << endl;
	g_running = true;
	return true;
}

// HandleEvents function. Gets input from user, e.g. mouse/keyboard/gamepad events.
void HandleEvents()
{
	//cout << "Getting input..." << endl;
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT: // Pressing 'X' button of window.
			g_running = false;
			break;
		case SDL_KEYDOWN: // Fires every frame key is down. Just like keystates in Init.
			if (event.key.keysym.sym == SDLK_SPACE) // or ' ' or 32
			{
				if (firing == false)
				{
					cout << "Firing the lazer sir!" << endl;
					firing = true;
				}
			}
			break;
		case SDL_KEYUP: // Single firing event. Good for resets. 
			if (event.key.keysym.sym == SDLK_SPACE) // or ' ' or 32
			{
				cout << "Done firing sir!" << endl;
				firing = false;
			}
			break;
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&g_mousePos.x, &g_mousePos.y);
			//cout << g_mousePos.x << "," << g_mousePos.y << endl;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				if (bullet == false)
				bullet = true;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				bullet = false;
			}
			break;


		}
	}
}

bool KeyDown(SDL_Scancode c)
{
	if (g_keystates != nullptr)
	{
		if (g_keystates[c] == 1) // Key we're testing for is down.
			return true;
	}
	return false;
}

// Update function. Moves objects, performs physics, e.g. projectiles, gravity, collisions.
void Update()
{
	//cout << "Updating game..." << endl;
	if (KeyDown(SDL_SCANCODE_S))
		g_dst.y += SPEED;
	if (KeyDown(SDL_SCANCODE_W))
		g_dst.y -= SPEED;

	if (KeyDown(SDL_SCANCODE_A))
		g_dst.x -= SPEED;
	if (KeyDown(SDL_SCANCODE_D))
		g_dst.x += SPEED;
}

// Render function. Renders changes in game objects to window.
void Render()
{
	//cout << "Rendering changes to window..." << endl;
	SDL_SetRenderDrawColor(g_pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(g_pRenderer);
	// Any drawing goes here...
	//SDL_SetRenderDrawColor(g_pRenderer, 255, 255, 255, 255);
	//SDL_RenderFillRect(g_pRenderer, &g_player); // & is the address-of operator.

	SDL_RenderCopy(g_pRenderer, g_pBGTexture, NULL, NULL); // For the bg.

	if (firing == true) // Just (firing) is okay too.
	{
		SDL_SetRenderDrawColor(g_pRenderer, 255, 128, 0, 255);
		SDL_RenderDrawLine(g_pRenderer, g_dst.x + g_dst.w / 2, g_dst.y + g_dst.h / 2,
			g_dst.x + g_dst.w / 2, g_dst.y - 300);
	}
	SDL_RenderCopy(g_pRenderer, g_pTankTexture, NULL, &g_dst); // For the ship.
	if (bullet == true) // Just (bullet) is okay too.
	{
		SDL_SetRenderDrawColor(g_pRenderer, 15, 15, 15, 255);
		SDL_RenderDrawLine(g_pRenderer, g_dst.x + g_dst.w / 2, g_dst.y + g_dst.h / 2,
			g_mousePos.x, g_mousePos.y);

	}

	
	SDL_RenderPresent(g_pRenderer); // Flip buffers - send data to window.
}

// Clean function. De-initialize SDL and de-allocate memory.
void Clean()
{
	cout << "Cleaning up..." << endl;
	SDL_DestroyRenderer(g_pRenderer);
	SDL_DestroyWindow(g_pWindow);
	SDL_DestroyTexture(g_pTankTexture);
	SDL_DestroyTexture(g_pBGTexture);
	IMG_Quit();
	SDL_Quit();
}

void Wake()
{
	g_start = SDL_GetTicks(); // Gets milliseconds since SDL initialization.
}

void Sleep()
{
	g_end = SDL_GetTicks();
	g_delta = g_end - g_start; // 1055 - 1050 = 5ms
	if (g_delta < g_fps) // if (5ms < 17ms)
		SDL_Delay(g_fps - g_delta); // Engine sleeps for 12ms.
}

// Run function. Contains the primary game loop.
int Run()
{
	if (g_running == true) // If engine is already running.
	{
		return 1;
	}
	if (Init("GAME1007_M1_LastnameFirstname", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0) == false) // If initialization failed.
	{
		return 2;
	}
	// If initialization passes, we are here.
	while (g_running) // Primary game loop. "frame"
	{
		Wake();
		HandleEvents(); // Input.
		Update(); // Processing.
		Render(); // Output.
		if (g_running == true)
			Sleep();
	}
	Clean();
	return 0;
}

// main function. Entry point in every C++ program.
int main(int argc, char* argv[])
{
	return Run();
}