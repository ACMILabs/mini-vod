#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxXmlSettings.h"
#include <fstream>

struct videoItem {
	string title;
	string description;
	string videoFile;
	string imageFile;
	ofImage posterImage;
	ofRectangle itemBox;
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
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
		
		// new methods for ACMIVod
		void setVideoPlaypause();
		void drawVideo();
		void drawMenu();
		void launchVideo(int videoId);
		void returnToMenu();
		void updateMenuItems();

		ofVideoPlayer	video_player;
		float			video_width;
		float			video_height;
		float			video_ratio;
		float			video_pos_y;
		bool			paused;

		// icons and controlbar
		ofImage			icon_play;
		ofImage			icon_play_select;
		ofImage			icon_pause;
		ofRectangle		icon_playpause_background;
		ofImage			icon_back;
		ofRectangle		icon_back_background;
		float			icon_size, icon_size_larger;
		ofParameter<float>	icon_padding;
		float			icon_back_pos_x;
		float			icon_playpause_pos_x;
		float			icon_playpause_pos_y;
		bool			icon_playpause_hover;
		float			controlbar_width;
		float			controlbar_height;
		float			controlbar_pos_y;
		ofParameter<ofColor> icon_highlight_color;

		// progress bar
		ofRectangle		progress_bar;
		ofRectangle		progress_bar_played;
		float			video_duration;
		float			video_percent_played;

		// fonts
		ofTrueTypeFont			font_stats;
		ofParameter<string>		font_stats_filename;
		ofTrueTypeFont			font_main_normal;
		ofParameter<string>		font_main_normal_filename;
		ofTrueTypeFont			font_main_italic;
		ofParameter<string>		font_main_italic_filename;

		ofParameter<ofColor> fonts_title_color;

		// show hide
		bool			show_controls;
		bool			show_stats;
		
		// gui
		ofxPanel			gui;
		ofParameter<float>	controlbar_show_length;
		ofParameter<float>	controlbar_anim_length;
		ofParameter<float>	fonts_space_between_words;
		ofParameter<float>	menu_margin_x;
		ofParameter<float>	menu_margin_y;
		
		// timers

		float			controlbar_start_time;
		float			controlbar_timer_end;

		float			menu_start_time;
		float			menu_timer_end;

		// menu

		bool					show_menu;
		ofImage					menu_background;
		ofParameter<string>		menu_background_filename;

		// video tiles

		vector<videoItem>	video_items;
		int					num_video_items;

		// settings
		ofxXmlSettings	settings;

		// version number
		string			ACMIVod_version_number;
};
