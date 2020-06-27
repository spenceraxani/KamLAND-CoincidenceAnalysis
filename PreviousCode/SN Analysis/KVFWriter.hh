#ifndef __KVFWriter_HH
#define __KVFWriter_HH

#include "KVFBase.hh"
#include <fstream>
using namespace std;

class KVFWriter : public KVFBase {
public:
  KVFWriter(string name);
  virtual ~KVFWriter();

  bool open(string name);
  bool bad(void);
  void close(void);
  void flush(void);
  int  size(void);
  bool write(KVFVirtualEvent* event);

private:
  ofstream* _fd;
};


#endif
