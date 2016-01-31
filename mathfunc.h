/* mathfunc.h -> Mathematical functions for truecube.c */


#define PI  3.1415926535897932

//degree acos (not used)
double dacos(double angle)
{
	return(acos(angle)*(180/PI));
}

//calc norm of two vectors
void cross(double u_i,double u_j,double u_k,double v_i,double v_j,double v_k,double *norm_i,double *norm_j,double *norm_k)
{
	*norm_i=((v_k*u_j)-(v_j*u_k));
	*norm_j=-((v_k*u_i)-(v_i*u_k));
	*norm_k=((v_j*u_i)-(v_i*u_j));
}

//calc dot product of two vectors
double dot(double n_i,double n_j,double n_k,double l_i,double l_j,double l_k)
{
	return((n_i*l_i)+(n_j*l_j)+(n_k*l_k));
}