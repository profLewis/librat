#include <math.h>
double rpv (double thetas, double thetav, double relphi, double rho0, double k, double bigtet, double rhoc){

/*      REAL FUNCTION rpv (thetas, thetav, relphi, rho0, k,
     &   bigtet, rhoc)
C
C***********************************************************************
C
C  Function RPV
C
C  Version 1.1, 28 February 2001
C
C  Author: Michel M. Verstraete
C          Space Applications Institute (SAI)
C          EC Joint Research Centre, TP 440
C          I-21020 Ispra (VA), Italy
C          E-mail: Michel.Verstraete@jrc.it
C
C  MODIFIED: 11/2008 JLW: enable 4 parameter version of rpv
c
C  Purpose:
C
C  To return the value of the bidirectional reflectance factor for an
C  arbitrary geophysical medium, given the illumination and observation
C  geometry, as well as the values of three coefficients describing the
C  anisotropy of that medium, as computed with the RPV model.
C
C  Computer language:
C
C  FORTRAN-77
C
C  Argument(s):
C
      REAL         thetas, thetav, relphi, rho0, k, bigtet, rhoc
C
C  Definitions:
C
C  thetas          Illumination zenith angle, in radians.
C                  The code verifies that thetas is within the
C                  range equivalent to [100, 180] degrees.
C  thetav          Observation zenith angle, in radians.
C                  The code verifies that thetav is within the
C                  range equivalent to [0, 80] degrees.
C  relphi          Relative azimuth angle, in radians.
C                  The code verifies that relphi is within the
C                  range equivalent to [0, 180] degrees, where 0
C                  corresponds to forward and 180 to backward
C                  scattering (hot spot).
C  rho0            Overall reflectance level parameter, N/D.
C  k               Anisotropy shape parameter, N/D.
C                  k < 1.0 (> 1.0) corresponds to a bowl (bell) shape.
C  bigtet          Asymmetry parameter, N/D.
C                  bigtet < 0.0 (> 0.0) corresponds to a
C                  predominantly backward (forward) scattering.
C  rhoc            hotspot function parameter (1.0 = no hotspot), N/D.
C  verbos          Flag to print out input, intermediary and
C                  output values on standard output for
C                  diagnostic purposes. Please note that this setting
C                  has no effect if an error is detected (see below).
C  err             Error code. This variable is set to a non-zero value
C                  whenever an erroneous input or a computational error
C                  is detected.
C                  err = 0 under normal conditions.
C                  err = 10 and rpv = -1.0 for invalid thetas.
C                  err = 20 and rpv = -1.0 for invalid thetav.
C                  err = 30 and rpv = -1.0 for invalid relphi.
C                  err = 100 and rpv = -1.0 if the denominator of the
C                  Henyey and Greenstein phase function vanishes.
C
C  Remarks:
C
C  1. All angles are defined between oriented directions.
C     The local vertical is always oriented upwards.
C     Whenever a direction corresponds to the propagation of
C     radiation, it is oriented in that direction. Hence, direct solar
C     radiation propagates downward (with zenith angles larger
C     than 90 deg) , and rays contributing to the observation
C     propagate upward (with zenith angles smaller than 90 deg).
C  2. Azimuth angles are defined in the usual mathematical sense,
C     i.e., counted positively in a counterclockwise rotation
C     from the x-axis (Eastward) towards the y-axis (Northward).
C     For this RPV model, conventions are less important because only
C     the relative azimuth between the illumination and observation
C     directions is used as argument to an even function (cos).
C     Hence, only the absolute value of the azimuth difference matters.
C  3. WARNING: All variables in this routine are declared explicitly,
C     and their names have been selected to correspond to those
C     of the mathematical variables in the appropriate reference.
C     DO NOT ASSUME THAT A VARIABLE IS intEGER OR REAL BASED ON
C     ITS FIRST LETTER.
C
C  Disclaimer: 
C
C     Every effort has been made to produce a computer programme
C     as efficient and correct as possible. Nevertheless, the
C     authors make no warranty, express or implied, that this
C     routine is free of error, or is consistent with any particular
C     standard of merchantability, or that it will meet your
C     requirements for any particular application. The authors disclaim
C     all liability for direct or consequential damages resulting from
C     your use of this routine.
C
C
C  Copyright (C) 2001 Michel M. Verstraete:
C
C     This code is made available publicly for scientific purposes.
C     Nevertheless, the copyright remains with its author, and any
C     use of this code must be accompanied by appropriate and usual
C     acknowledgments, by referring to the origin of the source code
C     and to the publications indicated below. If your students or
C     colleagues also need to access this code for research purposes,
C     please refer them to one of the official source sites (e.g.,
C     http://www.enamors.org/), where they can be sure to download
C     the ultimate version.
C     Use of this code (RPV) in commercial applications is strictly
C     forbidden without the written approval of the author. Even with
C     such an authorization to use the code, you may not distribute
C     or sell it to any other commercial or business partners under
C     any circumstances.
C
C  References:
C
C  1. Rahman, H., Verstraete M. M. and Pinty B. (1993) `Coupled
C     Surface-Atmosphere Reflectance (CSAR) Model. Part 1: Model
C     Description and Inversion on Synthetic Data', Journal of
C     Geophysical Research, 98, 20,779-20,789.
C  2. Rahman, H., Pinty B. and Verstraete M. M. (1993) `Coupled 
C     Surface-Atmosphere Reflectance (CSAR) Model. Part 2: 
C     Semiempirical Surface Model Usable With NOAA Advanced Very High
C     Resolution Radiometer Data', Journal of Geophysical Research, 98,
C     20,791-20,801.
C
C  Copies of these two papers can be obtained from Michel M. Verstraete
C  at the address above.
C
C  History of releases:
C
C  Date        Version  Notes
C  ====        =======  =====
C  1993-07-01  1.0      Initial publication of the model.
C  2001-02-28  1.1      Certification, documentation, and public
C                       release of this code.
C  2010-04-09  X.X      Translation to C code: plewis@geog.ucl.ac.uk
C***********************************************************************
C
C  Declaration of local variables:
C
      REAL coss, cosv, sins, sinv, cosp, tans, tanv
      REAL bgthsq, csmllg, bigg, expon, denom, f1, f2, f3
      REAL r80, r100, r180
      intRINSIC ABS, COS, SQRT
      DATA r80 /1.396263402/, r100 /1.745329252/, r180 /3.141592654/
C
C  Initialize the error code and check input values:
C
C      err = 0
C      IF ((thetas .LT. r100) .OR. (thetas .GT. r180)) THEN
C         err = 10
C         rpv = -1.0
C         RETURN
C      ENDIF
C      IF ((thetav .LT. 0.0) .OR. (thetav .GT. r80)) THEN
C         err = 20
C         rpv = -1.0
C         RETURN
C      ENDIF
C      IF ((relphi .LT. 0.0) .OR. (relphi .GT. r180)) THEN
C         err = 30
C         rpv = -1.0
C         RETURN
C      ENDIF
C
C  Compute the trigonometric functions:
C
      coss = ABS (COS (thetas))
      cosv = COS (thetav)
      if (coss .eq. 0) then
        coss = 1e-20;
      endif
      if (cosv .eq. 0) then
        cosv = 1e-20;
      endif
      sins = SQRT (1.0 - coss * coss)
      sinv = SQRT (1.0 - cosv * cosv)
      cosp = -COS (relphi)
      tans = sins / coss
      tanv = sinv / cosv
      csmllg = coss * cosv + sins * sinv * cosp
      bigg = SQRT (tans * tans + tanv * tanv -
     &   2.0 * tans * tanv * cosp)
      bgthsq = bigtet * bigtet
*/
      double coss,cosv,sins,sinv,cosp,tans,tanv,csmllg,bigg,bgthsq;
      double expon,f1,denom,f2,f3,rpv;

      coss = fabs(cos(thetas));
      cosv = cos(thetav);
      if (coss == 0) coss = 1e-20;
      if (cosv == 0) cosv = 1e-20;
      sins = sqrt(1.0 - coss * coss);
      sinv = sqrt(1.0 - cosv * cosv);
      cosp = -cos(relphi);
      tans = sins / coss;
      tanv = sinv / cosv;
      csmllg = coss * cosv + sins * sinv * cosp;
      bigg = sqrt(tans * tans + tanv * tanv -  2.0 * tans * tanv * cosp);
      bgthsq = bigtet * bigtet;
      expon = k - 1.0;
      if  (expon != 0.0) f1 = pow(coss * cosv,expon) * pow(coss + cosv,expon);
      else f1 = 1.0;
      denom = pow(1.0 + bgthsq + 2.0 * bigtet * csmllg,1.5);
      if (denom != 0.0) f2 = (1.0 - bgthsq) / denom;
      else f2 = (1.0 - bgthsq)*1e20;
      f3 = (1.0 + ((1 - rhoc) / (1.0 + bigg)));
      rpv = rho0 * f1 * f2 * f3;
      return(rpv);

/*
C
C  Compute the Minnaert-like factor:
C
      expon = k - 1.0
      IF (expon .NE. 0.0) THEN
         f1 = ((coss * cosv) ** expon) *
     &      ((coss + cosv) ** expon)
      ELSE
         f1 = 1.0
      ENDIF
C
C  Compute the asymmetry function (Henyey and Greenstein):
C
      denom = (1.0 + bgthsq + 2.0 * bigtet * csmllg) ** (1.5)
      IF (denom .NE. 0.0) THEN
         f2 = (1.0 - bgthsq) / denom
      ELSE
         f2 = (1.0 - bgthsq)*1e20;
C         err = 100
C         rpv = -1.0
C         RETURN
      ENDIF
C
C  Compute the hotspot function
C
      f3 = (1.0 + ((1 - rhoc) / (1.0 + bigg)))
C
C  Compute the overall RPV output
C     
      rpv = rho0 * f1 * f2 * f3
C
C  Remark:
C
C  The following IF-ENDIF block is useful for diagnostic purposes
C  only, and should not be used under operational conditions.
C  Whenever run-time efficiency is a prime consideration,
C  1. remove the 'verbos' variable from the argument list,
C  2. delete the associated definition, and
C  3. delete the following IF-ENDIF block.
C
C
      RETURN
      END
*/
}
