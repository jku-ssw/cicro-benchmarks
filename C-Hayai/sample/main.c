
#include <chayai.h>
#include "delivery_man.h"
#include <chayai_main.c>

int main(int argc, char **argv)
{
    REGISTER_BENCHMARK(DeliveryMan, DeliverPackage);
    
    REGISTER_BENCHMARK_P(DeliveryMan, DeliverPackage, 1);
    REGISTER_BENCHMARK_P(DeliveryMan, DeliverPackage, 2);
    REGISTER_BENCHMARK_P(DeliveryMan, DeliverPackage, 3);

    REGISTER_BENCHMARK_F(SlowDeliveryManFixture, DeliverPackage);

    REGISTER_BENCHMARK_P_F(FastDeliveryManFixture, DeliverPackage, 1);
    REGISTER_BENCHMARK_P_F(FastDeliveryManFixture, DeliverPackage, 2);
    REGISTER_BENCHMARK_P_F(FastDeliveryManFixture, DeliverPackage, 3);

    REGISTER_BENCHMARK(SomeSleep, Sleep1ms);
    REGISTER_BENCHMARK(SomeSleep, Sleep10ms);
    REGISTER_BENCHMARK(SomeSleep, Sleep20ms);

    RUN_BENCHMARKS(argc, argv);
    
    return 0;
}
