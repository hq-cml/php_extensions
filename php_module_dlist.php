<?php
    $list = dlist_create();
    dlist_add_head($list, "a");
    dlist_add_head($list, "b");
    dlist_add_head($list, "c");
    
    
    $length = dlist_element_nums($list);
    
    echo "The Length of dlist:".$length."<br>";
    
    echo "The Content of dlist:<br>";
    for($i = 0; $i < $length; $i++)
        echo dlist_fetch_index($list, $i)." ";