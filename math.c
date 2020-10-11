#include "math.h"

//参数处理，-n -r -e -a
void do_args(int argc, char *argv[], int *count, int *range){
    //参数必为偶数个，故argc必为奇数
    if(argc % 2 == 0)
        oops("usage:math.exe [options] [numbers] ...\n", 1);
    char **argvp = &argv[1];
    while(*argvp != NULL){
        if(strcmp(*argvp, "-n") == 0)
            *count = atoi(*(++argvp));
        else if(strcmp(*argvp, "-r") == 0)
            *range = atoi(*(++argvp));
        else
            oops("options:-n -r ...\n", 2);
        ++argvp;
    }
}

int gcd(int a, int b){
    int mod = a % b;
    while(mod != 0){
        a = b;
        b = mod;
        mod = a % b;
    }
    return b;
}

void do_fraction(mynum *num){
    int tmp = 0;
    tmp = gcd(num->numerator, num->denominator);
    num->numerator /= tmp;
    num->denominator /= tmp;
}

void produce_expression(myexpression *expression, int range){
    //运算符的数量,>0同时<=3，同时也得到了运算数的数量
    int opecount = rand() % 3 + 1;//1-3
    expression->operator[opecount] = '\0';
    //将运算符存入数组
    int i = 0;
    while(i < opecount){
        char c;
        //生成运算符+-*/分别对应随机数0123
        switch(rand() % 4){
            case 0: c='+'; break;
            case 1: c='*'; break;
            case 2: c='-'; break;
            case 3: c='/'; break;
        }
        expression->operator[i++] = c;
    }
    i = 0;
    //生成数
    while(i <= opecount){
        if(rand() % 2 == 1)
            expression->nums[i].TYPE = FRACTION;
        else
            expression->nums[i].TYPE = NATURAL;
        //生成自然数
        if(expression->nums[i].TYPE == NATURAL){
            expression->nums[i].number = rand() % range + 1;
            expression->nums[i].numerator = 1;
            expression->nums[i].denominator = 1;
        }
        //生成真分数
        else if(expression->nums[i].TYPE == FRACTION){
            expression->nums[i].number = 0;
            expression->nums[i].denominator = rand() % (range - 1) + 2;
            expression->nums[i].numerator = rand() % (expression->nums[i].denominator - 1) + 1;
            do_fraction(&expression->nums[i]);    
        }
        else
            oops("error:undefined type\n", 1);
        ++i;
    }   
}

void int_to_string(int num, char *buf, int *bufindex){
    int tmp = num % 10;
    num /= 10;
    while(num != 0){
        tmp *= 10;        
        tmp += num % 10;
        num /= 10;
    }
    while(tmp != 0){
        buf[*bufindex] = tmp % 10 + '0';
        tmp /= 10; 
        (*bufindex)++;
    }
    buf[*bufindex] = '\0';
}

void num_to_string(mynum *num, char *exercisesbuf, int *bufindex){
    //加入数
    //真分数
    if(num->TYPE == FRACTION){
        int_to_string(num->numerator, exercisesbuf, bufindex);
        exercisesbuf[(*bufindex)++] = '/';
        int_to_string(num->denominator, exercisesbuf, bufindex);
     }
    //自然数
    else if(num->TYPE == NATURAL){
        int_to_string(num->number, exercisesbuf, bufindex);
    }
    else
        oops("error:undefined type\n", 1);
}

void expression_to_string(myexpression *expression, char *exercisesbuf, char *operator){
    //判断要不要在表达式末尾加右括号
    int isright = 0;
    //缓冲区索引
    int bufindex = 0;
    int index = 0;
    exercisesbuf[bufindex++] = ' ';
    char c;
    //表达式生成
    while((c = operator[index]) != '\0'){
        //特别的，表达式+-后跟*/时，要加括号
        switch (c){
        // * / 后跟+ - 就加右括号
        case '+':     
        case '-':
            if(operator[index + 1] == '*' || operator[index + 1] == '/'){
                exercisesbuf[0] = '(';
                isright = 1;
            }   
            //加入第一个数，加入空格，加入符号，加入空格
            num_to_string(&expression->nums[index], exercisesbuf, &bufindex);
            break;
        case '/':
        case '*':
            num_to_string(&expression->nums[index], exercisesbuf, &bufindex);
            if(isright == 1){
                isright = 0;
                exercisesbuf[bufindex++] = ')';
            }
            break;
        }
        //加入空格和字符和空格
        exercisesbuf[bufindex++] = ' ';
        exercisesbuf[bufindex++] = c;
        exercisesbuf[bufindex++] = ' ';               
        ++index;
    }
    //最后一个数加入字符串
    num_to_string(&expression->nums[index], exercisesbuf, &bufindex);
    //文件结束符
    exercisesbuf[bufindex] = '\0';
}

void answer_to_string(mynum *answer, char *answerbuf){
    answerbuf[0] = ' ';
    int index = 1;
    if(answer->TYPE == NATURAL){
        int_to_string(answer->number, answerbuf, &index);
    }
    else if(answer->TYPE == FRACTION){
        if(answer->numerator > answer->denominator){
            while(answer->numerator > answer->denominator){
                answer->number += (answer->numerator - answer->denominator) ? 1 : 0;
                answer->numerator = answer->numerator - answer->denominator;
            }
            int_to_string(answer->number, answerbuf, &index);
            answerbuf[index++] = '\'';
        }
        int_to_string(answer->numerator, answerbuf, &index);
        answerbuf[index++] = '/';
        int_to_string(answer->denominator, answerbuf, &index);
        answerbuf[index++] = '\0';           
    }
    else
        oops("error:undefined type\n", 1);
    
}

