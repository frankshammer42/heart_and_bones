//
//  heart_particle.hpp
//  heart_sensor_vis
//
//  Created by Frankshammer42 on 10/22/18.
//

#ifndef heart_particle_hpp
#define heart_particle_hpp

#include <stdio.h>

#endif /* heart_particle_hpp */

#pragma once
#include "ofMain.h"

enum particleMode{
    PARTICLE_MODE_ATTRACT = 0,
    PARTICLE_MODE_REPEL,
};

class heart_particle{
	public:
		heart_particle();
		
		void setMode(particleMode newMode);
		void switchMode();
		void change_status(bool getBeat);
		
		void reset();
		void update();
		void draw();
		
		ofPoint attractPt;
		ofPoint pos;
		ofPoint vel;
		ofPoint frc;
		
		float drag;
		float uniqueVal;
		float scale;

		int switch_counter; // increase the counter until it reaches certain number
		bool got_heart_beat;
        int side_space;
		
		particleMode mode;
		
};

