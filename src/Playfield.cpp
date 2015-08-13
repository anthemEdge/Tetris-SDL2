/*
 * Playfield.cpp
 *
 *  Created on: 8 Aug 2015
 *      Author: eva
 */

#include "Playfield.h"

Playfield::Playfield(SDL_Renderer* renderer) :
		mRenderer(renderer), screenWidth(0), screenHeight(0), mLevel(1), mCurrentPosX(
				0), mCurrentPosY(0), mFont(NULL), mHoldLock(false), mLockDelay(
				500), mGameOver(false), mLeft(false), mRight(false) {

	// Colour Array
	mColourArray.push_back( { 0x00, 0xBF, 0xFF });	// Cyan
	mColourArray.push_back( { 0xFF, 0xD7, 0x00 });	// Yellow
	mColourArray.push_back( { 0x80, 0x00, 0x80 });	// Purple
	mColourArray.push_back( { 0x00, 0xFF, 0x00 });	// Green
	mColourArray.push_back( { 0xFF, 0x00, 0x00 });	// Red
	mColourArray.push_back( { 0x00, 0x00, 0xFF });	// Blue
	mColourArray.push_back( { 0xFF, 0x8C, 0x00 });	// Orange
	mColourArray.push_back( { 0xFF, 0xFF, 0xFF });	// white, has a index of 7

	// Load Text font
	mFont = TTF_OpenFont("assets/FFFFORWA.TTF", PF_BLOCKSIZE);
	if (mFont == NULL) {
		printf("Failed to load font for play field! TTF_Error: %s. \n",
		TTF_GetError());
	}
	mNext.setRenderer(mRenderer);
	mNext.loadFromRenderedText(mFont, "NEXT:", mColourArray.at(7));
	mHold.setRenderer(mRenderer);
	mHold.loadFromRenderedText(mFont, "HOLD: ", mColourArray.at(7));

	// Randem Seed
	srand(time(NULL));

	init();

}

void Playfield::init() {
	//printf("Init Called\n");
	// Initialise primitive
	for (int x = 0; x < PF_WIDTH; x++) {
		for (int y = 0; y < PF_HEIGHT; y++) {
			// Fill every element to be -1
			mPlayField[x][y] = -1;
		}
	}
	mGame.start();	// Start timer
	checkQueue();
	// Genesis
	newTetromino();
	//printf("New Tetromino\n");
}

void Playfield::reset() {
	mLevel = 1;
	mCurrentPosX = 0;
	mCurrentPosY = 0;
	mHoldLock = false;
	mLockDelay = 500;
	mGameOver = false;
	mLeft = false;
	mRight = false;
	mQueue.clear();
	mHoldTetromino.clear();
	mCurrentTetromino.clear();
	init();
}

void Playfield::setScreenSize(int width, int height) {
	screenWidth = width;
	screenHeight = height;
}

void Playfield::tic(int elapsed) {

	if (!mGameOver) {
		checkQueue();
		// Update y position
		double speed = 0.75 * mLevel / 1000.0; // grid per ms
		mCurrentPosY += (speed * elapsed);

		// Can't move down anymore
		if (!isLegal(mCurrentPosX, mCurrentPosY)) {
			// Reverse position & change to lowest possile position
			mCurrentPosY -= (speed * elapsed);
			mCurrentPosY = project();

			// lock delay
			if (mLockDelayTimer.isStarted()) {
				if (mLockDelayTimer.getTicks() >= mLockDelay) {
					lock();
				}
			} else {
				mLockDelayTimer.start();
			}
		}
	}
}

