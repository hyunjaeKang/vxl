#include <boxm/boxm_scene.txx>
#include <boct/boct_tree.h>
#include <boxm/opt/boxm_opt3_sample.h>
#include <boxm/boxm_apm_traits.h>

typedef boct_tree<short,boxm_opt3_sample<BOXM_APM_MOG_GREY> > tree;
BOXM_SCENE_INSTANTIATE(tree);
