#include "hdf5.h"
#include <string>
#include <iostream>

const std::string FILE_NAME = "file.h5";
const std::string DATASET_NAME = "/array";
constexpr hsize_t dims[2] = {5, 5};

int main(){

    unsigned write_array[dims[0]][dims[1]];
    for(unsigned i=0; i < dims[0]; ++i){
        for(unsigned j=0; j < dims[1]; ++j){
            write_array[i][j] = i*dims[1] + j;
        }
    }

    herr_t status;
    
    hid_t file_id = H5Fcreate(FILE_NAME.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    hid_t dataspace_id = H5Screate_simple(2, dims, NULL);
    hid_t dataset_id = H5Dcreate(file_id, DATASET_NAME.c_str(), H5T_STD_U32BE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    status = H5Dwrite(dataset_id, H5T_NATIVE_UINT, H5S_ALL, H5S_ALL, H5P_DEFAULT, write_array);

    status = H5Dclose(dataset_id);
    status = H5Sclose(dataspace_id);
    status = H5Fclose(file_id);

    unsigned read_array[dims[0]][dims[1]];
    
    hid_t read_file_id = H5Fopen(FILE_NAME.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
    hid_t read_dataset_id = H5Dopen(read_file_id, DATASET_NAME.c_str(), H5P_DEFAULT);
    status = H5Dread(read_dataset_id, H5T_NATIVE_UINT, H5S_ALL, H5S_ALL, H5P_DEFAULT, read_array);
    status = H5Dclose(read_dataset_id);
    status = H5Fclose(read_file_id);

    for(unsigned i=0; i < dims[0]; ++i){
        for(unsigned j=0; j < dims[1]; ++j){
            std::cout << read_array[i][j]  - write_array[i][j] << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}
