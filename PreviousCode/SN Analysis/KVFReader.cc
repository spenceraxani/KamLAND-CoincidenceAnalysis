#include "KVFReader.hh"
#include "KVFHeader.hh"
#include <stdio.h>

KVFReader :: KVFReader(string name) : KVFBase() 
{
  _couldRead = false;
  _fd = 0;
  if( open(name) == false ){
    printf("ERROR: KVFReader :: KVFReader(string)\n");
    printf("       Cannot open file: %s\n", name.c_str());
    abort();
  }

  setName(name);

  _event_offset = 0;
}

KVFReader :: ~KVFReader(void)
{
  if(_fd!=0) KVFReader :: close();
}

bool KVFReader :: open(string name)
{
  _fd = new ifstream(name.c_str());

  return (_fd->is_open());
}

bool KVFReader :: bad(void)
{
  if(_fd==0) return false;
  return _fd->bad();
}

void KVFReader :: close(void)
{
  if(_fd==0) return;
  _fd->close();
  delete _fd;
  _fd=0;
}

int KVFReader :: offset(void)
{
  return (int)(_fd->tellg());
}

int KVFReader :: seek(int n, int buf_size)
{
  int last_offset = (int)_fd->tellg();
  _fd->seekg( n*buf_size, ios::cur);
  int current_offset = (int)_fd->tellg();
  return (int) (current_offset - last_offset)/buf_size;
}

void KVFReader :: manualSeek(int n)
{
  _fd->seekg( n, ios::beg);
}

bool KVFReader :: readHeader(KVFVirtualEvent* event)
{
  event->Header = new KVFHeader;

  _buf = new char[256];
  _fd->read( _buf, 256 );
  event->Header->restoreFrom( (const char*) _buf);
  _couldRead = !(_fd->eof());
  if(!(_couldRead)){
    _fd->clear();
    return false;
  }

  return true;
}

bool KVFReader :: read(KVFVirtualEvent* event)
{
  if(event->isHeader()==true && event->Header==0){
    _event_offset = (int)_fd->tellg();
    _fd->seekg(0, ios::beg);
    if(readHeader(event)==false) return false;    
    if(_event_offset < event->Header->Offset){
      _event_offset += event->Header->Offset;
    }
    _fd->seekg(_event_offset, ios::beg);
  }

  if( _buf_size == event->getBufSize() ){}
  else if(_buf_size > event->getBufSize()){
    _buf_size = event->getBufSize();
  } 
  else {
    delete [] _buf;
    _buf_size = event->getBufSize();
    _buf = new char[_buf_size];
  }
  _fd->read( _buf, _buf_size );
  event->restoreFrom( (const char*) _buf);
  _couldRead = !(_fd->eof());
  if(!(_couldRead))_fd->clear();
  return _couldRead;
}

bool KVFReader :: begin(KVFVirtualEvent* event)
{
  _fd->seekg(_event_offset, ios::beg);
  return read(event);  
}

bool KVFReader :: next(KVFVirtualEvent* event)
{
  return read(event);
}

bool KVFReader :: previous(KVFVirtualEvent* event)
{
  if( KVFReader :: seek(-2, event->getBufSize()) == -2 ) return read(event);
  else {
    _fd->clear();
    return false;
  }
}

bool KVFReader :: end(KVFVirtualEvent* event)
{
  _fd->seekg(0, ios::end);
  if( KVFReader :: seek(-1, event->getBufSize()) == -1 ) return read(event);
  else return false;
}
