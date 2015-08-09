/*
 * Playfield.cpp
 *
 *  Created on: 8 Aug 2015
 *      Author: eva
 */

#include "Playfield.h"

Playfield::Playfield(SDL_Renderer* renderer) :
		mRenderer(renderer), screenWidth(0), screenHeight(0), mCurrentPosX(0), mCurrentPosY(
				0) {
	// Initialise primitive
	for (int x = 0; x < PF_WIDTH; x++) {
		for (int y = 0; y < PF_HEIGHT; y++) {
			// Fill every element to be -1
			mPlayField[x][y] = -1;
		}
	}

	// Colour Array
	mColourArray.push_back( { 0x00, 0xFF, 0xFF });	// Cyan
	mColourArray.push_back( { 0xFF, 0xFF, 0x00 });	// Yellow
	mColourArray.push_back( { 0x80, 0x00, 0x80 });	// Purple
	mColourArray.push_back( { 0x00, 0xFF, 0x00 });	// Green
	mColourArray.push_back( { 0xFF, 0x00, 0x00 });	// Red
	mColourArray.push_back( { 0x00, 0x00, 0xFF });	// Blue
	mColourArray.push_back( { 0xFF, 0xA5, 0x00 });	// Orange
	mColourArray.push_back( { 0xFF, 0xFF, 0xFF });	// white, has a index of 7

}

void Playfield::setScreenSize(int width, int height) {
	screenWidth = width;
	screenHeight = height;
}

void Playfield::tic() {
	// the queue for randomly generated tetrominos must have a minmum of 7
	while (mQueue.size() < 7) {
		vector<int> newQueue = randomiser();
		newQueue.insert(newQueue.end(), mQueue.begin(), mQueue.end());
		mQueue = newQueue;
	}


}

void Playfield::draw() {
	SDL_Point playFieldTopLeft = { PF_BLOCKSIZE, screenHeight
			- (PF_HEIGHT - 1) * PF_BLOCKSIZE };

	for (int row = 0; row < PF_WIDTH; row++) {
		// col init at 2, the first 2 rows are not visible
		for (int col = 2; col < PF_HEIGHT; col++) {
			// Block position
			SDL_Rect block;
			block.w = PF_BLOCKSIZE;
			block.h = PF_BLOCKSIZE;
			block.x = playFieldTopLeft.x + PF_BLOCKSIZE * row;
			block.y = playFieldTopLeft.y + PF_BLOCKSIZE * (col - 2);

			// Get the colour of the block
			int element = mPlayField[row][col];
			if (element > 0 && element < 7) {
				SDL_Color colour = mColourArray.at(element);
				SDL_SetRenderDrawColor(mRenderer, colour.r, colour.g, colour.b,
						0xFF);
				SDL_RenderFillRect(mRenderer, &block);
			}

			//  Draw outline
			SDL_Color white = mColourArray.at(mColourArray.size() - 1);
			SDL_SetRenderDrawColor(mRenderer, white.r, white.g, white.b, 0xFF);
			SDL_RenderDrawRect(mRenderer, &block);

		}
	}

}

void Playfield::handleEvent(SDL_Event& event) {
	if (event.type == SDL_KEYDOWN) {
		switch (event.key.keysym.sym) {
		case SDLK_LEFT:
			mCurrentPosX--;
			break;
		case SDLK_RIGHT:
			mCurrentPosX++;
			break;
		}

	}

}

vector<int> Playfield::randomiser() {
	// Generate nature array
	vector<int> random;
	for (int i = 0; i < 7; i++) {
		random.push_back(i);
	}
	// Shuffle and return
	random_shuffle(random.begin(), random.end());
	return random;
}

Playfield::~Playfield() {
}

