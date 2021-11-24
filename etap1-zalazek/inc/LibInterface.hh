#ifndef LIBINTERFACE_HH
#define LIBINTERFACE_HH

#include "Interp4Command.hh"

#include <iostream>
#include <map>
#include <string>

#include <dlfcn.h>


using namespace std;

/*!
 * \file
 * \brief Definicja Klasy LibInterface
 *
 * Definicja klasy LibInterfejs, która modeluje interfejs wykorzystujący komendy
 * do obsługi pluginów, sterujących robotem.
 *
 *
 *
 */
class LibInterface{


    public:

    /*!
     *
     *  \brief Nazwa polecenia dla robota
     *  Docelowo są to 4 nazyw: Set, Pasue, Rotate, Move
     *
     *
     */
        string name;



    /*!
     *
     *  \brief Konstruktor parametryczny
     *  tworzy instancje klasy podstawie podanej ścieżki biblioteki
     *
     *  \param[in] BibPath - sciezka do biblioteki dynamicznej
     */
    LibInterface(string BibPath);


    /*!
     *  \brief Destruktor obiektu LibInterface
     *
     *  Niszczy obiekt, uprzednio zamykajac wtyczke
     */
        ~LibInterface(){ dlclose(BibHandler);}

    
    /*!
     *  \brief Wskaźnik na nowa instancje interpretera polecen
     *  
     *
     */
        Interp4Command*(*pCreateCmd)(void);

    private:
    /*!
     *   \brief Wskaźnik na bibliotekę
     *     
     *
     */
 void *BibHandler; 
};
#endif