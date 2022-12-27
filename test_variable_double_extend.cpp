#include "hdf5.h"
#include <string>
#include <vector>
#include "vec.h"
#include <iostream>
#include <algorithm>

class WriteString
{
    public:
        WriteString(hid_t dset_id, hid_t dtype_id, hid_t dspace_id, hid_t memspace_id):
            _dset_id(dset_id), _dtype_id(dtype_id), _dspace_id(dspace_id), _memspace_id(memspace_id), _last_pos(0)
        {}

        void operator()(const std::vector<std::vector<double>>::value_type& v){
            hsize_t count[1] = {1};
            hsize_t offset[1] = {_last_pos++};

            hsize_t newdims[1] = {_last_pos};
            H5Dset_extent(_dset_id, newdims);

            _dspace_id = H5Dget_space(_dset_id);
            H5Sselect_hyperslab(_dspace_id, H5S_SELECT_SET, offset, NULL, count, NULL);

            _memspace_id = H5Dget_space(_dset_id);
            hsize_t mem_offset[1] = {0};
            hsize_t mem_count[1] = {1};
            H5Sselect_hyperslab(_memspace_id, H5S_SELECT_SET, mem_offset, NULL, mem_count, NULL);

            double* warr = new double[v.size()];
            std::copy(v.begin(), v.end(), &warr[0]);
            
            hvl_t vw_data[1];
            vw_data[0].len = v.size();
            vw_data[0].p = &warr[0];

            H5Dwrite(_dset_id, _dtype_id, _memspace_id, _dspace_id, H5P_DEFAULT, &vw_data[0]);
        }


    private:
        hid_t _dset_id;
        hid_t _dtype_id;
        hid_t _dspace_id;
        hid_t _memspace_id;
        hsize_t _last_pos;
 
};


void write_string_vector(const std::vector<std::vector<double>>& vec){
    hid_t file_id = H5Fcreate("extend_file_double", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    hsize_t dims[1] = {0};
    hsize_t max_dims[1] = {H5S_UNLIMITED};
    hid_t dspace_id = H5Screate_simple(1, dims, max_dims);
    hid_t memspace_id = H5Screate_simple(1, dims, max_dims);


    hid_t dtype_id = H5Tvlen_create(H5T_NATIVE_DOUBLE);

    hid_t pid = H5Pcreate(H5P_DATASET_CREATE);
    hsize_t chunk_dims[1] = {1};
    H5Pset_chunk(pid, 1, chunk_dims);
    
    hid_t dset_id = H5Dcreate(file_id, "doubles", dtype_id, dspace_id, H5P_DEFAULT, pid, H5P_DEFAULT);

    auto ws = WriteString(dset_id, dtype_id, dspace_id, memspace_id);
    std::for_each(vec.begin(), vec.end(), ws);

    H5Dclose(dset_id);
    H5Tclose(dtype_id);
    H5Sclose(memspace_id);
    H5Sclose(dspace_id);
    H5Fclose(file_id);
}



int main(){

    std::vector<std::vector<double>> vvec;
    for(size_t i = 0; i < 10; ++i){
        vvec.push_back(std::vector<double>());
        for(size_t j = 0; j < i + 1; ++j){
            vvec[i].push_back(j);
        }
    }
    
    write_string_vector(vvec);
    return 0;
}
























