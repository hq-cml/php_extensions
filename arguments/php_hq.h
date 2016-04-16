//php_hq.h
#ifndef HQ_H
#define HQ_H
 
// //加载config.h，如果配置了的话
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
  
//加载php头文件
#include "php.h"
#define phpext_hq_ptr &hq_module_entry
extern zend_module_entry hq_module_entry;
   
#endif
