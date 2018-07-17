add_library(chayai
    chayai_benchmarker.c
    chayai_clock.c
    chayai_console.c
    chayai_console_outputter.c
    chayai_json_outputter.c
    chayai_main.c
    chayai_papi.c
    chayai_util.c
)

target_link_libraries(chayai m)

if( USE_PAPI )
    target_link_libraries(chayai papi)
endif()


set_property(TARGET chayai PROPERTY C_STANDARD 99)