#include <testlib/testlib_test.h>
#include <vcl_iostream.h>
#include <vpl/vpl.h>
#include <depth_map/depth_map_scene.h>
#include <depth_map/depth_map_region_sptr.h>
#include <vsl/vsl_binary_io.h>
#include <bbas/volm/volm_tile.h>
#include <bbas/volm/volm_geo_index.h>
#include <bbas/volm/volm_io.h>
#include <volm/volm_category_io.h>
#include <bkml/bkml_parser.h>
#include <vcl_where_root_dir.h>
#if 0 // used in commented-out part
#include <vul/vul_file.h>
#endif

#include <bsol/bsol_algs.h>
static void test_load_conf_tag_file(vcl_string xml_file)
{
  depth_map_scene_sptr dms = new depth_map_scene;
  float floor_height = 4.5f;
  vcl_string world_region, query_name;
  unsigned img_ni, img_nj;
  bool success = volm_io::read_conf_query_tags(xml_file, floor_height, dms, world_region, img_ni, img_nj, query_name);
  //bool parser_success = volm_io::read_conf_query_tags(xml_file, floor_height, dms, world_region, img_ni, img_nj, query_name);
  TEST("parse LCM matcher tag file",success,true);

  vcl_cout << "Tag content output: " << vcl_endl;
  vcl_cout << "  region: " << world_region;
  vcl_cout << "  queryname: " << query_name << vcl_endl;
  unsigned n_region = dms->scene_regions().size();
  for (unsigned i = 0; i < n_region; i++)
  {
    depth_map_region_sptr region_sptr = dms->scene_regions()[i];
    // obtain the image pixel tag from polygon
    vgl_polygon<double> poly = bsol_algs::vgl_from_poly(region_sptr->region_2d());
    vgl_point_2d<double> pixel = poly[0][0];
    vcl_cout << "  name: " << region_sptr->name()
             << ", mindist: " << region_sptr->min_depth()
             << ", maxdist: " << region_sptr->max_depth()
             << ", height: " << region_sptr->height()
             << ", reference: " << region_sptr->is_ref()
             << ", point: (" << vcl_fixed << pixel.x() << ", " << vcl_fixed << pixel.y() << ")"
             << ", land: " << volm_osm_category_io::volm_land_table[region_sptr->land_id()].name_
             << vcl_endl;
  }
}

static void test_io()
{
  // test the volm_fallback_category
  vcl_map<unsigned char, vcl_vector<unsigned char> >::iterator mit = volm_fallback_label::fallback_id.begin();
  vcl_map<unsigned char, vcl_vector<float> >::iterator mit_w = volm_fallback_label::fallback_weight.begin();

  for (; mit != volm_fallback_label::fallback_id.end(); ++mit) {
    vcl_cout << (int)mit->first << '(' << volm_osm_category_io::volm_land_table[mit->first].name_ << ") ---> ";
    volm_fallback_label::print_id(mit->first);
    //for (vcl_vector<unsigned char>::iterator vit = mit->second.begin(); vit != mit->second.end(); ++vit)
    //  vcl_cout << volm_label_table::land_string(*vit) << ", ";
    vcl_cout << vcl_setw(10) << vcl_setfill(' ') << " ------ ";
    volm_fallback_label::print_wgt(mit->first);
    for (vcl_vector<unsigned char>::iterator vit = mit->second.begin(); vit != mit->second.end(); ++vit)
      vcl_cout << vcl_setprecision(3) << (int)*vit << ' ';
    vcl_cout << '\n' << vcl_endl;
    ++mit_w;
  }

  TEST("number of fallback_label and weight", volm_fallback_label::fallback_id.size(), volm_fallback_label::fallback_weight.size());

  vcl_cout << " ------------ TEST parse LCM matcher tag xml file -------------- " << vcl_endl;
  vcl_string xml_file = vcl_string(VCL_SOURCE_ROOT_DIR) + "/contrib/brl/bbas/volm/tests/test.xml";
  test_load_conf_tag_file(xml_file);
}

TESTMAIN(test_io);
