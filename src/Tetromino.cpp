/*
 * Tetromino.cpp
 *
 *  Created on: 7 Aug 2015
 *      Author: eva
 */

#include "Tetromino.h"

Tetromino::Tetromino() :
		mTType(-1), mRotation(0) {
}

void Tetromino::generate(int type) {

	// Clear pre-exiting Tetromino
	mBlockPos.clear();

	// if input is out of range
	if (type < 0 || type > 6) {
		type = rand() % 7;
	}

	// Assign Tetromino type
	mTType = type;

	// The crs points represent the centre of each block

	switch (type) {
	case TTYPE_I:
		mBlockPos.push_back(TetrominoCRS(-1.5, -0.5));
		mBlockPos.push_back(TetrominoCRS(-0.5, -0.5));
		mBlockPos.push_back(TetrominoCRS(0.5, -0.5));
		mBlockPos.push_back(TetrominoCRS(1.5, -0.5));
		break;
	case TTYPE_O:
		mBlockPos.push_back(TetrominoCRS(-0.5, -0.5));
		mBlockPos.push_back(TetrominoCRS(0.5, -0.5));
		mBlockPos.push_back(TetrominoCRS(-0.5, 0.5));
		mBlockPos.push_back(TetrominoCRS(0.5, 0.5));
		break;
	case TTYPE_T:
		mBlockPos.push_back(TetrominoCRS(0, -1));
		mBlockPos.push_back(TetrominoCRS(-1, 0));
		mBlockPos.push_back(TetrominoCRS(0, 0));
		mBlockPos.push_back(TetrominoCRS(1, 0));
		break;
	case TTYPE_S:
		mBlockPos.push_back(TetrominoCRS(0, -1));
		mBlockPos.push_back(TetrominoCRS(1, -1));
		mBlockPos.push_back(TetrominoCRS(-1, 0));
		mBlockPos.push_back(TetrominoCRS(0, 0));
		break;
	case TTYPE_Z:
		mBlockPos.push_back(TetrominoCRS(-1, -1));
		mBlockPos.push_back(TetrominoCRS(0, -1));
		mBlockPos.push_back(TetrominoCRS(0, 0));
		mBlockPos.push_back(TetrominoCRS(1, 0));
		break;
	case TTYPE_J:
		mBlockPos.push_back(TetrominoCRS(-1, -1));
		mBlockPos.push_back(TetrominoCRS(-1, 0));
		mBlockPos.push_back(TetrominoCRS(0, 0));
		mBlockPos.push_back(TetrominoCRS(1, 0));
		break;
	case TTYPE_L:
		mBlockPos.push_back(TetrominoCRS(1, -1));
		mBlockPos.push_back(TetrominoCRS(-1, 0));
		mBlockPos.push_back(TetrominoCRS(0, 0));
		mBlockPos.push_back(TetrominoCRS(1, 0));
		break;
	}
}

// Rotations
void Tetromino::clock() {
	for (vector<TetrominoCRS>::iterator it = mBlockPos.begin();
			it != mBlockPos.end(); it++) {
		TetrominoCRS pos = *it;
		it->x = -pos.y;
		it->y = pos.x;
	}
	mRotation++;
	if (mRotation > 3) {
		mRotation -= 4;
	}
}

void Tetromino::antiClock() {
	for (vector<TetrominoCRS>::iterator it = mBlockPos.begin();
			it != mBlockPos.end(); it++) {
		TetrominoCRS pos = *it;
		it->x = pos.y;
		it->y = -pos.x;
	}
	mRotation--;
	if (mRotation < 0) {
		mRotation += 4;
	}
}

void Tetromino::clear() {
	mBlockPos.clear();
	mTType = -1;
	mRotation = 0;
}

// Getters
vector<TetrominoCRS> Tetromino::getBlockPos() {
	return mBlockPos;
}

int Tetromino::getTType() {
	return mTType;
}
int Tetromino::getRotation() {
	return mRotation;
}

