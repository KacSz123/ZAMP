#include "LibInterface.hh"


//deklaracja konstruktora
LibInterface::LibInterface(string BibPath)
{
    //otworzenie biblioteki przezwskaznik
    BibHandler=dlopen(BibPath.c_str(), RTLD_LAZY); 

    //niepowodzenie
    if(!BibHandler)
        cerr <<"!!! Blad wczytania biblioteki !!!" << BibPath<<endl;
    else
        cout<< "* Udalo sie znalezc biblioteke * "<< BibPath <<endl;
    
    //Wyszukanie  polecenia
    void *Cmd = dlsym(BibHandler, "CreateCmd");
    if(!Cmd)
        cerr <<"!!! Blad Nie znaleziono CreateCmd !!!" << BibPath<<endl;

    // tworzenie wskaznika na polecenie
    pCreateCmd = *reinterpret_cast<Interp4Command*(*)(void)>(Cmd);
    Interp4Command *InterpCmd = pCreateCmd();
    name=InterpCmd->GetCmdName();
    //usuniecie wskaznika na polecenie
    delete InterpCmd; 
}

