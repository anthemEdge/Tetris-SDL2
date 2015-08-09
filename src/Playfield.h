/*
 * Playfield.h
 *
 *  Created on: 8 Aug 2015
 *      Author: eva
 */

#ifndef PLAYFIELD_H_
#define PLAYFIELD_H_

#include <SDL.h>
#include <vector>
#include <stdio.h> // printf
#include <algorithm> // shuffle
#include <cstdlib> // rand and srand

#include "Tetromino.h"

class Playfield {
public:
	Playfield(SDL_Renderer* renderer);
	void tic();
	void draw(); // add the grid to render
	void setScreenSize(int width, int height);
	void handleEvent(SDL_Event& event);
	virtual ~Playfield();

private:
	static const int PF_WIDTH = 10;
	static const int PF_HEIGHT = 22;
	static const int PF_BLOCKSIZE = 32;

	int mPlayField[PF_WIDTH][PF_HEIGHT];
	vector<SDL_Color> mColourArray;
	SDL_Renderer* mRenderer;

	int screenWidth;
	int screenHeight;

	Tetromino mCurrentTetromino;
	double mCurrentPosX;
	double mCurrentPosY;

	vector<int> mQueue;

private:
	vector<int> randomiser();

};

#endif /* PLAYFIELD_H_ */
