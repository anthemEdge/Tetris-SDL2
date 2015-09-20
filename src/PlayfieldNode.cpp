/*
 * PlayfieldNode.cpp
 *
 *  Created on: 17 Sep 2015
 *      Author: eva
 */

#include "PlayfieldNode.h"

PlayfieldPosition::PlayfieldPosition(double x, double y, int rotation,
		int ttype) :
		x(x), y(y), rotation(rotation), ttype(ttype) {
}

PlayfieldPosition::PlayfieldPosition() :
		x(0), y(0), rotation(0), ttype(0) {
}

PlayfieldNode::PlayfieldNode() {
}

