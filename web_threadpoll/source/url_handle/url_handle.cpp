
#include "url_handler.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <Python.h>

static const char* ok_200_title = "OK";

#define PYTHON_PATH "./source/url_handle/python"

#define WRITE_BUFFER_SIZE 1024

static int python_sort(int *pArray, int ElemNum);
static int split_num(char *pStr, int *pArray, int *pElemNum);


int dynamic_hello(bool bLinger, char *pResponseStr, int *pStrLen)
{
	const char* ok_string = "<html><body>hello</body></html>";
	
	if(true != add_status_line(pResponseStr, pStrLen, 200, ok_200_title ))
	{
		printf("[%s:%d] %s\n", __FUNCTION__, __LINE__, pResponseStr);
		return false;
	}
	if(true != add_headers(pResponseStr, pStrLen, strlen( ok_string ), bLinger))
	{
		return false;
	}
	
	if (true != add_content(pResponseStr, pStrLen, ok_string ))
	{
		return false;
	}

	return true;
}
int dynamic_sort(bool bLinger, char *pDataNum, char *pResponseStr, int *pStrLen)
{
	char sBody[128] = {0};
	int dwArray[64] = {0};
	char sSortedNum[128];
	int dwElemNum = 0;
	int i = 0;
	split_num(pDataNum, dwArray, &dwElemNum);
	if(python_sort(dwArray, dwElemNum) < 0)
	{
		return false;
	}
	
	for(i=0; i < dwElemNum; ++i)
	{
		snprintf(sSortedNum+strlen(sSortedNum), sizeof(sSortedNum), "%d ", dwArray[i]); 
	}
	
	snprintf(sBody, sizeof(sBody),"<html><body>%s</body></html>", sSortedNum);
	if(true != add_status_line(pResponseStr, pStrLen, 200, ok_200_title ))
		return false;
	 
	if(true != add_headers(pResponseStr, pStrLen, strlen( sBody ), bLinger))
		return false;
	
	if (true != add_content(pResponseStr, pStrLen, sBody ))
	{
		printf("[%s:%d] %s\n", __FUNCTION__, __LINE__, pResponseStr);
		return false;
	}
	
	return true;
}
//根据url，进行处理，返回值为html的长度，返回0， 表示失败
int dynamic_request_handle(char *url, bool bLinger, char *pResponseStr, int dwStrLen)
{
	char *pUrlPath = NULL;
	char *pUrlData = NULL;
	char sStr[WRITE_BUFFER_SIZE]={0};
	int dwLen = 0;
	
	printf("[%s:%d] url =%s\n", __FUNCTION__, __LINE__, url);
	if(0 == strcmp(url, "/hello"))
	{
		if(true != dynamic_hello(bLinger, sStr, &dwLen))
		{
			printf("[%s:%d] dynamic_hello failed\n", __FUNCTION__, __LINE__);
			return false;
		}
	}
	else
	{
		if(url=strchr(url,'?'))
		{
			*url = '\0';
			++url;
			pUrlData = strstr(url, "number=");
			if(NULL == pUrlData)
				return 0;
			
			pUrlData = pUrlData + strlen("number=");
			
			if(true != dynamic_sort(bLinger, pUrlData, sStr, &dwLen))
			{
				printf("[%s:%d] dynamic_sort failed\n", __FUNCTION__, __LINE__);
				return false;
			}
		}
	}
	printf("[%s:%d]\n%s\n", __FUNCTION__, __LINE__, sStr);
	if(dwLen <= dwStrLen)
	{
		memcpy(pResponseStr, sStr, dwLen);
		return dwLen;
	}
	return 0;
}

bool add_response(char *pWriteBuf, int *pWriteIndex, const char* format, ... )
{
	if( *pWriteIndex >= WRITE_BUFFER_SIZE )
    {
        return false;
    }
    va_list arg_list;
    va_start( arg_list, format );
    int len = vsnprintf( pWriteBuf + *pWriteIndex, WRITE_BUFFER_SIZE - 1 - *pWriteIndex, format, arg_list );
    if( len >= ( WRITE_BUFFER_SIZE - 1 - *pWriteIndex ) )
    {
        return false;
    }
    *pWriteIndex += len;
    va_end( arg_list );
    return true;
}

bool add_status_line(char *pWriteBuf, int *pWriteIndex, int status, const char* title )
{
    return add_response(pWriteBuf, pWriteIndex, "%s %d %s\r\n", "HTTP/1.1", status, title );
}

