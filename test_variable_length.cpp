#include "hdf5.h"
#include <string>
#include <vector>
#include "vec.h"
#include <iostream>
#include <cstddef>
#include <algorithm>

//void write_timed_vecs(const std::string& filename, const std::string& dset_name, const std::vector<std::vector<double>>& vec_vec){
void write_timed_vecs(const std::string& filename, const std::string& dset_name, const std::vector<double>& var_vec){

    // Make the hdf5 file.
    hid_t file_id = H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    // Dataspace.  
    hsize_t dims[1] = {1};
    hid_t dspace_id = H5Screate_simple(1, dims, NULL);

    // Because const vector& is passed.
    double write_array[var_vec.size()];
    std::copy(var_vec.cbegin(), var_vec.cend(), &write_array[0]);
    
    hvl_t vw_data[1];
    vw_data[0].len = var_vec.size();
    vw_data[0].p = &write_array[0];
    hid_t file_type = H5Tvlen_create(H5T_IEEE_F64BE);
    hid_t memory_type = H5Tvlen_create(H5T_NATIVE_DOUBLE);

    hid_t dset_id = H5Dcreate(file_id, "var_vecs", file_type, dspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    herr_t status = H5Dwrite(dset_id, memory_type, H5S_ALL, H5S_ALL, H5P_DEFAULT, vw_data);
    status = H5Dclose(dset_id);
    status = H5Tclose(memory_type);
    status = H5Tclose(file_type);
    status = H5Sclose(dspace_id);
    status = H5Fclose(file_id);

    //// Make the variable sized 
    //for(unsigned i = 0; i < vec_vec.size(); ++i){
    //    hvl_t vw_data;
    //    double write_array[vec_vec[i].size()];
    //    for(unsigned j = 0; j < vec_vec[i].size(); ++j){
    //        write_array[j] = vec_vec[i][j];
    //    }
    //    vw_data.len = vec_vec[i].size();
    //    vw_data.p = write_array;

    //    herr_t status = H5Dwrite(dset_id, compound_type_id, H5S_ALL, H5S_ALL, H5P_DEFAULT, &vecs[0]);

}


int main(){

    //std::vector<vector<double>> vec_vec;
    //for(unsigned i=0; i < 100; ++i){
    //    vec_vec.push_back(std::vector<double>());
    //    for(unsigned j = 1; j < i + 1; ++j){
    //       vec_vec[i].push_back(static_cast<double>(j)); 
    //    }
    //    vecs_to_write.push_back(tv);
    //}
    std::vector<double> var_vec;
    for(unsigned i = 0; i < 100; ++i){
        var_vec.push_back(static_cast<double>(i));
    }
    write_timed_vecs("variable_sized_file.h5", "variable_sized_vectors", var_vec);
    return 0;
}
