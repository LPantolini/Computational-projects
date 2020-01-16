#include "my_header.h"

void Euler( void Ydot(double, double*, double*), double t, double h, double *y, int neq )
{
	double R[neq];

	Ydot(t, y, R);

	double k1[neq];
	double k2[neq];
	double Y1[neq];

	for (int i = 0; i < neq; ++i)
	{
		k1[i] = h*R[i];
		Y1[i] = y[i] + k1[i];
	}


	Ydot(t + h, Y1, R);

	for (int i = 0; i < neq; ++i)
	{
		k2[i] = h*R[i];
		y[i] += (k1[i] + k2[i])*0.5;
	}

	return;
}

void Rkutta( void Ydot(double, double*, double*), double t, double h, double *y, int neq )
{
	double R[neq];

	Ydot(t, y, R);

	double k1[neq];
	double k2[neq];
	double Y1[neq];

	for (int i = 0; i < neq; ++i)
	{
		k1[i] = h*R[i];
		Y1[i] = y[i] + k1[i]*0.5;
	}


	Ydot(t + h*0.5, Y1, R);

	for (int i = 0; i < neq; ++i)
	{
		k2[i] = h*R[i];
		y[i] += k2[i];
	}

	return;
}

void Rkutta4( void Ydot(double, double*, double*), double t, double h, double *y, int neq )
{
	double R[neq];

	Ydot(t, y, R);

	double k1[neq];
	double k2[neq];
	double k3[neq];
	double k4[neq];
	double Y[neq];
	
	for (int i = 0; i < neq; ++i)
	{
		k1[i] = h*R[i];
		Y[i] = y[i] + k1[i]*0.5;
	}


	Ydot(t + h*0.5, Y, R);

	for (int i = 0; i < neq; ++i)
	{
		k2[i] = h*R[i];
		Y[i] = y[i] + k2[i]*0.5;
	}

	Ydot(t + h*0.5, Y, R);

	for (int i = 0; i < neq; ++i)
	{
		k3[i] = h*R[i];
		Y[i] = y[i] + k3[i];
	}

	Ydot(t + h, Y, R);

	for (int i = 0; i < neq; ++i)
	{
		k4[i] = h*R[i];
		y[i] += (1.0/6.0)*(k1[i] +2*(k2[i] + k3[i]) + k4[i]);
	}

	return;
}


void VerletVelocity( double* x, double *v, int neq, double dt,
					 void (Acceleration(double*, double* ) ) )
{	
	//acceleration at t = tn
	double a[neq];
	Acceleration(x,a);

	for (int i = 0; i < neq; ++i)
	{
		v[i] += 0.5*dt*a[i]; 
	}

	for (int i = 0; i < neq; ++i)
	{
		x[i] += dt*v[i]; 
	}

	Acceleration(x,a);

	for (int i = 0; i < neq; ++i)
	{
		v[i] += 0.5*dt*a[i];
	}


	return;
}

