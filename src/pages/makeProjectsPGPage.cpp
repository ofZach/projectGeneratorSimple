#include "makeProjectsPGPage.h"
#include "Utils.h"
#include <stdio.h>

//--------------------------------------------------------------
void makeProjectsPGPage::setup(){
    
    
    //  Sketch button
    //
    
    textButton button;
    
    button.font = font;
    button.secondFont = secondFont;
    button.prefix = "Name: ";
	button.topLeftAnchor.set(76, 160+40); //set top button position - others are set relative to this.
    button.setText("");
    
    button.secondaryText = "<< CLICK TO CHANGE THE NAME";
    buttons.push_back(button);
    
    //  Path button
    //
    button.deliminater = "/";
    button.prefix = "Path: ";
    button.setText("");
    button.secondaryText = "<< CLICK TO CHANGE THE DIRECTORY";
	button.topLeftAnchor.set(button.topLeftAnchor.x, button.topLeftAnchor.y + button.rect.height + 20);
    buttons.push_back(button);
    
    //  Platform text
    //
    button.deliminater = ", ";
    button.prefix = "Platforms: ";
    button.secondaryText = "";
    button.bDrawLong = false;
    button.secondaryText = "";
    button.bSelectable = false;
    button.setText("");
    
    button.topLeftAnchor.set(button.topLeftAnchor.x, button.topLeftAnchor.y + button.rect.height + 20);
    buttons.push_back(button);
    
    //  Addons button
    //
    button.deliminater = ", ";
    button.bDrawLong = true;
    button.prefix = "Addons: ";
    button.secondaryText = "<< CLICK TO SELECT ADDONS";
    button.bSelectable = true;
    button.setText("");
    
    button.topLeftAnchor.set(button.topLeftAnchor.x, button.topLeftAnchor.y + button.rect.height + 20);
    buttons.push_back(button);
    
    
    
    
    //ofSetLogLevel(OF_LOG_VERBOSE);
	project = NULL;

	while(!checkConfigExists()){
		askOFRoot();
	}

	setOFRoot(getOFRootFromConfig());

	
	int targ = ofGetTargetPlatform();
	//plat = OF_TARGET_IPHONE;

    setupForTarget(targ);
    if(projectPath!="" || buildAllExamples){
    	for(int i = 0; i < (int)targetsToMake.size(); i++){
			setupForTarget(targetsToMake[i]);
			if(buildAllExamples){
				generateExamples();
			}else{
				project->setup(target);
				project->create(projectPath);
				vector < string > addons;
				parseAddonsDotMake(project->getPath() + "addons.make", addons);
				for (int i = 0; i < (int)addons.size(); i++){
					ofAddon addon;
					addon.fromFS(ofFilePath::join(ofFilePath::join(getOFRoot(), "addons"), addons[i]),target);
					project->addAddon(addon);
				}
				project->save(false);
			}
    	}
        std::exit(0);
    }

#ifndef COMMAND_LINE_ONLY
    

    
    
	examplesPanel.setup("generate examples", "examples.xml", 400, 10);
	examplesPanel.add(generateButton.setup("<--Generate"));
	examplesPanel.add(wincbToggle.setup("win CB projects",ofGetTargetPlatform()==OF_TARGET_WINGCC));
	examplesPanel.add(winvsToggle.setup("win VS projects", ofGetTargetPlatform()==OF_TARGET_WINVS));
	examplesPanel.add(linuxcbToggle.setup("linux CB projects",ofGetTargetPlatform()==OF_TARGET_LINUX));
	examplesPanel.add(linux64cbToggle.setup("linux64 CB projects",ofGetTargetPlatform()==OF_TARGET_LINUX64));
	examplesPanel.add(linuxarmv6lcbToggle.setup("linuxarmv6l CB projects",ofGetTargetPlatform()==OF_TARGET_LINUXARMV6L));
	examplesPanel.add(linuxarmv7lcbToggle.setup("linuxarmv7l CB projects",ofGetTargetPlatform()==OF_TARGET_LINUXARMV7L));
	examplesPanel.add(osxToggle.setup("osx projects",ofGetTargetPlatform()==OF_TARGET_OSX));
	examplesPanel.add(iosToggle.setup("ios projects",ofGetTargetPlatform()==OF_TARGET_IPHONE));

	generateButton.addListener(this,&makeProjectsPGPage::generateExamplesCB);

    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
	ofSetFrameRate(60);
#else
	std::exit(0);
#endif
}

