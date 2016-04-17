<?php
    $resource = my_fopen("/tmp/test.txt", "a");
    unset($resourse); // 这句话注释掉的话，释放仍然会执行，但是和End的顺序会有所不同
    echo "End\n";
