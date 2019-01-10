#pragma once


typedef struct paddle{ // enemy paddle;
	SDL_Rect paddle;
}e_paddle, p_paddle;

typedef struct pong{
	pong() = default;
	pong(const SDL_Color &color){}
	pong(float x, float y):
		x_vel(x), y_vel(y){}

	SDL_Rect ball;

	float x_vel;
	float y_vel;
	void update_movement(float dt, float x, float y){
		x_vel = x;
		y_vel = y;

		ball.x += x_vel * dt;
		ball.y += y_vel * dt;
	}
	void handle_collision(float dt, const paddle &player, const paddle &opponent);	

}pong;
using u32 = u_int32_t;
// clock for delta time
struct game_clock{
	u32 last_time = 0;
	float delta_time = 0;

	void tick(){
		u32 tick_time = SDL_GetTicks();
		delta_time = tick_time - last_time;
		last_time = tick_time;
	}
}; 
