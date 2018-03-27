#include "chayai.h"

#include "yinqiwen-geohash-int/geohash.h"

BENCHMARK(yinqiwen, geohash, 10, 100) {
    GeoHashBits hash, fast_hash;

    GeoHashRange lat_range, lon_range;
    lat_range.max = 90.0;
    lat_range.min = -90.0;
    lon_range.max = 180.0;
    lon_range.min = -180.0;

    for(double latitude = -90; latitude <= 90; latitude += 1) {
        for(double longitude = -180; longitude <= 180; longitude += 1) {
            for (int step = 1; step <= 32; step++) {
                geohash_encode(lat_range, lon_range, latitude, longitude, step, &hash);
                geohash_fast_encode(lat_range, lon_range, latitude, longitude, step, &fast_hash);
            }
        }
    }
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(yinqiwen, geohash); // calculate Geohash

    RUN_BENCHMARKS(argc, argv);

    return 0;
}