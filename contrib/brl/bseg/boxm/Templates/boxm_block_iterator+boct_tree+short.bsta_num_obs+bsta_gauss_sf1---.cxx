#include <bsta/bsta_attributes.h>
#include <bsta/bsta_gauss_sf1.h>
#include <boxm/boxm_scene.txx>

typedef bsta_num_obs<bsta_gauss_sf1> gauss_type;
typedef boct_tree<short,gauss_type> tree_type;

BOXM_BLOCK_ITERATOR_INSTANTIATE(tree_type);
