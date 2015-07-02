#include "stdafx.h"
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

#define TDLL1_API  extern "C"  _declspec(dllimport)
TDLL1_API FILE *read_file_yrq(char *p);	
TDLL1_API FILE *write_file_yrq(char *p);
TDLL1_API void InitHuffman_yrq(NODE nodes[], int n);
TDLL1_API void count_bytes_yrq(FILE *input, NODE nodes[]);
TDLL1_API void scale_counts_yrq(NODE nodes[]);
TDLL1_API int build_tree_yrq(NODE nodes[]);
TDLL1_API void convert_tree_to_code_yrq(CODE codes[], NODE nodes[]);
TDLL1_API void output_counts_yrq(FILE *output, NODE nodes[]);
TDLL1_API void printf_model_yrq(NODE nodes[], CODE codes[]);
TDLL1_API void compress_data_yrq(FILE *input, FILE *output, CODE codes[]);
TDLL1_API void print_ratios_yrq(FILE *input, FILE *output);

int main(int argc, char *argv[])
{
	FILE *input = read_file_yrq(argv[1]);
	FILE *output = write_file_yrq(argv[2]);

	CODE codes[Max_number];//��257���ַ����������һ������������ascii��Ϊ257
	NODE nodes[2*Max_number-1]; // ��257���ַ������Դ���2*257-1=513�����
	printf("\n ѹ��  %s  ��  %s\n",argv[1],argv[2]);
	input=read_file_yrq(argv[1]);//��ԭʼ�ļ������ļ�
	output=write_file_yrq(argv[2]); //����һ�����ļ���׼��д��
	InitHuffman_yrq(nodes,Max_number);//��ʼ��dLL��exe���߶���

	count_bytes_yrq(input,nodes);//����ÿ��ascii����ֵĴ���������Ȩ��
	scale_counts_yrq(nodes); // dLL��exe���߶��У���Ȩ�ؽ���������ʹ���Ȩ��max'������255����ȻҲ�ɲ�����������������������洢�ռ�Ӵ󣬶�output_counts�е��ַ�ҲҪ����fwrite
	output_counts_yrq(output,nodes);  //��ÿ���ַ���Ȩֵ���������ļ���
	build_tree_yrq(nodes); //dLL��exe���߶��У�����huffman��build_tree(nodes); ���ظ�����λ�ã���ѹ��ʱ���践�ظ���㣬��ԭ������Ҫ����ֵ

	convert_tree_to_code_yrq(codes,nodes);  // dLL��exe���߶��У���huffman��ת���ɶ����ƴ���,�����룬���ӣ���С������Ϊ0���Һ���(��С)����Ϊ1
	if (argc>3&&strcmp(argv[3],"-d")==0) //argc>3����Ҫ�У��������û�е�3���������Ͳ��ܽ���strcmp����
		printf_model_yrq(nodes,codes); // dLL��exe���߶��У���ѡ�������ú������Ǳ���ģ����е�3������"-d"�����ӡ�������ģ��

	compress_data_yrq(input,output,codes);//׼��ѹ���ļ����������OutputBits����
	print_ratios_yrq(input,output);//��ѡ������ӡѹ���ȣ�ע���ļ�ָ��Ҫ��λ��ĩβ�����ر��ļ�
	printf("ѹ�����\n");	

	return 0;
}

