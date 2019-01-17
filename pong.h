#pragma once

struct paddle{ // enemy paddle;
	SDL_Rect paddle;
	int score;
	float vy;
};

struct pong{
	pong(float v_x, float v_y, float x_pos, float y_pos):
		vx(v_x), vy(v_y), x(x_pos), y(y_pos){}	

	SDL_Rect ball;
	float vx;
	float vy;
	float x;
	float y;

	/*
	 * First calculate how far the ball is from the center ( rel_intersect )
	 * then normalize that value ( make it between -1 and 1 ) so that we can scale our angle between -1 and 1. If it was not normalized than our bounce_angle when
	 * multiplied by 75 becomes really large  for example ( norm_intersect is 5  * 75 ) 
	 */ 
	void handle_collision(const paddle &player, const paddle &opponent);	

};
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

float angle(const float &py, const float &bally, const int &h);
void paddle_update( paddle &opponent, const pong &p);
