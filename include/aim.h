
/************************************************************************
*        Gsatcom LTD., All Rights Reserved
* -----------------------------------------------------------------------
* FILE NAME     :aim.h
* AUTHORS       : Raviv Kiron
* CREATE TIME   : 19/06/2009 09:54:06
************************************************************************/
#ifndef __AIM__H__
#define __AIM__H__

/***** Include files ***************************************************/
#include "matrix.h"
#include "rc.h"
#include "defs_p.h"
#include "SGP4.h"
#include <math.h>


/***** Defines  ********************************************************/
#define pi 3.141592653589793
#define DEG2RAD pi/180.0
#define RAD2DEG 180.0/pi

#define WGS84_a  6378137.0
#define WGS84_e  8.1819190842622e-2
/***** Typedefs ********************************************************/


/***** Private Return Code *********************************************/

/***** Functions Declarations ******************************************/

RC aim_init(MAT *dcm_fix,MAT *dcm,VEC *ant_angles,VEC *Vned,VEC *Vant);
VEC sat_aim(double sat, double lat,double longt, double roll, double pitch, double yaw, double roll_fix, double pitch_fix, double yaw_fix);
RC update_dcm(MAT *dcm,double roll, double pitch, double yaw);
RC update_angles(MAT *dcm_fix,MAT *dcm,VEC *ant_angles,VEC *Vned,VEC *Vant);
FLOAT32 update_lla(VEC *Vned,double sat, double lat,double longt);
RC predict_angles(MAT *dcm,VEC *ant_angles,VEC *Vned,VEC *Vant,VEC* rates);
RC lla_to_ecef(VEC* xyz, VEC* lla);
RC ecef_to_lla(VEC* xyz, VEC *lla);
RC lla_to_ned(VEC* Vned ,VEC* sat_xyz, VEC* station_lla);
void Get_SatLatLong(VEC* lla_sgp,char* satname);





#endif
