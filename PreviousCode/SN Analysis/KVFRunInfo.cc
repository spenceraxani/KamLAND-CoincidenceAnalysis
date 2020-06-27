#include "KVFRunInfo.hh"
#include <fstream>
#include <iostream>
#include <stdlib.h>

/*
  -- Added information from table produced from logger information:  $KAMLAND_CONST_DIR/vf/trigger_conditions.dat
  Run   Unix Start  Time   Pompt   Delay   dT   Prescale  PreFraction  Run Type
  8245  1230686855   90   90  1000   40    1.024      physics

*/

// Mar.11, 2009 by Kyohei Nakajima
//  -- Added on-axis source table:  $KAMLAND_CONST_DIR/vf/source_summary_on_axis.dat
//  Run   sourceID    poistion[m]
//  8433      1          -6.0

// Apr.13, 2009 by Kyohei Nakajima
//  -- Added single live time table:  KAMLAND_CONST_DIR/live-time/livetime_single.dat
// RunNumber  RunTime  AvailableTime  Ratio  LiveTime  VetoTime  DeadTime  NofAllMuon  NofAliveMuon  NofAllpps  NofAlivepps  RunGrade
//   8287     86153.5    86032.7    0.988997  85205.6  947.985   120.957     32721        32408        86031       85221        0

// Mar.7, 2009 by Kyohei Nakajima
//  -- Added C11 live time table:  KAMLAND_CONST_DIR/live-time/livetime_C11_X.Xm.dat
// RunNumber  RunTime  LiveTime   Ratio
//   8287     80753.5  35192.6   0.435803

using namespace std;

map<int, int> KVFRunInfo :: _has_reactor_info;
map<int, int> KVFRunInfo :: _grade;
map<int, int> KVFRunInfo :: _unixtime;
map<int, string> KVFRunInfo :: _comment;
map<int, double> KVFRunInfo :: _runtime_hours;
vector<int> KVFRunInfo :: _run;

/* All this information is taken from Kinoko logger */
map<int, int> KVFRunInfo :: _unixstarttime; 
map<int, int> KVFRunInfo :: _prompt_nsum;
map<int, int> KVFRunInfo :: _delay_nsum;
map<int, int> KVFRunInfo :: _prescale_nsum;
map<int, double> KVFRunInfo :: _promptdelay_dt;
map<int, double> KVFRunInfo :: _prescale_fraction;  // this is given in percentage in the table and logger.  I convert to the more useful fraction.
map<int, string> KVFRunInfo :: _run_type;
vector<int> KVFRunInfo :: _run_runinfotable;

// for source information table
vector<int> KVFRunInfo :: _run_source;
map<int, int> KVFRunInfo :: _source_ID;
map<int, double> KVFRunInfo :: _source_position;

// for single live time table
vector<int>      KVFRunInfo :: _single_run;
map<int, double> KVFRunInfo :: _single_run_time;
map<int, double> KVFRunInfo :: _single_available_time;
map<int, double> KVFRunInfo :: _single_ratio;
map<int, double> KVFRunInfo :: _single_live_time;
map<int, double> KVFRunInfo :: _single_veto_time;
map<int, double> KVFRunInfo :: _single_dead_time;
map<int, int>    KVFRunInfo :: _single_nof_all_muon;
map<int, int>    KVFRunInfo :: _single_nof_alive_muon;
map<int, int>    KVFRunInfo :: _single_nof_all_pps;
map<int, int>    KVFRunInfo :: _single_nof_alive_pps;
map<int, int>    KVFRunInfo :: _single_run_grade;

// for C11 live time
vector<int>      KVFRunInfo :: _C11_run;
map<int, double> KVFRunInfo :: _C11_run_time;
map<int, double> KVFRunInfo :: _C11_live_time;
map<int, double> KVFRunInfo :: _C11_ratio;


