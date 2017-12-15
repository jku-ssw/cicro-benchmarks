#include <string.h>

#include "harness.h"

#include <stdio.h>
#include <brainfuck.h>

int __attribute__ ((noinline)) test_harness(void) {
    BrainfuckState *state = brainfuck_state();
    BrainfuckExecutionContext *context = brainfuck_context(BRAINFUCK_TAPE_SIZE);
    BrainfuckInstruction *instruction = brainfuck_parse_string("+++[->-[->-[->-[-]<]<]<]");
    brainfuck_add(state, instruction);
    brainfuck_execute(state->root, context);
    brainfuck_destroy_context(context);
    brainfuck_destroy_state(state);
    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="fabianishere-brainfuck",
        .description="Brainfuck interpreter written in C ",
        .test_harness=*test_harness,
        .expected_runtime=370L
    };
    return _execute_harness(argc, argv, harness);
}
