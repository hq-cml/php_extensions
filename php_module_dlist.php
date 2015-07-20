<?php
    $list = dlist_create();
    $a = "a";
    $b = "b";
    $c = "c";
    dlist_add_head($list, $a);
    dlist_add_head($list, $b);
    dlist_add_head($list, $c);

    
    
    $length = dlist_element_nums($list);
    
    echo "The Length of dlist:".$length."<br>";
    
    echo "The Content of dlist:<br>";
    for($i = 0; $i < $length; $i++)
    {
        var_dump( dlist_fetch_index($list, $i));
        echo "<br>";
    }