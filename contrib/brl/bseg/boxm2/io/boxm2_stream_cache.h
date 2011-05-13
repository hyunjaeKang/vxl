#ifndef boxm2_stream_cache_h_
#define boxm2_stream_cache_h_
//:
// \file
#include <boxm2/boxm2_scene.h>
#include <boxm2/boxm2_block.h>
#include <boxm2/boxm2_data.h>
#include <boxm2/basic/boxm2_block_id.h>

#include <vcl_fstream.h>

#include <vbl/vbl_ref_count.h>
#include <vbl/vbl_smart_ptr.h>  

class boxm2_stream_cache_helper : public vbl_ref_count 
{
  public:
    boxm2_stream_cache_helper() : index_(-1), buf_(0) {}
    ~boxm2_stream_cache_helper();

    bool open_file(vcl_string filename);
    void read(unsigned long size, boxm2_block_id id); 
    void close_file();

    //: return num cells on the buf
    int num_cells(vcl_size_t cell_size); 

    //: return the byte buffer that contains ith cell, i is with respect to the global file
    char *get_cell(int i, vcl_size_t cell_size, boxm2_block_id id);  
 
    int index_;  // index of the data point at the beginning of buf_   
    vcl_ifstream ifs_;
    boxm2_data_base *buf_;
};

typedef vbl_smart_ptr<boxm2_stream_cache_helper> boxm2_stream_cache_helper_sptr;

class boxm2_stream_cache_datatype_helper : public vbl_ref_count
{
  public:
    boxm2_stream_cache_datatype_helper() : current_index_(-1), cell_cnt_(-1) {}

    int current_index_;        // global index in the file
    boxm2_block_id current_block_;
    
    //: the size of datatype that is stored in each cell
    vcl_size_t cell_size_;     
    
    //: the size of chunks that will be read from the file, computed based on cell size and available memory
    unsigned long buf_size_;   

    //: number of cells in the data files, computed based on file size and cell size
    int cell_cnt_;
};

typedef vbl_smart_ptr<boxm2_stream_cache_datatype_helper> boxm2_stream_cache_datatype_helper_sptr;

class boxm2_stream_cache_destroyer;

class boxm2_stream_cache: public vbl_ref_count
{
  public:
    
    //: Use this instead of constructor
    static boxm2_stream_cache* instance();
    static bool         exists() { return (boxm2_stream_cache::instance_!=0); }

    //: create function used instead of constructor
    static void create(boxm2_scene_sptr scene, const vcl_vector<vcl_string>& data_types, const vcl_vector<vcl_string>& identifier_list, float num_giga);

    //: the destroyer instance to make sure memory is deallocated when the program exits
    static boxm2_stream_cache_destroyer destroyer_;  // its not a pointer so C++ will make sure that it's descructor will be called
    friend class boxm2_stream_cache_destroyer;

    //: return the next cells in the streams of each data block given by the identifier list for this datatype, pass index if available to check synchronization
    template <boxm2_data_type T, class datatype> vcl_vector<datatype> get_next(boxm2_block_id id, int index = -1);

  protected:

    //: hidden constructor (singleton class)
    boxm2_stream_cache(boxm2_scene_sptr scene, 
      const vcl_vector<vcl_string>& data_types, const vcl_vector<vcl_string>& identifier_list, float num_giga = 1.0f);

    //: hidden destructor (singleton class)
    ~boxm2_stream_cache();

    boxm2_stream_cache_datatype_helper_sptr get_helper(vcl_string& data_type);

    template <boxm2_data_type T> bool open_streams(vcl_string data_type, boxm2_stream_cache_datatype_helper_sptr h);
    
    //: singleton instance of boxm2_stream_cache
    static boxm2_stream_cache* instance_; 

    //: boxm2_scene needs to be around to get path of the files
    boxm2_scene_sptr scene_;

    vcl_vector<vcl_string> identifier_list_;

    //: available RAM size in bytes
    unsigned long mem_size_;

    //: map to store various info about each datatype
    vcl_map<vcl_string, boxm2_stream_cache_datatype_helper_sptr > data_types_;

    //: for each data type, there is a list for each identifier 
    vcl_map<vcl_string, vcl_vector<boxm2_stream_cache_helper_sptr> > data_streams_;
};

typedef vbl_smart_ptr<boxm2_stream_cache> boxm2_stream_cache_sptr;


//: Binary write boxm2_cache  to stream
void vsl_b_write(vsl_b_ostream& os, boxm2_stream_cache const& scene);
void vsl_b_write(vsl_b_ostream& os, const boxm2_stream_cache* &p);
void vsl_b_write(vsl_b_ostream& os, boxm2_stream_cache_sptr& sptr); 
void vsl_b_write(vsl_b_ostream& os, boxm2_stream_cache_sptr const& sptr);

//: Binary load boxm2_cache  from stream.
void vsl_b_read(vsl_b_istream& is, boxm2_stream_cache &scene);
void vsl_b_read(vsl_b_istream& is, boxm2_stream_cache* p);
void vsl_b_read(vsl_b_istream& is, boxm2_stream_cache_sptr& sptr);
void vsl_b_read(vsl_b_istream& is, boxm2_stream_cache_sptr const& sptr);

//: create another class whose sole purpose is to destroy the singleton instance
class boxm2_stream_cache_destroyer {
public:
  boxm2_stream_cache_destroyer(boxm2_stream_cache* s = 0);
  ~boxm2_stream_cache_destroyer();

  void set_singleton(boxm2_stream_cache* s);
private:
  boxm2_stream_cache* s_;
};

#endif //boxm2_stream_cache_h_