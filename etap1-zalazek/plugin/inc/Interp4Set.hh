#ifndef  COMMAND4MOVE_HH
#define  COMMAND4MOVE_HH

#ifndef __GNUG__
# pragma interface
# pragma implementation
#endif

#include "Interp4Command.hh"
#include<sstream>
#include <string>
using namespace std;
/*!
 * \file
 * \brief Definicja klasy Interp4Set
 *
 * Plik zawiera definicję klasy Interp4Set , ktora 
 * definiuje jedno z polecen dostepnych do kontroli robota
 */

/*!
 * \brief Modeluje polecenie dla robota mobilnego, które wymusza ustawienie robota w okreslonej pozycji
 *
 *  Klasa modeluje jego orientacje w przestrzeni, a takze polozenie
 */
class Interp4Set: public Interp4Command {
  /*
   *  Tu należy zdefiniować pola, które są niezbędne
   *  do przechowywania wartości parametrów danego polecenia.
   *  Ponieżej zdefiniowane jest tylko jedno pole jako przykład.
   */
  std::string _ObjName;

  /*!
   * \brief Zmienna opisująca kat robota wzgledem osi X
   */
  double  _Coord_X;

  /*!
   * \brief Zmienna opisująca kat robota wzgledem osi Y
   */
  double  _Coord_Y;

  //double  _Coord_Z;

  /*!
   * \brief Zmienna opisująca polozenie robota wzdluz osi X
   */
  double  _Angle_OX;

    /*!
   * \brief Zmienna opisująca polozenie robota wzdluz osi Y
   */
  double  _Angle_OY;

  /*!
   * \brief Zmienna opisująca polozenie robota wzdluz osi Z
   */
  double  _Angle_OZ;
 public:
  /*!
   * \brief
   */
  Interp4Set();  
  /*!
   * \brief Wyświetla postać bieżącego polecenia (nazwę oraz wartości parametrów)
   */
  virtual void PrintCmd() const;
  /*!
   * \brief Wyświetla składnię polecenia
   */
  virtual void PrintSyntax() const;
  /*!
   * \brief Wyświetla nazwę polecenia
   */
  virtual const char* GetCmdName() const;
  /*!
   * \brief Wyświetla nazwę polecenia
   */
  virtual std::string GetObjName() const;
  /*!
   * \brief Wykonuje polecenie oraz wizualizuje jego realizację
   */
  virtual bool ExecCmd( MobileObj  *pMobObj, int Socket) const;
  /*!
   * \brief Czyta wartości parametrów danego polecenia
   */
  virtual bool ReadParams(std::istream& Strm_CmdsList);
  /*!
   * \brief Wyświetla wartości wczytanych parametrów
   */
  virtual void PrintParams() {}
  /*!
   * \brief
   *
   *  Ta metoda nie musi być zdefiniowna w klasie bazowej.
   */
  static Interp4Command* CreateCmd();
 };

#endif
