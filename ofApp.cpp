#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ACMIVod_version_number = "ACMI Video on Demand v 0.0.1.1";

	ofBackground(0, 0, 0);
	ofSetVerticalSync(true);

	ofSetWindowTitle("ACMI VoD");

	// read the directory for the images
	// we know that they are named in seq
	
	string path = "videos/";
	string imageFileTemp;
	string metaDataFileTemp;
	ofDirectory dir(path);
	dir.allowExt("mp4");

	int nFiles = dir.listDir();
	if (nFiles) {

		for (int i = 0; i<dir.size(); i++) {

			// add the image to the vector
			
			ofLog(OF_LOG_SILENT) << "Video found: " << dir.getPath(i);

			videoItem vid;

			vid.videoFile = dir.getPath(i);
			imageFileTemp = dir.getPath(i);
			
			vid.imageFile = imageFileTemp.replace(imageFileTemp.end() - 4, imageFileTemp.end(), ".jpg");
			
			ifstream vidMetaData;
			
			metaDataFileTemp = dir.getPath(i);
			string fileToOpen = metaDataFileTemp.replace(metaDataFileTemp.end() - 4, metaDataFileTemp.end(), ".txt");

			vidMetaData.open( ofToDataPath(fileToOpen).c_str() );
			ofLog(OF_LOG_SILENT) << "metadata file opened: " << vidMetaData.is_open();

			getline(vidMetaData, vid.title);
			ofLog(OF_LOG_SILENT) << "video title found: " << vid.title;
			
			getline(vidMetaData, vid.description);
			ofLog(OF_LOG_SILENT) << "video description found: " << vid.description;
		
			vidMetaData.close();
						
			video_items.push_back(vid);

		}

	}
	else ofLog(OF_LOG_WARNING) << "Could not find folder";

	// gui
		
	gui.setup("settings");
	gui.add(controlbar_show_length.set("controbar_show_length", 3000.0, 0.0, 10000.0));
	gui.add(controlbar_anim_length.set("controlbar_anim_length", 500.0, 0.0, 10000.0));
	gui.add(icon_padding.set("icon_padding", 10.0, 0.0, 50.0));
	gui.add(fonts_space_between_words.set("fonts_space_between_words", 8.5, 0.0, 50.0));
	gui.add(menu_margin_x.set("menu_margin_x", 0.15, 0.0, 1.0));
	gui.add(menu_margin_y.set("menu_margin_y", 0.1, 0.0, 1.0));
	gui.add(fonts_title_color.set("font_title_color", ofColor(252, 69, 19), ofColor(0, 0), ofColor(255, 255)));
	gui.add(icon_highlight_color.set("icon_highlight_color", ofColor(252, 69, 19), ofColor(0, 0), ofColor(255, 255)));
	gui.add(menu_background_filename.set("menu_background_img_filename", "backgrounds/background_menu.jpg"));
	gui.add(font_main_normal_filename.set("font_main_regular_filename_ttf", "fonts/opensans-regular.ttf"));
	gui.add(font_main_italic_filename.set("font_main_italic_filename_ttf", "fonts/opensans-italic.ttf"));
	gui.add(font_stats_filename.set("font_stats_filename_ttf", "fonts/opensans-regular.ttf"));
		
	// Attempt to load XML settings from file
	gui.loadFromFile("settings.xml");

	// set up video sizes

	video_width = ofGetWidth();
	video_height = ofGetWidth() / 1.777776;
	video_pos_y = (ofGetHeight() / 2) - (video_height / 2);
	
	// icons
	icon_size = 64;
	icon_size_larger = 80;
	icon_play_select.load("icons/play_select.png");
	icon_play.load("icons/play.png");
	icon_pause.load("icons/pause.png");
	icon_back.load("icons/back.png");
	icon_play_select.resize(icon_size_larger, icon_size_larger);
	icon_play.resize(icon_size, icon_size);
	icon_pause.resize(icon_size, icon_size);
	icon_back.resize(icon_size, icon_size);
	icon_playpause_hover = false;

	controlbar_width = ofGetWidth();
	controlbar_height = icon_size * 2.5;
	controlbar_pos_y = ofGetHeight();

	// fonts

	//old OF default is 96 - but this results in fonts looking larger than in other programs. 
	ofTrueTypeFont::setGlobalDpi(72);
	
	font_stats.load(font_stats_filename, 14, true, true);
	font_stats.setLineHeight(18.0f);
	font_stats.setLetterSpacing(1.037);

	font_main_normal.load(font_main_normal_filename, 24, true, true);
	font_main_normal.setLineHeight(30.0f);
	font_main_normal.setLetterSpacing(1);

	font_main_italic.load(font_main_italic_filename, 24, true, true);
	font_main_italic.setLineHeight(30.0f);
	font_main_italic.setLetterSpacing(1);

	// stats and controls
	show_stats = false;
	show_controls = false;

	// timers

	controlbar_timer_end = false;
	controlbar_start_time = ofGetElapsedTimeMillis();

	menu_timer_end = false;
	menu_start_time = ofGetElapsedTimeMillis();

	// menu

	show_menu = true;

	num_video_items = video_items.size();
	updateMenuItems();

}

