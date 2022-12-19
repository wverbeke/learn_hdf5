#include "hdf5.h"
#include "vec.h"
#include <stdexcept>
#include <vector>

template <typename T> hid_t get_h5_type(T){
    throw std::domain_error("Invalid type for h5 writing.");
}

hid_t get_h5_type(double){
    return H5T_IEEE_F64BE;
}

hid_t get_h5_type(float){
    return H5T_IEEE_F32BE;
}

template <typename T> hid_t get_native_type(T){
    throw std::domain_error("Invalid type for h5 writing.");
}

hid_t get_native_type(double){
    return H5T_NATIVE_DOUBLE; 
}

hid_t get_native_type(float){
    return H5T_NATIVE_FLOAT;
}


template <typename T> void write_vecs(const std::string& file_name, const std::string& dset_name, const std::vector<T>& vecs){

    // Dimensionality.
    hsize_t dims[2] = {vecs.size(), 3};
    hsize_t max_dims[2] = {H5S_UNLIMITED, 3};

    // Vector to array.
    typename T::value_type write_array[dims[0]][dims[1]];
    for(size_t i=0; i < dims[0]; ++i){
        write_array[i][0] = vecs[i].x();
        write_array[i][1] = vecs[i].y();
        write_array[i][2] = vecs[i].z();
    }
    
    hid_t file_id = H5Fcreate(file_name.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    hid_t dspace_id = H5Screate_simple(2, dims, NULL);
    hid_t dset_id = H5Dcreate(file_id, dset_name.c_str(), get_h5_type(typename T::value_type()), dspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    herr_t status = H5Dwrite(dset_id, get_native_type(typename T::value_type()), H5S_ALL, H5S_ALL, H5P_DEFAULT, write_array);
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
}
