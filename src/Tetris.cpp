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
	LTimer frameTimer;
	LTimer fpsTimer;
	fpsTimer.start();
	int framesCounter = 0;
	LTexture fpsTexture;
	fpsTexture.setRenderer(graphics.getRenderer());
	SDL_Color fpsColour = { 0xFF, 0xFF, 0xFF };

	// Load frame rate text font
	TTF_Font* fpsFont = TTF_OpenFont("assets/FFFFORWA.TTF", 12);
	TTF_Font* titleFont = TTF_OpenFont("assets/FFFFORWA.TTF", 96);
	TTF_Font* buttonFont = TTF_OpenFont("assets/FFFFORWA.TTF", 18);
	TTF_Font* gameOverFont = TTF_OpenFont("assets/FFFFORWA.TTF", 32);

	// Title texture
	SDL_Color white = { 0xFF, 0xFF, 0xFF };
	LTexture titleTexture;
	titleTexture.setRenderer(graphics.getRenderer());
	titleTexture.loadFromRenderedText(titleFont, "Tetris", white);

	LTexture startTexture;
	startTexture.setRenderer(graphics.getRenderer());
	startTexture.loadFromRenderedText(buttonFont, "Press [S] to Start", white);

	LTexture quitTexture;
	quitTexture.setRenderer(graphics.getRenderer());
	quitTexture.loadFromRenderedText(buttonFont, "Press [Q] to Quit", white);

	LTexture gameOverTexture;
	gameOverTexture.setRenderer(graphics.getRenderer());
	gameOverTexture.loadFromRenderedText(gameOverFont, "GAME OVER!", white);

	//PlayField setup
	Playfield playField(graphics.getRenderer());
	playField.setScreenSize(graphics.getScreenWidth(),
			graphics.getScreenHeight());

	// Pause game status

	// Game loop
	while (!quit) {
		frameTimer.start();

		// fps counter
		int fpsNum = round(framesCounter / (fpsTimer.getTicks() / 1000.f));
		stringstream fpsSS;
		fpsSS << fpsNum;
		// fps counter: get only average over the last second
		if (fpsTimer.getTicks() > 4000) {
			fpsTimer.start();
			framesCounter = 0;
		}

		// Handle user input
		SDL_Event event;
		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT) {
				quit = true;
			} else if (event.type == SDL_KEYDOWN
					&& event.key.keysym.sym == SDLK_q) {
				quit = true;
			} else if (event.type == SDL_KEYDOWN
					&& event.key.keysym.sym == SDLK_s) {
				if (playField.getGameState() != GAME_STATE_INGAME) {
					playField.start();
				}
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

		if (playField.getGameState() != GAME_STATE_PAUSED) {
			// Playfield render
			playField.tick();
			playField.draw();
		} else {
			titleTexture.render(
					(graphics.SCREEN_WIDTH - titleTexture.getWidth()) / 2,
					graphics.getScreenHeight() / 6);

			startTexture.render(
					(graphics.SCREEN_WIDTH - startTexture.getWidth()) / 2,
					graphics.getScreenHeight() * 3 / 6);

			quitTexture.render(
					(graphics.SCREEN_WIDTH - quitTexture.getWidth()) / 2,
					graphics.getScreenHeight() * 3.5 / 6);
		}

		if (playField.getGameState() != GAME_STATE_INGAME
				&& playField.getGameState() != GAME_STATE_PAUSED) {
			// Draw end game over lay
			SDL_Rect playArea;
			playArea.x = playField.PF_BLOCKSIZE;
			playArea.y = graphics.SCREEN_HEIGHT
					- (playField.PF_HEIGHT - 1) * playField.PF_BLOCKSIZE;
			playArea.w = playField.PF_WIDTH * playField.PF_BLOCKSIZE;
			playArea.h = (playField.PF_HEIGHT - 2) * playField.PF_BLOCKSIZE;

			SDL_SetRenderDrawBlendMode(graphics.getRenderer(),
					SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(graphics.getRenderer(), 0x00, 0x00, 0x00,
					0xA0);
			SDL_RenderFillRect(graphics.getRenderer(), &playArea);

			playArea.h /= 3;
			SDL_SetRenderDrawColor(graphics.getRenderer(), 0x50, 0x50, 0x50,
					0xFF);
			SDL_RenderFillRect(graphics.getRenderer(), &playArea);

			if (playField.getGameState() == GAME_STATE_WON) {
				gameOverTexture.loadFromRenderedText(gameOverFont, "You Win!",
						white);
			} else if (playField.getGameState() == GAME_STATE_LOST) {
				gameOverTexture.loadFromRenderedText(gameOverFont, "GAME OVER!",
						white);
			}

			gameOverTexture.render(
					playArea.x + (playArea.w - gameOverTexture.getWidth()) / 2,
					playArea.y + playField.PF_BLOCKSIZE);

			startTexture.render(
					playArea.x + (playArea.w - startTexture.getWidth()) / 2,
					playArea.y + 3 * playField.PF_BLOCKSIZE);

			quitTexture.render(
					playArea.x + (playArea.w - startTexture.getWidth()) / 2,
					playArea.y + 4 * playField.PF_BLOCKSIZE);

		}
		graphics.render();

		framesCounter++;
	}

	return 0;
}
