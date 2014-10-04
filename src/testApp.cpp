/*
 * Based on BallDroppings by artist/designer Josh Nimoy (C) 2003
 * Reworked for laser drawing by Nicolas Thill <nico@openwrt.org>
 *
 * This is free software, licensed under Creative Commons Attribution-ShareAlike 3.0 Unported License (CC BY-SA 3.0)
 * See /LICENSE for more information.
 */

#include "testApp.h"

#define MY_DEF_BALL_DROP_RATE  3000
#define MY_DEF_FREQUENCY_RANGE  50000
#define MY_DEF_FRICTION  0.99997f
#define MY_DEF_GRAVITY  0.01f

#define MY_DEF_PPS  25000

#define MY_FRAME_W  400
#define MY_FRAME_H  300

long testApp::myBallDropRate = MY_DEF_BALL_DROP_RATE;
float testApp::myFriction = MY_DEF_FRICTION;
float testApp::myFrequencyRange = MY_DEF_FREQUENCY_RANGE;
float testApp::myGravity = MY_DEF_GRAVITY;

static char *stats =
    "ball drop rate: %i [- +]\n"
    "friction: %f [f F]\n"
    "gravity: %f [g G]\n"
    "pitch range: %f [r R]\n"
    "\n"
    "shift-space clears balls\n"
    "ctrl-space clears lines\n"
    "space alone clears all\n"
    "ctrl-z to undo line editing\n"
    "ctrl-f toggles fullscreen\n"
    "w inverts video\n"
    "b moves balldropper to mouse\n"
    "p pause\n";

void testApp::setup()
{
    my_balls = new List();
    my_lines = new List();
    my_line = 0;

    wFrame = MY_FRAME_W;
    hFrame = MY_FRAME_H;
    xFrameMin = (ofGetWidth() - wFrame) / 2;
    yFrameMin = (ofGetHeight() - hFrame) / 2;
    xFrameMax = xFrameMin + wFrame;
    yFrameMax = yFrameMin + hFrame;

    ofBackground(0);
	font.loadFont("Verdana.ttf",8);

	oldMillis=0;

	hole.copyFrom(xFrameMin + wFrame / 2, yFrameMin);

	paused = false;

	//load a new ball.
	Ball *b = new Ball(hole);
	my_balls->enqueue(b);

    etherdream.setup();
    etherdream.setPPS(MY_DEF_PPS);

    ildaFrame.params.output.transform.doFlipX = true;
    ildaFrame.params.output.transform.doFlipY = true;
}

//--------------------------------------------------------------
void testApp::update()
{
    Link *link;

	if (!paused) {
        step();
	}

    ildaFrame.clear();

    List_for_each(my_lines, link) {
        Line *l = (Line *)(link->data);
        ofPolyline o;
        o.addVertex((l->x1 - xFrameMin) / MY_FRAME_W, (l->y1 - yFrameMin) / MY_FRAME_H);
        o.addVertex((l->x2 - xFrameMin) / MY_FRAME_W, (l->y2 - yFrameMin) / MY_FRAME_H);
        ildaFrame.params.output.color = l->color;
        ildaFrame.addPoly(o);
	}

    List_for_each(my_balls, link) {
        Ball *b = (Ball *)(link->data);
        ofPolyline o = ofPolyline();
        o.arc((b->x - xFrameMin) / MY_FRAME_W, (b->y -yFrameMin) / MY_FRAME_H, (b->jitter*5.0f+2) / MY_FRAME_W, (b->jitter*5.0f+2) / MY_FRAME_H, 0, 360, MY_CIRCLE_RES);
        ildaFrame.params.output.color = b->color;
        ildaFrame.addPoly(o);
    }

    ildaFrame.update();

    etherdream.setPoints(ildaFrame);
}

void testApp::draw()
{
    ildaFrame.draw(xFrameMin, yFrameMin, MY_FRAME_W, MY_FRAME_H);

	ofSetColor(255);
	ofNoFill();
    ofRect(xFrameMin - 2, yFrameMin - 2, MY_FRAME_W + 4, MY_FRAME_H + 4);

    ofSetColor(255, 255, 255, 255 );
    char displayString[512];
    sprintf(displayString, stats, myBallDropRate, myFriction, myGravity, myFrequencyRange);
    font.drawString(displayString,20,20);
    font.drawString(ildaFrame.getString(), 20, 300);
}


