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

/*!
 * \brief Wczytuje i przetwarza plik przez preprocesor
 *
 * Funkcja wczytuje do strumienia plik i przetwarza go przez preprocesor  
 * \param[in] inStr4Cmds - Strumien z poleceniami. Zawiera dane nieprzetworzone, np. komentarze 
 * \param [in] FileName - Nazwa pliku z poleceniami do przetworzenia
 * \retval true - Wczytanie i przetworzenie polecen powiodlo sie
 * \retval false - Wczytanie i przetworzenie polecen nie powiodlo sie
 */
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
 * \brief Interpretuje serie polecen podanych przez strumien
 *
 * Funkcja interpretuje i rozpoznaje ciag polecen dla robota, przetworzonych przez preprocesor  
 * \param[in] inStr - Strumień utworzony i zinterpretowany z pliku przez funkcję ExecPreprocesor() 
 * \param[in] LibList - Lista wszystkich dostepnych polecen
 * \retval true - Wczytanie i utworzenie polecen powiodlo sie
 * \retval false - Wczytanie i utworzenie polecen nie powiodlo sie
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
  return 0;
}



