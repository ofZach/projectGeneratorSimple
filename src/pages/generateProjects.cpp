#include "generateProjects.h"
#include "Utils.h"
#include <stdio.h>
#include "ofConstants.h"





//------------------------------------------------------
string generateProjects::setupForTarget(int targ){

    if(project){
		delete project;
	}
    string target;
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
        case OF_TARGET_LINUX64:
            project = new CBLinuxProject;
            target = "linux64";
            break;
        case OF_TARGET_LINUX:
            project = new CBLinuxProject;
            target = "linux";
            break;
    }

    project->setup(target);

    return target;
}



void generateProjects::setStatus(string newStatus){
    statusEnergy = 1;
    status = newStatus;
    statusSetTime = ofGetElapsedTimef();
}



//--------------------------------------------------------------
void generateProjects::setup(){
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetVerticalSync(true);

    
    statusEnergy = 0;
    
    mode = 0;
    bInited = false;
    project = NULL;
    sketchName = "mySketch";
	

   
    //-------------------------------------
    // calculate the bin path (../../../ on osx) and the sketch path (bin -> root - > defaultLoc)
    //-------------------------------------

    // if appToRoot is wrong, we have alot of issues.  all these paths are used in this project:

#ifdef TARGET_OSX
    string binPath = ofFilePath::getAbsolutePath(ofFilePath::join(ofFilePath::getCurrentWorkingDirectory(), "../../../"));
#else
    string binPath = ofFilePath::getCurrentExeDir();
#endif

    string ofRoot = ofFilePath::getAbsolutePath(ofFilePath::join(binPath, settings.appRoot));
    string examplesPath = ofFilePath::join(ofRoot, "examples");
    
    addonsPath = ofFilePath::getAbsolutePath(ofFilePath::join(ofRoot,"addons"));
    sketchPath = ofFilePath::getAbsolutePath(ofFilePath::join(ofRoot, defaultLoc));


    convertWindowsToUnixPath(ofRoot);
    convertWindowsToUnixPath(addonsPath);
    convertWindowsToUnixPath(sketchPath);

    // there's some issues internally in OF with non unix paths for OF root
    setOFRoot(ofRoot);

    //-------------------------------------
    // get settings
    //-------------------------------------

    //  Sketch button
    //
    button.font = font;
    button.secondFont = secondFont;
   

    //  Path button
    //
    button.topLeftAnchor.set(76, 160+40);
    button.deliminater = "/";
    button.prefix = "Base OF path: ";
    button.setText(sketchPath);
    button.secondaryText = "<< CLICK TO CHANGE THE DIRECTORY";
	button.topLeftAnchor.set(button.topLeftAnchor.x, button.topLeftAnchor.y + button.rect.height + 20);
    buttons.push_back(button);

    
    button.deliminater = "/";
    button.prefix = "Examples path: ";
    button.setText(examplesPath);
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
    button.setText(platform);

    button.topLeftAnchor.set(button.topLeftAnchor.x, button.topLeftAnchor.y + button.rect.height + 20);
    buttons.push_back(button);


    //  Generate
    //
    generateButton = button;
    generateButton.topLeftAnchor.set(906, 535);
	//generateButton.setColor(ofColor(50, 150, 255));
    generateButton.deliminater = ",";
    generateButton.prefix = "GENERATE PROJECT";
    generateButton.bSelectable = true;
    generateButton.setText("");
    generateButton.bDrawLong = false;
    
    addonButton = button;
    addonButton.topLeftAnchor.set(906, 535);
    addonButton.prefix = "<< BACK";
    addonButton.setText("");
    addonButton.bDrawLong = false;
    

    for (int i = 0; i < buttons.size(); i++){
        buttons[i].calculateRect();
    }
    addonButton.calculateRect();
    generateButton.calculateRect();

    

    //-------------------------------------
    // platform panel (not used, really, but here just in case)
    //-------------------------------------
    panelPlatforms.setup();
    panelPlatforms.add(wincbToggle.setup("windows (codeblocks)",ofGetTargetPlatform()==OF_TARGET_WINGCC));
	panelPlatforms.add(winvsToggle.setup("windows (visual studio)", ofGetTargetPlatform()==OF_TARGET_WINVS));
	panelPlatforms.add(linuxcbToggle.setup("linux (codeblocks)",ofGetTargetPlatform()==OF_TARGET_LINUX));
	panelPlatforms.add(linux64cbToggle.setup("linux64 (codeblocks)",ofGetTargetPlatform()==OF_TARGET_LINUX64));

//for ios, we need to fake that the target is ios (since we're compiling w/ osx OF)

//#define MAKE_IOS
    
#ifdef MAKE_IOS
	panelPlatforms.add(osxToggle.setup("osx (xcode)",false));
	panelPlatforms.add(iosToggle.setup("ios (xcode)",true));
#else
    panelPlatforms.add(osxToggle.setup("osx (xcode)",ofGetTargetPlatform()==OF_TARGET_OSX));
	panelPlatforms.add(iosToggle.setup("ios (xcode)",ofGetTargetPlatform()==OF_TARGET_IPHONE));
#endif
    
    
    // update the platforms text in the platform button
    string platforms = "";
    for (int i = 0; i < panelPlatforms.getNumControls(); i++){
        if (*((ofxToggle *)panelPlatforms.getControl(i))){
            if (platforms.length() > 0) platforms+=", ";
            platforms += ((ofxToggle *)panelPlatforms.getControl(i))->getName();

        };
    }
    buttons[2].setText(platforms);


    panelPlatforms.setPosition(10,40);
    panelCoreAddons.setPosition(10,40);
    panelOtherAddons.setPosition(330,40);
   

    logo.loadImage("images/ofw-logo.png");

    ofBackground(230,230,230);
    
    
    generateButton.topLeftAnchor.set(ofGetWidth() - buttons[0].rect.x - generateButton.rect.width + 10 ,
                                     ofGetHeight() - generateButton.rect.height - 40);// 535);
    generateButton.calculateRect();
    
    addonButton.topLeftAnchor.set(ofGetWidth() - buttons[0].rect.x - addonButton.rect.width + 10 ,
                                  ofGetHeight() - addonButton.rect.height - 40);// 535);
    addonButton.calculateRect();

}






