
/************************************************************************
*        Gsatcom LTD., All Rights Reserved
* -----------------------------------------------------------------------
* FILE NAME     : aim.c
* AUTHORS       : Raviv Kiron
* CREATE TIME   : 19/06/2009 09:54:06
************************************************************************/

#include "aim.h"
#include <math.h>
#include "rc.h"
//#include "sgp4.h"


RC aim_init(MAT *dcm_fix,MAT *dcm,VEC *ant_angles,VEC *Vned,VEC *Vant)
{
	RC rc;
	double pol;
	rc=OK;
	//char longstr1[70]="1 28132U 03059A   15249.31807869 -.00000065  00000-0  00000+0 0  9997";
	//char longstr2[70]="2 28132   0.0639 239.9787 0002197 299.9965 275.6391  1.00274395 42919";
	int kkk;
	//elsetrec* satrec;
static volatile VEC Vned1,lla1,sat1,xy_sat,xyz1,lla2,Vned2;



lla1.A[1]=35;
lla1.A[2]=0;

sat1.A[0]=0;
sat1.A[1]=-4;
sat1.A[2]=36000000;

	rc=update_lla(Vned,-4,32,35);
//rc=lla_to_ecef(&xyz1,&lla1);
//rc=lla_to_ecef(&xy_sat,&sat1);
//rc=lla_to_ned(&Vned2 ,&xy_sat, &lla1);



//twoline2rv(&longstr1,&longstr2,&satrec);

	rc=update_dcm(dcm,0, 0, 0);
	rc=update_dcm(dcm_fix,0, 0, 0);
	ant_angles->A[0]=0;
	ant_angles->A[1]=0;
	ant_angles->A[2]=0;
	return rc;
}

RC lla_to_ecef(VEC *xyz, VEC *lla)
{
	FLOAT32 N,lat,lon,alt,cln,clt,sln,slt;

	lat=DEG2RAD*(lla->A[0]);
	lon=DEG2RAD*(lla->A[1]);
	alt=lla->A[2];
	
	slt=sin(lat);
	clt=cos(lat);
	sln=sin(lon);
	cln=cos(lon);
	N = (1 - WGS84_e*WGS84_e * slt*slt);
	N=sqrt(N);
	N=WGS84_a/N;

	xyz->A[0] = (N+alt) * clt * cln;
	xyz->A[1] = (N+alt) * clt * sln;
	xyz->A[2] = ((1-WGS84_e*WGS84_e) * N + alt) * slt;

	return OK;
}
RC ecef_to_lla(VEC* xyz, VEC* lla)
{
	double x,y,z,lat,lon,alt,slt,b,ep,p,th,N,sth,cth;
	x=xyz->A[0];
	y=xyz->A[1];
	z=xyz->A[2];

	b   = sqrt(WGS84_a*WGS84_a*(1-WGS84_e*WGS84_e));
	ep  = sqrt((WGS84_a*WGS84_a-b*b)/(b*b));
	p   = sqrt(x*x+y*y);
	th  = atan2(WGS84_a*z,b*p);
	sth=sin(th);
	cth=cos(th);
	lla->A[1]= lon = atan2(y,x);
	lla->A[0]= lat = atan2((z+ep*ep*b*sin(th)*sin(th)*sin(th)),(p-WGS84_e*WGS84_e*WGS84_a*cos(th)*cos(th)*cos(th)));
	slt=sin(lat);
	N   = WGS84_a/sqrt(1-WGS84_e*WGS84_e*slt*slt);
	lla->A[2]= alt = p/cos(lat)-N;

	//return lon in range [0,2*pi)
	//lon = mod(lon,2*pi);


	return OK;
}

RC lla_to_ned(VEC* Vned ,VEC* sat_xyz, VEC* station_lla)

{
	int i;
	VEC v,station_xyz;
	 MAT dcm;
	FLOAT32 lat,longt,alt;
	lat=station_lla->A[0];
	longt=station_lla->A[1];
	
	lla_to_ecef(&station_xyz, station_lla);
	for (i=0;i<3;i++)
		v.A[i]=sat_xyz->A[i]-station_xyz.A[i];
	update_dcm(&dcm,0,-90.0-lat,longt);
	*Vned=mat_vec(dcm,v);
		
	

	return OK;

}


