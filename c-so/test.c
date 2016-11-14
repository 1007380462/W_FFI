#include<stdio.h>
#include<dlfcn.h>

int main(){
double(*funcP)();
void*handler=dlopen("/mnt/hgfs/gongxiang/W_FFI/W_FFI/c-so/main.so",RTLD_LAZY);
if(handler==NULL){

printf("failed load libaray");
return -1;
}
funcP=dlsym(handler,"test_a");
if(funcP==NULL){
printf("call is failure");
return 0;
}
double p=(*funcP)(1);
// int a=*((int*)p);
//printf("%d",p);
return 0;
}