//--------------------------------------------------------------
void generateProjects::update(){

    float diff = ofGetElapsedTimef()- statusSetTime;
    if (diff > 3){
        statusEnergy *= 0.99;;
    }
    //-------------------------------------
    // if we are in addon mode check
    //-------------------------------------

    if (mode == MODE_ADDON ) addonButton.checkMousePressed(ofPoint(ofGetMouseX(), ofGetMouseY()));


    //-------------------------------------
    // layout our normal buttons, check the mouse
    //-------------------------------------

    for (int i = 0; i < buttons.size(); i++){
        buttons[i].calculateRect();
        buttons[i].checkMousePressed(ofPoint(ofGetMouseX(), ofGetMouseY()));
    }

    generateButton.checkMousePressed(ofPoint(ofGetMouseX(), ofGetMouseY()));
    
    for (int i = 0; i < buttons.size(); i++){
        if (i != 0){
			buttons[i].topLeftAnchor.y = buttons[i-1].topLeftAnchor.y +buttons[i-1].rect.height + 20;
        }
    }


    //-------------------------------------
    // addons panels can be really long, so use the mouse pos to move them if we need to
    //-------------------------------------

    if (panelCoreAddons.getShape().height > ofGetHeight()){
        float pct = ofMap(ofGetMouseY(), 0,ofGetHeight(), 0,1,true);
        float diff = panelCoreAddons.getShape().height - ofGetHeight();
        panelCoreAddons.setPosition(panelCoreAddons.getPosition().x,-diff * pct);
    }

    if (panelOtherAddons.getShape().height > ofGetHeight()){
        float pct = ofMap(ofGetMouseY(), 0,ofGetHeight(), 0,1,true);
        float diff = panelOtherAddons.getShape().height - ofGetHeight();
        panelOtherAddons.setPosition(panelOtherAddons.getPosition().x,-diff * pct);
    }

}

