#include "chayai_util.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>


#define WORKDIR_ENV_VARIABLE "CHAYAI_WORKDIR"

static bool tmp_dir_created = false;
static char tmp_dir[] = "/tmp/chayai.XXXXXX";

static void _chayai_remove_tmp_dir() {
    if(tmp_dir_created) {
        tmp_dir_created = false;
        rmdir(tmp_dir);
    }
}

char *chayai_util_get_tmp_dir() {
    // tmp_dir already created?
    if(tmp_dir_created) {
        return tmp_dir;
    }

    // check for environment variable
    char *env_var = getenv(WORKDIR_ENV_VARIABLE); // secure_getenv would require "#define _GNU_SOURCE"
    if(env_var != NULL) {
        return env_var;
    }

    char *created_dir = mkdtemp(tmp_dir);
    if(created_dir == tmp_dir) {
        tmp_dir_created = true;
        atexit(_chayai_remove_tmp_dir);
        return created_dir;
    }

    abort(); // TODO: should not happen
}
