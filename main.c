#include "math.h"

int main(int argc, char *argv[]){
    //默认题目数量，默认数值范围均为10
    int count = 10, range = 10;
    //参数处理
    do_args(argc, argv, &count, &range);
    //题目文件和答案文件，需要截断，创建，可写
    FILE *exercisesfile = fopen("./Exercises.txt", "w");
    FILE *answerfile = fopen("./Answer.txt", "w");
    //表达式
    myexpression *expression;
    //初始化表达式
    expression = (myexpression *)malloc(sizeof(myexpression));
    //答案
    mynum *answer;
    answer = (mynum *)(mynum *)malloc(sizeof(mynum));
    //设置随机数种子
    srand((unsigned)time(NULL));
    //要写入两个文件的字符串
    char *exercisesbuf, *answerbuf;
    exercisesbuf = (char *)malloc(sizeof(char) * BUFSIZ);
    answerbuf = (char *)malloc(sizeof(char) * BUFSIZ);
    int index = 1;
    //生成表达式并写入文件中,表达式数量与count有关
    while(count--){
        //生成一条表达式
        produce_expression(expression, range);
        expression_to_string(expression, exercisesbuf, expression->operator);
        //答案计算
        //遇到负数，删除表达式，重新生成
        if (calculate(expression, answerbuf, answer) == 1){
            ++count;
            continue;
        }
        if(answer->TYPE == FRACTION){
            do_fraction(answer);
            if(answer->denominator == 1){
                answer->TYPE = NATURAL;
                answer->number = answer->numerator;
            }
        }
        //将运算结果写入字符串answerbuf中
        answer_to_string(answer, answerbuf);
        //写入文件
        fprintf(exercisesfile, "%d:%s\n", index, exercisesbuf);
        fprintf(answerfile, "%d:%s\n", index, answerbuf);
        index++;
    }    
    //内存释放
    free(expression);
    free(answer);
    free(answerbuf);
    free(exercisesbuf);
    fclose(exercisesfile);
    fclose(answerfile);
    return 0;
}
