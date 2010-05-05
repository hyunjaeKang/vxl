#include <boxm2/boxm_block.txx>
#include <boxm2/boxm_apm_traits.h>
#include <boxm2/sample/boxm_sample_multi_bin.h>
#include <boct/boct_tree.h>

typedef boct_tree<short,boxm_sample_multi_bin<BOXM_APM_MOG_GREY> >  tree_type;
BOXM_BLOCK_INSTANTIATE(tree_type);
