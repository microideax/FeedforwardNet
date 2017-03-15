
#ifndef __POW_FUNCTION_H__
#define __POW_FUNCTION_H__

#include <iostream>

double pow_i(double num, int n)//计算num的n次幂，其中n为整数 
{
	double powint = 1;
	int i;
	for (i = 1; i <= n; i++) powint *= num;
	return powint;
}
double pow_f(double num, double m)//计算num的m次幂，num和m可为双精度，num大于零 
{
	int i, j;
	double powf = 0, x, tmpm = 1;
	x = num - 1;
	for (i = 1; tmpm>1e-12 || tmpm<-1e-12; i++)//当tmpm不在次范围时，停止循环,范围可改 
	{
		for (j = 1, tmpm = 1; j <= i; j++)
			tmpm *= (m - j + 1)*x / j;
		powf += tmpm;
	}
	return powf + 1;
}
double pow_ff(double num, double m)//调用pow_f()和pow_i(),计算num的m次幂,是计算幂的入口
{
	if (num == 0 && m != 0) return 0;//若num为0，则返回0
	else if (num == 0 && m == 0) return 1;// 若num和m都为0，则返回1
	else if (num<0 && m - int(m) != 0) return 0;//若num为负，且m不为整数，则出错，返回0 
	if (num>2)//把底数大于2的情况转为(1/num)^-m计算
	{
		num = 1 / num;
		m = -m;
	}
	if (m<0) return 1 / pow_ff(num, -m);//把指数小于0的情况转为1/num^-m计算
	if (m - int(m) == 0) return pow_i(num, m);/*当指数为浮点数是，分成整数和小数分别求
											  幂，这是因为当底数较小时，用pow_f直接求幂
											  误差大，所以分为指数的整数部分用pow_i,小
											  数部分用pow_f求.*/
	else return pow_f(num, m - int(m))*pow_i(num, int(m));
	return pow_f(num, m);
}
#endif
