#ifndef MY_HEADER_H
#define MY_HEADER_H

#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>

using namespace std;

//root solvers
int Bisection( double(*func)(double), double a, double b, 
			   double xtoll, double ftoll, double &zero, int &ntry );

int R_falsi( double(*func)(double), double a, double b, 
			 double toll, double ftoll, double &zero, int &ntry );

int Secant( double(*func)(double), double a, double b, 
			 double toll, double ftoll, double &zero, int &ntry );

int Newton( double(*func)(double), double(*der)(double), double a, double b, 
			 double toll, double ftoll, double &zero, int &ntry );

void Bracket( double(*func)(double), double a, double b, 
			  int N, int &Nr, double *xL, double *xR );

//ode solvers
void Euler( void Ydot(double, double*, double*), double t, double h, double *y, int neq );

void Rkutta( void Ydot(double, double*, double*), double t, double h, double *y, int neq );

void Rkutta4( void Ydot(double, double*, double*), double t, double h, double *y, int neq );

void VerletVelocity( double* x, double *v, int neq, double dt,
					 void (Acceleration(double*, double* ) ) );


//matrix tools
void show_matrix( double **A, int N );

void show_vector( double *v, int N );

void m_v_dot( double **A, double* v, double* new_v, int N );

void Gauss_el( double **A, double *b, double *x, int N );
//cambia anche le "righe" di un vettore
void Swap_row( double **A, double *b, int row, int col, int N ); 

#endif