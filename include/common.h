#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_PATH_LEN 256
#define MAX_IDENTIFERLEN_LEN 32
#define MAX_NUMBER_LEN 32

void error(const char *msg);
void warning(const char *msg);

#endif