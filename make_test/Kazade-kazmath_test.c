#include "chayai.h"

#include "Kazade-kazmath/kazmath/vec3.h"
#include "Kazade-kazmath/kazmath/mat4.h"
#include "Kazade-kazmath/kazmath/quaternion.h"


BENCHMARK(Kazade, kazmath, 10, 100) {
    for(int i = 0; i < 1000; i++) {
        struct kmVec3 v1;
        kmVec3Fill(&v1, 0, 1, 0);

        struct kmVec3 v2;
        kmVec3Fill(&v2, 1, 0, 0);

        struct kmVec3 fallback;
        kmVec3Fill(&fallback, 0, 0, 1);

        struct kmQuaternion q;
        kmQuaternionRotationBetweenVec3(&q, &v1, &v2, &fallback);

        kmQuaternionMultiplyVec3(&v1, &q, &v1);

        kmMat4 mat;
        kmMat4RotationQuaternion(&mat, &q);

        kmQuaternionRotationAxisAngle(&q, &KM_VEC3_POS_Y, kmDegreesToRadians(90));
        kmMat4RotationAxisAngle(&mat, &KM_VEC3_POS_Y, kmDegreesToRadians(90));
    }
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(Kazade, kazmath); // C math library for games

    RUN_BENCHMARKS(argc, argv);

    return 0;
}