VEC sph2cart(double THETA, double PHI,double R)
{
	VEC vec;
	double c_phi,s_phi,c_th,s_th;
	
	c_phi=cosf(PHI);
	s_phi=sinf(PHI);
	c_th=cosf(THETA);
	s_th=sinf(THETA);
	vec.A[0]= R*c_phi*c_th;
	vec.A[1]=R*c_phi*s_th;
	vec.A[2]=R*s_phi;
	return vec;
}

VEC cart2sph(double X,double Y,double Z)
{
	VEC vec;
	vec.A[0]=atan2(Y,X)*RAD2DEG;
	vec.A[1]=atan2(Z,sqrt(X*X+Y*Y))*RAD2DEG;
	//vec.A[2]=sqrt(X*X+Y*Y+Z*Z);
	return vec;
}

VEC sat_aim(double sat, double lat,double longt, double roll, double pitch, double yaw, double roll_fix, double pitch_fix, double yaw_fix)
{
	
	double el;
	double az;
	double pol;
	


	VEC euler,align,align_att,pol_vect,pol_euler,pol_ned,pol_vect_ant;
	MAT B,B2,B3,C,C1,C2,C3,D,D1,D2,D3,E,E2,E3,DC;
	double ca,sa,ce,se,cy,sy,cr,sr,cp,sp,cpf,spf,crf,srf;
	double c_lg_st,c_lt,c_lg_st_c_lt;
	roll=roll/180*pi;
	pitch=pitch/180*pi;
	yaw=yaw/180*pi;
	
	roll_fix=roll_fix/180*pi;
	pitch_fix=pitch_fix/180*pi;
	yaw_fix=yaw_fix/180*pi;
	
	longt=longt/180*pi;
	lat=lat/180*pi;
	sat=sat/180*pi;


	/*el=atanf( (cosf(-sat+longt)*cosf(lat)-0.1512)/sqrt(1-(cosf(-sat+longt))*(cosf(-sat+longt))*(cosf(lat)*cosf(lat))));
	az= pi+atanf(tanf(-sat+longt)/sinf(lat));
	pol=-atanf(sinf(sat-longt)/tanf(lat));*/
	c_lg_st=cosf(-sat+longt);
	c_lt=cosf(lat);
	c_lg_st_c_lt=c_lg_st*c_lt;
	el=atanf( (c_lg_st_c_lt-0.1512)/sqrt(1-c_lg_st_c_lt*c_lg_st_c_lt));
	az= pi+atanf(tanf(-sat+longt)/sinf(lat));
	pol=-atanf(sinf(sat-longt)/tanf(lat));
	align=sph2cart(az,-el,1);
	ca=cosf(az);
	sa=sinf(az);
	ce=cosf(el);
	se=sinf(el);
	B2.A[0][0]=ce;B2.A[0][1]=0;B2.A[0][2]=-se;
	B2.A[1][0]=0;B2.A[1][1]=1;B2.A[1][2]=0;
	B2.A[2][0]=se;B2.A[2][1]=0;B2.A[2][2]=ce;

	B3.A[0][0]=ca;B3.A[0][1]=sa;B3.A[0][2]=0;
	B3.A[1][0]=-sa;B3.A[1][1]=ca;B3.A[1][2]=0;
	B3.A[2][0]=0;B3.A[2][1]=0;B3.A[2][2]=1;

	B=mat_mat(B2,B3);
	

	cr=cosf(roll);
	sr=sinf(roll);
	cy=cosf(yaw);
	sy=sinf(yaw);
	cp=cosf(pitch);
	sp=sinf(pitch);
	C1.A[0][0]=1;C1.A[0][1]=0;C1.A[0][2]=0;
	C1.A[1][0]=0;C1.A[1][1]=cr;C1.A[1][2]=sr;
	C1.A[2][0]=0;C1.A[2][1]=-sr;C1.A[2][2]=cr;

	C2.A[0][0]=cp;C2.A[0][1]=0;C2.A[0][2]=-sp;
	C2.A[1][0]=0;C2.A[1][1]=1;C2.A[1][2]=0;
	C2.A[2][0]=sp;C2.A[2][1]=0;C2.A[2][2]=cp;

	C3.A[0][0]=cy;C3.A[0][1]=sy;C3.A[0][2]=0;
	C3.A[1][0]=-sy;C3.A[1][1]=cy;C3.A[1][2]=0;
	C3.A[2][0]=0;C3.A[2][1]=0;C3.A[2][2]=1;
	//C=mat_mat(C1,C2);
	C=mat_mat(C1,mat_mat(C2,C3));


	crf=cosf(roll_fix);
	srf=sinf(roll_fix);
	D1.A[0][0]=1;D1.A[0][1]=0;D1.A[0][2]=0;
	D1.A[1][0]=0;D1.A[1][1]=crf;D1.A[1][2]=srf;
	D1.A[2][0]=0;D1.A[2][1]=-srf;D1.A[2][2]=crf;

	cpf=cosf(pitch_fix);
	spf=sinf(pitch_fix);
	D2.A[0][0]=cpf;D2.A[0][1]=0;D2.A[0][2]=-spf;
	D2.A[1][0]=0;D2.A[1][1]=1;D2.A[1][2]=0;
	D2.A[2][0]=spf;D2.A[2][1]=0;D2.A[2][2]=cpf;

	D3.A[0][0]=cosf(yaw_fix);D3.A[0][1]=sinf(yaw_fix);D3.A[0][2]=0;
	D3.A[1][0]=-sinf(yaw_fix);D3.A[1][1]=cosf(yaw_fix);D3.A[1][2]=0;
	D3.A[2][0]=0;D3.A[2][1]=0;D3.A[2][2]=1;
	//D=mat_mat(D1,D2);
	D=mat_mat(D1,mat_mat(D2,D3));
	DC=mat_mat(D,C);
	align_att=mat_vec(DC,align);
	euler=cart2sph(align_att.A[0],align_att.A[1],align_att.A[2]);
	az=euler.A[0];
	el=-euler.A[1];
	
	ca=cosf(az);
	sa=sinf(az);
	ce=cosf(el);
	se=sinf(el);

	E2.A[0][0]=ce;E2.A[0][1]=0;E2.A[0][2]=-se;
	E2.A[1][0]=0;E2.A[1][1]=1;E2.A[1][2]=0;
	E2.A[2][0]=se;E2.A[2][1]=0;E2.A[2][2]=ce;

	E3.A[0][0]=ca;E3.A[0][1]=sa;E3.A[0][2]=0;
	E3.A[1][0]=-sa;E3.A[1][1]=ca;E3.A[1][2]=0;
	E3.A[2][0]=0;E3.A[2][1]=0;E3.A[2][2]=1;
	E=mat_mat(E2,E3);

	pol_vect.A[0]=0;
	pol_vect.A[1]=-cosf(pol);
	pol_vect.A[2]=sinf(pol);
	
	pol_ned=mat_vec(mat_t(B),pol_vect);
	//pol_ned_euler=cart2sph(pol_ned.A[0],pol_ned.A[1],pol_ned.A[2]);
	pol_vect_ant=mat_vec(mat_mat(E,DC),pol_ned);
	pol_euler=cart2sph(pol_vect_ant.A[0],pol_vect_ant.A[1],pol_vect_ant.A[2]);
	euler.A[0]=euler.A[0]/pi*180;//az
	euler.A[1]=-euler.A[1]/pi*180;//el
	euler.A[2]=pol_euler.A[1]/pi*180;//pol

	return euler;
}
FLOAT32 update_lla(VEC *Vned,double sat, double lat,double longt)
{
	
	double el;
	double az;
	double pol;
	


	VEC euler,align,align_att,pol_vect,pol_euler,pol_ned,pol_vect_ant;

	double c_lg_st,c_lt,c_lg_st_c_lt,long2sat;

	
	longt=longt/180*pi;
	lat=lat/180*pi;
	sat=sat/180*pi;


	/*el=atanf( (cosf(-sat+longt)*cosf(lat)-0.1512)/sqrt(1-(cosf(-sat+longt))*(cosf(-sat+longt))*(cosf(lat)*cosf(lat))));
	az= pi+atanf(tanf(-sat+longt)/sinf(lat));
	pol=-atanf(sinf(sat-longt)/tanf(lat));*/
	c_lg_st=cosf(-sat+longt);
	c_lt=cosf(lat);
	c_lg_st_c_lt=c_lg_st*c_lt;
	el=atanf( (c_lg_st_c_lt-0.1512)/sqrt(1-c_lg_st_c_lt*c_lg_st_c_lt));
	long2sat=longt-sat;	
	if (long2sat>pi)
	    long2sat=long2sat-2*pi;
	else if (long2sat<-pi)
	    long2sat=long2sat+2*pi;
	
	
	az= pi+atan2(tanf(-sat+longt),sinf(lat));
	if (fabs(long2sat)>pi/2)
		az-=pi;
	pol=-atanf(sinf(sat-longt)/tanf(lat));
	align=sph2cart(az,-el,1);
	
	Vned->A[0]=align.A[0];
	Vned->A[1]=align.A[1];
	Vned->A[2]=align.A[2];
	return pol;
}

