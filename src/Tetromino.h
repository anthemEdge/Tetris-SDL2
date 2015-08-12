/*
 * Tetromino.h
 *
 *  Created on: 7 Aug 2015
 *      Author: eva
 */

#ifndef TETROMINO_H_
#define TETROMINO_H_

#include <stdlib.h> // rand
#include <stdio.h> // printf
#include <vector> // vector
#include <SDL.h> // SDL Colour

using namespace std;
// vector

enum TetrominoType {
	TTYPE_I, TTYPE_O, TTYPE_T, TTYPE_S, TTYPE_Z, TTYPE_J, TTYPE_L
};

struct TetrominoCRS {
// CRS short for coordinates reference system
	double x;
	double y;
	// simple constructor
	TetrominoCRS(double x, double y) :
			x(x), y(y) {
	}
};

class Tetromino {
public:
	Tetromino();

// Generate a specified tetromino, default is random
	void generate(int type = -1);
	void clock();
	void antiClock();

public:
	vector<TetrominoCRS> getBlockPos();
	int getTType();
	int getRotation();
	void clear();

private:
	vector<TetrominoCRS> mBlockPos;
	int mTType;
	int mRotation;

};

#endif /* TETROMINO_H_ */
