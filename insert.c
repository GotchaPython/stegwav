#include <stdio.h>
#include <stdlib.h>

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

    if(argc < 4)
    {
        printf("Usage: %s WAV INPUT OUTPUT\n", argv[0]);
        return 0;
    }

    // Read wav file
    long s1, s2;
    unsigned char *f1 = read_file(argv[1], &s1);
    unsigned char *f2 = read_file(argv[2], &s2);
    if(f1 == NULL || f2 == NULL)
    {
        printf("Error reading files\n");
        return 0;
    }
    
    // Merge files / do some magic
    for(int i=44,o=i-44;i<s1;i+=2,o=i-44)
        f1[i]&=254,f1[i]|=o/16<s2?(f2[o/16]>>(7-o%16/2))&1:0;

    // Write to output file
    write_file(argv[3], f1, s1);

    // Some feedback
    printf("Merging done\n");
}
