//php_hq.h
#ifndef HQ_H
#define HQ_H
 
//加载config.h，如果配置了的话
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
  
//加载php头文件
#include "php.h"
#define phpext_hq_ptr &hq_module_entry
extern zend_module_entry hq_module_entry;
   
//声明全局变量   
ZEND_BEGIN_MODULE_GLOBALS(hq)
    unsigned long counter;     //一个计数器
ZEND_END_MODULE_GLOBALS(hq)
   
#endif
