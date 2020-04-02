#include	<stdio.h>
#include	<math.h>
#include	"verstraete.h"

#define DTOR(alpha)     ((alpha)*M_PI/180.0)
#define RTOD(alpha)     ((alpha)*180.0/M_PI)
main(){
	double	brdf;
	double	mvbp1_();
	Verstraete_model	verstraete;

	verstraete.theta[0]=DTOR(10.0);
	verstraete.theta[1]=DTOR(20.0);
	verstraete.theta[2]=DTOR(0.0);
	
	verstraete.model_type = 1;
	verstraete.kappa_flag = 1;
	verstraete.phase_model = 1;
	verstraete.scattering_model = 1;

	verstraete.kappa[0] = 0.01;
	verstraete.kappa[1] = 1.0;
	verstraete.kappa[2] = 0.2;
	verstraete.kappa[3] = 0.0;

	verstraete.optics[0] = 0.8;
	verstraete.optics[1] = -0.4;
	verstraete.optics[2] = 0.0;
	
	init_verstraete(&verstraete);
    	brdf = mvbp1_(&verstraete);
	fprintf(stdout,"%f\n",(float)brdf);
}
