
#ifndef __POW_FUNCTION_H__
#define __POW_FUNCTION_H__

#include <iostream>

//double pow_i(double num, int n)//����num��n���ݣ�����nΪ���� 
//{
//	double powint = 1;
//	int i;
//	for (i = 1; i <= n; i++) powint *= num;
//	return powint;
//}
//double pow_f(double num, double m)//����num��m���ݣ�num��m��Ϊ˫���ȣ�num������ 
//{
//	int i, j;
//	double powf = 0, x, tmpm = 1;
//	x = num - 1;
//	for (i = 1; tmpm>1e-12 || tmpm<-1e-12; i++)//��tmpm���ڴη�Χʱ��ֹͣѭ��,��Χ�ɸ� 
//	{
//		for (j = 1, tmpm = 1; j <= i; j++)
//			tmpm *= (m - j + 1)*x / j;
//		powf += tmpm;
//	}
//	return powf + 1;
//}
double pow_ff(double num, double m)//����pow_f()��pow_i(),����num��m����,�Ǽ����ݵ����
{
	if (num == 0 && m != 0) return 0;//��numΪ0���򷵻�0
	else if (num == 0 && m == 0) return 1;// ��num��m��Ϊ0���򷵻�1
	else if (num<0 && m - int(m) != 0) return 0;//��numΪ������m��Ϊ���������������0 

	if (num>2)//�ѵ�������2�����תΪ(1/num)^-m����
	{
		num = 1 / num;
		m = -m;
	}
	if (m < 0) {//��ָ��С��0�����תΪ1/num^-m����
		m = -m;
		int i, j;
		double powf = 0, x, tmpm = 1;
		x = num - 1;
		for (i = 1; tmpm>1e-12 || tmpm<-1e-12; i++)//��tmpm���ڴη�Χʱ��ֹͣѭ��,��Χ�ɸ� 
		{
			for (j = 1, tmpm = 1; j <= i; j++)
				tmpm *= (m - j + 1)*x / j;
			powf += tmpm;
		}
		return 1 / (powf + 1);
	}
	/*��ָ��Ϊ������ʱ���ֳ�������С���ֱ����ݣ�������Ϊ��������Сʱ����pow_fֱ�������������Է�Ϊָ��������������pow_i,С
	��������pow_f��.*/
	if (m - int(m) == 0) {
		double powint = 1;
		int i;
		for (i = 1; i <= int(m); i++) powint *= num;
		return powint;
	}
	else {
		//С������
		int i, j;
		double powf = 0, x, tmpm = 1;
		x = num - 1;
		for (i = 1; tmpm>1e-12 || tmpm<-1e-12; i++)//��tmpm���ڴη�Χʱ��ֹͣѭ��,��Χ�ɸ� 
		{
			for (j = 1, tmpm = 1; j <= i; j++)
				tmpm *= (m - int(m) - j + 1)*x / j;
			powf += tmpm;
		}
		//��������
		double powint = 1;
		int k;
		for (k = 1; k <= int(m); k++) powint *= num;
		return (powf + 1) * powint;
	}

	int i, j;
	double powf = 0, x, tmpm = 1;
	x = num - 1;
	for (i = 1; tmpm>1e-12 || tmpm<-1e-12; i++)//��tmpm���ڴη�Χʱ��ֹͣѭ��,��Χ�ɸ� 
	{
		for (j = 1, tmpm = 1; j <= i; j++)
			tmpm *= (m - j + 1)*x / j;
		powf += tmpm;
	}
	return powf + 1;
}
#endif
