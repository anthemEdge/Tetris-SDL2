/*
 * Tetromino.cpp
 *
 *  Created on: 7 Aug 2015
 *      Author: eva
 */

#include "Tetromino.h"

Tetromino::Tetromino() {
}

void Tetromino::generate(int type) {
	// if input is out of range
	if (type < 0 || type > 6) {
		type = rand() % 7;
	}

	switch (type) {
	case TTYPE_I:
		mColour = {0x00, 0xFF,0xFF};	// Cyan
		mBlockPos.push_back(TetrominoCRS(-1.5, -0.5));
		mBlockPos.push_back(TetrominoCRS(-0.5, -0.5));
		mBlockPos.push_back(TetrominoCRS(0.5, -0.5));
		mBlockPos.push_back(TetrominoCRS(1.5, -0.5));
		break;
		case TTYPE_O:
		mColour = {0xFF, 0xFF,0x00};	// Yellow
		mBlockPos.push_back(TetrominoCRS(-0.5, -0.5));
		mBlockPos.push_back(TetrominoCRS(0.5, -0.5));
		mBlockPos.push_back(TetrominoCRS(-0.5, 0.5));
		mBlockPos.push_back(TetrominoCRS(0.5, 0.5));
		break;
		case TTYPE_T:
		mColour = {0x80, 0x00,0x80};	// Purple
		mBlockPos.push_back(TetrominoCRS(0, -1));
		mBlockPos.push_back(TetrominoCRS(-1, 0));
		mBlockPos.push_back(TetrominoCRS(0, 0));
		mBlockPos.push_back(TetrominoCRS(1, 0));
		break;
		case TTYPE_S:
		mColour = {0x00, 0xFF,0x00};	// Green
		mBlockPos.push_back(TetrominoCRS(0, -1));
		mBlockPos.push_back(TetrominoCRS(1, -1));
		mBlockPos.push_back(TetrominoCRS(-1, 0));
		mBlockPos.push_back(TetrominoCRS(0, 0));
		break;
		case TTYPE_Z:
		mColour = {0xFF, 0x00,0x00};	// Red
		mBlockPos.push_back(TetrominoCRS(-1, -1));
		mBlockPos.push_back(TetrominoCRS(0, -1));
		mBlockPos.push_back(TetrominoCRS(0, 0));
		mBlockPos.push_back(TetrominoCRS(1, 0));
		break;
		case TTYPE_J:
		mColour = {0x00, 0x00,0xFF};	// Blue
		mBlockPos.push_back(TetrominoCRS(-1, -1));
		mBlockPos.push_back(TetrominoCRS(-1, 0));
		mBlockPos.push_back(TetrominoCRS(0, 0));
		mBlockPos.push_back(TetrominoCRS(1, 0));
		break;
		case TTYPE_L:
		mColour = {0xFF, 0xA5,0x00};	// Orange
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
}

void Tetromino::antiClock() {
	for (vector<TetrominoCRS>::iterator it = mBlockPos.begin();
			it != mBlockPos.end(); it++) {
		TetrominoCRS pos = *it;
		it->x = pos.y;
		it->y = -pos.x;
	}
}

// Getters
vector<TetrominoCRS> Tetromino::getBlockPos() {
	return mBlockPos;
}

SDL_Color Tetromino::getColour() {
	return mColour;
}