//--------------------------------------------------------------
void generateProjects::draw(){

    
    //cout << ofFilePath::join(ofFilePath::join(ofFilePath::getCurrentExeDir(), "../../../"), appToRoot) << endl;
    
    cout << settings.appRoot << endl;
    //cout << ofFilePath::join(ofFilePath::join(ofFilePath::getCurrentExeDir(), "../../../"), appToRoot) << endl;
    
    if (mode != MODE_ADDON ) {
        
        ofSetColor(100);
        logo.draw(64, 61,logo.getWidth(),logo.getHeight());
        
        //ofSetColor(74,255,203);
        titleFont->drawString("PROJECT", 64 + logo.getWidth() + 25, 85);
        titleFont->drawString("GENERATOR",  64 + logo.getWidth() + 25, 117);
    }
    
	if (mode == 0){
		for (int i = 0; i < buttons.size(); i++){
			buttons[i].draw();
		}
        
        generateButton.draw();
        
    } else if (mode == 1){
        panelCoreAddons.draw();
        if (bHaveNonCoreAddons){
            panelOtherAddons.draw();
        }
    } else if (mode == 2){
        panelPlatforms.draw();
    }
    //cout << panelAddons.getShape().height << endl;


    
    
    if (mode == 1 ){
        addonButton.draw();
        
        ofRectangle rect = secondFont->getStringBoundingBox("select core and non-core addons to add", addonButton.topLeftAnchor.x-200, 60);
        ofSetColor(220,220,220);
        ofRect(rect.x-10, rect.y-10, rect.width+20, rect.height+20);
        ofSetColor(0,0,0);
        secondFont->drawString("select core and non-core addons to add", addonButton.topLeftAnchor.x-200, 60);
    }
    
    if (mode == 0){
        ofFill();
        ofSetColor(0 + 220 * (1-statusEnergy),0 + 220 * (1-statusEnergy),0 + 220 * (1-statusEnergy));
        ofRect(0,ofGetHeight(), ofGetWidth(), -25);
        ofSetColor(255,255,255, 255 * statusEnergy);
        ofDrawBitmapString(status, 10,ofGetHeight()-8);
    }
}

//--------------------------------------------------------------
void generateProjects::keyPressed(int key){

    if (key == ' '){

        //printf("%s -------- \n", ResultBuffer);
        //std::exit(0);
    }


}

void generateProjects::generateProject(){

    vector <int> targetsToMake;
	if( osxToggle )		targetsToMake.push_back(OF_TARGET_OSX);
	if( iosToggle )		targetsToMake.push_back(OF_TARGET_IPHONE);
	if( wincbToggle )	targetsToMake.push_back(OF_TARGET_WINGCC);
	if( winvsToggle )	targetsToMake.push_back(OF_TARGET_WINVS);
	if( linuxcbToggle )	targetsToMake.push_back(OF_TARGET_LINUX);
	if( linux64cbToggle )	targetsToMake.push_back(OF_TARGET_LINUX64);

	if( targetsToMake.size() == 0 ){
		cout << "Error: makeNewProjectViaDialog - must specifiy a project to generate " <<endl;
		ofSystemAlertDialog("Error: makeNewProjectViaDialog - must specifiy a project platform to generate");
        return;
	}

    if (buttons[0].myText.size() == 0){
        ofSystemAlertDialog("Error: project must have a name");
        return;
    }



    printf("start with project generation \n");

    string path = ofFilePath::join(buttons[1].myText, buttons[0].myText);

	for(int i = 0; i < (int)targetsToMake.size(); i++){
		string target = setupForTarget(targetsToMake[i]);
        if(project->create(path)){
            vector<string> addonsToggles = panelCoreAddons.getControlNames();
            for (int i = 0; i < (int) addonsToggles.size(); i++){
                ofxToggle toggle = panelCoreAddons.getToggle(addonsToggles[i]);
                if(toggle){
                    ofAddon addon;
                    addon.pathToOF = getOFRelPath(path);
                    addon.fromFS(ofFilePath::join(addonsPath, addonsToggles[i]),target);
                    project->addAddon(addon);

                }
            }


            addonsToggles = panelOtherAddons.getControlNames();
            for (int i = 0; i < (int) addonsToggles.size(); i++){
                ofxToggle toggle = panelOtherAddons.getToggle(addonsToggles[i]);
                if(toggle){
                    ofAddon addon;

                    addon.pathToOF = getOFRelPath(path);
                    cout << getOFRelPath(path) << " " << path << endl;
                    addon.fromFS(ofFilePath::join(addonsPath, addonsToggles[i]),target);
                    project->addAddon(addon);

                }
            }

            project->save(true);
        }
	}


    printf("done with project generation \n");
    setStatus("generated: " + buttons[1].myText + "/" + buttons[0].myText);

    // go through the control panels, do stuff
}

