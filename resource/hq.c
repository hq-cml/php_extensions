//hq.c
#include "php_hq.h"

ZEND_FUNCTION(hq_hello)
{
    php_printf("Hello World!\n");
}

//一个自定义类型作为资源类型
typedef struct _php_my_resource_type
{
    char *filename;
    FILE *fp;
}php_my_resource_type;

//普通文件资源的析构函数
static void php_my_resource_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	php_printf("释放了一个文件资源~\n");
    php_my_resource_type *fdata = (php_my_resource_type*)rsrc->ptr;
    fclose(fdata->fp);
    efree(fdata->filename); //注意，这里是e打头！
    efree(fdata);
}

//持久文件资源的析构函数
static void php_my_resource_persistent_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	php_printf("释放了一个永久资源~\n");
    php_my_resource_type *fdata = (php_my_resource_type*)rsrc->ptr;
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
	php_my_resource_type *fdata;
    FILE* fp;
    char* filename, *mode;
    int filename_len, mode_len;
	zend_bool persist = 0; //typedef unsigned char zend_bool;
	        
	char* hash_key;
    int hash_key_len;
	zend_rsrc_list_entry *existing_file;
	
	//可选参数，是否持久化
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss|b",&filename, &filename_len,&mode, &mode_len,&persist) == FAILURE)
    {
        RETURN_NULL();
    }
	
    if(!filename_len || !mode_len)
    {
        php_error_docref(NULL TSRMLS_CC, E_WARNING,"Invalid filename or mode length");
        RETURN_FALSE;
    }
	
    //看看persistent_list中是否已经存在永久资源，如果已经存在就直接使用，不再创建
    hash_key_len = spprintf(&hash_key, 0,"my_persistent_resource:%s:%s", filename, mode);
    if(zend_hash_find(&EG(persistent_list), hash_key,hash_key_len + 1, (void**)&existing_file) == SUCCESS)
    {
		php_printf("找到存在一个持久资源，so，直接使用啦~\n");
        //存在一个，则直接注册到普通资源列表
		//TODO 实际场景中，此处应该验证一下这个持久资源是否还有效，比如mysql连接之类，如果长时间不使用，可能服务端会关闭连接导致资源失效
        ZEND_REGISTER_RESOURCE(return_value, existing_file->ptr, le_resource_persistent_id);
        efree(hash_key);
        return;
    }
	else
	{
		//防止泄露~
		efree(hash_key);
	}
	
    fp = fopen(filename, mode);
    if(!fp)
    {
        php_error_docref(NULL TSRMLS_CC, E_WARNING,"Unable to open %s using mode %s",filename, mode);
        RETURN_FALSE;
    }
	
	if(!persist)
	{
		php_printf("创建了一个普通资源~\n");
		//将fdata添加到资源池中HashTable去，并标记它为le_resource_id类型的。这样的话，析构的时候就知道用哪个析构函数析构这个资源了
		//此外，把此资源在其中对应的数字Key赋给return_value。
		fdata = emalloc(sizeof(php_my_resource_type));
		fdata->fp = fp;
		fdata->filename = estrndup(filename, filename_len);
		ZEND_REGISTER_RESOURCE(return_value, fdata, le_resource_id);	
	}
	else
	{
		php_printf("创建了一个永久资源~\n");
		zend_rsrc_list_entry le;

 
        fdata =pemalloc(sizeof(php_my_resource_type),1);
        fdata->filename = pemalloc(filename_len + 1, 1);
        memcpy(fdata->filename, filename, filename_len + 1);
        fdata->fp = fp;
         
        //在EG(regular_list中存一份)
        ZEND_REGISTER_RESOURCE(return_value, fdata, le_resource_persistent_id);
 
        //在EG(persistent_list)中再存一份
        le.type = le_resource_persistent_id;
        le.ptr = fdata;
        hash_key_len = spprintf(&hash_key, 0,"my_persistent_resource:%s:%s", filename, mode);
        zend_hash_update(&EG(persistent_list),hash_key, hash_key_len + 1,(void*)&le, sizeof(zend_rsrc_list_entry), NULL);
        efree(hash_key);
	}
}

//资源的使用
ZEND_FUNCTION(my_fwrite)
{
	php_my_resource_type *fdata;
    zval *file_resource; 
    char *data;
    int data_len;
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs",&file_resource, &data, &data_len) == FAILURE )
    {
        RETURN_NULL();
    }
    /* Use the zval* to verify the resource type and retrieve its pointer from the lookup table */
    //ZEND_FETCH_RESOURCE(fdata,php_my_resource_type*,&file_resource,-1,PHP_MY_FIRST_RES_TYPE_NAME,le_resource_id);
    
	//兼容普通和持久两种类型资源
	ZEND_FETCH_RESOURCE2(fdata,php_my_resource_type*,&file_resource,-1,PHP_MY_FIRST_RES_TYPE_NAME,le_resource_id,le_resource_persistent_id);
	
	//如果有更多种类型，可以直接用zend_fetch_resource，即把宏ZEND_FETCH_RESOURCE展开
	//fp = (FILE*) zend_fetch_resource(
	//	&file_descriptor TSRMLS_CC,
	//	-1,
	//	PHP_SAMPLE_DESCRIPTOR_RES_NAME,
	//	NULL,
	//	3,
	//	le_sample_descriptor,
	//	le_sample_descriptor_persist,
	//	le_sample_othertype
	//);
	//ZEND_VERIFY_RESOURCE(fp);
	
    /* Write the data, and
     * return the number of bytes which were
     * successfully written to the file */
    RETURN_LONG(fwrite(data, 1, data_len, fdata->fp));
}

//关闭文件资源
PHP_FUNCTION(my_fclose)
{
    zval *file_resource;
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r",&file_resource) == FAILURE ) {
        RETURN_NULL();
    }
     
    /* Force the resource into self-destruct mode，处罚删除回调 */
    zend_hash_index_del(&EG(regular_list),Z_RESVAL_P(file_resource));
    RETURN_TRUE;
}

//获得文件名
PHP_FUNCTION(get_fname)
{
    php_my_resource_type *fdata;
    zval *file_resource;
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r",&file_resource) == FAILURE )
    {
        RETURN_NULL();
    }
    ZEND_FETCH_RESOURCE(fdata, php_my_resource_type*,&file_resource, -1, PHP_MY_FIRST_RES_TYPE_NAME, le_resource_id);
    RETURN_STRING(fdata->filename, 1);
}

static zend_function_entry hq_functions[] = {
	ZEND_FE(my_fopen,        NULL)
	ZEND_FE(my_fwrite,       NULL)
	ZEND_FE(my_fclose,       NULL)
	ZEND_FE(get_fname,       NULL)
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