// Launch Video (called from menu)
//--------------------------------------------------------------
void ofApp::launchVideo(int videoId) {
	video_player.load(video_items[videoId].videoFile);
	video_player.setLoopState(OF_LOOP_NONE);
	video_player.play();
	controlbar_timer_end = false;
	controlbar_start_time = ofGetElapsedTimeMillis();
	show_controls = true;
	paused = false;
	show_menu = false;
}

// Update Menu Items (called during setup, and whenever the window is resized
//--------------------------------------------------------------

void ofApp::updateMenuItems() {

	menu_background.load(menu_background_filename);
	menu_background.resize(ofGetHeight() * 1.777777778, ofGetHeight());

	for (int i = 0; i < num_video_items; i++) {
		video_items[i].itemBox.x = 0 + (ofGetWidth() * menu_margin_x);
		video_items[i].itemBox.width = ofGetWidth() - (ofGetWidth() * (menu_margin_x * 2));
		video_items[i].itemBox.height = ((ofGetHeight() - (ofGetHeight() * (menu_margin_y * 2))) / num_video_items) - 4;
		video_items[i].itemBox.y = ((video_items[i].itemBox.height + 4) * i) + (ofGetHeight() * menu_margin_y);
		// reload the poster images as we're going to be resizing them again (otherwise you degrade the already resized version)
		video_items[i].posterImage.load(video_items[i].imageFile);
		// then check that it's loaded
		if (video_items[i].posterImage.isAllocated()) {
			video_items[i].posterImage.resize((video_items[i].itemBox.height - 30) * 1.77777778, video_items[i].itemBox.height - 30);
		}
	}
}

// set pause
//--------------------------------------------------------------

void ofApp::setVideoPlaypause() {
	controlbar_timer_end = false;
	controlbar_start_time = ofGetElapsedTimeMillis();
	paused = !paused;
	if (paused) {
		show_controls = true;
	}
	video_player.setPaused(paused);
}

void ofApp::returnToMenu() {
	show_menu = true;
	video_player.stop();
	menu_timer_end = false;
	menu_start_time = ofGetElapsedTimeMillis();
}

//--------------------------------------------------------------
void ofApp::update(){
	video_player.update();
	if (video_player.getPosition() > 0.9999 && show_menu == false) {
		returnToMenu();
	}

}

