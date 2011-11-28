#ifndef boxm2_multi_update_cell_h_
#define boxm2_multi_update_cell_h_
//:
// \file
// \brief This class does the cumulative seg len and cumulative observation on the GPU.

#include <boxm2_multi_cache.h>
#include <boxm2/boxm2_scene.h>
#include <boxm2/ocl/boxm2_opencl_cache.h>
#include <vil/vil_image_view.h>
#include <vpgl/vpgl_perspective_camera.h>
#include <bocl/bocl_device.h>
#include <bocl/bocl_kernel.h>
#include <brdb/brdb_value.h>

//: boxm2_multi_cache - example realization of abstract cache class
class boxm2_multi_update_cell
{
  public:

    //three separate sub procedures (three separate map reduce tasks)
    static float update_cells(boxm2_multi_cache& cache,
                              const vil_image_view<float>& img,
                              vpgl_camera_double_sptr cam,
                              vcl_map<bocl_device*, float*>& vis_map,
                              vcl_map<bocl_device*, float*>& pre_map,
                              float*                         norm_image );

  private:

    //renders single image
    static float calc_beta_scene( boxm2_scene_sptr          scene,
                                  bocl_device_sptr          device,
                                  boxm2_opencl_cache*       opencl_cache,
                                  cl_command_queue &        queue,
                                  vpgl_camera_double_sptr & cam,
                                  bocl_mem_sptr &           vis_image,
                                  bocl_mem_sptr &           pre_image,
                                  bocl_mem_sptr &           norm_image,
                                  bocl_mem_sptr &           img_dim,
                                  vcl_string                data_type,
                                  bocl_kernel*              kernel,
                                  vcl_size_t *              lthreads,
                                  unsigned                  cl_ni,
                                  unsigned                  cl_nj,
                                  int                       apptypesize );

    static float calc_beta_reduce( boxm2_multi_cache& mcache,
                                   vcl_vector<boxm2_opencl_cache*>& ocl_caches );


    //map keeps track of all kernels compiled and cached
    static vcl_map<vcl_string, vcl_vector<bocl_kernel*> > kernels_;

    //compile kernels and cache
    static vcl_vector<bocl_kernel*>& get_kernels(bocl_device_sptr device, vcl_string opts);
};

#endif