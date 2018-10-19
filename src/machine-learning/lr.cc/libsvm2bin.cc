#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define MALLOC(type, n) (type *) malloc(sizeof(type) * n)

int buff_size = 1024;
char * buff = MALLOC(char, buff_size);

static int max_line_len = 1024;
static char *line = (char *)malloc(max_line_len);


static char* readline(FILE *input)
{
	int len;

	if(fgets(line,max_line_len,input) == NULL)
		return NULL;

	while(strrchr(line,'\n') == NULL)
	{
		max_line_len *= 2;
		line = (char *) realloc(line,max_line_len);
		len = (int) strlen(line);
		if(fgets(line+len,max_line_len-len,input) == NULL)
			break;
	}
	return line;
}


/**
 * fp 文件
 * label 标签或者ID
 * n 特征数量
 * idx idx列表
 * val 值列表
*/
void read_libsvm(FILE *fp, char *label, int n, int *idx, double *val){
    char * data = readline(fp);
    
}

int main(int argc, char ** argv){
    FILE *fp = fopen("digits.libsvm", "r");
    while(!feof(fp)){
        read_libsvm(fp);
    }
}