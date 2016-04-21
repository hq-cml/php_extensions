//hq.c
#include "php_hq.h"

ZEND_FUNCTION(hq_hello)
{
    php_printf("Hello World!\n");
}

//类的句柄
zend_class_entry *myclass_ce;
//接口句柄
zend_class_entry *myinterface_ce;

//定义类方法：ZEND_METHOD，注意区别上面那个叫ZEND_FUNCTION
ZEND_METHOD( myclass , public_method )
{
    php_printf("我是public类型的方法\n");
}
 
ZEND_METHOD( myclass , __construct )
{
    php_printf("我是__construct方法\n");
}

//自定义类的方法列表
static zend_function_entry myclass_method[] = {
	//注意这个叫ZEND_ME，普通扩展函数是ZEND_FE
	ZEND_ME(myclass,    public_method,  NULL,   ZEND_ACC_PUBLIC)
    ZEND_ME(myclass,    __construct,    NULL,   ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
    { NULL, NULL, NULL }
};
//自定义接口的方法列表
static zend_function_entry myinterface_method[]={
    ZEND_ABSTRACT_ME(myinterface, hello, NULL) //注意这里的null指的是arginfo
    {NULL,NULL,NULL}
};

//MINIT函数，则侧一个类
ZEND_MINIT_FUNCTION(my_minit_func)
{
    zend_class_entry ce1;
    zend_class_entry ce2;
     
    //注册一个类，"myclass"是这个类的名称。myclass_method是类的方法列表
    INIT_CLASS_ENTRY(ce1, "myclass",myclass_method);
    myclass_ce = zend_register_internal_class(&ce1 TSRMLS_CC);
	
	//定义属性:$public_var, 默认值null，权限public
    zend_declare_property_null(myclass_ce, "public_var", strlen("public_var"), ZEND_ACC_PUBLIC TSRMLS_CC);
	
	//定义常量
	zend_declare_class_constant_long(myclass_ce, "MY_CONST" , strlen("MY_CONST"), 10 TSRMLS_CC);
	
	//注册一个接口    
	INIT_CLASS_ENTRY(ce2, "myinterface", myinterface_method);
    myinterface_ce = zend_register_internal_interface(&ce2 TSRMLS_CC);
	
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