void Playfield::draw() {
// Top left point of the play Field
	SDL_Point playFieldTopLeft = { PF_BLOCKSIZE, screenHeight
			- (PF_HEIGHT - 1) * PF_BLOCKSIZE };

// Drawing the play Area
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
	double roundedY = roundY(mCurrentPosY);
// Centre of the Tetromino on display
// -2 on y to correct for hidden rows
	SDL_Point currentCentreOnDisplay;
	currentCentreOnDisplay.x = (int) (playFieldTopLeft.x
			+ mCurrentPosX * PF_BLOCKSIZE);
	currentCentreOnDisplay.y = (int) (playFieldTopLeft.y
			+ (roundedY - 2) * PF_BLOCKSIZE);

	SDL_Point ghostCenterOnDisplay;
	ghostCenterOnDisplay.x = currentCentreOnDisplay.x;
	ghostCenterOnDisplay.y = playFieldTopLeft.y
			+ (project() - 2) * PF_BLOCKSIZE;

	drawTetromino(ghostCenterOnDisplay, mCurrentTetromino, true);
	drawTetromino(currentCentreOnDisplay, mCurrentTetromino);

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
	if (!mGameOver) {

		// Drawing text
		mNext.render(playArea.x + playArea.w + PF_BLOCKSIZE,
				playFieldTopLeft.y + PF_BLOCKSIZE);
		// Draw upcoming tetromino

		SDL_Point currentNext;
		currentNext.x = playArea.x + playArea.w + PF_BLOCKSIZE;
		currentNext.y = playArea.y + 3 * PF_BLOCKSIZE;
		for (int i = 1; i < 5; i++) {
			Tetromino upcoming;
			upcoming.generate(mQueue.at(mQueue.size() - i));
			int thickness = 1;

			if (upcoming.getTType() != TTYPE_I) {
				thickness++;
			}

			SDL_Point center = topLeftToCenter(currentNext,
					upcoming.getTType());
			drawTetromino(center, upcoming);
			currentNext.y += (thickness + 1) * PF_BLOCKSIZE;
		}

		// Draw hold text and tetromino
		SDL_Point holdTopLeft = { playArea.x + playArea.w + PF_BLOCKSIZE,
				playFieldTopLeft.y + 16 * PF_BLOCKSIZE };
		mHold.render(holdTopLeft.x, holdTopLeft.y);
		holdTopLeft.y += 2 * PF_BLOCKSIZE;	// Spacing
		SDL_Point holdCenter = topLeftToCenter(holdTopLeft,
				mHoldTetromino.getTType());
		drawTetromino(holdCenter, mHoldTetromino);
	}

	// Draw level info
	LTexture levelText;
	levelText.setRenderer(mRenderer);
	stringstream levelSS;
	levelSS << "Lvl: ";
	levelSS << mLevel;
	levelText.loadFromRenderedText(mFont, levelSS.str().c_str(),
			mColourArray.at(7));
	levelText.render(playFieldTopLeft.x, playFieldTopLeft.y - 2 * PF_BLOCKSIZE);

	// Draw time info
	LTexture gameTime;
	gameTime.setRenderer(mRenderer);
	stringstream timeSS;
	timeSS << "Time: ";
	timeSS << (int) (mGame.getTicks() / 1000);
	gameTime.loadFromRenderedText(mFont, timeSS.str().c_str(),
			mColourArray.at(7));
	gameTime.render(playFieldTopLeft.x + 6 * PF_BLOCKSIZE,
			playFieldTopLeft.y - 2 * PF_BLOCKSIZE);
}