//--------------------------------------------------------------
void ofApp::drawVideo() {
	ofSetColor(255, 255, 255);
	video_player.draw(0, video_pos_y, video_width, video_height);

	float controlbar_timer = ofGetElapsedTimeMillis() - controlbar_start_time;

	float hide_anim_timer = ofMap(controlbar_timer, controlbar_show_length, controlbar_show_length + controlbar_anim_length, 1.0, 0.0);
	float hide_back_anim_timer = ofMap(controlbar_timer, controlbar_show_length, controlbar_show_length + controlbar_anim_length, 0.0, 1.0);
	
	if (controlbar_timer >= controlbar_show_length) {
		controlbar_timer_end = true;
	}

	if (video_player.isPlaying() && show_stats) {
		font_stats.drawString("Video is playing", 30, 650);
	}
	else if (show_stats) {
		font_stats.drawString("Video is not playing", 30, 650);
	}

	if (controlbar_timer <= controlbar_show_length) {
		controlbar_pos_y = ofGetHeight() - controlbar_height;
		icon_back_pos_x = 30;
		if (show_stats) {
			font_stats.drawString("We should be displaying a controlbar", 30, 600);
		}
	}
	if ((controlbar_timer >= controlbar_show_length) &&
		!paused &&
		(controlbar_timer <= controlbar_show_length + controlbar_anim_length + 1)) {
		// calculate position of the controlbar
		controlbar_pos_y = ofGetHeight() - (controlbar_height * hide_anim_timer);
		// calculate position of the back button
		icon_back_pos_x = 30 - ((icon_padding + icon_size + 40) * hide_back_anim_timer);
		ofHideCursor();
		show_controls = false;
		if (show_stats) {
			font_stats.drawString("We should be hiding a controlbar", 30, 600);
		}
	}

	// Draw control bar
	ofSetColor(26, 26, 26, 96);
	ofEnableAlphaBlending();
	ofDrawRectangle(0, controlbar_pos_y, controlbar_width, controlbar_height);

	// Draw progress bar
	//ofSetHexColor(0x333333);
	ofSetColor(0, 0, 0, 96);
	progress_bar.x = 50;
	progress_bar.y = controlbar_pos_y + 15;
	progress_bar.width = controlbar_width - 100;
	progress_bar.height = icon_size / 2;
	ofDrawRectRounded(progress_bar, 15);

	
	//ofSetHexColor(0xFC4513);
	ofSetColor(icon_highlight_color);
	progress_bar_played.x = progress_bar.x;
	progress_bar_played.y = progress_bar.y;
	progress_bar_played.width = progress_bar.width * video_player.getPosition();
	progress_bar_played.height = progress_bar.height;
	ofDrawRectRounded(progress_bar_played, 15);

	// get position of play/pause button
	icon_playpause_pos_x = controlbar_width / 2 - icon_size / 2;
	//icon_playpause_pos_y = controlbar_pos_y + (controlbar_height / 2) - (icon_size / 2);
	icon_playpause_pos_y = controlbar_pos_y + (controlbar_height / 2) - (progress_bar.height / 2);

	// draw button background if hovered
	if (icon_playpause_hover) {
		//ofSetHexColor(0xFC4513);
		ofSetColor(icon_highlight_color);
		icon_playpause_background.x = icon_playpause_pos_x - icon_padding;
		icon_playpause_background.y = icon_playpause_pos_y - icon_padding;
		icon_playpause_background.width = icon_size + (icon_padding * 2);
		icon_playpause_background.height = icon_size + (icon_padding * 2);
		ofDrawRectRounded(icon_playpause_background, 5);
	}
	// draw background for back button
	//ofSetHexColor(0xFC4513);
	ofSetColor(icon_highlight_color);
	icon_back_background.x = icon_back_pos_x - icon_padding;
	icon_back_background.y = 30 - icon_padding;
	icon_back_background.width = icon_size + (icon_padding * 2);
	icon_back_background.height = icon_size + (icon_padding * 2);
	ofDrawRectRounded(icon_back_background, 5);
	// draw button
	ofSetColor(255);
	if (paused) {
		icon_play.draw(icon_playpause_pos_x, icon_playpause_pos_y);
	}
	else {
		icon_pause.draw(icon_playpause_pos_x, icon_playpause_pos_y);
	}
	icon_back.draw(icon_back_pos_x, 30);
	ofDisableAlphaBlending();
	//}

}

