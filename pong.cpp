#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "pong.h"
// variables like width and height and titles
const int SCREEN_WIDTH = 740;
const int SCREEN_HEIGHT = 580;
const std::string TITLE = "Pong";

// keep track of all texture and free at end
std::vector<SDL_Texture*> tex_tracker;


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

		struct game_clock c;

		pong p(-.3f,0,SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		p.ball.x = ( SCREEN_WIDTH / 2 );
		p.ball.y = ( SCREEN_HEIGHT / 2 );
		p.ball.h = 16;
		p.ball.w = 16;

		paddle player1;
		player1.paddle.x = 0;
		player1.paddle.w = 25;
		player1.paddle.h = 100;
		player1.paddle.y = y;

		paddle player2;
		player2.paddle.x = SCREEN_WIDTH - 25;
		player2.paddle.w = 25;
		player2.paddle.h = 100;
		player2.paddle.y = SCREEN_HEIGHT / 2;

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
			// handle bounds
			if(player1.paddle.y < 0){
				player1.paddle.y = 0;
			}
			if(player1.paddle.y + player1.paddle.h >= SCREEN_HEIGHT){
				player1.paddle.y = SCREEN_HEIGHT -  player1.paddle.h;
			}

			if(player2.paddle.y < 0){
				player2.paddle.y = 0;
			}
			if(player2.paddle.y + player2.paddle.h > SCREEN_HEIGHT){
				player2.paddle.y = SCREEN_HEIGHT - player2.paddle.h;
			}

			if(p.ball.y < 0){
				p.vy *= -1;
			}
			if(p.ball.y + p.ball.h > SCREEN_HEIGHT){
				p.ball.y = SCREEN_HEIGHT - p.ball.h - 1;
				p.vy *= -1;
			}
   
			// paddle movement
			 if(y < SCREEN_HEIGHT - player1.paddle.h){
				 player1.paddle.y = y;
			 }
			 if(p.ball.y > player2.paddle.y){
			 	player2.paddle.y -= .1 * c.delta_time;
			 } else if(p.ball.y < player2.paddle.y){
			 	player2.paddle.y += .1 * c.delta_time;
			 }


			p.ball.x += p.vx * c.delta_time;
			p.ball.y += p.vy * c.delta_time;

			p.handle_collision(c.delta_time, player1, player2);

// 			std::cout << "x: " << p.ball.x << std::endl;
// 			std::cout << "y: " << p.ball.y << std::endl;
			

			// std::cout << "delta time" << " "  << c.delta_time / 1000 << std::endl;	


			SDL_SetRenderDrawColor(gRenderer , 0x00 , 0x00, 0x00, 0xFF);
			SDL_RenderClear(gRenderer);	
			SDL_SetRenderDrawColor(gRenderer , 0xFF , 0xFF, 0xFF, 0xFF);
			SDL_RenderFillRect(gRenderer, &p.ball);
			SDL_RenderFillRect(gRenderer, &player1.paddle);
			SDL_RenderFillRect(gRenderer, &player2.paddle);
			SDL_SetRenderDrawColor(gRenderer, 0xAA, 0xAA, 0xAA, 0xFF);
			SDL_RenderPresent( gRenderer );

			SDL_Delay(1000 / 60);
		}

		clean(&gRenderer, &gWindow);
		return 0;
	}
}

void pong::handle_collision(float dt, const paddle &player, const paddle &opponent){
	if(SDL_HasIntersection(&ball,&player.paddle)){
		float bounce_angle = angle(player.paddle.y, ball.y, player.paddle.h);
		vx =  .2f *  cos(bounce_angle);
		vy = ((vy > 0) ? -1 : 1) * 0.2f * sin(bounce_angle);
	}

	if(SDL_HasIntersection(&ball,&opponent.paddle)){
		float bounce_angle = angle(opponent.paddle.y, ball.y, opponent.paddle.h);
		vx =  -1 * .2 * cos(bounce_angle);
		vy = ((vy > 0) ? -1 : 1) * 0.2 * sin(bounce_angle);
	}
	
}

float angle(const float &py, const float &bally, const int &h){
	float relative_y = ((py + h) / 2) - bally;
	relative_y = relative_y / ((py + h) / 2);
	relative_y *= 1.2f;
	return relative_y;

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

