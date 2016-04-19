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
   
    //释放普通资源，这两句unset注释掉的话，释放仍然会执行，但是和End的顺序会有所不同
    unset($file); 
    unset($file2); 
    
    //释放持久资源，这句应该没什么卵用，因为持久资源只能由MSHUTDOWN释放
    unset($file_p);
    
    echo "\n\n普通资源均已释放，看看持久资源是否还有用呢：\n";
    //这句应该会利用到持久资源 
    $file3 = my_fopen("/tmp/test.txt", "a");
    $cnt = my_fwrite($file3, "World HAHA\n");
    echo "写入了".$cnt."个字节\n";
   
    echo "\nEnd\n";
