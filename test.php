<?php
/*以前老的测试*/
$library = new W_FFI\Library("/score");
$arr=array(1,1,1,1);
$puts = $library->bind('ext_score', $arr,3);   //函数名称 参数类型  返回参数类型
echo $puts(1,2,3,4);

die;

/*测试传递类型的参数 函数调用*/
$library=new W_FFI\Library("/main");
$typeArr=array('int'=>1,'float'=>2,'double'=>3,'long'=>4,'char'=>5,'void*'=>6);
//$arr=array(4,4,4,4,4,4,4,4,3); //9个参数,表示参数类型
$arr=array($typeArr['int'],$typeArr['long'],$typeArr['long'],$typeArr['long'],$typeArr['double'],$typeArr['long'],$typeArr['long'],$typeArr['float'],$typeArr['float']);
$retype=4;    //返回参数类型
$puts=$library->bind('test_a',$arr,$retype);
$arr_value=array(4,2,3,45);
for($i=0;$i<100;$i++){
     $puts($arr_value,2,3,44,5.5,6,7,8.5,1.5);
     $puts($arr_value,2,3,44,5.5,6,7,8.5,1.5);
     $puts($arr_value,2,3,44,5.5,6,7,8.5,1.5);
     $puts($arr_value,2,3,44,5.5,6,7,8.5,1.5);
}

die;

$getenv = $library->bind('getenv', [ MFFI\Type::TYPE_STRING ], MFFI\Type::TYPE_STRING);
var_dump($getenv('USER'));

$strlen = $library->bind('strlen', [ MFFI\Type::TYPE_STRING ], MFFI\Type::TYPE_INT);
var_dump($strlen("Hello world"));
?>

<!--int 1;float 2;double 3;long 4,char 5;void* 6;char* 7;8 char[];-->