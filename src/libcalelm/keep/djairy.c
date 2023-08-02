/* djairy.f -- translated by f2c (version 19961017).
 *
 *
 */

#include "calpack.h"

/*DECK DJAIRY 
 */
int
nsp_calpack_djairy (double *x, double *rx, double *c__, double *ai,
		    double *dai)
{
  /* Initialized data */

  static int n1 = 14;
  static double con2 = 5.03154716196777;
  static double con3 = .380004589867293;
  static double con4 = .833333333333333;
  static double con5 = .866025403784439;
  static double ak1[14] =
    { .220423090987793, -.1252902427877, .0103881163359194,
    8.22844152006343e-4, -2.34614345891226e-4, 1.63824280172116e-5,
    3.06902589573189e-7,
    -1.29621999359332e-7, 8.22908158823668e-9, 1.53963968623298e-11,
    -3.39165465615682e-11,
    2.03253257423626e-12, -1.10679546097884e-14, -5.1616949778508e-15
  };
  static double ak2[23] =
    { .274366150869598, .00539790969736903, -.0015733922062119,
    4.2742752824875e-4, -1.12124917399925e-4, 2.88763171318904e-5,
    -7.36804225370554e-6,
    1.87290209741024e-6, -4.75892793962291e-7, 1.21130416955909e-7,
    -3.09245374270614e-8,
    7.92454705282654e-9, -2.03902447167914e-9, 5.26863056595742e-10,
    -1.36704767639569e-10,
    3.56141039013708e-11, -9.3138829654843e-12, 2.44464450473635e-12,
    -6.43840261990955e-13,
    1.70106030559349e-13, -4.50760104503281e-14, 1.19774799164811e-14,
    -3.19077040865066e-15
  };
  static double ak3[14] =
    { .280271447340791, -.00178127042844379, 4.03422579628999e-5,
    -1.63249965269003e-6, 9.21181482476768e-8, -6.52294330229155e-9,
    5.47138404576546e-10,
    -5.2440825180026e-11, 5.60477904117209e-12, -6.56375244639313e-13,
    8.31285761966247e-14,
    -1.12705134691063e-14, 1.62267976598129e-15, -2.46480324312426e-16
  };
  static double ajp[19] =
    { .0778952966437581, -.184356363456801, .0301412605216174,
    .0305342724277608, -.00495424702513079, -.00172749552563952,
    2.4313763783919e-4,
    5.04564777517082e-5, -6.16316582695208e-6, -9.03986745510768e-7,
    9.70243778355884e-8,
    1.09639453305205e-8, -1.04716330588766e-9, -9.60359441344646e-11,
    8.25358789454134e-12,
    6.36123439018768e-13, -4.96629614116015e-14, -3.29810288929615e-15,
    2.35798252031104e-16
  };
  static double ajn[19] =
    { .0380497887617242, -.245319541845546, .165820623702696,
    .0749330045818789, -.0263476288106641, -.00592535597304981,
    .00144744409589804,
    2.18311831322215e-4, -4.10662077680304e-5, -4.66874994171766e-6,
    7.1521880727716e-7,
    6.52964770854633e-8, -8.44284027565946e-9, -6.44186158976978e-10,
    7.20802286505285e-11,
    4.72465431717846e-12, -4.66022632547045e-13, -2.67762710389189e-14,
    2.36161316570019e-15
  };
  static double a[15] =
    { .490275424742791, .00157647277946204, -9.66195963140306e-5,
    1.35916080268815e-7, 2.98157342654859e-7, -1.86824767559979e-8,
    -1.03685737667141e-9,
    3.28660818434328e-10, -2.5709141063278e-11, -2.32357655300677e-12,
    9.57523279048255e-13,
    -1.20340828049719e-13, -2.90907716770715e-15, 4.55656454580149e-15,
    -9.99003874810259e-16
  };
  static int n2 = 23;
  static double b[15] =
    { .278593552803079, -.00352915691882584, -2.31149677384994e-5,
    4.7131784226356e-6, -1.12415907931333e-7, -2.00100301184339e-8,
    2.60948075302193e-9,
    -3.55098136101216e-11, -3.50849978423875e-11, 5.83007187954202e-12,
    -2.04644828753326e-13,
    -1.10529179476742e-13, 2.87724778038775e-14, -2.88205111009939e-15,
    -3.32656311696166e-16
  };
  static int n1d = 14;
  static int n2d = 24;
  static int n3d = 19;
  static int n4d = 15;
  static int m1d = 12;
  static int m2d = 22;
  static int m3d = 17;
  static int m4d = 13;
  static double dak1[14] =
    { .204567842307887, -.0661322739905664, -.00849845800989287,
    .00312183491556289, -2.70016489829432e-4, -6.35636298679387e-6,
    3.02397712409509e-6,
    -2.18311195330088e-7, -5.36194289332826e-10, 1.1309803562231e-9,
    -7.43023834629073e-11,
    4.28804170826891e-13, 2.23810925754539e-13, -1.39140135641182e-14
  };
  static int n3 = 19;
  static double dak2[24] =
    { .29333234388323, -.00806196784743112, .0024254017233314,
    -6.82297548850235e-4, 1.85786427751181e-4, -4.97457447684059e-5,
    1.32090681239497e-5,
    -3.49528240444943e-6, 9.24362451078835e-7, -2.44732671521867e-7,
    6.4930783764891e-8,
    -1.72717621501538e-8, 4.60725763604656e-9, -1.2324905529155e-9,
    3.30620409488102e-10,
    -8.89252099772401e-11, 2.39773319878298e-11, -6.4801392115345e-12,
    1.75510132023731e-12,
    -4.76303829833637e-13, 1.2949824110081e-13, -3.5267962221043e-14,
    9.62005151585923e-15,
    -2.62786914342292e-15
  };
  static double dak3[14] =
    { .284675828811349, .0025307307261908, -4.83481130337976e-5,
    1.84907283946343e-6, -1.01418491178576e-7, 7.05925634457153e-9,
    -5.85325291400382e-10,
    5.56357688831339e-11, -5.908890947795e-12, 6.88574353784436e-13,
    -8.68588256452194e-14,
    1.17374762617213e-14, -1.68523146510923e-15, 2.55374773097056e-16
  };
  static double dajp[19] =
    { .0653219131311457, -.120262933688823, .00978010236263823,
    .0167948429230505, -.00197146140182132, -8.45560295098867e-4,
    9.42889620701976e-5,
    2.25827860945475e-5, -2.29067870915987e-6, -3.76343991136919e-7,
    3.45663933559565e-8,
    4.29611332003007e-9, -3.58673691214989e-10, -3.57245881361895e-11,
    2.72696091066336e-12,
    2.26120653095771e-13, -1.58763205238303e-14, -1.12604374485125e-15,
    7.31327529515367e-17
  };
  static double dajn[19] =
    { .0108594539632967, .0853313194857091, -.315277068113058,
    -.0878420725294257, .0553251906976048, .00941674060503241,
    -.00332187026018996,
    -4.11157343156826e-4, 1.01297326891346e-4, 9.87633682208396e-6,
    -1.87312969812393e-6,
    -1.50798500131468e-7, 2.32687669525394e-8, 1.59599917419225e-9,
    -2.07665922668385e-10,
    -1.24103350500302e-11, 1.39631765331043e-12, 7.3940097115574e-14,
    -7.328874756275e-15
  };
  static double da[15] =
    { .491627321104601, .00311164930427489, 8.23140762854081e-5,
    -4.61769776172142e-6, -6.13158880534626e-8, 2.8729580465652e-8,
    -1.81959715372117e-9,
    -1.44752826642035e-10, 4.53724043420422e-11, -3.99655065847223e-12,
    -3.24089119830323e-13,
    1.62098952568741e-13, -2.40765247974057e-14, 1.69384811284491e-16,
    8.17900786477396e-16
  };
  static double db[15] =
    { -.277571356944231, .0044421283341992, -8.42328522190089e-5,
    -2.5804031841871e-6, 3.42389720217621e-7, -6.24286894709776e-9,
    -2.36377836844577e-9,
    3.16991042656673e-10, -4.40995691658191e-12, -5.18674221093575e-12,
    9.64874015137022e-13,
    -4.9019057660871e-14, -1.77253430678112e-14, 5.55950610442662e-15,
    -7.1179333757953e-16
  };
  static int n4 = 15;
  static int m1 = 12;
  static int m2 = 21;
  static int m3 = 17;
  static int m4 = 13;
  static double fpi12 = 1.30899693899575;

  /* System generated locals */
  int i__1;

  /* Local variables */
  double rtrx, temp1, temp2;
  int i__, j;
  double t, e1, e2, f1, f2, ec, cv, tt, ccv, scv;

  /****BEGIN PROLOGUE  DJAIRY 
   ****SUBSIDIARY 
   ****PURPOSE  Subsidiary to DBESJ and DBESY 
   ****LIBRARY   SLATEC 
   ****TYPE      DOUBLE PRECISION (JAIRY-S, DJAIRY-D) 
   ****AUTHOR  Amos, D. E., (SNLA) 
   *          Daniel, S. L., (SNLA) 
   *          Weston, M. K., (SNLA) 
   ****DESCRIPTION 
   * 
   *                 DJAIRY computes the Airy function AI(X) 
   *                  and its derivative DAI(X) for DASYJY 
   * 
   *                                  INPUT 
   * 
   *        X - Argument, computed by DASYJY, X unrestricted 
   *       RX - RX=SQRT(ABS(X)), computed by DASYJY 
   *        C - C=2.*(ABS(X)**1.5)/3., computed by DASYJY 
   * 
   *                                 OUTPUT 
   * 
   *       AI - Value of function AI(X) 
   *      DAI - Value of the derivative DAI(X) 
   * 
   ****SEE ALSO  DBESJ, DBESY 
   ****ROUTINES CALLED  (NONE) 
   ****REVISION HISTORY  (YYMMDD) 
   *  750101  DATE WRITTEN 
   *  890531  Changed all specific intrinsics to generic.  (WRB) 
   *  891009  Removed unreferenced variable.  (WRB) 
   *  891214  Prologue converted to Version 4.0 format.  (BAB) 
   *  900328  Added TYPE section.  (WRB) 
   *  910408  Updated the AUTHOR section.  (WRB) 
   ****END PROLOGUE  DJAIRY 
   * 
   */
  /****FIRST EXECUTABLE STATEMENT  DJAIRY 
   */
  if (*x < 0.)
    {
      goto L90;
    }
  if (*c__ > 5.)
    {
      goto L60;
    }
  if (*x > 1.2)
    {
      goto L30;
    }
  t = (*x + *x - 1.2) * con4;
  tt = t + t;
  j = n1;
  f1 = ak1[j - 1];
  f2 = 0.;
  i__1 = m1;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      --j;
      temp1 = f1;
      f1 = tt * f1 - f2 + ak1[j - 1];
      f2 = temp1;
      /* L10: */
    }
  *ai = t * f1 - f2 + ak1[0];
  /* 
   */
  j = n1d;
  f1 = dak1[j - 1];
  f2 = 0.;
  i__1 = m1d;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      --j;
      temp1 = f1;
      f1 = tt * f1 - f2 + dak1[j - 1];
      f2 = temp1;
      /* L20: */
    }
  *dai = -(t * f1 - f2 + dak1[0]);
  return 0;
  /* 
   */
