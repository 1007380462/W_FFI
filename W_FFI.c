/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_W_FFI.h"

PHP_RINIT_FUNCTION(W_FFI)
{

	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(W_FFI)
{

	return SUCCESS;
}


PHP_MINIT_FUNCTION(W_FFI)
{
  PHP_MINIT(W_MAIN)(INIT_FUNC_ARGS_PASSTHRU);
  PHP_MINIT(W_FUNC)(INIT_FUNC_ARGS_PASSTHRU);
   return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(W_FFI)
{
  return SUCCESS;
}


PHP_MINFO_FUNCTION(W_FFI)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "W_FFI support", "enabled");
	php_info_print_table_end();
}


const zend_function_entry W_FFI_function[] = {
	               PHP_FE_END
};

zend_module_entry W_FFI_module_entry = {
	STANDARD_MODULE_HEADER,
	"W_FFI",
	W_FFI_function,
	PHP_MINIT(W_FFI),
	PHP_MSHUTDOWN(W_FFI),
	NULL,
	NULL,
	PHP_MINFO(W_FFI),
	PHP_W_FFI_VERSION,
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_W_FFI
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(W_FFI)
#endif

