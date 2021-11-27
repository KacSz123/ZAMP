//#include <cassert>
//#include "Interp4Command.hh"
#include "Set4LibInterfaces.hh"
//#include "MobileObj.hh"
//#include <sstream>
#include <cstdio>
//#include "Configuration.hh"
#include "xmlinterp.hh"
#include "Scene.hh"

#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/util/XMLString.hpp>
#include "xmlinterp.hh"
//#include <iostream>
#include <list>

#include "Sender.hh"

#define LINE_SIZE 500

using namespace std;
using namespace xercesc;

/*!
 * \brief Funkcja czyta plik przy wykorzystaniu preprocesora.
 *
 * Funkcja interpretuje plik preprocesorem języka C, a następnie przepisuje go do strumienia.
 * \param[in] FileName - Nazwa pliku do interpretacji.
 * \param[out] IStrm4Cmds - Strumień, do którego zostanie zapisany zinterpretowany plik.
 * \retval true - Interpretacja preprocesorem została udana, a plik został poprawnie zamknięty.
 * \retval false - Funkcja nie została poprawnie wykonana.
 */
bool ExecPreprocesor(const char *FileName, istringstream &IStrm4Cmds)
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

  IStrm4Cmds.str(OTmpStrm.str());

  return pclose(pProc) == 0;
}

/*!
 * \brief Funkcja tworzy serię komend wraz z parametrami na podstawie zadanego strumienia.
 *
 * Funkcja interpretuje ciąg obiektów typu string w strumieniu utworzonym przez funkcję ExecPreprocesor(...) na serię komend wraz z parametrami.  
 * \param[in] iStrm - Strumień utworzony przez funkcję ExecPreprocesor(...) z zapisanym plikiem do przeparsowania na komendy.
 * \param[in] LibraryList - Lista wszystkich komend, potrzebna do sprawdzania poprawności zadanych komend w pliku.
 * \param[in] ProgramScene - Scena programu zawierające listę obiektów mobilnych.
 * \retval true - Utworzenie wszystkich komend zawartych w strumieniu z wartościami przepisanymi z pliku powiodło się.
 * \retval false - Utworzenie komend zawartych w strumieniu z wartościami przepisanymi z pliku nie powiodło się.
 */
bool ExecActions(istringstream &iStrm, Set4LibInterfaces &LibraryList, Scene &ProgramScene)
{
  std::string command_name, object_name;

  cout << "Nazwa_kom\tNazwa_Ob\tparametry\n";

  while (iStrm >> command_name) // sprawdza, czy w strumieniu jest jeszcze jakaś komenda
  {

    // Sprawdzenie, czy komenda o podanej nazwie istnieje
    LibMap::iterator cmd_iterator = LibraryList.FindLibrary(command_name);
    if (cmd_iterator == LibraryList.GetEndMap())
    {
      cerr << "Komenda o nazwie '" << command_name << "' nie istnieje" << endl;
      return false;
    }

    // Tworzy komendę na podstawie funkcji wtyczki
    Interp4Command *pCommand = cmd_iterator->second->pCreateCmd();
    if (!pCommand->ReadParams(iStrm))
    {
      cerr << "Czytanie parametrów komendy '" << command_name << "' nie powiodło się" << endl;
      return false;
    }

     object_name = pCommand->GetObjName();
    //cout<<endl<<endl<<"!!debug!!"<<object_name<<endl<<endl;
    if (!ProgramScene.IfMobileObjectExists(object_name))
    {
      cerr << "Obiekt o nazwie '" << object_name << "' nie istnieje" << endl;
      return false;
    }

    auto object_ptr(ProgramScene.FindMobileObject(object_name));

    
    pCommand->PrintCmd();

    pCommand->ExecCmd(object_ptr.get(), 0);
    
  }
  return true;
}

/*!
 * \brief Czyta z pliku opis poleceń i dodaje je do listy komend,
 * które robot musi wykonać.
 * \param sFileName - (\b we.) nazwa pliku z opisem poleceń.
 * \param rConfig - (\b we.) obiekt konfiguracji XML.
 * \retval true - jeśli wczytanie zostało zrealizowane poprawnie,
 * \retval false - w przeciwnym przypadku.
 */
