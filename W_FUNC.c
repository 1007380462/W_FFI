#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_W_FFI.h"
#include <string.h>
#include <stdio.h>
#include <dlfcn.h>

zend_class_entry* w_ffi_ce_function;

static zend_object_handlers w_ffi_function_object_handlers;
PHP_METHOD(W_FUNC, __construct)
{}



PHP_METHOD(W_FUNC, __invoke)
{
	zval *args = NULL;
	long arg_count = 0;
	long i = 0;

    long arrI=0;
    long arrParNumLength=20;
    long*arrParNum=NULL;
    arrParNum=(long*) ecalloc(arrParNumLength, sizeof(long));//存储参数类型是数组类型的个数
     for(arrI=0;arrI<arrParNumLength;arrI++)
     {

      arrParNum[arrI]=-1;
     }

    void*(*Fun)();
	php_w_ffi_function_object *intern = NULL;

	php_w_ffi_value ret_long_value,ret_double_value, *values = NULL,*values_two=NULL,*values_three=NULL;


    /*表示传递任意个数和类型的参数，arg_count参数个数，args是参数数组，args[0]表示传递的第一个参数值*/
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "*", &args, &arg_count) == FAILURE)
	{
		RETURN_NULL();
	}

     intern = php_w_ffi_function_fetch_object(Z_OBJ_P(getThis()));
     if (arg_count != intern->arg_count)
     {
        return;
     }
     Fun=intern->function;

    long*r10P;     /*store r10 register's value*/
    long*rspP;    /*store rsp register's value*/
    void*retVal;  /*store return value of calling function*/


    /*store the return value of a variable,返回值*/
    char*retP=php_w_ffi_get_str_by_num(intern->php_return_type);

 /*获取浮点数参数个数，非浮点数参数个数*/
    int j=0;  /*存储浮点数类型的参数个数*/
    int k=0;  /*存储不是浮点数类型的参数个数*/
    int arr=0;  /*存储数组类型的参数个数*/
    for(i=0;i<arg_count;i++)
    {
      if(Z_TYPE(args[i])==IS_DOUBLE)
      {
        j+=1;
      }
      else
      {
       k+=1;
       if(Z_TYPE(args[i])==IS_ARRAY)
       {
        arr+=1;
       }
      }
    }
   if(k>6)
   {
    values=(php_w_ffi_value *) ecalloc(k, sizeof(php_w_ffi_value)); //分配参数个数的空间,存储非浮点数类型
   }
   else
   {
     values =(php_w_ffi_value *) ecalloc(6, sizeof(php_w_ffi_value));
   }
     values_two  =(php_w_ffi_value *) ecalloc(8,sizeof(php_w_ffi_value));      //只存储浮点数类型,只有xmm0~xmm7


   //c的类型short int long float double char，void*
   /*将传递过来的值存储在values中，其中有浮点数类型和其余的类型*/
    char*p;
    int J=0;   /*对应当前浮点数类型参数的位置*/
    int K=0;   /*对应当前非浮点数类型参数的位置*/
  	for (i = 0; i < arg_count; i++)
  	{
  	 /*long*/
  	  if(Z_TYPE(args[i])==IS_LONG)
  	  {
  	      p=php_w_ffi_get_str_by_num(intern->php_arg_types[i]);
  	      if(p=="int")
  	      {
  	        values[K].l=args[i].value.lval;
  	      }
  	      if(p=="long")
  	      {
  	       values[K].l=args[i].value.lval;
  	      }
  	      K++;
  	  }
  	  /*string，将php 字符转换为 c 字符或字符指针*/
  	  if(Z_TYPE(args[i])==IS_STRING)
      {
         p=php_w_ffi_get_str_by_num(intern->php_arg_types[i]);
         if(p=="char*")  //是一个字符串，可能有点问题，需要测试
         {
          values[K].s=ZSTR_VAL(args[i].value.str);
         }
         if(p=="char") //是单个字符
         {
          values[K].c=ZSTR_VAL(args[i].value.str);
         }
         K++;
      }

       /*double,需要与其余的类型进行区分，这个很特殊*/
       if(Z_TYPE(args[i])==IS_DOUBLE)
       {
         p=php_w_ffi_get_str_by_num(intern->php_arg_types[i]);
         if(J>=j)
         {
            continue;
         }
          if(p=="float")
          {
            values_two[J].f=args[i].value.dval;
          }
          if(p=="double")
          {
           values_two[J].d=args[i].value.dval;
          }
            J++;
       }

       /*array，先将php数组转化为c数组,c数组：字符数组，整型数组，浮点型数组*/
       if(Z_TYPE(args[i])==IS_ARRAY)
       {
         arrParNum[i]=i;  //存储哪个参数是数组类型，同时根据intern->php_arg_types[i]还可以获得数组元素类型
         //zend_array *arr=args[i].values.arr;
         //Bucket*arData=arr->arData;
         int nNumOfElements=args[i].value.arr->nNumOfElements;  //获取数组元素个数
         p=php_w_ffi_get_str_by_num(intern->php_arg_types[i]);
         int*values_int;
         long*values_long;
         char*values_char;
         float*values_float;
         double*values_double;
         if(p=="int")
         {
          values_int=(int*)ecalloc(nNumOfElements,sizeof(int));
         }
         if(p=="long")
         {
          values_long=(long*)ecalloc(nNumOfElements,sizeof(long));
         }
         if(p=="float")
         {
          values_float=(float*)ecalloc(nNumOfElements,sizeof(float));
         }
         if(p=="double")
         {
          values_double=(double*)ecalloc(nNumOfElements,sizeof(double));
         }
         if(p=="char")
         {
         values_char=(char*)ecalloc(nNumOfElements,sizeof(char));
         }
         int elementCount=0;
         //将数组类型的参数转化为c的数组类型
            ZEND_HASH_FOREACH(args[i].value.arr,0);
            /*整型数组*/
            if(Z_TYPE_P(_z)==IS_LONG)
            {
            values_int[elementCount]=_z->value.lval;
            }
            /*浮点型数组*/
            if(Z_TYPE_P(_z)==IS_DOUBLE)
            {
                if(p=="float")
                {
                 values_float[elementCount]=(float)_z->value.dval;
                }
                if(p=="double")
                {
                 values_double[elementCount]=_z->value.dval;
                }
            }
            /*字符数组*/
            if(Z_TYPE_P(_z)==IS_STRING)
            {
             values_char[elementCount]=ZSTR_VAL(_z->value.str);
            }
             elementCount++;
            ZEND_HASH_FOREACH_END();

            if(p=="int")
            {
            values[i].p=&values_int[0];
            }
            if(p=="long")
            {
            values[i].p=&values_long[0];
            }
            if(p=="char")
            {
            values[i].p=&values_char[0];
            }
            if(p=="float")
            {
            values[i].p=&values_float[0];
            }
            if(p=="double")
            {
            values[i].p=&values_double[0];
            }

         K++;
       }
       /**/
       if(Z_TYPE(args[i])==IS_CONSTANT)
       {
         p=php_w_ffi_get_str_by_num(intern->php_arg_types[i]);
         if(p=="")
         {

         }
         K++;
       }
       /*ptr*/
       if(Z_TYPE(args[i])==IS_PTR)
       {

         K++;
       }
  	}

   /*将rsp寄存器的值存放在rsp中*/
   asm volatile("mov %%rsp,%0":"=r"(rspP));
  /*必须将fun存储到r10或r11寄存器中其余寄存器不行,查看 x86_64 ABI文档*/
  asm volatile("mov %1,%%r10;" "mov %%r10,%0" :"=rsi"(r10P) :"0"(Fun) :"%r10");

  /*6个参数寄存器 rdi，rsi,rdx,rcx,r8,r9*/
