#include <boct/boct_tree_cell_reader.txx>
#include <boxm2/sample/boxm_rt_sample.h>

typedef boxm_rt_sample<float> sample;
BOCT_TREE_CELL_READER_INSTANTIATE(short,sample);