void testApp::step()
{
    Link *my_ball_link, *my_ball_link_temp;
    Link *my_line_link;

	if (glutGet(GLUT_ELAPSED_TIME) - oldMillis > myBallDropRate) {
		createBall();
		oldMillis = glutGet(GLUT_ELAPSED_TIME);
	}

    List_for_each_safe(my_balls, my_ball_link, my_ball_link_temp) {
		Ball *b = (Ball*)(my_ball_link->data);
        if ((b->x < xFrameMin) || (b->x > xFrameMax) || (b->y < yFrameMin) || (b->y > yFrameMax) || b->force.getLength() == 0) {
            my_balls->remove(my_ball_link);
            delete b;
        } else {
            b->applyForce(0, myGravity); // gravity
            List_for_each(my_lines, my_line_link) {
                Line *l = (Line*)(my_line_link->data);
                if (l->checkBallCollide(b)) {
                    b->amnesia();
                    b->bounce(l->x1, l->y1, l->x2, l->y2);
                    break; // skip the rest of the lines
                }
            }
            b->stepPhysics();
        }
	}
}


void testApp::createBall()
{
	Ball *b = new Ball(hole);
	b->applyForce(0.0001,0); // give it an initial push
    my_balls->enqueue(b);
}


void testApp::resetWorld(){
    resetLines();
    resetBalls();
    resetVars();
}


void testApp::resetVars(){
    myBallDropRate = MY_DEF_BALL_DROP_RATE;
    myFriction = MY_DEF_FRICTION;
    myFrequencyRange = MY_DEF_FREQUENCY_RANGE;
    myGravity = MY_DEF_GRAVITY;
	hole.copyFrom(400,100,0);
}


void testApp::resetBalls(){
    Ball *b;
    while (b = (Ball *)my_balls->dequeue()) {
        delete b;
    }
}


void testApp::resetLines(){
    Line *l;
    while (l = (Line *)my_lines->dequeue()) {
        delete l;
    }
}


void testApp::keyPressed(int key){
    if (key == 6) { //toggle full screen
		ofToggleFullscreen();
	} else if (key == 'B' || key == 'b') {
		hole.copyFrom(mouseX,mouseY,0);
	} else if (key == 'r') {
		myFrequencyRange /= 1.09;
	} else if (key == 'R') {
		myFrequencyRange *= 1.09;
	} else if (key == 'f') {
		myFriction += 0.0001f;
	} else if (key == 'F') {
		myFriction -= 0.0001f;
	} else if (key == 'g') {
		myGravity-=0.001;
	} else if (key == 'G') {
		myGravity += 0.001;
	} else if (key == 'p' || key == 'P') {
		paused = !paused;
	} else if (key == '0') {
		resetVars();
	} else if (key == 45 || key == 95) { // -
		myBallDropRate -=100;
	} else if (key == 61 || key == 43) { // +
		myBallDropRate += 100;
	} else if (key == ' ') {
		if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {
            resetLines();
		} else if (glutGetModifiers() == GLUT_ACTIVE_SHIFT ){
            resetBalls();
		} else { // else kill both the lines and the balls.
            resetBalls();
            resetLines();
	  }
    }
}


void testApp::mouseDragged(int x, int y, int button)
{
    if (!my_line) return;
    if ((x > xFrameMin) && (x < xFrameMax)) {
        my_line->x2 = x;
    }
    if ((y > yFrameMin) && (y < yFrameMax)) {
        my_line->y2 = y;
    }
}


void testApp::mousePressed(int x, int y, int button)
{
    if (my_line) return;
    if (x < xFrameMin) return;
    if (x > xFrameMax) return;
    if (y < yFrameMin) return;
    if (y > yFrameMax) return;

    my_line = new Line(x,y,x,y);
    my_lines->enqueue(my_line);
}


void testApp::mouseReleased(int x, int y, int button)
{
    if (!my_line) return;
    if (x < xFrameMin) {
        my_line->x2 = xFrameMin;
    } else if (x > xFrameMax) {
        my_line->x2 = xFrameMax;
    } else {
        my_line->x2 = x;
    }
    if (y < yFrameMin) {
        my_line->y2 = yFrameMin;
    } else if (y > yFrameMax) {
        my_line->y2 = yFrameMax;
    } else {
        my_line->y2 = y;
    }
    my_line = 0;
}
