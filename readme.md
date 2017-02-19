# PHP扩展样例：
## 目录：
#### 1. hello
最简单的手动创建的扩展，包含一个最简单的标准输出函数：hello world



## 手动创建模式：

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
#/usr/local/php_nginx/bin/phpize  //以comfig.m4为源产出configure脚本
#./configure --enable-debug --with-php-config=/usr/local/php_nginx/bin/php-config //检测环境及相关依赖，产出了makefile
#make; make install //以makefile为源，产出最终的so文件
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



工具创建模式：
1. cd /usr/local/src/php/ext/
2. ext_skel --extname=hello_world   #利用ext_skel生成模块代码框架
3. vim config.m4文件
    去掉10-12行的dnl
4. vim php_hello_world.h
    新增声明导出函数
5. vim hello_world.c
    新增声明zend函数块
6. vim hello_world.c
    新增实现导出函数
7. /usr/local/apache/bin/phpize
8. 编译
   ./configure --with-php-config=/usr/local/apache/bin/php-config
9. make
10. make install   
11. vim /etc/php.ini
    新增extionsion.so