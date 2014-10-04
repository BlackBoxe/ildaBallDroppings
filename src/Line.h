/*
 * Based on BallDroppings by artist/designer Josh Nimoy (C) 2003
 * Reworked for laser drawing by Nicolas Thill <nico@openwrt.org>
 *
 * This is free software, licensed under Creative Commons Attribution-ShareAlike 3.0 Unported License (CC BY-SA 3.0)
 * See /LICENSE for more information.
 */

#ifndef _LINE_H_
#define _LINE_H_

#include "Ball.h"
#include "V3.h"

class Line {
	public:
		float x1;
		float y1;
		float x2;
		float y2;
		ofFloatColor color;
		void initMem();
		Line();
		Line(float _x1,float _y1, float _x2,float _y2);
		~Line();
		void set1(float x,float y);
		void  set2(float x,float y);
		int whichSideY(float x,float y);
		int fixDirection();
		bool checkBallCollide(Ball *ball);
		float checkAngle( float point_x, float point_y,float line_x, float line_y, V3 lineVec);
		bool diffSign(float a,float b);

};

#endif /* _LINE_H */

