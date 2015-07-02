#include "stdafx.h"
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

	CODE codes[Max_number];//共257个字符，假设存在一个结束符，其ascii码为257
	NODE nodes[2*Max_number-1]; // 共257个字符，所以存在2*257-1=513个结点
	printf("\n 压缩  %s  到  %s\n",argv[1],argv[2]);
	input=read_file_yrq(argv[1]);//打开原始文件，读文件
	output=write_file_yrq(argv[2]); //创建一个新文件，准备写入
	InitHuffman_yrq(nodes,Max_number);//初始化dLL和exe两者都行

	count_bytes_yrq(input,nodes);//计算每个ascii码出现的次数，即求权重
	scale_counts_yrq(nodes); // dLL和exe两者都行，对权重进行伸缩，使最大权重max'不超过255，当然也可不进行伸缩，若不伸缩，则存储空间加大，而output_counts中的字符也要换成fwrite
	output_counts_yrq(output,nodes);  //把每个字符的权值输出到输出文件中
	build_tree_yrq(nodes); //dLL和exe两者都行，构建huffman树build_tree(nodes); 返回根结点的位置，但压缩时无需返回根结点，还原程序需要返回值

	convert_tree_to_code_yrq(codes,nodes);  // dLL和exe两者都行，把huffman树转化成二进制代码,即编码，左孩子（最小）编码为0，右孩子(次小)编码为1
	if (argc>3&&strcmp(argv[3],"-d")==0) //argc>3必须要有，否则出错，没有第3个参数，就不能进行strcmp操作
		printf_model_yrq(nodes,codes); // dLL和exe两者都行，可选函数，该函数不是必须的，若有第3个参数"-d"，则打印输出编码模型

	compress_data_yrq(input,output,codes);//准备压缩文件，里面调用OutputBits函数
	print_ratios_yrq(input,output);//可选函数打印压缩比，注意文件指针要定位到末尾，并关闭文件
	printf("压缩完成\n");	

	return 0;
}

