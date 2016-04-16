//hq.c
#include "php_hq.h"

ZEND_FUNCTION(hq_hello)
{
    php_printf("Hello World!\n");
}

//接受一个普通的整形
ZEND_FUNCTION(new_getlong) {
 
    long foo;
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"l", &foo) == FAILURE)
    {
        RETURN_NULL();
    }
    php_printf("The integer value of the parameter is: %ld\n", foo);
    RETURN_TRUE;
}

//接受字符串
ZEND_FUNCTION(get_str) {
    char *name;
    int name_len;
 
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",&name, &name_len) == FAILURE)
    {
        RETURN_NULL();
    }
    php_printf("Hello ");
    PHPWRITE(name, name_len);
    php_printf("!\n");
}

//接受可选参数
ZEND_FUNCTION(new_hello_world) {
    char *name;
    int name_len;
    char *greeting = "Mr./Mrs.";
    int greeting_len = sizeof("Mr./Mrs.") - 1;
 
 
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|s",
      &name, &name_len, &greeting, &greeting_len) == FAILURE) {
        RETURN_NULL();
    }
    php_printf("Hello ");
    PHPWRITE(greeting, greeting_len);
    php_printf(" ");
    PHPWRITE(name, name_len);
    php_printf("!\n");
}

static zend_function_entry hq_functions[] = {
    ZEND_FE(hq_hello,        NULL)
	ZEND_FE(new_getlong,   NULL)
	ZEND_FE(get_str,   NULL)
    ZEND_FE(new_hello_world,   NULL)

    { NULL, NULL, NULL }
};

//module entry
zend_module_entry hq_module_entry = {

#if ZEND_MODULE_API_NO >= 20010901
     STANDARD_MODULE_HEADER,
#endif
    "hq", //这个地方是扩展名称，往往我们会在这个地方使用一个宏。
    hq_functions, /* Functions */
    NULL, /* MINIT */
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
