<?php
    $file = my_fopen("/tmp/test.txt", "a");
    $file_p = my_fopen("/tmp/test.txt", "a", true);
    unset($file_p);//这句unset没有卵用，因为是永久资源，只会在执行MSHUTDOWN的时候才会被析构
    $cnt = my_fwrite($file, "Hello\n");
    echo "写入了".$cnt."个字节\n";
    $file2 = $file;
    $filename = get_fname($file2);
    echo "文件名：".$filename."\n";
    //my_fclose($file); //强制释放
    unset($file); // 这两句unset注释掉的话，释放仍然会执行，但是和End的顺序会有所不同
    unset($file2); 
    echo "End\n";
