/*
 * TetrisAI.cpp
 *
 *  Created on: 17 Sep 2015
 *      Author: eva
 */

#include "TetrisAI.h"

TetrisAI::TetrisAI() :
		mNewThread(false), mReady(true), mSearchDepth(MAX_SEARCH_DEPTH), roughnessMod(
				-20), holeMod(-40), heightMod(-10), densityMod(100) {
}

void TetrisAI::setCurrentGame(PlayfieldNode node, vector<int> queue) {
	// Detected if the queue changes
	if (queue != mTetrominoQueue) {
		mNewThread = true;
		mCurrentNode = node;
		mTetrominoQueue = queue;
	}
}

bool TetrisAI::requireNewThread() {
	return mNewThread;
}

bool TetrisAI::isReady() {
	return mReady;
}

void TetrisAI::decreaseDepth() {
	mSearchDepth--;
	if (mSearchDepth < MIN_SEARCH_DEPTH) {
		mSearchDepth = MIN_SEARCH_DEPTH;
	}
	printf("Depth change: %i\n", mSearchDepth);
}

void TetrisAI::resetSearchDepth() {
	mSearchDepth = MAX_SEARCH_DEPTH;
}

void TetrisAI::setGradingModifier(int roughness, int hole, int height) {
	roughnessMod = roughness;
	holeMod = hole;
	heightMod = height;
}

void TetrisAI::search() {

	// Beam Search
	mSearchTimer.start();
	mReady = false;
	mNewThread = false;

	multimap<int, PlayfieldNode> nodeMap;
	multimap<int, PlayfieldNode> newNodeMap;
	nodeMap.insert(make_pair(grade(mCurrentNode), mCurrentNode));

	// Queue
	for (vector<int>::iterator it = mTetrominoQueue.begin();
			it != mTetrominoQueue.begin() + mSearchDepth; it++) {
		// Current Map
		newNodeMap.clear();
		for (multimap<int, PlayfieldNode>::iterator nit = nodeMap.begin();
				nit != nodeMap.end(); nit++) {
			// New Node
			vector<PlayfieldNode> newSet = generate(nit->second, *it);
			// Adding to new Map
			for (vector<PlayfieldNode>::iterator vit = newSet.begin();
					vit != newSet.end(); vit++) {
				newNodeMap.insert(make_pair(grade(*vit), *vit));
			}

		}

		if (newNodeMap.size() > SEARCH_BEAM_WIDTH) {
			// Beaming
			multimap<int, PlayfieldNode>::iterator beamingIt =
					newNodeMap.begin();
			advance(beamingIt, newNodeMap.size() - SEARCH_BEAM_WIDTH);
			newNodeMap.erase(newNodeMap.begin(), beamingIt);
		}
		nodeMap = newNodeMap;
	}
	// Best node is last node
	multimap<int, PlayfieldNode>::iterator bestNodeIt = nodeMap.end();
	advance(bestNodeIt, -1);
	PlayfieldNode bestNode = bestNodeIt->second;

	// Constructing inputs
	vector<int> commands;

	PlayfieldPosition instruction = bestNode.mTetrominoPositions.at(0);
	for (int i = 0; i < instruction.rotation; i++) {
		commands.push_back(INPUT_CLOCKWISE);
	}
	PlayfieldPosition spawn = getSpawnPosition(instruction.ttype);
	int relativeX = instruction.x - spawn.x;
	int directionInput = INPUT_RIGHT;
	if (relativeX < 0) {
		directionInput = INPUT_LEFT;
	}
	for (int i = 0; i < abs(relativeX); i++) {
		commands.push_back(directionInput);
	}
	commands.push_back(INPUT_HARDDROP);
	execute(commands);
	int searchTime = mSearchTimer.getTicks();
	mReady = true;
	//printf("Thinking time: %i\n", searchTime);
}

