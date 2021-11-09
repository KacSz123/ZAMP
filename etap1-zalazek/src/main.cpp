#include <iostream>
#include <dlfcn.h>
#include <cassert>

#include <cstdio>
#include <sstream>
#include "Interp4Command.hh"
#include "MobileObj.hh"

using namespace std;

#define LINE_SIZE 500

bool ExecPreprocesor(const char *FileName, istringstream &IStrm4Cmds)
{
  string Cmd4Preproc = "cpp -p ";
  char Line[LINE_SIZE];
  ostringstream OTmpStrm;

  Cmd4Preproc += FileName;
  FILE *pProc = popen(Cmd4Preproc.c_str(), "r");

  if (!pProc)
    return false;

  while (fgets(Line, LINE_SIZE, pProc))
  {
    OTmpStrm << Line;
  }

  IStrm4Cmds.str(OTmpStrm.str());

  return pclose(pProc) == 0;
}

int main(int argc, char **argv)
{
  void *pLibHnd_Move = dlopen("libInterp4Move.so", RTLD_LAZY);
  void *pLibHnd_Rotate = dlopen("libInterp4Rotate.so", RTLD_LAZY);
  void *pLibHnd_Pause = dlopen("libInterp4Pause.so", RTLD_LAZY);
  void *pLibHnd_Set = dlopen("libInterp4Set.so", RTLD_LAZY);

  Interp4Command *(*pCreateCmd_Move)(void);
  Interp4Command *(*pCreateCmd_Rotate)(void);
  Interp4Command *(*pCreateCmd_Pause)(void);
  Interp4Command *(*pCreateCmd_Set)(void);

  void *pFun;
  void *pFun2;
  void *pFun3;
  void *pFun4;
  /**/

  if (argc < 2)
  {
    cout << endl
         << "Zbyt mało argumentow" << endl
         << endl;
    return 1;
  }
  istringstream str;

  ExecPreprocesor(argv[1], str);
  cout << str.str() << endl;
/***/



  if (!pLibHnd_Move)
  {
    cerr << "!!! Brak biblioteki: Interp4Move.so" << endl;
    return 1;
  }


  if (!pLibHnd_Rotate)
  {
    cerr << "!!! Brak biblioteki: Interp4Rotate.so" << endl;
    return 1;
  }


  if (!pLibHnd_Pause)
  {
    cerr << "!!! Brak biblioteki: Interp4Pause.so" << endl;
    return 1;
  }


  if (!pLibHnd_Set)
  {
    cerr << "!!! Brak biblioteki: Interp4Set.so" << endl;
    return 1;
  }


/////
  pFun2 = dlsym(pLibHnd_Rotate, "CreateCmd");

  if (!pFun2)
  {
    cerr << "!!! Nie znaleziono funkcji CreateCmd dla Rotate" << endl;
    return 1;
  }

  pFun = dlsym(pLibHnd_Move, "CreateCmd");

  if (!pFun)
  {
    cerr << "!!! Nie znaleziono funkcji CreateCmd dla Move" << endl;
    return 1;
  }


  pFun3 = dlsym(pLibHnd_Pause, "CreateCmd");

  if (!pFun3)
  {
    cerr << "!!! Nie znaleziono funkcji CreateCmd dla Pause" << endl;
    return 1;
  }

  pFun4 = dlsym(pLibHnd_Set, "CreateCmd");

  if (!pFun4)
  {
    cerr << "!!! Nie znaleziono funkcji CreateCmd dla Set" << endl;
    return 1;
  }


/////


  pCreateCmd_Move = *reinterpret_cast<Interp4Command *(**)(void)>(&pFun);
  pCreateCmd_Rotate = *reinterpret_cast<Interp4Command *(**)(void)>(&pFun2);
  pCreateCmd_Pause = *reinterpret_cast<Interp4Command *(**)(void)>(&pFun3);
  pCreateCmd_Set = *reinterpret_cast<Interp4Command *(**)(void)>(&pFun4);
////

  Interp4Command *pCmd = pCreateCmd_Move();
  Interp4Command *pCmd2 = pCreateCmd_Rotate();
  Interp4Command *pCmd3 = pCreateCmd_Pause();
  Interp4Command *pCmd4 = pCreateCmd_Set();

/////

  cout << endl;
  cout << pCmd->GetCmdName() << endl;
  cout << endl;
  pCmd->PrintSyntax();
  cout << endl;
  pCmd->PrintCmd();
  cout << endl;

  cout << endl;
  cout << pCmd2->GetCmdName() << endl;
  cout << endl;
  pCmd2->PrintSyntax();
  cout << endl;
  pCmd2->PrintCmd();
  cout << endl;

  cout << endl;
  cout << pCmd3->GetCmdName() << endl;
  cout << endl;
  pCmd3->PrintSyntax();
  cout << endl;
  pCmd3->PrintCmd();
  cout << endl;

  cout << endl;
  cout << pCmd4->GetCmdName() << endl;
  cout << endl;
  pCmd4->PrintSyntax();
  cout << endl;
  pCmd4->PrintCmd();
  cout << endl;
///
  delete pCmd;
  delete pCmd2;
  delete pCmd3;
  delete pCmd4;

  dlclose(pLibHnd_Move);
  dlclose(pLibHnd_Rotate);
  dlclose(pLibHnd_Pause);
  dlclose(pLibHnd_Set);
}
