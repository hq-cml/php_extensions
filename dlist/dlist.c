/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2014 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_dlist.h"
#include "dlist_src.h"


dlist_head *dlist_create()
{
    dlist_head *head;
    
    head = (dlist_head *)malloc(sizeof(dlist_head));
    
    if(head)
    {
        head->size = 0;
        head->head = NULL;
        head->tail = NULL;
    }
    
    return head;
}

int dlist_add_head(dlist_head *head, zval *value)
{
    dlist_node *node;
    node = (dlist_node *)malloc(sizeof(*node));
    if(!node)
    {
        return 0;
    }
    
    node->value = value;
    node->prev = NULL;
    node->next = head->head;
    
    if(head->head)
    {
        head->head->prev = node;
    }
    
    head->head = node;
    if(!head->tail)
    {
        head->tail = head->head;
    }
    head->size++;
    
    return 1;
}

int dlist_add_tail(dlist_head *head, zval *value)
{
    dlist_node *node;
    node = (dlist_node *)malloc(sizeof(*node));
    if(!node)
    {
        return 0;
    }
    
    node->value = value;
    node->prev = head->tail;
    node->next = NULL;
    
    if(head->tail)
    {
        head->tail->next = node;
    }
    
    head->tail = node;
    if(!head->head)
    {
        head->head = head->tail;
    }
    head->size++;
    
    return 1;
}

int dlist_delete_index(dlist_head *head, int index)
{
    dlist_node *curr;
    
    if(index < 0)
    {
        index = (-index)-1;
        curr = head->tail;
        while(index>0)
        {
            curr = curr->prev;
            index--;
        }
    }
    else
    {
        curr = head->head;
        while(index>0)
        {
            curr = curr->next;
            index--;
        }
    }
    
    if(!curr || index>0) return 0;
    
    if(curr->prev)
    {
        curr->prev->next = curr->next;
    }
    else
    {
        head->head = curr->next;
    }
    
    if(curr->next)
    {
        curr->next->prev = curr->prev;
    }
    else
    {
        head->tail = curr->prev;
    }
    
    return 1;
    
}

int dlist_fetch(dlist_head *head, int index, zval **retval)
{
    dlist_node *node;
    if(index > 0)
    {
        node = head->head;
        while(node && index > 0)
        {
            node = node->next;
            index --;
        }
    }
    else
    {
        index = (-index)-1;
        node = head->tail;
        while(node && index > 0)
        {
            node = node->prev;
            index--;
        }
    }
    
    if(!node || index>0)
        return 0;
    
    *retval = node->value;
    
    return 1;
}

int dlist_length(dlist_head *head)
{
    if(head)
    {
        return head->size;
    }
    else
    {
        return 0;
    }
}

void dlist_destroy(dlist_head *head)
{
    dlist_node *curr, *next;
    curr = head->head;
    while(curr)
    {
        next = curr->next;
        free(curr);
        curr = next;
    }
    
    free(head);
}
/* If you declare any globals in php_dlist.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(dlist)
*/

/* True global resources - no need for thread safety here */
static int le_dlist;

/* {{{ dlist_functions[]
 *
 * Every user visible function must have an entry in dlist_functions[].
 */
//ÉùÃ÷zendº¯Êý¿é
const zend_function_entry dlist_functions[] = {
	PHP_FE(dlist_create,	NULL)
	PHP_FE(dlist_add_head,	NULL)
	PHP_FE(dlist_add_tail,	NULL)
	PHP_FE(dlist_fetch_index,	NULL)
	PHP_FE(dlist_delete_index,	NULL)
	PHP_FE(dlist_destroy,	NULL)
	PHP_FE(dlist_element_nums,	NULL)
	PHP_FE_END	/* Must be the last line in dlist_functions[] */
};
/* }}} */

/* {{{ dlist_module_entry
 */
zend_module_entry dlist_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"dlist",
	dlist_functions,
	PHP_MINIT(dlist),
	PHP_MSHUTDOWN(dlist),
	PHP_RINIT(dlist),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(dlist),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(dlist),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_DLIST_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_DLIST
ZEND_GET_MODULE(dlist)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("dlist.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_dlist_globals, dlist_globals)
    STD_PHP_INI_ENTRY("dlist.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_dlist_globals, dlist_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_dlist_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_dlist_init_globals(zend_dlist_globals *dlist_globals)
{
	dlist_globals->global_value = 0;
	dlist_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(dlist)
{
	le_dlist = zend_register_list_destructors_ex(dlist_destroy_handler, NULL, "list_resource", module_number);
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(dlist)
{
	
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(dlist)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(dlist)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(dlist)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "dlist support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_dlist_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_dlist_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "dlist", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