//--------------------------------------------------------------
void generateProjects::keyReleased(int key){



}

//--------------------------------------------------------------
void generateProjects::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void generateProjects::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void generateProjects::mousePressed(int x, int y, int button){

    if (mode == MODE_NORMAL){


        // check the mouse for press

        for (int i = 0; i < buttons.size(); i++){
            buttons[i].checkMousePressed(ofPoint(x, y));
        }
        

       

       
        //-------------------------------------
        // 0 = OF root
        //-------------------------------------

        if (buttons[0].bMouseOver == true){

            string command = "";

            ofDirectory dir( getOFRoot() );


        #ifdef TARGET_WIN32
                    ofFileDialogResult res = ofSystemLoadDialog("please select OF root folder", true, windowsFromUnixPath(dir.path()));
        #else 
                    ofFileDialogResult res = ofSystemLoadDialog("please select OF root folder", true, dir.path());
        #endif
            

            if (res.bSuccess){
                string result = res.filePath;
                convertWindowsToUnixPath(result);
                buttons[0].setText( result );
                setStatus("path set to: " + result);
            }


        }
        
        
        //-------------------------------------
        // 1 = examples directory
        //-------------------------------------
        
        if (buttons[1].bMouseOver == true){
            
            string command = "";
            
            ofDirectory dir( ofFilePath::join(getOFRoot(), "examples" ));
            
            
#ifdef TARGET_WIN32
            ofFileDialogResult res = ofSystemLoadDialog("please select examples folder", true, windowsFromUnixPath(dir.path()));
#else
            ofFileDialogResult res = ofSystemLoadDialog("please select examples folder", true, dir.path());
#endif
            
            
            if (res.bSuccess){
                string result = res.filePath;
                convertWindowsToUnixPath(result);
                buttons[1].setText( result );
                setStatus("path set to: " + result);
            }
        }


        //-------------------------------------
        // 2 = platform  (disabled)
        //-------------------------------------


        if (buttons[2].bMouseOver == true){
            // platform is diabled for now
             mode = 2;
        }
        
    }

    if (mode == MODE_PLATFORM){

    }
}

//--------------------------------------------------------------
void generateProjects::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void generateProjects::windowResized(int w, int h){
    generateButton.topLeftAnchor.set(ofGetWidth() - buttons[0].rect.x - generateButton.rect.width + 10 ,
                                     ofGetHeight() - generateButton.rect.height - 40);// 535);
    generateButton.calculateRect();
    
    addonButton.topLeftAnchor.set(ofGetWidth() - buttons[0].rect.x - addonButton.rect.width + 10 ,
                                  ofGetHeight() - addonButton.rect.height - 40);// 535);
    addonButton.calculateRect();
    
}

//--------------------------------------------------------------
void generateProjects::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void generateProjects::dragEvent(ofDragInfo dragInfo){

}
