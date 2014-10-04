/*
 * Based on BallDroppings by artist/designer Josh Nimoy (C) 2003
 * Reworked for laser drawing by Nicolas Thill <nico@openwrt.org>
 * 
 * This is free software, licensed under Creative Commons Attribution-ShareAlike 3.0 Unported License (CC BY-SA 3.0)
 * See /LICENSE for more information.
 */

#include "Ball.h"
#include "testApp.h"

#include <math.h>

//----------------------------------------------------
Ball::Ball(){
  initMem();
}
//----------------------------------------------------
void Ball::initMem(){
  oldPos.copyFrom(0,0,0);
  force.copyFrom(0,0,0);
  volume = 0;

#if defined (MY_USE_SOUND)
  sound.loadSound("sine.wav");
  sound.setVolume(0.75f);
#endif

  lastBounceTimes = new long[16];
  jitter = 0;
  bounceTimeDelta = 10000;
  tooMuchBouncingThreshold = 300;
}
//----------------------------------------------------
Ball::Ball(V3 v):V3(v.x,v.y,0){
  initMem();
  oldPos.copyFrom(v.x,v.y,0);
}
//------------------------------------------------------------------------------
Ball::Ball(V3 v,float oldX_,float oldY_,float forceX_,float forceY_,float jitter):V3(v.x,v.y,0){
  initMem();
  copyFrom(v);
  oldPos.copyFrom(oldX_,oldY_,0);
  force.copyFrom(forceX_,forceY_);
}
//----------------------------------------------------
Ball::~Ball(){
#if defined (MY_USE_SOUND)
	sound.stop();
#endif
	delete [] lastBounceTimes;
}
//----------------------------------------------------
void Ball::setPos(float _x,float _y){
  x = _x;
  y = _y;
}
//----------------------------------------------------
void Ball::stepPhysics(){
  //apply the forces
  oldPos.copyFrom(x,y,0);
  x+= force.x;
  y+= force.y;

  force *= testApp::myFriction;

  if(jitter>0)jitter-=0.1;
}
//----------------------------------------------------
void Ball::applyForce(float applyX,float applyY){
  force += V3(applyX,applyY,0);
}
//----------------------------------------------------
void Ball::bounce(float x1,float y1,float x2,float y2){

	//Thank you to Theo Watson for helping me out here.
	//V
	V3 v(force);

	//N
	V3 n(x2-x1,y2-y1,0);
	n = n.getLeftNormal();
	n.normalize();

	//2 * V [dot] N
	float dotVec = v.dot(n) * 2;

	// ( 2 * V [dot] N ) N
	n.scaleVec(dotVec);

	//V - ( 2 * V [dot] N ) N
	//change direction
	V3 mvn(v.minVecNew(n));
	force.copyFrom(mvn);

	////now some checks, with some sound playback

  for(int i=15;i>0;i--){//shift the queue
    lastBounceTimes[i] = lastBounceTimes[i-1];
  }
  lastBounceTimes[0] = (glutGet(GLUT_ELAPSED_TIME));//then add the new value

  //now check for unusual behavior
  bounceTimeDelta = lastBounceTimes[0] - lastBounceTimes[15];
  if (bounceTimeDelta<tooMuchBouncingThreshold){ //softeners for the balls
    force.copyFrom(0,0);//make it still
  } else {
    unsigned long freq = force.getLength() * testApp::myFrequencyRange;

#if defined (MY_USE_SOUND)
	sound.setSpeed(freq/44100.0);
	sound.play();
#endif

    jitter = force.getLength();
  }

}

//----------------------------------------------------
void Ball::amnesia(){
  oldPos.copyFrom(x,y,0);
}

