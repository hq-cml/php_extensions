//hq.c
#include "php_hq.h"

ZEND_FUNCTION(hq_hello)
{
    php_printf("Hello World!\n");
}

//定义全局变量(声明在头文件中)
ZEND_DECLARE_MODULE_GLOBALS(hq);

//全局变量的初始化函数
static void php_hq_globals_ctor(zend_hq_globals *hq_globals TSRMLS_DC)
{
    /* Initialize a new zend_hq_globals struct During thread spin-up */
     hq_globals->counter = 0;
	 php_printf("初始化全局变量!\n");
}

//全局变量的析构（此处为空因为没有额外的emalloc数据）
static void php_hq_globals_dtor(zend_hq_globals *hq_globals TSRMLS_DC)
{
	php_printf("释放全局变量!\n");
    /* Any resources allocated during initialization May be freed here */
}

//全局变量的获取与使用
PHP_FUNCTION(hq_counter) {
#ifdef ZTS
        RETURN_LONG(++TSRMG(hq_globals_id, \
                zend_hq_globals*, counter));
#else
        /* non-ZTS */
        RETURN_LONG(++hq_globals.counter);
#endif
    //RETURN_LONG(++hq_G(counter));
}


PHP_MINIT_FUNCTION(hq)
{
	php_printf("MINIT!\n");
	REGISTER_STRING_CONSTANT("HQ_VERSION", "1.0", CONST_CS | CONST_PERSISTENT);
	
#ifdef ZTS
    ts_allocate_id(&hq_globals_id, sizeof(zend_hq_globals),(ts_allocate_ctor)php_hq_globals_ctor,(ts_allocate_dtor)php_hq_globals_dtor);
    //ts_allocate_id(&hq_globals_id, sizeof(zend_hq_globals),(ts_allocate_ctor)php_hq_globals_ctor,NULL);
#else
    php_hq_globals_ctor(&hq_globals TSRMLS_CC);
#endif

    return SUCCESS;
}
 
PHP_MSHUTDOWN_FUNCTION(hq) 
{
#ifndef ZTS
    php_hq_globals_dtor(&hq_globals TSRMLS_CC);
#endif
    return SUCCESS;
}
 
PHP_RINIT_FUNCTION(hq) 
{
    return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(hq) 
{
    return SUCCESS;
}
 
PHP_MINFO_FUNCTION(hq) {
	php_info_print_table_start();
    php_info_print_table_row(2, "hq Module", "enabled");
    php_info_print_table_row(2, "version", "1.0");
    php_info_print_table_end();
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
    hq_functions,      /* Functions */
    PHP_MINIT(hq),     /* MINIT */
    PHP_MSHUTDOWN(hq), /* MSHUTDOWN */
    PHP_RINIT(hq),     /* RINIT */
    PHP_RSHUTDOWN(hq), /* RSHUTDOWN */
    PHP_MINFO(hq),     /* MINFO */
#if ZEND_MODULE_API_NO >= 20010901
    "2.1", //这个地方是我们扩展的版本
#endif
    STANDARD_MODULE_PROPERTIES
};
 
#ifdef COMPILE_DL_HQ
ZEND_GET_MODULE(hq)
#endif
