//hq.c
#include "php_hq.h"

ZEND_FUNCTION(hq_hello)
{
    php_printf("Hello World!\n");
}

ZEND_FUNCTION(simple_return)
{
    ZVAL_LONG(return_value, 10); //HQ：return_value是指针，这个变量是宏ZEND_FUCTION展开后就有的~
    return;
}

ZEND_FUNCTION(sample_array_range)
{
	if(return_value_used) 
         {
		int i;
		array_init(return_value);//把返回值初始化成一个PHP语言中的数组

		for(i = 0; i < 1000; i++)
		{
			//向retrun_value里不断的添加新元素，值为i
			add_next_index_long(return_value, i);
		}
		return;
	}
	else
	{
		//抛出一个E_NOTICE级错误
		php_error_docref(NULL TSRMLS_CC, E_NOTICE, "猫了个咪的，我就知道你没用我的劳动成果！");
		RETURN_NULL();
	}
}
static zend_function_entry hq_functions[] = {
    ZEND_FE(hq_hello,        NULL)
    ZEND_FE(simple_return,   NULL)
    ZEND_FE(sample_array_range,   NULL)
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
