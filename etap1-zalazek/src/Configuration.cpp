

#include"Configuration.hh"

void Configuration::AddMobileObject(const std::string &Name,
                          const std::string &RGB,Vector3D &Scale)
{

 auto _nMobObj = std::make_shared<MobileObj>(); //nowy obiekt
 
 _nMobObj -> SetName(Name.c_str());
 _nMobObj -> SetColourInRGB(RGB);
 _nMobObj -> SetScale(Scale);

 MObjList.insert({Name, _nMobObj});
}

//typedef std::vector<std::string> LibraryList;



