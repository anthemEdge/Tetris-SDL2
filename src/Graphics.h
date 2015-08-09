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

using namespace std;

class Graphics {
public:
	const static int SCREEN_WIDTH = 700;
	const static int SCREEN_HEIGHT = 700;

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
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	TTF_Font* mFont;
};

#endif /* GRAPHICS_H_ */
