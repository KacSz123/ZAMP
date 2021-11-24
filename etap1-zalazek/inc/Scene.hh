#ifndef SCENE_HH
#define SCENE_HH
//#include "LibInterface.hh"
#include"MobileObj.hh"
#include "AccessControl.hh"
#include <memory>
#include <map>
#include <string>

using namespace std;



/*! \brief Lista obiektow mobilnych
 *  Lista jest pzrechowywana w postaci par nazwa(string) - i adres(int wskaznik)
 */
typedef std::map<std::string, std::shared_ptr<MobileObj>> MobileObjectList;



/*! \brief Definicja klasy Scene
 *  Modeluje klase Scene, przechowuje luste obiektow na scenei
 */
class Scene: public AccessControl{
private: 

/*! \brief Lista obiektow mobilnych na scenie
 *  
 */
MobileObjectList MObjList;

public:
/*! \brief
 * Konstruktor bezparametryczny wykozrystuje referenchje
 */

Scene(){};

/*! \brief 
 * Dekonstruktor 
 */
~Scene(){};

/*! \brief Ładuje liste obiketow do sceny
 *  
 * Kopiuje przez referencje do listy obiektow mobilnych
 * \param[in] MOL - referencja do listy obiektow
 */
void LoadMobileObjectsList(MobileObjectList &MOL){MObjList=MOL;};


/*! \brief Wypisuje nazwy wszystkich obiektow na scenei
 *  
 */
void PrintMobileObjectList();


/*! \brief Zwraca liste obiektow mobilnych ze sceny
 *  \param[out] MObjList - Lista aktualnych obiektow mobilnych na scenie
 */

MobileObjectList GetMobileObjectList(){return MObjList;};

/*! \brief Przeszukuje liste i zwraca wskaznik na obiekt o zadanej nazwie
 * \param[in] Name - porzadana nazwa obiektu 
 *  \param[out] It - wskaznik na znaleziony obiekt lub nullptr
 */
std::shared_ptr<MobileObj> FindMobileObject(std::string Name);


/*! \brief Sprawdza czy obiekt o zadanej nazwie istnieje w liscie
 *  \param[in] Name - nazwa szukanego obiektu (string)
 */
bool IfMobileObjectExists(string Name);



};

#endif