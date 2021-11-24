#include "Set4LibInterfaces.hh"

// konstruktor zaladowanie 4 polecen

Set4LibInterfaces::Set4LibInterfaces()
{
    LoadLibrary("libs/libInterp4Set.so");
    LoadLibrary("libs/libInterp4Pause.so");
    LoadLibrary("libs/libInterp4Rotate.so");
    LoadLibrary("libs/libInterp4Move.so");
}

LibMap::iterator Set4LibInterfaces::FindLibrary(string LibName)
{
    return _MapLib.find(LibName);
}

LibMap::iterator Set4LibInterfaces::GetEndMap()
{
    return _MapLib.end();
}


void Set4LibInterfaces::LoadLibrary(string LibPath)
{
    auto pLib = make_shared<LibInterface>(LibPath);
    _MapLib.insert({pLib->name, pLib});
}