vector<PlayfieldNode> TetrisAI::generate(PlayfieldNode node, int ttype) {
	vector<PlayfieldNode> generatedNode;
	vector<PlayfieldPosition> generatedPosition;

// Finding valid ending positions
	Tetromino testTetromion;
	testTetromion.generate(ttype);
	PlayfieldPosition spawn = getSpawnPosition(ttype);
	int uniqueRotation = getUniqueRotation(ttype);
// Rotation
	for (int rotation = 0; rotation < uniqueRotation; rotation++) {
		// x Position
		for (int xModifier = -5; xModifier <= 5; xModifier++) {
			// Check Edge
			if (isInBound(testTetromion, spawn.x + (double) xModifier)) {
				double xPos = spawn.x + (double) xModifier;
				double yPos = getProjectedY(node, testTetromion, xPos);
				generatedPosition.push_back(
						PlayfieldPosition(xPos, yPos, rotation, ttype));
			}
		}
		testTetromion.clock();
	}

// Generating node from those positions
	for (vector<PlayfieldPosition>::iterator it = generatedPosition.begin();
			it != generatedPosition.end(); it++) {
		generatedNode.push_back(lock(node, *it));

	}
	return generatedNode;
}

int TetrisAI::grade(PlayfieldNode node) {

	// Core position grading system, dictates the behaviour of AI
	int grade = 0;

	// Hole detection
	int holes = 0;
	for (int x = 0; x < 10; x++) {
		for (int y = 1; y < 22; y++) {
			if (node.mSimplePlayfield[x][y]) {
				int holeLength = 1;
				while (!node.mSimplePlayfield[x][y + holeLength]
						&& y + holeLength < 22) {
					holes++;
					holeLength++;
				}
			}
		}
	}

	// Maximum height
	int maxHeght = 0;
	for (int height = 0; height < 22; height++) {
		bool detected = false;
		for (int i = 0; i < 10; i++) {
			if (node.mSimplePlayfield[i][height]) {
				detected = true;
				maxHeght = 22 - height;
				break;
			}
		}

		if (detected) {
			break;
		}
	}

	// Density
	int numFill = 0;
	// Roughness
	int roughness = 0;
	for (int x = 0; x < 10; x++) {
		for (int y = 0; y < 22; y++) {
			if (node.mSimplePlayfield[x][y]) {
				numFill++;
				// Check Left
				if (x != 0 && !node.mSimplePlayfield[x - 1][y]) {
					roughness++;
				}
				// Check Right
				if (x != 21 && !node.mSimplePlayfield[x + 1][y]) {
					roughness++;
				}

			}
		}
	}

	// Density
	double weightedDensity = (double) numFill / (10 * maxHeght);
	// round(densityMod * weightedDensity) +
	grade = roughnessMod * roughness + holeMod * holes + heightMod * maxHeght;
	return grade;
}

PlayfieldNode TetrisAI::lock(PlayfieldNode node,
		PlayfieldPosition instruction) {
	// Generate instructed tetromino
	Tetromino lockingTetromino;
	lockingTetromino.generate(instruction.ttype);
	for (int rotation = 0; rotation < instruction.rotation; rotation++) {
		lockingTetromino.clock();
	}

	// Locking
	vector<TetrominoCRS> blockPosArry = lockingTetromino.getBlockPos();
	for (vector<TetrominoCRS>::iterator it = blockPosArry.begin();
			it != blockPosArry.end(); it++) {
		int blockX = instruction.x + (it->x - 0.5);
		int blockY = instruction.y + (it->y - 0.5);
		node.mSimplePlayfield[blockX][blockY] = true;
	}
	lineCheck(node);
	node.mTetrominoPositions.push_back(instruction);
	return node;
}

