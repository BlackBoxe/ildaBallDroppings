/*
 * Based on BallDroppings by artist/designer Josh Nimoy (C) 2003
 * Reworked for laser drawing by Nicolas Thill <nico@openwrt.org>
 * 
 * This is free software, licensed under Creative Commons Attribution-ShareAlike 3.0 Unported License (CC BY-SA 3.0)
 * See /LICENSE for more information.
 */

#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

int main( ){
    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 0,0, OF_FULLSCREEN);
	ofRunApp(new testApp());
}

