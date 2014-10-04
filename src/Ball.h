/*
 * Based on BallDroppings by artist/designer Josh Nimoy (C) 2003
 * Reworked for laser drawing by Nicolas Thill <nico@openwrt.org>
 *
 * This is free software, licensed under Creative Commons Attribution-ShareAlike 3.0 Unported License (CC BY-SA 3.0)
 * See /LICENSE for more information.
 */

#ifndef _BALL_H_
#define _BALL_H_

#include "V3.h"

#define MY_CIRCLE_RES 5
#define MY_USE_SOUND 1

#if defined (MY_USE_SOUND)
#include "ofSoundPlayer.h"
#endif


class Ball: public V3{
	public:
		ofFloatColor color;
#if defined (MY_USE_SOUND)
		ofSoundPlayer sound;
#endif
		V3 oldPos;
		V3 force;
        float jitter;
		unsigned char volume;
		long *lastBounceTimes;
		unsigned long  bounceTimeDelta;
		unsigned long tooMuchBouncingThreshold;

		void setPos(float x,float y);
		Ball(V3 v);
		Ball(V3 v,float oldX_,float oldY_,float forceX_,float forceY_,float jitter);
		Ball();
		~Ball();
		void stepPhysics();
		void applyForce(float x,float y);
		void initMem();
		void bounce(float x1,float y1,float x2,float y2);
		void amnesia();
};

#endif