bool KVFRunInfo :: ReadRunInfoTable(const char* filename)
{
  ifstream f(filename);
  if(!f){
    cerr<<"ERROR: Cannot open run-info table: "<<filename<<endl;
    return false;
  }
  int run, prompt, delay, prescale, unixtime;
  char runtype[256];
  double prescalefraction,dt;
  while(f>>run>>unixtime>>prompt>>delay>>dt>>prescale>>prescalefraction>>runtype){
    _run_runinfotable.push_back(run);
    _unixstarttime.insert( make_pair((int)run,(int)unixtime) );
    _prompt_nsum.insert( make_pair((int)run,(int)prompt) );
    _delay_nsum.insert( make_pair((int)run,(int)delay) );
    _promptdelay_dt.insert(make_pair((int)run,(double)dt) );
    _prescale_nsum.insert( make_pair((int)run,(int)prescale) );
    _prescale_fraction.insert( make_pair((int)run,(double)(prescalefraction/100.0)) );
    _run_type.insert( make_pair((int)run,(string)runtype) );
  }
  f.close();
  return true;
}

bool KVFRunInfo :: ReadTable(const char* filename)
{
  ifstream f(filename);
  if(!f){
    cerr<<"ERROR: Cannot open run-quality log-file: "<<filename<<endl;
    return false;
  }
  int run, reactor, grade, unixtime;
  char date[1024], comment[4096];
  double runtime;
  while(f>>run>>date>>runtime>>unixtime>>reactor>>grade){
    f.getline(comment, 4096);
    _run.push_back(run);
    _has_reactor_info.insert( make_pair((int)run,(int)reactor) );
    _grade.insert( make_pair((int)run,(int)grade) );
    _unixtime.insert( make_pair((int)run,(int)unixtime) );
    _runtime_hours.insert( make_pair((int)run,(double)runtime) );
    _comment.insert( make_pair((int)run,(string)comment) );
  }
  f.close();
  return true;
}

bool KVFRunInfo :: ReadSourceTable(const char* filename)
{
  ifstream f(filename);
  if(!f){
    cerr<<"ERROR: Cannot open source table: "<<filename<<endl;
    return false;
  }
  int run, ID;
  double position;
  while(f>>run>>ID>>position){
    _run_source.push_back(run);
    _source_ID.insert( make_pair((int)run,(int)ID) );
    _source_position.insert( make_pair((int)run,(double)position) );
  }
  f.close();
  return true;
}

bool KVFRunInfo :: ReadSingleTable(const char* filename)
{
  ifstream f(filename);
  if(!f){
    cerr<<"ERROR: Cannot open single table: "<<filename<<endl;
    return false;
  }

  int run, nof_all_muon, nof_alive_muon, nof_all_pps, nof_alive_pps, run_grade;
  double run_time, available_time, ratio, live_time, veto_time, dead_time;
  
    while(f>>run>>run_time>>available_time>>ratio>>live_time>>veto_time>>dead_time>>nof_all_muon>>nof_alive_muon>>nof_all_pps>>nof_alive_pps>>run_grade) {
      _single_run.push_back(run);
      _single_run_time.insert      (make_pair((int)run, (double)run_time      ));
      _single_available_time.insert(make_pair((int)run, (double)available_time));
      _single_ratio.insert         (make_pair((int)run, (double)ratio         ));
      _single_live_time.insert     (make_pair((int)run, (double)live_time     ));
      _single_veto_time.insert     (make_pair((int)run, (double)veto_time     ));
      _single_dead_time.insert     (make_pair((int)run, (double)dead_time     ));
      _single_nof_all_muon.insert  (make_pair((int)run, (int)nof_all_muon     ));
      _single_nof_alive_muon.insert(make_pair((int)run, (int)nof_alive_muon   ));
      _single_nof_all_pps.insert   (make_pair((int)run, (int)nof_all_pps      ));
      _single_nof_alive_pps.insert (make_pair((int)run, (int)nof_alive_pps    ));
      _single_run_grade.insert     (make_pair((int)run, (int)run_grade        ));
  }
  f.close();
  return true;
}

bool KVFRunInfo :: ReadC11Table(const char* filename)
{
  ifstream f(filename);
  if(!f){
    cerr<<"ERROR: Cannot open C11 table: "<<filename<<endl;
    return false;
  }

  int run;
  double run_time, live_time, ratio;
  
  while(f>>run>>run_time>>live_time>>ratio){
      _C11_run.push_back(run);
      _C11_run_time.insert (make_pair((int)run, (double)run_time ));
      _C11_live_time.insert(make_pair((int)run, (double)live_time));
      _C11_ratio.insert    (make_pair((int)run, (double)ratio    ));
  }
  f.close();
  return true;
}

