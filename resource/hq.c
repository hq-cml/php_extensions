//hq.c
#include "php_hq.h"

ZEND_FUNCTION(hq_hello)
{
    php_printf("Hello World!\n");
}

//自定义一个资源
#define PHP_MY_FIRST_RES_TYPE_NAME "我的第一个资源" //资源类型的名称
static int le_resource_id;                         //静态整形变量存储资源id（或者说资源对应析构函数的id）
ZEND_MINIT_FUNCTION(my_minit_func)
{
    //在析构列表中注册一个析构函数，并且得到对应的id返回给全局变量le_resource_id
    le_resource_id = zend_register_list_destructors_ex(NULL, NULL, PHP_MY_FIRST_RES_TYPE_NAME,module_number);
    return SUCCESS;
}


static zend_function_entry hq_functions[] = {

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
