/*
 * Based on BallDroppings by artist/designer Josh Nimoy (C) 2003
 * Reworked for laser drawing by Nicolas Thill <nico@openwrt.org>
 *
 * This is free software, licensed under Creative Commons Attribution-ShareAlike 3.0 Unported License (CC BY-SA 3.0)
 * See /LICENSE for more information.
 */

#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include "Ball.h"
#include "Line.h"
#include "List.h"

#include "ofxEtherdream.h"

#ifdef TARGET_LINUX
	#include <GL/glut.h>
#endif

using namespace std;

class testApp : public ofSimpleApp {

	public:
        List *my_balls;
        List *my_lines;
        Line *my_line;

		void setup();
		void update();
		void draw();
		void step();

		void keyPressed(int key);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);

		void createBall(); // create a new ball at the emmitter's position

		void resetWorld(); // reset the entire app
		void resetVars(); // reset global state variables
		void resetBalls(); // clear only the balls
		void resetLines(); // clear only the lines

		float oldMillis; // remembering what time it was
		bool paused; // freezing the app like pausing the game

		static long myBallDropRate; // how fast or slow to release balls
		static float myFrequencyRange; // pitch range for ball song
		static float myFriction; // friction multiplier to keep the physics realistic
		static float myGravity; // strength of gravity pulling towards bottom of screen

		V3 hole;

		ofTrueTypeFont font; // typeface for the info display
        int xFrameMin;
        int yFrameMin;
        int xFrameMax;
        int yFrameMax;
        ofxIlda::Frame ildaFrame;
        ofxEtherdream etherdream;
};

#endif /* _TEST_APP */
