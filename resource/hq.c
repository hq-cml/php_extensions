//hq.c
#include "php_hq.h"

ZEND_FUNCTION(hq_hello)
{
    php_printf("Hello World!\n");
}

//一个更为复杂的自定义类型作为资源类型
typedef struct _php_my_resource_persistent_data
{
    char *filename;
    FILE *fp;
}php_my_resource_persistent_data;

//文件资源的析构函数
static void php_my_resource_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	php_printf("释放了一个文件资源~\n");
    FILE *fp = (FILE*)rsrc->ptr;
    fclose(fp);
}

//复杂资源的析构函数
static void php_my_resource_persistent_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	php_printf("释放了一个复杂资源~\n");
    php_my_resource_persistent_data *fdata = (php_my_resource_persistent_data*)rsrc->ptr;
    fclose(fdata->fp);
    pefree(fdata->filename, 1); //注意，这里是pe打头！
    pefree(fdata, 1);
}

//自定义一个资源
#define PHP_MY_FIRST_RES_TYPE_NAME "我的第一个资源" //资源类型的名称
static int le_resource_id;                          //静态整形变量存储资源id（或者说资源对应析构函数的id）
static int le_resource_persistent_id;               //持久资源类型id
ZEND_MINIT_FUNCTION(my_minit_func)
{
    //在析构列表中注册一个析构函数，并且得到对应的id返回给全局变量le_resource_id
    le_resource_id = zend_register_list_destructors_ex(php_my_resource_dtor, NULL, PHP_MY_FIRST_RES_TYPE_NAME,module_number);
	php_printf("自定义了一个资源类型~\n");
	
	//在析构列表中注册一个永久资源的析构函数，注意第一个参数是NULL，第二个参数是析构函数！
    le_resource_persistent_id = zend_register_list_destructors_ex(NULL, php_my_resource_persistent_dtor, PHP_MY_FIRST_RES_TYPE_NAME,module_number);
	php_printf("自定义了一个永久资源类型~\n");
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

//资源的使用
ZEND_FUNCTION(my_fwrite)
{
    FILE *fp;
    zval *file_resource; 
    char *data;
    int data_len;
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs",&file_resource, &data, &data_len) == FAILURE )
    {
        RETURN_NULL();
    }
    /* Use the zval* to verify the resource type and retrieve its pointer from the lookup table */
    ZEND_FETCH_RESOURCE(fp,FILE*,&file_resource,-1,PHP_MY_FIRST_RES_TYPE_NAME,le_resource_id);
     
    /* Write the data, and
     * return the number of bytes which were
     * successfully written to the file */
    RETURN_LONG(fwrite(data, 1, data_len, fp));
}

//关闭文件资源
PHP_FUNCTION(my_fclose)
{
    FILE *fp;
    zval *file_resource;
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r",&file_resource) == FAILURE ) {
        RETURN_NULL();
    }
     
    /* Force the resource into self-destruct mode，处罚删除回调 */
    zend_hash_index_del(&EG(regular_list),Z_RESVAL_P(file_resource));
    RETURN_TRUE;
}

static zend_function_entry hq_functions[] = {
	ZEND_FE(my_fopen,        NULL)
	ZEND_FE(my_fwrite,       NULL)
	ZEND_FE(my_fclose,       NULL)
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
