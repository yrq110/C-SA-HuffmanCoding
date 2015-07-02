#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "huff.h"
#define Max_code_length 256		//��257���ַ������ܵ�����������λ��Ϊ256���������257���ַ�Ϊ������
#define Max_number 257				//���Ͻ���������257���ַ�


FILE *read_file_yrq(char *filename)
{
	FILE *fp = fopen(filename,"rb");		//�Զ�д��ʽ��rb�����ļ������������
	if(fp == NULL)
	{
		printf("��ʧ�ܣ������ļ��Ƿ���ڣ�\n");
		exit(0);
	}
	else
	{
		return fp;
	}
}


FILE *write_file_yrq(char *filename)
{
	FILE *fp = fopen(filename,"wb");		//��ֻд��ʽ��wb���򿪻��½�һ���ļ�������д����
	if(fp == NULL)
	{
		printf("�½��ļ�ʧ�ܣ�\n");
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
	nodes[256].weight = 1;		//��������ȨֵΪ1
	for(i = 0; i<Max_number - 1; i++)
	{
		int ch;
		int j = 0;
		while((ch = fgetc(filename)) != EOF)		//���û�������ļ���������һֱ��ȡ��ÿ�ζ�ȡһ���ַ�
		{
			if(i == ch)
			{
				j++;
			}
		}
		nodes[i].weight = j;		 //��ָ�뷵�ص��ļ�ͷ
		fseek(filename, 0, SEEK_SET);		//�ڶ���������ʾ��ƫ��������"SEEK_SET"��ʾ���ļ�ͷ��ʼƫ��
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
				nodes[i].weight = 1;		//��ȨֵΪ0ʱ����ȨֵΪ1
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

//������д
void find_min2_yrq(NODE nodes[], int&min1, int&min2)    //��������Ȩֵ��С���������
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

	for(i = 0; i< 2 * Max_number - 1; i++)		//��ʼ��min1
	{
		if((nodes[i].weight != 0) && (nodes[i].parent == 0) && (min1 != i) && (nodes[min1].weight > nodes[i].weight))
			min1 = i;
	}

	for(j = 0; j < 2 * Max_number - 1; j++)		//��ʼ��min2
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

		if(min2 == -1)		//��min2 == -1ʱ������ѭ������������Ϊmin1
		{
			root_node = min1;
			break;
		}

		nodes[min1].parent = nodes[min2].parent = i;		//����min1��min2��˫��Ϊi
		nodes[i].Lchild = min1;			//����i������Ϊmin1
		nodes[i].Rchild = min2;			//����i���Һ���Ϊmin2
		nodes[i].weight = nodes[min1].weight + nodes[min2].weight;	//i��ȨֵΪmin1��min2��Ȩֵ֮��
	}
	return root_node;
} 

//������д
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
			printf("ASCII --> %3d��Weight --> %3d��", i, int(nodes[i].weight));
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
		*pascii_code = *pascii_code | (char)codes.code[i];		//��λ�򣬽����һλ��ֵ����*pascii_code�����һλ
		(*pbyte_length)--;
		if(*pbyte_length == 0)			//�����ƹ�8λʱ������ļ�
		{
			fputc(*pascii_code, output);
			*pascii_code = 0;
			*pbyte_length = 8;
		}
		*pascii_code = *pascii_code<<  1;		//����һλ
	}
}

void compress_data_yrq(FILE *input, FILE *output, CODE codes[])
{
	int byte_length = 8;
	unsigned char ascii_code = 0;
	int ch = fgetc(input);

	while(ch != EOF)		//��������ļ���β��һֱ��ȡ��ÿ�ζ�ȡһ���ַ�
	{
		OutputBits_yrq(output, codes[ch], &byte_length, &ascii_code);
		ch = fgetc(input);
	}

	OutputBits_yrq(output, codes[Max_number - 1], &byte_length, &ascii_code);		//����ļ�������ǣ���������в������ļ���ѹʱ���޷�ֹͣ

	if(byte_length != 8)		//������һ���ַ�����8λ������Ч�ַ��ƶ�����ǰ��Ȼ�����
	{
		ascii_code = ascii_code<< (byte_length - 1);
		fputc(ascii_code, output);
	}
}

void print_ratios_yrq(FILE *input, FILE *output)               
{
	double i = ftell(input);               //ʹ��ftell�������Դ�ļ�������ļ����ļ���С
	double j = ftell(output);
	double k = i / j;
	printf("�����ļ���С --> %.2f\n", i);
	printf("ѹ���ļ���С --> %.2f\n", j);
	printf("ѹ���� --> %f\n", k);
	fclose(input);                         //ʹ��fclose�����ر�Դ�ļ�������ļ�
	fclose(output);
}

