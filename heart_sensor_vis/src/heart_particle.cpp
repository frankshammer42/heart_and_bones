//
//  heart_particle.cpp
//  heart_sensor_vis
//
//  Created by Frankshammer42 on 10/22/18.
//

#include "heart_particle.hpp"

//------------------------------------------------------------------
heart_particle::heart_particle(){
	attractPt.x = ofGetWidth()/2;
	attractPt.y = ofGetHeight()/2;
	got_heart_beat = false;
	side_space = 30; 
}

//------------------------------------------------------------------
void heart_particle::setMode(particleMode newMode){
	mode = newMode;
}

void heart_particle::change_status(bool getBeat){
	got_heart_beat = getBeat;
}

//------------------------------------------------------------------
void heart_particle::reset(){
	//the unique val allows us to set properties slightly differently for each particle
	uniqueVal = ofRandom(-10000, 10000);

	pos.x = ofRandomWidth();
	pos.y = ofRandomHeight();

	vel.x = ofRandom(-3.9, 3.9);
	vel.y = ofRandom(-3.9, 3.9);

	frc   = ofPoint(0,0,0);

	scale = ofRandom(0.5, 1.0);

	drag  = ofRandom(0.95, 0.998);
	mode = PARTICLE_MODE_ATTRACT;
}

//------------------------------------------------------------------
void heart_particle::switchMode(){
	if (mode == PARTICLE_MODE_ATTRACT){
		mode = PARTICLE_MODE_REPEL;
	}
	else{
		mode = PARTICLE_MODE_ATTRACT;
	}
}

//------------------------------------------------------------------
void heart_particle::update(){
	if (got_heart_beat){
		if (switch_counter > 95){
			switchMode();
			switch_counter = 0;
		}
	}
	else{
		mode = PARTICLE_MODE_ATTRACT;
	}
    switch_counter++;

	if ( mode == PARTICLE_MODE_ATTRACT ){
		//ofPoint attractPt(ofGetMouseX(), ofGetMouseY());
		frc = attractPt-pos; // we get the attraction force/vector by looking at the mouse pos relative to our pos
		frc.normalize(); //by normalizing we disregard how close the particle is to the attraction point

		vel *= drag; //apply drag
		vel += frc * 0.6; //apply force
	}
	else if( mode == PARTICLE_MODE_REPEL ){
		//ofPoint attractPt(ofGetMouseX(), ofGetMouseY());
		frc = attractPt-pos;

		float dist = frc.length();
		frc.normalize();

		vel *= drag;
		if( dist < 150 ){
			vel += -frc * 0.6; //notice the frc is negative
		}else{
			frc.x = ofSignedNoise(uniqueVal, pos.y * 0.01, ofGetElapsedTimef()*0.2);
			frc.y = ofSignedNoise(uniqueVal, pos.x * 0.01, ofGetElapsedTimef()*0.2);
			vel += frc * 0.04;
		}
	}

	pos += vel;

	if( pos.x > ofGetWidth() - side_space){
		pos.x = ofGetWidth() - side_space;
		vel.x *= -1.0;
	}else if( pos.x < side_space ){
		pos.x = side_space;
		vel.x *= -1.0;
	}
	if( pos.y > ofGetHeight() - side_space){
		pos.y = ofGetHeight() - side_space;
		vel.y *= -1.0;
	}
	else if( pos.y < side_space){
		pos.y = side_space;
		vel.y *= -1.0;
	}


}

//------------------------------------------------------------------
void heart_particle::draw(){

	if( mode == PARTICLE_MODE_ATTRACT ){
		ofSetColor(255, 63, 180);
	}
	else if( mode == PARTICLE_MODE_REPEL ){
		ofSetColor(208, 255, 63);
	}
	ofDrawCircle(pos.x, pos.y, scale);
}
