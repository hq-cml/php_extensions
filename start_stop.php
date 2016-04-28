<?php
    hq_hello();
    echo "常量HQ_VERSION的值是:".HQ_VERSION;
    echo "\n";

    $cnt = hq_counter();
    echo "cnt:$cnt\n";
    $cnt = hq_counter();
    echo "cnt:$cnt\n";
    $cnt = hq_counter();
    echo "cnt:$cnt\n";
    $cnt = hq_counter();
    echo "cnt:$cnt\n";
    
    echo "\n\n超级全局变量_SUPER_GLOBAL:\n";
    print_r($_SUPER_GLOBAL);
