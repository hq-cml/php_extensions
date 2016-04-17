<?php
    $file = my_fopen("/tmp/test.txt", "a");
    $cnt = my_fwrite($file, "Hello\n");
    echo "写入了".$cnt."个字节\n";
    $file2 = $file;
    my_fclose($file);
    unset($file); // 这句话注释掉的话，释放仍然会执行，但是和End的顺序会有所不同
    echo "End\n";
