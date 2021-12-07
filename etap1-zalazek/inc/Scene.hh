#ifndef SCENE_HH
#define SCENE_HH
//#include "LibInterface.hh"
#include"MobileObj.hh"
#include "AccessControl.hh"
#include <memory>
#include <map>
#include <string>
#include<vector>

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

Scene(MobileObjectList ML){MObjList=ML;};

/*! \brief 
 * Dekonstruktor 
 */
~Scene(){ };

/*! \brief Ładuje liste obiketow do sceny
 *  
 * Kopiuje przez referencje do listy obiektow mobilnych
 * \param[in] MOL - referencja do listy obiektow
 */
void LoadMobileObjectsList(MobileObjectList &MOL){MObjList=MOL;};


/*! \brief Wypisuje nazwy wszystkich obiektow na scenei
 *  
 *
 * Wypisuje nazwy wszystkich obiektow na scenie przez stdo
 */
void PrintMobileObjectList();


/*! \brief Zwraca liste obiektow mobilnych ze sceny
 *
 * 
 * Zwraca liste obiektow mobilnych znajdujacych sie na scenie.
 *  \param[out] MObjList - Lista aktualnych obiektow mobilnych na scenie
 */

MobileObjectList GetMobileObjectList(){return MObjList;};

/*! \brief Przeszukuje liste i zwraca wskaznik na obiekt o zadanej nazwie
 * 
 * Przeszukuje liste i zwraca wskaznik na obiekt o zadanej nazwie
 * 
 * \param[in] Name - porzadana nazwa obiektu 
 */
std::shared_ptr<MobileObj> FindMobileObject(std::string Name);


/*! \brief Sprawdza czy obiekt o zadanej nazwie istnieje w liscie 
 * 
 * Sprawdza czy obiekt o zadanej nazwie istnieje w liscie
 * 
 *  \param[in] Name - nazwa szukanego obiektu (string)
 * \retval true - obiekt o podanej nazwie istnieje
 * \retval false - obiekt o podanej nazwie nie istnieje
 */
bool IfMobileObjectExists(string Name);


/*! \brief Zwraca vector wskaźników na obiekty mobilne
 * 
 * Zwraca vector wskaźników na obiekty mobilne dostepne na scenie
 * 
 * \param[out] Ptrs - vector obiektow na scenie
 */
std::vector<std::shared_ptr<MobileObj>> GetPtrs()
{
    std::vector<std::shared_ptr<MobileObj>> Ptrs;
    MobileObjectList::iterator i;
    for (i= MObjList.begin(); i != MObjList.end(); i++)
    {
            Ptrs.push_back(i->second);
    }
    return Ptrs;
}

/*! \brief Zwraca vector stringow
 *
 * Zwraca vector obiektów typu string - nazwy obiektow
  *
  * na scenie
 * 
 * \param[out] TmpVecStr - vector obiektow na scenie
 */
std::vector<std::string> GetNamesList()
{
    std::vector<std::string> TmpVecStr;
    MobileObjectList::iterator i ;
    for(i=MObjList.begin(); i != MObjList.end(); i++)
    {
        TmpVecStr.push_back(i->first);
    }
    return TmpVecStr;
}
/*! \brief Tworzy shared_ptr na MobileObj
 *
 * Tworzy shared_ptr na MobileObj i dodaje do listy
  * na scenie
 * 
 * \param[in] Name - nazwa nowego obiektu [string]
 */
void AddMobileObject(std::string Name)
{
    std::shared_ptr<MobileObj> new_Ptr = std::make_shared<MobileObj>();
    this->MObjList.insert({Name, new_Ptr});
}
};

#endif