#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "huff.h"
#define Max_code_length 256		//共257个字符，可能的最长代码二进制位数为256个，假设第257个字符为结束符
#define Max_number 257				//加上结束符，共257个字符


FILE *read_file_yrq(char *filename)
{
	FILE *fp = fopen(filename,"rb");		//以读写方式“rb”打开文件，允许读数据
	if(fp == NULL)
	{
		printf("打开失败，请检查文件是否存在！\n");
		exit(0);
	}
	else
	{
		return fp;
	}
}


FILE *write_file_yrq(char *filename)
{
	FILE *fp = fopen(filename,"wb");		//以只写方式“wb”打开或新建一个文件，允许写数据
	if(fp == NULL)
	{
		printf("新建文件失败！\n");
		exit(0);
	}
	else
	{
		return fp;
	}
}

void InitHuffman_yrq(NODE nodes[], int n)
{
int i;
for(i = 0; i< 2 * n - 1; i++)
{
		nodes[i].flag = 0;
		nodes[i].parent = 0;
		nodes[i].weight = 0;
		nodes[i].Lchild = -1;
		nodes[i].Rchild = -1;
	}
}

void count_bytes_yrq(FILE *filename, NODE nodes[])
{
	int i;
	nodes[256].weight = 1;		//结束符的权值为1
	for(i = 0; i<Max_number - 1; i++)
	{
		int ch;
		int j = 0;
		while((ch = fgetc(filename)) != EOF)		//如果没有遇到文件结束符，一直读取，每次读取一个字符
		{
			if(i == ch)
			{
				j++;
			}
		}
		nodes[i].weight = j;		 //将指针返回到文件头
		fseek(filename, 0, SEEK_SET);		//第二个参数表示“偏移量”，"SEEK_SET"表示从文件头开始偏移
	}
}

void scale_counts_yrq(NODE nodes[])
{
	double Max_weight = 0;
	int i = 0;
	for(i = 0; i<Max_number; i++)
	{
		if(Max_weight< nodes[i].weight)
			Max_weight = nodes[i].weight;
	}
	double G = floor(Max_weight/256) + 1;
	for(i = 0; i<Max_number; i++)
	{
		if(nodes[i].weight != 0)
		{
			nodes[i].weight = int(nodes[i].weight / G);  
			if(nodes[i].weight == 0)
			{
				nodes[i].weight = 1;		//当权值为0时，设权值为1
			}
		}
	}
}

void output_counts_yrq(FILE *filename, NODE nodes[])
{
	int i = 0;
	for(i = 0; i<Max_number; i++)
	{
		int t = nodes[i].weight;
		fputc(t, filename);
	}
}

//他人所写
void find_min2_yrq(NODE nodes[], int&min1, int&min2)    //查找树中权值最小的两个结点
{
	int i = 0, j = 0;
	int tmp = min2;
	int t = 1;

	for(i = 0; i< 2 * Max_number - 1; i++)
	{
		if(nodes[i].parent == 0 && nodes[i].weight != 0)
		{
			min1 = i;
			break;
		}
	}

	for(i = 0; i< 2 * Max_number - 1; i++)		//初始化min1
	{
		if((nodes[i].weight != 0) && (nodes[i].parent == 0) && (min1 != i) && (nodes[min1].weight > nodes[i].weight))
			min1 = i;
	}

	for(j = 0; j < 2 * Max_number - 1; j++)		//初始化min2
	{
		if(nodes[j].parent == 0 && nodes[j].weight != 0 && j != min1)
		{
			min2 = j;
			break;
		}
	}

	for(j = 0; j < 2 * Max_number - 1; j++)
	{
		if((nodes[j].weight != 0) && (nodes[j].parent == 0) && (min1 != j) && (nodes[min2].weight > nodes[j].weight))
		{
			min2 = j;
		}
	}
	if(tmp == min2)
		min2 = -1;
}

