# PHP扩展：


## 手动创建方式：

1. 在php源码文件夹的ext目录创建扩展目录，目录名称为扩展名称，如hq
2. 在目录中创建config.m4文件，内容类似如下：

```
PHP_ARG_ENABLE(hq,
    [Whether to enable the "hq" extension],
    [  enable-hq        Enable "hq" extension support])

if test $PHP_HQ != "no"; then
    PHP_SUBST(HQ_SHARED_LIBADD)
    PHP_NEW_EXTENSION(hq, hq.c, $ext_shared)
fi
```
3. 创建.c和.h两个文件，比如hq.c和php_hq.h
4. 编译扩展：在当前目录执行如下命令

```
# /usr/local/php_nginx/bin/phpize  //以comfig.m4为源产出configure脚本
# ./configure --enable-debug --with-php-config=/usr/local/php_nginx/bin/php-config //检测环境及相关依赖，产出了makefile
# make; make install //以makefile为源，产出最终的so文件
```
5. 加载扩展：编辑php.ini
```
extension_dir=/usr/local/php_nginx/lib/php/extensions/no-debug-zts-20121212
extension=hq.so
```
6. 检验是否加载成功

```
php -m | grep hq
```

---


## 脚手架工具ext_skel创建方式：
1. 进入PHP源文件目录ext

   cd /usr/local/src/php/ext/
2. 利用ext_skel生成模块代码框架

   ext_skel --extname=extskel   # 扩展名字extskel
   
   脚手架会自动生成目录和文件，关键的几个文件是：config.m4，extskel.c，php_extskel.h
3. vim config.m4文件，去掉如下行前面的dnl

    如果扩展本身存在对依赖第三方lib，则去掉如下的注释
```
dnl PHP_ARG_WITH(extskel, for extskel support,
dnl Make sure that the comment is aligned:
dnl [  --with-extskel             Include extskel support])
```
    否则，去掉如下的注释：
    
```
dnl PHP_ARG_ENABLE(extskel, whether to enable extskel support,
dnl Make sure that the comment is aligned:
dnl [  --enable-extskel           Enable extskel support])
```

4. vim extskel.c，实现自定义函数并将其加入zend_function_entry中，如：

```
ZEND_FUNCTION(hq_hello)
{
    php_printf("Hello World!\n");
}

const zend_function_entry extskel_functions[] = {
    ZEND_FE(hq_hello,        NULL)
    PHP_FE_END      /* Must be the last line in extskel_functions[] */
};
```

5. vim extskel.c，如果有必要，填充实现四个基本函数:MINIT,RINIT,RSHUTDOWN,MSHUTDOWN，如：

```
PHP_MINIT_FUNCTION(extskel)
{       
        php_printf("Minit!\n");
        return SUCCESS;
}
```
6. 编译扩展：在当前目录执行如下命令

```
# /usr/local/php_nginx/bin/phpize  //以comfig.m4为源产出configure脚本
# ./configure --enable-debug --with-php-config=/usr/local/php_nginx/bin/php-config //检测环境及相关依赖，产出了makefile
# make; make install //以makefile为源，产出最终的so文件
``` 
7. 加载扩展：编辑php.ini
```
extension_dir=/usr/local/php_nginx/lib/php/extensions/no-debug-zts-20121212
extension=hq.so
```
8. 检验是否加载成功

```
php -m | grep hq
```

---

## 目录说明：
### 1. hello
最简单的手动创建的扩展，包含一个最简单的标准输出函数：hello world
### 2. return
手动创建，扩展中的函数返的样例
### 3. arguments
手动创建，扩展中的函数传参的样例
### 4. array
手动创建，扩展中的HashTable的使用样例
### 5. resource
手动创建，扩展中的资源的使用样例
### 6. class
手动创建，扩展中的面向对象的样例
### 7. start_stop
手动创建，扩展中的启动终止样例（生命周期，常量、全局变量和超级全局变量的定义和使用）
### 8. extskel
用脚手架程序创建扩展样例
### 9. dlist
综合样例，用脚手架程序创建，一个双向链表
### 10. *.php
每个扩展对应的PHP使用文件