void makeProjectsPGPage::setupForTarget(int targ){

    if(project){
		delete project;
	}

    switch(targ){
        case OF_TARGET_OSX:
            project = new xcodeProject;
            target = "osx";
            break;
        case OF_TARGET_WINGCC:
            project = new CBWinProject;
            target = "win_cb";
            break;
        case OF_TARGET_WINVS:
            project = new visualStudioProject;
            target = "vs";
            break;
        case OF_TARGET_IPHONE:
            project = new xcodeProject();
            target = "ios";
            break;
        case OF_TARGET_ANDROID:
            break;
        case OF_TARGET_LINUX:
            project = new CBLinuxProject;
            target = "linux";
            break;
        case OF_TARGET_LINUX64:
            project = new CBLinuxProject;
            target = "linux64";
            break;
        case OF_TARGET_LINUXARMV6L:
            project = new CBLinuxProject;
            target = "linuxarmv6l";
            break;
        case OF_TARGET_LINUXARMV7L:
            project = new CBLinuxProject;
            target = "linuxarmv7l";
            break;
    }
}

void makeProjectsPGPage::generateExamplesCB(){

#ifndef COMMAND_LINE_ONLY

	targetsToMake.clear();
	if( osxToggle )		targetsToMake.push_back(OF_TARGET_OSX);
	if( iosToggle )		targetsToMake.push_back(OF_TARGET_IPHONE);
	if( wincbToggle )	targetsToMake.push_back(OF_TARGET_WINGCC);
	if( winvsToggle )	targetsToMake.push_back(OF_TARGET_WINVS);
	if( linuxcbToggle )         targetsToMake.push_back(OF_TARGET_LINUX);
	if( linux64cbToggle )       targetsToMake.push_back(OF_TARGET_LINUX64);
	if( linuxarmv6lcbToggle )	targetsToMake.push_back(OF_TARGET_LINUXARMV6L);
	if( linuxarmv7lcbToggle )	targetsToMake.push_back(OF_TARGET_LINUXARMV7L);

	if( targetsToMake.size() == 0 ){
		cout << "Error: generateExamplesCB - must specifiy a project to generate " <<endl;
	}

	for(int i = 0; i < (int)targetsToMake.size(); i++){
		setupForTarget(targetsToMake[i]);
		generateExamples();
	}

	int target = ofGetTargetPlatform();
    setupForTarget(target);
#endif

}

void makeProjectsPGPage::generateExamples(){
    ofDirectory dir;
    string examplesPath = ofFilePath::join(getOFRoot(),"examples");

	ofLogNotice() << "Generating examples (from: " << examplesPath << ")";

    dir.listDir(examplesPath);

    for (int i = 0; i < (int)dir.size(); i++){

        // don't check subdirectories that aren't directories! (eg., .gitignore etc)
        if(!dir.getFile(i).isDirectory()) continue;

		if( target == "ios" ){
			if( dir.getName(i) != "ios" ) continue;
		}else{
			if (dir.getName(i) == "android" || dir.getName(i) == "ios") continue;
        }

        ofDirectory subdir;
        string examplesPath = dir.getPath(i);

		ofLogNotice() << "Generating examples in folder: " << examplesPath;

        subdir.listDir(examplesPath);

        for (int j = 0; j < (int)subdir.size(); j++){

            // don't create projects that aren't directories! (eg., .gitkeep etc)
            if(!subdir.getFile(j).isDirectory()) continue;

			ofLogNotice() << "------------------------------------------------";
			ofLogNotice() << "Generating example: " << subdir.getPath(j);
			ofLogNotice() << "------------------------------------------------";

            project->setup(target);
            project->create(subdir.getPath(j));
            vector < string > addons;
            parseAddonsDotMake(project->getPath() + "addons.make", addons);
            for (int i = 0; i < (int)addons.size(); i++){
                ofAddon addon;
                addon.pathToOF = getOFRelPath(subdir.getPath(j));
                addon.fromFS(ofFilePath::join(ofFilePath::join(getOFRoot(), "addons"), addons[i]),target);
                project->addAddon(addon);
            }
            project->save(false);

        }
    }
    ofLogNotice() << " ";
    ofLogNotice() << "Finished generating examples for " << target;
}




