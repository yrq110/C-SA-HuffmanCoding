#ifdef TDLL1_API//若定义了TDLL1_API，则什么都不做
#else
#define TDLL1_API  extern "C"  _declspec(dllimport)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define END_OF_STREAM 256  //最后结束符为257
#define Max_code_length 256		//共257个字符，可能的最长代码二进制位数为256个，假设第257个字符为结束符
#define Max_number 257		//加上结束符，共257个字符

typedef struct tree_node
{
	double weight;			//权重
	int flag;			//标识是否为待构建结点，是的话用0表示，否则用1表示
	int parent;				//父节点
	int Lchild;				//左节点
	int Rchild;				//右节点
}NODE;

typedef struct codetype
{
	int code[Max_code_length];		//共257个字符，可能的最长代码二进制位数为256个，假设第257个字符为结束符
	int code_length;
}CODE;

TDLL1_API FILE *read_file_yrq(char *p);	
TDLL1_API FILE *write_file_yrq(char *p);
TDLL1_API void InitHuffman_yrq(NODE nodes[], int n);
TDLL1_API void count_bytes_yrq(FILE *input, NODE nodes[]);
TDLL1_API void scale_counts_yrq(NODE nodes[]);
TDLL1_API int build_tree_yrq(NODE nodes[]);
TDLL1_API void output_counts_yrq(FILE *output, NODE nodes[]);
TDLL1_API void convert_tree_to_code_yrq(CODE codes[], NODE nodes[]);
TDLL1_API void printf_model_yrq(NODE nodes[], CODE codes[]);
TDLL1_API void compress_data_yrq(FILE *input, FILE *output, CODE codes[]);
TDLL1_API void print_ratios_yrq(FILE *input, FILE *output);