#ifdef TDLL1_API//��������TDLL1_API����ʲô������
#else
#define TDLL1_API  extern "C"  _declspec(dllimport)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define END_OF_STREAM 256  //��������Ϊ257
#define Max_code_length 256		//��257���ַ������ܵ�����������λ��Ϊ256���������257���ַ�Ϊ������
#define Max_number 257		//���Ͻ���������257���ַ�

typedef struct tree_node
{
	double weight;			//Ȩ��
	int flag;			//��ʶ�Ƿ�Ϊ��������㣬�ǵĻ���0��ʾ��������1��ʾ
	int parent;				//���ڵ�
	int Lchild;				//��ڵ�
	int Rchild;				//�ҽڵ�
}NODE;

typedef struct codetype
{
	int code[Max_code_length];		//��257���ַ������ܵ�����������λ��Ϊ256���������257���ַ�Ϊ������
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