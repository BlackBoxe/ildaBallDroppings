/*
 * Based on BallDroppings by artist/designer Josh Nimoy (C) 2003
 * Reworked for laser drawing by Nicolas Thill <nico@openwrt.org>
 * 
 * This is free software, licensed under Creative Commons Attribution-ShareAlike 3.0 Unported License (CC BY-SA 3.0)
 * See /LICENSE for more information.
 */

#include "V3.h"

// for sqrt
#include <cmath>

V3::V3(){
	x=0;
	y=0;
	z=0;
}

V3::V3(float xx,float yy,float zz){
	x=xx;
	y=yy;
	z=zz;
}

float V3::dot(V3 vec){
	return ((x*vec.x)+(y*vec.y)+(z*vec.z));
}

void V3::copyFrom(V3 that){
	x=that.x;
	y=that.y;
	z=that.z;
}

void V3::copyFrom(float xx,float yy,float zz){
	x=xx;
	y=yy;
	z=zz;
}

void V3::copyFrom(float xx,float yy){
	x=xx;
	y=yy;
}

V3 V3::getRightNormal(){
	return V3(y,-x,0);
}

V3 V3::getLeftNormal(){
	return V3(-y,x,0);
}

void V3::normalize(){
	float norm = getLength();
	x/=norm;
	y/=norm;
	z/=norm;
}

float V3::getLength(){
	return std::sqrt( x*x + y*y + z*z );
}

void V3::scaleVec(float scalar){
	x*=scalar;
	y*=scalar;
	z*=scalar;
}

V3 V3::minVecNew(V3 vec){
	return V3( x - vec.x , y - vec.y , z - vec.z );
}


void V3::operator*=(float a){
	x*=a;
	y*=a;
	z*=a;
}

void V3::operator+=(V3 v){
	x+=v.x;
	y+=v.y;
	z+=v.z;
}

