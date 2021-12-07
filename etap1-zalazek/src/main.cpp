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

void InitObj(int Socket4Sending, Scene *Sc)
{
  auto ObScptr = Sc->GetPtrs();

  for (auto obj_ptr : ObScptr)
  {

    auto Tmpobj = obj_ptr.get();
    std::string msg = "AddObj " + Tmpobj->GetStateDesc();
    Send(Socket4Sending, msg.c_str());

    Sc->MarkChange();
  }
  usleep(2000000);
}
int main(int argc, char **argv)
{

  Scene *ProgramScene;
  Sender *ProgramSender;
  Configuration ProgramConfig;
  Set4LibInterfaces LibraryList;
  if (argc != 2)
  {
    cout << endl
         << "Niepoprawna liczba argumentów" << endl;
    return 1;
  }

  // Wczytanie konfiguracji z pliku XML
  if (!ReadFile("config/config.xml", ProgramConfig))
  {
    cout << endl
         << "Wczytywanie pliku konfiguracji config/config.xml nie powiodło się" << endl;
    return 2;
  }

  auto ConfigLibraryList = ProgramConfig.GetLibraryList();
  ;

  for (size_t i; i < ConfigLibraryList.size(); i++)
  {
    LibraryList.LoadLibrary(ConfigLibraryList.at(i));
  }

  auto ProgramObjList = ProgramConfig.GetMobileObjList();

  ProgramScene = new Scene(ProgramObjList);
  ProgramScene->PrintMobileObjectList();

  std::istringstream InStream;
  // Czytanie pliku preprocesorem do strumienia
  ExecPreprocesor("opis_dzialan.cmd", InStream);
  cout << endl
       << "Cały plik:" << endl
       << InStream.str() << endl;

  int Socket4Sending;
  ProgramSender = new Sender(Socket4Sending, ProgramScene);

  if (!OpenConnection(Socket4Sending))
  {
    cout << "Nie mozna sie polaczyc\n";
    return 1;
  }

  Send(Socket4Sending, "Clear\n");
  InitObj(Socket4Sending, ProgramScene);

  std::string ProgCmdName, ProgObjName;
  while (InStream >> ProgCmdName)
  {
    std::vector<std::thread *> ListOfThreads;
    ////komendy rownolegle -- na watkach
    if ((ProgCmdName == "Begin_Parallel_Actions"))
    {
      cout << "komenda rownolegla\n";
       //nastepne wczytanie aby wczytac nazwe komendy

      while (!(ProgCmdName == "End_Parallel_Actions"))
      {
        InStream >> ProgCmdName;
        // InStream>>ProgCmdName;
        if (ProgCmdName == "End_Parallel_Actions")
        {
          break;
        }
        //InStream >> ProgObjName;
        LibMap::iterator cmd_iterator = LibraryList.FindLibrary(ProgCmdName);
        if (cmd_iterator == LibraryList.GetEndMap())
        {
          cerr << "Komenda o nazwie '" << ProgCmdName << "' nie istnieje" << endl;
          // return false;
        }
        Interp4Command *pCommand = cmd_iterator->second->pCreateCmd();

        if (!pCommand->ReadParams(InStream))
        {
          cerr << "Czytanie parametrów komendy '" << ProgCmdName << "' nie powiodło się" << endl;
          return false;
        }
        ProgObjName = pCommand->GetObjName();
        //cout<<endl<<endl<<"!!debug!!"<<object_name<<endl<<endl;
        if (!ProgramScene->IfMobileObjectExists(ProgObjName))
        {
          cerr << "Obiekt o nazwie '" << ProgObjName << "' nie istnieje" << endl;
          return false;
        }

        auto object_ptr(ProgramScene->FindMobileObject(ProgObjName));

        pCommand->PrintCmd();

        pCommand->ExecCmd(object_ptr.get(), 0);
      }
    }



    ///////////komendy nie rownolegle/////////////
    else
    {
      cout << "komendy nie równolegle\n";
      // InStream>>ProgObjName;
      LibMap::iterator cmd_iterator = LibraryList.FindLibrary(ProgCmdName);
      if (cmd_iterator == LibraryList.GetEndMap())
      {
        cerr << "Komenda o nazwie '" << ProgCmdName << "' nie istnieje" << endl;
        // return false;
      }
      Interp4Command *pCommand = cmd_iterator->second->pCreateCmd();

      if (!pCommand->ReadParams(InStream))
      {
        cerr << "Czytanie parametrów komendy '" << ProgCmdName << "' nie powiodło się" << endl;
        return false;
      }
      ProgObjName = pCommand->GetObjName();
      //cout<<endl<<endl<<"!!debug!!"<<object_name<<endl<<endl;
      if (!ProgramScene->IfMobileObjectExists(ProgObjName))
      {
        cerr << "Obiekt o nazwie '" << ProgObjName << "' nie istnieje" << endl;
        return false;
      }

      auto object_ptr(ProgramScene->FindMobileObject(ProgObjName));

      pCommand->PrintCmd();

      pCommand->ExecCmd(object_ptr.get(), 0);
    }

  } //koniec glownego while

  //if(false== ProgramSender->open_)
  /*Configuration Config;           // Obiekt konfiguracji, konfiguracja zostanie wczytana po odczytaniu pliku XML
  // Lista bibliotek odczytana z pliku konfiguracyjnego
  MobileObjectList MobileObjList; // Lista obiektów mobilnych odczytana z pliku konfiguracyjnego
   // Lista (zestaw) wczytanych bibliotek
  Scene ProgramScene;             // Scena, lista obiektów mobilnych



 

  int Socket4Sending;
  if (!OpenConnection(Socket4Sending))
  {
    cout << 1;
    return 1;
  }

  Sender ClientSender(Socket4Sending, &ProgramScene);

  thread T4S(Fun_CommunicationThread, &ClientSender);

  //thread   Thread4Sending(Fun_CommunicationThread,&ClientSender);

  istringstream iStrm; // strumień danych wejściowych komend

  // Wczytanie konfiguracji z pliku XML
  if (!ReadFile("config/config.xml", Config))
  {
    cout << endl
         << "Wczytywanie pliku konfiguracji config/config.xml nie powiodło się" << endl;
    return 2;
  }

  Config.PrintMobileObjectList();
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

  cout << "Konfiguracja:" << endl;

  auto ObScptr = ProgramScene.GetPtrs();

  for (auto obj_ptr : ObScptr)
  {

    auto Tmpobj = obj_ptr.get();
    std::string msg = "AddObj " + Tmpobj->GetStateDesc();
    Send(Socket4Sending, msg.c_str());

    ProgramScene.MarkChange();

    
    if (argc != 2)
    {
      cout << endl
           << "Niepoprawna liczba argumentów" << endl;
      return 1;
    }

    usleep(200000);
  }

  // Czytanie pliku preprocesorem do strumienia
  ExecPreprocesor("opis_dzialan.cmd", iStrm);
  cout << endl
       << "Cały plik:" << endl
       << iStrm.str() << endl;

  // Parsowanie pliku na komendy z parametrami
  if (!ExecActions(iStrm, LibraryList, ProgramScene))
    return 3;
  


  cout << "Close\n"
       << endl; // To tylko, aby pokazac wysylana instrukcje

  //zamykanie
  Send(Socket4Sending, "Clear\n");
  Send(Socket4Sending, "Close\n");
  ClientSender.CancelCountinueLooping();
  T4S.join();
  close(Socket4Sending);*/
  //zamykanie
  Send(Socket4Sending, "Clear\n");
  Send(Socket4Sending, "Close\n");
  ProgramSender->CancelCountinueLooping();
  //T4S.join();
  close(Socket4Sending);
}
