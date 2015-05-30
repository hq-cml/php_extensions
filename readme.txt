PHP模块编写步骤：
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
   