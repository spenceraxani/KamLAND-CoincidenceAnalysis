#ifndef __KVFBase_HH
#define __KVFBase_HH

#include "KVFVirtualEvent.hh"
#include <string>
using namespace std;

class KVFBase {
public:
  KVFBase(){ 
    _buf_size=KVFVirtualEvent::DefaultBufSize; 
    _buf = new char[_buf_size]; 
  }
  virtual ~KVFBase(){ if(_buf!=0) delete [] _buf; }

  string  name(void){return _name;}
  int     getBufSize(void){return _buf_size;}

protected:
  int   setBufSize(int size){ _buf_size = size; return _buf_size;}
  void  setName(string name){ _name = name; }

protected:
  string _name;
  char*  _buf;
  int    _buf_size;
  
};

#endif