void ofApp::drawMenu() {
	ofSetColor(255);
	
	// timers
	float menu_timer = (ofGetElapsedTimeMillis() - menu_start_time) - 10;
	//float show_anim_timer = ofMap(menu_timer, 0, 1000.0, 1.0, 0.0);
	float fade_in_timer = ofMap(menu_timer, 0, 500.0, 0.0, 1.0);

	vector<float> menu_timers;

	for (int i = 0; i < num_video_items; i++) {
		// easing algorithm from http://robertpenner.com/easing/
		// and: http://gizma.com/easing/
	
		// Linear
		//float anim_timer = 1.0 * menu_timer / 1000.0 + 0;
	
		
		// Quadratic easing out
		float time_current, b_start_value, change_value, duration;
		
		int j;

		j = num_video_items - i;

		time_current = menu_timer + (j * 125);
		b_start_value = 0.0;
		change_value = 1.0;
		duration = 1500.0;

		
		if (time_current > duration) {
			time_current = duration;
		}

		time_current /= duration;

		//float anim_timer = change_value * time_current * time_current + b_start_value;
		float anim_timer = -change_value * time_current * (time_current-2) + b_start_value;

		if (anim_timer > 1.0) {
			anim_timer = 1.0;
		}

		// Make it go in the other direction!
		float show_anim_timer = 1 - anim_timer;

		menu_timers.push_back(show_anim_timer);
	}

	// position variables for drawing strings
	float string_x, string_y;

	if (menu_timer >= 1500.0) {
		menu_timer_end = true;
	}

	// draw background
	ofEnableAlphaBlending();
	ofSetColor(255, 255, 255, 255 * fade_in_timer);
	menu_background.draw(0, 0);

	for (int i = 0; i < num_video_items; i++) {

		// animate the video tiles into position
		if (menu_timer <= 1500.0) {
			// set destination position
			video_items[i].itemBox.x = 0 + (ofGetWidth() * menu_margin_x);
			// apply animation to that position
			video_items[i].itemBox.x -= ((ofGetWidth() * 1.5) * menu_timers[i]);
		}
		else {
			video_items[i].itemBox.x = 0 + (ofGetWidth() * menu_margin_x);
		}

		ofSetColor(16,16,16,208);
		ofDrawRectRounded(video_items[i].itemBox, 5);
		// draw poster image
		ofSetColor(255);
		if (video_items[i].posterImage.isAllocated()) {
			video_items[i].posterImage.draw(video_items[i].itemBox.x + 15, video_items[i].itemBox.y + 15);
			// draw play icon over poster image
			ofSetColor(255,255,255,192);
			icon_play_select.draw(video_items[i].itemBox.x + 15 + (video_items[i].posterImage.getWidth() / 2) - (icon_size_larger / 2), video_items[i].itemBox.y + 15 + (video_items[i].posterImage.getHeight() / 2) - (icon_size_larger / 2));
			
		}
		
		// draw Title
		ofSetColor(fonts_title_color);
		string_x = 0;
		string_y = 0;
		vector<string> splitString = ofSplitString(video_items[i].title, " ");
		for (int j = 0; j < splitString.size(); j++) {
			if (splitString[j][0] == '_') {
				font_main_italic.drawString(splitString[j].substr(1, splitString[j].size() - 2), video_items[i].itemBox.x + 30 + video_items[i].posterImage.getWidth() + string_x, video_items[i].itemBox.y + 35);
				string_x += font_main_italic.stringWidth(splitString[j].substr(1, splitString[j].size() - 2)) + fonts_space_between_words;
			}
			else {
				font_main_normal.drawString(splitString[j], video_items[i].itemBox.x + 30 + video_items[i].posterImage.getWidth() + string_x, video_items[i].itemBox.y + 35);
				string_x += font_main_normal.stringWidth(splitString[j]) + fonts_space_between_words;
			}
		}
		// draw Description
		ofSetHexColor(0xFFFFFF);			
		splitString = ofSplitString(video_items[i].description, " ");
		string_x = 0;
		string_y = 0;
		// loop through the results
		for (int j = 0; j<splitString.size(); j++) {
			if (splitString[j][0] == '_') {
				font_main_italic.drawString(splitString[j].substr(1, splitString[j].size() - 2), video_items[i].itemBox.x + 30 + video_items[i].posterImage.getWidth() + string_x, video_items[i].itemBox.y + 65 + (string_y * 30));
				string_x += font_main_italic.stringWidth(splitString[j].substr(1, splitString[j].size() - 2)) + fonts_space_between_words;
			}
			else {
				font_main_normal.drawString(splitString[j] + " ", video_items[i].itemBox.x + 30 + video_items[i].posterImage.getWidth() + string_x, video_items[i].itemBox.y + 65 + (string_y * 30));
				string_x += font_main_normal.stringWidth(splitString[j]) + fonts_space_between_words;
			}
			
			// if we're past the width of the tile, wrap around to the next line
			if (string_x >(video_items[i].itemBox.width - 30 - video_items[i].posterImage.getWidth() - 150)) {
				string_x = 0;
				string_y++;
			}
		}

	}
	ofDisableAlphaBlending();
	if (show_stats) {
		font_stats.drawString("animation timer: " + to_string(menu_timers[0]), 30, ofGetHeight() / 2);
	}
	if (show_stats) {
		font_stats.drawString("menu_start_time: " + to_string(menu_start_time), 30, (ofGetHeight() / 2) + 30);
		font_stats.drawString("menu_timer: " + to_string(menu_timer), 30, (ofGetHeight() / 2) + 60);
		font_stats.drawString("fade_in_timer: " + to_string(fade_in_timer), 30, (ofGetHeight() / 2) + 90);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(255);
	
	if (show_menu) {
		drawMenu();
	}
	else {
		drawVideo();
	}

	if (show_stats) {
		gui.draw();
		
		font_stats.drawString("Controlbar start time: " + to_string(controlbar_start_time), 30, ofGetHeight() - 160);
		font_stats.drawString("Video duration: " + to_string(video_player.getDuration()), 30, ofGetHeight() - 120);
		font_stats.drawString("Video progress: " + to_string(video_player.getPosition()), 30, ofGetHeight() - 90);
		font_stats.drawString("Video width: " + to_string(video_width), 30, ofGetHeight() - 60);
		font_stats.drawString("Video height: " + to_string(video_height), 30, ofGetHeight() - 30);

		// draw version number
		font_stats.drawString(ACMIVod_version_number, ofGetWidth() - font_stats.stringWidth(ACMIVod_version_number) - 30, 30);

	}

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
	case ' ':
		if (!show_menu) {
			setVideoPlaypause();
		}
		break;
	case 'f':
		ofToggleFullscreen();
		// update video sizes
		video_width = ofGetWidth();
		video_height = ofGetWidth() / 1.777776;
		video_pos_y = (ofGetHeight() / 2) - (video_height / 2);

		// update control bar size
		controlbar_width = ofGetWidth();

		// update menu item sizes
		updateMenuItems();
		break;
	case 's':
		show_stats = !show_stats;
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	ofShowCursor();
	if (!show_menu) {
		icon_playpause_hover = false;
		if (show_controls) {
			if (x >= (icon_playpause_pos_x - icon_padding) &&
				x <= (icon_playpause_pos_x + icon_size + icon_padding) &&
				y >= (icon_playpause_pos_y - icon_padding) &&
				y <= (icon_playpause_pos_y + icon_size + icon_padding)) {
				icon_playpause_hover = true;
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if (!show_menu) {
		if (x >= progress_bar.x &&
			x <= progress_bar.x + progress_bar.width &&
			y >= progress_bar.y &&
			y <= progress_bar.y + progress_bar.height) {

			video_player.setPosition((x - progress_bar.x) / progress_bar.width);
			controlbar_timer_end = false;
			controlbar_start_time = ofGetElapsedTimeMillis();
		}
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){


}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	if (!show_menu) {
		// handle clicking on play / pause button
		if (x >= (icon_playpause_pos_x - icon_padding) &&
			x <= (icon_playpause_pos_x + icon_size + icon_padding) &&
			y >= (icon_playpause_pos_y - icon_padding) &&
			y <= (icon_playpause_pos_y + icon_size + icon_padding)) {

			setVideoPlaypause();
		}
		// handle clicking on progress bar
		else if (x >= progress_bar.x &&
			x <= progress_bar.x + progress_bar.width &&
			y >= progress_bar.y &&
			y <= progress_bar.y + progress_bar.height) {

			video_player.setPosition((x - progress_bar.x) / progress_bar.width);
			controlbar_timer_end = false;
			controlbar_start_time = ofGetElapsedTimeMillis();
		}
		else if (x >= icon_back_background.x &&
			x <= icon_back_background.x + icon_back_background.width &&
			y >= icon_back_background.y &&
			y <= icon_back_background.y + icon_back_background.height) {
			returnToMenu();
		}
		else if (true) {
			if (y <= controlbar_pos_y) {
				setVideoPlaypause();
			}
		}
	}
	else if (show_menu) {
		for (int i = 0; i < num_video_items; i++) {
			if (x >= video_items[i].itemBox.x &&
				x <= video_items[i].itemBox.x + video_items[i].itemBox.width &&
				y >= video_items[i].itemBox.y &&
				y <= video_items[i].itemBox.y + video_items[i].itemBox.height) {
				launchVideo(i);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	// update video sizes
	video_width = ofGetWidth();
	video_height = ofGetWidth() / 1.777776;
	video_pos_y = (ofGetHeight() / 2) - (video_height / 2);
	
	// update control bar size
	controlbar_width = ofGetWidth();

	// update menu item sizes
	updateMenuItems();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
