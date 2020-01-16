#include "my_header.h"



int Bisection( double(*func)(double), double a, double b, 
			   double xtoll, double ftoll, double &zero, int &ntry )
{

	int N = 1;

	double c = 0.5*(a+b);
	double f_c = func(c);
	double f_a = func(a);
	double f_b = func(b);

	do
	{

		if( f_a*f_c < 0 )
		{
			b = c;
			f_b = f_c;
		}

		else if( f_b*f_c < 0 )
		{
			a = c;
			f_a = f_c;
		}

		c = 0.5*(a+b);
		f_c = func(c);

		if( fabs(a-b) < xtoll || fabs(f_c) < ftoll ) //condizione di uscita
		{
			zero = c;
			ntry = N;
			return 1;
		}

		if( N > 100 )
		{
			cout <<"superate le 100 iterazioni"<< endl;
			break;
		}

		N++;

	}while(true);


	return 0;		//se esce dal ciclo è per un errore
}






int R_falsi( double(*func)(double), double a, double b, 
			   double toll, double ftoll, double &zero, int &ntry )
{

	int N = 1;
	double del;


	if( func(a)*func(b) > 0 )
	{
		cout <<"error"<<endl;
		return 0;
	}

	do
	{
		double f_a = func(a);
		double f_b = func(b);

		double c = ( f_b*a - f_a*b )/(f_b - f_a);
		double f_c = func(c);

		if( f_a*f_c < 0 )
		{
			del= c - b;
			b = c;
			//f_c = f_b;	
		}

		else if( f_b*f_c < 0 )
		{
			del= c - a;
			a = c;
			//f_c = f_a;
		}


		if( fabs(del) < toll || fabs(func(c)) < ftoll ) //condizione di uscita
		{
			zero = c;
			ntry = N;
			return 1;
		}

		if( N > 100 )
		{
			cout <<"too mutch interactions"<< endl;
			break;
		}

		
		N++;

	}while(true);
	

	return 0;		//se esce dal ciclo è per un errore
}






int Secant( double(*func)(double), double a, double b, 
			   double toll, double ftoll, double &zero, int &ntry )
{

	int N = 1;

	double k1 = a;
	double k2 = b;


	double f_k1 = func(k1);
	double f_k2 = func(k2);

	double k3 = k2 - f_k2*( k2 - k1 )/(f_k2 - f_k1);
	//double f_k3 = func(k3);

	do
	{

		if( fabs(k2 - k3) < toll || fabs(func(k3)) < ftoll ) //condizione di uscita
		{
			zero = k3;
			ntry = N;
			return 1;
		}

		if( N > 100 )
		{
			cout <<"too mutch interactions"<< endl;
			break;
		}

		
		N++;

		k1 = k2;
		k2 = k3;

		f_k1 = func(k1);
		f_k2 = func(k2);

		
		k3 = k2 - f_k2*( k2 - k1 )/(f_k2 - f_k1);
	
	

	}while(true);
	

	return 0;	//se esce dal ciclo è per un errore
}





int Newton( double(*func)(double), double(*der)(double), double a, double b, 
			 double toll, double ftoll, double &zero, int &ntry )
{
	double k;
	int N = 1;

	double x_m = 0.5*(a+b);


	while(true)
	{
		k = x_m - func(x_m)/der(x_m);

		if( fabs(k-b) < toll || fabs(func(k)) < ftoll )
		{
			zero = k;
			ntry = N;

			return 1;

		}
		//cout <<"k = " << k << endl;
		if(N > 100)
		{
			cout <<"too mutch iteration"<< endl;
			break;
		}

		x_m = k;
		N++;
	}

	return 0;
}


void Bracket(double(*func)(double), double a, double b, 
			  int N, int &Nr, double *xL, double *xR)

{
	double xl;
	double xr;
	double del = fabs(b -a)/N;
	int k = 0;

	double f_a = func(a);
	double f_b;

	for (int i = 0; i < N; ++i)
	{
		xl = a + del*i;
		xr = a + del*(i+1);

		f_b = func(xr);


		if( f_a*f_b < 0 )
		{
			xL[k] = xl;
			xR[k] = xr;

			k++;
		}

		f_a = f_b;
	}

	Nr = k;

	return;
}
