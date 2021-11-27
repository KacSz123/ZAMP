#include "Scene.hh"

void Scene::PrintMobileObjectList()
{
    cout<<endl<<"Obiekty na scenie:\n";
    for(MobileObjectList::iterator i = MObjList.begin(); i!=MObjList.end(); ++i)
     {
        cout<<"\t->\t"<<i->first<<":\t";
        cout<<"Pos"<<i->second->GetPositoin_m();
        cout<<"\tRPY("<<i->second->GetAng_Roll_deg()<<","
        <<i->second->GetAng_Pitch_deg()<<","<<i->second->GetAng_Yaw_deg()<<")\n";
     }   

    cout<<endl<<endl;
}

bool Scene::IfMobileObjectExists(string Name)
{
    auto It = MObjList.find(Name);

    if (It == MObjList.end())
        return false;
    else    
        return true;
}


std::shared_ptr<MobileObj> Scene::FindMobileObject(std::string Name)
{
    auto It = MObjList.find(Name);

    if(It == MObjList.end())
        return nullptr;
    else
        return It->second; 
        

}