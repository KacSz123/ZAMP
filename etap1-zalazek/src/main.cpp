#include <iostream>
#include <dlfcn.h>
#include <cassert>

#include <cstdio>
#include <sstream>
//#include "Interp4Command.hh"
#include "MobileObj.hh"
#include "Set4LibInterfaces.hh"

#define LINE_SIZE 500

using namespace std;

bool ExecPreprocesor(const char *FileName, istringstream &inStr4Cmds)
{
  string Cmd4Preproc = "cpp -P ";
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

  inStr4Cmds.str(OTmpStrm.str());

  return pclose(pProc) == 0;
}

/*!
 * \brief Funkcja tworzy serię komend wraz z parametrami na podstawie zadanego strumienia.
 *
 * Funkcja interpretuje ciąg obiektów typu string w strumieniu utworzonym przez funkcję ExecPreprocesor(...) na serię komend wraz z parametrami.  
 * \param[in] inStr - Strumień utworzony przez funkcję ExecPreprocesor(...) z zapisanym plikiem do przeparsowania na komendy.
 * \param[in] LibList - Lista wszystkich komend, potrzebna do sprawdzania poprawności zadanych komend w pliku.
 * \retval true - Utworzenie wszystkich komend zawartych w strumieniu z wartościami przepisanymi z pliku powiodło się.
 * \retval false - Utworzenie komend zawartych w strumieniu z wartościami przepisanymi z pliku nie powiodło się.
 */
bool ExecActions(istringstream &inStr, Set4LibInterfaces &LibList)
{
  string cmdName;

//dopoki wczytywane jest polecenie
  while (inStr >> cmdName) 
  {
    // Czy istnieje polecenie o takiej nazwie
    LibMap::iterator cmdIter = LibList.FindLibrary(cmdName);
    if (cmdIter == LibList.GetEndMap())
    {
      cerr << "To polecenie nie istnieje:" << cmdName << endl;
      return false;
    }

    // Jesli znajdzie polecenie tworzy instancje wtyczki
    Interp4Command *pCmd = cmdIter->second->pCreateCmd();
    if (!pCmd->ReadParams(inStr))
    {
      cerr << "Zle parametry w poleceniu" << cmdName<< endl;
      return false;
    }
    cout << "Polecenie: ";
    pCmd->PrintCmd();
  }
  return true;
}

int main(int argc, char **argv)
{
  Set4LibInterfaces LibList; // Lista (zestaw) bibliotek
  istringstream inStr;           // strumień danych wejściowych komend

  // Sprawdzanie liczby argumentów
  if (argc != 2)
  {
    cout << endl << "Bledna liczba argumentow"<< endl;
    return 1;
  }
// przekierowanie pliku do strumienia przez preprocesor
  ExecPreprocesor(argv[1], inStr);

  cout << endl<< "Zawartosc pliku:\n"<< inStr.str()<< endl;

  // Odczytanie polecen
  if (!ExecActions(inStr, LibList))
    return 2;

  cout<<endl<<endl;
}










/*
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
  */





/*
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
  */
