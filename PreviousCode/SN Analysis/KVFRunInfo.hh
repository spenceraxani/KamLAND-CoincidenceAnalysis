#ifndef __KVFRunInfo_HH
#define __KVFRunInfo_HH

#include <vector>
#include <map>
#include <string>
using namespace std;

class KVFRunInfo {
public:
  ~KVFRunInfo(void){}
  static bool ReadTable(const char* filename);
  static bool ReadRunInfoTable(const char* filename);
  static bool ReadSourceTable(const char* filename);
  static bool ReadSingleTable(const char* filename);
  static bool ReadC11Table(const char* filename);

  static int     NofRun(void){return (int)_run.size();}
  static int     RunNumber(int i){if(i<0 || i>=(int)_run.size()) return -1; else return _run[i];}
  static bool    HasReactorInfo(int run_number);
  static int     Grade(int run_number);
  static int     UnixTime(int run_number);
  static double  RunTimeAsHours(int run_number);
  static string  Comment(int run_number);
  static int     VersionOfKamLAND(int run_number);

  static int     Prompt(int run_number);
  static int     Delay(int run_number);
  static int     Prescale(int run_number);
  static double     PromptDelaydT(int run_number);
  static double  PrescaleFraction(int run_number);
  static string  RunType(int run_number);
  static int     GMTStartTime(int run_number);
  static int     JSTStartTime(int run_number);

  static bool    HasSourceInfo(int run_number);
  static int     SourceID(int run_number);
  static string  SourceName(int run_number);
  static double  SourcePosition(int run_number);

  static bool    HasSingleInfo(int run_number);
  static double  SingleRunTime(int run_number);
  static double  SingleAvailableTime(int run_number);
  static double  SingleRatio(int run_number);
  static double  SingleLiveTime(int run_number);
  static double  SingleVetoTime(int run_number);
  static double  SingleDeadTime(int run_number);
  static int     SingleNofAllMuon(int run_number);
  static int     SingleNofAliveMuon(int run_number);
  static int     SingleNofAllpps(int run_number);
  static int     SingleNofAlivepps(int run_number);
  static int     SingleRunGrade(int run_number);

  static bool    HasC11Info(int run_number);
  static double  C11RunTime(int run_number);
  static double  C11LiveTime(int run_number);
  static double  C11Ratio(int run_number);

private:
  KVFRunInfo(void){}
  static vector<int>   _run;
  static map<int, int> _has_reactor_info;
  static map<int, int> _grade;
  static map<int, int> _unixtime;
  static map<int, string> _comment;
  static map<int, double> _runtime_hours;

  static vector<int> _run_runinfotable;
  static map<int, int>  _unixstarttime;
  static map<int, int>  _prompt_nsum;
  static map<int, int>  _delay_nsum;
  static map<int, double>  _promptdelay_dt;
  static map<int, int>  _prescale_nsum;
  static map<int, double>  _prescale_fraction;
  static map<int, string>  _run_type;

  static vector<int> _run_source;
  static map<int, int> _source_ID;
  static map<int, double> _source_position;

  static vector<int>      _single_run;
  static map<int, double> _single_run_time;
  static map<int, double> _single_available_time;
  static map<int, double> _single_ratio;
  static map<int, double> _single_live_time;
  static map<int, double> _single_veto_time;
  static map<int, double> _single_dead_time;
  static map<int, int>    _single_nof_all_muon;
  static map<int, int>    _single_nof_alive_muon;
  static map<int, int>    _single_nof_all_pps;
  static map<int, int>    _single_nof_alive_pps;
  static map<int, int>    _single_run_grade;

  static vector<int>      _C11_run;
  static map<int, double> _C11_run_time;
  static map<int, double> _C11_live_time;
  static map<int, double> _C11_ratio;
};
#endif
