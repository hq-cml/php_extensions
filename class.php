<?php
/*
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
*/

    $obj= new myclass();
    $obj->public_var = 1;
    $obj->public_method();
    echo "类有一个常量MY_CONST：".myclass::MY_CONST."\n";
    var_dump($obj);