int calculate(myexpression *expression, char *answerbuf, mynum *answer){
    int i = 0;
    answer->TYPE = expression->nums[0].TYPE;
    answer->number = expression->nums[0].number;
    answer->numerator = expression->nums[0].numerator;
    answer->denominator = expression->nums[0].denominator;
    int minus = 0;
    char c;
    while((c = expression->operator[i]) != '\0'){   
        switch (c)
        {
        //运算，只需按逆波兰法读入即可
        //需要判断三种情况，并根据情况约分
        case '+':
            //同为自然数
            if (answer->TYPE == NATURAL && expression->nums[i + 1].TYPE == NATURAL)
                answer->number = answer->number + expression->nums[i + 1].number;
            //同为真分数
            else if (answer->TYPE == FRACTION && expression->nums[i + 1].TYPE == FRACTION){
                //分母相同
                if (answer->denominator == expression->nums[i + 1].denominator)
                    answer->numerator = answer->numerator + expression->nums[i + 1].numerator;
                //分母不同
                else {
                    int tmp = answer->denominator;
                    answer->denominator = answer->denominator * expression->nums[i + 1].denominator;
                    answer->numerator = answer->numerator * expression->nums[i + 1].denominator + expression->nums[i + 1].numerator * tmp;
                }
            }
            //一自然数，一真分数
            else if (answer->TYPE == NATURAL && expression->nums[i+1].TYPE == FRACTION){
                answer->TYPE = FRACTION;
                answer->numerator = answer->number * expression->nums[i+1].denominator + expression->nums[i+1].numerator;
                answer->denominator = expression->nums[i+1].denominator;
                answer->number = 0;
            }
            //一真分数，一自然数
            else if (answer->TYPE == FRACTION && expression->nums[i+1].TYPE == NATURAL){
                answer->numerator = answer->numerator + expression->nums[i+1].number * answer->denominator;
            }
            break;
        case '-':
            //同为自然数
            if (answer->TYPE == NATURAL && expression->nums[i+1].TYPE == NATURAL){
                answer->number = answer->number - expression->nums[i+1].number;
                if (answer->number < 0) minus=1; 
            //同为真分数
            }else if (answer->TYPE == FRACTION && expression->nums[i+1].TYPE == FRACTION){
                //分母相同
                if (answer->denominator == expression->nums[i+1].denominator)
                    answer->numerator = answer->numerator - expression->nums[i+1].numerator;
                //分母不同
                else {
                    int tmp = answer->denominator;
                    answer->denominator = answer->denominator * expression->nums[i + 1].denominator;
                    answer->numerator = answer->numerator * expression->nums[i + 1].denominator - expression->nums[i + 1].numerator * tmp;
                }
                if (answer->numerator < 0) minus=1; 
            }
            //一自然数，一真分数
            else if (answer->TYPE == NATURAL && expression->nums[i+1].TYPE == FRACTION){
                answer->TYPE = FRACTION;
                answer->numerator = answer->number * expression->nums[i+1].denominator - expression->nums[i+1].numerator;
                answer->denominator = expression->nums[i+1].denominator;
                answer->number = 0;
                if (answer->numerator < 0) minus=1; 
            }
            //一真分数，一自然数
            else if (answer->TYPE == FRACTION && expression->nums[i+1].TYPE == NATURAL){
                answer->numerator = answer->numerator - expression->nums[i+1].number * answer->denominator;
                if (answer->numerator < 0) minus=1; 
            }
            break;
        case '*':
            //同为自然数
            if (answer->TYPE == NATURAL && expression->nums[i+1].TYPE == NATURAL)
            answer->number = answer->number * expression->nums[i+1].number;
            //同为真分数
            else if (answer->TYPE == FRACTION && expression->nums[i+1].TYPE == FRACTION){
                answer->denominator = answer->denominator * expression->nums[i+1].denominator;
                answer->numerator = answer->numerator * expression->nums[i+1].numerator;
            }
            //一自然数，一真分数
            else if (answer->TYPE == NATURAL && expression->nums[i+1].TYPE == FRACTION){
                answer->TYPE = FRACTION;
                answer->numerator = answer->number * expression->nums[i+1].numerator;
                answer->denominator = expression->nums[i+1].denominator;
                answer->number = 0;
            }
            //一真分数，一自然数
            else if (answer->TYPE == FRACTION && expression->nums[i+1].TYPE == NATURAL){
                answer->numerator =  expression->nums[i+1].number * answer->numerator;
            }
            break;
        case '/':
            //同为自然数
            if (answer->TYPE == NATURAL && expression->nums[i+1].TYPE == NATURAL){
                //能被整除
                if (answer->number % expression->nums[i+1].number == 0)
                    answer->number = answer->number / expression->nums[i+1].number;
                //不能被整除
                else {
                    answer->TYPE = FRACTION;
                    answer->numerator = answer->number;
                    answer->denominator = expression->nums[i+1].number;
                    answer->number = 0;
                }
            }
            //同为真分数
            else if (answer->TYPE == FRACTION && expression->nums[i+1].TYPE == FRACTION){
                answer->denominator = answer->denominator * expression->nums[i+1].numerator;
                answer->numerator = answer->numerator * expression->nums[i+1].denominator;
            }
            //一自然数，一真分数
            else if (answer->TYPE == NATURAL && expression->nums[i+1].TYPE == FRACTION){
                answer->TYPE = FRACTION;
                answer->numerator = answer->number * expression->nums[i+1].denominator;
                answer->denominator = expression->nums[i+1].numerator;
                answer->number = 0;
            }
            //一真分数，一自然数
            else if (answer->TYPE == FRACTION && expression->nums[i+1].TYPE == NATURAL){
                answer->denominator =  expression->nums[i+1].number * answer->denominator;
            }
            break;
        }
        if (minus) break;
        ++i;
    }
    return minus;
}