L30:
  t = (*x + *x - con2) * con3;
  tt = t + t;
  j = n2;
  f1 = ak2[j - 1];
  f2 = 0.;
  i__1 = m2;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      --j;
      temp1 = f1;
      f1 = tt * f1 - f2 + ak2[j - 1];
      f2 = temp1;
      /* L40: */
    }
  rtrx = sqrt (*rx);
  ec = exp (-(*c__));
  *ai = ec * (t * f1 - f2 + ak2[0]) / rtrx;
  j = n2d;
  f1 = dak2[j - 1];
  f2 = 0.;
  i__1 = m2d;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      --j;
      temp1 = f1;
      f1 = tt * f1 - f2 + dak2[j - 1];
      f2 = temp1;
      /* L50: */
    }
  *dai = -ec * (t * f1 - f2 + dak2[0]) * rtrx;
  return 0;
  /* 
   */
L60:
  t = 10. / *c__ - 1.;
  tt = t + t;
  j = n1;
  f1 = ak3[j - 1];
  f2 = 0.;
  i__1 = m1;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      --j;
      temp1 = f1;
      f1 = tt * f1 - f2 + ak3[j - 1];
      f2 = temp1;
      /* L70: */
    }
  rtrx = sqrt (*rx);
  ec = exp (-(*c__));
  *ai = ec * (t * f1 - f2 + ak3[0]) / rtrx;
  j = n1d;
  f1 = dak3[j - 1];
  f2 = 0.;
  i__1 = m1d;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      --j;
      temp1 = f1;
      f1 = tt * f1 - f2 + dak3[j - 1];
      f2 = temp1;
      /* L80: */
    }
  *dai = -rtrx * ec * (t * f1 - f2 + dak3[0]);
  return 0;
  /* 
   */
