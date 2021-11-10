#ifndef SET4LIBINTERFACES_HH
#define SET4LIBINTERFACES_HH

#include "LibInterface.hh"

#include <memory>

using namespace std;

/*!
*   \file Plik zawiera definicję klasy Set4LibInterfaces
*
*
*/


/*!
*
*   \brief Typ, ktory definiuje liste polecen(wtyczek).
*
*   Polecenia definiowane sa przez int. wskaznik i nazwe
*/
typedef map<std::string, std::shared_ptr<LibInterface>> LibMap;


/*!
*
*   \brief Definicja klasy Set4LibInterfaces
*
*   Klasa modeluje obsługę wtyczek do sterowania robotem. Komendy (wtyczki)
*   reprezentowane są przez int. wskaźnik(shared_ptr) oraz nazwę (string) 
*/
class Set4LibInterfaces{
public:


/*!
* \brief Konstruktor bezparametryczny
* Dodaje 4 wtyczki do listy polecen
*/
Set4LibInterfaces();


/*!
* \brief Destruktor
*/
~ Set4LibInterfaces(){};

/*!
*   \brief Funkcja wyszukujaca z listy polecenie i zwraca jej iterator
*   \param[in] - path sciezka do biblioteki dynamicznej
*/
LibMap::iterator FindLibrary(string LibPath);


/*!
* \brief Funkcja zwraca kolejny iterator
*/
LibMap::iterator GetEndMap();

/*!
*\brief Funkcja wczytujaca wskazana biblioteke
*
* \param[in] - LibName - nazwa polecenia(wtyczki)
*/
void LoadLibrary(string LibName);


private:

/*!
*\brief lista polecen
*/
LibMap _MapLib;
};





#endif