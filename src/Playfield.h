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
#include <vector>
#include <stdio.h> // printf
#include <algorithm> // shuffle
#include <cstdlib> // rand and srand

using namespace std;

#include "Tetromino.h"

class Playfield {
public:
	Playfield(SDL_Renderer* renderer);
	void tic(int elapsed);
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

	static const double DEFAULT_GRAVITY = 1 / 1000.0; //  grid per millisecond
	double mGravityModifier;

	Tetromino mCurrentTetromino;
	double mCurrentPosX;
	double mCurrentPosY;

	vector<int> mQueue;

private:
	vector<int> randomiser();
	void checkQueue();
	void newTetromino();
	void drawBlock(SDL_Point& topLeft, int tType, bool ghost = false,
			bool outline = true);
	bool isLegal(double x, double y);
	void lock();
	double project();

private:
	double roundY(double y);

};

#endif /* PLAYFIELD_H_ */
