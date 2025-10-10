#include <stdio.h>
#include "../include/lexer.h"

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        printf("Usage: %s <source_file> <output_base>\n", argv[0]);
        printf("Example: %s test.mini output/test\n", argv[0]);
        return 1;
    }
    const char *sourcePath = argv[1];
    const char *outputPath = argv[2];
    runLexer(sourcePath, outputPath);
    return 0;
}