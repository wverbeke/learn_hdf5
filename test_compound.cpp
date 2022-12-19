#include "hdf5.h"
#include <string>
#include <vector>
#include "vec.h"
#include <iostream>
#include <cstddef>
#include <algorithm>

struct TimedVec{
    double t;
    double vec[3];
};

void write_timed_vecs(const std::string& filename, const std::string& dset_name, const std::vector<TimedVec>& vecs){

    // Make the hdf5 file.
    hid_t file_id = H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    hsize_t dims[1] = {vecs.size()};
    hid_t dspace_id = H5Screate_simple(1, dims, NULL);

    hsize_t array_dims[1] = {3};
    hid_t array_type_id = H5Tarray_create(H5T_NATIVE_DOUBLE, static_cast<hsize_t>(1), array_dims);
    hid_t compound_type_id = H5Tcreate(H5T_COMPOUND, sizeof(vecs[0]));
    H5Tinsert(compound_type_id, "t", offsetof(TimedVec, t), H5T_NATIVE_DOUBLE);
    H5Tinsert(compound_type_id, "vec", offsetof(TimedVec, vec), array_type_id);

    hid_t dset_id = H5Dcreate(file_id, "timedvecs", compound_type_id, dspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    herr_t status = H5Dwrite(dset_id, compound_type_id, H5S_ALL, H5S_ALL, H5P_DEFAULT, &vecs[0]);


    H5Dclose(dset_id);
    H5Tclose(compound_type_id);
    H5Tclose(array_type_id);
    H5Sclose(dspace_id);
    H5Fclose(file_id);

}


int main(){

    std::vector<TimedVec> vecs_to_write;
    for(unsigned i=0; i < 100; ++i){
        TimedVec tv;
        tv.t = static_cast<double>(i);
        auto vec = Vec<double>{static_cast<double>(i), static_cast<double>(i), static_cast<double>(i)};
        tv.vec[0] = vec.x();
        tv.vec[1] = vec.y();
        tv.vec[2] = vec.z();
        vecs_to_write.push_back(tv);
    }
    write_timed_vecs("compound_file.h5", "vectors", vecs_to_write);
    
    return 0;
}
