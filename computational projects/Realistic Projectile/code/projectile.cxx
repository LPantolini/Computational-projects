#include "my_header.h"

#define STAGE 1

double B = 4e-05;
double g = 9.81;
double g_v; 
double g_L;
double g_altitude = 0;


void Ydot( double t, double *V, double *R );
double Residual( double theta );

int main()
{
	cout << setiosflags(ios::scientific);
	cout << setprecision(12);

#if STAGE == 1
{	
	cout <<"Insert the distance from the target:" << endl;
	cin >> g_L;

	cout <<"Insert the magnitude of the initial velocity: " << endl;
	cin >> g_v;
	
	//variables for the function Bracket
	double a;
	if ( g_altitude > 0 ) a = -M_PI/2;
	else a = 0;
	
	double b = M_PI/2;
	int N = 100;
	int Nr; 	

	double xL[100];
	double xR[100];

	double xtoll = 1e-10;
	double ftoll = 1e-10;

	double zero;
	int ntry;

	//the number of angles is at plus 2
	double angles[2];

	Bracket(*Residual, a, b, N, Nr, xL, xR);

	int find;

	for (int i = 0; i < Nr; i++)
	{
		
		find = Bisection(*Residual, xL[i], xR[i], xtoll, ftoll, zero, ntry);

		if ( find )
		{
			angles[i] = zero;
			cout <<"In order to hit the target you have to orient the cannon at: " << zero << endl;

		}

		else
		{
			cout <<"No zeros was found" << endl;
		}
	}


	//check if the target is at maximum range
	if( Nr == 0 )
	{
		double Nh = 1000;
		//maximum theta in vacuum
		double theta = M_PI/4.0;
		double h = theta/(Nh-1);

		double L = 0;
		double L_max = 0;
		double theta_max;

		for (int i = 0; i < Nh; i++)
		{
			L = Residual(theta) + g_L;

			if ( L > L_max )
			{
				L_max = L;
				theta_max = theta;
			}

			//if decreases I've already found L_max
			else if (L < L_max)		
			{
				break;
			}

			theta -= h;
		}

		double toll_L = L_max*1e-03;
		double diff = fabs(g_L - L_max);

		if ( diff < toll_L )
		{
			angles[0] = theta_max;
			angles[1] = 0;
			cout <<"The target is at the maximum range." << endl;
			cout <<"You have to orient the cannon at: " << theta_max << endl;

		}

		else
		{
			angles[0] = 0;
			angles[1] = 0;
			cout <<"The target is out of range"<< endl;
			cout <<"L max = " << L_max << endl;
		}
		
	}

	//now we compute the trajectory
	ofstream file("trajectory.dat", ios::out);
	
	for (int i = 0; i < 2; i++)
	{
		if( angles[i] != 0 )
		{
			int n_eq = 4;     	
			double Y[n_eq];
	
			Y[0] = 0;
			Y[1] = g_altitude;
			Y[2] = g_v*cos(angles[i]);
			Y[3] = g_v*sin(angles[i]);

			double t = 0;
			//h = temporal step
			double h = 1e-03; 

			do
			{
				t += h;
				Rkutta4( *Ydot, t, h, Y, n_eq );

				file << t <<" "<< Y[0] <<" " << Y[1] << endl;
	
			}while( Y[1] > 0 );
		}

		file <<"\n\n";
	}
  						

}

#endif


#if STAGE == 2
{	//compute distances in function of theta

	cout <<"Insert the magnitude of the initial velocity: " << endl;
	cin >> g_v;

	double Nh = 1000;
	double b = M_PI;
	double h = b/(Nh-1);
	double theta = -M_PI*0.5;
	double L = 0;

	ofstream file("distances.dat", ios::out);

	for (int i = 0; i < Nh; i++)
	{
		L = Residual(theta) + g_L;
		file << theta <<" "<< L << endl;

		theta += h;
	}

}
#endif


#if STAGE == 3
{
	double v_max;
	cout <<"Insert the magnitude of the maximum velocity: " << endl;
	cin >> v_max;

	double v_inc;
	cout <<"Insert the magnitude of the velocity increment: " << endl;
	cin >> v_inc;

	double v = 0;
	double Nh = 1000;
	double b = M_PI/2;
	double h = b/(Nh-1);
	double L;
	double L_max;
	double theta_max;
	double theta = 0;


	ofstream file2("theta_max.dat", ios::out);

	while( v < v_max )
	{
		L_max = 0;
		theta = 0;
		g_v = v;

		for (int i = 0; i < Nh; i++)
		{
			L = Residual(theta) + g_L;

			if (L > L_max)
			{
				L_max = L;
				theta_max = theta;
			}	

			theta += h;
		}

		file2 << v <<" "<< theta_max <<" "<< L_max << endl;

		v += v_inc;
		cout << v << endl;
	}


}
#endif


return 0;

}


void Ydot( double t, double *Y, double *R )
{
	double vx = Y[2];
	double vy = Y[3];
	double v = sqrt(vx*vx + vy*vy);

	R[0] = vx;
	R[1] = vy;
	R[2] = - B*v*vx;
	R[3] = - g - B*v*vy;


	return;
}


double Residual( double theta )
{
	int n_eq = 4;     		

	double Y[n_eq];
	
	Y[0] = 0;
	Y[1] = g_altitude;
	Y[2] = g_v*cos(theta);
	Y[3] = g_v*sin(theta);

	double t = 0;
	double h = 1e-03;	

	do
	{
		t += h;
		Rkutta4( *Ydot, t, h, Y, n_eq );
	
	}while( Y[1] > 0 );


	return (Y[0] - g_L);  						

}