RC update_dcm(MAT *dcm,double roll, double pitch, double yaw)
{
	
	RC rc;
	MAT C,C0,C1,C2,C3;
	double cy,sy,cr,sr,cp,sp;

	rc=OK;
	//C=dcm;
	roll=roll/180*pi;
	pitch=pitch/180*pi;
	yaw=yaw/180*pi;

	

	cr=cosf(roll);
	sr=sinf(roll);
	cy=cosf(yaw);
	sy=sinf(yaw);
	cp=cosf(pitch);
	sp=sinf(pitch);
	C1.A[0][0]=1;C1.A[0][1]=0;C1.A[0][2]=0;
	C1.A[1][0]=0;C1.A[1][1]=cr;C1.A[1][2]=sr;
	C1.A[2][0]=0;C1.A[2][1]=-sr;C1.A[2][2]=cr;

	C2.A[0][0]=cp;C2.A[0][1]=0;C2.A[0][2]=-sp;
	C2.A[1][0]=0;C2.A[1][1]=1;C2.A[1][2]=0;
	C2.A[2][0]=sp;C2.A[2][1]=0;C2.A[2][2]=cp;

	C3.A[0][0]=cy;C3.A[0][1]=sy;C3.A[0][2]=0;
	C3.A[1][0]=-sy;C3.A[1][1]=cy;C3.A[1][2]=0;
	C3.A[2][0]=0;C3.A[2][1]=0;C3.A[2][2]=1;
	//C=mat_mat(C1,C2);
	C=mat_mat(C2,C3);
	*dcm=mat_mat(C1,C);
	
	


	//D=mat_mat(D1,D2);


	return rc;
}

RC update_angles(MAT *dcm_fix,MAT *dcm,VEC *ant_angles,VEC *Vned,VEC *Vant)
{
	//MAT DC;
	RC rc;
	//DC=mat_mat(*dcm_fix,*dcm);
	*Vant=mat_vec(*dcm,*Vned);
	*Vant=mat_vec(*dcm_fix,*Vant);
	*ant_angles=cart2sph(Vant->A[0],Vant->A[1],Vant->A[2]);
	return rc;


}

RC predict_angles(MAT *dcm,VEC *ant_angles,VEC *Vned,VEC *Vant,VEC* rates)
{
	MAT skew;
	RC rc;
	skew=mat_skew(rates->A[0], rates->A[1],rates->A[2]);
	*dcm=mat_mat(skew,*dcm);
	*Vant=mat_vec(*dcm,*Vned);
	*ant_angles=cart2sph(Vant->A[0],Vant->A[1],Vant->A[2]);
	return rc;


}
