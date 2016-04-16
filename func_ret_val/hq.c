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

//返回引用
#if (PHP_MAJOR_VERSION > 5) || (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION > 0)
//arguinfo通知内核这个函数特殊性，返回的是引用
ZEND_BEGIN_ARG_INFO_EX(return_by_ref_arginfo, 0, 1, 0)
ZEND_END_ARG_INFO ()

ZEND_FUNCTION(return_by_ref)
{
	zval **a_ptr;
	zval *a;

	//检查全局作用域中是否有$a这个变量，如果没有则添加一个
	//在内核中真的是可以胡作非为啊，:-)
	if(zend_hash_find(&EG(symbol_table) , "a",sizeof("a"),(void **)&a_ptr ) == SUCCESS )
	{
		a = *a_ptr;
	}
	else
	{
		ALLOC_INIT_ZVAL(a);
		zend_hash_add(&EG(symbol_table), "a", sizeof("a"), &a,sizeof(zval*), NULL);
	}
	
	//废弃return_value,使用return_value_ptr来接替它的工作。HQ：原本的返回值被释放，用全局变量$a作为返回值的zval。
	zval_ptr_dtor(return_value_ptr);
	
	//如果$a存在隐式的引用，则要进行分离，否则不分离，大多数情况不会进入这段逻辑的，为了便于首次学习理解，这一段逻辑暂时不考虑进入的情况
	if( !a->is_ref__gc && a->refcount__gc > 1 )
	{
		zval *tmp;
		MAKE_STD_ZVAL(tmp);
		*tmp = *a;
		zval_copy_ctor(tmp);
		tmp->is_ref__gc = 0;
		tmp->refcount__gc = 1;
		zend_hash_update(&EG(symbol_table), "a", sizeof("a"), &tmp,sizeof(zval*), NULL);
		a = tmp;
	}
	a->is_ref__gc = 1;
	a->refcount__gc++;
	*return_value_ptr = a; //返回值成了一个引用（不用新生成zval，和全局变量$a公用一个zval作为返回值，也就是返回值就是全局$a本身啦）
}
#endif/* PHP >= 5.1.0 */
static zend_function_entry hq_functions[] = {
    ZEND_FE(hq_hello,        NULL)
    ZEND_FE(simple_return,   NULL)
    ZEND_FE(sample_array_range,   NULL)
#if (PHP_MAJOR_VERSION > 5) || (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION > 0)
    ZEND_FE(return_by_ref, return_by_ref_arginfo)
#endif /* PHP >= 5.1.0 */
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
