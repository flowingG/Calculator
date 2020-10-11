#ifndef __MY_MATH_H_
#define __MY_MATH_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define oops(m, x){perror(m); exit(x);}

/*
 * 大体思路，先创建一个表达式的结构体，里面包含运算符的字符数组和运算数的数数组
 * 每个表达式都用这个结构体来表达0
 * 由题意可知，运算符最多3个，数4个
 * 用波兰法的思想可以根据运算符的顺序唯一生成一个表达式，即保证了算式不会重复（两个表达式完全一样的概率极低，可视为不重复）
 * 出现负数就重新执行这个表达式的随机生成过程
 * 最后把生成的表达式结构体转换为字符
 * 写入文件
 * 逆波兰表达式保证了运算顺序对式子唯一性的影响
*/
/*按逆波兰法，可以保证式子的不同
现在的问题是，怎么把括号加入字符输出区中
解决法1：读两次，一次从右往左把数据逐个加入字符串中，输出是逆序的，如3+3*(3+3)，* /后接的是+-就加括号
        第二次读则是从左往右进行数据的计算
***
3*3*3*3 81
**+ +**
3*3*3+3 30
(3+3)*3*3 54
==>3*3*(3+3)
*++ +*+ ++*
3*3+3+3 15
(3+3)*3+3 21
==>3+3*(3+3)
(3+3+3)*3 27
==>3*(3+3+3)
+++
3+3+3+3 12
*+*
(3*3+3)*3 36
*/

//区分自然数和真分数
enum mytype{
    NATURAL,
    FRACTION
};

//数结构体
typedef struct mynum{
    enum mytype TYPE;
    //分子分母以外的值
    int number;
    //分子，如是自然数，则分子分母均为1
    int numerator;
    //分母
    int denominator;

}mynum;

//表达式结构体
typedef struct myexpression{
    //数数组，存放各种数
    mynum nums[4];
    //字符数组。用来存放运算符
    char operator[4];
}myexpression;

//参数处理，传入参数数量，参数字符串，题目数量，数值范围
void do_args(int argc, char *argv[], int *count, int *range);
//生成一条表达式
void produce_expression(myexpression *expression, int range);
//欧几里得算法求最大公约数
int gcd(int a,int b);
//约分
void do_fraction(mynum *num);
//表达式转换为字符串
void expression_to_string(myexpression *expression, char *exercisesbuf, char *operator);
//单个数加入字符串
void num_to_string(mynum *num, char *exercisesbuf, int *bufindex);
//答案转字符串
void answer_to_string(mynum *answer, char *answerbuf);
//数转字符串
void int_to_string(int num, char *buf, int *bufindex);
//计算答案
int calculate(myexpression *expression, char *answerbuf, mynum *answer);

#endif