bool add_headers(char *pWriteBuf, int *pWriteIndex, int content_len, bool bLinger)
{
    if(true != add_content_length(pWriteBuf, pWriteIndex, content_len ))
		return false;
    if(true != add_linger(pWriteBuf, pWriteIndex, bLinger))
		return false;
    if(true != add_blank_line(pWriteBuf, pWriteIndex))
		return false;
	
	return true;
}

bool add_content_length(char *pWriteBuf, int *pWriteIndex, int content_len )
{
    return add_response(pWriteBuf, pWriteIndex, "Content-Length: %d\r\n", content_len );
}

bool add_linger(char *pWriteBuf, int *pWriteIndex, bool bLinger)
{
    return add_response(pWriteBuf, pWriteIndex, "Connection: %s\r\n", ( bLinger == true ) ? "keep-alive" : "close" );
}

bool add_blank_line(char *pWriteBuf, int *pWriteIndex)
{
    return add_response(pWriteBuf, pWriteIndex, "%s", "\r\n" );
}

bool add_content(char *pWriteBuf, int *pWriteIndex, const char* content )
{
    return add_response(pWriteBuf, pWriteIndex, "%s", content );
}





int python_sort(int *pArray, int ElemNum)
{
	char sPythonPath[128]={0};
	/*Pass by List: Transform an C Array to Python List*/
    //int pArray[] = {1, 4, 6, 8, 1, 0};
 
	//初始化，载入python的扩展模块
    Py_Initialize();
    //判断初始化是否成功
    if(!Py_IsInitialized())
    {
        printf("Python init failed!\n");
        return -1;
    }
     //导入当前路径
    PyRun_SimpleString("import sys");
	snprintf(sPythonPath, sizeof(sPythonPath), "sys.path.append(\'%s\')", PYTHON_PATH);
    PyRun_SimpleString(sPythonPath);
    
	PyObject * pModule = NULL;
	PyObject * pFunc = NULL;
	PyObject *pDict = NULL;
	PyObject *pReturn = NULL;
 
	pModule = PyImport_ImportModule("sort");
	if(!pModule)
    {
        printf("Load sort.py failed!\n");
        return -1;
    }
	pDict = PyModule_GetDict(pModule); 
	if(!pDict)
    {
        printf("Can't find dict in sort!\n");
        return -1;
    }
    pFunc = PyDict_GetItemString(pDict, "func_sort");
    if(!pFunc || !PyCallable_Check(pFunc))
    {
        printf("Can't find function func_sort!\n");
        return -1;
    }
    printf("C Array Pass Into The Python List:\n");
	
    PyObject *PyList  = PyList_New(ElemNum);//定义一个与数组等长的PyList对象数组
    PyObject *ArgList = PyTuple_New(1);//定义一个Tuple对象，Tuple对象的长度与Python函数参数个数一直，上面Python参数个数为1，所以这里给的长度为1
    for(int i = 0; i < PyList_Size(PyList); i++)
    {
        //PyList_SetItem(PyList,i, PyFloat_FromDouble(pArray[i]));//给PyList对象的每个元素赋值
        PyList_SetItem(PyList,i, PyInt_FromLong(pArray[i]));//给PyList对象的每个元素赋值
    }
    PyTuple_SetItem(ArgList, 0, PyList);//将PyList对象放入PyTuple对象中
    pReturn=PyObject_CallObject(pFunc, ArgList);//调用函数，完成传递
	
	if(PyList_Check(pReturn))
	{ //检查是否为List对象
		printf("返回的结果result： ");
		int SizeOfList = PyList_Size(pReturn);//List对象的大小，这里SizeOfList = 
		for(int i = 0; i < SizeOfList; i++){
			PyObject *Item = PyList_GetItem(pReturn, i);//获取List对象中的每一个元素
            int result;
			PyArg_Parse(Item, "i", &result);//i表示转换成int型变量
			printf("%d ",result);
			pArray[i] = result;
			Py_DECREF(Item); //释放空间
         }
		printf("\n");
	}else{
		printf("Not a List\n");
	}
 
	Py_Finalize();
	return 0;
}

int split_num(char *pStr, int *pArray, int *pElemNum)
{
	int j,in = 0;
	char *p[64];
	char *buf = pStr;
	char *outer_ptr = NULL;
	char *inner_ptr = NULL;
	while ((p[in] = strtok_r(buf, ",", &outer_ptr)) != NULL)
	{
	buf = p[in];
	while ((p[in] = strtok_r(buf, " ", &inner_ptr)) != NULL)
	{
	  in++;
	  buf = NULL;
	}
	buf = NULL;
	}
	printf("Here we have %d strings\n", in);
	for (j = 0; j < in; j++)
	{
		printf(">%s<\n", p[j]);
		pArray[j] = atoi(p[j]);
	}
	*pElemNum = in;
	
	return 0;
}