// R Interface file
// See https://teuder.github.io/rcpp4everyone_en/140_dataframe.html
// [[Rcpp::plugins("cpp17")]]
#include <Rcpp.h>
#include <defines.h>
#include <block.h>
#include <machine.h>
#include <program.h>
#include <point.h>
#include <iostream>

using namespace Rcpp;
using namespace std;


class CNCpp {
public:
  CNCpp(string ini_file) {
    _machine = machine_new(ini_file.c_str());
  }
  
  ~CNCpp() {
    if (_prog)
      program_free(_prog);
    if (_machine)
      machine_free(_machine);
  }
  
  void load(string program_file) {
    _prog = program_new(program_file.c_str());
    program_parse(_prog, _machine);
  }
  
  void print() {
    program_print(_prog, stdout);
  }
  
  string version() {
    return VERSION;
  }
  
  void reset() {
    program_reset(_prog);
    _n.erase(_n.begin(), _n.end());
    _type.erase(_type.begin(), _type.end());
    _t_time.erase(_t_time.begin(), _t_time.end());
    _time.erase(_time.begin(), _time.end());
    _lambda.erase(_lambda.begin(), _lambda.end());
    _speed.erase(_speed.begin(), _speed.end());
    _acc.erase(_acc.begin(), _acc.end());
    _x.erase(_x.begin(), _x.end());
    _y.erase(_y.begin(), _y.end());
    _z.erase(_z.begin(), _z.end());
  }
  
  DataFrame simulate() {
    reset();
    data_t dt = machine_tq(_machine);
    size_t i = 0;
    block_t *b = NULL;
    point_t *pt = NULL;
    data_t lambda, speed, acc, t = 0, tt = 0;
    
    while ((b = program_next(_prog)) != NULL) {
      if (block_type(b) == RAPID || block_type(b) == NO_MOTION) {
        continue;
      }
      t = 0;
      while (t < block_dt(b) + dt) {
        pt = block_interpolate_t(b, t, &lambda, &speed, &acc);
        _n.push_back(block_n(b));
        _type.push_back(block_type(b));
        _time.push_back(t);
        _t_time.push_back(tt);
        _lambda.push_back(lambda);
        _speed.push_back(speed);
        _acc.push_back(acc);
        _x.push_back(point_x(pt));
        _y.push_back(point_y(pt));
        _z.push_back(point_z(pt));
        t += dt;
        tt += dt;
      }
    }
    
    DataFrame df = DataFrame::create(
      _["n"] = _n,
      _["type"] = _type,
      _["time"] = _time,
      _["t_time"] = _t_time,
      _["lambda"] = _lambda,
      _["speed"] = _speed,
      _["acc"] = _acc,
      _["x"] = _x,
      _["y"] = _y,
      _["z"] = _z
    );
    
    return df;
  }

  
public:
  List _blocks;
  machine_t *_machine;
  program_t *_prog;
  NumericVector _n, _type, _t_time, _time, _lambda, _speed, _acc, _x, _y, _z;
};

void finalizer(CNCpp *cncpp) {
  // Rprintf("Deleting instance of CNCpp\n");
}


RCPP_MODULE(cncpp) {
  class_<CNCpp>("CNCpp")
  .constructor<string>()
  .method("load", &CNCpp::load, "Load a program file")
  .method("simulate", &CNCpp::simulate, "Simulate the program")
  .method("print", &CNCpp::print, "Print the program")
  .method("reset", &CNCpp::reset, "Reset the program to first block")
  .method("version", &CNCpp::version, "Get the version of the library")
  .field_readonly("n", &CNCpp::_n)
  .field_readonly("t_time", &CNCpp::_t_time)
  .field_readonly("time", &CNCpp::_time)
  .field_readonly("lambda", &CNCpp::_lambda)
  .field_readonly("speed", &CNCpp::_speed)
  .field_readonly("acc", &CNCpp::_acc)
  .field_readonly("x", &CNCpp::_x)
  .field_readonly("y", &CNCpp::_y)
  .field_readonly("z", &CNCpp::_z)
  .finalizer(&finalizer)
  ;
}