#include "chayai.h"  // first include!

#include <assert.h>

#include "Kazade-kazmath/kazmath/vec3.h"
#include "Kazade-kazmath/kazmath/mat4.h"
#include "Kazade-kazmath/kazmath/quaternion.h"


BENCHMARK(Kazade, kazmath, 100, 100) {
    kmMat4 res;
    kmMat4Identity(&res);

    for(int i = 0; i < 1000; i++) {
        struct kmVec3 v1;
        kmVec3Fill(&v1, i, 1, 2);

        struct kmVec3 v2;
        kmVec3Fill(&v2, 10, 5, i);

        struct kmVec3 fallback;
        kmVec3Fill(&fallback, 1, 0, 0);

        struct kmQuaternion q;
        kmQuaternionRotationBetweenVec3(&q, &v1, &v2, &fallback);

        kmQuaternionMultiplyVec3(&v1, &q, &v1);

        kmMat4 mat;
        kmMat4RotationQuaternion(&mat, &q);

        kmQuaternionRotationAxisAngle(&q, &KM_VEC3_POS_Y, kmDegreesToRadians(i));

        kmMat4Multiply(&res, &mat, &res);
    }

    assert((int)(res.mat[0]*1000) == 395 && (int)(res.mat[1]*1000) == -887);
    assert((int)(res.mat[4]*1000) == -126 && (int)(res.mat[5]*1000) == -309);
    assert((int)(res.mat[8]*1000) == 909 && (int)(res.mat[9]*1000) == 343);
    assert(res.mat[12] == 0. && res.mat[13] == 0.);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(Kazade, kazmath); // C math library for games

    RUN_BENCHMARKS(argc, argv);

    return 0;
}