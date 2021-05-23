#include "matrix.h"
#include <math.h>


/*needs to convert to pointers in all functions*/
/***********************************************/

MAT mat_scalar(MAT mat_a, double x)
{
	int i,j;
	MAT mat_b;
	


     for(i=0;i<3;i++)
     {
          for(j=0;j<3;j++)
          {     
              mat_b.A[i][j]=mat_a.A[i][j]*x;
               
          }
     }

	return mat_b;

}

VEC vec_scalar(VEC vec, FLOAT32 x)
{
	int i;

     for(i=0;i<3;i++)
     {
        vec.A[i]=vec.A[i]*x;
     }

	return vec;
}

VEC vec_add_scalar(VEC vec, FLOAT32 x,FLOAT32 modulu)
{
	int i;

	 if (x<0)
		for(i=0;i<3;i++)
		{
			vec.A[i]=-x-vec.A[i];
			if (vec.A[i]<0)
				vec.A[i]+=modulu;
				
		}
	else
		for(i=0;i<3;i++)
		{
			vec.A[i]=x+vec.A[i];
		}	
	return vec;
}


MAT mat_t(MAT mat_a)
{
int i,j;
MAT mat_x;
for(j=0;j<3;j++)
{
	for(i=0;i<3;i++)
	{
		if (i!=j)
		{
			mat_x.A[i][j]=mat_a.A[j][i];
		}

	}
}

return mat_x;
}

MAT mat_inv(MAT mat_a)
{
int i,j;
MAT mat_b,mat_c,mat_x;
double x,n=0;

          for(i=0,j=0;j<3;j++)
          {     
               if(j==2) 
               n+=mat_a.A[i][j]*mat_a.A[i+1][0]*mat_a.A[i+2][1];
               else if(j==1)
               n+=mat_a.A[i][j]*mat_a.A[i+1][j+1]*mat_a.A[i+2][0];
               else 
               n+=mat_a.A[i][j]*mat_a.A[i+1][j+1]*mat_a.A[i+2][j+2];
          }
          for(i=2,j=0;j<3;j++)
          {     
               if(j==2) 
               n-=mat_a.A[i][j]*mat_a.A[i-1][0]*mat_a.A[i-2][1];
               else if(j==1)
               n-=mat_a.A[i][j]*mat_a.A[i-1][j+1]*mat_a.A[i-2][0];
               else
               n-=mat_a.A[i][j]*mat_a.A[i-1][j+1]*mat_a.A[i-2][j+2];
          }

	if (n!=0)
		x=1/n;
	else
		x=0.0000001;
	

    for(i=0;i<3;i++)
     {
          
          for(j=0;j<3;j++)
          {     
                    
               mat_b.A[i][j]=mat_a.A[j][i];
               
               
          }
     }


     mat_c.A[0][0]=mat_b.A[1][1]*mat_b.A[2][2]-(mat_b.A[2][1]*mat_b.A[1][2]);
     mat_c.A[0][1]=(-1)*(mat_b.A[1][0]*mat_b.A[2][2]-(mat_b.A[2][0]*mat_b.A[1][2]));
     mat_c.A[0][2]=mat_b.A[1][0]*mat_b.A[2][1]-(mat_b.A[2][0]*mat_b.A[1][1]);
     
     mat_c.A[1][0]=(-1)*(mat_b.A[0][1]*mat_b.A[2][2]-mat_b.A[2][1]*mat_b.A[0][2]);
     mat_c.A[1][1]=mat_b.A[0][0]*mat_b.A[2][2]-mat_b.A[2][0]*mat_b.A[0][2];
     mat_c.A[1][2]=(-1)*(mat_b.A[0][0]*mat_b.A[2][1]-mat_b.A[2][0]*mat_b.A[0][1]);
 
     mat_c.A[2][0]=mat_b.A[0][1]*mat_b.A[1][2]-mat_b.A[1][1]*mat_b.A[0][2];
     mat_c.A[2][1]=(-1)*(mat_b.A[0][0]*mat_b.A[1][2]-mat_b.A[1][0]*mat_b.A[0][2]);
     mat_c.A[2][2]=mat_b.A[0][0]*mat_b.A[1][1]-mat_b.A[1][0]*mat_b.A[0][1];


     for(i=0;i<3;i++)
     {
          for(j=0;j<3;j++)
          {     
              mat_x.A[i][j]=mat_c.A[i][j]*x;
               
          }
     }

	return mat_x;
}



