// This is brl/bpro/core/vpgl_pro/processes/vpgl_load_affine_camera_process.cxx
#include <bprb/bprb_func_process.h>
//:
// \file

#include <bprb/bprb_parameters.h>
#include <vcl_iostream.h>
#include <vpgl/vpgl_camera.h>
#include <vpgl/vpgl_affine_camera.h>

//: initialization
bool vpgl_load_affine_camera_process_cons(bprb_func_process& pro)
{
  //this process takes one input: the filename
  bool ok=false;
  vcl_vector<vcl_string> input_types;
  input_types.push_back("vcl_string"); 
  input_types.push_back("double"); // view distance 
  input_types.push_back("double"); // viewing direction (x)
  input_types.push_back("double"); // viewing direction (y)
  input_types.push_back("double"); // viewing direction (z)
  ok = pro.set_input_types(input_types);
  if (!ok) return ok;

  // set default values for new parameter view direction to keep backwards-compatibility
  // the view direction will be flipped to make the dot product with the input direction positive
  // this is needed due to the abiguity in the viewing direction of an affine camera  as defined by the projection matrix alone
  brdb_value_sptr default_view_x = new brdb_value_t<double>(0.0);
  pro.set_input(2, default_view_x);
  brdb_value_sptr default_view_y = new brdb_value_t<double>(0.0);
  pro.set_input(3, default_view_y);
  brdb_value_sptr default_view_z = new brdb_value_t<double>(-1.0);
  pro.set_input(4, default_view_z);

  vcl_vector<vcl_string> output_types;
  output_types.push_back("vpgl_camera_double_sptr");  // label image
  ok = pro.set_output_types(output_types);
  if (!ok) return ok;
  
  return true;

}

//: Execute the process
bool vpgl_load_affine_camera_process(bprb_func_process& pro)
{
  if (pro.n_inputs() != 5) {
    vcl_cout << "vpgl_load_affine_camera_process: The input number should be 2" << vcl_endl;
    return false;
  }
  // get the inputs
  vcl_string camera_filename = pro.get_input<vcl_string>(0);
  double dist = pro.get_input<double>(1);
  double look_x = pro.get_input<double>(2);
  double look_y = pro.get_input<double>(3);
  double look_z = pro.get_input<double>(4);
  vcl_ifstream ifs(camera_filename.c_str());
  if (!ifs.is_open()) {
    vcl_cerr << "Failed to open file " << camera_filename << '\n';
    return false;
  }
  vnl_matrix_fixed<double,3,4> affine_matrix;
  ifs >> affine_matrix;
  ifs.close();
  //vpgl_camera_double_sptr procam = new vpgl_affine_camera<double>(affine_matrix);
  vpgl_affine_camera<double>* procam = new vpgl_affine_camera<double>(affine_matrix);
  procam->set_viewing_distance(dist);
  procam->orient_ray_direction(vgl_vector_3d<double>(look_x, look_y, look_z));
  pro.set_output_val<vpgl_camera_double_sptr>(0, procam);

  return true;
}

