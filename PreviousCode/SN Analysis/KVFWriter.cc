#include "KVFWriter.hh"
#include <stdio.h>

KVFWriter :: KVFWriter(string name) : KVFBase() 
{
  _fd = 0;
  if( open(name) == false ){
    printf("ERROR: KVFWriter :: KVFWriter(string)\n");
    printf("       Cannot open file: %s\n", name.c_str());
    abort();
  }

  setName(name);
}

KVFWriter :: ~KVFWriter(void)
{
  if(_fd!=0) KVFWriter :: close();
}

bool KVFWriter :: open(string name)
{
  _fd = new ofstream(name.c_str());
  return (_fd->is_open());
}

bool KVFWriter :: bad(void)
{
  if(_fd==0) return false;
  return _fd->bad();
}

void KVFWriter :: flush(void)
{
  _fd->flush();
}

void KVFWriter :: close(void)
{
  if(_fd==0) return;
  _fd->flush();
  _fd->close();
  delete _fd;
  _fd=0;
}

int KVFWriter :: size(void)
{
  return (int)(_fd->tellp());
}

bool KVFWriter :: write(KVFVirtualEvent* event)
{
  if( _buf_size == event->getBufSize() ){}
  else if(_buf_size > event->getBufSize()){
    _buf_size = event->getBufSize();
  } 
  else {
    delete [] _buf;
    _buf_size = event->getBufSize();
    _buf = new char[_buf_size];
  }
  event->storeTo(_buf);
  _fd->write( (const char*)_buf, _buf_size );
  return true;
}
