#ifndef NMWR_GB_CONTROLLER_H
#define NMWR_GB_CONTROLLER_H


//----------------------------------------------------------------
//   (c) Guntram Berti, 1997
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


#include <iostream.h>
#include <string> 

#include "IO/mutator.h"

/////////////////////////////////////////////////////////////////
//
//   abstract interface for controlling parameters,
//   e.g. they may be 
//   - read from a file
//   - set interactively via a GUI or a command interpreter
//
/////////////////////////////////////////////////////////////////





// implementation class
class control_device_impl : public controlable {
public:
  virtual void update() = 0;
  virtual void add(std::string const&, Mutator*) = 0;

  virtual void print_values(std::ostream&) const = 0;


  virtual void print_unrecognized(std::ostream&) const = 0;
  virtual void print_unrecognized(std::ostream&, std::string const&) const = 0;

  virtual void attach_to(std::istream& in) = 0;
  virtual control_device_impl* get_sub_device(std::string const& nm) = 0;

  virtual std::string name() const = 0;
  virtual ~control_device_impl() {}
};

// envelope class
class ControlDevice  {
public:
  ControlDevice(control_device_impl* imp = 0) : impl(imp) {}

  void add(std::string const& nm, Mutator* value_ref);
  void add(char        const* nm, Mutator* value_ref);

  void update();
  void print_values      (std::ostream&) const;
  void print_unrecognized(std::ostream&) const;
  void attach_to         (std::istream& in);


  void register_at(ControlDevice&, std::string const& prefix);

  std::string name() const;

  ControlDevice getSubDevice(std::string const& name);
  ControlDevice getSubDevice(char        const* name);
private:
  control_device_impl* impl;
};


template<class T>
inline void RegisterAt(ControlDevice& Ctrl, std::string const& name, T& t)
{ 
  TypedMutator<T>* p = new TypedMutator<T>(t);
  Ctrl.add(name, p); 
}

template<class T>
inline void RegisterAt(ControlDevice& Ctrl, char const*   name, T& t)
{  Ctrl.add(name, new TypedMutator<T>(t)); }



// a simple ControlDevice that reads name-value-pairs from a file
extern ControlDevice GetStreamDevice(istream* in, std::string const& name = "");
//extern ControlDevice GetFileControlDevice(const string& filename, const string& name);
extern ControlDevice GetFileControlDevice(char const*   filename, std::string const& name);

extern ControlDevice GetCommandlineAndFileControlDevice(int argc, char* argv[],
							std::string const& filename, 
							std::string const& name);


extern ControlDevice GetDuplexControlDevice(std::istream     & in2,
					    char        const* filename, 
                                            std::string const& name);
extern ControlDevice GetDuplexControlDevice(std::istream     & in2,
					    std::string const& filename, 
                                            std::string const& name);
#endif
