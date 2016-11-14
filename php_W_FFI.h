/* $Id$ */

#ifndef PHP_W_FFI_H
#define PHP_W_FFI_H

extern zend_module_entry W_FFI_module_entry;
#define phpext_W_FFI_ptr &W_FFI_module_entry

#define PHP_W_FFI_VERSION "0.1.0" /* Replace with version number for your extension */

#define W_FFI_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(W_FFI, v)

#ifdef PHP_WIN32
#	define PHP_W_FFI_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_W_FFI_API __attribute__ ((visibility("default")))
#else
#	define PHP_W_FFI_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

/*
  	Declare any global variables you may need between the BEGIN
	and END macros here:
*/
ZEND_BEGIN_MODULE_GLOBALS(W_FFI)
	zend_long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(W_FFI)


/* Always refer to the globals in your function as W_FFI_G(variable).
   You are encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

//自定义对象结构
typedef struct _php_w_ffi_object {
	void *handle;
	zend_object std;
} php_w_ffi_object;


//用于存储绑定的函数，使用bind方法
typedef struct _php_w_ffi_function_object {
	void *function;          //函数指针
	long php_return_type;    //函数的返回值类型
	long *php_arg_types;    //参数类型值 存储所有传递的参数的类型
	long arg_count;        //参数个数
	zend_object std;       //function 对象实例
} php_w_ffi_function_object;

typedef union _php_w_ffi_value {
	char c;
	int i;
	long l;
	float f;
	double d;
	//long double D;
	void *p;
	char *s;
} php_w_ffi_value;

 zend_class_entry *w_ffi_ce_function;

/*language data type transform*/
char*php_w_ffi_get_str_by_num(long);
long php_w_ffi_get_num_by_str(char*);


php_w_ffi_object*php_w_ffi_fetch_object(zend_object*obj);

php_w_ffi_function_object*php_w_ffi_function_fetch_object(zend_object *obj);

PHP_MINIT_FUNCTION(W_MAIN);
PHP_MINIT_FUNCTION(W_FUNC);

#if defined(ZTS) && defined(COMPILE_DL_W_FFI)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#endif	/* PHP_W_FFI_H */