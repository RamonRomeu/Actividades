#include <SDL.h>		// Always needs to be included for an SDL app
#include <SDL_image.h>
#include <exception>
#include <iostream>
#include <string>
#include <SDL_ttf.h> //fonts

//Game general information
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main(int, char*[]) 
{
	// --- INIT SDL ---
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) 
		throw "No es pot inicialitzar SDL subsystems";

	// --- WINDOW ---
	SDL_Window *m_window{ SDL_CreateWindow("SDL...", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN) };
	if (m_window == nullptr) 
		throw "No es pot inicialitzar SDL_Window";

	// --- RENDERER ---
	SDL_Renderer *m_renderer{ SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) };
	if (m_renderer == nullptr) 
		throw "No es pot inicialitzar SDL_Renderer"; 

	//-->SDL_Image 
	const Uint8 imgFlags{ IMG_INIT_PNG | IMG_INIT_JPG };
	if (!(IMG_Init(imgFlags) & imgFlags)) throw "Error: SDL_image init";

	//-->SDL_TTF
	if (TTF_Init() != 0) throw "No es pot inicialitzar SDL_ttf";  //permet que hi hagi fonts

	//-->SDL_Mix

	// --- SPRITES ---
		//Background
		SDL_Texture* bgTexture{ IMG_LoadTexture(m_renderer, "../../res/img/bg.jpg") };
		if (bgTexture == nullptr) throw "Error: bgTexture init";
		SDL_Rect bgRect{ 0,0,SCREEN_WIDTH, SCREEN_HEIGHT };

		//Player 
		SDL_Texture *playerTexture{ IMG_LoadTexture(m_renderer, "../../res/img/kintoun.png") };
			if (playerTexture == nullptr) throw "Nos'han pogut crear les textures";
				SDL_Rect playerRect{0,0,350,189};
				SDL_Rect playerTarget{ 0,0,100,100 };    //serveix per fer una mica de delay, que el nuvol no vagi rapidisim cap al ratoli, suavitza el movient
		
		

	//-->Animated Sprite ---

	// --- TEXT ---
				TTF_Font *font{ TTF_OpenFont("../../res/ttf/saiyan.ttf",80) }; //80 es el tamany de la font
				if (font == nullptr) throw "No es pot inicialitzar the TTF_Font";
				SDL_Surface *tmpSurf{ TTF_RenderText_Blended(font, "My first SDL game",SDL_Color{255,150,0,255}) };
				if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
				SDL_Texture *textTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
				SDL_Rect textRect{ 100,50, tmpSurf->w,tmpSurf->h }; //100 es la posicio de la font
				SDL_FreeSurface(tmpSurf);    //alliberar la surface i puc tornar a fer servir la variable *tmpSurf
				TTF_CloseFont(font);


				//Hover
				tmpSurf = { TTF_RenderText_Blended(font, "My first SDL game",SDL_Color{233,152,121,255}) };
				SDL_Texture *textTexture2{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };



	// --- AUDIO ---

	// --- GAME LOOP ---
	SDL_Event event;
	bool isRunning = true;
	while (isRunning) {
		// HANDLE EVENTS
		while (SDL_PollEvent(&event)) {                //devuelve true meintras haya eventos, y en el parametro te pone el evento //ves agafant els events de la pila fins que no en quedi cap, fem servir el while pk els pugui agafar tots els inputs, per si en fem més d'un al mateix frame //funciona com una pila, si agafa un event el treu fora
			switch (event.type) {
			case SDL_QUIT:		                        //es una constante
				isRunning = false; 
				break;
			case SDL_KEYDOWN:	
				if (event.key.keysym.sym == SDLK_ESCAPE) isRunning = false; 
				break;

			case SDL_MOUSEMOTION:
				//playerRect.x = event.motion.x;
				//playerRect.y = event.motion.y;
				playerTarget.x = event.motion.x -50;		//posa la posició del mouse a la posició del target
				playerTarget.y = event.motion.y -50;
				break;

			
			default:;
			}
		}

		// UPDATE
		playerRect.x += (playerTarget.x - playerRect.x) / 10; //com més a prop esta el nuvol del ratoli més lent va, aixi suavitza el moviment
		playerRect.y += (playerTarget.y - playerRect.y) / 10;
		//hover

		if (textRect.x + event.motion.x < textRect.x + 100 && textRect.y + event.motion.y < textRect.y - 50)
			std::cout << "yay"<<std::endl;
			

		// DRAW
		SDL_RenderClear(m_renderer);
			//Background
		SDL_RenderCopy(m_renderer, bgTexture, nullptr, &bgRect);
			//Player 
		SDL_RenderCopy(m_renderer, playerTexture, nullptr, &playerRect);
			//Text 
		SDL_RenderCopy(m_renderer, textTexture, nullptr, &textRect);
			//Hover
		SDL_RenderCopy(m_renderer, textTexture2, nullptr, &textRect);

		SDL_RenderPresent(m_renderer);

	}

	// --- DESTROY ---
	SDL_DestroyTexture(bgTexture);
	SDL_DestroyTexture(playerTexture);
	SDL_DestroyTexture(textTexture);
	SDL_DestroyTexture(textTexture2);
	IMG_Quit();
	TTF_Quit();
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);

	// --- QUIT ---
	SDL_Quit();

	return 0;
}