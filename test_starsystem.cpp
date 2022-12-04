#include "hdf5.h"
#include "body.h"
#include "vec.h"
#include "starsystem.h"
#include <string>


void write_star_system(const std::string& filename, const std::string& dset_name, const StarSystem& star_system){

    const size_t num_bodies{star_system.size()};

    // per body: 3 position entries, 3 velocity entries, 1 mass entry
    const hsize_t dims[2] = {7, num_bodies};

    double data_array[dims[0]][dims[1]];
    for(size_t i=0; i < star_system.size(); ++i){
        const auto& x = star_system[i].x();
        data_array[0][i] = x.x();
        data_array[1][i] = x.y();
        data_array[2][i] = x.z();

        const auto& v = star_system[i].v();
        data_array[3][i] = v.x();
        data_array[4][i] = v.y();
        data_array[5][i] = v.z();

        data_array[6][i] = star_system[i].m();
    }


    hid_t file_id = H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    hid_t dspace_id = H5Screate_simple(2, dims, NULL);
    hid_t dset_id = H5Dcreate(file_id, dset_name.c_str(), H5T_IEEE_F64BE, dspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    
    herr_t status = H5Dwrite(dset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, data_array);
    status = H5Dclose(dset_id);
    status = H5Sclose(dspace_id);
    status = H5Fclose(file_id);
}


int main(){
    std::vector<Body> bodies;
    for(size_t i = 0; i < 100; ++i){
        double xval = static_cast<double>(i);
        Vec x{xval, xval, xval};
        double vval = static_cast<double>(i*2);
        Vec v{vval, vval, vval};
        double m = static_cast<double>(i*3);
        bodies.push_back(Body(x, v, m));
    }
    StarSystem s(bodies);
    
    write_star_system("single_star_system.h5", "star_systen", bodies);
}