/*传递6个非浮点数类型参数后的非浮点数类型的参数，对于整数，字符，指针都是8个字节*/
for(i=6;i<k;i++)
{
 // asm volatile("movq %1,%%rax;" "movq %%rax,%0" :"=rax"(*(rspP+1)):"0"(values[k-1]):"%rax");
  //asm volatile("movq %1,%%rax;" "movq %%rax,%0" :"=rax"(*(rspP+0)):"0"(values[k-2]):"%rax");
 // asm volatile("movq %1,%%rax;" "movq %%rax,%0" :"=rax"(*(rspP+0)):"0"(values[arg_count-3]):"%rax");
 // break;
 //asm volatile("movq %1,%%rax;" "movq %%rax,%0" :"=rax"(*(rspP+arg_count-i-1)):"0"(values[arg_count-3]):"%rax");

 /*注意压栈的顺序不能变，先压最右边的，依次往左边，最后将arg_count变量的值给覆盖掉了*/
 /*参数从右边到左边压栈，最左边就是栈顶，右边参数的地址大于左边的地址*/
  asm volatile("movq %1,%%rax;" "movq %%rax,%0" :"=rax"(*(rspP+k-i-1)):"0"(values[k-i+5]):"%rax");
}


/*传递浮点数类型的参数*/
long lL=0;
asm volatile("mov %1,%%rbx;"
               "movsd (%%rbx),%%xmm0;"
               "movsd 8(%%rbx),%%xmm1;"
               "movsd 16(%%rbx),%%xmm2;"
               "movsd 24(%%rbx),%%xmm3;"
               "movsd 32(%%rbx),%%xmm4;"
               "movsd 40(%%rbx),%%xmm5;"
               "movsd 48(%%rbx),%%xmm6;"
               "movsd 56(%%rbx),%%xmm7;"
               "mov %%rbx,%0"
               :"=rbx"(lL)
               :"0"(values_two)
               :"%rbx","%xmm0","%xmm1","%xmm2","%xmm3","%xmm4","%xmm5","%xmm6","%xmm7"
               );