void Playfield::handleEvent(SDL_Event& event) {
	if (!mGameOver) {

		if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
			case SDLK_LEFT:
				mLeft = true;
				break;
			case SDLK_RIGHT:
				mRight = true;
				break;
			case SDLK_UP:
				mCurrentPosY = project();
				break;
			case SDLK_DOWN:
				if (isLegal(mCurrentPosX, mCurrentPosY + 1)) {
					mCurrentPosY++;
				}
				break;
			case SDLK_SPACE:
				mCurrentPosY = project();
				lock();
				break;
			case SDLK_a:
				mCurrentTetromino.antiClock();
				if (!isLegal(mCurrentPosX, mCurrentPosY)) {
					if (!kick()) {
						mCurrentTetromino.clock();
					} else {
						mLockDelayTimer.reset();
					}
				} else {
					mLockDelayTimer.reset();
				}
				break;
			case SDLK_o:
				mCurrentTetromino.clock();
				if (!isLegal(mCurrentPosX, mCurrentPosY)) {
					if (!kick()) {
						mCurrentTetromino.antiClock();
					} else {
						mLockDelayTimer.reset();
					}
				} else {
					mLockDelayTimer.reset();
				}
				break;
			case SDLK_e:
				hold();
				break;
			}
			// Need to check if the move is legal or out of bound
		} else if (event.type == SDL_KEYUP) {
			switch (event.key.keysym.sym) {
			case SDLK_LEFT:
				mLeft = false;
				break;
			case SDLK_RIGHT:
				mRight = false;
				break;
			}

		}

		// Core movement
		if (!(mLeft && mRight)) {
			if (mLeft) {
				if (isLegal(mCurrentPosX - 1, mCurrentPosY)) {
					mCurrentPosX--;
					mLockDelayTimer.reset();
				}
			}
			if (mRight) {
				if (isLegal(mCurrentPosX + 1, mCurrentPosY)) {
					mCurrentPosX++;
					mLockDelayTimer.reset();
				}
			}
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

void Playfield::checkQueue() {
	while (mQueue.size() < 7) {
		vector<int> newQueue = randomiser();
		newQueue.insert(newQueue.end(), mQueue.begin(), mQueue.end());
		mQueue = newQueue;
	}
}

void Playfield::newTetromino(int tType) {
	//printf("newPiece\n");
	// Generate new Piece

	if (tType == -1) {
		tType = mQueue.back();
		// Remove generate piece from the queue
		mQueue.pop_back();
	}
	mCurrentTetromino.generate(tType);

	// Update new positions
	switch (tType) {
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

	// Check if game is over
	if (!isLegal(mCurrentPosX, mCurrentPosY)) {
		mGameOver = true;
		mGame.pause();
	}

}

SDL_Point Playfield::topLeftToCenter(SDL_Point& topLeft, int tType) {
// This only works for unroated pieces

	SDL_Point center;

// Get piece centre
	double dx, dy;
	dx = 2; // Horziontally centred
	switch (tType) {
	case TTYPE_I:
		dy = 1;
		break;
	case TTYPE_O:
		dy = 1;
		break;
	default:
		dy = 1.5;
		break;
	}

	center.x = topLeft.x + PF_BLOCKSIZE * dx;
	center.y = topLeft.y + PF_BLOCKSIZE * dy;

	return center;

}

void Playfield::drawBlock(SDL_Point& topLeft, int tType, bool ghost) {
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

	// Polish effect
	if (!ghost) {
		Uint8 white = 0xFF;
		int shineSize = PF_BLOCKSIZE / 7;
		SDL_Rect shine1, shine2, shine3, shine4;
		//shine1 = {block.x, block.y, shineSize,shineSize};
		shine2 = {block.x + shineSize, block.y + shineSize,shineSize, shineSize};
		shine3 = {block.x + 2*shineSize, block.y + shineSize, shineSize, shineSize};
		shine4 = {block.x + shineSize, block.y + 2*shineSize,shineSize, shineSize};
		SDL_SetRenderDrawColor(mRenderer, white, white, white, white);
		//SDL_RenderFillRect(mRenderer, &shine1);
		SDL_RenderFillRect(mRenderer, &shine2);
		SDL_RenderFillRect(mRenderer, &shine3);
		SDL_RenderFillRect(mRenderer, &shine4);

		SDL_Rect shadow1, shadow2;
		shadow1 = {block.x, block.y + block.h - shineSize, block.w, shineSize};
		shadow2 = {block.x + block.w - shineSize, block.y, shineSize, block.h};
		SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderFillRect(mRenderer, &shadow1);
		SDL_RenderFillRect(mRenderer, &shadow2);
	}

	// Line is too thick with shadows
//	// Drawing outline
//	if (!ghost) {
//		SDL_Color outline = { 0x00, 0x00, 0x00 };
//		SDL_SetRenderDrawColor(mRenderer, outline.r, outline.g, outline.b,
//				transparency);
//		SDL_RenderDrawRect(mRenderer, &block);
//	}
}

void Playfield::drawTetromino(SDL_Point& center, Tetromino& tetromino,
		bool ghost) {

	vector<TetrominoCRS> blockPos = tetromino.getBlockPos();
	for (vector<TetrominoCRS>::iterator it = blockPos.begin();
			it != blockPos.end(); it++) {

		SDL_Point blockTopLeft;
		// - 0.5 correction to caluclate the topleft conor
		blockTopLeft.x = center.x + (it->x - 0.5) * PF_BLOCKSIZE;
		blockTopLeft.y = center.y + (it->y - 0.5) * PF_BLOCKSIZE;
		if (blockTopLeft.y >= screenHeight - 21 * PF_BLOCKSIZE) {
			drawBlock(blockTopLeft, tetromino.getTType(), ghost);
		}
	}
}

bool Playfield::isLegal(double x, double y) {

	bool legal = true;
// Ronud y
	y = roundY(y);
//printf("Legal Ronuded Y : %f\n", y);

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

	mHoldLock = false;	// Reset holdlock
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

// Check for complete here and generate new
	lineCheck();	// Check for complete lines
	newTetromino();		// New pieces
	mLockDelayTimer.stop();	// stop lock delay timer
}

void Playfield::hold() {
	if (!mHoldLock) {
		if (mHoldTetromino.getTType() == -1) {
			mHoldTetromino.generate(mCurrentTetromino.getTType());
			newTetromino();
		} else {
			int tempType = mCurrentTetromino.getTType();
			newTetromino(mHoldTetromino.getTType());
			mHoldTetromino.generate(tempType);
		}
	}

	mHoldLock = true;
}

bool Playfield::kick() {
	bool kicked = false;
	int kickRange = 0;
	switch (mCurrentTetromino.getTType()) {
	case TTYPE_I:
		kickRange = 2;
		break;
	case TTYPE_O:
		break;
	default:
		kickRange = 1;
		break;
	}
// Floor Kick
	if (mCurrentPosY >= project()) {
		//(mCurrentTetromino.getRotation() == 1 || mCurrentTetromino.getRotation() == 3) secondary condition
		for (int i = 1; i <= kickRange + 1; i++) {
			if (isLegal(mCurrentPosX, mCurrentPosY - i)) {
				mCurrentPosY -= i;
				kicked = true;
				break;
			}
		}
		// Wall kick
	} else if (mCurrentPosX <= 1 || mCurrentPosX >= 9) {
		// (mCurrentTetromino.getRotation() == 0 || mCurrentTetromino.getRotation() == 2)
		// Left Wall
		if (mCurrentPosX <= 1) {
			for (int i = 1; i <= kickRange; i++) {
				if (isLegal(mCurrentPosX + i, mCurrentPosY)) {
					mCurrentPosX += i;
					kicked = true;
					break;
				}
			}
		}
		// Right wall
		if (mCurrentPosX >= 9) {
			for (int i = 1; i <= kickRange; i++) {
				if (isLegal(mCurrentPosX - i, mCurrentPosY)) {
					mCurrentPosX -= i;
					kicked = true;
					break;
				}
			}
		}
	}

	return kicked;
}

void Playfield::lineCheck() {

// TODO add line clearing animations

	vector<int> completedLine;

// Check every single row
	for (int row = 0; row < PF_HEIGHT; row++) {
		int elementInRow = 0;
		for (int index = 0; index < PF_WIDTH; index++) {
			if (mPlayField[index][row] != -1) {
				elementInRow++;
			}
		}

		// if the row is complete
		if (elementInRow == PF_WIDTH) {
			// Record completed rows
			completedLine.push_back(row);

			// empty the row
			for (int i = 0; i < PF_WIDTH; i++) {
				mPlayField[i][row] = -1;
			}
		}
	}
// Collide when lines are cleared
	if (completedLine.size() != 0) {
		sort(completedLine.begin(), completedLine.end());// From the top of the playfield
		for (vector<int>::iterator it = completedLine.begin();
				it != completedLine.end(); it++) {

			for (int row = *it - 1; row >= 0; row--) {
				for (int index = 0; index < PF_WIDTH; index++) {
					mPlayField[index][row + 1] = mPlayField[index][row];// move the element down by one
					mPlayField[index][row] = -1;	// Clear original element
				}
			}
		}

		// Change to level
		switch (completedLine.size()) {
		case 1:
			mLevel += 1;
			break;
		case 2:
			mLevel += 2;
			break;
		case 3:
			mLevel += 4;
			break;
		case 4:
			mLevel += 6;
			break;
		}

		// Change to lockdelay

		if (mLevel > 500 && mLevel < 750) {
			mLockDelay = 1000 - mLevel;
		}

		if (mLevel > 1000) {
			mGameOver = true;
		}

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

bool Playfield::isGameOver() {
	return mGameOver;
}

int Playfield::getLevel() {
	return mLevel;
}

