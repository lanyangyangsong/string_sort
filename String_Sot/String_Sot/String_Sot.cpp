// String_Sot.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <string>

/***************************************************************************************************************************
 输入两个字符串，字符串1为原始字符串，字符串2为待检查字符串，检查字符串2与字符串1的差异并输出不同，字符串1和字符串2用','分隔
 字符串1和2的总长度限定在10000以内。
 ***************************************************************************************************************************/

#define MAX_STRING_LENGTH (10000)

typedef struct T_OMSYS_STRING
{
	char *pString;
	T_OMSYS_STRING *pNext;
} OMSYS_STRING;


bool Find_String(OMSYS_STRING *pStringHead, char *pString)
{
	if ((pStringHead == NULL) || (pString == NULL))
	{
		return false;
	}

	OMSYS_STRING *pStr = pStringHead;
	while (pStr != NULL)
	{
		//if ((memcmp(pStr->pString, pString, strlen(pString)) != 0) || (strlen(pStr->pString) != strlen(pString)))
		if (strncmp(pStr->pString, pString, strlen(pString)) != 0)
		{
			pStr = pStr->pNext;
		}
		else
		{
			return true;
		}
	}

	return false;
}


/* 实现尾部插入，并返回LIST首部指针，插入的时候需要去重 */
OMSYS_STRING *Insert_String(OMSYS_STRING *pStringHead, char *pString, int strLength)
{
	if (pString == NULL)
	{
		return pStringHead;
	}

	if (Find_String(pStringHead, pString) == true)
	{
		/* 去重 */
		return pStringHead;
	}

	OMSYS_STRING *pTemp = (OMSYS_STRING *)malloc(sizeof(OMSYS_STRING));
	if (pTemp == NULL)
	{
		return pStringHead;
	}
	char *pTempString = (char *)malloc(sizeof(char)*(strLength + 1));
	if (pTempString == NULL)
	{
		free(pTemp);
		return pStringHead;
	}
	memcpy(pTempString, pString, strLength);
	*(pTempString + strLength) = '\0';
	pTemp->pString = pTempString;
	pTemp->pNext = NULL;

	OMSYS_STRING *pStr = pStringHead;
	if (pStr != NULL)
	{
		while (pStr->pNext != NULL)
		{
			pStr = pStr->pNext;
		}
		pStr->pNext = pTemp;
	}
	else
	{
		pStringHead = pTemp;
	}

	return pStringHead;
}

void Free_String_Node(OMSYS_STRING *pString)
{
	if (pString != NULL)
	{
		if (pString->pString != NULL)
		{
			free(pString->pString);
			pString->pString = NULL;
		}
		pString->pNext = NULL;
		free(pString);
	}

return;
}

void Destory_String_List(OMSYS_STRING *pHead)
{
	OMSYS_STRING *pTemp = pHead;

	while (pTemp != NULL)
	{
		pHead = pTemp->pNext;
		Free_String_Node(pTemp);
		pTemp = pHead;
	}
	return;
}


void Delete_String(OMSYS_STRING *pStringHead, char *pString)
{
	if ((pStringHead == NULL) || (pString == NULL))
	{
		return;
	}

	OMSYS_STRING *pStr = pStringHead;
	//if ((memcmp(pStr->pString, pString, strlen(pStr->pString)) == 0) && (strlen(pStr->pString) == strlen(pString)))
	if (strncmp(pStr->pString, pString, strlen(pString)) != 0)
	{
		/* 删除的是字符链表首部,释放节点首部内存，修改Head指针 */
		pStr = pStringHead->pNext;
		Free_String_Node(pStringHead);
		pStringHead = pStr;
		return;
	}

	OMSYS_STRING *pStrPrev = pStringHead;
	while (pStr != NULL)
	{
		if (memcmp(pStr->pString, pString, strlen(pString)) != 0)
		{
			pStrPrev = pStr;
			pStr = pStr->pNext;
		}
		else
		{
			pStrPrev->pNext = pStr->pNext;
			Free_String_Node(pStr);
			return;
		}
	}

	return;
}


