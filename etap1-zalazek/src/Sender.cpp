#include "Sender.hh"

using std::cerr;
  int Send(int Sk2Server, const char *sMsg)
  {
      ssize_t HowManySent;
      ssize_t HowManyToSend=(ssize_t)strlen(sMsg);

      while((HowManySent=write(Sk2Server, sMsg,HowManyToSend))>0 )
        {
            HowManyToSend-=HowManySent;
            sMsg +=HowManySent;
        }
    if (HowManySent < 0)
    {

        cerr << "!!!! Blad przesylania !!!!!\n\n";
    }
    return 0;

  }

bool OpenConnection(int &rSocket)
{
  struct sockaddr_in  DaneAdSerw;

  bzero((char *)&DaneAdSerw,sizeof(DaneAdSerw));

  DaneAdSerw.sin_family = AF_INET;
  DaneAdSerw.sin_addr.s_addr = inet_addr("127.0.0.1");
  DaneAdSerw.sin_port = htons(PORT);


  rSocket = socket(AF_INET,SOCK_STREAM,0);

  if (rSocket < 0) {
     cerr << "*** Blad otwarcia gniazda." << endl;
     return false;
  }

  if (connect(rSocket,(struct sockaddr*)&DaneAdSerw,sizeof(DaneAdSerw)) < 0)
   {
     cerr << "*** Brak mozliwosci polaczenia do portu: " << PORT << endl;
     return false;
   }
  return true;
}

bool ChangeState(Scene &Scn) 
{/*
  bool Changed;

  while (true) {
    Scn.LockAccess(); // Zamykamy dostęp do sceny, gdy wykonujemy
                            // modyfikacje na obiekcie.
    for (MobileObj &rObj : Scn.Lis) {
       if (!(Changed = rObj.IncStateIndex())) { Scn.UnlockAccess();  return false; }
    }
    Scn.MarkChange();
    Scn.UnlockAccess();
    usleep(300000);
  }*/
  return true;
}

void Fun_CommunicationThread(Sender *pSnd){pSnd->Watching_and_Sending();}

/************************!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*******************/
/************************!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*******************/



  void Sender::Watching_and_Sending() 
   {
     while (ShouldCountinueLooping()) 
     {
       if (!_pScn->IsChanged()) 
       
        {
           usleep(10000);
          // _pScn->MarkChange();
         continue;
        }
       
       _pScn->LockAccess();
       
       

       //Lista obiektow MobileObjectList mapowane w scene.hh
        auto _MobObjs = _pScn->GetPtrs();


 
        for(auto ObjPtr:_MobObjs)
       {
            auto tmpMobObj = ObjPtr.get();

            std::string ObjStatus ="UpdateObj "+ tmpMobObj->GetStateDesc();  
                                     // Ta instrukcja to tylko uproszczony przykład
	        cout << "\n\nWysylanie:"<<ObjStatus;

          Send(_Socket,ObjStatus.c_str()); // Tu musi zostać wywołanie odpowiedniej
          // usleep(10000);
        //_pScn->MarkChange(); 
        
       
                             // metody/funkcji gerującej polecenia dla serwera.
       } 
      _pScn->CancelChange();
      _pScn->UnlockAccess();     
     }
   }

   int Sender:: Send_m(const char *Msg)
   {
     ssize_t MsgLen = (ssize_t)strlen(Msg);
     ssize_t SendMsgLen;

     if(true==this->ifConnected)
     {
       while((SendMsgLen = write(this->_Socket,Msg, MsgLen))>0)
       {
        MsgLen-=SendMsgLen;
        Msg+=SendMsgLen;
       }
       if(SendMsgLen<0)
        std::cerr<<"!!!!!* Blad przesylania *!!!!!!\n";
       else
      std::cerr<<"!!!!!!* Polaczenie nie zostało otwarte *!!!!!!!\n";
     }
     
      
      
      return 0;
   }