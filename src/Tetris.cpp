//============================================================================
// Name        : Tetris.cpp
// Author      : anthemEdge
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <sstream>

#include <vector>

#include "Graphics.h"
#include "LTexture.h"
#include "LTimer.h"

#include "Tetromino.h"
#include "Playfield.h"
using namespace std;

int main() {
	// Setup main graphics
	Graphics graphics;
	bool quit = !graphics.init();

	// Define a frame rate cap and timer
	const int FRAME_RATE_CAP = 60;
	const int IDEAL_TICKS_PER_FRAME = 1000 / FRAME_RATE_CAP;
	LTimer frameTimer;
	LTimer fpsTimer;
	fpsTimer.start();
	int fpsCounter = 0;
	LTexture fpsTexture;
	fpsTexture.setRenderer(graphics.getRenderer());
	SDL_Color fpsColour = { 0xFF, 0xFF, 0xFF };

	// Load frame rate text font
	TTF_Font* fpsFont = TTF_OpenFont("assets/FFFFORWA.TTF", 12);
	if (fpsFont == NULL) {
		printf("Failed to load font for fps counter! TTF_Error: %s. \n",
		TTF_GetError());
		quit = true;
	}

	//PlayField setup
	Playfield playField(graphics.getRenderer());
	playField.setScreenSize(graphics.getScreenWidth(),
			graphics.getScreenHeight());
	LTimer playFieldTimer;

	// Pause game status

	// Game loop
	while (!quit) {
		frameTimer.start();

		// fps counter
		int fpsNum = (int) (fpsCounter / (fpsTimer.getTicks() / 1000.f));
		stringstream fpsSS;
		fpsSS << fpsNum;
		// fps counter: get only average over the last second
		if (fpsTimer.getTicks() > 1000) {
			fpsTimer.start();
			fpsCounter = 0;
		}

		// Handle user input
		SDL_Event event;
		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT) {
				quit = true;
			} else if (event.key.keysym.sym == SDLK_q) {
			} else {
				playField.handleEvent(event);
			}
		}

		// Render Loop
		graphics.clear();
		// Render fps counter to the top right corner
		fpsTexture.loadFromRenderedText(fpsFont, fpsSS.str().c_str(),
				fpsColour);
		fpsTexture.render(Graphics::SCREEN_WIDTH - fpsTexture.getWidth(), 0);

		// Playfield render
		playField.tic(playFieldTimer.getTicks());
		playFieldTimer.start();

		playField.draw();

		graphics.render();

		fpsCounter++;
		// Cap frames
		int frameTicks = frameTimer.getTicks();
		if (frameTicks < IDEAL_TICKS_PER_FRAME) {
			SDL_Delay(IDEAL_TICKS_PER_FRAME - frameTicks);
		}
	}

	return 0;
}
