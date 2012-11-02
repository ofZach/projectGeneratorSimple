#include "CodeliteLinuxProject.h"

#define LOG_NAME "codelite"

CodeliteLinuxProject::CodeliteLinuxProject()
{
    codeliteDirName = "codelite/";
}

CodeliteLinuxProject::~CodeliteLinuxProject()
{
    //dtor
}


void CodeliteLinuxProject::setup(){
    templatePath = ofToDataPath("templates/codeliteLinux/", true);
}

bool CodeliteLinuxProject::createProjectFile(){
    ofDirectory dir(projectDir);
	if(!dir.exists()) dir.create(true);

	ofDirectory codeliteDir(ofFilePath::join(projectDir, codeliteDirName));
    if(!codeliteDir.exists()) codeliteDir.create(true);

    //prepare paths
    ofFile project(ofFilePath::join(codeliteDir.getAbsolutePath(), projectName + ".project"));
    string src =  ofFilePath::join(templatePath,"emptyExample.project");
    string dst = project.path();

    bool ret = true;

    //copy the project file over if necessary
     if(!project.exists()){
        ret = ofFile::copyFromTo(src,dst);
        if(!ret){
            ofLogError(LOG_NAME) << "error copying codelite template from " << src << " to " << dst;
            return false;
        }else{
            findandreplaceInTexfile(dst, "emptyExample", projectName);
        }
    }

    ofFile workspace(ofFilePath::join(codeliteDir.getAbsolutePath(), projectName + ".workspace"));
    if(!workspace.exists()){
		src = ofFilePath::join(templatePath,"emptyExample.workspace");
		dst = workspace.path();
		ret = ofFile::copyFromTo(src,dst);
		if(!ret){
			ofLogError(LOG_NAME) << "error copying workspace template from "<< src << " to " << dst;
			return false;
		}else{
			findandreplaceInTexfile(dst, "emptyExample", projectName);
		}
    }

    ofFile makefile(ofFilePath::join(projectDir,"Makefile"));
    if(!makefile.exists()){
		src = ofFilePath::join(templatePath,"Makefile");
		dst = makefile.path();
		ret = ofFile::copyFromTo(src,dst);
		if(!ret){
			ofLogError(LOG_NAME) << "error copying Makefile template from " << src << " to " << dst;
			return false;
		}
    }

    ofFile config(ofFilePath::join(projectDir,"config.make"));
    if(!config.exists()){
    	src = ofFilePath::join(templatePath,"config.make");
    	dst = config.path();
    	ret = ofFile::copyFromTo(src,dst);
    	if(!ret){
    		ofLogError(LOG_NAME) << "error copying config.make template from " << src << " to " << dst;
    		return false;
    	}
    }

    // handle the relative roots.
    string relRoot = getOFRelPath(ofFilePath::removeTrailingSlash(projectDir));
    if (relRoot != "../../../"){
        string relPath2 = relRoot;
        relPath2.erase(relPath2.end()-1);
        findandreplaceInTexfile(projectDir + "config.make", "../../..", relPath2);
        findandreplaceInTexfile(ofFilePath::join(projectDir , projectName + ".workspace"), "../../../", relRoot);
        findandreplaceInTexfile(ofFilePath::join(projectDir , projectName + ".cbp"), "../../../", relRoot);
    }

    return ret;
}

bool CodeliteLinuxProject::loadProjectFile(){
    ofFile project(projectDir + codeliteDirName + projectName + ".project");
	if(!project.exists()){
		ofLogError(LOG_NAME) << "error loading" << project.path() << "doesn't exist";
		return false;
	}
	pugi::xml_parse_result result = doc.load(project);
	bLoaded =result.status==pugi::status_ok;
	return bLoaded;
}

bool CodeliteLinuxProject::saveProjectFile(){
    findandreplaceInTexfile(ofFilePath::join(projectDir , codeliteDirName+projectName + ".workspace"),"emptyExample",projectName);
    pugi::xpath_node_set title = doc.select_nodes("//CodeLite_Project[@Name]");
    if(!title.empty()){
        if(!title[0].node().attribute("Name").set_value(projectName.c_str())){
            ofLogError(LOG_NAME) << "can't set title";
        }
    }
    return doc.save_file((projectDir + codeliteDirName + projectName + ".project").c_str());
}

void CodeliteLinuxProject::addSrc(string srcFile, string folder){
    std::vector<string> path = ofSplitString(folder, "/");
    pugi::xml_node node = doc.child("CodeLite_Project");
    for(unsigned int i=0;i<path.size();i++){
        string folder = path[i];
        if(folder != ".."){
            pugi::xml_node newNode = node.find_child_by_attribute("VirtualDirectory", "Name", folder.c_str());
            if(newNode == NULL){
                newNode = node.append_child();
                newNode.set_name("VirtualDirectory");
                pugi::xml_attribute nameAttr = newNode.append_attribute("Name");
                nameAttr.set_value(folder.c_str());
            }
            node = newNode;
        }
    }

    pugi::xml_node fileNode = node.append_child();
    fileNode.set_name("File");
    pugi::xml_attribute fileAttr = fileNode.append_attribute("Name");
    string fullPath = srcFile;
    if(codeliteDirName != "")
        fullPath = "../"+fullPath;
    fileAttr.set_value(fullPath.c_str());

    doc.save_file((projectDir + codeliteDirName + projectName + ".project").c_str());
}

void CodeliteLinuxProject::addInclude(string path){
    //ofLogNotice() << "adding include " << path;
    //appendValue(doc, "VirtualDirectory", "Name", path, true);
}
void CodeliteLinuxProject::addLibrary(string libraryName, LibType libType ){
    //ofLogNotice() << "adding library " << libraryName;
    //appendValue(doc, "Dependencies", "library", libraryName, true);
}
