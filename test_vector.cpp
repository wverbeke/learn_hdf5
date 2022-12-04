#include "hdf5.h"
#include <string>
#include <vector>
#include "vec.h"


void write_vecs(const std::string& filename, const std::string& dset_name, const std::vector<Vec>& vecs){


    // Each vector has 3 elements.
    const hsize_t dims[2] = {vecs.size(), 3};
    
    // Convert the vectors to a 2D arr
    //const double vec_array[
    //std::vector<std::vector<double>> vec_array(
    double vec_array[vecs.size()][3];
    for(size_t i=0; i < vecs.size(); ++i){
        vec_array[i][0] = vecs[i].x();
        vec_array[i][1] = vecs[i].y();
        vec_array[i][2] = vecs[i].z();
    }

    hid_t file_id = H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    hid_t dataspace_id = H5Screate_simple(2, dims, NULL);
    hid_t dataset_id = H5Dcreate(file_id, dset_name.c_str(), H5T_IEEE_F64BE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    
    herr_t status = H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, vec_array);
    status = H5Dclose(dataset_id);
    status = H5Sclose(dataspace_id);
    status = H5Fclose(file_id);
}


int main(){

    std::vector<Vec> vecs_to_write;
    for(unsigned i=0; i < 100; ++i){
        vecs_to_write.push_back(Vec{static_cast<double>(i), static_cast<double>(i), static_cast<double>(i)});
    }
    write_vecs("vec_file.h5", "vectors", vecs_to_write);
    
    return 0;
}
