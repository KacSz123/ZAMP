

#include"Configuration.hh"

void Configuration::AddMobileObject(const std::string &Name, const std::string &RGB, const Vector3D &Scale,
const Vector3D &Shift, const Vector3D &Trans, const Vector3D &Rotation )
{

 auto _nMobObj = std::make_shared<MobileObj>(); //nowy obiekt
 
 _nMobObj -> SetName(Name.c_str());
 _nMobObj -> SetColourInRGB(RGB);
 _nMobObj -> SetScale(Scale);
 _nMobObj -> SetShift_m(Shift);
 _nMobObj -> SetPosition_m(Trans);

 _nMobObj -> SetAng_Roll_deg(Rotation[0]);
 _nMobObj -> SetAng_Pitch_deg(Rotation[1]);
 _nMobObj -> SetAng_Yaw_deg(Rotation[2]);



 _MObjList.insert({Name, _nMobObj});
}

//typedef std::vector<std::string> LibraryList;



