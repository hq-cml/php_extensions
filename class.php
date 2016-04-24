<?php
/*
//C扩展实现了同等效果的如下PHP代码：
//普通类定义
class myclass
{
    public $public_var;
    public const MY_CONST=10;
    public function __construct()
    {
        echo"我是__construct方法\n";
    }
    public function public_method()
    {
        echo"我是public类型的方法\n";
    }
}

//接口定义
interface myinterface
{
    public function hello();
}

//类实现了上面的接口
class parent_class implements myinterface
{
    public function hello()
    {
        echo"Good Morning!\n";
    }
}
 
//子类继承上面的基类
final class son_class extends parent_class
{
    public function call_hello()
    {
        $this->hello();
    }
}
*/

$obj= new myclass();
$obj->public_var = 1;
$obj->public_method();
echo "类有一个常量MY_CONST：".myclass::MY_CONST."\n";
var_dump($obj);

//用PHP实现接口myinterface
class myclass2 implements myinterface
{
    public $name="Ha Ha!";

    public function hello()
    {
    	echo $this->name."\n";
    }
}
echo "MyClass2's hello():";
$obj= new myclass2();
$obj->hello();

//直接实例化化parent_class
$parent = new parent_class();
echo "Parents's hello():";
$parent->hello();










