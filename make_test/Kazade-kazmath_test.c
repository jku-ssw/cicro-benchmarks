#include "harness.h"

#include "Kazade-kazmath/kazmath/vec3.h"
#include "Kazade-kazmath/kazmath/mat4.h"
#include "Kazade-kazmath/kazmath/quaternion.h"


int __attribute__ ((noinline)) test_harness(void) {
    for(int i = 0; i < 1000000; i++) {
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

    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="Kazade-kazmath",
        .description="C math library for games",
        .test_harness=*test_harness,
        .expected_runtime=340L
    };
    return _execute_harness(argc, argv, harness);
}