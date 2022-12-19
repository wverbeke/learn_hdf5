#include "hdf5.h"
#include <string>
#include <vector>
#include "vec.h"
#include <iostream>


void write_vecs(const std::string& filename, const std::string& dset_name, const std::vector<Vec<double>>& vecs){

    // Make the hdf5 file.
    hid_t file_id = H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    // Each vector has 3 elements.
    hsize_t dims[2] = {1, 3};
    const hsize_t max_dims[2] = {H5S_UNLIMITED, 3};
    const hsize_t chunk_dims[2] = {1, 3};

    hid_t chunk_prop = H5Pcreate(H5P_DATASET_CREATE);
    herr_t status = H5Pset_chunk(chunk_prop, 2, chunk_dims);

    hid_t dspace_id = H5Screate_simple(2, dims, max_dims);
    hid_t dset_id = H5Dcreate(file_id, dset_name.c_str(), H5T_IEEE_F64BE, dspace_id, H5P_DEFAULT, chunk_prop, H5P_DEFAULT);

    hid_t memspace = H5Dget_space(dset_id);

    double write_array[3] = {vecs[0].x(), vecs[0].y(), vecs[0].z()};
    status = H5Dwrite(dset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, write_array);

    hsize_t offset[2] = {0, 0};

    for(auto it = vecs.cbegin() + 1; it != vecs.cend(); ++it){
        auto v = (*it);
        write_array[0] = v.x();
        write_array[1] = v.y();
        write_array[2] = v.z();

        hsize_t offset[2] = {dims[0], 0};
        dims[0] += 1;

        status = H5Dset_extent(dset_id, dims);

        hid_t space = H5Dget_space(dset_id);

        status = H5Sselect_hyperslab(space, H5S_SELECT_SET, offset, NULL, chunk_dims, NULL);
        status = H5Dwrite(dset_id, H5T_NATIVE_DOUBLE, memspace, space, H5P_DEFAULT, write_array);
    }
    
    
    status = H5Dclose(dset_id);
    status = H5Sclose(dspace_id);
    status = H5Fclose(file_id);
}


int main(){

    std::vector<Vec<double>> vecs_to_write;
    for(unsigned i=0; i < 100; ++i){
        vecs_to_write.push_back(Vec<double>{static_cast<double>(i), static_cast<double>(i), static_cast<double>(i)});
    }
    write_vecs("vec_file.h5", "vectors", vecs_to_write);
    
    return 0;
}
