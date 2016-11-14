#!/bin/bash
make --debug && php -d extension=modules/W_FFI.so test.php 
