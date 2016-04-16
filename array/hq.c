//hq.c
#include "php_hq.h"

ZEND_FUNCTION(hq_hello)
{
    php_printf("Hello World!\n");
}


ZEND_FUNCTION(sample_array)
{
    zval *subarray;
 
    array_init(return_value);
     
    // Add some scalars 
    add_assoc_long(return_value, "life", 42);
    add_index_bool(return_value, 123, 1);
    add_next_index_double(return_value, 3.1415926535);
     
    // Toss in a static string, dup'd by PHP 
    add_next_index_string(return_value, "Foo", 1);
     
    // Now a manually dup'd string 
    add_next_index_string(return_value, estrdup("Bar"), 0);
 
    // Create a subarray 
    MAKE_STD_ZVAL(subarray);
    array_init(subarray);
     
    // Populate it with some numbers 
    add_next_index_long(subarray, 1);
    add_next_index_long(subarray, 20);
    add_next_index_long(subarray, 300);
     
    // Place the subarray in the parent 
    add_index_zval(return_value, 444, subarray);
}

static zend_function_entry hq_functions[] = {

    ZEND_FE(sample_array,   NULL)
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
