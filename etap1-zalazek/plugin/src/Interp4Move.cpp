#include <iostream>
#include "Interp4Move.hh"
#include "MobileObj.hh"

using std::cout;
using std::endl;


extern "C" {
 Interp4Command* CreateCmd(void);
  const char* GetCmdName() { return "Move"; }
}




Interp4Command* CreateCmd(void)
{
  return Interp4Move::CreateCmd();
}


/*!
 *
 */
Interp4Move::Interp4Move(): _Speed_mmS(0)
{}


/*!
 *
 */
void Interp4Move::PrintCmd() const
{
  /*
   *  Tu trzeba napisać odpowiednio zmodyfikować kod poniżej.
   */
  cout << GetCmdName() << " "<<_ObjName<< " "<< _Speed_mmS  << " "<<_Path_mm<< " "<<endl;
}


/*!
 *
 */
const char* Interp4Move::GetCmdName() const
{
  return ::GetCmdName();
}
/*!
 *
 */
std::string Interp4Move::GetObjName() const
{
  return _ObjName;
}

/*!
 *
 */
bool Interp4Move::ExecCmd( MobileObj  *pMobObj,  AccessControl *pAC) const
{
int direction = _Speed_mmS > 0 ? 1 : -1;
  int iterations = std::floor(_Path_mm/_Speed_mmS);
  cout<<"MOVE: ITER: "<<iterations<<endl;
  for (int i = 0; i < iterations; ++i)
  {
    pAC->LockAccess();

    Vector3D position = pMobObj->GetPositoin_m();
    //double angle = pMobObj->GetAng_Roll_deg();

    position[0] +=_Speed_mmS * direction * (cos(M_PI * pMobObj->GetAng_Yaw_deg()/180));
    position[1] += _Speed_mmS* direction * sin(M_PI * pMobObj->GetAng_Yaw_deg()/180);
    //position[2] += _Speed_mmS* direction *  (cos(M_PI * pMobObj->GetAng_Yaw_deg()/180));
    pMobObj->SetPosition_m(position);

    

    pAC->MarkChange();
    pAC->UnlockAccess();
    usleep(100000);
  }
  
  return true;
}


/*!
 *
 */
bool Interp4Move::ReadParams(std::istream& Strm_CmdsList)
{
  /*
   *  Tu trzeba napisać odpowiedni kod.
   */
   Strm_CmdsList>>_ObjName>>_Speed_mmS>>_Path_mm;

  return true;
}


/*!
 *
 */
Interp4Command* Interp4Move::CreateCmd()
{
  return new Interp4Move();
}


/*!
 *
 */
void Interp4Move::PrintSyntax() const
{
  cout << "   Move  NazwaObiektu  Szybkosc[m/s]  DlugoscDrogi[m]" << endl;
}
