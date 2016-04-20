//hq.c
#include "php_hq.h"

ZEND_FUNCTION(hq_hello)
{
    php_printf("Hello World!\n");
}

//类的句柄
zend_class_entry *myclass_ce;

//自定义类的方法列表
static zend_function_entry myclass_method[] = {
    { NULL, NULL, NULL }
};
 
//MINIT函数，则侧一个类
ZEND_MINIT_FUNCTION(my_minit_func)
{
    zend_class_entry ce;
     
    //注册一个类，"myclass"是这个类的名称。
    INIT_CLASS_ENTRY(ce, "myclass",myclass_method);
    myclass_ce = zend_register_internal_class(&ce TSRMLS_CC);
    return SUCCESS;
}

static zend_function_entry hq_functions[] = {
	ZEND_FE(hq_hello,        NULL)
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