//--------------------------------------------------------------
void makeProjectsPGPage::update(){

    for (int i = 0; i < buttons.size(); i++){
        if (i != 0){
			buttons[i].topLeftAnchor.y = buttons[i-1].topLeftAnchor.y +buttons[i-1].rect.height + 20;
        }
    }


}

//--------------------------------------------------------------
void makeProjectsPGPage::draw(){

#ifndef COMMAND_LINE_ONLY
    //ofBackgroundGradient(ofColor::gray,ofColor::black);

    panelOptions.draw();
	examplesPanel.draw();

	ofSetColor(0,0,0,100);

	ofRect(ofPathRect);

    /*ofDrawBitmapString("press 'm' to make all files\npress ' ' to make a specific file", ofPoint(30,30));*/
	ofSetColor(255);
    ofDrawBitmapString(drawableOfPath, ofPathDrawPoint);
#endif

    
    
    for (int i = 0; i < buttons.size(); i++){
        buttons[i].draw();
    }
    
}

//--------------------------------------------------------------
void makeProjectsPGPage::keyPressed(int key){


}

//--------------------------------------------------------------
void makeProjectsPGPage::keyReleased(int key){

}

//--------------------------------------------------------------
void makeProjectsPGPage::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void makeProjectsPGPage::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void makeProjectsPGPage::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void makeProjectsPGPage::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void makeProjectsPGPage::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void makeProjectsPGPage::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void makeProjectsPGPage::dragEvent(ofDragInfo dragInfo){

}
//
////--------------------------------------------------------------
//void makeProjectsPGPage::setupDrawableOFPath(){
//
//#ifndef COMMAND_LINE_ONLY
//	vector<string> subdirs = ofSplitString("OF path: " + getOFRoot(), "/");
//	int textLength = 0;
//	int padding = 5;
//	string path = "";
//	int lines=1;
//	int fontSize = 8;
//	float leading = 1.7;
//
//	ofPathRect.x = padding;
//	ofPathRect.y = padding;
//	ofPathDrawPoint.x = padding*2;
//	ofPathDrawPoint.y = padding*2 + fontSize * leading;
//
//	for(int i = 0; i < subdirs.size(); i++) {
//		if (i > 0 && i<subdirs.size()-1) {
//			subdirs[i] += "/";
//		}
//		if(textLength + subdirs[i].length()*fontSize < ofGetWidth()-padding){
//			textLength += subdirs[i].length()*fontSize;
//			path += subdirs[i];
//		}else {
//			path += "\n";
//			textLength = 0;
//			lines++;
//		}
//	}
//	ofPathRect.width = textLength + padding*2;
//	if (lines > 1){
//		ofPathRect.width = ofGetWidth() - padding*2;
//	}
//	ofPathRect.height = lines * fontSize * leading + (padding*2);
//
//	drawableOfPath = path;
//
//	examplesPanel.setPosition(examplesPanel.getPosition().x, ofPathRect.y + ofPathRect.height + padding);
//#endif
//
//
//}
