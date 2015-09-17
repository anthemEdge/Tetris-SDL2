/*
 * Playfield.h
 *
 *  Created on: 8 Aug 2015
 *      Author: eva
 */

#ifndef PLAYFIELD_H_
#define PLAYFIELD_H_

#include <iostream> // cout
#include <SDL.h>
#include <SDL_ttf.h>
#include "LTexture.h"
#include "LTimer.h"
#include <vector>
#include <ctime> // to seed for srand
#include <stdio.h> // printf
#include <algorithm> // shuffle
#include <cstdlib> // rand and srand
#include <sstream>
#include "Tetromino.h"

using namespace std;

enum GameState {
	GAME_STATE_INGAME,
	GAME_STATE_PAUSED,
	GAME_STATE_WON,
	GAME_STATE_LOST,
	GAME_STATE_QUIT
};

enum ColourIndex {
	COLOUR_INDEX_CYAN,
	COLOUR_INDEX_YELLOW,
	COLOUR_INDEX_PURPLE,
	COLOUR_INDEX_GREEN,
	COLOUR_INDEX_RED,
	COLOUR_INDEX_BLUE,
	COLOUR_INDEX_ORANGE,
	COLOUR_INDEX_WHITE,
	COLOUR_INDEX_GREY,
	COLOUR_INDEX_TOTAL
};

class Playfield {
public:
	Playfield(SDL_Renderer* renderer);
	void tick();
	void draw(); // add the grid to render
	void setScreenSize(int width, int height);
	void handleEvent(SDL_Event& event);
	int getGameState();
	void start();
	SDL_Rect getPlayArea();

public:
	static const int PF_WIDTH = 10;
	static const int PF_HEIGHT = 22;
	static const int PF_BLOCKSIZE = 28;

private:
	int mGameState;
	LTimer mGlobalTimer;
	LTimer mElapsedTimer;

	SDL_Rect mPlayArea;
	int mPlayfield[PF_WIDTH][PF_HEIGHT];
	SDL_Color mColourArray[COLOUR_INDEX_TOTAL];
	SDL_Renderer* mRenderer;

	int screenWidth;
	int screenHeight;

	int mLevel;

	Tetromino mCurrentTetromino;
	double mCurrentPosX;
	double mCurrentPosY;

	vector<int> mQueue;

	TTF_Font* mFont;
	LTexture mNext;
	LTexture mHold;
	Tetromino mHoldTetromino;
	bool mHoldLock;

	LTimer mLockDelayTimer;
	unsigned int mLockDelay;

	bool mLeft;
	bool mRight;

private:
	vector<int> randomiser();
	void checkQueue();
	void newTetromino(int tType = -1);
	void drawBlock(SDL_Point& topLeft, int tType, bool ghost = false);
	SDL_Point topLeftToCenter(SDL_Point& topLeft, int tType);
	void drawTetromino(SDL_Point& center, Tetromino& tetromino, bool ghost =
			false);
	bool isLegal(double x, double y);

	void lock();
	void hold();
	bool kick();
	double project();
	double backTrack();
	void lineCheck();

	double roundY(double y);

};

#endif /* PLAYFIELD_H_ */
