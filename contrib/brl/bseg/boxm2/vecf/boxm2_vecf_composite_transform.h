#ifndef boxm2_vecf_composite_transform_h_included_
#define boxm2_vecf_composite_transform_h_included_

#include <vcl_vector.h>
#include <boxm2/boxm2_scene.h>
#include <boxm2/boxm2_block.h>
#include <boxm2/boxm2_data_traits.h>

#include "boxm2_vecf_vector_field.h"

class boxm2_vecf_composite_transform : public boxm2_vecf_vector_field_base
{
  public:
    boxm2_vecf_composite_transform(vcl_vector<boxm2_vecf_vector_field_base_sptr> xforms);

    bool compute_forward_transform(boxm2_scene_sptr source, 
                                   boxm2_block_id const& blk_id,
                                   const boxm2_data_traits<BOXM2_POINT>::datatype *source_pts,
                                   boxm2_data_traits<BOXM2_POINT>::datatype *target_pts);
    
    //: write the locations of the cooresponding source points to target's BOXM2_POINT data
    bool compute_inverse_transform(boxm2_scene_sptr target,
                                   boxm2_block_id const& blk_id,
                                   const boxm2_data_traits<BOXM2_POINT>::datatype *target_pts,
                                   boxm2_data_traits<BOXM2_POINT>::datatype *source_pts);

  private:
    vcl_vector<boxm2_vecf_vector_field_base_sptr> xforms_;

};

#endif
