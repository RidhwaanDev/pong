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
	/*
	 * First calculate how far the ball is from the center ( rel_intersect )
	 * then normalize that value ( make it between -1 and 1 ) so that we can scale our angle between -1 and 1. If it was not normalized than our bounce_angle when
	 * multiplied by 75 becomes really large  for example ( norm_intersect is 5  * 75 ) 
	 */ 
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

inline float calc(const paddle &p,  int y_collide);
inline float calc_win(const float &ball_pos, const bool isDown);
