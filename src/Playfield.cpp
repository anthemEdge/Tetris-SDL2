/*
 * Playfield.cpp
 *
 *  Created on: 8 Aug 2015
 *      Author: eva
 */

#include "Playfield.h"

Playfield::Playfield(SDL_Renderer* renderer) :
		mRenderer(renderer), screenWidth(0), screenHeight(0), mGravityModifier(
				1.0), mCurrentPosX(0), mCurrentPosY(23) {
	// Initialise primitive
	for (int x = 0; x < PF_WIDTH; x++) {
		for (int y = 0; y < PF_HEIGHT; y++) {
			// Fill every element to be -1
			mPlayField[x][y] = -1;
		}
	}
	mPlayField[8][18] = 4;
	mPlayField[2][21] = 2;

	// Colour Array
	mColourArray.push_back( { 0x00, 0xFF, 0xFF });	// Cyan
	mColourArray.push_back( { 0xFF, 0xFF, 0x00 });	// Yellow
	mColourArray.push_back( { 0x80, 0x00, 0x80 });	// Purple
	mColourArray.push_back( { 0x00, 0xFF, 0x00 });	// Green
	mColourArray.push_back( { 0xFF, 0x00, 0x00 });	// Red
	mColourArray.push_back( { 0x00, 0x00, 0xFF });	// Blue
	mColourArray.push_back( { 0xFF, 0xA5, 0x00 });	// Orange
	mColourArray.push_back( { 0xFF, 0xFF, 0xFF });	// white, has a index of 7

	// Genesis
	mCurrentTetromino.generate();

}

void Playfield::setScreenSize(int width, int height) {
	screenWidth = width;
	screenHeight = height;
}

void Playfield::tic(int elapsed) {
	// the queue for randomly generated tetrominos must have a minmum of 7
	while (mQueue.size() < 7) {
		vector<int> newQueue = randomiser();
		newQueue.insert(newQueue.end(), mQueue.begin(), mQueue.end());
		mQueue = newQueue;
	}

	// Update y position
	mCurrentPosY += (DEFAULT_GRAVITY * mGravityModifier * elapsed);

	// To Do: Need to check for legal and bound of the new update
	// Temp
	bool newPiece = false;

	// Can't move down anymore
	if (!isLegal(mCurrentPosX, mCurrentPosY)) {
		// Reverse position update
		// THIS IS NOT THE CORRECT METHODE AND WILL CAUSE PIECES TO GO MISING AT HIGH SPEED
		mCurrentPosY -= (DEFAULT_GRAVITY * mGravityModifier * elapsed);
		lock();
		newPiece = true;
	}
	// printf("CurrentY: %f\n", mCurrentPosY);

	if (newPiece) {
		newTetromino();
	}

}

void Playfield::draw() {
	// Top left point of the play Field
	SDL_Point playFieldTopLeft = { PF_BLOCKSIZE, screenHeight
			- (PF_HEIGHT - 1) * PF_BLOCKSIZE };

	// Drawing the play Area
	//
	SDL_Rect playArea;
	playArea.x = playFieldTopLeft.x;
	playArea.y = playFieldTopLeft.y;
	playArea.w = PF_WIDTH * PF_BLOCKSIZE;
	playArea.h = (PF_HEIGHT - 2) * PF_BLOCKSIZE;
	SDL_Color grey = { 0x80, 0x80, 0x80 };
	SDL_Color white = mColourArray.at(7);
	SDL_SetRenderDrawColor(mRenderer, grey.r, grey.g, grey.b, 0xFF);
	SDL_RenderFillRect(mRenderer, &playArea);
	SDL_SetRenderDrawColor(mRenderer, white.r, white.g, white.b, 0xFF);
	SDL_RenderDrawRect(mRenderer, &playArea);

	// Drawing the Current Tetromino and its ghost pieces
	// Draw X and draw Y are rounded depending on current TType
	double roundedY = roundY(mCurrentPosY);

	// Centre of the Tetromino on display
	// -2 on y to correct for hidden rows
	TetrominoCRS centreOnDisplay(
			playFieldTopLeft.x + mCurrentPosX * PF_BLOCKSIZE,
			playFieldTopLeft.y + (roundedY - 2) * PF_BLOCKSIZE);

	double ghostY = playFieldTopLeft.y + (project() - 2) * PF_BLOCKSIZE;

	vector<TetrominoCRS> blockPos = mCurrentTetromino.getBlockPos();
	for (vector<TetrominoCRS>::iterator it = blockPos.begin();
			it != blockPos.end(); it++) {

		// Main Tetromino
		SDL_Point blockTopLeft;
		// - 0.5 to rechieve the topleft conor
		blockTopLeft.x = centreOnDisplay.x + (it->x - 0.5) * PF_BLOCKSIZE;
		blockTopLeft.y = centreOnDisplay.y + (it->y - 0.5) * PF_BLOCKSIZE;
		if (blockTopLeft.y >= playFieldTopLeft.y) {
			drawBlock(blockTopLeft, mCurrentTetromino.getTType());
		}

		// Ghost Tetromino

		SDL_Point ghostTopLeft;
		ghostTopLeft.x = blockTopLeft.x;
		ghostTopLeft.y = ghostY + (it->y - 0.5) * PF_BLOCKSIZE;

		if (ghostTopLeft.y >= playFieldTopLeft.y
				&& ghostTopLeft.y < playArea.y + playArea.h) {
			drawBlock(ghostTopLeft, mCurrentTetromino.getTType(), true);
			//printf("Ghost can be draw\n");
		}

	}

	// Drawing elements of the board
	for (int row = 0; row < PF_WIDTH; row++) {
		// col init at 2, the first 2 rows are not visible
		for (int col = 2; col < PF_HEIGHT; col++) {
			// Block position
			SDL_Point blockTopLeft;
			blockTopLeft.x = playFieldTopLeft.x + PF_BLOCKSIZE * row;
			blockTopLeft.y = playFieldTopLeft.y + PF_BLOCKSIZE * (col - 2);

			// Get the colour of the block
			int element = mPlayField[row][col];
			// if the element is valid
			if (element >= 0 && element < 7) {
				drawBlock(blockTopLeft, element);
			}
		}
	}

}

