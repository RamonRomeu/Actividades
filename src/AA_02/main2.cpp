#include <SDL.h>		// Always needs to be included for an SDL app
#include <SDL_image.h>
#include <exception>
#include <iostream>
#include <string>
#include <SDL_ttf.h> //fonts
#include <SDL_mixer.h> //audio
#include <time.h>
 
//Game general information
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define GAME_TIME 60
#define CLOCKS_PER_SEC ((clock_t)1000)

const int FPS = 60;
//const float DELAY_TIME = 1000.0f / FPS;

int main(int, char*[]) 
{
	//Bool sound dice si la musica esta activa o pausada
	bool sound = true;
	bool play = false;
	//variables
	bool hooverText=false;
	bool hooverPlay = false;
	bool hooverExit = false;
	bool hooversoundOn = false;
	bool hooversoundOff = false;
	bool click = false;
	bool amunt = false;
	bool avall = false;
	bool esquerra = false;
	bool dreta = false;
	bool amunt2 = false;
	bool avall2 = false;
	bool esquerra2 = false;
	bool dreta2 = false;
	int punts = 0;
	int punts2=0;
	std::string str=" ";
	int timer=GAME_TIME;

	srand(time(NULL));

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
	const Uint8 mixFlags{ MIX_INIT_MP3 | MIX_INIT_OGG };
	if (!(Mix_Init(mixFlags)& mixFlags)) throw "Error: SDL_mixer init";

	// --- SPRITES ---
		//Background
		SDL_Texture* bgTexture{ IMG_LoadTexture(m_renderer, "../../res/img/backgr.png") };
		SDL_Texture* bgTexture2{ IMG_LoadTexture(m_renderer, "../../res/img/grill.png") };
		if (bgTexture == nullptr) throw "Error: bgTexture init";
		SDL_Rect bgRect{ 0,0,SCREEN_WIDTH, SCREEN_HEIGHT };

		//Player 
		SDL_Texture *playerTexture{ IMG_LoadTexture(m_renderer, "../../res/img/heart.png") };
			if (playerTexture == nullptr) throw "No s'han pogut crear les textures";
				SDL_Rect playerRect{0,0,40,40};
				SDL_Rect playerTarget{ 0,0,100,100 };    //serveix per fer una mica de delay, que el nuvol no vagi rapidisim cap al ratoli, suavitza el movient

		//Doggo
				SDL_Texture *goldTexture{ IMG_LoadTexture(m_renderer, "../../res/img/doggo.png") };
				int goldWidth, goldHeight;
				SDL_QueryTexture(goldTexture, NULL, NULL, &goldWidth, &goldHeight);
				SDL_Rect goldRect[3];
				for (int i = 0; i < 3; i++) {
					goldRect[i] = { rand() % 740, rand() % 400 + 130, goldWidth / 6, goldWidth / 6 };
				}

	//-->Animated Sprite ---
				SDL_Texture *spriteTexture{ IMG_LoadTexture(m_renderer,"../../res/img/papy.png")}; //es un sprite gegant que conté 6 sprites diferents 
				SDL_Rect spriteRect, spritePosition;
				int textWidth, textHeight, frameWidth, frameHeight;
				SDL_QueryTexture(spriteTexture, NULL, NULL, &textWidth, &textHeight); //assigna als dos &text l'alçada i l'amplada de la textura que triem
				frameWidth = textWidth / 3; //4 pk sabem que esta dividit en 4 columnes 
				frameHeight = textHeight /4; 
				spritePosition.x = 450;
				spritePosition.y = 280;
				spriteRect.x = 0;
				spriteRect.h = frameHeight;
				spriteRect.w = frameWidth;
				spritePosition.h= frameHeight*2.5;
				spritePosition.w = frameWidth*2.5;
				spriteRect.y = 0;
				int frameTime = 0;

	// -- > Animated Sprite2-- -
				SDL_Texture * sprite2Texture{ IMG_LoadTexture(m_renderer,"../../res/img/sans.png") }; //es un sprite gegant que conté 96 sprites diferents 
				SDL_Rect sprite2Rect, sprite2Position;
				int text2Width, text2Height, frame2Width, frame2Height;
				SDL_QueryTexture(sprite2Texture, NULL, NULL, &text2Width, &text2Height); //assigna als dos &text l'alçada i l'amplada de la textura que triem
				frame2Width = text2Width / 4; //4 pk sabem que esta dividit en 4 columnes 
				frame2Height = text2Height / 4; // 4 pk sabem que esta dividit en 4 files
				sprite2Position.x = 350;
				sprite2Position.y = 300;
				sprite2Rect.x = sprite2Rect.y = 0;
				sprite2Rect.h = frame2Height;
				sprite2Rect.w = frame2Width;
				sprite2Position.h = frame2Height/2.2;
				sprite2Position.w = frame2Width/2.2;
				
	// -- > PUNTS-- -
				//punts1
				SDL_Texture * puntsTexture{ IMG_LoadTexture(m_renderer,"../../res/img/nums.png") }; //es un sprite gegant que conté 96 sprites diferents 
				SDL_Rect puntsRect, puntsPosition;
				int textpuntsWidth, textpuntsHeight, framepuntsWidth, framepuntsHeight;
				SDL_QueryTexture(puntsTexture, NULL, NULL, &textpuntsWidth, &textpuntsHeight); //assigna als dos &text l'alçada i l'amplada de la textura que triem
				framepuntsWidth = textpuntsWidth / 10; 
				framepuntsHeight = textpuntsHeight;
				puntsPosition.x = 60;
				puntsPosition.y = 550;
				puntsRect.x = puntsRect.y = 0;
				puntsRect.h = framepuntsHeight;
				puntsRect.w = framepuntsWidth;
				puntsPosition.h = framepuntsHeight / 2;
				puntsPosition.w = framepuntsWidth / 2;
				//punts1 decimas
				SDL_Texture * puntsdecTexture{ IMG_LoadTexture(m_renderer,"../../res/img/nums.png") }; //es un sprite gegant que conté 96 sprites diferents 
				SDL_Rect puntsdecRect, puntsdecPosition;
				int textpuntsdecWidth, textpuntsdecHeight, framepuntsdecWidth, framepuntsdecHeight;
				SDL_QueryTexture(puntsdecTexture, NULL, NULL, &textpuntsdecWidth, &textpuntsdecHeight); //assigna als dos &text l'alçada i l'amplada de la textura que triem
				framepuntsdecWidth = textpuntsdecWidth / 10;
				framepuntsdecHeight = textpuntsdecHeight;
				puntsdecPosition.x = 25;
				puntsdecPosition.y = 550;
				puntsdecRect.x = puntsdecRect.y = 0;
				puntsdecRect.h = framepuntsdecHeight;
				puntsdecRect.w = framepuntsdecWidth;
				puntsdecPosition.h = framepuntsdecHeight / 2;
				puntsdecPosition.w = framepuntsdecWidth / 2;
				//punts2
				SDL_Texture * punts2Texture{ IMG_LoadTexture(m_renderer,"../../res/img/nums.png") }; //es un sprite gegant que conté 96 sprites diferents 
				SDL_Rect punts2Rect, punts2Position;
				int textpunts2Width, textpunts2Height, framepunts2Width, framepunts2Height;
				SDL_QueryTexture(punts2Texture, NULL, NULL, &textpunts2Width, &textpunts2Height); //assigna als dos &text l'alçada i l'amplada de la textura que triem
				framepunts2Width = textpunts2Width / 10;
				framepunts2Height = textpunts2Height;
				punts2Position.x = 735;
				punts2Position.y = 550;
				punts2Rect.x = punts2Rect.y = 0;
				punts2Rect.h = framepunts2Height;
				punts2Rect.w = framepunts2Width;
				punts2Position.h = framepunts2Height / 2;
				punts2Position.w = framepunts2Width / 2;
				//punts2 decimas
				SDL_Texture * puntsdec2Texture{ IMG_LoadTexture(m_renderer,"../../res/img/nums.png") }; //es un sprite gegant que conté 96 sprites diferents 
				SDL_Rect puntsdec2Rect, puntsdec2Position;
				int textpuntsdec2Width, textpuntsdec2Height, framepuntsdec2Width, framepuntsdec2Height;
				SDL_QueryTexture(puntsdec2Texture, NULL, NULL, &textpuntsdec2Width, &textpuntsdec2Height); //assigna als dos &text l'alçada i l'amplada de la textura que triem
				framepuntsdec2Width = textpuntsdec2Width / 10;
				framepuntsdec2Height = textpuntsdec2Height;
				puntsdec2Position.x = 700;
				puntsdec2Position.y = 550;
				puntsdec2Rect.x = puntsdec2Rect.y = 0;
				puntsdec2Rect.h = framepuntsdec2Height;
				puntsdec2Rect.w = framepuntsdec2Width;
				puntsdec2Position.h = framepuntsdec2Height / 2;
				puntsdec2Position.w = framepuntsdec2Width / 2;
				
	// --- TEXT ---
				TTF_Font *font{ TTF_OpenFont("../../res/ttf/det.ttf",80) }; //80 es el tamany de la font
				if (font == nullptr) throw "No es pot inicialitzar the TTF_Font";
				TTF_Font *font3{ TTF_OpenFont("../../res/ttf/det.ttf",40) }; //80 es el tamany de la font
				if (font3 == nullptr) throw "No es pot inicialitzar the TTF_Font";
			
				SDL_Surface *tmpSurf{ TTF_RenderText_Blended(font, "My first SDL game",SDL_Color{255,255,255,255}) };
				if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
				SDL_Texture *textTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };

				//Hover
				tmpSurf = { TTF_RenderText_Blended(font, "My first SDL game",SDL_Color{5,3,121,255}) };
				SDL_Texture *textTexture2{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
				SDL_Rect textRect{ 80,470, tmpSurf->w,tmpSurf->h }; //100 es la posicio de la font


				//Playbutton texture
				tmpSurf = { TTF_RenderText_Blended(font3, "Play",SDL_Color{255,255,255,255}) };
				SDL_Texture *playTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };

				 //hover playbutton
				tmpSurf = { TTF_RenderText_Blended(font3, "Play",SDL_Color{5,3,121,255}) };
				SDL_Texture *playTexture2{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };

				SDL_Rect playRect{ 80,200, tmpSurf->w,tmpSurf->h };

				//Sound texture

				//Sound ON button

				tmpSurf = { TTF_RenderText_Blended(font3, "Sound ON",SDL_Color{255,255,255,255}) };
				SDL_Texture *soundOnTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
					//soundon hover
				tmpSurf = { TTF_RenderText_Blended(font3, "Sound ON",SDL_Color{5,3,121,255}) };
				SDL_Texture *soundOnTexture2{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };

				//Sound OFF

				tmpSurf = { TTF_RenderText_Blended(font3, "Sound OFF",SDL_Color{255,255,255,255}) };
				SDL_Texture *soundOffTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
					//soundoff hover
				tmpSurf = { TTF_RenderText_Blended(font3, "Sound OFF",SDL_Color{5,3,121,255}) };
				SDL_Texture *soundOffTexture2{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };

				SDL_Rect soundRect{ 80,300, tmpSurf->w,tmpSurf->h };

				//Exit texture
				tmpSurf = { TTF_RenderText_Blended(font3, "Exit",SDL_Color{255,255,255,255}) };
				SDL_Texture *exitTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
						//exitbutton hover
				tmpSurf = { TTF_RenderText_Blended(font3, "Exit",SDL_Color{5,3,121,255}) };
				SDL_Texture *exitTexture2{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };

				SDL_Rect exitRect{ 80,400, tmpSurf->w,tmpSurf->h };

				//Timer texture
				
				SDL_Texture *tempsTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
				SDL_Rect tempsRect = { SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2, tmpSurf->w,tmpSurf->h };

	// --- AUDIO ---
				if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
					throw"unable to initialize SDL_mixer audio systems";
				}
				Mix_Music *soundtrack{ Mix_LoadMUS("../../res/au/song.mp3") };
				if (!soundtrack) throw "unable to load the Mix_Music soundtrack";
				Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
				Mix_PlayMusic(soundtrack, -1);   //el -1 vol dir que es reprodueix en loop, es el nombre de cops que sona
		
	// --- TIME ---

				clock_t lastTime = clock();
				float timeDown = 1;
				float deltaTime = 0;
				//Uint32 frameStart, frameTime;
	
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
				else if (event.key.keysym.sym == SDLK_p) play = false; //tornar al menú
				else if (event.key.keysym.sym == SDLK_RIGHT)  dreta2 = true; 
				else if (event.key.keysym.sym == SDLK_LEFT)  esquerra2 = true; 
				else if (event.key.keysym.sym == SDLK_UP) amunt2 = true;
				else if (event.key.keysym.sym == SDLK_DOWN) avall2 = true; 
				else if (event.key.keysym.sym == SDLK_d)  dreta = true;
				else if (event.key.keysym.sym == SDLK_a) esquerra = true; 
				else if (event.key.keysym.sym == SDLK_w)  amunt = true;
				else if (event.key.keysym.sym == SDLK_s)  avall = true; 
				break;
			case SDL_KEYUP:
				if (event.key.keysym.sym == SDLK_RIGHT)  dreta2 = false; 
				else if (event.key.keysym.sym == SDLK_LEFT)  esquerra2 = false; 
				else if (event.key.keysym.sym == SDLK_UP) amunt2 = false; 
				else if (event.key.keysym.sym == SDLK_DOWN) avall2 = false; 
				else if (event.key.keysym.sym == SDLK_d)dreta = false; 
				else if (event.key.keysym.sym == SDLK_a)  esquerra = false; 
				else if (event.key.keysym.sym == SDLK_w) amunt = false; 
				else if (event.key.keysym.sym == SDLK_s) avall = false; 

				break;

			case SDL_MOUSEMOTION:
				//playerRect.x = event.motion.x;
				//playerRect.y = event.motion.y;
				playerTarget.x = event.motion.x - playerRect.w / 2;		//posa la posició del mouse a la posició del target
				playerTarget.y = event.motion.y - playerRect.h / 2;
				break;
			case SDL_MOUSEBUTTONDOWN:
				click = true;

			default:;
			}
		}

	// UPDATE

		//hitbox gold-sprite1
		for (int i = 0; i < 3; i++) {
			if (goldRect[i].x + goldRect[i].w >= spritePosition.x &&
				goldRect[i].x <= spritePosition.x + spritePosition.w &&
				goldRect[i].y + goldRect[i].h >= spritePosition.y &&
				goldRect[i].y <= spritePosition.y + spritePosition.h)
			{
				goldRect[i].x = rand() % 740;
				goldRect[i].y = rand() % 400 + 130;

				//punts unitats
				puntsRect.x += framepuntsWidth;

				if (puntsRect.x >= textpuntsWidth - frameWidth) {
					puntsRect.x = 0;
					puntsdecRect.x += framepuntsdecWidth; //punts decimes
				}

			}

			//hitbox gold-sprite2
			if (goldRect[i].x + goldRect[i].w >= sprite2Position.x &&
				goldRect[i].x <= sprite2Position.x + sprite2Position.w &&
				goldRect[i].y + goldRect[i].h >= sprite2Position.y &&
				goldRect[i].y <= sprite2Position.y + sprite2Position.h) {
				goldRect[i].x = rand() % 740;
				goldRect[i].y = rand() % 400 + 130;


				//punts unitats

				punts2Rect.x += framepunts2Width;

				if (punts2Rect.x >= textpunts2Width - frame2Width) {
					punts2Rect.x = 0;
					puntsdec2Rect.x += framepuntsdec2Width;
				}

			}
		}

		if (!play) {
			//nuvol
			playerRect.x += (playerTarget.x - playerRect.x) / 10; //com més a prop esta el nuvol del ratoli més lent va, aixi suavitza el moviment
			playerRect.y += (playerTarget.y - playerRect.y) / 10;

			if (sound) Mix_ResumeMusic(); //reanudar musica
			else Mix_PauseMusic(); //parar musica

			//titol
			if (event.motion.x <= textRect.x + textRect.w && event.motion.x >= textRect.x && event.motion.y <= textRect.y + textRect.h && event.motion.y >= textRect.y)
				hooverText = true; //Hoover Text
			else
				hooverText = false; //TEXT

			//butons
				//play
			if (event.motion.x <= playRect.x + playRect.w && event.motion.x >= playRect.x && event.motion.y <= playRect.y + playRect.h && event.motion.y >= playRect.y){
				if (click == true) { 
					play = true;
					click = false; 
					timer = 60;       //reset del joc
					punts2Rect.x = 0;
					puntsRect.x = 0;
					puntsdecRect.x = 0;
					puntsdec2Rect.x = 0;
					spritePosition.x = 450;
					spritePosition.y = 280;
					sprite2Position.x = 350;
					sprite2Position.y = 300;
				}; //empezar partida al pulsar play
			
			}

				//textura
			if (event.motion.x <= playRect.x + playRect.w && event.motion.x >= playRect.x && event.motion.y <= playRect.y + playRect.h && event.motion.y >= playRect.y)
				hooverPlay = true; //Hoover 
			else
				hooverPlay = false;//play button 

				//sound
			if (event.motion.x <= soundRect.x + soundRect.w && event.motion.x >= soundRect.x && event.motion.y <= soundRect.y + soundRect.h && event.motion.y >= soundRect.y) if (click == true) { sound = !sound; click = false; }; //si le das al boton de sonido para la musica o activa

			//textura
			if (sound) {
				if (event.motion.x <= soundRect.x + soundRect.w && event.motion.x >= soundRect.x && event.motion.y <= soundRect.y + soundRect.h && event.motion.y >= soundRect.y)
					hooversoundOn = true; //Hoover 
				else
					hooversoundOn = false;//soundOn button

			}
			else {
				if (event.motion.x <= soundRect.x + soundRect.w && event.motion.x >= soundRect.x && event.motion.y <= soundRect.y + soundRect.h && event.motion.y >= soundRect.y)
					hooversoundOff = true; //Hoover 
				else
					hooversoundOff = false;//soundOff button
			}

			//exit
			if (event.motion.x <= exitRect.x + exitRect.w && event.motion.x >= exitRect.x && event.motion.y <= exitRect.y + exitRect.h && event.motion.y >= exitRect.y) if (click == true) { isRunning = false; click = false; };//cerrar al darle a exit

			//textura
			if (event.motion.x <= exitRect.x + exitRect.w && event.motion.x >= exitRect.x && event.motion.y <= exitRect.y + exitRect.h && event.motion.y >= exitRect.y)
				hooverExit = true; //Hoover 
			else
				hooverExit = false;//exit button
		}
		
				//PARTIDA

				//sprite1
				frameTime++;
				if (FPS / frameTime <= 9) {
					frameTime = 0;
					
					 if (dreta == true) { 
						if (spritePosition.x < SCREEN_WIDTH - 70) //limit dreta
						spritePosition.x += 20;
						spriteRect.y = 2 * frameHeight;
						spriteRect.x += frameWidth;
						if (spriteRect.x >= textWidth-frameWidth )
							spriteRect.x = 0;

					}
					 else if (esquerra == true) {
						 if (spritePosition.x > SCREEN_WIDTH - 800) //limit esquerra
						spritePosition.x -= 20;
						spriteRect.y = 1 * frameHeight;
						spriteRect.x += frameWidth;
						if (spriteRect.x >= textWidth-frameWidth )
							spriteRect.x = 0;

					}
					else if (avall == true) {
						if (spritePosition.y < SCREEN_HEIGHT - 70) //limit baix
						spritePosition.y += 20;
						spriteRect.y = 0;
						spriteRect.x += frameWidth;
						if (spriteRect.x >= textWidth-frameWidth )
							spriteRect.x = 0;

					}
					else if (amunt == true) {
						if (spritePosition.y > SCREEN_HEIGHT - 450) //limit dalt
						spritePosition.y -= 20;
						spriteRect.y = 3 * frameHeight;
						spriteRect.x += frameWidth;  //posem aquí dins perquè al quedarte quiet no es mogui, si ho posesim a fora l'spirte smempre avançaria
						if (spriteRect.x >= textWidth-frameWidth )
							spriteRect.x = 0;

					}

					//sprite2
					if (dreta2==true) {
						if (sprite2Position.x < SCREEN_WIDTH - 70) //limit dreta
						sprite2Position.x += 20;
						sprite2Rect.y = 2*frame2Height;
						sprite2Rect.x += frame2Width;
						if (sprite2Rect.x >= text2Width-frame2Width )
							sprite2Rect.x = 0;
					
					}
					else if (esquerra2 == true) {	
						if (sprite2Position.x > SCREEN_WIDTH - 800) //limit esquerra
						sprite2Position.x -= 20;
							sprite2Rect.y = 3*frame2Height;
							sprite2Rect.x += frame2Width;
							if (sprite2Rect.x >= text2Width-frame2Width)
								sprite2Rect.x = 0;
					
					}
					else if (avall2 == true) {
						 if (sprite2Position.y < SCREEN_HEIGHT - 70) //limit baix
						sprite2Position.y += 20;
							sprite2Rect.y = 0;
							sprite2Rect.x += frame2Width;
							if (sprite2Rect.x >= text2Width-frame2Width ) 
								sprite2Rect.x = 0;
						
					}
					else if (amunt2 == true) {	
						 if (sprite2Position.y > SCREEN_HEIGHT - 450) //limit dalt
							sprite2Position.y -= 20;
							sprite2Rect.y = 1*frame2Height;
							sprite2Rect.x += frame2Width;
							if (sprite2Rect.x >= text2Width-frame2Width )
								sprite2Rect.x = 0;
						
					}

				}

				//fps
				if (play == true) {
					deltaTime = (clock() - lastTime); //es el temps del frame anterior 
					lastTime = clock();
					deltaTime /= CLOCKS_PER_SEC;
					timeDown -= deltaTime; //ens serveix per veure que sigui quin sigui el delta time a tots els ordinadors en va igual, es constant. es una demostració de que funciona

					if (timeDown <= 0) { timer--; timeDown = 1; }
					//std::cout << timer << std::endl;
					
					if (timer <= 0)
						play = false;
				} 
						
				str = std::to_string (timer);
				const char *c = str.c_str();
				std::cout << c << std::endl;

				tmpSurf = { TTF_RenderText_Blended(font, c ,SDL_Color{123,32,43,255}) };
				tempsTexture={ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
				tempsRect={ SCREEN_WIDTH/2-tmpSurf->w/2,0, tmpSurf->w,tmpSurf->h };

		// DRAW
		
			SDL_RenderClear(m_renderer);

			if (play == false) {
				//Background
				SDL_RenderCopy(m_renderer, bgTexture, nullptr, &bgRect);

				//Player 
				SDL_RenderCopy(m_renderer, playerTexture, nullptr, &playerRect);
				//titol
				if (hooverText == true)
					SDL_RenderCopy(m_renderer, textTexture2, nullptr, &textRect); //Hoover Text
				else
					SDL_RenderCopy(m_renderer, textTexture, nullptr, &textRect); //TEXT


					//BUTONS

					//play

				if (hooverPlay == true)
					SDL_RenderCopy(m_renderer, playTexture2, nullptr, &playRect); //Hoover 
				else
					SDL_RenderCopy(m_renderer, playTexture, nullptr, &playRect);//play button

				//sound
				if (sound) {
					if (hooversoundOn == true)
						SDL_RenderCopy(m_renderer, soundOnTexture2, nullptr, &soundRect); //Hoover 
					else
						SDL_RenderCopy(m_renderer, soundOnTexture, nullptr, &soundRect);//soundOn button

				}
				else {
					if (hooversoundOff == true)
						SDL_RenderCopy(m_renderer, soundOffTexture2, nullptr, &soundRect); //Hoover 
					else
						SDL_RenderCopy(m_renderer, soundOffTexture, nullptr, &soundRect);//soundOff button
				}

				//exit
				if (hooverExit == true)
					SDL_RenderCopy(m_renderer, exitTexture2, nullptr, &exitRect); //Hoover 
				else
					SDL_RenderCopy(m_renderer, exitTexture, nullptr, &exitRect);//exit button
			}

				if (play) {
					 SDL_RenderCopy(m_renderer, bgTexture2, nullptr, &bgRect); //obre la pantalla del play

					SDL_RenderCopy(m_renderer, spriteTexture, &spriteRect, &spritePosition); //Animated Sprite

					SDL_RenderCopy(m_renderer, sprite2Texture, &sprite2Rect, &sprite2Position); //Animated Sprite 2
					for (int i = 0; i < 3; i++) {
						SDL_RenderCopy(m_renderer, goldTexture, nullptr, &goldRect[i]); //gold
					}
					
					 SDL_RenderCopy(m_renderer, tempsTexture, nullptr, &tempsRect); //Timer


					 SDL_RenderCopy(m_renderer, puntsTexture, &puntsRect, &puntsPosition); //punts unitat
					 SDL_RenderCopy(m_renderer, puntsdecTexture, &puntsdecRect, &puntsdecPosition); //punts dec

					 SDL_RenderCopy(m_renderer, punts2Texture, &punts2Rect, &punts2Position); //punts unitat player 2
					 SDL_RenderCopy(m_renderer, puntsdec2Texture, &puntsdec2Rect, &puntsdec2Position);
				} //punts dec player 2
			
			SDL_RenderPresent(m_renderer);

	}

	// --- DESTROY ---
	SDL_FreeSurface(tmpSurf);    //alliberar la surface i puc tornar a fer servir la variable *tmpSurf
	TTF_CloseFont(font);
	TTF_CloseFont(font3);
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
	SDL_DestroyTexture(spriteTexture);
	SDL_DestroyTexture(sprite2Texture);
	SDL_DestroyTexture(tempsTexture);
	SDL_DestroyTexture(goldTexture);
	SDL_DestroyTexture(puntsTexture);
	SDL_DestroyTexture(puntsdecTexture);
	SDL_DestroyTexture(punts2Texture);
	SDL_DestroyTexture(puntsdec2Texture);

	IMG_Quit();
	TTF_Quit();
	Mix_CloseAudio();
	Mix_Quit();
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);

	// --- QUIT ---
	SDL_Quit();

	return 0;
}