/*
 * Graphics.h
 *
 *  Created on: 26 Jul 2015
 *      Author: eva
 */

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "LTimer.h"
using namespace std;

class Graphics {
public:
	const static int SCREEN_WIDTH = 476;
	const static int SCREEN_HEIGHT = 672;
	const static int TICKS_PER_FRAME_LIMIT = 16;

public:
	Graphics();
	bool init();

	void clear();
	void render();

	int getScreenWidth();
	int getScreenHeight();

	virtual ~Graphics();

	SDL_Renderer* getRenderer();

private:
	LTimer mFrameTimer;		// For system without VSync
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	TTF_Font* mFont;
};

#endif /* GRAPHICS_H_ */
