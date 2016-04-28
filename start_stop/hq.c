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

//超级全局变量的初始化回调函数
zend_bool php_super_autoglobal_callback(char *name, uint name_len TSRMLS_DC)
{
    zval *super_global;
    int i;
    MAKE_STD_ZVAL(super_global);
    array_init(super_global);
    for(i = 0; i < 10; i++) {
        add_next_index_long(super_global, i);
    }
    ZEND_SET_SYMBOL(&EG(symbol_table), "_SUPER_GLOBAL", super_global);
    return 0;
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

	//注册超级全局变量
    //zend_register_auto_global("_SUPER_GLOBAL", sizeof("_SUPER_GLOBAL") - 1, 0, php_super_autoglobal_callback TSRMLS_CC);
    zend_register_auto_global("_SUPER_GLOBAL", sizeof("_SUPER_GLOBAL") - 1
	, 0 //HQ：这个参数在教程里面是没有的，可能是版本不同，看了PHP5的源码之后，自己加的，具体的含义不是很清楚，但是看代码感觉应该是0
	#ifdef ZEND_ENGINE_2
        , php_super_autoglobal_callback
	#endif
    TSRMLS_CC);
	
    return SUCCESS;
}
 
PHP_MSHUTDOWN_FUNCTION(hq) 
{
	php_printf("MSHUTDOWN!\n");

//这句如果不加上，直接coredump。
//在目前的教程中均没有提及，在stackoverflow上找到的答案:
//http://stackoverflow.com/questions/10200193/php-module-crashes-on-ts-allocate-dtor
#ifdef ZTS
    ts_free_id(hq_globals_id); 
#endif

//注意，这里是ndef，因为如果是ZTS的话，dtor是自动的
#ifndef ZTS
    php_hq_globals_dtor(&hq_globals TSRMLS_CC);
#endif
    return SUCCESS;
}
 
PHP_RINIT_FUNCTION(hq) 
{
	php_printf("RINIT!\n");
	//为了兼容PHP4，要在每个脚本开始的时候去调用初始化回调函数
#ifndef ZEND_ENGINE_2
    php_super_autoglobal_callback("_SUPER_GLOBAL",
                    sizeof("_SUPER_GLOBAL") - 1
                    TSRMLS_CC);
#endif
    return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(hq) 
{
	php_printf("RSHUTDOWN!\n");
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
    ZEND_FE(hq_counter,      NULL)
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
