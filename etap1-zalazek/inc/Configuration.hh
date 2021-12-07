#ifndef CONFIGURATION_HH
#define CONFIGURATION_HH

#include<vector>

#include "MobileObj.hh"
#include "Scene.hh"
using namespace std;


typedef std::vector<std::string> LibraryList;


/*! \brief Definicja klasy Configuration
 *
 *  Modeluje klase Configuration, pzrechowuje liste obiektow i Wtyczek
 */
class Configuration {
  

/*! \brief Vector string
 * 
 * Vector string - lista nazw dostepnych wtyczek
 * 
 */
LibraryList _LibList;

/*! \brief Vector MobileObject
 * 
 * Lista dostepnycg obiektow na scenie
 * 
 */
MobileObjectList _MObjList;


public:



/*! \brief Dodaje obiekt mobilny do listy
 *  
 * Dodaje obiekt mobilny do listy i umozliwia wykonywanie na nim polecen
 * Przypisuje przekazane na wejściu atrybuty do powstalego obiektu
 * 
 * \param[in] Name - strin - nazwa nieistniejacego obiektu
 * \param[in] Shift - vector3D współrzędne położenia
 * \param[in] Scale - vector3D - Miary skali obiektu
 * \param[in] Rotation - vector3D wspolrzedne katow obrotu
 * \param[in] Trans - wspolrzedne pzesuniecia
 * \param[in] RGB - string opisujacy kolor obiektu
 */
void AddMobileObject(const std::string &Name,const Vector3D &Shift,  const Vector3D &Scale,
const Vector3D &Rotation,const Vector3D &Trans,  const std::string &RGB );

/*! \brief Dodaje nazwe wtyczki do listy komend
 *  
 * Dodaje nazwe wtyczki do listy komend metoda push_back
 * 
 * \param[in] Name - string nazwa wtyczki
 */
void AddLibraryName(const std::string &LibraryName){_LibList.push_back(LibraryName);};


/*! \brief Zwraca listę wtyczek
 *  
 * Zwraca listę wtyczek w postaci vectora nazwa (string)
 * 
 * \param[out] _LibList - vector obiektow string
 */
LibraryList GetLibraryList() {return _LibList;};


/*! \brief Zwraca listę obiektów
 *  
 * Zwraca listę obiektow wczytanych z pliku
 * 
 * \param[out] _MObjList - lista obiektow MobileObj
 * */
MobileObjectList GetMobileObjList(){return _MObjList;};


/*! \brief Wypisuje wszystkie wczytane obiekty
 * 
 * Wypisuje na standardowym wyjściu obiekty
 * 
 * */
void PrintMobileObjectList()
{
    cout<<endl<<"Obiekty na scenie:\n";
    for(MobileObjectList::iterator i = _MObjList.begin(); i!=_MObjList.end(); ++i)
     {
        cout<<"\t->\t"<<i->first<<":\t";
        cout<<"Pos"<<i->second->GetPositoin_m();
        cout<<"\tRPY("<<i->second->GetAng_Roll_deg()<<","
        <<i->second->GetAng_Pitch_deg()<<","<<i->second->GetAng_Yaw_deg()<<")\n";
     }   

    cout<<endl<<endl;
}

};


#endif
