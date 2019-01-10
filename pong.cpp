#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "pong.h"
// variables like width and height and titles
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const std::string TITLE = "Pong";

// keep track of all texture and free at end

std::vector<SDL_Texture*> tex_tracker;

	
// methods 
bool init(SDL_Window**, SDL_Renderer**);
void clean(SDL_Renderer** , SDL_Window** );
void err(const char *func_name, const unsigned line_num);
void handle_collide(pong ball, const e_paddle &player, const e_paddle &enemy);
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

		struct game_clock c;
		pong p(.1,0);
		p.ball.x = ( SCREEN_WIDTH / 2 );
		p.ball.y = y;
		p.ball.h = 16;
		p.ball.w = 16;

		p_paddle player_paddle;
		player_paddle.paddle.x = 0;
		player_paddle.paddle.w = 25;
		player_paddle.paddle.h = 100;
		player_paddle.paddle.y = y;
	
		e_paddle enemy_paddle;
		enemy_paddle.paddle.x = SCREEN_WIDTH - 25;
		enemy_paddle.paddle.w = 25;
		enemy_paddle.paddle.h = 100;
		enemy_paddle.paddle.y = y;


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

			player_paddle.paddle.y = y;
			enemy_paddle.paddle.y = p.ball.y;

			p.update_movement(c.delta_time, p.x_vel, p.y_vel);
			p.handle_collision(c.delta_time, player_paddle, enemy_paddle);


			// std::cout << "delta time" << " "  << c.delta_time / 1000 << std::endl;	

			SDL_SetRenderDrawColor(gRenderer , 0x00 , 0x00, 0x00, 0xFF);
			SDL_RenderClear(gRenderer);	
			SDL_SetRenderDrawColor(gRenderer , 0xFF , 0xFF, 0xFF, 0xFF);
			SDL_RenderFillRect(gRenderer, &p.ball);
			SDL_RenderFillRect(gRenderer, &player_paddle.paddle);
			SDL_RenderFillRect(gRenderer, &enemy_paddle.paddle);
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

void pong::handle_collision(float dt, const paddle &player, const paddle &opponent){
	 std::cout << "handling collision" << std::endl;
	// handle collision with with paddle
	// y-axis collision
	 std::cout << "ball.x " << ball.x << " ball.y " << ball.y << std::endl;
	 std::cout << "paddle.x " << player.paddle.x << " ball.y " << ball.y << std::endl;
	 if(ball.y >= player.paddle.y && ball.y <= player.paddle.y + player.paddle.h){
		if(ball.x == player.paddle.x){
			// collision has happened
			std::cout << "ball collision" << std::endl;
			x_vel *= -1;
		}	
	 }
	 if(ball.y >= opponent.paddle.y && ball.y <= opponent.paddle.y + opponent.paddle.h){
		 
		if(ball.x == opponent.paddle.x){
			// collision has happened
			std::cout << "ball collision" << std::endl;
			x_vel *= -1;
		}	
	 }


	// handle collisions to paddles
	// axis	
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

