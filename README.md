# aesCtrMultiThread

this c++2011 code want to split CTR coding into threads. 
Finally the code will have file read/write threads, too.

## next concept

    1) Read Data From File
    2) Split Data to threads (loop)
    A) Read Data From File
    3) join threads to coded data (loop)
    START LOOP
       B) II) Split Data to threads (loop)
       I) Read Data From File -> no data? BREAK
       4) D) IV) write coded data
       C) III) join threads to coded data (loop)
    END LOOP
    write coded data
    join threads to coded data (loop)
    write coded data

## far away

 -  good balance main loop and threads
 -  performace tests
 -  automatic balance
 -  less memory usage
 -  include old code (ecb, cbc) as cli option
 -  user documentation / manual