L90:
  if (*c__ > 5.)
    {
      goto L120;
    }
  t = *c__ * .4 - 1.;
  tt = t + t;
  j = n3;
  f1 = ajp[j - 1];
  e1 = ajn[j - 1];
  f2 = 0.;
  e2 = 0.;
  i__1 = m3;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      --j;
      temp1 = f1;
      temp2 = e1;
      f1 = tt * f1 - f2 + ajp[j - 1];
      e1 = tt * e1 - e2 + ajn[j - 1];
      f2 = temp1;
      e2 = temp2;
      /* L100: */
    }
  *ai = t * e1 - e2 + ajn[0] - *x * (t * f1 - f2 + ajp[0]);
  j = n3d;
  f1 = dajp[j - 1];
  e1 = dajn[j - 1];
  f2 = 0.;
  e2 = 0.;
  i__1 = m3d;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      --j;
      temp1 = f1;
      temp2 = e1;
      f1 = tt * f1 - f2 + dajp[j - 1];
      e1 = tt * e1 - e2 + dajn[j - 1];
      f2 = temp1;
      e2 = temp2;
      /* L110: */
    }
  *dai = *x * *x * (t * f1 - f2 + dajp[0]) + (t * e1 - e2 + dajn[0]);
  return 0;
  /* 
   */
