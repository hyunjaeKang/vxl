#ifndef bstm_data_similarity_traits_h
#define bstm_data_similarity_traits_h

#include <boxm2/boxm2_data_traits.h>
#include <bstm/bstm_data_traits.h>
#include <vcl_iostream.h>
#include <boxm2/cpp/algo/boxm2_mog3_grey_processor.h>

#define SIMILARITY_T 0.0001


template <bstm_data_type TYPE, boxm2_data_type BOXM2_TYPE>
class bstm_similarity_traits;

template<>
class bstm_similarity_traits<BSTM_MOG3_GREY, BOXM2_MOG3_GREY>
{
 public:
  static bool is_similar(bstm_data_traits<BSTM_MOG3_GREY>::datatype app, boxm2_data_traits<BOXM2_MOG3_GREY>::datatype boxm2_app, float p, float boxm2_p )
  {
    double isabel_measure = (bstm_data_traits<BSTM_MOG3_GREY>::processor::expected_color(app) + 1) * p;
    double isabel_measure_boxm2 = (boxm2_data_traits<BOXM2_MOG3_GREY>::processor::expected_color(boxm2_app) + 1) * boxm2_p;

    vcl_cout << "Curr measure: " << isabel_measure << " and next measure: " << isabel_measure_boxm2
             << " verdict: " <<  ( vcl_fabs( isabel_measure - isabel_measure_boxm2) < SIMILARITY_T && (boxm2_p > 0 || p == 0) ) << vcl_endl;

    //hack:  boxm2_p > 0 to make sure empty voxels always lead to time division
    //otherwise it leads to motion artefacts.
    return vcl_fabs( isabel_measure - isabel_measure_boxm2) < SIMILARITY_T && (boxm2_p > 0 || p == 0);
  }
};

template<>
class bstm_similarity_traits<BSTM_MOG6_VIEW, BOXM2_MOG6_VIEW>
{
 public:
  static bool is_similar(bstm_data_traits<BSTM_MOG6_VIEW>::datatype /*app*/,
                         boxm2_data_traits<BOXM2_MOG6_VIEW>::datatype /*boxm2_app*/,
                         float /*p*/, float /*boxm2_p*/ )                           { return true; }
};

template<>
class bstm_similarity_traits<BSTM_MOG6_VIEW_COMPACT, BOXM2_MOG6_VIEW_COMPACT>
{
 public:
  static bool is_similar_asd(bstm_data_traits<BSTM_MOG6_VIEW_COMPACT>::datatype app, boxm2_data_traits<BOXM2_MOG6_VIEW_COMPACT>::datatype boxm2_app, float p, float boxm2_p )
  {
    //all 6 components must be similar
    bool similar = true;
    for (int i = 0; i < 6; ++i)
    {
      double isabel_measure = ( ((float)app[2*i])/255.0 + 1) * p;
      double isabel_measure_boxm2 = ( ((float)boxm2_app[2*i])/255.0 + 1) * boxm2_p;

      //hack:  boxm2_p > 0 to make sure empty voxels always lead to time division
      //otherwise it leads to motion artefacts.
      similar = similar && (vcl_fabs( isabel_measure - isabel_measure_boxm2) < SIMILARITY_T && (boxm2_p > 0 || p == 0));
    }
    return similar;
  }
  static bool is_similar(bstm_data_traits<BSTM_MOG6_VIEW_COMPACT>::datatype app, boxm2_data_traits<BOXM2_MOG6_VIEW_COMPACT>::datatype boxm2_app, float p, float boxm2_p )
  {
    //
    if (vcl_fabs(p - boxm2_p) > 0.2 || (boxm2_p == 0 && p != 0)  ) //if p change is over SIMILARITY_T or the new p is 0 but the current p is not.
      return false;
    else
    {
      //all 8 components must be similar
      float mean_change = 0;
      for (int i = 0; i < 8; ++i)
        mean_change  += vcl_fabs( ((float)app[2*i])/255.0 - ((float)boxm2_app[2*i])/255.0) ;
      mean_change /= 8;

      if (mean_change < 0.5)
        return true;
      else
        return false;
    }
  }
};


#endif