bool KVFRunInfo :: HasReactorInfo(int run_number)
{
  map<int, int>::iterator p;
  p = _has_reactor_info.find(run_number);
  if( p == _has_reactor_info.end() ) return false;
  //cout<<(p->second)<<endl;
  return (bool)( p->second );
}

int KVFRunInfo :: Grade(int run_number)
{
  map<int, int>::iterator p;
  p = _grade.find(run_number);
  if( p == _grade.end() ) return (-1);
  return ( p->second );
}

int KVFRunInfo :: UnixTime(int run_number)
{
  map<int, int>::iterator p;
  p = _unixtime.find(run_number);
  if( p == _unixtime.end() ) return (0);
  return ( p->second );
}

double KVFRunInfo :: RunTimeAsHours(int run_number)
{
  map<int, double>::iterator p;
  p = _runtime_hours.find(run_number);
  if( p == _runtime_hours.end() ) return 0;
  return ( p->second );
}

string KVFRunInfo :: Comment(int run_number)
{
  map<int, string>::iterator p;
  p = _comment.find(run_number);
  if( p == _comment.end() ) return (string)"";
  return ( p->second );
}

int KVFRunInfo :: VersionOfKamLAND(int run_number)
{
 if(run_number<1896) return 1;
  else if(run_number<2194) return 2;
  else if(run_number<6802){
    if((KVFRunInfo::Grade(run_number)!=6) && (KVFRunInfo::Grade(run_number)!=9)) return 3;
    else return 2;
  }
  else{ // after 1st purification
    if((KVFRunInfo::Grade(run_number)!=6) && (KVFRunInfo::Grade(run_number)!=9)) return 5;
    else return 4;
  }
}


int KVFRunInfo :: Prompt(int run_number){
  map<int, int>::iterator p;
  p = _prompt_nsum.find(run_number);
  if( p == _prompt_nsum.end() ) return 0;
  return ( p->second );
}

int KVFRunInfo :: Delay(int run_number){
  map<int, int>::iterator p;
  p = _delay_nsum.find(run_number);
  if( p == _delay_nsum.end() ) return 0;
  return ( p->second );
}

int KVFRunInfo :: Prescale(int run_number){
  map<int, int>::iterator p;
  p = _prescale_nsum.find(run_number);
  if( p == _prescale_nsum.end() ) return 0;
  return ( p->second );
}

double KVFRunInfo :: PromptDelaydT(int run_number){
  map<int, double>::iterator p;
  p = _promptdelay_dt.find(run_number);
  if( p == _promptdelay_dt.end() ) return 0;
  return ( p->second );
}

double KVFRunInfo :: PrescaleFraction(int run_number)
{
  map<int, double>::iterator p;
  p = _prescale_fraction.find(run_number);
  if( p == _prescale_fraction.end() ) return 0;
  return ( p->second );
}


string KVFRunInfo :: RunType(int run_number)
{
  map<int, string>::iterator p;
  p = _run_type.find(run_number);
  if( p == _run_type.end() ) return (string)"";
  return ( p->second );
}


int KVFRunInfo :: GMTStartTime(int run_number)
{
  map<int, int>::iterator p;
  p = _unixstarttime.find(run_number);
  if( p == _unixstarttime.end() ) return (0);
  return ( p->second - 9*60*60);
}


int KVFRunInfo :: JSTStartTime(int run_number)
{
  map<int, int>::iterator p;
  p = _unixstarttime.find(run_number);
  if( p == _unixstarttime.end() ) return (0);
  return ( p->second );
}

// for source information table
bool KVFRunInfo :: HasSourceInfo(int run_number)
{
  map<int, int>::iterator p;
  p = _source_ID.find(run_number);
  if( p == _source_ID.end() ) return false;
  return true;
}

int KVFRunInfo :: SourceID(int run_number){
  map<int, int>::iterator p;
  p = _source_ID.find(run_number);
  if( p == _source_ID.end() ) return 0;
  return ( p->second );
}

string KVFRunInfo :: SourceName(int run_number)
{
  map<int, int>::iterator p;
  p = _source_ID.find(run_number);
  if( p == _source_ID.end() ) return "";

  int ID = p->second;
  string source_name;
  if (ID == 1) 
    source_name = "Co";
  else if (ID == 2)
    source_name = "Zn";
  else if (ID == 3)
    source_name = "AmBe";
  else if (ID == 4)
    source_name = "Ge";
  else if (ID == 5)
    source_name = "Hg";
  else if (ID == 6)
    source_name = "CoGe";
  else if (ID == 7)
    source_name = "Cs";
  else if (ID == 8)
    source_name = "PoC";
  else if (ID == 9)
    source_name = "Sr";

  return source_name;
}

