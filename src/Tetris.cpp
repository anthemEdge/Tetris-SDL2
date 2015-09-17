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

enum TextureIndex {
	TEXTURE_INDEX_TITLE,
	TEXTURE_INDEX_START,
	TEXTURE_INDEX_QUIT,
	TEXTURE_INDEX_GAMEOVER,
	TEXTURE_INDEX_YOUWIN,
	TEXTURE_INDEX_TOTAL
};

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
	SDL_Color white = { 0xFF, 0xFF, 0xFF };

	// Loading fonts
	TTF_Font* fpsFont = TTF_OpenFont("assets/FFFFORWA.TTF", 12);
	TTF_Font* titleFont = TTF_OpenFont("assets/FFFFORWA.TTF", 96);
	TTF_Font* instructionFont = TTF_OpenFont("assets/FFFFORWA.TTF", 18);
	TTF_Font* resultFont = TTF_OpenFont("assets/FFFFORWA.TTF", 32);

	// Loading textures
	// set renders
	LTexture textureArray[TEXTURE_INDEX_TOTAL];
	for (int i = 0; i < TEXTURE_INDEX_TOTAL; i++) {
		textureArray[i].setRenderer(graphics.getRenderer());
	}
	// Title
	textureArray[TEXTURE_INDEX_TITLE].loadFromRenderedText(titleFont, "Tetris",
			white);
	TTF_CloseFont(titleFont);

	// Start and quit instuctions
	textureArray[TEXTURE_INDEX_START].loadFromRenderedText(instructionFont,
			"Press [S] to Start", white);
	textureArray[TEXTURE_INDEX_QUIT].loadFromRenderedText(instructionFont,
			"Press [Q] to Quit", white);
	TTF_CloseFont(instructionFont);

	// Results
	textureArray[TEXTURE_INDEX_GAMEOVER].loadFromRenderedText(resultFont,
			"GAME OVER!", white);
	textureArray[TEXTURE_INDEX_YOUWIN].loadFromRenderedText(resultFont,
			"YOU WIN!", white);
	TTF_CloseFont(resultFont);

	//PlayField setup
	Playfield playfield(graphics.getRenderer());
	playfield.setScreenSize(graphics.getScreenWidth(),
			graphics.getScreenHeight());

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
				if (playfield.getGameState() != GAME_STATE_INGAME) {
					playfield.start();
				}
			} else {
				playfield.handleEvent(event);
			}
		}

		// Render Loop
		graphics.clear();
		// Render fps counter to the top right corner
		fpsTexture.loadFromRenderedText(fpsFont, fpsSS.str().c_str(), white);
		fpsTexture.draw(Graphics::SCREEN_WIDTH - fpsTexture.getWidth(), 0);

		if (playfield.getGameState() != GAME_STATE_PAUSED) {
			// Playfield render
			playfield.tick();
			playfield.draw();
		} else {
			textureArray[TEXTURE_INDEX_TITLE].draw(
					(graphics.SCREEN_WIDTH
							- textureArray[TEXTURE_INDEX_TITLE].getWidth()) / 2,
					graphics.getScreenHeight() / 6);

			textureArray[TEXTURE_INDEX_START].draw(
					(graphics.SCREEN_WIDTH
							- textureArray[TEXTURE_INDEX_START].getWidth()) / 2,
					graphics.getScreenHeight() * 3 / 6);

			textureArray[TEXTURE_INDEX_QUIT].draw(
					(graphics.SCREEN_WIDTH
							- textureArray[TEXTURE_INDEX_QUIT].getWidth()) / 2,
					graphics.getScreenHeight() * 3.5 / 6);
		}

		if (playfield.getGameState() != GAME_STATE_INGAME
				&& playfield.getGameState() != GAME_STATE_PAUSED) {
			// Draw end game over lay
			SDL_Rect playArea = playfield.getPlayArea();
			SDL_SetRenderDrawBlendMode(graphics.getRenderer(),
					SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(graphics.getRenderer(), 0x00, 0x00, 0x00,
					0xA0);
			SDL_RenderFillRect(graphics.getRenderer(), &playArea);

			playArea.h /= 3;
			SDL_SetRenderDrawColor(graphics.getRenderer(), 0x50, 0x50, 0x50,
					0xFF);
			SDL_RenderFillRect(graphics.getRenderer(), &playArea);

			int resultIndex = TEXTURE_INDEX_GAMEOVER;
			if (playfield.getGameState() == GAME_STATE_WON) {
				resultIndex = TEXTURE_INDEX_YOUWIN;
			}
			textureArray[resultIndex].draw(
					playArea.x
							+ (playArea.w - textureArray[resultIndex].getWidth())
									/ 2, playArea.y + playfield.PF_BLOCKSIZE);

			textureArray[TEXTURE_INDEX_START].draw(
					playArea.x
							+ (playArea.w
									- textureArray[TEXTURE_INDEX_START].getWidth())
									/ 2,
					playArea.y + 3 * playfield.PF_BLOCKSIZE);

			textureArray[TEXTURE_INDEX_QUIT].draw(
					playArea.x
							+ (playArea.w
									- textureArray[TEXTURE_INDEX_QUIT].getWidth())
									/ 2,
					playArea.y + 4 * playfield.PF_BLOCKSIZE);

		}
		graphics.render();

		framesCounter++;
	}

	return 0;
}
