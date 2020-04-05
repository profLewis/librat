#ifndef _RPV_
#define _RPV_
/*
**	parameters for volume model
*/

typedef struct{
  double	rho0;
  double 	k;
  double 	bigtheta;
  double 	rhoc;
}Rpv;

double rpv (double thetas, double thetav, double relphi, double rho0, double k, double bigtet, double rhoc);
#endif
