/*************************************************************************
	> File Name: main.c
	> Author: Ezreal
	> Mail: liangenze@pinecone.net
	> Created Time: 2017年12月04日 星期一 10时17分12秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<errno.h>

#define INTERVAL 1024

/********
 arg1:input file 1
 arg2:input file 2
 arg3:output yuv file
 arg4:input file size
 */
int main(int argc, char *argv[])
{
    int n = 0;
    printf("argc = %d\n", argc);
    while(--argc)
    {
        printf("arg%d : %s\n", n++, argv[n+1]);
    }
    char *in1, *in2, *out;
    int pos1 = 0, pos2 = 0, pos3 = 0;
    long int size1, size2, size_out;
    char *buf_in1, *buf_in2, *buffer, *result;
    char s[3];
    FILE *fp1, *fp2, *fp3;
    int i, tmp;
    struct stat f_st1, f_st2;
    in1 = argv[1];//input memory dump file 1
    in2 = argv[2];//input memory dump file 2
    out = argv[3];//output yuv file name
    printf("%s %s %s\n", in1, in2, out);
    int res = stat(in1, &f_st1);
    if(res != 0)
    {
        printf("get file %s stat error!\n", in1);
    } else {
        size1 = f_st1.st_size;
        printf("file %s size = %ld\n", in1, size1);
    }
    res = stat(in2, &f_st2);
    if(res != 0)
    {
        printf("get file %s stat error!\n", in2);
    } else {
        size2 = f_st2.st_size;
        printf("file %s size = %ld\n", in2, size2);
    }
    buf_in1 = malloc(size1);
    buf_in2 = malloc(size2);
    buffer = malloc(size1 + size2);
    size_out = (size1 + size2)/2;
    result = malloc(size_out);

    fp1 = fopen(in1, "r");
    fp2 = fopen(in2, "r");
    fp3 = fopen(out, "w");
    fread(buf_in1, sizeof(char), size1, fp1);
    fread(buf_in2, sizeof(char), size2, fp2);

    while(pos1 < size1 || pos2 < size2)
    {   
        if(pos1 < size1)
        {   
            if((size1 - pos1) < INTERVAL)
            {   
                memcpy(&buffer[pos3], &buf_in1[pos1], size1 % INTERVAL);
                pos1 += (size1 % INTERVAL);
                pos3 += (size1 % INTERVAL);
            }   
            else
            {   
                memcpy(&buffer[pos3], &buf_in1[pos1], INTERVAL);
                pos1 += INTERVAL;
                pos3 += INTERVAL;
            }   
        }   
        if(pos2 < size2)
        {   
            if((size2 - pos2) < INTERVAL)
            {   
                memcpy(&buffer[pos3], &buf_in2[pos2], size2 % INTERVAL);
                pos2 += (size2 % INTERVAL);
                pos3 += (size2 % INTERVAL);
            }
            else
            {
                memcpy(&buffer[pos3], &buf_in2[pos2], INTERVAL);
                pos2 += INTERVAL;
                pos3 += INTERVAL;
            }
        }
    }

    for(i = 0;i < (size1 + size2);i+=2)
    {
        memcpy(s, &buffer[i], 2);
        s[2] = '\0';
        tmp = strtol(s, NULL, 16);
        result[i/2] = tmp;
    }
    printf("convert end\n");
    fwrite(result, sizeof(char), size_out, fp3);

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);

    return 0;
}


