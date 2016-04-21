<?php
    $obj= new myclass();
    $obj->public_var = 1;
    $obj->public_method();
    echo "类有一个常量MY_CONST：".myclass::MY_CONST."\n";
    var_dump($obj);
