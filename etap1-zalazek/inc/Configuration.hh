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

void AddMobileObject(const std::string &Name,const Vector3D &Shift,  const Vector3D &Scale,
const Vector3D &Rotation,const Vector3D &Trans,  const std::string &RGB );


void AddLibraryName(const std::string &LibraryName){_LibList.push_back(LibraryName);};

LibraryList GetLibraryList() {return _LibList;};

MobileObjectList GetMobileObjList(){return _MObjList;};

void PrintMobileObjectList()
{
    cout<<endl<<"Obiekty na scenie:\n";
    for(MobileObjectList::iterator i = _MObjList.begin(); i!=_MObjList.end(); ++i)
     {
        cout<<"\t->\t"<<i->first<<":\t";
        cout<<"Pos"<<i->second->GetPositoin_m();
        cout<<"\tRPY("<<i->second->GetAng_Roll_deg()<<","
        <<i->second->GetAng_Pitch_deg()<<","<<i->second->GetAng_Yaw_deg()<<")\n";
     }   

    cout<<endl<<endl;
}

};


#endif
