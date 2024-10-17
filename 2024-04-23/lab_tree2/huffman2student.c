// 文件压缩-Huffman实现
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXSIZE 32

struct tnode
{ // Huffman树结构
	char c;
	int weight; // 树节点权重，叶节点为字符和它的出现次数
	struct tnode *left, *right;
};
int Ccount[128] = {0};			  // 存放每个字符的出现次数，如Ccount[i]表示ASCII值为i的字符出现次数
struct tnode *Root = NULL;		  // Huffman树的根节点
char HCode[128][MAXSIZE] = {{0}}; // 字符的Huffman编码，如HCode['a']为字符a的Huffman编码（字符串形式）
int Step = 0;					  // 实验步骤
FILE *Src, *Obj;

void statCount();	// 步骤1：统计文件中字符频率
void createHTree(); // 步骤2：创建一个Huffman树，根节点为Root
void makeHCode();	// 步骤3：根据Huffman树生成Huffman编码
void atoHZIP();		// 步骤4：根据Huffman编码将指定ASCII码文本文件转换成Huffman码文件

void print1();				  // 输出步骤1的结果
void print2(struct tnode *p); // 输出步骤2的结果
void print3();				  // 输出步骤3的结果
void print4();				  // 输出步骤4的结果

int main()
{
	if ((Src = fopen("input.txt", "r")) == NULL)
	{
		fprintf(stderr, "%s open failed!\n", "input.txt");
		return 1;
	}
	if ((Obj = fopen("output.txt", "w")) == NULL)
	{
		fprintf(stderr, "%s open failed!\n", "output.txt");
		return 1;
	}
	scanf("%d", &Step); // 输入当前实验步骤

	statCount(); // 实验步骤1：统计文件中字符出现次数（频率）
	if (Step == 1)
		print1();  // 输出实验步骤1结果
	createHTree(); // 实验步骤2：依据字符频率生成相应的Huffman树
	if (Step == 2)
		print2(Root); // 输出实验步骤2结果
	makeHCode();	  // 实验步骤3：依据Root为树的根的Huffman树生成相应Huffman编码
	if (Step == 3)
		print3(); // 输出实验步骤3结果
	if (Step >= 4)
		atoHZIP(), print4(); // 实验步骤4：据Huffman编码生成压缩文件，并输出实验步骤4结果

	fclose(Src);
	fclose(Obj);

	return 0;
}

// 【实验步骤1】开始

void statCount()
{
	char ch;
	Ccount[0] = 1;
	while ((ch = fgetc(Src)) != EOF)
	{
		Ccount[(int)ch]++;
	}
}

// 【实验步骤1】结束

// 【实验步骤2】开始

struct tnode *forest[128] = {NULL}; // forest is an array of pointers to tnode

void sortForest(int sum)
{
	int i, j;
	struct tnode *temp;
	for (i = 0; i < sum - 1; i++)
	{
		for (j = i + 1; j < sum; j++)
		{
			if (forest[i]->weight > forest[j]->weight || (forest[i]->weight == forest[j]->weight && forest[i]->c > forest[j]->c))
			{
				temp = forest[i];
				forest[i] = forest[j];
				forest[j] = temp;
			}
		}
	}
}

void createHTree()
{
	int sum = 0;
	for (int i = 0; i < 128; i++)
	{
		if (Ccount[i] > 0)
		{
			struct tnode *p = (struct tnode *)malloc(sizeof(struct tnode));
			p->c = i;
			p->weight = Ccount[i];
			p->left = p->right = NULL;
			forest[sum] = p;
			sum++;
		}
	}
	sortForest(sum);
	while (sum > 1)
	{
		struct tnode *p = (struct tnode *)malloc(sizeof(struct tnode));
		p->left = forest[0];
		p->right = forest[1];
		p->weight = forest[0]->weight + forest[1]->weight;

		// delete forest[0] and forest[1]
		for (int i = 2; i < sum; i++)
		{
			forest[i - 2] = forest[i];
		}
		sum -= 2;

		// insert p to forest by order
		int i = 0;
		while (i < sum && forest[i]->weight <= p->weight)
		{
			i++;
		}
		for (int j = sum; j > i; j--)
		{
			forest[j] = forest[j - 1];
		}
		forest[i] = p;
		sum++;
	}
	// at the end, forest[0] is the root of the huffman tree
	Root = forest[0];
}

