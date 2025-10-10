#include "../include/common.h"

void error(const char *msg)
{
    fprintf(stderr, "[Error] %s", msg);
    exit(EXIT_FAILURE);
}

void warning(const char *msg)
{
    fprintf(stderr, "[Warning] %s", msg);
}
