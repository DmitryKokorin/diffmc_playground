#include "optics.h"


namespace Optics {

//physical constants

const Vector3 director = Vector3(1., 0., 0.);

const Float eps_par = 3.0;
const Float eps_perp = 2.2;
const Float eps_a = eps_par - eps_perp;
const Float _no = sqrt(eps_perp);


//const Float K3 = 6.1e-7;
const Float K3 = 7.5e-7;
const Float t1 = 0.79;
const Float t2 = 0.43;
const Float K1 = t1*K3;
const Float K2 = t2*K3;

/*const Float sum = 6.1e-7*(1. + 0.79 + 0.43);
const Float K3 = sum*0.7;
const Float K1 = sum*0.2;
const Float K2 = sum*0.1;

const Float t1 = K1/K3;
const Float t2 = K2/K3;
*/

//const Float lambda = 4.88e-5; //cm  scatmc
const Float lambda = 5.145e-5; //stark97, experimental
//const Float lambda = 5.464e-5; //stark97, calculation
//const Float lambda = 4.05e-5;  //wiersma99
//const Float lambda = 5.50e-5;
const Float k0 = 2.*M_PI / lambda;
//const Float xi = 4.2e-4;

const Float T = 301.;
const Float Kb = 1.38e-16;

const Float H = 36.0e+4;
//const Float Hi_alpha = 5e-6;
const Float Hi_alpha = 1.1e-7;
const Float xi = sqrt(K3/Hi_alpha)/H;


const Float c = 29979245800; //cm/s 299 792 458

//precalculated constants

const Float s0 = (0.25*Kb*T*eps_a*eps_a)/(lambda*lambda*K3);
const Float add = 0.25*lambda*lambda/(M_PI*M_PI*xi*xi);

/*void init()
{
    k0 = 2.*M_PI / lambda;
    xi = sqrt(K3/Hi_alpha)/H;
    s0 = (0.25*Kb*T*eps_a*eps_a)/(lambda*lambda*K3);
    add = 0.25*lambda*lambda/(M_PI*M_PI*xi*xi);
}*/

}  //namespace Optics
