#ifndef CONFIGURATION_HH
#define CONFIGURATION_HH

#include<vector>

#include "MobileObj.hh"
#include "Scene.hh"
using namespace std;


typedef std::vector<std::string> LibraryList;

class Configuration {
  
LibraryList LibList;


MobileObjectList MObjList;
public:

void AddMobileObject(const std::string &Name, const std::string &RGB,Vector3D &Scale);


void AddLibraryName(const std::string &LibraryName){LibList.push_back(LibraryName);};

LibraryList GetLibraryList() {return LibList;};

MobileObjectList GetMobileObjList(){return MObjList;};


};


#endif