void TetrisAI::lineCheck(PlayfieldNode& node) {
	vector<int> completedLine;
	// Check every single row
	for (int row = 0; row < 22; row++) {
		int elementInRow = 0;
		for (int index = 0; index < 10; index++) {
			if (node.mSimplePlayfield[index][row]) {
				elementInRow++;
			}
		}

		// if the row is complete
		if (elementInRow == 10) {
			// Record completed rows
			completedLine.push_back(row);

			// empty the row
			for (int i = 0; i < 10; i++) {
				node.mSimplePlayfield[i][row] = false;
			}
		}
	}
	// Collide when lines are cleared
	if (completedLine.size() != 0) {
		sort(completedLine.begin(), completedLine.end()); // From the top of the playfield
		for (vector<int>::iterator it = completedLine.begin();
				it != completedLine.end(); it++) {

			for (int row = *it - 1; row >= 0; row--) {
				for (int index = 0; index < 10; index++) {
					node.mSimplePlayfield[index][row + 1] =
							node.mSimplePlayfield[index][row]; // move the element down by one
					node.mSimplePlayfield[index][row] = false; // Clear original element
				}
			}
		}
	}

}

PlayfieldPosition TetrisAI::getSpawnPosition(int ttype) {
	PlayfieldPosition spawn;
	switch (ttype) {
	case TTYPE_I:
	case TTYPE_O:
		spawn.x = 5;
		spawn.y = 1;
		break;
	case TTYPE_T:
	case TTYPE_S:
	case TTYPE_Z:
	case TTYPE_J:
	case TTYPE_L:
		spawn.x = 4.5;
		spawn.y = 1.5;
		break;
	}

	return spawn;
}

int TetrisAI::getUniqueRotation(int ttype) {
	int uniqueRotation = 0;
	switch (ttype) {
	case TTYPE_I:
	case TTYPE_S:
	case TTYPE_Z:
		uniqueRotation = 2;
		break;
	case TTYPE_O:
		uniqueRotation = 1;
		break;
	case TTYPE_T:
	case TTYPE_L:
	case TTYPE_J:
		uniqueRotation = 4;
		break;
	}
	return uniqueRotation;
}

bool TetrisAI::isInBound(Tetromino tetromion, double xPos) {
	bool inBound = true;
	vector<TetrominoCRS> blockPosArray = tetromion.getBlockPos();
	for (vector<TetrominoCRS>::iterator it = blockPosArray.begin();
			it != blockPosArray.end(); it++) {
		// The most left line of every block
		int left = xPos + (it->x - 0.5);
		if (tetromion.getTType() == TTYPE_I) {
		}

		if (left < 0 || left >= 10) {
			inBound = false;
			break;
		}

	}

	return inBound;
}

double TetrisAI::getProjectedY(PlayfieldNode node, Tetromino tetromino,
		double xPos) {
	PlayfieldPosition spawn = getSpawnPosition(tetromino.getTType());
	double projected = spawn.y;

	while (!checkCollision(node, tetromino, xPos, projected + 1)) {
		projected++;
	}

	return projected;
}

bool TetrisAI::checkCollision(PlayfieldNode node, Tetromino tetromino,
		double xPos, double yPos) {
	// y axis only
	bool collision = false;

	vector<TetrominoCRS> blockPosArray = tetromino.getBlockPos();
	for (vector<TetrominoCRS>::iterator it = blockPosArray.begin();
			it != blockPosArray.end(); it++) {
		int col = xPos + (it->x - 0.5);
		int row = yPos + (it->y - 0.5);

		if (node.mSimplePlayfield[col][row]) {
			collision = true;
			break;
		} else if (row >= 22) {
			collision = true;
			break;
		}
	}

	return collision;
}

void TetrisAI::execute(vector<int> commands) {
	for (vector<int>::iterator it = commands.begin(); it != commands.end();
			it++) {
		SDL_Delay(AI_DELAY);
		pushEvent(*it);
	}
}

void TetrisAI::pushEvent(int keysum) {
	SDL_Event keyEvent;
	keyEvent.type = SDL_KEYDOWN;
	keyEvent.key.keysym.sym = keysum;
	SDL_PushEvent(&keyEvent);

}

// Debug

void TetrisAI::nodeDump(PlayfieldNode node, string name) {
	printf("Node Dump from %s\n", name.c_str());
	for (int y = 0; y < 22; y++) {
		printf("%i: \t", y);
		for (int x = 0; x < 10; x++) {
			char output = 'O';
			if (node.mSimplePlayfield[x][y]) {
				output = 'X';
			}
			printf(" %c", output);
		}
		printf("\n");
	}
}

