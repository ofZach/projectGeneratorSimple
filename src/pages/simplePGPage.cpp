#include "simplePGPage.h"
#include "Utils.h"
#include <stdio.h>
#include "ofConstants.h"


//------------------------------------------------------
bool simplePGPage::isAddonCore(string addon){


    if (bInited == false){
        coreAddons.push_back("ofx3DModelLoader");
        coreAddons.push_back("ofxAssimpModelLoader");
        coreAddons.push_back("ofxDirList");
        coreAddons.push_back("ofxNetwork");
        coreAddons.push_back("ofxOpenCv");
        coreAddons.push_back("ofxOsc");
        coreAddons.push_back("ofxThread");
        coreAddons.push_back("ofxThreadedImageLoader");
        coreAddons.push_back("ofxVectorGraphics");
        coreAddons.push_back("ofxVectorMath");
        coreAddons.push_back("ofxXmlSettings");
        coreAddons.push_back("ofxSvg");
        coreAddons.push_back("ofxGui");
        coreAddons.push_back("ofxKinect");
        bInited = true;
    }


    for (int i = 0; i < coreAddons.size(); i++){
        if (coreAddons[i] == addon){
            return true;
        }
    }
    return false;
}




//------------------------------------------------------
string simplePGPage::setupForTarget(int targ){

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



void simplePGPage::setStatus(string newStatus){
    statusEnergy = 1;
    status = newStatus;
    statusSetTime = ofGetElapsedTimef();
}



//--------------------------------------------------------------
void simplePGPage::setup(){
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

    addonsPath = ofFilePath::getAbsolutePath(ofFilePath::join(ofRoot,"addons"));
    sketchPath = ofFilePath::getAbsolutePath(ofFilePath::join(ofRoot, settings.defaultNewProjectLocation));

    
    
    
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
    button.setup(font, secondFont, "Name: ", sketchName, "<< CLICK TO CHANGE THE NAME", ofPoint(76, 160+40));
    buttons.push_back(button);
    
    //  Path button
    //
    button.setup(font, secondFont, "Path: ", sketchPath, "<< CLICK TO CHANGE THE DIRECTORY", ofPoint(76, button.topLeftAnchor.y + button.rect.height + 20), "/");
    buttons.push_back(button);

    //  Platform text
    //
    button.deliminater = ", ";
    button.prefix = "Platforms: ";
    button.secondaryText = "";
    button.bDrawLong = false;
    button.secondaryText = "";
    button.bSelectable = settings.multiplatform == true;
    button.setText(platform);

    button.topLeftAnchor.set(button.topLeftAnchor.x, button.topLeftAnchor.y + button.rect.height + 20);
    buttons.push_back(button);

    //  Addons button
    //
    button.deliminater = ", ";
    button.bDrawLong = true;
    button.prefix = "Addons: ";
    button.secondaryText = "<< CLICK TO SELECT ADDONS";
    button.bSelectable = true;
    button.setText(addons);

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

    platformButton = button;
    platformButton.topLeftAnchor.set(906, 535);
    platformButton.prefix = "<< BACK";
    platformButton.setText("");
    platformButton.bDrawLong = false;
    

    for (int i = 0; i < buttons.size(); i++){
        buttons[i].calculateRect();
    }
    addonButton.calculateRect();
    generateButton.calculateRect();
    platformButton.calculateRect();

    //-------------------------------------
    // addons panels:
    //-------------------------------------

    panelCoreAddons.setup();
    panelOtherAddons.setup();

    ofDirectory addons(addonsPath);

    addons.listDir();
    for(int i=0;i<(int)addons.size();i++){
    	string addon = addons.getName(i);

    	if(addon.find("ofx")==0){

            if (isAddonCore(addon)){
                ofxToggle * toggle = new ofxToggle();
                panelCoreAddons.add(toggle->setup(addon,false,300));
            } else {
                bHaveNonCoreAddons = true;
                ofxToggle * toggle = new ofxToggle();
                panelOtherAddons.add(toggle->setup(addon,false,300));
            }


    	}
    }

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
void simplePGPage::update(){

    float diff = ofGetElapsedTimef()- statusSetTime;
    if (diff > 3){
        statusEnergy *= 0.99;;
    }
    //-------------------------------------
    // if we are in addon mode check
    //-------------------------------------

    if (mode == MODE_ADDON ) addonButton.checkMousePressed(ofPoint(ofGetMouseX(), ofGetMouseY()));
    if (mode == MODE_PLATFORM) platformButton.checkMousePressed(ofPoint(ofGetMouseX(), ofGetMouseY()));


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
void simplePGPage::draw(){

    
    
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


    
    
    if (mode == MODE_ADDON ){
        addonButton.draw();
        
        ofRectangle rect = secondFont->getStringBoundingBox("select core and non-core addons to add", addonButton.topLeftAnchor.x-200, 60);
        ofSetColor(220,220,220);
        ofRect(rect.x-10, rect.y-10, rect.width+20, rect.height+20);
        ofSetColor(0,0,0);
        secondFont->drawString("select core and non-core addons to add", addonButton.topLeftAnchor.x-200, 60);
    }
    
    
    if (mode == MODE_PLATFORM){
        panelPlatforms.draw();
        platformButton.draw();
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
void simplePGPage::keyPressed(int key){

    if (key == ' '){

        //printf("%s -------- \n", ResultBuffer);
        //std::exit(0);
    }


}

void simplePGPage::generateProject(){

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
void simplePGPage::keyReleased(int key){



}

//--------------------------------------------------------------
void simplePGPage::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void simplePGPage::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void simplePGPage::mousePressed(int x, int y, int button){

    if (mode == MODE_NORMAL){


        // check the mouse for press

        for (int i = 0; i < buttons.size(); i++){
            buttons[i].checkMousePressed(ofPoint(x, y));
        }
        

        //-------------------------------------
        // 4 = genearate
        //-------------------------------------

        
        if (generateButton.bMouseOver == true){
            generateProject();
        }

        //-------------------------------------
        // 0 = sketch name
        //-------------------------------------

        if (buttons[0].bMouseOver == true){
            string text = ofSystemTextBoxDialog("choose sketch name", buttons[0].myText);
            fixStringCharacters(text);
            setStatus("sketch name set to: " + text);
            buttons[0].setText(text);
        }

        //-------------------------------------
        // 1 = sketch path
        //-------------------------------------

        if (buttons[1].bMouseOver == true){

            string command = "";

            ofDirectory dir(ofFilePath::join(getOFRoot(),defaultLoc));

            if (!dir.exists()){
                dir.create();
            }

          	
        #ifdef TARGET_WIN32
                    ofFileDialogResult res = ofSystemLoadDialog("please select sketch folder", true, windowsFromUnixPath(dir.path()));
        #else 
                    ofFileDialogResult res = ofSystemLoadDialog("please select sketch folder", true, dir.path());
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
             mode = MODE_PLATFORM;
        }

        //-------------------------------------
        // 3 = addon
        //-------------------------------------

        if (buttons[3].bMouseOver == true){
            mode = MODE_ADDON;

        }
    }

    //-------------------------------------
    // handle addon mode
    //-------------------------------------

    if (mode == MODE_ADDON ){

        //-------------------------------------
        // if we hit he back button, collect the addons for display
        //-------------------------------------

        if (addonButton.bMouseOver){

            string addons = "";

            for (int i = 0; i < panelCoreAddons.getNumControls(); i++){
                if (*((ofxToggle *)panelCoreAddons.getControl(i))){
                   if (addons.length() > 0) addons+=", ";
                    addons += ((ofxToggle *)panelCoreAddons.getControl(i))->getName();

                }

            }
            for (int i = 0; i < panelOtherAddons.getNumControls(); i++){
                if (*((ofxToggle *)panelOtherAddons.getControl(i))){
                    if (addons.length() > 0) addons+=", ";
                    addons += ((ofxToggle *)panelOtherAddons.getControl(i))->getName();

                }

            }
            buttons[3].setText(addons);

            setStatus("addons set to: " + addons);

            addonButton.bMouseOver = false;
            mode = MODE_NORMAL;
        }
    }

    if (mode == MODE_PLATFORM){
        
        if (platformButton.bMouseOver){
            
            string platforms = "";
            
            for (int i = 0; i < panelPlatforms.getNumControls(); i++){
                if (*((ofxToggle *)panelPlatforms.getControl(i))){
                    if (platforms.length() > 0) platforms+=", ";
                    platforms += ((ofxToggle *)panelPlatforms.getControl(i))->getName();
                    
                }
                
            }
            
            buttons[2].setText(platforms);
            
            setStatus("platforms set to: " + platforms);
            
            platformButton.bMouseOver = false;
            mode = MODE_NORMAL;
            
        }
    }
}

//--------------------------------------------------------------
void simplePGPage::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void simplePGPage::windowResized(int w, int h){
    generateButton.topLeftAnchor.set(ofGetWidth() - buttons[0].rect.x - generateButton.rect.width + 10 ,
                                     ofGetHeight() - generateButton.rect.height - 40);// 535);
    generateButton.calculateRect();
    
    addonButton.topLeftAnchor.set(ofGetWidth() - buttons[0].rect.x - addonButton.rect.width + 10 ,
                                  ofGetHeight() - addonButton.rect.height - 40);// 535);
    addonButton.calculateRect();
    
    platformButton.topLeftAnchor.set(ofGetWidth() - buttons[0].rect.x - addonButton.rect.width + 10 ,
                                  ofGetHeight() - addonButton.rect.height - 40);// 535);
    platformButton.calculateRect();
    
}

//--------------------------------------------------------------
void simplePGPage::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void simplePGPage::dragEvent(ofDragInfo dragInfo){

}
