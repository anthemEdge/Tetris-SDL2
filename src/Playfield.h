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

using namespace std;

#include "Tetromino.h"

class Playfield {
public:
	Playfield(SDL_Renderer* renderer);
	void tic(int elapsed);
	void draw(); // add the grid to render
	void setScreenSize(int width, int height);
	void handleEvent(SDL_Event& event);
	bool isGameOver();
	void reset();

public:
	static const int PF_WIDTH = 10;
	static const int PF_HEIGHT = 22;
	static const int PF_BLOCKSIZE = 28;

private:

	LTimer mGame;

	int mPlayField[PF_WIDTH][PF_HEIGHT];
	vector<SDL_Color> mColourArray;
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
	int mLockDelay;

	bool mGameOver;

	bool mLeft;
	bool mRight;

private:
	void init();

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
