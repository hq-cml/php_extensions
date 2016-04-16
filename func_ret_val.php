<?php
    echo "<pre>";

    $foo= 'I am a string';
    byref_compiletime($foo);
    echo $foo."\n";
    $a='china';
    $b=&return_by_ref();
    $b="This is a ref example";
    echo $a."\n";//此时程序输出php

    //废弃了
    //$str = "abc";
    //byref_calltime(&$str);
    //echo $str."\n";
    hq_hello();

    echo "<br>";
    echo simple_return();
    echo "\n";
    
    $a = sample_array_range();
    print_r(count($a)); 
    echo "\n";
    sample_array_range();
