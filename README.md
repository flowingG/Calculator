# Calculator
实现的具体要求：

    1.使用 -n 参数控制生成题目的个数
    2.使用 -r 参数控制题目中数值（自然数、真分数和真分数分母）的范围
    3.生成的题目中计算过程不能产生负数，也就是说算术表达式中如果存在形如e1− e2的子表达式，那么e1≥ e2
    4.生成的题目中如果存在形如e1÷ e2的子表达式，那么其结果应是真分数。
    5.每道题目中出现的运算符个数不超过3个。
    6.程序一次运行生成的题目不能重复，即任何两道题目不能通过有限次交换+和×左右的算术表达式变换为同一道题目。
    7.生成题目的同时，计算出所有题目的答案，并存入执行程序的当前目录下的Answers.txt文件，生成的题目存入执行程序的当前目录下的Exercises.txt文件。
    8.支持生成1w道题目

编译(gcc编译器)：
gcc main.c math.c -o main.exe

运行命令：
  main.exe |
	main.exe -n 10000 |
  main.exe -r 100 |
  main.exe -n 10000 -r 100
  
