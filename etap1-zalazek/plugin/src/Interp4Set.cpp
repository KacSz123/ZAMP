#include <iostream>
#include "Interp4Set.hh"
#include "MobileObj.hh"

using std::cout;
using std::endl;


extern "C" {
 Interp4Command* CreateCmd(void);
  const char* GetCmdName() { return "Set"; }
}




/*!
 * \brief
 *
 *
 */
Interp4Command* CreateCmd(void)
{
  return Interp4Set::CreateCmd();
}


/*!
 *
 */
Interp4Set::Interp4Set():   _ObjName("No"), _Coord_X(0),_Coord_Y(0), _Coord_Z(0), _Angle_OX(0),_Angle_OY(0),_Angle_OZ(0)
{


}


/*!
 *
 */
void Interp4Set::PrintCmd() const
{
  /*
   *  Tu trzeba napisać odpowiednio zmodyfikować kod poniżej.
   */
  cout << GetCmdName()<<" " << _ObjName<<" " << _Coord_X << " " <<  _Coord_Y<< " "<<  _Coord_Z<< " ";
  cout << _Angle_OX<<" "<< _Angle_OY <<" " << _Angle_OZ<<endl;
}


/*!
 *
 */
const char* Interp4Set::GetCmdName() const
{
  return ::GetCmdName();
}


/*!
 *
 */
std::string Interp4Set::GetObjName() const
{
  return _ObjName;
}


/*!
 *
 */
bool Interp4Set::ExecCmd( MobileObj  *pMobObj, AccessControl *pAC) const
{
  Vector3D tmpVec;
  tmpVec[0]=_Coord_X;
  tmpVec[1]=_Coord_Y;
  tmpVec[2]=_Coord_Z;

  pMobObj->SetPosition_m(tmpVec);

  
  pMobObj->SetAng_Roll_deg(_Angle_OX);  
  pMobObj->SetAng_Pitch_deg(_Angle_OY);  
  pMobObj->SetAng_Yaw_deg(_Angle_OZ);
  return true;
}


/*!
 *
 */
bool Interp4Set::ReadParams(std::istream& Strm_CmdsList)
{
  /*
   *  Tu trzeba napisać odpowiedni kod.
   */
   Strm_CmdsList>>_ObjName>>_Coord_X>>_Coord_Y>>_Coord_Z>>_Angle_OX>>_Angle_OY>>_Angle_OZ;
  return true;
}


/*!
 *
 */
Interp4Command* Interp4Set::CreateCmd()
{
  return new Interp4Set();
}


/*!
 *
 */
void Interp4Set::PrintSyntax() const
{
  cout << "   Set  NazwaObiektu  wsp_x wsp_y kat_OX kat_oY kat_OZ" << endl;
}
