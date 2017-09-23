#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char *read_file(char *path, long *size)
{
    // Open file
    FILE *f = fopen(path, "rb");
    if(f==NULL) return NULL;

    // Get file size
    fseek(f, 0, SEEK_END);
    *size = ftell(f);
    fseek(f, 0, SEEK_SET);

    // Read data
    unsigned char *data = malloc(*size);
    fread(data, *size, 1, f);
    fclose(f);

    return data;
}

void write_file(char *path, char *data, long size)
{
    FILE *f = fopen(path, "wb");
    fwrite(data, size, 1, f);
    fclose(f);
}

int main(int argc, char **argv)
{
    // Print help
    printf( "Note: This program only supports files of wav format\n"
            "      with a 16bit sample size and a header of exact 44 bytes.\n");

    if(argc < 3)
    {
        printf("Usage: %s WAV OUTPUT\n", argv[0]);
        return 0;
    }

    // Read wav file
    long s1, s2;
    unsigned char *f1 = read_file(argv[1], &s1);
    if(f1 == NULL)
    {
        printf("Error reading files\n");
        return 0;
    }
    
    // Allocate some memory for the output file
    s2 = (s1-44)/16;
    unsigned char *f2 = malloc(s2);
    memset(f2, 0, s2);
    
    // Extract file / do some magic
    for(int i=0,o=i+44;i<s2*8;i++,o=i+44)
        f2[i/8]|=(f1[i*2+44]&1)<<(7-(i%8));

    // Write to output file
    write_file(argv[2], f2, s2);

    // Some feedback
    printf("Extracting done\n");
}
