#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// variables like width and height and titles
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const std::string TITLE = "Pong";

// keep track of all texture and free at end

std::vector<SDL_Texture*> tex_tracker;

// some objects 
struct ball {
	float x;
	float y;
	float vx;
	float vy;
	
};

struct e_paddle { // enemy paddle;
	float y;
	float vy;
};

struct game_clock{
		
};
// methods 
bool init(SDL_Window**, SDL_Renderer**);
void clean(SDL_Renderer** , SDL_Window** );
void err(const char *func_name, const unsigned line_num);

SDL_Texture* loadTexture(const std::string &path, SDL_Renderer* renderer);

int main(int argc, char *args[]){
	SDL_Window *gWindow;
	SDL_Renderer *gRenderer;
	std::cout << __func__ ;
	if(!init(&gWindow,&gRenderer)){
		err(__func__,__LINE__);
		// handle initliazie error	
	} else {
		int x,y = 0;
		bool quit = false;
		
		float last_time = 0;
		float delta_time = 0;
		
		SDL_Event e;

		SDL_Rect paddle;
		paddle.x = SCREEN_WIDTH / 4;
		paddle.y = y;
		paddle.h = 200;
		paddle.w = 25;

		while(!quit){
			
			float current_time = SDL_GetTicks() / 1000;
			// if statement just to make sure the time passed is greater than last_Time ( otherwise delta would be negative);
			if(current_time > last_time){
			// delta time how much time passed since last_frame update
			   delta_time = (current_time - last_time); 
			   last_time  = current_time;
			}

			// std::cout << "delta time" << "  " << delta_time<< std::endl;


			while(SDL_PollEvent(&e) != 0){
				if(e.type == SDL_QUIT){
					quit = true;
				}
				if(e.type ==  SDL_MOUSEMOTION){
					x = e.motion.x;
					y = e.motion.y;
				}

			} 

			
			paddle.x += 1 * (delta_time / 100);
			paddle.y = y;

			SDL_Texture *mario_tex = loadTexture("mario.png", gRenderer);
		// 	SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00 );
		//	SDL_RenderClear( gRenderer );
		//	//Render red filled quad
		//	SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
		//	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );		
		//	SDL_RenderFillRect( gRenderer, &paddle);
			SDL_RenderClear(gRenderer);	
			SDL_RenderCopy(gRenderer, mario_tex,nullptr,nullptr);
			SDL_RenderPresent( gRenderer );

		}

		clean(&gRenderer, &gWindow);
		return 0;
	}
}

bool init(SDL_Window **gWindow, SDL_Renderer **gRenderer){
	
	bool success = true;
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		err(__func__,__LINE__);
		success = false;

	} else {
		*gWindow = SDL_CreateWindow(TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
		if(*gWindow == nullptr){
			err(__func__,__LINE__);
			success = false;
		} else {
			*gRenderer = SDL_CreateRenderer(*gWindow, -1, SDL_RENDERER_ACCELERATED );
			if(*gRenderer == nullptr){
				err(__func__,__LINE__);
				success = false;
			} else {

				SDL_SetRenderDrawColor(*gRenderer, 0xFF,0xFF,0xFF,0xFF);
				int imgflags = IMG_INIT_PNG;
				if(!(IMG_Init(imgflags) & imgflags)){
					err(__func__,__LINE__);
					success = false;
				}
			}
		}
	}
	return success;
}

SDL_Texture* loadTexture(const std::string &path, SDL_Renderer *renderer){
	SDL_Texture* result_texture = nullptr;
	SDL_Surface* loaded_surface = IMG_Load(path.c_str());
	if(loaded_surface == nullptr){
		err(__func__,__LINE__);
	} else {
		result_texture = SDL_CreateTextureFromSurface(renderer,loaded_surface);
		if(result_texture == nullptr){
			err(__func__,__LINE__);
		} else {
			SDL_FreeSurface(loaded_surface);
		}
	}
	tex_tracker.push_back(result_texture);
	return result_texture;
}

void clean(SDL_Renderer**  gRenderer , SDL_Window** gWindow ){
	for(auto &a: tex_tracker){
		SDL_DestroyTexture(a);
		a = nullptr;
	}
	//Destroy window	
	SDL_DestroyRenderer( *gRenderer );
	SDL_DestroyWindow( *gWindow );
	*gWindow = nullptr;
	*gRenderer = nullptr;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

inline void err(const char* func_name, const unsigned line_num){ 
	std::cout << "function name" << " " << func_name << " " << "line number" << " " << line_num<<  std::endl;
	std::cout << "ERROR" << " " << SDL_GetError() << std::endl;
	std::cout << "ERROR" << " " << IMG_GetError() << std::endl;
}