L120:
  t = 10. / *c__ - 1.;
  tt = t + t;
  j = n4;
  f1 = a[j - 1];
  e1 = b[j - 1];
  f2 = 0.;
  e2 = 0.;
  i__1 = m4;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      --j;
      temp1 = f1;
      temp2 = e1;
      f1 = tt * f1 - f2 + a[j - 1];
      e1 = tt * e1 - e2 + b[j - 1];
      f2 = temp1;
      e2 = temp2;
      /* L130: */
    }
  temp1 = t * f1 - f2 + a[0];
  temp2 = t * e1 - e2 + b[0];
  rtrx = sqrt (*rx);
  cv = *c__ - fpi12;
  ccv = cos (cv);
  scv = sin (cv);
  *ai = (temp1 * ccv - temp2 * scv) / rtrx;
  j = n4d;
  f1 = da[j - 1];
  e1 = db[j - 1];
  f2 = 0.;
  e2 = 0.;
  i__1 = m4d;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      --j;
      temp1 = f1;
      temp2 = e1;
      f1 = tt * f1 - f2 + da[j - 1];
      e1 = tt * e1 - e2 + db[j - 1];
      f2 = temp1;
      e2 = temp2;
      /* L140: */
    }
  temp1 = t * f1 - f2 + da[0];
  temp2 = t * e1 - e2 + db[0];
  e1 = ccv * con5 + scv * .5;
  e2 = scv * con5 - ccv * .5;
  *dai = (temp1 * e1 - temp2 * e2) * rtrx;
  return 0;
}				/* djairy_ */
