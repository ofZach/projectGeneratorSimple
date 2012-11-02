#ifndef CODELITELINUXPROJECT_H
#define CODELITELINUXPROJECT_H

#include <baseProject.h>

#define OF_TARGET_LINUX_CODELITE 99 //Philip: this is a little bit of an ugly hack but somehow we have to separate the linux codelite project from the general linux project

class CodeliteLinuxProject : virtual public baseProject
{
    public:
        CodeliteLinuxProject();
        virtual ~CodeliteLinuxProject();
        void setup();
        bool createProjectFile();
        bool loadProjectFile();
        bool saveProjectFile();
        void addSrc(string srcFile, string folder);
        void addInclude(string path);
        void addLibrary(string libraryName, LibType libType = RELEASE_LIB);
    protected:
    private:
        string codeliteDirName;
};

#endif // CODELITELINUXPROJECT_H