OMSYS_STRING * Construct_String_List(char *pString)
{
	if (pString == NULL)
	{
		return NULL;
	}

	OMSYS_STRING *pHead = NULL;
	char *pTemp = pString;
	char *pTempBegin = pString;
	int stringLength = 0;
	while (*pTemp != '\0')
	{
		if ((*pTemp == ',') || (*(pTemp + 1) == '\0'))
		{
			/* 本字符为非，且是最后一个字符串需要特殊处理，进行加长，防止最后一个字符漏拷贝 */
			if ((*pTemp != ',') && (*(pTemp + 1) == '\0'))
			{
				stringLength += 1;
			}

#if 0
			/* 遇到，进行分隔 */
			char *pSplitString = (char *)malloc(sizeof(char)*(stringLength + 1));
			if (pSplitString == NULL)
			{
				return NULL;
			}

			memcpy(pSplitString, pTempBegin, sizeof(char)*stringLength);
			*(pSplitString + stringLength) = '\0';
#endif

			pHead = Insert_String(pHead, pTempBegin, stringLength);

			stringLength = 0;
			pTempBegin = pTemp + 1; /* 指向分隔符下一个字符 */
		}
		else
		{
			stringLength++;   /* 计算待拷贝字符串长度 */
		}
		pTemp++;
	}

	return pHead;
}


void Compare_String_List(OMSYS_STRING *pStringExample, OMSYS_STRING *pStringToCompare,
	OMSYS_STRING **pStringLeak, OMSYS_STRING **pStringAdd, OMSYS_STRING **pStringSame)
{
	OMSYS_STRING *pExample = pStringExample;
	while (pExample != NULL)
	{
		if (Find_String(pStringToCompare, pExample->pString))
		{
			*pStringSame = Insert_String(*pStringSame, pExample->pString, strlen(pExample->pString));
		}
		else
		{
			*pStringLeak = Insert_String(*pStringLeak, pExample->pString, strlen(pExample->pString));
		}
		pExample = pExample->pNext;
	}

	OMSYS_STRING *pCompare = pStringToCompare;
	while (pCompare != NULL)
	{
		if (Find_String(pStringExample, pCompare->pString))
		{
		}
		else
		{
			*pStringAdd = Insert_String(*pStringAdd, pCompare->pString, strlen(pCompare->pString));
		}
		pCompare = pCompare->pNext;
	}

}


void Print_String_List(OMSYS_STRING *pHead)
{	
	if (pHead == NULL)
	{
		printf("空链表！\n");
		return;
	}

	OMSYS_STRING *pTemp = pHead;
	
	int i = 0;
	
	while (pTemp != NULL)
	{
		i++;
		printf("节点<%u>， 字符串：%s\n", i, pTemp->pString);
		pTemp = pTemp->pNext;
	}

}

void Compare_Input(char *stringExample, char *stringToCompare)
{
	OMSYS_STRING *pStringExample = Construct_String_List(stringExample);
	OMSYS_STRING *pStringToCompare = Construct_String_List(stringToCompare);

	OMSYS_STRING *pStringLeak = NULL;
	OMSYS_STRING *pStringAdd = NULL;
	OMSYS_STRING *pStringSame = NULL;

	Compare_String_List(pStringExample, pStringToCompare, &pStringLeak, &pStringAdd, &pStringSame);

	printf("===================原始字符串1===================\n");
	Print_String_List(pStringExample);
	printf("===================原始字符串2===================\n");
	Print_String_List(pStringToCompare);

	printf("===================字符串2比1少的有下面这些===================\n");
	Print_String_List(pStringLeak);
	printf("===================字符串2比1多的有下面这些===================\n");
	Print_String_List(pStringAdd);
	printf("===================字符串2比1相同的有下面这些===================\n");
	Print_String_List(pStringSame);


	Destory_String_List(pStringExample);
	Destory_String_List(pStringToCompare);
	Destory_String_List(pStringLeak);
	Destory_String_List(pStringAdd);
	Destory_String_List(pStringSame);

}

int main()
{
//	char stringExample[] = "ATN-908C,ATN-910B,ATN-950C,ATN-908C,ATN-990B,";
//	char stringToCompare[] = "ATN-940B,ATN-908C,ATN-950C,ATN-908C,ATN-910B,";

	char stringExample[MAX_STRING_LENGTH] = { 0 };
	char stringToCompare[MAX_STRING_LENGTH] = { 0 };

	printf("请输入两个字符串进行比较！\n字符串1：\n");
	scanf_s("%s", stringExample, MAX_STRING_LENGTH-1);
	printf("字符串2：\n");
	scanf_s("%s", stringToCompare, MAX_STRING_LENGTH-1);

	printf("stringExample :\n%s\n", stringExample);
	printf("stringToCompare :\n%s\n", stringToCompare);


	Compare_Input(stringExample, stringToCompare);
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
