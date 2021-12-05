#ifndef CONFIGURATION_HH
#define CONFIGURATION_HH

#include<vector>

#include "MobileObj.hh"
#include "Scene.hh"
using namespace std;


typedef std::vector<std::string> LibraryList;

class Configuration {
  
LibraryList _LibList;
MobileObjectList _MObjList;


public:

void AddMobileObject(const std::string &Name, const std::string &RGB, const Vector3D &Scale,
const Vector3D &Shift, const Vector3D &Trans, const Vector3D &Rotation );


void AddLibraryName(const std::string &LibraryName){_LibList.push_back(LibraryName);};

LibraryList GetLibraryList() {return _LibList;};

MobileObjectList GetMobileObjList(){return _MObjList;};


};


#endif
