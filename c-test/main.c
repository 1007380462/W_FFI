#include<stdio.h>
//void swap();
//int buf[2]={1,2};

int te(long a,int b,int c,int d,int e,int f,int g,int h,int i,int j,long L){
  return 2;
}
int main()
{
       long wmr=18;   
       long*sp;
       //asm volatile("mov %%rsp,%0":"=r"(sp));
       //asm volatile("mov %1,%0":"=rax"(*(sp+8)):"rax"(wmr):"rax");
       //asm volatile("mov %1,%%rdi;" "mov %%rdi,%0" :"=rsi"(sp) :"0"(wmr):"%rsi");
//     asm volatile("xchg %rdi,8(%rsp)");
  //   asm volatile("mov %rdi,8(%rsp)");
//     long a=1;
       te(90,2,3,4,5,6,7,8,9,10,11);
       printf("%ld",wmr);
       
	return 0;
}
