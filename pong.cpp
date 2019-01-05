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
	int r;
	int g;
	int b;
};

struct e_paddle { // enemy paddle;
	float y;
	float vy;
};
// clock for delta time
struct game_clock{
	u_int32_t last_time = 0;
	float delta_time = 0;
	
	void tick(){
		uint32_t tick_time = SDL_GetTicks();
		delta_time = tick_time - last_time;
		last_time = tick_time;
	}
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

		SDL_Event e;

		SDL_Rect paddle;
		paddle.x = SCREEN_WIDTH / 4;
		paddle.y = y;
		paddle.h = 200;
		paddle.w = 25;

		struct game_clock c;

		SDL_Rect ball_rect;
		ball_rect.w = 8 * 2;
		ball_rect.h = 8 * 2;
		ball_rect.x = SCREEN_WIDTH / 2;

		while(!quit){
			c.tick();	
			
			while(SDL_PollEvent(&e) != 0){
				if(e.type == SDL_QUIT){
					quit = true;
				}
				if(e.type ==  SDL_MOUSEMOTION){
					x = e.motion.x;
					y = e.motion.y;
				}

			} 
			ball_rect.x +=  .1  * (c.delta_time);
			ball_rect.y = y;

			std::cout << "delta time" << " "  << c.delta_time / 1000 << std::endl;	

			SDL_SetRenderDrawColor(gRenderer , 0x00 , 0x00, 0x00, 0xFF);
			SDL_RenderClear(gRenderer);	
			SDL_SetRenderDrawColor(gRenderer , 0xFF , 0xFF, 0xFF, 0xFF);
			SDL_RenderFillRect(gRenderer, &ball_rect);
			SDL_RenderPresent( gRenderer );
			
			SDL_Delay(1000 / 60);
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

				SDL_SetRenderDrawColor(*gRenderer, 0x00,0x00,0x00,0x00);

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
	std::cout << "freed " << tex_tracker.size() <<  "  elements " << std::endl;
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

