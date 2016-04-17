//hq.c
#include "php_hq.h"

ZEND_FUNCTION(hq_hello)
{
    php_printf("Hello World!\n");
}

//文件资源的析构函数
static void php_my_resource_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	php_printf("释放了一个资源~\n");
    FILE *fp = (FILE*)rsrc->ptr;
    fclose(fp);
}

//自定义一个资源
#define PHP_MY_FIRST_RES_TYPE_NAME "我的第一个资源" //资源类型的名称
static int le_resource_id;                         //静态整形变量存储资源id（或者说资源对应析构函数的id）
ZEND_MINIT_FUNCTION(my_minit_func)
{
    //在析构列表中注册一个析构函数，并且得到对应的id返回给全局变量le_resource_id
    le_resource_id = zend_register_list_destructors_ex(php_my_resource_dtor, NULL, PHP_MY_FIRST_RES_TYPE_NAME,module_number);
	php_printf("自定义了一个资源类型~\n");
    return SUCCESS;
}

//创建资源
PHP_FUNCTION(my_fopen)
{
    FILE* fp;
    char* filename, *mode;
    int filename_len, mode_len;
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss",&filename, &filename_len,&mode, &mode_len) == FAILURE)
    {
        RETURN_NULL();
    }
    if(!filename_len || !mode_len)
    {
        php_error_docref(NULL TSRMLS_CC, E_WARNING,"Invalid filename or mode length");
        RETURN_FALSE;
    }
    fp = fopen(filename, mode);
    if(!fp)
    {
        php_error_docref(NULL TSRMLS_CC, E_WARNING,"Unable to open %s using mode %s",filename, mode);
        RETURN_FALSE;
    }
	php_printf("创建了一个资源~\n");
    //将fp添加到资源池中HashTable去，并标记它为le_sample_descriptor类型的。这样的话，析构的时候就知道用哪个析构函数析构这个资源了
	//此外，把此资源在其中对应的数字Key赋给return_value。
    ZEND_REGISTER_RESOURCE(return_value, fp, le_resource_id);
}

static zend_function_entry hq_functions[] = {
	ZEND_FE(my_fopen,        NULL)
    { NULL, NULL, NULL }
};

//module entry
zend_module_entry hq_module_entry = {

#if ZEND_MODULE_API_NO >= 20010901
     STANDARD_MODULE_HEADER,
#endif
    "hq", //这个地方是扩展名称，往往我们会在这个地方使用一个宏。
    hq_functions, /* Functions */
		ZEND_MINIT(my_minit_func), /* MINIT */
    NULL, /* MSHUTDOWN */
    NULL, /* RINIT */
    NULL, /* RSHUTDOWN */
    NULL, /* MINFO */
#if ZEND_MODULE_API_NO >= 20010901
    "2.1", //这个地方是我们扩展的版本
#endif
    STANDARD_MODULE_PROPERTIES
};
 
#ifdef COMPILE_DL_HQ
ZEND_GET_MODULE(hq)
#endif