MAT mat_mat(MAT mat_a,MAT mat_b)
{
int i=i,j,k;
MAT mat_c;
double c=0;

for(k=0;k<3;k++)
{
	for(j=0;j<3;j++)
	{
		c=0;
		for(i=0;i<3;i++)
		{
			c=c+mat_a.A[j][i]*mat_b.A[i][k];
		}
	mat_c.A[j][k]=c;
	
	}		
}
	return mat_c;
}
	

VEC mat_vec(MAT mat_a,VEC vec_a)

{
int i=i,j;
VEC vec_b;
double b=0;


	for(j=0;j<3;j++)
	{
		b=0;
		for(i=0;i<3;i++)
		{
			b=b+mat_a.A[j][i]*vec_a.A[i];
		}
		vec_b.A[j]=b;	
	}		

	return vec_b;
}

MAT mat_skew(double x, double y,double z)
{

	MAT m;
    m.A[0][0]=1;
    m.A[0][1]=z;
    m.A[0][2]=-y;
    
    m.A[1][0]=-z;
    m.A[1][1]=1;
    m.A[1][2]=x;

    m.A[2][0]=y;
    m.A[2][1]=-x;
    m.A[2][2]=1;
	return m;
}

MAT mat_I()
{

	MAT m;
    m.A[0][0]=1;
    m.A[0][1]=0;
    m.A[0][2]=0;
    
    m.A[1][0]=0;
    m.A[1][1]=1;
    m.A[1][2]=0;

    m.A[2][0]=0;
    m.A[2][1]=0;
    m.A[2][2]=1;
	return m;
	
}

MAT mat_add_mat(MAT mat_a,MAT mat_b)
{
int i=i,j;
MAT mat_c;
double c=0;


	for(j=0;j<3;j++)
	{
		c=0;
		for(i=0;i<3;i++)
		{
			mat_c.A[i][j]=mat_a.A[i][j]+mat_b.A[i][j];
		}

	}		

	return mat_c;
}


MAT mat_inv_GJ(MAT mat_a,int n)
{

double a[10][10];
int k,i,j;
MAT c;
double ratio;
		 for(i=0;i<n;i++)
		 {
			  for(j=0;j<n;j++)
			  {
				  
				   a[i][j]=mat_a.A[i][j];
			  }
		 }
		 /* Augmenting Identity Matrix of Order n */
		 for(i=0;i<n;i++)
		 {
			  for(j=0;j<n;j++)
			  {
				   if(i==j)
				   {
				    	a[i][j+n] = 1;
				   }
				   else
				   {
				    	a[i][j+n] = 0;
				   }
			  }
		 }   
		 /* Applying Gauss Jordan Elimination */
		 for(i=0;i<=n;i++)
		 {
			  if(a[i][i] == 0.0)
			  {
//error
			  }
			  for(j=0;j<n;j++)
			  {
				   if(i!=j)
				   {
					    ratio = a[j][i]/a[i][i];
					    for(k=1;k<=2*n;k++)
					    {
					     	a[j][k] = a[j][k] - ratio*a[i][k];
					    }
				   }
			  }
		 }
		 /* Row Operation to Make Principal Diagonal to 1 */
		 for(i=0;i<n;i++)
		 {
			  for(j=n;j<2*n;j++)
			  {
			   	a[i][j] = a[i][j]/a[i][i];
			  }
		 }
		 /* Displaying Inverse Matrix */

		 for(i=1;i<n;i++)
		 {
			  for(j=n;j<2*n;j++)
			  {
			   	c.A[i][j]=a[i][j];
			  }

		 }
return c;
}

