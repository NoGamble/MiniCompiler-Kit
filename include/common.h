#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAXPATHLEN 256

void error(const char *msg);
void warning(const char *msg);

#endif