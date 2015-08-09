/*
 * LTimer.h
 *
 *  Created on: 6 Aug 2015
 *      Author: eva
 */

#ifndef LTIMER_H_
#define LTIMER_H_

#include <SDL.h>

class LTimer {
public:
	LTimer();

	void start();
	void stop();
	void pause();
	void unPause();

	// Get this timer's time
	Uint32 getTicks();

	// Getters
	bool isStarted();
	bool isPaused();

private:
	Uint32 mStartTicks;
	Uint32 mPausedTicks;

	bool mPaused;
	bool mStarted;

};

#endif /* LTIMER_H_ */
