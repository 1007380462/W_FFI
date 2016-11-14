#!/bin/bash
for((i=0;i<10000;i++))
do
     php -d extension=modules/W_FFI.so test.php
done
