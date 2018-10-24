#pragma once

#include "ofMain.h" 
#include "heart_particle.hpp"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void resetParticles();
		void changeParticleMode();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofSoundPlayer hb_sound;
        int overall_switch_counter;
		ofSerial mySerial;
		
		//particleMode currentMode;
		string currentModeStr; 
		bool incoming_hb;

		vector <heart_particle> p;
};

