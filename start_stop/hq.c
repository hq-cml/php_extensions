//hq.c
#include "php_hq.h"

ZEND_FUNCTION(hq_hello)
{
    php_printf("Hello World!\n");
}

PHP_MINIT_FUNCTION(hq)
{
	REGISTER_STRING_CONSTANT("HQ_VERSION", "1.0", CONST_CS | CONST_PERSISTENT);
    return SUCCESS;
}
 
PHP_MSHUTDOWN_FUNCTION(hq) 
{
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
