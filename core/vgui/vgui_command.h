#ifndef vgui_command_h_
#define vgui_command_h_
#ifdef __GNUC__
#pragma interface
#endif
// .NAME vgui_command
// .INCLUDE vgui/vgui_command.h
// .FILE vgui_command.cxx
/*
  fsm@robots.ox.ac.uk \and pcp@robots.ox.ac.uk
  awf renamed derived classes to be consistent with 
  header-file naming convention.
  fsm fixed everything afterwards....
*/
#include <vbl/vbl_ref_count.h>

// this defines the abstract interface to commands.
struct vgui_command : vbl_ref_count
{
  vgui_command();
  virtual ~vgui_command();
  virtual void execute() =0;
};

// an implementation using a C callback function.
struct vgui_command_cfunc : vgui_command
{
  typedef void (*function_pv)(void const*);
  typedef void (*function)();
  function_pv fn_pv;
  function fn;
  void const *data;

  vgui_command_cfunc(function_pv, void const *);
  vgui_command_cfunc(function);
 ~vgui_command_cfunc();
  void execute();
};

// command for toggle buttons
struct vgui_command_toggle : vgui_command
{
  bool state;
  vgui_command_toggle(bool v) : state(v) { }
  ~vgui_command_toggle();
  void execute();
};

// pcp's templated bound member functions. All methods are inline, so 
// we don't need a separate .cxx file.
//
// vgui_command_simple is a convenient way to build vgui_commands from 
// object/method pairs where the method is of the form 
// void receiver::method(void);
template <class receiver>
class vgui_command_simple : public vgui_command {
public:
  typedef void (receiver::* action)();
  
  vgui_command_simple(receiver* o, action m) : obj(o), mem(m) { }
  void execute() { (obj->*mem)(); }
  
  receiver* obj;
  action mem;
};
#define VGUI_COMMAND_SIMPLE_INSTANTIATE(receiver) \
template class vgui_command_simple<receiver >

// similar, but for methods that take a single argument (fsm).
template <class object_t, class data_t>
struct vgui_command_bound_method : vgui_command
{
  typedef void (object_t::*action_t)(data_t);
  
  vgui_command_bound_method(object_t *o, action_t m, data_t d) : obj(o), mem(m), dat(d) { }
  void execute() { (obj->*mem)(dat); }
  
  object_t *obj;
  action_t mem;
  data_t dat;
};
#define VGUI_COMMAND_BOUND_METHOD_INSTANTIATE(O, D) \
template struct vgui_command_bound_method<O, D >

//----------------------------------------------------------------------

#define INSTANTIATE_VGUI_simple_command(receiver) \
VGUI_COMMAND_SIMPLE_INSTANTIATE(receiver) /* backwards compat */

#endif // vgui_command_h_