bool ReadFile(const char *sFileName, Configuration &rConfig)
{
  try
  {
    XMLPlatformUtils::Initialize();
  }
  catch (const XMLException &toCatch)
  {
    char *message = XMLString::transcode(toCatch.getMessage());
    cerr << "Error during initialization! :\n";
    cerr << "Exception message is: \n"
         << message << "\n";
    XMLString::release(&message);
    return 1;
  }

  SAX2XMLReader *pParser = XMLReaderFactory::createXMLReader();

  pParser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);
  pParser->setFeature(XMLUni::fgSAX2CoreValidation, true);
  pParser->setFeature(XMLUni::fgXercesDynamic, false);
  pParser->setFeature(XMLUni::fgXercesSchema, true);
  pParser->setFeature(XMLUni::fgXercesSchemaFullChecking, true);

  pParser->setFeature(XMLUni::fgXercesValidationErrorAsFatal, true);

  DefaultHandler *pHandler = new XMLInterp4Config(rConfig);
  pParser->setContentHandler(pHandler);
  pParser->setErrorHandler(pHandler);

  try
  {

    if (!pParser->loadGrammar("config/config.xsd",
                              xercesc::Grammar::SchemaGrammarType, true))
    {
      cerr << "!!! Plik grammar/actions.xsd, '" << endl
           << "!!! ktory zawiera opis gramatyki, nie moze zostac wczytany."
           << endl;
      return false;
    }
    pParser->setFeature(XMLUni::fgXercesUseCachedGrammarInParse, true);
    pParser->parse(sFileName);
  }
  catch (const XMLException &Exception)
  {
    char *sMessage = XMLString::transcode(Exception.getMessage());
    cerr << "Informacja o wyjatku: \n"
         << "   " << sMessage << "\n";
    XMLString::release(&sMessage);
    return false;
  }
  catch (const SAXParseException &Exception)
  {
    char *sMessage = XMLString::transcode(Exception.getMessage());
    char *sSystemId = xercesc::XMLString::transcode(Exception.getSystemId());

    cerr << "Blad! " << endl
         << "    Plik:  " << sSystemId << endl
         << "   Linia: " << Exception.getLineNumber() << endl
         << " Kolumna: " << Exception.getColumnNumber() << endl
         << " Informacja: " << sMessage
         << endl;

    XMLString::release(&sMessage);
    XMLString::release(&sSystemId);
    return false;
  }
  catch (...)
  {
    cout << "Zgloszony zostal nieoczekiwany wyjatek!\n";
    return false;
  }

  delete pParser;
  delete pHandler;
  return true;  
}

int main(int argc, char **argv)
{
  Configuration Config;          // Obiekt konfiguracji, konfiguracja zostanie wczytana po odczytaniu pliku XML
  LibraryList ConfigLibraryList;     // Lista bibliotek odczytana z pliku konfiguracyjnego
  MobileObjectList MobileObjList;   // Lista obiektów mobilnych odczytana z pliku konfiguracyjnego
  Set4LibInterfaces LibraryList; // Lista (zestaw) wczytanych bibliotek
  Scene ProgramScene;            // Scena, lista obiektów mobilnych

  int Socket4Sending;
   if (!OpenConnection(Socket4Sending)){cout<<1; return 1;}
  
  Sender   ClientSender(Socket4Sending,&ProgramScene);
  
  thread T4S(Fun_CommunicationThread,&ClientSender);
  
  //thread   Thread4Sending(Fun_CommunicationThread,&ClientSender);
  const char *sConfigCmds =
"Clear\n"
"AddObj Name=Podstawa RGB=(20,200,200) Scale=(4,2,1) Shift=(0.5,0,0) RotXYZ_deg=(0,-45,20) Trans_m=(-1,3,0)\n"
"AddObj Name=Podstawa.Ramie1 RGB=(200,0,0) Scale=(3,3,1) Shift=(0.5,0,0) RotXYZ_deg=(0,-45,0) Trans_m=(4,0,0)\n"
"AddObj Name=Podstawa.Ramie1.Ramie2 RGB=(100,200,0) Scale=(2,2,1) Shift=(0.5,0,0) RotXYZ_deg=(0,-45,0) Trans_m=(3,0,0)\n";       


  cout << "Konfiguracja:" << endl;
  cout << sConfigCmds << endl;
  
  Send(Socket4Sending,sConfigCmds);
 

    ProgramScene.MarkChange();
 
  
  usleep(100000);
  if (argc != 2)
  {
    cout << endl<< "Niepoprawna liczba argumentów"<< endl;
    return 1;
  }

  istringstream iStrm; // strumień danych wejściowych komend

  // Wczytanie konfiguracji z pliku XML
  if (!ReadFile("config/config.xml", Config))
  {
    cout << endl<< "Wczytywanie pliku konfiguracji config/config.xml nie powiodło się"<< endl;
    return 2;
  }

  // Ładowanie bibliotek
  ConfigLibraryList = Config.GetLibraryList();
  for (long unsigned int i = 0; i < ConfigLibraryList.size(); i++) // long unsigned int żeby uniknąć ostrzeżenia przy kompilacji
  {
    LibraryList.LoadLibrary(ConfigLibraryList.at(i));
  }

  // Wczytanie obiektów mobilnych z XML
  MobileObjList = Config.GetMobileObjList();
  ProgramScene.LoadMobileObjectsList(MobileObjList);

  ProgramScene.PrintMobileObjectList(); // testowo wyświetla nazwy obiektów

  // Czytanie pliku preprocesorem do strumienia
  ExecPreprocesor("opis_dzialan.cmd", iStrm);
  cout << endl<< "Cały plik:"<< endl<< iStrm.str()<< endl;

  // Parsowanie pliku na komendy z parametrami
 if (!ExecActions(iStrm, LibraryList, ProgramScene))
    return 3;

    
ProgramScene.PrintMobileObjectList(); 
  usleep(3000000);
  
  cout << "Close\n" << endl; // To tylko, aby pokazac wysylana instrukcje
 
 
 //zamykanie
  Send(Socket4Sending,"Clear\n");
  Send(Socket4Sending,"Close\n");
  ClientSender.CancelCountinueLooping();
  T4S.join();
  close(Socket4Sending);
}
