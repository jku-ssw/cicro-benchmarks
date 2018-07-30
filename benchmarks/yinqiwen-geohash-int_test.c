#include "chayai.h"  // first include!

#include <assert.h>

#include "yinqiwen-geohash-int/geohash.h"

BENCHMARK(yinqiwen, geohash, 10, 1) {
    GeoHashBits hash, fast_hash;

    uint64_t sum_bits = 0;

    GeoHashRange lat_range, lon_range;
    lat_range.max = 90.0;
    lat_range.min = -90.0;
    lon_range.max = 180.0;
    lon_range.min = -180.0;

    for(double latitude = -90; latitude <= 90; latitude += 1) {
        for(double longitude = -180; longitude <= 180; longitude += 1) {
            for (int step = 1; step <= 32; step++) {
                int ret = geohash_encode(lat_range, lon_range, latitude, longitude, step, &hash);
                assert(ret == 0);
                sum_bits += hash.bits;

                ret = geohash_fast_encode(lat_range, lon_range, latitude, longitude, step, &fast_hash);
                assert(ret == 0);
                sum_bits += hash.bits;
            }
        }
    }

    assert(sum_bits == 0x38E38E38E36C6F08);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(yinqiwen, geohash); // calculate Geohash

    RUN_BENCHMARKS(argc, argv);

    return 0;
}