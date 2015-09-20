/*
 * PlayfieldNode.h
 *
 *  Created on: 17 Sep 2015
 *      Author: eva
 */

#ifndef PLAYFIELDNODE_H_
#define PLAYFIELDNODE_H_

#include <vector>

using namespace std;

struct PlayfieldPosition {
	// The position of a point on the playfield
	double x, y;
	int rotation, ttype;
	// The rotation stored is there is how many times you have to rotate from the spawn position
	PlayfieldPosition();
	PlayfieldPosition(double x, double y, int rotation, int ttype);

};

struct PlayfieldNode {
	PlayfieldNode();
	bool mSimplePlayfield[10][22];
	vector<PlayfieldPosition> mTetrominoPositions;
};

#endif /* PLAYFIELDNODE_H_ */