int build_tree_yrq(NODE nodes[])
{
	int i = 0;
	int min1 = 0, min2 = 0;
	int root_node = Max_number;

	for(i = Max_number; i< 2 * Max_number - 1; i++)
	{
		find_min2_yrq(nodes, min1, min2);

		if(min2 == -1)		//当min2 == -1时，跳出循环。并令根结点为min1
		{
			root_node = min1;
			break;
		}

		nodes[min1].parent = nodes[min2].parent = i;		//设置min1和min2的双亲为i
		nodes[i].Lchild = min1;			//设置i的左孩子为min1
		nodes[i].Rchild = min2;			//设置i的右孩子为min2
		nodes[i].weight = nodes[min1].weight + nodes[min2].weight;	//i的权值为min1和min2的权值之和
	}
	return root_node;
} 

//他人所写
void convert_tree_to_code_yrq(CODE codes[],NODE nodes[])
{
	int c[Max_number - 1];
	int i = 0, j = 0, k = 0;
	int cd, pt;
	for(i=0; i<= Max_number - 1; i++)
	{
		if(nodes[i].weight != 0)
		{
			cd = i;
			pt = nodes[cd].parent;
			while(pt != 0)
			{
				if(nodes[pt].Lchild == cd)
					c[j] = 0;
				else
					c[j] = 1;
				j++;
				cd = pt;
				pt = nodes[cd].parent;
			}
			codes[i].code_length = j;
			for(k = 0; j > 0; k++)
			{
				j--;
				codes[i].code[k] = c[j];
			}
		}
	}
}

void printf_model_yrq(NODE nodes[], CODE codes[])
{
	int i = 0, j = 0;
	for(i = 0; i<= Max_number - 1; i++)
	{
		if(nodes[i].weight != 0)
		{
			printf("ASCII --> %3d，Weight --> %3d，", i, int(nodes[i].weight));
			printf("Code --> ");
			for(j = 0; j < codes[i].code_length; j++)
			{
				printf("%d", codes[i].code[j]);
			}
			printf("\n"); 
		}
	}
}

void OutputBits_yrq(FILE *output, CODE codes, int *pbyte_length, unsigned char *pascii_code)
{
	int i = 0;
	for(i = 0; i<codes.code_length; i++)
	{
		*pascii_code = *pascii_code | (char)codes.code[i];		//按位或，将最后一位的值赋给*pascii_code的最后一位
		(*pbyte_length)--;
		if(*pbyte_length == 0)			//当左移够8位时输出到文件
		{
			fputc(*pascii_code, output);
			*pascii_code = 0;
			*pbyte_length = 8;
		}
		*pascii_code = *pascii_code<<  1;		//左移一位
	}
}

void compress_data_yrq(FILE *input, FILE *output, CODE codes[])
{
	int byte_length = 8;
	unsigned char ascii_code = 0;
	int ch = fgetc(input);

	while(ch != EOF)		//如果不到文件结尾，一直读取，每次读取一个字符
	{
		OutputBits_yrq(output, codes[ch], &byte_length, &ascii_code);
		ch = fgetc(input);
	}

	OutputBits_yrq(output, codes[Max_number - 1], &byte_length, &ascii_code);		//输出文件结束标记，如果不进行操作，文件解压时将无法停止

	if(byte_length != 8)		//如果最后一个字符不足8位，将有效字符移动到最前，然后输出
	{
		ascii_code = ascii_code<< (byte_length - 1);
		fputc(ascii_code, output);
	}
}

void print_ratios_yrq(FILE *input, FILE *output)               
{
	double i = ftell(input);               //使用ftell函数求出源文件和输出文件的文件大小
	double j = ftell(output);
	double k = i / j;
	printf("输入文件大小 --> %.2f\n", i);
	printf("压缩文件大小 --> %.2f\n", j);
	printf("压缩比 --> %f\n", k);
	fclose(input);                         //使用fclose函数关闭源文件和输出文件
	fclose(output);
}