// 【实验步骤2】结束

// 【实验步骤3】开始

// use dfs to get the huffman code of each character
void dfs(struct tnode *p, char *code, int len)
{
	if (p->left == NULL && p->right == NULL)
	{
		code[len] = 0;
		strcpy(HCode[(int)p->c], code);
		return;
	}
	if (p->left != NULL)
	{
		code[len++] = '0';
		dfs(p->left, code, len);
		len--;
	}
	if (p->right != NULL)
	{
		code[len++] = '1';
		dfs(p->right, code, len);
		len--;
	}
}

void makeHCode()
{
	char code[MAXSIZE] = {0};
	int len = 0;
	dfs(Root, code, len);
}

// 【实验步骤3】结束

// 【实验步骤4】开始

void atoHZIP()
{
	char *pc;			  // 用于访问哈夫曼编码中字符串的0、1位
	unsigned char hc = 0; // 用于生成8位二进制位，不需要符号位
	int ch = 0, codeLen = 0;
	fseek(Src, 0, SEEK_SET); // 将文件读写指针移到开头
	do
	{
		ch = fgetc(Src); // 读取文件中的字符
		if (ch == EOF)
			ch = 0;		// 如果文件结束，则将ascii设置0,表示压缩文件结束
		pc = HCode[ch]; // 拿到单个字符的哈夫曼编码
		for (; *pc != '\0'; pc++)
		{								  // 将"110"这样字符串转换为对应的二进制0、1位，存储到hc中
			hc = (hc << 1) | (*pc - '0'); // 根据当前字符'0'、'1'，修改hc的最后一位为二进制0、1
			codeLen++;					  // 记录当前已经存储了多少位有效的二进制位
			if (codeLen == 8)
			{					// 如果存满8位，即生成一个完整字节，则写入文件
				fputc(hc, Obj); // 生成的8位二进制（即一个字节）写入压缩文件
				printf("%x", hc); // 输出最后一个字节的16进制表示
				codeLen = 0;
			}
		}
		if (ch == 0 && codeLen != 0)
		{ // 最后一个字符不满8位，不够一个字节
			while (codeLen++ < 8)
				hc = (hc << 1); // 后面移入若干个0，补满8位
			fputc(hc, Obj);
			printf("%x", hc); // 输出最后一个字节的16进制表示
		}
	} while (ch);
}

// 【实验步骤4】结束

void print1()
{
	int i;
	printf("NUL:1\n");
	for (i = 1; i < 128; i++)
		if (Ccount[i] > 0)
			printf("%c:%d\n", i, Ccount[i]);
}

void print2(struct tnode *p)
{
	if (p != NULL)
	{
		if ((p->left == NULL) && (p->right == NULL))
			switch (p->c)
			{
			case 0:
				printf("NUL ");
				break;
			case ' ':
				printf("SP ");
				break;
			case '\t':
				printf("TAB ");
				break;
			case '\n':
				printf("CR ");
				break;
			default:
				printf("%c ", p->c);
				break;
			}
		print2(p->left);
		print2(p->right);
	}
}

void print3()
{
	int i;

	for (i = 0; i < 128; i++)
	{
		if (HCode[i][0] != 0)
		{
			switch (i)
			{
			case 0:
				printf("NUL:");
				break;
			case ' ':
				printf("SP:");
				break;
			case '\t':
				printf("TAB:");
				break;
			case '\n':
				printf("CR:");
				break;
			default:
				printf("%c:", i);
				break;
			}
			printf("%s\n", HCode[i]);
		}
	}
}

void print4()
{
	long int in_size, out_size;

	fseek(Src, 0, SEEK_END);
	fseek(Obj, 0, SEEK_END);
	in_size = ftell(Src);
	out_size = ftell(Obj);

	printf("\n原文件大小：%ldB\n", in_size);
	printf("压缩后文件大小：%ldB\n", out_size);
	printf("压缩率：%.2f%%\n", (float)(in_size - out_size) * 100 / in_size);
}
