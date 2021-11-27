#include <iostream>
#include "Interp4Rotate.hh"
#include "MobileObj.hh"

using std::cout;
using std::endl;


extern "C" {
 Interp4Command* CreateCmd(void);
  const char* GetCmdName() { return "Rotate"; }
}




/*!
 * \brief
 *
 *
 */
Interp4Command* CreateCmd(void)
{
  return Interp4Rotate::CreateCmd();
}


/*!
 *
 */
Interp4Rotate::Interp4Rotate(): _Angle_deg(0), _Rot_Speed(0)
{}


/*!
 *
 */
void Interp4Rotate::PrintCmd() const
{
  /*
   *  Tu trzeba napisać odpowiednio zmodyfikować kod poniżej.
   */
  cout << GetCmdName()<<" " << _ObjName<<" "<< _Rot_Speed<<" "<< _Axis<<" " << _Angle_deg << endl;
}


/*!
 *
 */
const char* Interp4Rotate::GetCmdName() const
{
  return ::GetCmdName();
}

/*!
 *
 */
std::string Interp4Rotate::GetObjName() const
{
  return _ObjName;
}


/*!
 *
 */
bool Interp4Rotate::ExecCmd( MobileObj  *pMobObj,  int  Socket) const
{

  if     (_Axis=='X')
    pMobObj->SetAng_Roll_deg(pMobObj->GetAng_Roll_deg()+_Angle_deg);   
  else if(_Axis=='Y')
    pMobObj->SetAng_Pitch_deg(pMobObj->GetAng_Pitch_deg()+_Angle_deg);   
  else if(_Axis=='Z')
    pMobObj->SetAng_Yaw_deg(pMobObj->GetAng_Yaw_deg()+_Angle_deg);    


  return true;
}


/*!
 *
 */
bool Interp4Rotate::ReadParams(std::istream& Strm_CmdsList)
{
  /*
   *  Tu trzeba napisać odpowiedni kod.
   */
   Strm_CmdsList>>_ObjName>>_Rot_Speed>>_Axis>>_Angle_deg;
  return true;
}


/*!
 *
 */
Interp4Command* Interp4Rotate::CreateCmd()
{
  return new Interp4Rotate();
}


/*!
 *
 */
void Interp4Rotate::PrintSyntax() const
{
  cout << "   Rotate  NazwaObiektu PredkoscObrotu[stopnie/sek.] Kat[stopnie]" << endl;
}