#ifndef __URL_HANDLER_H__



#ifdef __cplusplus             //告诉编译器，这部分代码按C语言的格式进行编译，而不是C++的

extern "C"{

#endif

int dynamic_request_handle(char *url, bool bLinger, char *sResponseStr, int dwStrLen);

bool add_response(char *PWriteBuf, int *pWriteIndex, const char* format, ... );

bool add_status_line(char *PWriteBuf, int *pWriteIndex, int status, const char* title );

bool add_headers(char *pWriteBuf, int *pWriteIndex, int content_len, bool bLinger);

bool add_content_length(char *PWriteBuf, int *pWriteIndex, int content_len );

bool add_linger(char *PWriteBuf, int *pWriteIndex, bool bLinger);

bool add_blank_line(char *PWriteBuf, int *pWriteIndex);

bool add_content(char *PWriteBuf, int *pWriteIndex, const char* content );

#ifdef __cplusplus
}
#endif

#endif