double KVFRunInfo :: SourcePosition(int run_number)
{
  map<int, double>::iterator p;
  p = _source_position.find(run_number);
  if( p == _source_position.end() ) return 0;
  return ( p->second );
}

// for single live time table
bool KVFRunInfo :: HasSingleInfo(int run_number)
{
  map<int, double>::iterator p;
  p = _single_run_time.find(run_number);
  if( p == _single_run_time.end() ) return false;
  return true;
}

double KVFRunInfo :: SingleRunTime(int run_number)
{
  map<int, double>::iterator p;
  p = _single_run_time.find(run_number);
  if( p == _single_run_time.end() ) return 0;
  return ( p->second );
}

double KVFRunInfo :: SingleAvailableTime(int run_number)
{
  map<int, double>::iterator p;
  p = _single_available_time.find(run_number);
  if (p == _single_ratio.end()) return 0;
  return ( p->second );
}

double KVFRunInfo :: SingleRatio(int run_number)
{
  map<int, double>::iterator p;
  p = _single_ratio.find(run_number);
  if (p == _single_ratio.end()) return 0;
  return ( p->second );
}

double KVFRunInfo :: SingleLiveTime(int run_number)
{
  map<int, double>::iterator p;
  p = _single_live_time.find(run_number);
  if (p == _single_live_time.end()) return 0;
  return ( p->second );
}

double KVFRunInfo :: SingleVetoTime(int run_number)
{
  map<int, double>::iterator p;
  p = _single_veto_time.find(run_number);
  if (p == _single_veto_time.end()) return 0;
  return ( p->second );
}

double KVFRunInfo :: SingleDeadTime(int run_number)
{
  map<int, double>::iterator p;
  p = _single_dead_time.find(run_number);
  if (p == _single_dead_time.end()) return 0;
  return ( p->second );
}

int KVFRunInfo :: SingleNofAllMuon(int run_number)
{
  map<int, int>::iterator p;
  p = _single_nof_all_muon.find(run_number);
  if (p == _single_nof_all_muon.end()) return 0;
  return ( p->second );
}
int KVFRunInfo :: SingleNofAliveMuon(int run_number)
{
  map<int, int>::iterator p;
  p = _single_nof_alive_muon.find(run_number);
  if (p == _single_nof_alive_muon.end()) return 0;
  return ( p->second );
}

int KVFRunInfo :: SingleNofAllpps(int run_number)
{
  map<int, int>::iterator p;
  p = _single_nof_all_pps.find(run_number);
  if (p == _single_nof_all_pps.end()) return 0;
  return ( p->second );
}

int KVFRunInfo :: SingleNofAlivepps(int run_number)
{
  map<int, int>::iterator p;
  p = _single_nof_alive_pps.find(run_number);
  if (p == _single_nof_alive_pps.end()) return 0;
  return ( p->second );
}

int KVFRunInfo :: SingleRunGrade(int run_number)
{
  map<int, int>::iterator p;
  p = _single_run_grade.find(run_number);
  if (p == _single_run_grade.end()) return 0;
  return ( p->second );
}

// for C11 live time table
bool KVFRunInfo :: HasC11Info(int run_number)
{
  map<int, double>::iterator p;
  p = _C11_run_time.find(run_number);
  if( p == _C11_run_time.end() ) return false;
  return true;
}

double KVFRunInfo :: C11RunTime(int run_number)
{
  map<int, double>::iterator p;
  p = _C11_run_time.find(run_number);
  if( p == _C11_run_time.end() ) return 0;
  return ( p->second );
}

double KVFRunInfo :: C11LiveTime(int run_number)
{
  map<int, double>::iterator p;
  p = _C11_live_time.find(run_number);
  if (p == _C11_live_time.end()) return 0;
  return ( p->second );
}

double KVFRunInfo :: C11Ratio(int run_number)
{
  map<int, double>::iterator p;
  p = _C11_ratio.find(run_number);
  if (p == _C11_ratio.end()) return 0;
  return ( p->second );
}
