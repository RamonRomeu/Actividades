#include <SDL.h>		// Always needs to be included for an SDL app
#include <SDL_image.h>
#include <exception>
#include <iostream>
#include <string>
#include <SDL_ttf.h> //fonts
#include <SDL_mixer.h> //audio
 
//Game general information
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main(int, char*[]) 
{
	//Bool sound dice si la musica esta activa o pausada
	bool sound = true;
	bool play = false;

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
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) throw "Error: SDL_mixer init";

	// --- SPRITES ---
		//Background
		SDL_Texture* bgTexture{ IMG_LoadTexture(m_renderer, "../../res/img/bg.jpg") };
		SDL_Texture* bgTexture2{ IMG_LoadTexture(m_renderer, "../../res/img/bgCastle.jpg") };
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

				//Hover
				tmpSurf = { TTF_RenderText_Blended(font, "My first SDL game",SDL_Color{5,3,121,255}) };
				SDL_Texture *textTexture2{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
				SDL_Rect textRect{ 100,50, tmpSurf->w,tmpSurf->h }; //100 es la posicio de la font


				//Playbutton texture
				tmpSurf = { TTF_RenderText_Blended(font, "Play",SDL_Color{255,150,0,255}) };
				SDL_Texture *playTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };

				 //hover playbutton
				tmpSurf = { TTF_RenderText_Blended(font, "Play",SDL_Color{5,3,121,255}) };
				SDL_Texture *playTexture2{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };

				SDL_Rect playRect{ 100,200, tmpSurf->w,tmpSurf->h };

				//Sound texture

				//Sound ON button

				tmpSurf = { TTF_RenderText_Blended(font, "Sound ON",SDL_Color{255,150,0,255}) };
				SDL_Texture *soundOnTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
					//soundon hover
				tmpSurf = { TTF_RenderText_Blended(font, "Sound ON",SDL_Color{5,3,121,255}) };
				SDL_Texture *soundOnTexture2{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };

				//Sound OFF

				tmpSurf = { TTF_RenderText_Blended(font, "Sound OFF",SDL_Color{255,150,0,255}) };
				SDL_Texture *soundOffTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
					//soundoff hover
				tmpSurf = { TTF_RenderText_Blended(font, "Sound OFF",SDL_Color{5,3,121,255}) };
				SDL_Texture *soundOffTexture2{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };

				SDL_Rect soundRect{ 100,300, tmpSurf->w,tmpSurf->h };

				//Exit texture
				tmpSurf = { TTF_RenderText_Blended(font, "Exit",SDL_Color{255,150,0,255}) };
				SDL_Texture *exitTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
						//exitbutton hover
				tmpSurf = { TTF_RenderText_Blended(font, "Exit",SDL_Color{5,3,121,255}) };
				SDL_Texture *exitTexture2{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };

				SDL_Rect exitRect{ 100,400, tmpSurf->w,tmpSurf->h };

				SDL_FreeSurface(tmpSurf);    //alliberar la surface i puc tornar a fer servir la variable *tmpSurf
				TTF_CloseFont(font);


				



	// --- AUDIO ---
				Mix_Music *music = Mix_LoadMUS("../../res/au/mainTheme.mp3"); if (!music) throw "Error:Music not found";
				//Start background music
				Mix_PlayMusic(music, -1);


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
			case SDL_MOUSEBUTTONDOWN:
				if (event.motion.x <= soundRect.x + soundRect.w && event.motion.x >= soundRect.x && event.motion.y <= soundRect.y + soundRect.h && event.motion.y >= soundRect.y) sound = !sound; //si le das al boton de sonido para la musica o activa
				if (event.motion.x <= exitRect.x + exitRect.w && event.motion.x >= exitRect.x && event.motion.y <= exitRect.y + exitRect.h && event.motion.y >= exitRect.y) SDL_Quit(); //cerrar al darle a exit
				if (event.motion.x <= playRect.x + playRect.w && event.motion.x >= playRect.x && event.motion.y <= playRect.y + playRect.h && event.motion.y >= playRect.y) play = !play; //empezar partida al pulsar play

			
			default:;
			}
		}

		// UPDATE
		playerRect.x += (playerTarget.x - playerRect.x) / 10; //com més a prop esta el nuvol del ratoli més lent va, aixi suavitza el moviment
		playerRect.y += (playerTarget.y - playerRect.y) / 10;

		if (sound) Mix_ResumeMusic(); //reanudar musica
		else Mix_PauseMusic(); //parar musica
		

		// DRAW
		SDL_RenderClear(m_renderer);
			//Background
		SDL_RenderCopy(m_renderer, bgTexture, nullptr, &bgRect);
			//Player 
		SDL_RenderCopy(m_renderer, playerTexture, nullptr, &playerRect);

		if (event.motion.x <= textRect.x+textRect.w && event.motion.x >= textRect.x && event.motion.y <= textRect.y+textRect.h && event.motion.y >= textRect.y)
			SDL_RenderCopy(m_renderer, textTexture2, nullptr, &textRect); //Hoover Text
		else
			SDL_RenderCopy(m_renderer, textTexture, nullptr, &textRect); //TEXT

		//Buttons
			//play
			if (event.motion.x <= playRect.x + playRect.w && event.motion.x >= playRect.x && event.motion.y <= playRect.y + playRect.h && event.motion.y >= playRect.y)
			SDL_RenderCopy(m_renderer, playTexture2, nullptr, &playRect); //Hoover 
			else
				SDL_RenderCopy(m_renderer, playTexture, nullptr, &playRect);//play button

				
			//sound
			if (sound) {
				if (event.motion.x <= soundRect.x + soundRect.w && event.motion.x >= soundRect.x && event.motion.y <= soundRect.y + soundRect.h && event.motion.y >= soundRect.y)
			SDL_RenderCopy(m_renderer, soundOnTexture2, nullptr, &soundRect); //Hoover 
			else
				SDL_RenderCopy(m_renderer, soundOnTexture, nullptr, &soundRect);//play button
				
			}
				else {
				if (event.motion.x <= soundRect.x + soundRect.w && event.motion.x >= soundRect.x && event.motion.y <= soundRect.y + soundRect.h && event.motion.y >= soundRect.y)
					SDL_RenderCopy(m_renderer, soundOffTexture2, nullptr, &soundRect); //Hoover 
				else
					SDL_RenderCopy(m_renderer, soundOffTexture, nullptr, &soundRect);//play button
			}
				
				
			//exit
				if (event.motion.x <= exitRect.x + exitRect.w && event.motion.x >= exitRect.x && event.motion.y <= exitRect.y + exitRect.h && event.motion.y >= exitRect.y)
					SDL_RenderCopy(m_renderer, exitTexture2, nullptr, &exitRect); //Hoover 
				else
					SDL_RenderCopy(m_renderer, exitTexture, nullptr, &exitRect);//play button

		if(play) SDL_RenderCopy(m_renderer, bgTexture2, nullptr, &bgRect);

		SDL_RenderPresent(m_renderer);

	}

	// --- DESTROY ---
	SDL_DestroyTexture(bgTexture);
	SDL_DestroyTexture(bgTexture2);
	SDL_DestroyTexture(playerTexture);
	SDL_DestroyTexture(textTexture);
	SDL_DestroyTexture(textTexture2);
	SDL_DestroyTexture(playTexture);
	SDL_DestroyTexture(playTexture2);
	SDL_DestroyTexture(soundOnTexture);
	SDL_DestroyTexture(soundOnTexture2);
	SDL_DestroyTexture(soundOffTexture);
	SDL_DestroyTexture(soundOffTexture2);
	SDL_DestroyTexture(exitTexture);
	SDL_DestroyTexture(exitTexture2);
	IMG_Quit();
	TTF_Quit();
	Mix_CloseAudio();
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);

	// --- QUIT ---
	SDL_Quit();

	return 0;
}