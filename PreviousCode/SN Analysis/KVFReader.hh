#ifndef __KVFReader_HH
#define __KVFReader_HH

#include "KVFBase.hh"
#include <fstream>
using namespace std;

class KVFReader : public KVFBase {
public:
  KVFReader(string name);
  virtual ~KVFReader();

  bool open(string name);
  bool bad(void);
  void close(void);
  int  offset(void);

  bool readHeader(KVFVirtualEvent* event);

  bool read(KVFVirtualEvent* event);
  bool begin(KVFVirtualEvent* event);
  bool end(KVFVirtualEvent* event);
  bool next(KVFVirtualEvent* event);
  bool previous(KVFVirtualEvent* event);

  bool couldRead(void){return _couldRead;}

  int  seek(int n, int buf_size);

  void manualSeek(int n);

private:
  ifstream* _fd;
  bool _couldRead;

  int _event_offset;
};


#endif
