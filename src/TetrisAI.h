/*
 * TetrisAI.h
 *
 *  Created on: 17 Sep 2015
 *      Author: eva
 */

#ifndef TETRISAI_H_
#define TETRISAI_H_

#include <SDL.h>
// boost library
//#include <boost/thread.hpp>
//#include <boost/bind.hpp>

#include <pthread.h>
#include <vector>
#include <map>
#include <iterator> // beaming
#include "Playfield.h"
#include "LTimer.h"

using namespace std;

enum InputSet {
	INPUT_LEFT = SDLK_LEFT,
	INPUT_RIGHT = SDLK_RIGHT,
	INPUT_SOFTDROP = SDLK_UP,
	INPUT_HARDDROP = SDLK_SPACE,
	INPUT_CLOCKWISE = SDLK_x,
	INPUT_ANTICLOCKWISE = SDLK_z,
	INPUT_HOLD = SDLK_c,
	INPUT_TOTAL
};

class TetrisAI {
public:
	TetrisAI();
	void setCurrentGame(PlayfieldNode node, vector<int> queue);
	bool requireNewThread();
	void search();
	bool isReady();
	void decreaseDepth();
	void resetSearchDepth();
	void setGradingModifier(int roughness, int hole, int height);
private:
	vector<PlayfieldNode> generate(PlayfieldNode node, int ttype); // Generate all possible combination from the node
	int grade(PlayfieldNode node);		// Grade a rating for a certain position
	PlayfieldNode lock(PlayfieldNode node, PlayfieldPosition instruction);
	void lineCheck(PlayfieldNode& node);
	PlayfieldPosition getSpawnPosition(int ttype);
	int getUniqueRotation(int ttype);
	bool isInBound(Tetromino tetromino, double xPos);
	double getProjectedY(PlayfieldNode node, Tetromino tetromino, double xPos);
	bool checkCollision(PlayfieldNode node, Tetromino tetromino, double xPos,
			double yPos);
	void execute(vector<int> commands);
	void pushEvent(int keysum);

	// Debug
	void nodeDump(PlayfieldNode node, string name);
private:
	PlayfieldNode mCurrentNode;
	vector<int> mTetrominoQueue;
	bool mNewThread;
	bool mReady;
	LTimer mSearchTimer;
	int mSearchDepth;
	static const int AI_DELAY = 0;
	static const int MAX_SEARCH_DEPTH = 5;
	static const int MIN_SEARCH_DEPTH = 2;
	static const int SEARCH_BEAM_WIDTH = 12;

	int roughnessMod;
	int holeMod;
	int heightMod;
	int densityMod;

};

#endif /* TETRISAI_H_ */