void*pP=NULL;
/*传递前6个非浮点数类型的参数*/
asm volatile("mov %1,%%rax;"
             "mov (%%rax),%%rdi;"
             "mov 8(%%rax),%%rsi;"
             "mov 16(%%rax),%%rdx;"
             "mov 24(%%rax),%%rcx;"
             "mov 32(%%rax),%%r8;"
             "mov 40(%%rax),%%r9;"
             "mov %%rdi,%0"
                           :"=rax"(pP)
                           :"0"(values)
                           :"%rdi","rsi","rdx","rcx","r8","r9"
            );


 //将wmr变量的值赋值到rdi寄存器，然后把rdi寄存器的值赋值到w变量中，这样实现了给rdi寄存器赋值的功能，注意其他寄存器会修改这个寄存器的值
/*
asm volatile("movq $1,%rdi");
 asm volatile("movq $2,%rsi");
 asm volatile("movq $3,%rdx");
 asm volatile("movq $4,%rcx");
 asm volatile("movq $5,%r8");
 asm volatile("movq $6,%r9");
 */


 asm volatile("call  %r10");

 asm volatile("mov %%rax,%0":"=r"(ret_long_value));  //获得函数的返回值,浮点数是xmm0寄存器
 asm volatile("movsd %%xmm0,(%%rsp);" "mov (%%rsp),%0":"=r"(ret_double_value));

if(retP=="float")
 {
   double temp=(double)ret_double_value.f;
   return_value->value.dval=temp;
 }
 if(retP=="double")
 {

  return_value->value.dval=ret_double_value.d;
 }
 if(retP=="int"||retP=="long")
 {

    return_value->value.lval=ret_long_value.l;
 }
 if(retP=="char")
 {


 }


/* Free arguments*/
/*释放其中参数类型是数组时分配的空间*/
for(i=0;i<arrParNumLength;i++)
{
   if(arrParNum[i]!=-1)
   {
       efree(values[i].p);
   }
}
efree(values);       //存储所有的整型，指针，字符，由于其中有指针需要释放其中指针指向的空间
efree(arrParNum);
efree(values_two);   //存储浮点数类型的参数数组

if(retP=="float"||retP=="double")
 {
   RETURN_DOUBLE(return_value->value.dval);
 }
 if(retP=="int"||retP=="long")
 {
   RETURN_LONG(return_value->value.lval);
 }
return;
}

const zend_function_entry w_ffi_function_methods[] = {
	PHP_ME(W_FUNC, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR|ZEND_ACC_FINAL)
	PHP_ME(W_FUNC, __invoke, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};


php_w_ffi_function_object*php_w_ffi_function_fetch_object(zend_object *obj)
{
	return (php_w_ffi_function_object*)((char*)(obj) - XtOffsetOf(php_w_ffi_function_object, std));
}

static zend_object *w_ffi_function_object_new(zend_class_entry *ce)
{
	php_w_ffi_function_object *object = ecalloc(1, sizeof(php_w_ffi_function_object) + zend_object_properties_size(ce));
	zend_object_std_init(&object->std, ce);
	object_properties_init(&object->std, ce);
	object->std.handlers = &w_ffi_function_object_handlers;
	return &object->std;
}

/*free function object space*/
static void w_ffi_function_object_free(zend_object *object)
{
	php_w_ffi_function_object *intern = php_w_ffi_function_fetch_object(object);
	if (!intern)
	{
		return;
	}
	if(intern->php_arg_types)
	{
	 efree(intern->php_arg_types);
	}
	zend_object_std_dtor(&intern->std);
}

PHP_MINIT_FUNCTION(W_FUNC)
{
	zend_class_entry function_ce;

	memcpy(&w_ffi_function_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	w_ffi_function_object_handlers.offset = XtOffsetOf(php_w_ffi_function_object, std);
	w_ffi_function_object_handlers.free_obj = w_ffi_function_object_free;
	w_ffi_function_object_handlers.clone_obj = NULL;

	INIT_NS_CLASS_ENTRY(function_ce, "W_FFI", "FUNC", w_ffi_function_methods);
	function_ce.create_object = w_ffi_function_object_new;
    w_ffi_ce_function=zend_register_internal_class(&function_ce);

	return SUCCESS;
}
PHP_MSHUTDOWN_FUNCTION(W_FUNC)
{

   return SUCCESS;
}

PHP_RINIT_FUNCTION(W_FUNC)
{

	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(W_FUNC)
{

	return SUCCESS;
}