void Playfield::handleEvent(SDL_Event& event) {
	if (event.type == SDL_KEYDOWN) {
		switch (event.key.keysym.sym) {
		case SDLK_LEFT:
			if (isLegal(mCurrentPosX - 1, mCurrentPosY)) {
				mCurrentPosX--;
			}
			break;
		case SDLK_RIGHT:
			if (isLegal(mCurrentPosX + 1, mCurrentPosY)) {
				mCurrentPosX++;
			}
			break;
		case SDLK_UP:
			mCurrentPosY = 18;
			break;
		case SDLK_SPACE:
			mCurrentPosY = project();
			lock();
			newTetromino();
			break;
		case SDLK_a:
			mCurrentTetromino.antiClock();
			if (!isLegal(mCurrentPosX, mCurrentPosY)) {
				mCurrentTetromino.clock();
			}
			break;
		case SDLK_o:
			mCurrentTetromino.clock();
			if (!isLegal(mCurrentPosX, mCurrentPosY)) {
				mCurrentTetromino.antiClock();
			}
			break;
		}
		// Need to check if the move is legal or out of bound
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

void Playfield::newTetromino() {
	//printf("newPiece\n");
	// Generate new Piece
	mCurrentTetromino.generate(mQueue.back());

	// Update new positions
	switch (mQueue.back()) {
	case TTYPE_I:
	case TTYPE_O:
		mCurrentPosX = 5;
		mCurrentPosY = 1;
		break;
	case TTYPE_T:
	case TTYPE_S:
	case TTYPE_Z:
	case TTYPE_J:
	case TTYPE_L:
		mCurrentPosX = 4.5;
		mCurrentPosY = 1.5;
		break;
	}
	// Remove generate piece from the queue
	mQueue.pop_back();
}

void Playfield::drawBlock(SDL_Point& topLeft, int tType, bool ghost,
		bool outline) {
	Uint8 transparency = 0xFF;

	if (ghost) {
		transparency = 0x80;
		SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);
		//printf("Drawing ghost at >> %i:%i\n", topLeft.x, topLeft.y);
	}

	// Forming block
	SDL_Rect block;

	block.w = PF_BLOCKSIZE;
	block.h = PF_BLOCKSIZE;
	block.x = topLeft.x;
	block.y = topLeft.y;

	// Drawing block
	SDL_Color colour = mColourArray.at(tType);
	SDL_SetRenderDrawColor(mRenderer, colour.r, colour.g, colour.b,
			transparency);
	SDL_RenderFillRect(mRenderer, &block);

// Drawing outline
	if (outline) {
		SDL_Color white = mColourArray.at(mColourArray.size() - 1);
		SDL_SetRenderDrawColor(mRenderer, white.r, white.g, white.b,
				transparency);
		SDL_RenderDrawRect(mRenderer, &block);
	}

}

bool Playfield::isLegal(double x, double y) {

	bool legal = true;
	// Ronud y
	y = roundY(y);
	// printf("Legal Ronuded Y : %f\n", y);

	// Check every single block of current Tetromino
	vector<TetrominoCRS> blockPosArray = mCurrentTetromino.getBlockPos();

	// printf("BlockposArraySize: %i\n", blockPosArray.size());

	for (vector<TetrominoCRS>::iterator it = blockPosArray.begin();
			it != blockPosArray.end(); it++) {

		// Get top left point
		SDL_Point topLeft = { (int) (x + (it->x - 0.5)), (int) (y
				+ (it->y - 0.5)) };
		// printf("Top Left Y: %f\n", topLeft.y);

		// Check for bound
		if (topLeft.x < 0 || topLeft.x >= PF_WIDTH) {
			legal = false;
		}
		if (topLeft.y < 0 || topLeft.y >= PF_HEIGHT) {
			legal = false;
		}
		// Check for collision
		if (mPlayField[topLeft.x][topLeft.y] != -1) {
			legal = false;
		}
	}

	// printf("Legal: %i\n", legal);

	return legal;
}

void Playfield::lock() {
	//printf("%i is Locked\n", mCurrentTetromino.getTType());
	// Ronud y
	double y = roundY(mCurrentPosY);

	// Check every single block of current Tetromino
	vector<TetrominoCRS> blockPosArray = mCurrentTetromino.getBlockPos();
	for (vector<TetrominoCRS>::iterator it = blockPosArray.begin();
			it != blockPosArray.end(); it++) {
		// Get top left point
		int xIndex = (int) (mCurrentPosX + (it->x - 0.5));
		int yIndex = (int) (y + (it->y - 0.5));

		//printf("Locking X: %i Y: %i", xIndex, yIndex);

		mPlayField[xIndex][yIndex] = mCurrentTetromino.getTType();

	}
}

double Playfield::project() {
	double y = roundY(mCurrentPosY);
	while (isLegal(mCurrentPosX, y + 1)) {
		y++;
	}
	return y;
}

double Playfield::roundY(double y) {
// Ronud y
	if (mCurrentTetromino.getTType() < 2) {
		// Round to integer
		y = round(y);
	} else {
		// Ronud to half integer
		y = round(y - 0.5) + 0.5;
	}
	return y;
}

Playfield::~Playfield() {
}

