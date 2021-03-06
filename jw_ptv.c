#include <stdio.h>
#include <zlib.h>

#define nmax 5120

extern	int    	n_img;			       /* no of images */
extern  int		oddevenflag;           	   /* flag for oddeven */
extern  int		hp_flag;           	       /* flag for highpass */
extern	int    	tiff_flag;	               /* flag for tiff header */
extern	int    	chfield;	               /* flag for field mode */
extern	int    	nfix;	       		       /* no. of control points */
extern	int    	num[]; 			       /* no. of particles per image */
extern	int    	nump[4]; 	       	       /* no. of particles in previous image */
extern	int    	numc[4]; 	       	       /* no. of particles in current image */
extern	int    	numn[4]; 	       	       /* no. of particles in next image */
extern	int    	n_trac[];      		       /* no. of tracks */
extern	int    	match; 			       /* no. of matches */
extern	int    	match2;		    	       /* no. of matches in 2nd pass */
extern  int     corp, corc, corn;              /* no. of correspondences in p,c,n */

extern	int    	nr[4][4];		       /* point numbers for man. ori */
extern	int    	imx, imy, imgsize;     	       /* image size */
extern	int    	zoom_x[], zoom_y[], zoom_f[];  /* zoom parameters */
extern	int    	pp1, pp2, pp3, pp4;	       /* for man. orientation */
extern	int    	seq_first, seq_last;	       /* 1. and last img of seq */
extern	int    	demo_nr;		       /* for demo purposes */
extern	int    	examine;       		       /* extra output */
extern	int    	dump_for_rdb;		       /* # of dumpfiles for rdb */
extern  int     cr_sz;                         /* size of crosses */
extern  int     display;                       /* display flag */
extern  int     m[4];
extern  int     trackallocflag;  /* checkflag if mega, c4, t4 already allocated */

extern	double 	pix_x, pix_y;		     	/* pixel size */
extern	double 	pi, ro;				/* pi, ro */
extern	double 	cn, cnx, cny, csumg, eps0, corrmin;	 /* correspond. par */
extern	double 	rmsX, rmsY, rmsZ, mean_sigma0;		 /* a priori rms */
extern	double  X_lay[], Zmin_lay[], Zmax_lay[];         /* illu. layer */

extern	FILE   	*fp1, *fp2, *fp3, *fp4, *fpp;	/* file pointers */

extern	char  img_name[4][256];	        /* original image names */
extern	char  img_lp_name[4][256];    	/* lowpass image names */
extern	char  img_hp_name[4][256];      /* highpass image names */
extern	char  img_cal[4][128];		/* calibration image names */
extern	char  img_ori[4][128];		/* image orientation data */
extern	char  img_ori0[4][128];		/* orientation approx. values */
extern	char  img_addpar0[4][128];	/* approx. image additional parameters */
extern	char  img_addpar[4][128];	/* image additional parameters */
extern	char  fixp_name[128];	/* testfield fixpoints */
extern	char  seq_name[4][128];      	/* sequence names */
extern	char  track_dir[128];	       	/* directory with dap track data */
extern	char  res_name[128];	       	/* result destination */
extern	char  buf[], val[];		       	/* buffer */
extern	char  filename[128];

extern	unsigned char	*img[];			/* image data */
extern	unsigned char	*zoomimg;		/* zomm image data */

extern	Exterior        Ex[];	       	/* exterior orientation */
extern	Interior       	I[];	        /* interior orientation */
extern	ap_52	       	ap[];	       	/* add. parameters */
extern	mm_np	       	mmp;	       	/* 3-media parameters */
extern	target	       	pix[4][nmax];  	/* target pixel data */
extern	target	       	pix0[4][4];    	/* pixel data for man_ori points */
extern	coord_2d       	crd[4][nmax];  	/* (distorted) metric coordinates */
extern	coord_2d       	geo[4][nmax];  	/* corrected metric coordinates */
extern	coord_3d       	fix[];	        /* testfield points coordinates */
extern	n_tupel	       	con[];	      	/* list of correspondences */
extern	mm_LUT	       	mmLUT[4];     	/* LUT for mm radial displacement */
extern  coord_3d        *p_c3d;
extern  target          *p[4];
extern  target          *c[4];
extern  target          *n[4];
extern  target          *t4[4][4];
extern  int             nt4[4][4];
extern  corres          *corrp;
extern  corres	       	*corrc;
extern  corres	       	*corrn;
extern  corres	       	*c4[4];
extern  trackparameters tpar;

extern P *mega[4];

extern	FILE	*fopen_r ();
extern	double	multimed_r ();

/* ChrisB added: */
extern char szDynTParFilename[128];		/* Filename for dynamic tracking parameters*/
extern int	useCompression;				/* flag for using gz compression on output files */
extern int staticTracking;				/* Static/dynamic tracking flag. */
extern int verbose;						/* Verbosity flag */
extern gzFile fgz;						/* Used for opening compressed files */


int  jw_Init();
int  init_proc_c();
int  start_proc_c();
int  pre_processing_c ();
int  detection_proc_c();
int  correspondences_proc_c ();
int  determination_proc_c ();
int  sequence_proc_c();
void checkpoint_proc ();
int  quit_proc_c ();
int  parameter_panel_init();
int  done_proc_c();

void read_ori();
void write_ori ();
void compose_name_plus_nr ();
int  mouse_proc_c ();
void read_image();
int  read_tiff ();
int  write_tiff ();
void copy_images();
void histeq ();

int  peak_fit_new();
void highpass();
void zoom ();
void lowpass_n ();
void lowpass_3 ();
void filter_3 ();
void unsharp_mask ();
void split ();
void multimed_nlay ();

void quicksort_con ();
int  flow_demo_c();
void simple_connectivity();
void targ_rec();
void quicksort_target_y();
void transp();
void quicksort_coord2d_x();
void rotation_matrix();
void img_xy ();
void img_xy_mm ();
void img_xy_mm_geo ();
void pixel_to_metric();
void metric_to_pixel ();
void correct_brown_affin();
void distort_brown_affin ();
int  pix_in_next();
int  kill_in_list ();
void compose_name_plus_nr ();
void compose_name_plus_nr_str ();
int  nearest_neighbour_geo ();
int  nearest_neighbour_pix ();
void bubble_y ();
void init_mmLUT ();


void correspondences_4();
void det_lsq();
void ray_tracing();
void intersect_rt_3m();
void intersect_rt();
int  nearest_neighbour_pix();
void img_coord();
void raw_orient();
void sortgrid_man();
void det_lsq ();
void det_lsq_2 ();
void det_lsq_3 ();
void det_lsq_4 ();
void raw_orient ();
void orient();
void volumedimension();

double epi_line ();
int  epi_mm ();
void find_candidate_plus_msg ();
void find_candidate_plus ();

int  drawcross();
int  drawvector ();
int  draw_pnr ();
int  draw_value ();
void mark_detections ();
void mark_correspondences ();
void mark_corr ();

void ata ();
void matinv ();
void atl ();
void matmul ();
void mat_transpose ();

void tracking_proc ();
int  find_track ();

int  candsearch_in_pix ();
int  candsearch_in_pixrest ();
int  corrtest();
void predict();
void readseqtrackcrit();
void searchquader();
void angle_acc();
void sortwhatfound();
void bubble_foundpix1 ();
void bubble_foundpix2 ();
void tclimg2cimg();

void Pix_crd ();
void Back_trafo ();
void crd_pix();
int  vrmltracks_c();
int  trajectories_c();
int  vrmldetections_c();
int  vrmldettracks_c();


int  tracking();
void read_parameter();
void level1();
void level2();
void level3();
void read_ascii_data();
void rotate_dataset();
void write_ascii_data();
void readcoord_parameter();
void change_coordinates();
void sort();
void neighbours();
int  seq_track_proc_c();

void read_ascii_datanew();
void write_ascii_datanew();

void oddeven();



                                                                                                                                                                                                                                                                                                                                                                                            common/imgcoord.c                                                                                   0000644 0001131 0001131 00000005353 10253026712 012761  0                                                                                                    ustar   amns                            amns                                                                                                                                                                                                                   /****************************************************************************

Routine:	       	imgcoord.c

Author/Copyright:      	Hans-Gerd Maas

Address:	       	Institute of Geodesy and Photogrammetry
                        ETH - Hoenggerberg
		       	CH - 8093 Zurich

Creation Date:	       	22.4.88

Description:	       	computes x', y' from given Point and orientation
	       		(see: Kraus)

Routines contained:

****************************************************************************/

#include "ptv.h"

void img_coord (X,Y,Z, Ex, I, ap, mm, x,y)

double 	 X,Y,Z;
Exterior Ex;
Interior I;
ap_52  	 ap;
mm_np    mm;
double 	 *x,*y;

{
  double deno, r, dx, dy;

  multimed_nlay (Ex, mm, X,Y,Z, &X,&Y);

  X -= Ex.x0;  Y -= Ex.y0;  Z -= Ex.z0;

  deno = Ex.dm[0][2] * X + Ex.dm[1][2] * Y + Ex.dm[2][2] * Z;
  *x = I.xh - I.cc * (Ex.dm[0][0]*X + Ex.dm[1][0]*Y + Ex.dm[2][0]*Z) / deno;
  *y = I.yh - I.cc * (Ex.dm[0][1]*X + Ex.dm[1][1]*Y + Ex.dm[2][1]*Z) / deno;

  r = sqrt (*x * *x + *y * *y);

  dx = (*x) * (ap.k1*r*r + ap.k2*r*r*r*r + ap.k3*r*r*r*r*r*r)
    + ap.p1 * (r*r + 2*(*x)*(*x)) + 2*ap.p2*(*x)*(*y);
  dy = (*y) * (ap.k1*r*r + ap.k2*r*r*r*r + ap.k3*r*r*r*r*r*r)
    + ap.p2 * (r*r + 2*(*y)*(*y)) + 2*ap.p1*(*x)*(*y);

  *x += dx;
  *y += dy;

  *x = ap.scx * (*x) - sin(ap.she) * (*y);
  *y = cos(ap.she) * (*y);

}






void img_xy (X,Y,Z, Ex, I, x,y)

double 	X,Y,Z, *x,*y;
Exterior Ex;
Interior I;

{
  double deno;

  deno = Ex.dm[0][2] * (X-Ex.x0)
    + Ex.dm[1][2] * (Y-Ex.y0)
    + Ex.dm[2][2] * (Z-Ex.z0);

  *x = I.xh - I.cc *  (Ex.dm[0][0] * (X-Ex.x0)
		       + Ex.dm[1][0] * (Y-Ex.y0)
		       + Ex.dm[2][0] * (Z-Ex.z0)) / deno;

  *y = I.yh - I.cc *  (Ex.dm[0][1] * (X-Ex.x0)
		       + Ex.dm[1][1] * (Y-Ex.y0)
		       + Ex.dm[2][1] * (Z-Ex.z0)) / deno;
}




void img_xy_mm (X,Y,Z, Ex, I, mm, x,y)

double   X,Y,Z, *x,*y;
Exterior Ex;
Interior I;
mm_np    mm;

{
  double deno;

  multimed_nlay (Ex,mm,X,Y,Z,&X,&Y);

  deno = Ex.dm[0][2] * (X-Ex.x0)
    + Ex.dm[1][2] * (Y-Ex.y0)
    + Ex.dm[2][2] * (Z-Ex.z0);

  *x = I.xh - I.cc *  (Ex.dm[0][0] * (X-Ex.x0)
		       + Ex.dm[1][0] * (Y-Ex.y0)
		       + Ex.dm[2][0] * (Z-Ex.z0)) / deno;

  *y = I.yh - I.cc *  (Ex.dm[0][1] * (X-Ex.x0)
		       + Ex.dm[1][1] * (Y-Ex.y0)
		       + Ex.dm[2][1] * (Z-Ex.z0)) / deno;
}




void img_xy_mm_geo (X,Y,Z, Ex, I, mm, x,y)

double   X,Y,Z, *x,*y;
Exterior Ex;
Interior I;
mm_np    mm;

{
  double deno;

  multimed_nlay (Ex,mm,X,Y,Z,&X,&Y);

  deno = Ex.dm[0][2] * (X-Ex.x0)
    + Ex.dm[1][2] * (Y-Ex.y0)
    + Ex.dm[2][2] * (Z-Ex.z0);

  *x = - I.cc *  (Ex.dm[0][0] * (X-Ex.x0)
		  + Ex.dm[1][0] * (Y-Ex.y0)
		  + Ex.dm[2][0] * (Z-Ex.z0)) / deno;

  *y = - I.cc *  (Ex.dm[0][1] * (X-Ex.x0)
		  + Ex.dm[1][1] * (Y-Ex.y0)
		  + Ex.dm[2][1] * (Z-Ex.z0)) / deno;
}
                                                                                                                                                                                                                                                                                     common/intersect.c                                                                                  0000644 0001131 0001131 00000013771 10253026712 013161  0                                                                                                    ustar   amns                            amns                                                                                                                                                                                                                   /****************************************************************************

Routine:				intersect.c

Author/Copyright:		Hans-Gerd Maas

Address:				Institute of Geodesy and Photogrammetry
						ETH - Hoenggerberg
						CH - 8093 Zurich

Creation Date:			6.4.88
	
Description:			2 * (point + direction cosines) ==> intersection
	
Routines contained:		-

****************************************************************************/
#include "ptv.h"



/*-------------------------------------------------------------------------*/
/* 2 cameras */




void intersect_rt (X1,Y1,Z1,a1,b1,c1,X2,Y2,Z2,a2,b2,c2,X,Y,Z)

double  X1, Y1, Z1, a1, b1, c1, X2, Y2, Z2, a2, b2, c2, *X, *Y, *Z;

/* intersection, given two points with direction cosines */
/* only valid, if Z1 = Z2 = 0 , which is the case after ray tracing */

{
	if (fabs(b1-b2) > fabs(a1-a2))	*Z = (Y2-Y1) / ((b1/c1) - (b2/c2));
	else							*Z = (X2-X1) / ((a1/c1) - (a2/c2));
	
	*X = ( X1 + X2  +  *Z * (a1/c1 + a2/c2)) /2;
	*Y = ( Y1 + Y2  +  *Z * (b1/c1 + b2/c2)) /2;
}





void intersect (X1,Y1,Z1,a1,b1,c1,X2,Y2,Z2,a2,b2,c2,X,Y,Z)

double  X1, Y1, Z1, a1, b1, c1, X2, Y2, Z2, a2, b2, c2, *X, *Y, *Z;

/* intersection, given two points with direction cosines */

{
	if (fabs(b1-b2) > fabs(a1-a2))
		*Z = (Y2 - Y1 + Z1*(b1/c1) - Z2*(b2/c2)) / ((b1/c1) - (b2/c2));
	else
		*Z = (X2 - X1 + Z1*(a1/c1) - Z2*(a2/c2)) / ((a1/c1) - (a2/c2));
	
	*X = ( X1 + X2  +  *Z * (a1/c1 + a2/c2)) / 2;
	*Y = ( Y1 + Y2  +  *Z * (b1/c1 + b2/c2)) / 2;
}





/*-------------------------------------------------------------------------*/
/* 3 cameras */





void
intersect_rt_3 (X1,Y1,Z1,a1,b1,c1,X2,Y2,Z2,a2,b2,c2,X3,Y3,Z3,a3,b3,c3, X,Y,Z)

double	X1,Y1,Z1,a1,b1,c1, X2,Y2,Z2,a2,b2,c2, X3,Y3,Z3,a3,b3,c3, *X,*Y,*Z;

/* intersection, given three points with direction cosines */

{
	int		i, n_max;
	double	base[6], max_base=0;

	/* find the maximum base component to decide, wether to take 
	   X or Y for determination of Z */
	   /* should be taken out as constant */
	base[0] = fabs (X1 + X2 - 2*X3);
	base[1] = fabs (X1 + X3 - 2*X2);
	base[2] = fabs (X2 + X3 - 2*X1);
	base[3] = fabs (Y1 + Y2 - 2*Y3);
	base[4] = fabs (Y1 + Y3 - 2*Y2);
	base[5] = fabs (Y2 + Y3 - 2*Y1);

	for (i=0; i<6; i++)	if (base[i] > max_base)
	{
		max_base = base[i];
		n_max = i;
	}

	switch (n_max)
	{
		case 0:	*Z = (2*X3 - X1 - X2) / ((a1/c1) + (a2/c2) - (2*a3/c3));
				break;
		case 1:	*Z = (2*X2 - X1 - X3) / ((a1/c1) + (a3/c3) - (2*a2/c2));
				break;
		case 2:	*Z = (2*X1 - X2 - X3) / ((a2/c2) + (a3/c3) - (2*a1/c1));
				break;
		case 3:	*Z = (2*Y3 - Y1 - Y2) / ((b1/c1) + (b2/c2) - (2*b3/c3));
				break;
		case 4:	*Z = (2*Y2 - Y1 - Y3) / ((b1/c1) + (b3/c3) - (2*b2/c2));
				break;
		case 5:	*Z = (2*Y1 - Y2 - Y3) / ((b2/c2) + (b3/c3) - (2*b1/c1));
				break;
	}

	*X = (X1+(*Z)*(a1/c1) + X2+(*Z)*(a2/c2) + X3+(*Z)*(a3/c3)) / 3;
	*Y = (Y1+(*Z)*(b1/c1) + Y2+(*Z)*(b2/c2) + Y3+(*Z)*(b3/c3)) / 3;
}





void
intersect_rt_3m (X1,Y1,Z1,a1,b1,c1,X2,Y2,Z2,a2,b2,c2,X3,Y3,Z3,a3,b3,c3, X,Y,Z)

double	X1,Y1,Z1,a1,b1,c1, X2,Y2,Z2,a2,b2,c2, X3,Y3,Z3,a3,b3,c3, *X,*Y,*Z;

/* intersection, given three points with direction cosines */

{
	int		i, n_max1, n_max2;
	double	base[6], max_base1=0, max_base2=0, Za, Zb;

	/* find the maximum base component to decide, wether to take 
	   X or Y for determination of Z */
	   /* should be taken out as constant */
	base[0] = fabs (X1 + X2 - 2*X3);
	base[1] = fabs (X1 + X3 - 2*X2);
	base[2] = fabs (X2 + X3 - 2*X1);
	base[3] = fabs (Y1 + Y2 - 2*Y3);
	base[4] = fabs (Y1 + Y3 - 2*Y2);
	base[5] = fabs (Y2 + Y3 - 2*Y1);

	for (i=0; i<6; i++)
	{
		if (base[i] > max_base1)
		{
			max_base2 = max_base1;	max_base1 = base[i];
			n_max2 = n_max1;		n_max1 = i;
		}
		else if (base[i] > max_base2)
		{
			max_base2 = base[i];	n_max2 = i;
		}
	}

	/* compute *Z from the 2 longest base components,
	   what was empirically found to be the most accurate value */

	switch (n_max1)
	{
		case 0:	Za = (2*X3 - X1 - X2) / ((a1/c1) + (a2/c2) - (2*a3/c3));
				break;
		case 1:	Za = (2*X2 - X1 - X3) / ((a1/c1) + (a3/c3) - (2*a2/c2));
				break;
		case 2:	Za = (2*X1 - X2 - X3) / ((a2/c2) + (a3/c3) - (2*a1/c1));
				break;
		case 3:	Za = (2*Y3 - Y1 - Y2) / ((b1/c1) + (b2/c2) - (2*b3/c3));
				break;
		case 4:	Za = (2*Y2 - Y1 - Y3) / ((b1/c1) + (b3/c3) - (2*b2/c2));
				break;
		case 5:	Za = (2*Y1 - Y2 - Y3) / ((b2/c2) + (b3/c3) - (2*b1/c1));
				break;
	}
	switch (n_max2)
	{
		case 0:	Zb = (2*X3 - X1 - X2) / ((a1/c1) + (a2/c2) - (2*a3/c3));
				break;
		case 1:	Zb = (2*X2 - X1 - X3) / ((a1/c1) + (a3/c3) - (2*a2/c2));
				break;
		case 2:	Zb = (2*X1 - X2 - X3) / ((a2/c2) + (a3/c3) - (2*a1/c1));
				break;
		case 3:	Zb = (2*Y3 - Y1 - Y2) / ((b1/c1) + (b2/c2) - (2*b3/c3));
				break;
		case 4:	Zb = (2*Y2 - Y1 - Y3) / ((b1/c1) + (b3/c3) - (2*b2/c2));
				break;
		case 5:	Zb = (2*Y1 - Y2 - Y3) / ((b2/c2) + (b3/c3) - (2*b1/c1));
				break;
	}
	*Z = (Za + Zb) / 2;


	*X = (X1+(*Z)*(a1/c1) + X2+(*Z)*(a2/c2) + X3+(*Z)*(a3/c3)) / 3;
	*Y = (Y1+(*Z)*(b1/c1) + Y2+(*Z)*(b2/c2) + Y3+(*Z)*(b3/c3)) / 3;
}





void intersect_3 (X1,Y1,Z1,a1,b1,c1,X2,Y2,Z2,a2,b2,c2,X3,Y3,Z3,a3,b3,c3, X,Y,Z)

double	X1,Y1,Z1,a1,b1,c1, X2,Y2,Z2,a2,b2,c2, X3,Y3,Z3,a3,b3,c3, *X,*Y,*Z;

/* intersection, given three points with direction cosines */

{
	int		i, n_min;
	double	base[6], min_base=1e20;

	/* find the minimum base component to decide, wether to take 
	   X or Y for determination of Z */
	base[0] = fabs (a1-a2);  base[1] = fabs (a1-a3);  base[2] = fabs (a2-a3);
	base[3] = fabs (b1-b2);  base[4] = fabs (b1-b3);  base[5] = fabs (b2-b3);

	for (i=0; i<6; i++)	if (base[i] < min_base)
	{
		min_base = base[i];
		n_min = i;
	}
	
	if (n_min > 2)		/* min base in y-direction => take x */
		*Z = (X2 + X3 - 2*X1 + 2*Z1*(a1/c1) - Z2*(a2/c2) - Z3*(a3/c3))
		   / ((2*a1/c1) - (a2/c2) - (a3/c3));
	else				/* min base in y-direction => take x */
		*Z = (Y2 + Y3 - 2*Y1 + 2*Z1*(b1/c1) - Z2*(b2/c2) - Z3*(b3/c3))
		   / ((2*b1/c1) - (b2/c2) - (b3/c3));

	*X = (X1+(*Z-Z1)*(a1/c1) + X2+(*Z-Z2)*(a2/c2) + X3+(*Z-Z3)*(a3/c3)) / 3;
	*Y = (Y1+(*Z-Z1)*(b1/c1) + Y2+(*Z-Z2)*(b2/c2) + Y3+(*Z-Z3)*(b3/c3)) / 3;
}




       common/lsqadj.c                                                                                     0000644 0001131 0001131 00000005037 10253026712 012433  0                                                                                                    ustar   amns                            amns                                                                                                                                                                                                                   /* parts of code of adjlib.c from Horst Beyer, Hannes Kirndorfer */

#include "ptv.h"

void ata (a, ata, m, n)
int      m, n;
double   *a, *ata;  /* matrix a and resultmatrix ata = at a 
		       a is m * n, ata is n * n  */
{
  register int      i, j, k;
  
  for (i = 0; i < n; i++)
    {
      for (j = 0; j < n; j++)
	{
	  *(ata+i*n+j) = 0.0;
	  for (k = 0; k < m; k++)
	    *(ata+i*n+j) +=  *(a+k*n+i)  * *(a+k*n+j);
	}
    }
}	/* end ata.c */


void atl (u, a, l, m, n)
int      m, n;
double   *a, *u, *l;  /* matrix a , vector l and 
			 resultvector u = at l ,  a(m,n)  */

{  
  int      i, k;
  
  for (i = 0; i < n; i++)
    {
      *(u + i) = 0.0;
      for (k = 0; k < m; k++)
	*(u + i) += *(a + k * n + i) * *(l + k);
    }
  
} /* end atl.c */


void matinv (a, n)

double   *a;	/* input matrix size n * n */
int      n;         /* number of observations */

{
  int      ipiv, irow, icol;
  double   pivot;	/* pivot element = 1.0 / aii */
  double	npivot;	/*	negative of pivot */
  
  
  for (ipiv = 0; ipiv < n; ipiv++)
    {
      pivot = 1.0 / *(a + ipiv * n + ipiv);
      npivot = - pivot;
      for (irow = 0; irow < n; irow++)
	{
	  for (icol = 0; icol < n; icol++)
	    {
	      if (irow != ipiv && icol != ipiv)
		{
		  *(a + irow * n + icol) -= *(a + ipiv * n + icol) * 
		    *(a + irow * n + ipiv) * pivot;
		}
	    }
	}
      for (icol = 0; icol < n; icol++)
	{
	  if (ipiv != icol) 
	    *(a + ipiv * n + icol) *= npivot;
	}
      for (irow = 0; irow < n; irow++)
	{
	  if (ipiv != irow)
	    *(a + irow * n + ipiv) *= pivot;
	}
      *(a + ipiv * n + ipiv) = pivot;
    }
}	/* end matinv */


void matmul (a,b,c,m,n,k)
int    m,n,k;
double  *a,*b,*c;

{  int    i,j,l;
double  x,*pa,*pb,*pc;

for (i=0; i<k; i++)
  {  pb = b;
  pa = a++;
  for (j=0; j<m; j++)
    {  pc = c;
    x = 0.0;
    for (l=0; l<n; l++)
      {  x = x + *pb++ * *pc;
      pc += k;
      }
    *pa = x;
    pa += k;
    }
  c++;
  }
}

void transp (a,m,n)
double  a[];
int    m,n;
{  
  double  *b,*c,*d,*e;
  int    i,j;
  
  b = (double*) malloc (m*n*sizeof(double));
  if (b == 0) goto err;
  d = a;
  e = b;
  
  for (i=0; i<m; i++)
    {  c = b++;
    for (j=0; j<n; j++)
      {  *c = *a++;
      c += m;
      }
    }
  
  for (i=0; i<m*n; i++)
    *d++ = *e++;
  /*
    free (b);
    */   
  return;
  
err:
  printf ("\n\n ***   no memory space in C-subroutine transp   ***");
  printf ("\n\n");
  exit (-1);
}

void mat_transpose (mat1, mat2, m, n)
double	*mat1, *mat2;
int		n, m;
{
  int		i, j;
   
  for (i=0; i<m; i++)	for (j=0; j<n; j++)	*(mat2+j*m+i) = *(mat1+i*n+j);
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 common/multimed.c                                                                                   0000644 0001131 0001131 00000042757 10274652171 013016  0                                                                                                    ustar   amns                            amns                                                                                                                                                                                                                   /****************************************************************************

Routine:	       	multimed.c

Author/Copyright:      	Hans-Gerd Maas

Address:	      	Institute of Geodesy and Photogrammetry
		       	ETH - Hoenggerberg
	       	       	CH - 8093 Zurich

Creation Date:          20.4.88
	
Description:			
1 point, exterior orientation, multimedia parameters   	==> X,Y=shifts
		  (special case 3 media, medium 2 = plane parallel)
							
Routines contained:		-

****************************************************************************/

#include "ptv.h"

double get_mmf_from_mmLUT ();


void  multimed (ex,mm,X,Y,Z,Xq,Yq)
Exterior  ex;
mm_3p	  mm;
double    X, Y, Z, *Xq,*Yq;
{
  int    i, it=0;
  double beta1, beta2, beta3, r, rbeta, rdiff, rq, mmf;
  double ocf=1.0;      	/* over compensation factor for faster convergence */
   
  if (mm.n1==1 && mm.n2==1 && mm.n3==1)		/* 1-medium case */
    {
      *Xq = X;	*Yq = Y;	return;
    }
    
  /* interpolation in mmLUT, if selected (requires some global variables) */
  if (mm.lut)
    {
      /* check, which is the correct image */
      for (i=0; i<n_img; i++)
	if (Ex[i].x0 == ex.x0  &&  Ex[i].y0 == ex.y0  &&  Ex[i].z0 == ex.z0)
	  break;
      
      mmf = get_mmf_from_mmLUT (i, X,Y,Z);
      
      if (mmf > 0)
	{
	  *Xq = ex.x0 + (X-ex.x0) * mmf;
	  *Yq = ex.y0 + (Y-ex.y0) * mmf;
	  return;
	}
    }
  
  /* iterative procedure */
  
  r = sqrt ((X-ex.x0)*(X-ex.x0)+(Y-ex.y0)*(Y-ex.y0));
  rq = r;
  
  do
    {
      beta1 = atan (rq/(ex.z0-Z));
      beta2 = asin (sin(beta1) * mm.n1/mm.n2);
      beta3 = asin (sin(beta1) * mm.n1/mm.n3);
      
      rbeta = (ex.z0-mm.d) * tan(beta1) + mm.d * tan(beta2) - Z * tan(beta3);
      rdiff = r - rbeta;
      rdiff *= ocf;
      rq += rdiff;
      it++;
    }
  while (((rdiff > 0.001) || (rdiff < -0.001))  &&  it < 40);
  
  if (it >= 40)
    {
      puts ("Multimed stopped after 40. Iteration");
      if (fabs(rdiff) < 0.1)	
	{
	  *Xq = ex.x0 + (X-ex.x0) * rq/r;
	  *Yq = ex.y0 + (Y-ex.y0) * rq/r;
	}
      else
	{
	  *Xq = X; *Yq = Y;
	}
      return;
    }
   
  if (r != 0)
    {
      *Xq = ex.x0 + (X-ex.x0) * rq/r;
      *Yq = ex.y0 + (Y-ex.y0) * rq/r;
    }
  else
    {
      *Xq = X;
      *Yq = Y;
    }
}



double multimed_r (ex,mm,X,Y,Z)
/* calculates and returns the radial shift */
Exterior  ex;
mm_3p	  mm;
double	  X, Y, Z;
{
  int  	 i, it=0;
  double beta1, beta2, beta3, r, rbeta, rdiff, rq, mmf;
  double ocf=1.0;      	/* over compensation factor for faster convergence */
  
  if (mm.n1==1 && mm.n2==1 && mm.n3==1)	return (1.0);	/* 1-medium case */
    
  /* interpolation in mmLUT, if selected (requires some global variables) */
  if (mm.lut)
    {
      /* check, which is the correct image */
      for (i=0; i<n_img; i++)
	if (Ex[i].x0 == ex.x0  &&  Ex[i].y0 == ex.y0  &&  Ex[i].z0 == ex.z0)
	  break;
      
      mmf = get_mmf_from_mmLUT (i, X,Y,Z);
      
      if (mmf > 0)	return (mmf);
    }
  
  /* iterative procedure */
  r = sqrt ((X-ex.x0)*(X-ex.x0)+(Y-ex.y0)*(Y-ex.y0));
  rq = r;
  
  do
    {
      beta1 = atan (rq/(ex.z0-Z));
      beta2 = asin (sin(beta1) * mm.n1/mm.n2);
      beta3 = asin (sin(beta1) * mm.n1/mm.n3);
      
      rbeta = (ex.z0-mm.d) * tan(beta1) + mm.d * tan(beta2) - Z * tan(beta3);
      rdiff = r - rbeta;
      rdiff *= ocf;
      rq += rdiff;
      it++;
    }
  while (((rdiff > 0.001) || (rdiff < -0.001))  &&  it < 40);
  
  if (it >= 40)
    {
      puts ("Multimed_r stopped after 40. Iteration");	return (1.0);
    }
  
  if (r != 0)	return (rq/r);	else return (1.0);
}


void  multimed_nlay_true (ex,mm,X,Y,Z,Xq,Yq)
Exterior  ex;
mm_np	  mm;
double    X, Y, Z, *Xq,*Yq;
{
  int		i, it=0;
  double	beta1, beta2[32], beta3, r, rbeta, rdiff, rq, mmf;
  double	ocf=1.00;		/* over compensation factor for faster convergence */
  
  /* interpolation in mmLUT, if selected (requires some global variables) */
  if (mm.lut)
    {
      /* check, which is the correct image */
      for (i=0; i<n_img; i++)
	if (Ex[i].x0 == ex.x0  &&  Ex[i].y0 == ex.y0  &&  Ex[i].z0 == ex.z0)
	  break;
      
      mmf = get_mmf_from_mmLUT (i, X,Y,Z);
      
      if (mmf > 0)
	{
	  *Xq = ex.x0 + (X-ex.x0) * mmf;
	  *Yq = ex.y0 + (Y-ex.y0) * mmf;
	  return;
	}
    }
   
  /* iterative procedure */
  r = sqrt ((X-ex.x0)*(X-ex.x0)+(Y-ex.y0)*(Y-ex.y0));
  rq = r;
  
  do
    {
      beta1 = atan (rq/(ex.z0-Z));
      for (i=0; i<mm.nlay; i++)	beta2[i] = asin (sin(beta1) * mm.n1/mm.n2[i]);
      beta3 = asin (sin(beta1) * mm.n1/mm.n3);
      
      rbeta = (ex.z0-mm.d[0]) * tan(beta1) - Z * tan(beta3);
      for (i=0; i<mm.nlay; i++)	rbeta += (mm.d[i] * tan(beta2[i]));
      rdiff = r - rbeta;
      rdiff *= ocf;
      rq += rdiff;
      it++;
    }
  while (((rdiff > 0.001) || (rdiff < -0.001))  &&  it < 40);
  
  if (it >= 40)
    {
      *Xq = X; *Yq = Y;
      puts ("Multimed_nlay_true stopped after 40. Iteration");	return;
    }
    
  if (r != 0)
    {
      *Xq = ex.x0 + (X-ex.x0) * rq/r;
      *Yq = ex.y0 + (Y-ex.y0) * rq/r;
    }
  else
    {
      *Xq = X;
      *Yq = Y;
    }
}



void  multimed_nlay (ex,mm,X,Y,Z,Xq,Yq)
Exterior	ex;
mm_np		mm;
double  	X, Y, Z, *Xq,*Yq;
{
  int		i, it=0;
  double	beta1, beta2[32], beta3, r, rbeta, rdiff, rq, mmf;
  
  /* interpolation in mmLUT, if selected (requires some global variables) */
  if (mm.lut)
    {    
      /* check, which is the correct image */
      for (i=0; i<n_img; i++)
	if (Ex[i].x0 == ex.x0  &&  Ex[i].y0 == ex.y0  &&  Ex[i].z0 == ex.z0)
	  break;
      
      mmf = get_mmf_from_mmLUT (i, X,Y,Z);
      
      if (mmf > 0)
	{
	  *Xq = ex.x0 + (X-ex.x0) * mmf;
	  *Yq = ex.y0 + (Y-ex.y0) * mmf;
	  return;
	}
    }
  
  /* iterative procedure (if mmLUT does not exist or has no entry) */
  r = sqrt ((X-ex.x0)*(X-ex.x0)+(Y-ex.y0)*(Y-ex.y0));
  rq = r;
  
  do
    {
      beta1 = atan (rq/(ex.z0-Z));
      for (i=0; i<mm.nlay; i++)	beta2[i] = asin (sin(beta1) * mm.n1/mm.n2[i]);
      beta3 = asin (sin(beta1) * mm.n1/mm.n3);
      
      rbeta = (ex.z0-mm.d[0]) * tan(beta1) - Z * tan(beta3);
      for (i=0; i<mm.nlay; i++)	rbeta += (mm.d[i] * tan(beta2[i]));
      rdiff = r - rbeta;
      rq += rdiff;
      it++;
    }
  while (((rdiff > 0.001) || (rdiff < -0.001))  &&  it < 100);
  
  if (it >= 100)
    {
      *Xq = X; *Yq = Y;
      puts ("Multimed_nlay stopped after 100. Iteration");	return;
    }
    
  if (r != 0)
    {
      *Xq = ex.x0 + (X-ex.x0) * rq/r;
      *Yq = ex.y0 + (Y-ex.y0) * rq/r;
    }
  else
    {
      *Xq = X;
      *Yq = Y;
    }
}



double multimed_r_nlay (ex,mm,X,Y,Z)
/* calculates and returns the radial shift */
Exterior	ex;
mm_np		mm;
double		X, Y, Z;
{
  int  	i, it=0;
  double beta1, beta2[32], beta3, r, rbeta, rdiff, rq, mmf;
  double ocf=1.0; /* over compensation factor for faster convergence */
  
  
  /* 1-medium case */
  if (mm.n1==1 && mm.nlay == 1 && mm.n2[0]==1 && mm.n3==1) return (1.0);
    
  /* interpolation in mmLUT, if selected (requires some global variables) */
  if (mm.lut)
    {
      /* check, which is the correct image */
      for (i=0; i<n_img; i++)
	if (Ex[i].x0 == ex.x0  &&  Ex[i].y0 == ex.y0  &&  Ex[i].z0 == ex.z0)
	  break;
      
      mmf = get_mmf_from_mmLUT (i, X,Y,Z);
      
      if (mmf > 0)	return (mmf);
    }
 
  /* iterative procedure */
  r = sqrt ((X-ex.x0)*(X-ex.x0)+(Y-ex.y0)*(Y-ex.y0));
  rq = r;
  
  do
    {
      beta1 = atan (rq/(ex.z0-Z));
      for (i=0; i<mm.nlay; i++)	beta2[i] = asin (sin(beta1) * mm.n1/mm.n2[i]);
      beta3 = asin (sin(beta1) * mm.n1/mm.n3);
      
      rbeta = (ex.z0-mm.d[0]) * tan(beta1) - Z * tan(beta3);
      for (i=0; i<mm.nlay; i++)	rbeta += (mm.d[i] * tan(beta2[i]));
      rdiff = r - rbeta;
      rdiff *= ocf;
      rq += rdiff;
      it++;
    }
  while (((rdiff > 0.001) || (rdiff < -0.001))  &&  it < 40);
  
  if (it >= 40)
    {
      puts ("Multimed_r_nlay stopped after 40. Iteration");	return (1.0);
    }
  
  if (r != 0)	return (rq/r);	else return (1.0);
}



void init_mmLUT (i_cam)
int    	i_cam;
{
  register int	i,j, nr, nz;
  double       	X,Y,Z, R, X1,Y1,Z1, Zmin, Rmax=0,Zmax, a,b,c;
  double       	x,y, *Ri,*Zi;
  double       	rw = 2;
    
  /* find extrema of imaged object volume */
  /* ==================================== */
  
  /* find extrema in depth */
  
  fpp = fopen ("parameters/criteria.par", "r");
  fscanf (fpp, "%lf\n", &X);
  fscanf (fpp, "%lf\n", &Zmin);
  fscanf (fpp, "%lf\n", &Zmax);
  fscanf (fpp, "%lf\n", &X);
  fscanf (fpp, "%lf\n", &Z);	if (Z < Zmin)	Zmin = Z;
  fscanf (fpp, "%lf\n", &Z);	if (Z > Zmax)	Zmax = Z;
  fclose (fpp);
  
  Zmin -= fmod (Zmin, rw);
  Zmax += (rw - fmod (Zmax, rw));
  
  /* intersect with image vertices rays */
  
  pixel_to_metric (0., 0., imx,imy, pix_x,pix_y, &x,&y, chfield);
  x = x - I[i_cam].xh;
  y = y - I[i_cam].yh;
  correct_brown_affin (x, y, ap[i_cam], &x,&y);
  ray_tracing (x,y, Ex[i_cam], I[i_cam], mmp, &X1, &Y1, &Z1, &a, &b, &c);
  Z = Zmin;   X = X1 + (Z-Z1) * a/c;   Y = Y1 + (Z-Z1) * b/c;
  R = sqrt (  (X-Ex[i_cam].x0)*(X-Ex[i_cam].x0)
	      + (Y-Ex[i_cam].y0)*(Y-Ex[i_cam].y0));	
  
  if (R > Rmax)	Rmax = R;
  Z = Zmax;   X = X1 + (Z-Z1) * a/c;   Y = Y1 + (Z-Z1) * b/c;
  R = sqrt (  (X-Ex[i_cam].x0)*(X-Ex[i_cam].x0)
	      + (Y-Ex[i_cam].y0)*(Y-Ex[i_cam].y0));
  
  if (R > Rmax)	Rmax = R;
  pixel_to_metric (0., (double) imy, imx,imy, pix_x,pix_y, &x,&y, chfield);
  x = x - I[i_cam].xh;
  y = y - I[i_cam].yh;
  correct_brown_affin (x, y, ap[i_cam], &x,&y);
  ray_tracing (x,y, Ex[i_cam], I[i_cam], mmp, &X1, &Y1, &Z1, &a, &b, &c);
  Z = Zmin;   X = X1 + (Z-Z1) * a/c;   Y = Y1 + (Z-Z1) * b/c;
  R = sqrt (  (X-Ex[i_cam].x0)*(X-Ex[i_cam].x0)
	      + (Y-Ex[i_cam].y0)*(Y-Ex[i_cam].y0));
  
  if (R > Rmax)	Rmax = R;
  Z = Zmax;   X = X1 + (Z-Z1) * a/c;   Y = Y1 + (Z-Z1) * b/c;
  R = sqrt (  (X-Ex[i_cam].x0)*(X-Ex[i_cam].x0)
	      + (Y-Ex[i_cam].y0)*(Y-Ex[i_cam].y0));
  
  if (R > Rmax)	Rmax = R;
  
  pixel_to_metric ((double) imx, 0., imx,imy, pix_x,pix_y, &x,&y, chfield);
  x = x - I[i_cam].xh;
  y = y - I[i_cam].yh;
  correct_brown_affin (x, y, ap[i_cam], &x,&y);
  ray_tracing (x,y, Ex[i_cam], I[i_cam], mmp, &X1, &Y1, &Z1, &a, &b, &c);
  Z = Zmin;   X = X1 + (Z-Z1) * a/c;   Y = Y1 + (Z-Z1) * b/c;
  R = sqrt (  (X-Ex[i_cam].x0)*(X-Ex[i_cam].x0)
	      + (Y-Ex[i_cam].y0)*(Y-Ex[i_cam].y0));
  
  if (R > Rmax)	Rmax = R;
  Z = Zmax;   X = X1 + (Z-Z1) * a/c;   Y = Y1 + (Z-Z1) * b/c;
  R = sqrt (  (X-Ex[i_cam].x0)*(X-Ex[i_cam].x0)
	      + (Y-Ex[i_cam].y0)*(Y-Ex[i_cam].y0));	

  if (R > Rmax)	Rmax = R;
  pixel_to_metric ((double) imx, (double) imy,
		   imx,imy, pix_x,pix_y, &x,&y, chfield);
  x = x - I[i_cam].xh;
  y = y - I[i_cam].yh;
  correct_brown_affin (x, y, ap[i_cam], &x,&y);
  ray_tracing (x,y, Ex[i_cam], I[i_cam], mmp, &X1, &Y1, &Z1, &a, &b, &c);
  Z = Zmin;   X = X1 + (Z-Z1) * a/c;   Y = Y1 + (Z-Z1) * b/c;
  R = sqrt (  (X-Ex[i_cam].x0)*(X-Ex[i_cam].x0)
	      + (Y-Ex[i_cam].y0)*(Y-Ex[i_cam].y0));	
  
  if (R > Rmax)	Rmax = R;
  Z = Zmax;   X = X1 + (Z-Z1) * a/c;   Y = Y1 + (Z-Z1) * b/c;
  R = sqrt (  (X-Ex[i_cam].x0)*(X-Ex[i_cam].x0)
	      + (Y-Ex[i_cam].y0)*(Y-Ex[i_cam].y0));	
  
  if (R > Rmax)	Rmax = R;
  
  /* round values (-> enlarge) */
  Rmax += (rw - fmod (Rmax, rw));
    
  /* get # of rasterlines in r,z */
  nr = Rmax/rw + 1;
  nz = (Zmax-Zmin)/rw + 1;
 
  /* create twodimensional mmLUT structure */
  mmLUT[i_cam].origin.x = Ex[i_cam].x0;
  mmLUT[i_cam].origin.y = Ex[i_cam].y0;
  mmLUT[i_cam].origin.z = Zmin;
  mmLUT[i_cam].nr = nr;
  mmLUT[i_cam].nz = nz;
  mmLUT[i_cam].rw = rw;
  mmLUT[i_cam].data = (double *) malloc (nr*nz * sizeof (double));
   
  /* fill mmLUT structure */
  /* ==================== */
  
  Ri = (double *) malloc (nr * sizeof (double));
  for (i=0; i<nr; i++)	Ri[i] = i*rw;
  Zi = (double *) malloc (nz * sizeof (double));
  for (i=0; i<nz; i++)	Zi[i] = Zmin + i*rw;
  
  for (i=0; i<nr; i++)	for (j=0; j<nz; j++)
    {
      mmLUT[i_cam].data[i*nz + j]
	= multimed_r_nlay (Ex[i_cam], mmp,
			   Ri[i]+Ex[i_cam].x0, Ex[i_cam].y0, Zi[j]);
    }
}



double get_mmf_from_mmLUT (i_cam, X,Y,Z)
int		i_cam;
double	X,Y,Z;
{
  int		i, ir,iz, nr,nz, v4[4];
  double	R, sr,sz, rw, mmf=1;
  
  rw =  mmLUT[i_cam].rw;
  
  Z -= mmLUT[i_cam].origin.z; sz = Z/rw; iz = (int) sz;	sz -= iz;
  X -= mmLUT[i_cam].origin.x;
  Y -= mmLUT[i_cam].origin.y;
  R = sqrt (X*X + Y*Y);	sr = R/rw; ir = (int) sr; sr -= ir;
    
  nz =  mmLUT[i_cam].nz;
  nr =  mmLUT[i_cam].nr;
    
  /* check whether point is inside camera's object volume */
  if (ir > nr)				return (0);
  if (iz < 0  ||  iz > nz)	return (0);
  
  /* bilinear interpolation in r/z box */
  /* ================================= */
  
  /* get vertices of box */
  v4[0] = ir*nz + iz;
  v4[1] = ir*nz + (iz+1);
  v4[2] = (ir+1)*nz + iz;
  v4[3] = (ir+1)*nz + (iz+1);
  
  /* 2. check wther point is inside camera's object volume */
  /* important for epipolar line computation */
  for (i=0; i<4; i++)
    if (v4[i] < 0  ||  v4[i] > nr*nz)	return (0);
  
  /* interpolate */
  mmf = mmLUT[i_cam].data[v4[0]] * (1-sr)*(1-sz)
    + mmLUT[i_cam].data[v4[1]] * (1-sr)*sz
    + mmLUT[i_cam].data[v4[2]] * sr*(1-sz)
    + mmLUT[i_cam].data[v4[3]] * sr*sz;
  
  return (mmf);
}



void volumedimension (xmax, xmin, ymax, ymin, zmax, zmin)
double *xmax, *xmin, *ymax, *ymin, *zmax, *zmin;
{
  int	i_cam;
  double X,Y,Z, R, X1,Y1,Z1, Zmin, Rmax=0,Zmax, a,b,c;
  double x,y;
    
  /* find extrema of imaged object volume */
  /* ==================================== */
  
  fpp = fopen ("parameters/criteria.par", "r");
  fscanf (fpp, "%lf\n", &X);
  fscanf (fpp, "%lf\n", &Zmin);
  fscanf (fpp, "%lf\n", &Zmax);
  fscanf (fpp, "%lf\n", &X);
  fscanf (fpp, "%lf\n", &Z);	if (Z < Zmin)	Zmin = Z;
  fscanf (fpp, "%lf\n", &Z);	if (Z > Zmax)	Zmax = Z;
  fclose (fpp);

  *zmin=Zmin;
  *zmax=Zmax;

  for (i_cam=0;i_cam<n_img;i_cam++)
    {  
      /* intersect with image vertices rays */
      pixel_to_metric (0.0, 0.0, imx,imy, pix_x,pix_y, &x,&y, chfield);
      x = x - I[i_cam].xh;
      y = y - I[i_cam].yh;
      correct_brown_affin (x, y, ap[i_cam], &x,&y);
      ray_tracing (x,y, Ex[i_cam], I[i_cam], mmp, &X1, &Y1, &Z1, &a, &b, &c);
      Z = Zmin;   X = X1 + (Z-Z1) * a/c;   Y = Y1 + (Z-Z1) * b/c;
      R = sqrt (  (X-Ex[i_cam].x0)*(X-Ex[i_cam].x0)
		  + (Y-Ex[i_cam].y0)*(Y-Ex[i_cam].y0));	

      if ( X > *xmax) *xmax=X;
      if ( X < *xmin) *xmin=X;
      if ( Y > *ymax) *ymax=Y;
      if ( Y < *ymin) *ymin=Y;

      if (R > Rmax)	Rmax = R;
      Z = Zmax;   X = X1 + (Z-Z1) * a/c;   Y = Y1 + (Z-Z1) * b/c;
      R = sqrt (  (X-Ex[i_cam].x0)*(X-Ex[i_cam].x0)
		  + (Y-Ex[i_cam].y0)*(Y-Ex[i_cam].y0));

      if ( X > *xmax) *xmax=X;
      if ( X < *xmin) *xmin=X;
      if ( Y > *ymax) *ymax=Y;
      if ( Y < *ymin) *ymin=Y;
      
      if (R > Rmax)	Rmax = R;
      pixel_to_metric (0.0, (double) imy, imx,imy, pix_x,pix_y, &x,&y, chfield);
      x = x - I[i_cam].xh;
      y = y - I[i_cam].yh;
      correct_brown_affin (x, y, ap[i_cam], &x,&y);
      ray_tracing (x,y, Ex[i_cam], I[i_cam], mmp, &X1, &Y1, &Z1, &a, &b, &c);
      Z = Zmin;   X = X1 + (Z-Z1) * a/c;   Y = Y1 + (Z-Z1) * b/c;
      R = sqrt (  (X-Ex[i_cam].x0)*(X-Ex[i_cam].x0)
		  + (Y-Ex[i_cam].y0)*(Y-Ex[i_cam].y0));

      if ( X > *xmax) *xmax=X;
      if ( X < *xmin) *xmin=X;
      if ( Y > *ymax) *ymax=Y;
      if ( Y < *ymin) *ymin=Y;
      
      if (R > Rmax)	Rmax = R;
      Z = Zmax;   X = X1 + (Z-Z1) * a/c;   Y = Y1 + (Z-Z1) * b/c;
      R = sqrt (  (X-Ex[i_cam].x0)*(X-Ex[i_cam].x0)
		  + (Y-Ex[i_cam].y0)*(Y-Ex[i_cam].y0));

      if ( X > *xmax) *xmax=X;
      if ( X < *xmin) *xmin=X;
      if ( Y > *ymax) *ymax=Y;
      if ( Y < *ymin) *ymin=Y;
      
      if (R > Rmax)	Rmax = R;
      
      pixel_to_metric ((double) imx, 0., imx,imy, pix_x,pix_y, &x,&y, chfield);
      x = x - I[i_cam].xh;
      y = y - I[i_cam].yh;
      correct_brown_affin (x, y, ap[i_cam], &x,&y);
      ray_tracing (x,y, Ex[i_cam], I[i_cam], mmp, &X1, &Y1, &Z1, &a, &b, &c);
      Z = Zmin;   X = X1 + (Z-Z1) * a/c;   Y = Y1 + (Z-Z1) * b/c;
      R = sqrt (  (X-Ex[i_cam].x0)*(X-Ex[i_cam].x0)
		  + (Y-Ex[i_cam].y0)*(Y-Ex[i_cam].y0));

      if ( X > *xmax) *xmax=X;
      if ( X < *xmin) *xmin=X;
      if ( Y > *ymax) *ymax=Y;
      if ( Y < *ymin) *ymin=Y;
      
      if (R > Rmax)	Rmax = R;
      Z = Zmax;   X = X1 + (Z-Z1) * a/c;   Y = Y1 + (Z-Z1) * b/c;
      R = sqrt (  (X-Ex[i_cam].x0)*(X-Ex[i_cam].x0)
		  + (Y-Ex[i_cam].y0)*(Y-Ex[i_cam].y0));	

      if ( X > *xmax) *xmax=X;
      if ( X < *xmin) *xmin=X;
      if ( Y > *ymax) *ymax=Y;
      if ( Y < *ymin) *ymin=Y;
      
      if (R > Rmax)	Rmax = R;
      pixel_to_metric ((double) imx, (double) imy,
		       imx,imy, pix_x,pix_y, &x,&y, chfield);
      x = x - I[i_cam].xh;
      y = y - I[i_cam].yh;
      correct_brown_affin (x, y, ap[i_cam], &x,&y);
      ray_tracing (x,y, Ex[i_cam], I[i_cam], mmp, &X1, &Y1, &Z1, &a, &b, &c);
      Z = Zmin;   X = X1 + (Z-Z1) * a/c;   Y = Y1 + (Z-Z1) * b/c;
      R = sqrt (  (X-Ex[i_cam].x0)*(X-Ex[i_cam].x0)
		  + (Y-Ex[i_cam].y0)*(Y-Ex[i_cam].y0));	

      if ( X > *xmax) *xmax=X;
      if ( X < *xmin) *xmin=X;
      if ( Y > *ymax) *ymax=Y;
      if ( Y < *ymin) *ymin=Y;
      
      if (R > Rmax)	Rmax = R;
      Z = Zmax;   X = X1 + (Z-Z1) * a/c;   Y = Y1 + (Z-Z1) * b/c;
      R = sqrt (  (X-Ex[i_cam].x0)*(X-Ex[i_cam].x0)
		  + (Y-Ex[i_cam].y0)*(Y-Ex[i_cam].y0));           

      if ( X > *xmax) *xmax=X;
      if ( X < *xmin) *xmin=X;
      if ( Y > *ymax) *ymax=Y;
      if ( Y < *ymin) *ymin=Y; 
    }
    /*WAMNS - Output calculated volume extents*/
  if( verbose ) {
    printf("\n********************\n");
    printf("xmin = %f\n",*xmin);
    printf("xmax = %f\n",*xmax);
    printf("ymin = %f\n",*ymin);
    printf("ymax = %f\n",*ymax);
    printf("zmin = %f\n",*zmin);
    printf("zmax = %f\n",*zmax);
    printf("\n********************\n");
  }
    
}
                 common/pointpos.c                                                                                   0000644 0001131 0001131 00000053470 10274652323 013042  0                                                                                                    ustar   amns                            amns                                                                                                                                                                                                                   /****************************************************************************

Routine:	       	pointpos.c

Author/Copyright:      	Hans-Gerd Maas

Address:	       	Institute of Geodesy and Photogrammetry
		       	ETH - Hoenggerberg
		       	CH - 8093 Zurich

Creation Date:	       	July 1988, June 1989
	
Description:	       	point positioning with least squares adjustment
		       	(multimedia, 2 or 3 channels)
	
Routines contained:		

****************************************************************************/
#include "ptv.h"

void det_lsq (Ex, I, ap, mm, x1, y1, x2, y2, x3, y3, x4, y4, Xp, Yp, Zp)
Exterior	Ex[4];
Interior	I[4];
ap_52		ap[4];
mm_np		mm;
double		x1, y1, x2, y2, x3, y3, x4, y4, *Xp,*Yp,*Zp;
{
  int		i, j, itnum, stopflag, n_obs;
  double	X[8][3], XtX[3][3], y[8], Xty[3], beta[3],
    Xbeta[8], edach[8], omega=0., sigma0, sigmabeta[3];
  double	Zx, Zy, N, mmf;
  double	xp1, yp1;
  double	xp[4], yp[4];
  
  double	multimed_r_nlay ();
  
  /* static parameters for examination of Qxx, Qvv */
  static int   	Sn;
  static double	Ssigma0, SQxx[3][3], SQvv[8][8];
  double	Xt[3][8], buf[3][8], Qvv[8][8];
  
  xp[0] = x1;	xp[1] = x2;	xp[2] = x3;	xp[3] = x4;
  yp[0] = y1;	yp[1] = y2;	yp[2] = y3;	yp[3] = y4;

  /* approximate values: rt_is with the uncorrected image coordinates */
  /* of the first 2 rays		*/
  
  /*
    for (i=0; i<n_img; i++)	if (xp[i] > -1e10  &&  yp[i] > -1e10)
    {
    ray_tracing (xp[i], yp[i], Ex[i], I[i],
    mm, &X1, &Y1, &Z1, &a1, &b1, &c1);	break;
    }
    for (i=i+1; i<n_img; i++)	if (xp[i] > -1e10  &&  yp[i] > -1e10)
    {
    ray_tracing (xp[i], yp[i], Ex[i], I[i],
    mm, &X2, &Y2, &Z2, &a2, &b2, &c2);	break;
    }
    
    intersect_rt (X1,Y1,Z1,a1,b1,c1, X2,Y2,Z2,a2,b2,c2, Xp, Yp, Zp);
    */  
  
  /* hack due to problems with approx in det_lsq: */
  
  /************************************************************************/  
  /* least squares adjustment */
  itnum = 0;	stopflag = 0;
  while ((stopflag == 0) && (itnum < 10))
    {
      /********************************************************************/
      /* X-matrix  +  observation vector from image coordinates */
      
      for (i=0; i<4; i++)
	{
	  if (xp[i] > -1e10  &&  yp[i] > -1e10)
	    {
	      /* derivatives d(x(i),y(i)) / d(X,Y,Z)   	(Kraus)	*/
	      
	      Zx =  Ex[i].dm[0][0] * (*Xp - Ex[i].x0)
		+ Ex[i].dm[1][0] * (*Yp - Ex[i].y0)
		+ Ex[i].dm[2][0] * (*Zp - Ex[i].z0);
	      Zy =  Ex[i].dm[0][1] * (*Xp - Ex[i].x0)
		+ Ex[i].dm[1][1] * (*Yp - Ex[i].y0)
		+ Ex[i].dm[2][1] * (*Zp - Ex[i].z0);
	      N  =  Ex[i].dm[0][2] * (*Xp - Ex[i].x0)
		+ Ex[i].dm[1][2] * (*Yp - Ex[i].y0)
		+ Ex[i].dm[2][2] * (*Zp - Ex[i].z0);
	      
	      for (j=0; j<3; j++)
		X[2*i][j] = ((-I[i].cc)/(N*N))
		  * (N*Ex[i].dm[j][0] - Zx*Ex[i].dm[j][2]);
	      for (j=0; j<3; j++)
		X[2*i+1][j] = ((-I[i].cc)/(N*N))
		  * (N*Ex[i].dm[j][1] - Zy*Ex[i].dm[j][2]);
	            
	      /* multimedia factor (radial shift) */

	      mmf = multimed_r_nlay (Ex[i], mm, *Xp, *Yp, *Zp);

	      X[2*i][0] *= mmf;	X[2*i][1] *= mmf;
	      X[2*i+1][0] *= mmf;	X[2*i+1][1] *= mmf;
	      
	      /* observation vector */
	      img_coord (*Xp,*Yp,*Zp, Ex[i], I[i], ap[i], mm, &xp1, &yp1);
	      
	      y[2*i] = xp[i] - xp1;	y[2*i+1] = yp[i] - yp1;
	    }
	  
	  else							/* set values to 0 */
	    {
	      for (j=0; j<3; j++)
		{
		  X[2*i][j] = 0; X[2*i+1][j] = 0;
		}
	      y[2*i] = 0;  y[2*i+1] = 0;
	    }
	}
      
      /********************************************************************/
      
      /* Gaussian algorithm */
      
      ata (X, XtX, 8, 3);
      matinv (XtX, 3);
      atl (Xty, X, y, 8, 3);
      matmul (beta, XtX, Xty, 3,3,1);
      
      stopflag = 1;
      for (i=0; i<3; i++)  
	if (fabs(beta[i]) > 0.005)   stopflag = 0;
      
      *Xp += beta[0];  *Yp += beta[1];  *Zp += beta[2];	itnum++;
    }
  
  /************************************************************************/
   
  /* residuals */
  
  /* number of observations for sigma0 */
  for (i=0, n_obs=0; i<n_img; i++)
    if (xp[i] > -1e10  &&  yp[i] > -1e10)  n_obs += 2;

  matmul (Xbeta, X, beta, 8, 3, 1);
  for (i=0; i<n_obs; i++)
    {
      edach[i] = Xbeta[i] - y[i];
      omega += edach[i] * edach[i];
    }
  sigma0 = sqrt(omega /(n_obs-3));
  for (i=0; i<3; i++)  sigmabeta[i] = sigma0 * sqrt(XtX[i][i]);
  

  mean_sigma0 += sigma0 * sigma0;
  rmsX += sigmabeta[0] * sigmabeta[0];
  rmsY += sigmabeta[1] * sigmabeta[1];
  rmsZ += sigmabeta[2] * sigmabeta[2];

  if (examine == 5)
    {
      /* statistical examinations of Qxx, Qvv */
      
      Sn++;
      Ssigma0 += sigma0;
      
      for (i=0; i<3; i++)  for (j=0; j<3; j++)	SQxx[i][j] += XtX[i][j];
      
      mat_transpose (X, Xt, 8, 3);
      matmul (buf, XtX, Xt, 3,3,8);
      matmul (Qvv, X, buf, 8,3,8);
      for (i=0; i<8; i++)  for (j=0; j<8; j++)	Qvv[i][j] *= -1;
      for (i=0; i<8; i++)  Qvv[i][i] = 1 + Qvv[i][i];
      
      for (i=0; i<8; i++)  for (j=0; j<8; j++)	SQvv[i][j] += Qvv[i][j];
      
      if (Sn == match)
	{
	// chrisb added: verbosity flag
	if( verbose ) {
	  printf ("mean values over %d points", Sn);
	  printf ("mean sigma0 = %6.3f micron\n\n", 1000*Ssigma0/Sn);
	  puts ("mean Qxx:");
	  for (i=0; i<3; i++)
	    {
	      for (j=0; j<3; j++) printf ("%6.3f   ", SQxx[i][j]/Sn);
	      printf ("\n");
	    }
	  puts ("mean Qvv:");
	  for (i=0; i<8; i++)
	    {
	      for (j=0; j<8; j++) printf ("%6.3f   ", SQvv[i][j]/Sn);
	      printf ("\n");
	    }
	}
	}
    }
}


void det_lsq_3 (Ex, I, ap, mm, x1, y1, x2, y2, x3, y3, Xp, Yp, Zp)
Exterior	Ex[3];
Interior	I[3];
ap_52		ap[3];
mm_np		mm;
double		x1, y1, x2, y2, x3, y3, *Xp,*Yp,*Zp;
{
  int		i, itnum, stopflag;
  double	X[6][3], XtX[3][3], y[6], Xty[3], beta[3],
    Xbeta[6], edach[6], omega=0., sigma0, sigmabeta[3];
  double	Zx, Zy, N, mmf;
  double	xp1, yp1, xp2, yp2, xp3, yp3;
  double	X1,Y1,Z1, a1,b1,c1,X2,Y2,Z2, a2,b2,c2;
  
  double	multimed_r_nlay ();
  
  
  /* approximate values (rt_is with the uncorrected image coordinates) */
  ray_tracing (x1, y1, Ex[0], I[0],
	       mm, &X1, &Y1, &Z1, &a1, &b1, &c1);
  ray_tracing (x2, y2, Ex[1],I[1],
	       mm, &X2, &Y2, &Z2, &a2, &b2, &c2);
  intersect_rt (X1,Y1,Z1,a1,b1,c1, X2,Y2,Z2,a2,b2,c2, Xp, Yp, Zp);
  
  /************************************************************************/
  
  itnum = 0;	stopflag = 0;
  while ((stopflag == 0) && (itnum < 10))
    {
      
      /********************************************************************/
      /* derivatives d(x',y') / d(X,Y,Z)  (Kraus) */
      
      Zx =  Ex[0].dm[0][0] * (*Xp - Ex[0].x0)
	+ Ex[0].dm[1][0] * (*Yp - Ex[0].y0)
	+ Ex[0].dm[2][0] * (*Zp - Ex[0].z0);
      Zy =  Ex[0].dm[0][1] * (*Xp - Ex[0].x0)
	+ Ex[0].dm[1][1] * (*Yp - Ex[0].y0)
	+ Ex[0].dm[2][1] * (*Zp - Ex[0].z0);
      N  =  Ex[0].dm[0][2] * (*Xp - Ex[0].x0)
	+ Ex[0].dm[1][2] * (*Yp - Ex[0].y0)
	+ Ex[0].dm[2][2] * (*Zp - Ex[0].z0);
      
      X[0][0] = ((-I[0].cc)/(N*N)) * (N*Ex[0].dm[0][0] - Zx*Ex[0].dm[0][2]);
      X[0][1] = ((-I[0].cc)/(N*N)) * (N*Ex[0].dm[1][0] - Zx*Ex[0].dm[1][2]);
      X[0][2] = ((-I[0].cc)/(N*N)) * (N*Ex[0].dm[2][0] - Zx*Ex[0].dm[2][2]);
      X[1][0] = ((-I[0].cc)/(N*N)) * (N*Ex[0].dm[0][1] - Zy*Ex[0].dm[0][2]);
      X[1][1] = ((-I[0].cc)/(N*N)) * (N*Ex[0].dm[1][1] - Zy*Ex[0].dm[1][2]);
      X[1][2] = ((-I[0].cc)/(N*N)) * (N*Ex[0].dm[2][1] - Zy*Ex[0].dm[2][2]);
      
      mmf = multimed_r_nlay (Ex[0], mm, *Xp, *Yp, *Zp);
      X[0][0] *= mmf;	X[0][1] *= mmf;	X[1][0] *= mmf;	X[1][1] *= mmf;
      
      
      /********************************************************************/
      /* derivatives d(x'',y'') / d(X,Y,Z)  (Kraus) */
      
      Zx =  Ex[1].dm[0][0] * (*Xp - Ex[1].x0)
	+ Ex[1].dm[1][0] * (*Yp - Ex[1].y0)
	+ Ex[1].dm[2][0] * (*Zp - Ex[1].z0);
      Zy =  Ex[1].dm[0][1] * (*Xp - Ex[1].x0)
	+ Ex[1].dm[1][1] * (*Yp - Ex[1].y0)
	+ Ex[1].dm[2][1] * (*Zp - Ex[1].z0);
      N  =  Ex[1].dm[0][2] * (*Xp - Ex[1].x0)
	+ Ex[1].dm[1][2] * (*Yp - Ex[1].y0)
	+ Ex[1].dm[2][2] * (*Zp - Ex[1].z0);
      
      X[2][0] = ((-I[1].cc)/(N*N)) * (N*Ex[1].dm[0][0] - Zx*Ex[1].dm[0][2]);
      X[2][1] = ((-I[1].cc)/(N*N)) * (N*Ex[1].dm[1][0] - Zx*Ex[1].dm[1][2]);
      X[2][2] = ((-I[1].cc)/(N*N)) * (N*Ex[1].dm[2][0] - Zx*Ex[1].dm[2][2]);
      X[3][0] = ((-I[1].cc)/(N*N)) * (N*Ex[1].dm[0][1] - Zy*Ex[1].dm[0][2]);
      X[3][1] = ((-I[1].cc)/(N*N)) * (N*Ex[1].dm[1][1] - Zy*Ex[1].dm[1][2]);
      X[3][2] = ((-I[1].cc)/(N*N)) * (N*Ex[1].dm[2][1] - Zy*Ex[1].dm[2][2]);
      
      mmf = multimed_r_nlay (Ex[1], mm, *Xp, *Yp, *Zp);
      X[2][0] *= mmf;	X[2][1] *= mmf;	X[3][0] *= mmf;	X[3][1] *= mmf;
      
      
      /********************************************************************/
      /* derivatives d(x''',y''') / d(X,Y,Z)  (Kraus) */
      
      Zx =  Ex[2].dm[0][0] * (*Xp - Ex[2].x0)
	+ Ex[2].dm[1][0] * (*Yp - Ex[2].y0)
	+ Ex[2].dm[2][0] * (*Zp - Ex[2].z0);
      Zy =  Ex[2].dm[0][1] * (*Xp - Ex[2].x0)
	+ Ex[2].dm[1][1] * (*Yp - Ex[2].y0)
	+ Ex[2].dm[2][1] * (*Zp - Ex[2].z0);
      N  =  Ex[2].dm[0][2] * (*Xp - Ex[2].x0)
	+ Ex[2].dm[1][2] * (*Yp - Ex[2].y0)
	+ Ex[2].dm[2][2] * (*Zp - Ex[2].z0);
      
      X[4][0] = ((-I[2].cc)/(N*N)) * (N*Ex[2].dm[0][0] - Zx*Ex[2].dm[0][2]);
      X[4][1] = ((-I[2].cc)/(N*N)) * (N*Ex[2].dm[1][0] - Zx*Ex[2].dm[1][2]);
      X[4][2] = ((-I[2].cc)/(N*N)) * (N*Ex[2].dm[2][0] - Zx*Ex[2].dm[2][2]);
      X[5][0] = ((-I[2].cc)/(N*N)) * (N*Ex[2].dm[0][1] - Zy*Ex[2].dm[0][2]);
      X[5][1] = ((-I[2].cc)/(N*N)) * (N*Ex[2].dm[1][1] - Zy*Ex[2].dm[1][2]);
      X[5][2] = ((-I[2].cc)/(N*N)) * (N*Ex[2].dm[2][1] - Zy*Ex[2].dm[2][2]);
      
      mmf = multimed_r_nlay (Ex[2], mm, *Xp, *Yp, *Zp);
      X[4][0] *= mmf;	X[4][1] *= mmf;	X[5][0] *= mmf;	X[5][1] *= mmf;
      
      
      /********************************************************************/
      
      img_coord (*Xp,*Yp,*Zp, Ex[0],I[0], ap[0], mm, &xp1, &yp1);
      img_coord (*Xp,*Yp,*Zp, Ex[1],I[1], ap[1], mm, &xp2, &yp2);
      img_coord (*Xp,*Yp,*Zp, Ex[2],I[2], ap[2], mm, &xp3, &yp3);
      
      y[0] = x1 - xp1;	y[2] = x2 - xp2;	y[4] = x3 - xp3;
      y[1] = y1 - yp1;	y[3] = y2 - yp2;	y[5] = y3 - yp3;
      
      
      /********************************************************************/
      /* Gaussian algorithm */
      
      ata (X, XtX, 6, 3);
      matinv (XtX, 3);
      atl (Xty, X, y, 6, 3);
      matmul (beta, XtX, Xty, 3,3,1);
      
      stopflag = 1;
      for (i=0; i<3; i++)  
	if (fabs(beta[i]) > 0.005)  stopflag = 0;
      
      *Xp += beta[0];  *Yp += beta[1];  *Zp += beta[2];	itnum++;
    }
  
  
  /************************************************************************/
  
  
  /* residuals */
  
  matmul (Xbeta, X, beta, 6, 3, 1);
  for (i=0; i<6; i++)
    {
      edach[i] = Xbeta[i] - y[i];
      omega += edach[i] * edach[i];
    }
  sigma0 = sqrt(omega /(6-3));
  for (i=0; i<3; i++)  sigmabeta[i] = sigma0 * sqrt(XtX[i][i]);
  
  
  mean_sigma0 += sigma0 * sigma0;
  rmsX += sigmabeta[0] * sigmabeta[0];
  rmsY += sigmabeta[1] * sigmabeta[1];
  rmsZ += sigmabeta[2] * sigmabeta[2];
}


void det_lsq_4 (Ex, I, ap, mm, x1, y1, x2, y2, x3, y3, x4, y4, Xp, Yp, Zp)
Exterior	Ex[4];
Interior	I[4];
ap_52		ap[4];
mm_np		mm;
double		x1, y1, x2, y2, x3, y3, x4, y4, *Xp,*Yp,*Zp;
{
  int		i, itnum, stopflag;
  double	X[8][3], XtX[3][3], y[8], Xty[3], beta[3],
    Xbeta[8], edach[8], omega=0., sigma0, sigmabeta[3];
  double	Zx, Zy, N, mmf;
  double	xp1, yp1, xp2, yp2, xp3, yp3, xp4, yp4;
  double	X1,Y1,Z1, a1,b1,c1,X2,Y2,Z2, a2,b2,c2;
  
  double	multimed_r_nlay ();
  
  
  /* approximate values (rt_is with the uncorrected image coordinates) */
  ray_tracing (x1, y1, Ex[0], I[0],
	       mm, &X1, &Y1, &Z1, &a1, &b1, &c1);
  ray_tracing (x2, y2, Ex[1], I[1],
	       mm, &X2, &Y2, &Z2, &a2, &b2, &c2);
  intersect_rt (X1,Y1,Z1,a1,b1,c1, X2,Y2,Z2,a2,b2,c2, Xp, Yp, Zp);
  
  /************************************************************************/
  
  itnum = 0;	stopflag = 0;
  while ((stopflag == 0) && (itnum < 10))
    {
      
      /********************************************************************/
      /* derivatives d(x',y') / d(X,Y,Z)  (Kraus) */
      
      Zx =  Ex[0].dm[0][0] * (*Xp - Ex[0].x0)
	+ Ex[0].dm[1][0] * (*Yp - Ex[0].y0)
	+ Ex[0].dm[2][0] * (*Zp - Ex[0].z0);
      Zy =  Ex[0].dm[0][1] * (*Xp - Ex[0].x0)
	+ Ex[0].dm[1][1] * (*Yp - Ex[0].y0)
	+ Ex[0].dm[2][1] * (*Zp - Ex[0].z0);
      N  =  Ex[0].dm[0][2] * (*Xp - Ex[0].x0)
	+ Ex[0].dm[1][2] * (*Yp - Ex[0].y0)
	+ Ex[0].dm[2][2] * (*Zp - Ex[0].z0);
      
      X[0][0] = ((-I[0].cc)/(N*N)) * (N*Ex[0].dm[0][0] - Zx*Ex[0].dm[0][2]);
      X[0][1] = ((-I[0].cc)/(N*N)) * (N*Ex[0].dm[1][0] - Zx*Ex[0].dm[1][2]);
      X[0][2] = ((-I[0].cc)/(N*N)) * (N*Ex[0].dm[2][0] - Zx*Ex[0].dm[2][2]);
      X[1][0] = ((-I[0].cc)/(N*N)) * (N*Ex[0].dm[0][1] - Zy*Ex[0].dm[0][2]);
      X[1][1] = ((-I[0].cc)/(N*N)) * (N*Ex[0].dm[1][1] - Zy*Ex[0].dm[1][2]);
      X[1][2] = ((-I[0].cc)/(N*N)) * (N*Ex[0].dm[2][1] - Zy*Ex[0].dm[2][2]);
      
      mmf = multimed_r_nlay (Ex[0], mm, *Xp, *Yp, *Zp);
      X[0][0] *= mmf;	X[0][1] *= mmf;	X[1][0] *= mmf;	X[1][1] *= mmf;
      
      
      /********************************************************************/
      /* derivatives d(x'',y'') / d(X,Y,Z)  (Kraus) */
      
      Zx =  Ex[1].dm[0][0] * (*Xp - Ex[1].x0)
	+ Ex[1].dm[1][0] * (*Yp - Ex[1].y0)
	+ Ex[1].dm[2][0] * (*Zp - Ex[1].z0);
      Zy =  Ex[1].dm[0][1] * (*Xp - Ex[1].x0)
	+ Ex[1].dm[1][1] * (*Yp - Ex[1].y0)
	+ Ex[1].dm[2][1] * (*Zp - Ex[1].z0);
      N  =  Ex[1].dm[0][2] * (*Xp - Ex[1].x0)
	+ Ex[1].dm[1][2] * (*Yp - Ex[1].y0)
	+ Ex[1].dm[2][2] * (*Zp - Ex[1].z0);
      
      X[2][0] = ((-I[1].cc)/(N*N)) * (N*Ex[1].dm[0][0] - Zx*Ex[1].dm[0][2]);
      X[2][1] = ((-I[1].cc)/(N*N)) * (N*Ex[1].dm[1][0] - Zx*Ex[1].dm[1][2]);
      X[2][2] = ((-I[1].cc)/(N*N)) * (N*Ex[1].dm[2][0] - Zx*Ex[1].dm[2][2]);
      X[3][0] = ((-I[1].cc)/(N*N)) * (N*Ex[1].dm[0][1] - Zy*Ex[1].dm[0][2]);
      X[3][1] = ((-I[1].cc)/(N*N)) * (N*Ex[1].dm[1][1] - Zy*Ex[1].dm[1][2]);
      X[3][2] = ((-I[1].cc)/(N*N)) * (N*Ex[1].dm[2][1] - Zy*Ex[1].dm[2][2]);
      
      mmf = multimed_r_nlay (Ex[1], mm, *Xp, *Yp, *Zp);
      X[2][0] *= mmf;	X[2][1] *= mmf;	X[3][0] *= mmf;	X[3][1] *= mmf;
      
      
      /********************************************************************/
      /* derivatives d(x''',y''') / d(X,Y,Z)  (Kraus) */
      
      Zx =  Ex[2].dm[0][0] * (*Xp - Ex[2].x0)
	+ Ex[2].dm[1][0] * (*Yp - Ex[2].y0)
	+ Ex[2].dm[2][0] * (*Zp - Ex[2].z0);
      Zy =  Ex[2].dm[0][1] * (*Xp - Ex[2].x0)
	+ Ex[2].dm[1][1] * (*Yp - Ex[2].y0)
	+ Ex[2].dm[2][1] * (*Zp - Ex[2].z0);
      N  =  Ex[2].dm[0][2] * (*Xp - Ex[2].x0)
	+ Ex[2].dm[1][2] * (*Yp - Ex[2].y0)
	+ Ex[2].dm[2][2] * (*Zp - Ex[2].z0);
      
      X[4][0] = ((-I[2].cc)/(N*N)) * (N*Ex[2].dm[0][0] - Zx*Ex[2].dm[0][2]);
      X[4][1] = ((-I[2].cc)/(N*N)) * (N*Ex[2].dm[1][0] - Zx*Ex[2].dm[1][2]);
      X[4][2] = ((-I[2].cc)/(N*N)) * (N*Ex[2].dm[2][0] - Zx*Ex[2].dm[2][2]);
      X[5][0] = ((-I[2].cc)/(N*N)) * (N*Ex[2].dm[0][1] - Zy*Ex[2].dm[0][2]);
      X[5][1] = ((-I[2].cc)/(N*N)) * (N*Ex[2].dm[1][1] - Zy*Ex[2].dm[1][2]);
      X[5][2] = ((-I[2].cc)/(N*N)) * (N*Ex[2].dm[2][1] - Zy*Ex[2].dm[2][2]);
      
      mmf = multimed_r_nlay (Ex[2], mm, *Xp, *Yp, *Zp);
      X[4][0] *= mmf;	X[4][1] *= mmf;	X[5][0] *= mmf;	X[5][1] *= mmf;
      
      
      /********************************************************************/
      /* derivatives d(x'''',y'''') / d(X,Y,Z)  (Kraus) */
      
      Zx =  Ex[3].dm[0][0] * (*Xp - Ex[3].x0)
	+ Ex[3].dm[1][0] * (*Yp - Ex[3].y0)
	+ Ex[3].dm[2][0] * (*Zp - Ex[3].z0);
      Zy =  Ex[3].dm[0][1] * (*Xp - Ex[3].x0)
	+ Ex[3].dm[1][1] * (*Yp - Ex[3].y0)
	+ Ex[3].dm[2][1] * (*Zp - Ex[3].z0);
      N  =  Ex[3].dm[0][2] * (*Xp - Ex[3].x0)
	+ Ex[3].dm[1][2] * (*Yp - Ex[3].y0)
	+ Ex[3].dm[2][2] * (*Zp - Ex[3].z0);
      
      X[6][0] = ((-I[3].cc)/(N*N)) * (N*Ex[3].dm[0][0] - Zx*Ex[3].dm[0][2]);
      X[6][1] = ((-I[3].cc)/(N*N)) * (N*Ex[3].dm[1][0] - Zx*Ex[3].dm[1][2]);
      X[6][2] = ((-I[3].cc)/(N*N)) * (N*Ex[3].dm[2][0] - Zx*Ex[3].dm[2][2]);
      X[7][0] = ((-I[3].cc)/(N*N)) * (N*Ex[3].dm[0][1] - Zy*Ex[3].dm[0][2]);
      X[7][1] = ((-I[3].cc)/(N*N)) * (N*Ex[3].dm[1][1] - Zy*Ex[3].dm[1][2]);
      X[7][2] = ((-I[3].cc)/(N*N)) * (N*Ex[3].dm[2][1] - Zy*Ex[3].dm[2][2]);
      
      mmf = multimed_r_nlay (Ex[3], mm, *Xp, *Yp, *Zp);
      X[6][0] *= mmf;	X[6][1] *= mmf;	X[7][0] *= mmf;	X[7][1] *= mmf;
      
      
      /********************************************************************/
      
      img_coord (*Xp,*Yp,*Zp, Ex[0],I[0], ap[0], mm, &xp1, &yp1);
      img_coord (*Xp,*Yp,*Zp, Ex[1],I[1], ap[1], mm, &xp2, &yp2);
      img_coord (*Xp,*Yp,*Zp, Ex[2],I[2], ap[2], mm, &xp3, &yp3);
      img_coord (*Xp,*Yp,*Zp, Ex[3],I[3], ap[3], mm, &xp4, &yp4);
      
      y[0] = x1 - xp1;	y[1] = y1 - yp1;
      y[2] = x2 - xp2;	y[3] = y2 - yp2;
      y[4] = x3 - xp3;	y[5] = y3 - yp3;
      y[6] = x4 - xp4;	y[7] = y4 - yp4;
      
      
      /********************************************************************/
      /* Gaussian algorithm */
      
      ata (X, XtX, 8, 3);
      matinv (XtX, 3);
      atl (Xty, X, y, 8, 3);
      matmul (beta, XtX, Xty, 3,3,1);
      
      stopflag = 1;
      for (i=0; i<3; i++)  
	if (fabs(beta[i]) > 0.005)  stopflag = 0;
      
      *Xp += beta[0];  *Yp += beta[1];  *Zp += beta[2];	itnum++;
    }
  
  
  /************************************************************************/
  
  
  /* residuals */
  
  matmul (Xbeta, X, beta, 8, 3, 1);
  for (i=0; i<8; i++)
    {
      edach[i] = Xbeta[i] - y[i];
      omega += edach[i] * edach[i];
    }
  sigma0 = sqrt(omega /(8-3));
  for (i=0; i<3; i++)  sigmabeta[i] = sigma0 * sqrt(XtX[i][i]);
  
  
  mean_sigma0 += sigma0 * sigma0;
  rmsX += sigmabeta[0] * sigmabeta[0];
  rmsY += sigmabeta[1] * sigmabeta[1];
  rmsZ += sigmabeta[2] * sigmabeta[2];
}

void det_lsq_2 (Ex, I, ap, mm, x1, y1, x2, y2, Xp, Yp, Zp)
Exterior	Ex[2];
Interior	I[2];
ap_52		ap[2];
mm_np		mm;
double		x1, y1, x2, y2, *Xp,*Yp,*Zp;
{
  int		i, itnum, stopflag;
  double	X[4][3], XtX[3][3], y[4], Xty[3], beta[3],
    Xbeta[4], edach[4], omega=0., sigma0, sigmabeta[3];
  double	Zx, Zy, N, mmf;
  double	xp1, yp1, xp2, yp2;
  double	X1,Y1,Z1, a1,b1,c1, X2,Y2,Z2, a2,b2,c2;
  
  double	multimed_r_nlay ();
  
  
  /* approximate values (rt_is with the uncorrected image coordinates) */
  ray_tracing (x1, y1, Ex[0], I[0],
	       mm, &X1, &Y1, &Z1, &a1, &b1, &c1);
  ray_tracing (x2, y2, Ex[1],I[1],
	       mm, &X2, &Y2, &Z2, &a2, &b2, &c2);
  intersect_rt (X1,Y1,Z1,a1,b1,c1, X2,Y2,Z2,a2,b2,c2, Xp, Yp, Zp);
  /************************************************************************/
  
  itnum = 0;	stopflag = 0;
  while ((stopflag == 0) && (itnum < 10))
    {
      
      /********************************************************************/
      /* derivatives d(x',y') / d(X,Y,Z)  (Kraus) */
      
      Zx =  Ex[0].dm[0][0] * (*Xp - Ex[0].x0)
	+ Ex[0].dm[1][0] * (*Yp - Ex[0].y0)
	+ Ex[0].dm[2][0] * (*Zp - Ex[0].z0);
      Zy =  Ex[0].dm[0][1] * (*Xp - Ex[0].x0)
	+ Ex[0].dm[1][1] * (*Yp - Ex[0].y0)
	+ Ex[0].dm[2][1] * (*Zp - Ex[0].z0);
      N  =  Ex[0].dm[0][2] * (*Xp - Ex[0].x0)
	+ Ex[0].dm[1][2] * (*Yp - Ex[0].y0)
	+ Ex[0].dm[2][2] * (*Zp - Ex[0].z0);
      
      X[0][0] = ((-I[0].cc)/(N*N)) * (N*Ex[0].dm[0][0] - Zx*Ex[0].dm[0][2]);
      X[0][1] = ((-I[0].cc)/(N*N)) * (N*Ex[0].dm[1][0] - Zx*Ex[0].dm[1][2]);
      X[0][2] = ((-I[0].cc)/(N*N)) * (N*Ex[0].dm[2][0] - Zx*Ex[0].dm[2][2]);
      X[1][0] = ((-I[0].cc)/(N*N)) * (N*Ex[0].dm[0][1] - Zy*Ex[0].dm[0][2]);
      X[1][1] = ((-I[0].cc)/(N*N)) * (N*Ex[0].dm[1][1] - Zy*Ex[0].dm[1][2]);
      X[1][2] = ((-I[0].cc)/(N*N)) * (N*Ex[0].dm[2][1] - Zy*Ex[0].dm[2][2]);
      
      mmf = multimed_r_nlay (Ex[0], mm, *Xp, *Yp, *Zp);
      X[0][0] *= mmf;	X[0][1] *= mmf;	X[1][0] *= mmf;	X[1][1] *= mmf;
      
      
      /********************************************************************/
      /* derivatives d(x'',y'') / d(X,Y,Z)  (Kraus) */
      
      Zx =  Ex[1].dm[0][0] * (*Xp - Ex[1].x0)
	+ Ex[1].dm[1][0] * (*Yp - Ex[1].y0)
	+ Ex[1].dm[2][0] * (*Zp - Ex[1].z0);
      Zy =  Ex[1].dm[0][1] * (*Xp - Ex[1].x0)
	+ Ex[1].dm[1][1] * (*Yp - Ex[1].y0)
	+ Ex[1].dm[2][1] * (*Zp - Ex[1].z0);
      N  =  Ex[1].dm[0][2] * (*Xp - Ex[1].x0)
	+ Ex[1].dm[1][2] * (*Yp - Ex[1].y0)
	+ Ex[1].dm[2][2] * (*Zp - Ex[1].z0);
      
      X[2][0] = ((-I[1].cc)/(N*N)) * (N*Ex[1].dm[0][0] - Zx*Ex[1].dm[0][2]);
      X[2][1] = ((-I[1].cc)/(N*N)) * (N*Ex[1].dm[1][0] - Zx*Ex[1].dm[1][2]);
      X[2][2] = ((-I[1].cc)/(N*N)) * (N*Ex[1].dm[2][0] - Zx*Ex[1].dm[2][2]);
      X[3][0] = ((-I[1].cc)/(N*N)) * (N*Ex[1].dm[0][1] - Zy*Ex[1].dm[0][2]);
      X[3][1] = ((-I[1].cc)/(N*N)) * (N*Ex[1].dm[1][1] - Zy*Ex[1].dm[1][2]);
      X[3][2] = ((-I[1].cc)/(N*N)) * (N*Ex[1].dm[2][1] - Zy*Ex[1].dm[2][2]);
      
      mmf = multimed_r_nlay (Ex[1], mm, *Xp, *Yp, *Zp);
      X[2][0] *= mmf;	X[2][1] *= mmf;	X[3][0] *= mmf;	X[3][1] *= mmf;
      
      
      /********************************************************************/
      
      img_coord (*Xp,*Yp,*Zp, Ex[0],I[0], ap[0], mm, &xp1, &yp1);
      img_coord (*Xp,*Yp,*Zp, Ex[1],I[1], ap[1], mm, &xp2, &yp2);
      
      y[0] = x1 - xp1;	y[2] = x2 - xp2;
      y[1] = y1 - yp1;	y[3] = y2 - yp2;
      
      
      /********************************************************************/
      /* Gaussian algorithm */
      
      ata (X, XtX, 4, 3);
      matinv (XtX, 3);
      atl (Xty, X, y, 4, 3);
      matmul (beta, XtX, Xty, 3,3,1);
      
      stopflag = 1;
      for (i=0; i<3; i++)  
	if (fabs(beta[i]) > 0.005)  stopflag = 0;
      
      *Xp += beta[0];  *Yp += beta[1];  *Zp += beta[2];	itnum++;
    }
  
  
  /************************************************************************/  
  /* residuals */
  
  matmul (Xbeta, X, beta, 4, 3, 1);
  for (i=0; i<4; i++)
    {
      edach[i] = Xbeta[i] - y[i];
      omega += edach[i] * edach[i];
    }
  sigma0 = sqrt(omega /(4-3));
  for (i=0; i<3; i++)  sigmabeta[i] = sigma0 * sqrt(XtX[i][i]);
  
  
  mean_sigma0 += sigma0 * sigma0;
  rmsX += sigmabeta[0] * sigmabeta[0];
  rmsY += sigmabeta[1] * sigmabeta[1];
  rmsZ += sigmabeta[2] * sigmabeta[2];
}
                                                                                                                                                                                                        common/ptv.h                                                                                        0000644 0001131 0001131 00000000552 10273536456 012005  0                                                                                                    ustar   amns                            amns                                                                                                                                                                                                                   #include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

//#include <tcl.h>
//#include <tk.h>

#include "tiff.h"
#include "tiffio.h"

#include "typedefs.h"
#include "globals.h"


// Chris B: include header for zlib gzip compression
#include <zlib.h>

// Chris B: some defines to keep the code compiling:
#define TCL_OK 0
#define TCL_ERROR 1
                                                                                                                                                      common/ray_tracing.c                                                                                0000644 0001131 0001131 00000004155 10253026712 013457  0                                                                                                    ustar   amns                            amns                                                                                                                                                                                                                   /****************************************************************************

Routine:				ray_tracing

Author/Copyright:		Hans-Gerd Maas

Address:		       	Institute of Geodesy and Photogrammetry
			       		ETH - Hoenggerberg
			       		CH - 8093 Zurich

Creation Date:			21.4.88
	
Description:	       	traces one ray, given by image coordinates,
			       	exterior and interior orientation 
			       	through dufferent media
			       	(see Hoehle, Manual of photogrammetry)
	
Routines contained:		-

****************************************************************************/

#include "ptv.h"




void ray_tracing (x,y,Ex,I,mm,Xb2,Yb2,Zb2,a3,b3,c3)

double		x, y;
Exterior	Ex;
Interior	I;
mm_np		mm;
double		*Xb2, *Yb2, *Zb2, *a3, *b3, *c3;

/* ray-tracing, see HOEHLE and Manual of Photogrammetry */

{
	double  a1, b1, c1, a2, b2, c2, Xb1, Yb1, Zb1, d1, d2, cosi1, cosi2,
			vect1[3], vect2[3], factor, s2;

	s2 = sqrt (x*x + y*y + I.cc*I.cc);
	
	/* direction cosines in image coordinate system */
	vect1[0] = x/s2;  vect1[1] = y/s2;	vect1[2] = -I.cc/s2;

	matmul (vect2, Ex.dm, vect1, 3,3,1);
 	
	/* direction cosines in space coordinate system , medium n1 */
	a1 = vect2[0];  b1 = vect2[1];  c1 = vect2[2];  
	
       	d1 = -(Ex.z0 - mm.d[0]) / c1;

	/* point on the horizontal plane between n1,n2 */
	Xb1 = Ex.x0 + d1*a1;  Yb1 = Ex.y0 + d1*b1;  Zb1 = Ex.z0 + d1*c1;
	
	cosi1 = c1;
	factor = cosi1 * mm.n1/mm.n2[0]
		   + sqrt (1 - (mm.n1*mm.n1)/(mm.n2[0]*mm.n2[0])
		   			 + (cosi1*cosi1)*(mm.n1*mm.n1)/(mm.n2[0]*mm.n2[0]));

	/* direction cosines in space coordinate system , medium n2 */
	a2 = a1 * mm.n1/mm.n2[0];
	b2 = b1 * mm.n1/mm.n2[0];
	c2 = c1 * mm.n1/mm.n2[0] - factor;
	
	d2 = -mm.d[0]/c2;

	/* point on the horizontal plane between n2,n3 */
	*Xb2 = Xb1 + d2*a2;  *Yb2 = Yb1 + d2*b2;  *Zb2 = Zb1 + d2*c2;
	
	cosi2 = c2;
	factor = cosi2 * mm.n2[0]/mm.n3 
		   + sqrt (1 - (mm.n2[0]*mm.n2[0])/(mm.n3*mm.n3)
		   			 + (cosi2*cosi2)*(mm.n2[0]*mm.n2[0])/(mm.n3*mm.n3));

	/* direction cosines in space coordinate system , medium mm.n3 */
	*a3 = a2 * mm.n2[0]/mm.n3;
	*b3 = b2 * mm.n2[0]/mm.n3;
	*c3 = c2 * mm.n2[0]/mm.n3 - factor;
}

                                                                                                                                                                                                                                                                                                                                                                                                                   common/rotation.c                                                                                   0000644 0001131 0001131 00000002365 10253026712 013015  0                                                                                                    ustar   amns                            amns                                                                                                                                                                                                                   /****************************************************************************

Routine:	       	rotation.c

Author/Copyright:      	Hans-Gerd Maas

Address:	       	Institute of Geodesy and Photogrammetry
		       	ETH - Hoenggerberg
		       	CH - 8093 Zurich

Creation Date:	       	21.4.88
	
Description:	       	computes the rotation matrix for given three 
		       	angles omega, phi, kappa of exterior orientation
		      	(see: Kraus)
	
Routines contained:		-

****************************************************************************/

#include "ptv.h"

void rotation_matrix (Ex, dm)

Exterior  Ex;
Dmatrix   dm;

{
    dm[0][0] = cos(Ex.phi) * cos(Ex.kappa);
    dm[0][1] = (-1) * cos(Ex.phi) * sin(Ex.kappa);
    dm[0][2] = sin(Ex.phi);
    dm[1][0] = cos(Ex.omega) * sin(Ex.kappa)
             + sin(Ex.omega) * sin(Ex.phi) * cos(Ex.kappa);
    dm[1][1] = cos(Ex.omega) * cos(Ex.kappa)
             - sin(Ex.omega) * sin(Ex.phi) * sin(Ex.kappa);
    dm[1][2] = (-1) * sin(Ex.omega) * cos(Ex.phi);
    dm[2][0] = sin(Ex.omega) * sin(Ex.kappa)
             - cos(Ex.omega) * sin(Ex.phi) * cos(Ex.kappa);
    dm[2][1] = sin(Ex.omega) * cos(Ex.kappa)
             + cos(Ex.omega) * sin(Ex.phi) * sin(Ex.kappa);
    dm[2][2] = cos(Ex.omega) * cos(Ex.phi);
}

                                                                                                                                                                                                                                                                           common/tools.c                                                                                      0000644 0001131 0001131 00000032050 10303615647 012317  0                                                                                                    ustar   amns                            amns                                                                                                                                                                                                                   /******************************************************************************\
 * Amended: July 2005, Chris Bunney, University of Plymouth
 *          Implemented facility to read/write compressed data using the zlib
 *          library to save disk space.
\******************************************************************************/

#include "ptv.h"
#include <sys/stat.h>
#include <zlib.h>

/* declaration */
void qs_coord2d_x ();
void qs_target_y ();
void qs_coord2d_pnr ();
void qs_con ();
void bubble_foundpix1 ();
void bubble_foundpix2 ();
void tclimg2cimg();

/* tools / makros */

int round (double x)
/* round input double x to integer */
{
 if (x>=0)
   return((int)(x+.5));
 else
   return((int)(x-.5));
}


void write_ori (Ex, I, filename)  /* write exterior and interior orientation */
Exterior Ex;
Interior I;
char	 filename[64];
{
  FILE	*fp;
  int  	i;

  fp = fopen (filename, "w");
  fprintf (fp, "%11.4f %11.4f %11.4f\n    %10.7f  %10.7f  %10.7f\n\n",
	   Ex.x0, Ex.y0, Ex.z0, Ex.omega, Ex.phi, Ex.kappa);
  for (i=0; i<3; i++)  fprintf (fp, "    %10.7f %10.7f %10.7f\n",
				Ex.dm[i][0], Ex.dm[i][1], Ex.dm[i][2]);
  fprintf (fp,"\n    %8.4f %8.4f\n    %8.4f\n", I.xh, I.yh, I.cc);
  fclose (fp);
}


void read_ori (Ex, I, filename)	  /* read exterior and interior orientation */
Exterior *Ex;
Interior *I;
char	 filename[64];
{
  FILE	*fp;
  int  	i;

  fp = fopen_r (filename);
  fscanf (fp, "%lf %lf %lf %lf %lf %lf",
	  &(Ex->x0), &(Ex->y0), &(Ex->z0),
	  &(Ex->omega), &(Ex->phi), &(Ex->kappa));
  for (i=0; i<3; i++)  fscanf (fp, " %lf %lf %lf",
			       &(Ex->dm[i][0]), &(Ex->dm[i][1]), &(Ex->dm[i][2]));
  fscanf (fp, "%lf %lf %lf", &(I->xh), &(I->yh), &(I->cc));
  fclose (fp);
}



FILE *fopen_r (filename)
char	filename[256];
/*	tries to open a file;
	gives a message, if it cannot open it
	and waits until it has been created 	 */
{
  FILE	*fpr;
  int  	count;

  fpr = fopen (filename, "r");
  if ( ! fpr)
    {
      printf ("could not open %s, please create this file\n", filename);

      /* wait until file can be opened */
      while ( ! fpr)	fpr = fopen (filename, "r");

      /* wait until file really created */
      for (count=0; count<100000; count++);
    }

  return (fpr);
}

void read_image (/*Tcl_Interp* interp,*/ char path[128], unsigned char *img)
{
  int  i, j;
  struct stat filestat;
  char szCmpFn[128];

  sprintf( szCmpFn, "%s.gz", path );	// compressed filename
  
  // ChrisB: we are not using tiff....so disable this:
  if (tiff_flag) {
	  printf( "Oh Crap....we are in the TIFF section - but I disabled it.....\n" );
	  fprintf( stderr, "tools.c:read_image:94 - TIFF images are currently NOT supported\n" );
#if 0
    sprintf(val, "temp read %s", path);
    Tcl_Eval(interp, val);

    img_handle = Tk_FindPhoto( interp, "temp");
    Tk_PhotoGetImage (img_handle, &img_block);
    for ( j=0;j<imgsize;j++)
      {
	i=4*j;
	*(img +j) = *(img_block.pixelPtr + i);
      }
#endif
  } else {
	// ChrisB: added support for reading gzipped images on the fly:
	if( stat( path, &filestat ) == 0 ) {
		// unzipped file exists - use it:
		if( ( fp1 = fopen( path, "r" ) ) == NULL ) {
			fprintf( stderr, "Failed to open file {%s}\n", path );
			exit( 1 );
		}

		fread( img, 1, imgsize, fp1 );
		fclose( fp1 );
	} else if( stat( szCmpFn, &filestat ) == 0 ) {
		// gzipped file exists, use it:
		gzFile f;

		fprintf( stderr, "Using gzipped file for img {%s}\n", szCmpFn );

		if( ( f = gzopen( szCmpFn, "rb" ) ) == NULL ) {
			fprintf( stderr, "Failed to open file {%s}\n", szCmpFn );
			exit( 1 );
		}
		
		// read and uncompress .gz file:
		gzread( f, img, imgsize );

		gzclose( f );
	} else {
		fprintf( stderr, "Error: Image file {%s} does not exist as either a raw image or a .gz file.\n" );
		exit( 1 );
	}

#if 0
    fp1 = fopen_r (path);
    fread (img, 1, imgsize, fp1);
    fclose (fp1);
#endif
    //img_handle = Tk_FindPhoto( interp, "temp");
    //Tk_PhotoGetImage (img_handle, &img_block);
    //tclimg2cimg (interp, img, &img_block);
  }
}

int write_tiff (path, data, nx, ny)
const char   	path[256];
unsigned char *data;
int    	nx;
int    	ny;

{
  TIFF       *tif;
  unsigned char	*data1;	       			/* pixel data */
  unsigned char	*buf, *buf1, *begin, *end;	/* scanline buffer */
  int			y;

  /* open tiff file */
  tif = TIFFOpen(path, "w");

  if (tif == NULL)
    {
      fprintf (stderr, "Error opening TIFF file: %s\n", path);
      return(-1);
    }

  /* set tiff fields */
  TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, nx);
  TIFFSetField(tif, TIFFTAG_IMAGELENGTH, ny);
  TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
  TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
  TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
  TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
  TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
  TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

  /* memory for scanline buffer */
  buf = (unsigned char*) malloc (TIFFScanlineSize(tif));
  if (buf == NULL)
    {
      fprintf (stderr, "Save TIFF roi: Cannot allocate memory ");
      fprintf (stderr, "for scanline buffer.\n");
      return(-1);
    }

  /* read the required scanlines and copy the required portion */
  data1 = data;
  begin = buf;
  end = begin + nx;

  for (y = 0; y < ny; y++)
    {
      for (buf1 = begin; buf1 < end; buf1++)
	*buf1 = *data1++;
      TIFFWriteScanline (tif, buf, y, 0);
    }

  /* release scanline buffer */
  free (buf);

  /* flush data and close file */
  TIFFFlushData(tif);
  TIFFClose(tif);
  return (1);
}

/*************************************************************************/

void compose_name_plus_nr (basename, str, nr, filename)
char	basename[256], str[256], filename[256];
int		nr;
{
  char	nr_ch[256];

  if (nr < 10)		sprintf (nr_ch, "00%1d", nr);
  else if (nr < 100)	sprintf (nr_ch, "0%2d",  nr);
  else	sprintf (nr_ch, "%3d",  nr);

  sprintf (filename, "%s%s%s", basename, str, nr_ch);
}

void compose_name_plus_nr_str (basename, str, nr, filename)
char	basename[256], str[256], filename[256];
int		nr;
{
  char	nr_ch[256];

  if (nr < 10)		sprintf (nr_ch, "00%1d", nr);
  else if (nr < 100)	sprintf (nr_ch, "0%2d",  nr);
  else 	sprintf (nr_ch, "%3d",  nr);

  sprintf (filename, "%s%s%s", basename, nr_ch, str);
}

/* find nearest neighbours */

int kill_in_list (/*interp,*/ nr, num, ms_x, ms_y)
//Tcl_Interp* interp;
int    	nr, num;
int    	ms_x, ms_y;
{
  int 	i, imin = 9999, intx, inty;
  double  x, y, d, dmin = 9999;

  if (zoom_f[nr] > 1)
    {
      sprintf (buf, "cannot delete point from zoomed image");
      //Tcl_SetVar(interp, "tbuf", buf, TCL_GLOBAL_ONLY);
      //Tcl_Eval(interp, ".text delete 3");
      //Tcl_Eval(interp, ".text insert 3 $tbuf");
      return (0);
    }

  for (i=0; i<num; i++)
    {
      x = (double) ms_x - pix[nr][i].x;
      y = (double) ms_y - pix[nr][i].y;
      d = sqrt (x*x + y*y);
      if (d < dmin)
	{
	  dmin = d; imin = i;
	}
    }
  if (dmin > 10)	return (-1);	       	/*  limit: 10 pixel  */
  intx = (int) pix[nr][imin].x;
  inty = (int) pix[nr][imin].y;

  //drawcross (interp, intx, inty, cr_sz+1, nr, "magenta");

  for (i=imin; i<num; i++)  pix[nr][i] = pix[nr][i+1];

  return (imin);
}



int nearest_neighbour_geo (crd, num, x, y, eps)
coord_2d  crd[];
int       num;
double 	  x, y, eps;
{
  register int	j;
  int	       	j0, dj, pnr = -999;
  double       	d, dmin=1e20, xmin, xmax, ymin, ymax;

  xmin = x - eps;  xmax = x + eps;  ymin = y - eps;  ymax = y + eps;

  /* binarized search for start point of candidate search */
  for (j0=num/2, dj=num/4; dj>1; dj/=2)
    {
      if (crd[j0].x < xmin)  j0 += dj;
      else  j0 -= dj;
    }
  j0 -= 12;  if (j0 < 0)  j0 = 0;	       	/* due to trunc */

  for (j=j0; j<num; j++)		       	/* candidate search */
    {
      if (crd[j].x > xmax)  break;	       	/* finish search */

      if (crd[j].y > ymin  &&  crd[j].y < ymax)
	{
	  d = sqrt ((x-crd[j].x)*(x-crd[j].x) + (y-crd[j].y)*(y-crd[j].y));
	  if (d < dmin)
	    {
	      dmin = d; pnr = j;
	    }
	}
    }
  return (pnr);
}


int nearest_neighbour_pix (pix, num, x, y, eps)
target 	pix[];
int    	num;
double 	x, y, eps;
{
  register int	j;
  int	       	pnr = -999;
  double       	d, dmin=1e20, xmin, xmax, ymin, ymax;

  xmin = x - eps;  xmax = x + eps;  ymin = y - eps;  ymax = y + eps;

  for (j=0; j<num; j++)		    			/* candidate search */
    {
      if (pix[j].y>ymin && pix[j].y<ymax && pix[j].x>xmin && pix[j].x<xmax)
	{
	  d = sqrt ((x-pix[j].x)*(x-pix[j].x) + (y-pix[j].y)*(y-pix[j].y));
	  if (d < dmin)
	    {
	      dmin = d; pnr = j;
	    }
	}
    }
  return (pnr);
}


/***********************************************************************/
/***********************************************************************/

/* sorting routines */

/***********************************************************************/

/* bubble sorts */

void bubble_y (item, count)
coord_2d	*item;
int			count;
{
	int			i,j;
	coord_2d	temp;

	for (i=1; i<count; ++i)  for (j=count-1; j>=i; --j)
	{
		if (item[j-1].y > item[j].y)
		{
			temp = *(item+j-1);  *(item+j-1) = *(item+j);  *(item+j) = temp;
		}
	}
}



void bubble_conlist (item, count)
correspond	*item;
int    		count;
{
	int			i,j;
	correspond	temp;

	for (i=1; i<count; ++i)  for (j=count-1; j>=i; --j)
	{
		if (item[j-1].corr > item[j].corr)
		{
			temp = *(item+j-1);  *(item+j-1) = *(item+j);  *(item+j) = temp;
		}
	}
}

void bubble_foundpix1 ( foundpix *item)
{
  int i,j;
  foundpix temp;

  for (i=1; i<n_img; ++i)  for (j=n_img-1; j>=i; --j)
    {
      if (item[j-1].freq < item[j].freq)
	{
	  temp = *(item+j-1);  *(item+j-1) = *(item+j);  *(item+j) = temp;
	}
    }
}


/***********************************************************************/
/***********************************************************************/


/* quicksort algorithms for several issues */

/***********************************************************************/

/* quicksort of 2d coordinates in x-order */

void quicksort_coord2d_x (crd, num)
coord_2d	*crd;
int			num;
{
	qs_coord2d_x (crd, 0, num-1);
}



void qs_coord2d_x (crd, left, right)
coord_2d	*crd;
int			left, right;
{
	register int	i, j;
	double			xm;
	coord_2d		temp;

	i = left;	j = right;	xm = crd[(left+right)/2].x;

	do
	{
		while (crd[i].x < xm  &&  i<right)	i++;
		while (xm < crd[j].x  &&  j>left)	j--;

		if (i <= j)
		{
			temp = crd[i];
			crd[i] = crd[j];
			crd[j] = temp;
			i++;	j--;
		}
	}
	while (i <= j);

	if (left < j)	qs_coord2d_x (crd, left, j);
	if (i < right)	qs_coord2d_x (crd, i, right);
}


/***********************************************************************/

/* quicksort of 2d coordinates in pnr-order */

void quicksort_coord2d_pnr (crd, num)
coord_2d	*crd;
int	       	num;
{
  qs_coord2d_pnr (crd, 0, num-1);
}



void qs_coord2d_pnr (crd, left, right)
coord_2d	*crd;
int    		left, right;
{
  register int	i, j;
  double       	pnrm;
  coord_2d     	temp;

  i = left;	j = right;	pnrm = crd[(left+right)/2].pnr;

  do
    {
      while (crd[i].pnr < pnrm  &&  i<right)	i++;
      while (pnrm < crd[j].pnr  &&  j>left)	j--;

      if (i <= j)
	{
	  temp = crd[i];
	  crd[i] = crd[j];
	  crd[j] = temp;
	  i++;	j--;
	}
    }
  while (i <= j);

  if (left < j)	qs_coord2d_pnr (crd, left, j);
  if (i < right)	qs_coord2d_pnr (crd, i, right);
}




/***********************************************************************/


/* quicksort of targets in y-order */

void quicksort_target_y (pix, num)
target 	*pix;
int    	num;
{
  qs_target_y (pix, 0, num-1);
}




void qs_target_y (pix, left, right)
target 	*pix;
int    	left, right;
{
  register int	i, j;
  double			ym;
  target			temp;

  i = left;	j = right;	ym = pix[(left+right)/2].y;

  do
    {
      while (pix[i].y < ym  &&  i<right)	i++;
      while (ym < pix[j].y  &&  j>left)	j--;

      if (i <= j)
	{
	  temp = pix[i];
	  pix[i] = pix[j];
	  pix[j] = temp;
	  i++;	j--;
	}
    }
  while (i <= j);

  if (left < j)	qs_target_y (pix, left, j);
  if (i < right)	qs_target_y (pix, i, right);
}



/***********************************************************************/


/* quicksort for list of correspondences in order of match quality */
/* 4 camera version */

void quicksort_con (con, num)
n_tupel	*con;
int    	num;
{
  qs_con (con, 0, num-1);
}


void qs_con (con, left, right)
n_tupel	*con;
int    	left, right;
{
  register int	i, j;
  double       	xm;
  n_tupel      	temp;

  i = left;	j = right;	xm = con[(left+right)/2].corr;

  do
    {
      while (con[i].corr > xm  &&  i<right)	i++;
      while (xm > con[j].corr  &&  j>left)	j--;

      if (i <= j)
	{
	  temp = con[i];
	  con[i] = con[j];
	  con[j] = temp;
	  i++;	j--;
	}
    }
  while (i <= j);

  if (left < j)	qs_con (con, left, j);
  if (i < right)	qs_con (con, i, right);
}


/***********************************************************************/

#if 0
void tclimg2cimg (/*interp,*/ c_img/*, tcl_img*/)
//Tcl_Interp* interp;
unsigned char *c_img;
//Tk_PhotoImageBlock *tcl_img;
{
  int i, j;
  i=0;
  for ( j=0;j<imgsize;j++)
    {
      i=j*4;
      *(tcl_img->pixelPtr +i) = *(c_img +j);
      *(tcl_img->pixelPtr +i+1) = *(c_img +j);
      *(tcl_img->pixelPtr +i+2) = *(c_img +j);
      /* value for alpha canal 42, answer to all questions*/
      *(tcl_img->pixelPtr +i+3) = 42;
    }
}
#endif


                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        common/trafo.c                                                                                      0000644 0001131 0001131 00000005501 10253026713 012265  0                                                                                                    ustar   amns                            amns                                                                                                                                                                                                                   /****************************************************************************

Routine:	       		trafo.c

Author/Copyright:		Hans-Gerd Maas

Address:	       		Institute of Geodesy and Photogrammetry
		       	        ETH - Hoenggerberg
			       	CH - 8093 Zurich

Creation Date:			25.5.88
	
Description:			diverse transformations
	
Routines contained:		pix_crd, crd_pix, affin_trafo, affin_retour

****************************************************************************/

#include "ptv.h"



void pixel_to_metric (xp,yp, imx,imy, pix_x,pix_y, xc,yc, field)

double 	xp, yp;	       	/* input: pixel coordinates */
double 	*xc, *yc;      	/* output: metric coordinates */
int    	imx, imy;      	/* image format */
double 	pix_x, pix_y;	/* pixel size */
int    	field;

/*  transformation detection pixel coordinates -> geometric coordinates */
/*  single point  */

{
  switch (field)
    {
    case 1:  yp = 2 * yp + 1;  break;
    case 2:  yp *= 2;  break;
    }
  
  *xc = (xp - imx/2.) * pix_x;
  *yc = (imy/2. - yp) * pix_y;
}





void metric_to_pixel (xc,yc, imx,imy, pix_x,pix_y, xp,yp, field)

double 	xc, yc;	       	/* input: metric coordinates */
double 	*xp, *yp;      	/* output: pixel coordinates */
int    	imx, imy;      	/* image format */
double 	pix_x, pix_y;	/* pixel size */
int    	field;

/*  transformation detection pixel coordinates -> geometric coordinates */
/*  without read and write  */

{
  *xp = (xc/pix_x) + imx/2;
  *yp = imy/2 - (yc/pix_y);
  
  switch (field)
    {
    case 1:  *yp = (*yp-1)/2;  break;
    case 2:  *yp /= 2;  break;
    }
}


void distort_brown_affin (x, y, ap, x1, y1)

double	x, y, *x1, *y1;
ap_52	ap;

/*  transformation with Brown + affine  */
   
{
  double		r;
  
  
  r = sqrt (x*x + y*y);
  if (r != 0)
    {
      x += x * (ap.k1*r*r + ap.k2*r*r*r*r + ap.k3*r*r*r*r*r*r)
	+ ap.p1 * (r*r + 2*x*x) + 2*ap.p2*x*y;
      
      y += y * (ap.k1*r*r + ap.k2*r*r*r*r + ap.k3*r*r*r*r*r*r)
	+ ap.p2 * (r*r + 2*y*y) + 2*ap.p1*x*y;
      
      *x1 = ap.scx * x - sin(ap.she) * y;  *y1 = cos(ap.she) * y;
    }
}




void correct_brown_affin (x, y, ap, x1, y1)

double	x, y, *x1, *y1;
ap_52	ap;
/*  correct crd to geo with Brown + affine  */
   
{
  double  r, xq, yq;
	

  r = sqrt (x*x + y*y);
  if (r != 0)
    {
      xq = (x + y*sin(ap.she))/ap.scx
	- x * (ap.k1*r*r + ap.k2*r*r*r*r + ap.k3*r*r*r*r*r*r)
	- ap.p1 * (r*r + 2*x*x) - 2*ap.p2*x*y;
      yq = y/cos(ap.she)
	- y * (ap.k1*r*r + ap.k2*r*r*r*r + ap.k3*r*r*r*r*r*r)
	- ap.p2 * (r*r + 2*y*y) - 2*ap.p1*x*y;
    }
  r = sqrt (xq*xq + yq*yq);		/* one iteration */
  if (r != 0)
    {
      *x1 = (x + yq*sin(ap.she))/ap.scx
	- xq * (ap.k1*r*r + ap.k2*r*r*r*r + ap.k3*r*r*r*r*r*r)
	- ap.p1 * (r*r + 2*xq*xq) - 2*ap.p2*xq*yq;
      *y1 = y/cos(ap.she)
	- yq * (ap.k1*r*r + ap.k2*r*r*r*r + ap.k3*r*r*r*r*r*r)
	- ap.p2 * (r*r + 2*yq*yq) - 2*ap.p1*xq*yq;
    }
}



                                                                                                                                                                                               common/typedefs.h                                                                                   0000644 0001131 0001131 00000006037 10274643034 013013  0                                                                                                    ustar   amns                            amns                                                                                                                                                                                                                   #ifndef TYPEDEFS_H
#define TYPEDEFS_H

#define sqr(x) x*x
#define maxcand 32
#define maxtrack 32

#define M 5000

#ifndef  M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#define POSI 40

typedef	double	Dmatrix[3][3];	/* 3 x 3 rotation matrix */

typedef struct
{
  int pnr;
  double x, y, z;
    }
coord_3d;

typedef struct
{
  int pnr;
  double x, y;
}
coord_2d;

typedef struct
{
  double  x0, y0, z0;
  double  omega, phi, kappa;
  Dmatrix dm;
}
Exterior;

typedef struct
{
  double  dacc, dangle, dvxmax, dvxmin;
  double dvymax, dvymin, dvzmax, dvzmin;
  int  dsumg, dn, dnx, dny, add;
}
trackparameters;

typedef struct
{
  double xh, yh;
  double cc;
}
Interior;

typedef struct
{
  double k1,k2,k3,p1,p2,scx,she;
}
ap_52;

typedef struct
{
  double n1,n2,n3,d;
  int    lut;
}
mm_3p;

typedef struct
{
  int  	  nlay;
  double  n1;
  double  n2[3];
  double  d[3];
  double  n3;
  int     lut;
}
mm_np;

typedef struct
{
  int     pnr;
  double  x, y;
  int     n, nx, ny, sumg;
  int     tnr;
}
target;

typedef struct
{
  int 	pos, status;
  short	xmin, xmax, ymin, ymax;
  int   n, sumg;
  double  x, y;
  int   unr, touch[4], n_touch;	/* unified with target unr, touching ... */
}
peak;

typedef struct
{
  short	       	x,y;
  unsigned char	g;
  short	       	tnr;
}
targpix;

typedef struct
{
  int  	left, right;
  double  tol, corr;
}
conjugate;

typedef struct
{
  int  	pnr;
  double  tol, corr;
}
candidate;

typedef struct
{
  int     p[4];
  double  corr;
}
n_tupel;

typedef struct
{
  int nr;
  int p[4];
}
corres;

typedef struct
{
  int    	p1;	       	/* point number of master point */
  int    	n;	       	/* # of candidates */
  int    	p2[maxcand];	/* point numbers of candidates */
  double	corr[maxcand];	/* feature based correlation coefficient */
  double	dist[maxcand];	/* distance perpendicular to epipolar line */
}
correspond;	       	/* correspondence candidates */

typedef struct
{
  coord_3d	origin;
  int          	nr,nz;
  double       	rw;
  double       	*data;
}
mm_LUT;


typedef struct /* struct for what was found to corres */
{
 int ftnr, freq, whichcam[4];
}
foundpix;

typedef struct
{
  int multi, h[maxtrack], freq[maxtrack];
  double quali[maxtrack];
}
currlist;

typedef struct
{
  int p,n;
  double x1, y1, z1;
  int type;
}
vector;

typedef struct
{
  int z1, c[maxcand], n[maxcand];
  double quali[maxcand];
}
prevlist;

typedef struct Pstruct
{
  float x[3]; /*coordinates*/
  int prev, next; /*pointer to prev or next link*/
  int prio; /*Prority of link is used for differen levels*/
  float decis[POSI]; /*Bin for decision critera of possible links to next dataset*/
  float finaldecis; /*final decision critera by which the link was established*/
  int linkdecis[POSI]; /* pointer of possible links to next data set*/
  int inlist; /* Counter of number of possible links to next data set*/
} P;


#define MAX_FILENAME_LEN 1024
#define FILENAME_IN "res/rt_is"
#define FILENAME_OUT "res/ptv_is"
#define PARAMETERFILE "res/track.par"
#define COORDPARAFILE "res/coord.par"
#define STATUSFILE "res/track.out"

#endif // TYPDEFS_H
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 jw_ptv.tgz                                                                                          0000644 0001131 0001131 00000000000 10421530732 011540  0                                                                                                    ustar   amns                            amns                                                                                                                                                                                                                   Makefile                                                                                            0000644 0001131 0001131 00000002026 10274656271 011170  0                                                                                                    ustar   amns                            amns                                                                                                                                                                                                                   INCLUDE=-Isequence -Icommon -Itrack
LIBDIR=
LIBS=-lm -ltiff -lz
CFLAGS=-O3
BINDIR=./bin

COMMON=	common/imgcoord.o \
		common/intersect.o \
		common/lsqadj.o \
		common/multimed.o \
		common/pointpos.o \
		common/ray_tracing.o \
		common/rotation.o \
		common/tools.o \
		common/trafo.o

SEQUENCE_OBJS=	sequence/correspondences.o \
		sequence/jw_ptv.o \
		sequence/epi.o \
		sequence/image_processing.o \
		sequence/peakfitting.o \
		sequence/segmentation.o \
		sequence/sequence_main.o 

TRACK_OBJS=	track/track.o \
		track/ptv.o \
		track/jw_ptv_track.o \
		track/track_main.o \
		track/ttools.o \
		track/orientation.o \
		track/dynamic_track.o
	

.SUFFIXES: .c .o
	
all: sequence track
	
sequence: $(SEQUENCE_OBJS) $(COMMON) 
	gcc $(CFLAGS) -o $(BINDIR)/sequence $(LIBDIR) $(LIBS) $(SEQUENCE_OBJS) $(COMMON)
	
track: $(TRACK_OBJS) $(COMMON) 
	gcc $(CFLAGS) -o $(BINDIR)/track $(LIBDIR) $(LIBS) $(TRACK_OBJS) $(COMMON)



clean:
	rm -rf $(COMMON) $(TRACK_OBJS) $(SEQUENCE_OBJS)


# Implicit rules:
.c.o:
	gcc $(CFLAGS) -c $(INCLUDE) -o $@ $<

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          README.new_functionality                                                                            0000644 0001131 0001131 00000011475 10421530452 014142  0                                                                                                    ustar   amns                            amns                                                                                                                                                                                                                   =========================================================
IMPORTANT NOTICE:
=========================================================
This derivative version of the Particle Tracking Velocimetry
Software (originally developed by Armin Gruen et al. at IGP/IFU
at ETH Zurich) may only be distributed or used under the terms
of the LICENSE AGREEMENT FOR ACADEMIC USE OF THE PARTICLE TRACKING
VELOCIMETRY SOFTWARE.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE 
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION 
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS WITH THE SOFTWARE.

=========================================================
General Notes:
=========================================================
This revised version of the Particle Tracking Velocimetry 
Software is intended to aid in the processing of large 
quantities of data. However, the original version should 
still be used for setting up configuration options and testing.

Main features are (details and usage below):
-- Removal of GUI and separation of 'sequence' and 'track' to
   ease distributed processing of large datasets
-- Clean-up of build system
-- Ability to read and write compressed files
-- Provision of frame-dependent tracking parameters to optimise
   tracking in time-varying flows (e.g.	ocean flows with 
   oscillatory wave motion superimposed on a mean flow)

This revised version has been built, tested and run on GNU/Linux
systems only (so far).

These enhancements have been made for use with the submersible 3D-PTV
system developed by Dr Alex NIMMO SMITH, University of Plymouth,
United Kingdom (www.coastalprocesses.org/sptv/). 
It is hoped that you will also find these modifications
useful in your academic research. Please send feedback and enhancements
to: alex.nimmo.smith AT plymouth.ac.uk

Coding revisions (c) Chris Bunney, University of Plymouth, United Kingdom, 2005

=========================================================
Build:
=========================================================
-- libtiff-devel library is required
-- Create a new directory and change to it
-- tar -xzvf jw_ptv.tgz ./
-- make
-- New executables will be available in the "bin" directory

=========================================================
New Functionality of the Multimedia PTV Tracking Software
=========================================================


[1]	Software has had all Tcl/Tk code removed and can now be run from the
	commandline without the GUI.

[2]	The sequencing and tracking capabilities have been split into two separate
	executables that can be run independently of each other.

[3]	Both 'sequence' and 'track' now take a command line flag (-c) to read and
	write compressed input/output files in gzip format. This includes all files
	written/read from the 'res' directory and the '_target' files in the 'img'
	directory. Compressed images in the 'img' directory are also read if the
	uncompressed image does not exist - this is done implicitly and is not
	affected by the -c flag.

	Note: If you run 'sequence' with the -c flag then you must run 'track' with
	the -c flag as well!! After running the first time with the -c flag, it
	might be a good idea to purge all old uncompressed files in the img/ and
	res/ directories to ensure that they are not loaded by accident (for example
	if the -c flag was accidentally omitted).

	'track' can also take a flag (-p) to specify a different file to use for
	dynamic tracking parameters. The default is 'parameters/dynamic_track.par'.

	To use the original static tracking parameters file ('parameters/track.par')
	pass '-s' to 'track'.

	Run either 'sequence' or 'track' with the -h flag to see usage information.


	Sequence flags:
		-c Turn on gzip compression of input/output files.  
		-v Print verbose output.

	Track flags:
		-b performs backwards tracking after forward tracking.
		-B performs ONLY backward tracking.
		-c Turn on gzip compression of input/output files.
		-p Alternate location of dynamic tracking file
		-s Uses static tracking parameters contained in parameters/track.par
			(original tk_ptv file) instead of dynamic tracking.
		-v Print verbose output.


[4]	A new file now exists in the 'parameters' directory called
	'dynamic_track.par'. This file should contain average particle tracking
	parameters for each timestep (i.e. image set). Each timestep appears on a
	separate line and contains the following space separated fields:

			dvxmin dvxmax dvymin dvymax dvzmin dvzmax dangle dacc add

	There must be the same amount of lines in the dynamic_track.par file as
	there are timesteps in the sequence.

===================
                                                                                                                                                                                                   sequence/                                                                                           0000755 0001131 0001131 00000000000 10421530664 011327  5                                                                                                    ustar   amns                            amns                                                                                                                                                                                                                   sequence/correspondences.c                                                                          0000644 0001131 0001131 00000031746 10274656127 014713  0                                                                                                    ustar   amns                            amns                                                                                                                                                                                                                   /****************************************************************************

Routine:	       	correspondences.c

Author/Copyright:      	Hans-Gerd Maas

Address:	      	Institute of Geodesy and Photogrammetry
	      		ETH - Hoenggerberg
	      		CH - 8093 Zurich

Creation Date:	       	1988/89

Description:	       	establishment of correspondences for 2/3/4 cameras

****************************************************************************/

#include "ptv.h"

#define maxcand 32

/****************************************************************************/
/*--------------- 4 camera model: consistent quadruplets -------------------*/
/****************************************************************************/

void correspondences_4 ()

{
  int 	i,j,k,l,m,n,o,  i1,i2,i3;
  int   count, match0=0, match4=0, match3=0, match2=0;
  int 	p1,p2,p3,p4, p31, p41, p42;
  int  	pt1;
  int 	tim[4][nmax];
  int  	intx, inty;
  double       	xa12, xb12, ya12, yb12;
  double       	corr;
  candidate   	cand[maxcand];
  n_tupel     	*con0;
  correspond  	*list[4][4];
/* ----------------------------------------------------------------------- */

  /* allocate memory for lists of correspondences */
  for (i1=0; i1<n_img-1; i1++)	for (i2=i1+1; i2<n_img; i2++)
    list[i1][i2] = (correspond *) malloc (num[i1] * sizeof (correspond));


  con0 = (n_tupel *) malloc (4*nmax * sizeof (n_tupel));

  /* ----------------------------------------------------------------------- */

  /*  initialize ...  */
  sprintf (buf,"Establishing correspondences");

  match=0; match0=0; match2=0;

  for (i1=0; i1<n_img-1; i1++)
    for (i2=i1+1; i2<n_img; i2++)
      for (i=0; i<num[i1]; i++)
	{
	  list[i1][i2][i].p1 = 0;
	  list[i1][i2][i].n = 0;
	}

  for (i=0; i<nmax; i++)
    {
      for (j=0; j<4; j++) tim[j][i] = 0;
      for (j=0; j<4; j++) con0[i].p[j] = -1; con0[i].corr = 0;
    }


  /* ------------------------------------------------------------------ */
  /* ------------------------------------------------------------------ */

  /* matching  1 -> 2,3,4  +  2 -> 3,4  +  3 -> 4 */

  for (i1=0; i1<n_img-1; i1++)	for (i2=i1+1; i2<n_img; i2++)
    {
      if( verbose ) printf ( "Establishing correspondences  %d - %d\n", i1, i2);

      /* establish correspondences from num[i1] points of img[i1] to img[i2] */
      for (i=0; i<num[i1]; i++)	if (geo[i1][i].x != -999)
	{
	  o = epi_mm (geo[i1][i].x,geo[i1][i].y,
		      Ex[i1], I[i1], Ex[i2], I[i2], mmp,
		      &xa12, &ya12, &xb12, &yb12);

	  /* origin point in the list */
	  p1 = i;  list[i1][i2][p1].p1 = p1;	pt1 = geo[i1][p1].pnr;

	  /* search for a conjugate point in geo[i2] */
	  find_candidate_plus (geo[i2], pix[i2], num[i2],
			       xa12, ya12, xb12, yb12, eps0,
			       pix[i1][pt1].n,pix[i1][pt1].nx,pix[i1][pt1].ny,
			       pix[i1][pt1].sumg, cand, &count, i2);

	  /* write all corresponding candidates to the preliminary list */
	  /* of correspondences */
	  if (count > maxcand)	{ count = maxcand; }
	  for (j=0; j<count; j++)
	    {
	      list[i1][i2][p1].p2[j] = cand[j].pnr;
	      list[i1][i2][p1].corr[j] = cand[j].corr;
	      list[i1][i2][p1].dist[j] = cand[j].tol;
	    }
	  list[i1][i2][p1].n = count;
	}
    }

  /* repair memory fault (!?) */
  for (j=0; j<4; j++) for (i=0; i<nmax; i++) tim[j][i] = 0;


  /* ------------------------------------------------------------------ */
  /* ------------------------------------------------------------------ */

  /* search consistent quadruplets in the list */
  if (n_img == 4)
    {
      if( verbose )puts ("Search consistent quadruplets");
      for (i=0, match0=0; i<num[0]; i++)
	{
	  p1 = list[0][1][i].p1;
	  for (j=0; j<list[0][1][i].n; j++)
	    for (k=0; k<list[0][2][i].n; k++)
	      for (l=0; l<list[0][3][i].n; l++)
		{
		  p2 = list[0][1][i].p2[j];
		  p3 = list[0][2][i].p2[k];
		  p4 = list[0][3][i].p2[l];
		  for (m=0; m<list[1][2][p2].n; m++)
		    for (n=0; n<list[1][3][p2].n; n++)
		      {
			p31 = list[1][2][p2].p2[m];
			p41 = list[1][3][p2].p2[n];
			if (p3 == p31  &&  p4 == p41)
			  for (o=0; o<list[2][3][p3].n; o++)
			    {
			      p42 = list[2][3][p3].p2[o];
			      if (p4 == p42)
				{
				  corr = (list[0][1][i].corr[j]
					  + list[0][2][i].corr[k]
					  + list[0][3][i].corr[l]
					  + list[1][2][p2].corr[m]
					  + list[1][3][p2].corr[n]
					  + list[2][3][p3].corr[o])
				    / (list[0][1][i].dist[j]
				       + list[0][2][i].dist[k]
				       + list[0][3][i].dist[l]
				       + list[1][2][p2].dist[m]
				       + list[1][3][p2].dist[n]
				       + list[2][3][p3].dist[o]);
				  if (corr > corrmin)
				    {
				      /* accept as preliminary match */
				      con0[match0].p[0] = p1;
				      con0[match0].p[1] = p2;
				      con0[match0].p[2] = p3;
				      con0[match0].p[3] = p4;
				      con0[match0++].corr = corr;
				      if (match0 == 4*nmax)	/* security */
					{
					  printf ("Overflow in correspondences:");
					  printf (" > %d matches\n", match0);
					  i = num[0];
					}
				    }
				}
			    }
		      }
		}
	}


      /* -------------------------------------------------------------------- */

      /* sort quadruplets for match quality (.corr) */
      quicksort_con (con0, match0);

      /* -------------------------------------------------------------------- */

      /* take quadruplets from the top to the bottom of the sorted list */
      /* only if none of the points has already been used */
      for (i=0, match=0; i<match0; i++)
	{
	  p1 = con0[i].p[0];	if (p1 > -1)	if (++tim[0][p1] > 1)	continue;
	  p2 = con0[i].p[1];	if (p2 > -1)	if (++tim[1][p2] > 1)	continue;
	  p3 = con0[i].p[2];	if (p3 > -1)	if (++tim[2][p3] > 1)	continue;
	  p4 = con0[i].p[3];	if (p4 > -1)	if (++tim[3][p4] > 1)	continue;
	  con[match++] = con0[i];
	}

      match4 = match;
      if( verbose ) {
		  sprintf (buf, "%d consistent quadruplets, ", match4);	puts (buf);
	  }
    }

  /* ----------------------------------------------------------------------- */
  /* ----------------------------------------------------------------------- */

  /* search consistent triplets :  123, 124, 134, 234 */
  if (n_img >= 3)
    {
      if( verbose )puts ("Search consistent triplets");
      match0=0;
      for (i1=0; i1<n_img-2; i1++)
	for (i2=i1+1; i2<n_img-1; i2++)
	  for (i3=i2+1; i3<n_img; i3++)
	    for (i=0; i<num[i1]; i++)
	      {
		p1 = list[i1][i2][i].p1;
		if (p1 > nmax  ||  tim[i1][p1] > 0)	continue;

		for (j=0; j<list[i1][i2][i].n; j++)
		  for (k=0; k<list[i1][i3][i].n; k++)
		    {
		      p2 = list[i1][i2][i].p2[j];
		      if (p2 > nmax  ||  tim[i2][p2] > 0)	continue;
		      p3 = list[i1][i3][i].p2[k];
		      if (p3 > nmax  ||  tim[i3][p3] > 0)	continue;

		      for (m=0; m<list[i2][i3][p2].n; m++)
			{
			  p31 = list[i2][i3][p2].p2[m];
			  if (p3 == p31)
			    {
			      corr = (list[i1][i2][i].corr[j]
				      + list[i1][i3][i].corr[k]
				      + list[i2][i3][p2].corr[m])
				/ (list[i1][i2][i].dist[j]
				   + list[i1][i3][i].dist[k]
				   + list[i2][i3][p2].dist[m]);
 			      if (corr > corrmin)
				{ for (n=0; n<n_img; n++) con0[match0].p[n] = -2;
				  con0[match0].p[i1] = p1;
				  con0[match0].p[i2] = p2;
				  con0[match0].p[i3] = p3;
				  con0[match0++].corr = corr;
				}
			    }
			}
		    }
	      }

      /* ----------------------------------------------------------------------- */

      /* sort triplets for match quality (.corr) */
      quicksort_con (con0, match0);

      /* ----------------------------------------------------------------------- */

      /* pragmatic version: */
      /* take triplets from the top to the bottom of the sorted list */
      /* only if none of the points has already been used */
      for (i=0; i<match0; i++)
	{
	  p1 = con0[i].p[0];	if (p1 > -1)	if (++tim[0][p1] > 1)	continue;
	  p2 = con0[i].p[1];	if (p2 > -1)	if (++tim[1][p2] > 1)	continue;
	  p3 = con0[i].p[2];	if (p3 > -1)	if (++tim[2][p3] > 1)	continue;
	  p4 = con0[i].p[3];	if (p4 > -1  && n_img > 3) if (++tim[3][p4] > 1) continue;

	  con[match++] = con0[i];
	}

      match3 = match - match4;
      sprintf (buf, "%d consistent quadruplets, %d triplets ", match4, match3);
      if( verbose )puts (buf);

      /* repair artifact (?) */
      if (n_img == 3)	for (i=0; i<match; i++)	con[i].p[3] = -1;
    }

  /* ----------------------------------------------------------------------- */
  /* ----------------------------------------------------------------------- */

  /* search consistent pairs :  12, 13, 14, 23, 24, 34 */
  /* only if an object model is available or if only 2 images are used */
  if( verbose )puts ("Search pairs");


  match0 = 0;
  for (i1=0; i1<n_img-1; i1++)
    if ( n_img == 2 || (num[0] < 64 && num[1] < 64 && num[2] < 64 && num[3] < 64))
      for (i2=i1+1; i2<n_img; i2++)
	for (i=0; i<num[i1]; i++)
	  {
	    p1 = list[i1][i2][i].p1;
	    if (p1 > nmax  ||  tim[i1][p1] > 0)	continue;

	    /* take only unambigous pairs */
	    if (list[i1][i2][i].n != 1)	continue;

	    p2 = list[i1][i2][i].p2[0];
	    if (p2 > nmax  ||  tim[i2][p2] > 0)	continue;

	    corr = list[i1][i2][i].corr[0] / list[i1][i2][i].dist[0];

	    if (corr > corrmin)
	      {
		con0[match0].p[i1] = p1;
		con0[match0].p[i2] = p2;
		con0[match0++].corr = corr;
	      }
	  }

  /* ----------------------------------------------------------------------- */


  /* sort pairs for match quality (.corr) */
  quicksort_con (con0, match0);

  /* ----------------------------------------------------------------------- */


  /* take pairs from the top to the bottom of the sorted list */
  /* only if none of the points has already been used */
  for (i=0; i<match0; i++)
    {
      p1 = con0[i].p[0];	if (p1 > -1)	if (++tim[0][p1] > 1)	continue;
      p2 = con0[i].p[1];	if (p2 > -1)	if (++tim[1][p2] > 1)	continue;
      p3 = con0[i].p[2];	if (p3 > -1  && n_img > 2)
	if (++tim[2][p3] > 1)	continue;
      p4 = con0[i].p[3];	if (p4 > -1  && n_img > 3)
	if (++tim[3][p4] > 1)	continue;

      con[match++] = con0[i];
    }

  match2 = match-match4-match3;
  sprintf (buf, "%d consistent quadruplets, %d triplets and %d unambigous pairs",
	   match4, match3, match2);
  if( verbose )puts (buf);
  /* ----------------------------------------------------------------------- */

  /* give each used pix the correspondence number */
  for (i=0; i<match; i++)
    {
      for (j=0; j<n_img; j++)
	{
	  p1 = geo[j][con[i].p[j]].pnr;
	  if (p1 > -1)
	    {
	      pix[j][p1].tnr= i;
	    }
	}
    }

#if 0
  /* draw crosses on canvas */
  if (display) {
    for (i=0; i<match4; i++)	       	/* red crosses for quadruplets */
      {
	for (j=0; j<n_img; j++)
	  {
	    p1 = geo[j][con[i].p[j]].pnr;
	    if (p1 > -1)
	      {
		if (   (fabs(pix[j][p1].x-zoom_x[j]) < imx/(2*zoom_f[j]))
		       && (fabs(pix[j][p1].y-zoom_y[j]) < imy/(2*zoom_f[j])))
		  {
		    intx = (int) ( imx/2 + zoom_f[j] * (pix[j][p1].x-zoom_x[j]));
		    inty = (int) ( imy/2 + zoom_f[j] * (pix[j][p1].y-zoom_y[j]));
		    drawcross (interp, intx, inty, cr_sz, j, "red");
		    if (zoom_f[j]>=2) draw_pnr (interp, intx+5 , inty+0, i, j, "white");
		  }
	      }
	  }
      }

    for (i=match4; i<match4+match3; i++)	/* green crosses for triplets */
      {
	for (j=0; j<n_img; j++)
	  {
	    p1 = geo[j][con[i].p[j]].pnr;
	    if (p1 > -1 && con[i].p[j] > -1)
	      {
		if (   (fabs(pix[j][p1].x-zoom_x[j]) < imx/(2*zoom_f[j]))
		       && (fabs(pix[j][p1].y-zoom_y[j]) < imy/(2*zoom_f[j])))
		  {
		    intx = (int) ( imx/2 + zoom_f[j] * (pix[j][p1].x-zoom_x[j]));
		    inty = (int) ( imy/2 + zoom_f[j] * (pix[j][p1].y-zoom_y[j]));
		    drawcross ( interp, intx, inty, cr_sz, j, "green" );
		    if (zoom_f[j]>=2) draw_pnr (interp, intx+5 , inty+0, i, j, "white");/* number of triplet */
		  }
		
	      }
	  }
      }
    for (i=match4+match3; i<match4+match3+match2; i++)
      {			      	/* yellow crosses for pairs */
	for (j=0; j<n_img; j++)
	  {
	    p1 = geo[j][con[i].p[j]].pnr;
	    if (p1 > -1 && con[i].p[j] > -1)
	      {
		if (   (fabs(pix[j][p1].x-zoom_x[j]) < imx/(2*zoom_f[j]))
		       && (fabs(pix[j][p1].y-zoom_y[j]) < imy/(2*zoom_f[j])))
		  {
		    intx = (int) ( imx/2 + zoom_f[j] * (pix[j][p1].x-zoom_x[j]));
		    inty = (int) ( imy/2 + zoom_f[j] * (pix[j][p1].y-zoom_y[j]));
		    drawcross (interp, intx, inty, cr_sz, j, "yellow");
		    if (zoom_f[j]>=2) draw_pnr (interp, intx+5 , inty+0, i, j, "white"); /* number of triplet */
		  }
	      }
	  }
      }
    
    for (j=0; j<n_img; j++)
      {
	
	for (i=0; i<num[j]; i++)
	  {			      	/* blue crosses for unused detections */
	    p1 = pix[j][i].tnr;
	    if (p1 == -1 )
	      {
		if (   (fabs(pix[j][i].x-zoom_x[j]) < imx/(2*zoom_f[j]))
		       && (fabs(pix[j][i].y-zoom_y[j]) < imy/(2*zoom_f[j])))
		  {
		    intx = (int) ( imx/2 + zoom_f[j] * (pix[j][i].x-zoom_x[j]));
		    inty = (int) ( imy/2 + zoom_f[j] * (pix[j][i].y-zoom_y[j]));
		    drawcross (interp, intx, inty, cr_sz, j, "blue");
		    
		  }
	      }
	  }
      }
  }
#endif
  /* ----------------------------------------------------------------------- */
  /* free memory for lists of correspondences */
  for (i1=0; i1<n_img-1; i1++)
    {
      for (i2=i1+1; i2<n_img; i2++) free (list[i1][i2]);
    }

  free (con0);

  sprintf (buf,"Correspondences done");
}

                          sequence/epi.c                                                                                      0000644 0001131 0001131 00000027067 10252655470 012271  0                                                                                                    ustar   amns                            amns                                                                                                                                                                                                                   #include "ptv.h"

double epi_line (xl, yl, Ex1, I1, Ex2, I2)

double    xl, yl;
Exterior  Ex1, Ex2;
Interior  I1, I2;

{
  int i,j;
  double m2;
  double vect1[3], vect2[3], vect3[3], nk[3], n2[3],
    p1l[3], K2[3], k2[3], D2t[3][3];
  void crossprod ();

  /* base O1 -> O2 */
  vect1[0] = Ex2.x0 - Ex1.x0;
  vect1[1] = Ex2.y0 - Ex1.y0;
  vect1[2] = Ex2.z0 - Ex1.z0;

  /* coordinates of arbitrary point P1 in image1 */
  p1l[0] = xl;  p1l[1] = yl;	p1l[2] = - I1.cc;

  /* beam O1 -> P in space */
  matmul (vect2, Ex1.dm, p1l, 3,3,1);

  /* normale to epipolar plane */
  crossprod (vect1,vect2,nk);

  /* normale to image2 */
  vect3[0] = 0;	vect3[1] = 0;	vect3[2] = - I2.cc;

  /* normale to image 2, in space */
  matmul (n2, Ex2.dm, vect3, 3,3,1);

  /* epipolar line in image2, in space */
  crossprod (nk,n2,K2);

  /* epipolar line in image2 */
  for (i=0; i<3; i++)  for (j=0; j<3; j++)  D2t[i][j] = Ex2.dm[j][i];
  matmul (k2, D2t, K2, 3,3,1);
  m2 = k2[1] / k2[0];
  return (m2);
}



int epi_mm (x1, y1, Ex1, I1, Ex2, I2, mmp, xmin, ymin, xmax, ymax)

double     x1, y1;	  	/* input coord */
Exterior   Ex1, Ex2;           	/* orientation data */
Interior   I1, I2;	      	/* orientation data */
mm_np	   mmp;		        /* multimed param. (layers) */
double	   *xmin, *ymin, *xmax, *ymax;    /* output search window */

{
  /*  ray tracing gives the point of exit and the direction
      cosines at the waterside of the glass;
      min. and max. depth give window in object space,
      which can be transformed into _2 image
      (use img_xy_mm because of comparison with img_geo)  */

  double a, b, c, xa,ya,xb,yb;
  double X1,Y1,Z1, X, Y, Z;
  double Zmin, Zmax;

  ray_tracing (x1,y1, Ex1, I1, mmp, &X1, &Y1, &Z1, &a, &b, &c);

  /* calculate min and max depth for position (valid only for one setup) */
  Zmin = Zmin_lay[0]
    + (X1-X_lay[0]) * (Zmin_lay[1]-Zmin_lay[0]) / (X_lay[1]-X_lay[0]);
  Zmax = Zmax_lay[0]
    + (X1-X_lay[0]) * (Zmax_lay[1]-Zmax_lay[0]) / (X_lay[1]-X_lay[0]);


  Z = Zmin;   X = X1 + (Z-Z1) * a/c;   Y = Y1 + (Z-Z1) * b/c;
  img_xy_mm_geo (X,Y,Z, Ex2, I2, mmp, &xa, &ya);

  Z = Zmax;   X = X1 + (Z-Z1) * a/c;   Y = Y1 + (Z-Z1) * b/c;
  img_xy_mm_geo (X,Y,Z, Ex2, I2, mmp, &xb, &yb);

  /*  ==> window given by xa,ya,xb,yb  */

  *xmin = xa;  *ymin = ya;  *xmax = xb;  *ymax = yb;

  return (0);
}



void find_candidate (crd, pix, num, xa,ya,xb,yb,eps, n, nx, ny, sumg,
					 cand, count, nr)

/*  binarized search in a x-sorted coord-set, exploits shape information  */
/*  gives messages (in examination)  */

coord_2d	crd[];	       		/* metric coordinates */
target		pix[];		       	/* pixel data for correlation */
int	       	num, *count, nr;
double		xa, ya, xb, yb, eps;
int	       	n, nx, ny, sumg;
candidate	cand[];

{
  register int	j;
  int          	j0, dj, p2;
  double       	m, b, d, temp, qn, qnx, qny, qsumg, corr;
  double       	xmin = -4.40, xmax = 4.40, ymin = -2.94, ymax = 2.94;
  /* max. sensor format (HR 480 / Maxscan) */
  char	       	str[64], buf[32];



  /* define sensor format for search interrupt */
  xmin = (-1) * pix_x * imx/2;	xmax = pix_x * imx/2;
  ymin = (-1) * pix_y * imy/2;	ymax = pix_y * imy/2;
  xmin -= I[nr].xh;	ymin -= I[nr].yh;
  xmax -= I[nr].xh;	ymax -= I[nr].yh;
  correct_brown_affin (xmin,ymin, ap[nr], &xmin,&ymin);
  correct_brown_affin (xmax,ymax, ap[nr], &xmax,&ymax);


  if (nr != 0)
    {
      strcpy (str, "");	strcpy (buf, "");   puts (str);
    }


  for (j=0; j<8; j++)		       	/* initialize */
    {
      cand[j].pnr = -999;  cand[j].tol = -999;  cand[j].corr = -999;
    }


  m = (yb-ya)/(xb-xa);  b = ya - m*xa; 	/* line equation: y = m*x + b */


  if (xa > xb)			       	/* sort search window */
    {
      temp = xa;  xa = xb;  xb = temp;
    }
  if (ya > yb)
    {
      temp = ya;  ya = yb;  yb = temp;
    }


  if ( (xb>xmin) && (xa<xmax) && (yb>ymin) && (ya<ymax))  /* sensor area */
    {

      /* binarized search for start point of candidate search */
      for (j0=num/2, dj=num/4; dj>1; dj/=2)
	{
	  if (crd[j0].x < (xa - eps))  j0 += dj;
	  else  j0 -= dj;
	}
      j0 -= 12;  if (j0 < 0)  j0 = 0;	       	/* due to truncation */


      for (j=j0, *count=0; j<num; j++)         	/* candidate search */
	{
	  if (crd[j].x > xb+eps)  return;     	/* stop search */

	  if ((crd[j].y > ya-eps) && (crd[j].y < yb+eps))
	    {
	      if ((crd[j].x > xa-eps) && (crd[j].x < xb+eps))
		{
		  d = fabs ((crd[j].y - m*crd[j].x - b) / sqrt(m*m+1));
		  if (d < eps)
		    {
		      p2 = crd[j].pnr;
		      if (n  < pix[p2].n)      	qn  = (double) n/pix[p2].n;
		      else		       	qn  = (double) pix[p2].n/n;
		      if (nx < pix[p2].nx)	qnx = (double) nx/pix[p2].nx;
		      else		       	qnx = (double) pix[p2].nx/nx;
		      if (ny < pix[p2].ny)	qny = (double) ny/pix[p2].ny;
		      else		       	qny = (double) pix[p2].ny/ny;
		      if (sumg < pix[p2].sumg)
			qsumg = (double) sumg/pix[p2].sumg;
		      else	qsumg = (double) pix[p2].sumg/sumg;

		      /* empirical correlation coefficient
			 from shape and brightness parameters */
		      corr = (4*qsumg + 2*qn + qnx + qny);
		      /* create a tendency to prefer those matches
			 with brighter targets */
		      corr *= ((double) (sumg + pix[p2].sumg));

		      if (qn>=cn && qnx>=cnx && qny>=cny && qsumg>csumg)
			{
			  cand[*count].pnr = p2;
			  cand[*count].tol = d;
			  cand[*count].corr = corr;
			  (*count)++;
			  if (nr > 0)
			    {
			      sprintf (buf, "%3.0f/%3.1f + ", corr, d*1000);
			      strcat (str, buf);  puts (str);
			    }
			}
		    }
		}
	    }
	}
      if (*count == 0  &&  nr > 0)	puts ("- - -");
    }
  else  *count = -1;     		/* out of sensor area */
}






void find_candidate_plus (crd, pix, num, xa,ya,xb,yb, eps, n, nx, ny, sumg,
						  cand, count, nr)

/*  binarized search in a x-sorted coord-set, exploits shape information  */

coord_2d	crd[];
target		pix[];
int    		num, *count;
double		xa, ya, xb, yb, eps;
int    		n, nx, ny, sumg;
candidate	cand[];
int	       	nr;	       	/* image number for ap etc. */

{
  register int	j;
  int	       	j0, dj, p2;
  double      	m, b, d, temp, qn, qnx, qny, qsumg, corr;
  double       	xmin, xmax, ymin, ymax;

  /* define sensor format for search interrupt */
  xmin = (-1) * pix_x * imx/2;	xmax = pix_x * imx/2;
  ymin = (-1) * pix_y * imy/2;	ymax = pix_y * imy/2;
  xmin -= I[nr].xh;	ymin -= I[nr].yh;
  xmax -= I[nr].xh;	ymax -= I[nr].yh;
  correct_brown_affin (xmin,ymin, ap[nr], &xmin,&ymin);
  correct_brown_affin (xmax,ymax, ap[nr], &xmax,&ymax);


  for (j=0; j<4; j++)
    {
      cand[j].pnr = -999;  cand[j].tol = -999;  cand[j].corr = -999;
    }


  /* line equation: y = m*x + b */
  if (xa == xb)	xa += 1e-10;
  m = (yb-ya)/(xb-xa);  b = ya - m*xa;

  if (xa > xb)
    {
      temp = xa;  xa = xb;  xb = temp;
    }
  if (ya > yb)
    {
      temp = ya;  ya = yb;  yb = temp;
    }

  if ( (xb>xmin) && (xa<xmax) && (yb>ymin) && (ya<ymax))  /* sensor area */
    {
      /* binarized search for start point of candidate search */
      for (j0=num/2, dj=num/4; dj>1; dj/=2)
	{
	  if (crd[j0].x < (xa - eps))  j0 += dj;
	  else  j0 -= dj;
	}
      j0 -= 12;  if (j0 < 0)  j0 = 0;		       	/* due to trunc */

      for (j=j0, *count=0; j<num; j++)			/* candidate search */
	{
	  if (crd[j].x > xb+eps)  return;		/* finish search */

	  if ((crd[j].y > ya-eps) && (crd[j].y < yb+eps))
	    {
	      if ((crd[j].x > xa-eps) && (crd[j].x < xb+eps))
		{
		  d = fabs ((crd[j].y - m*crd[j].x - b) / sqrt(m*m+1));

		  if ( d < eps )
		    {
		      p2 = crd[j].pnr;
		      if (n  < pix[p2].n)      	qn  = (double) n/pix[p2].n;
		      else		       	qn  = (double) pix[p2].n/n;
		      if (nx < pix[p2].nx)	qnx = (double) nx/pix[p2].nx;
		      else		       	qnx = (double) pix[p2].nx/nx;
		      if (ny < pix[p2].ny)	qny = (double) ny/pix[p2].ny;
		      else		       	qny = (double) pix[p2].ny/ny;
		      if (sumg < pix[p2].sumg)
			qsumg = (double) sumg/pix[p2].sumg;
		      else	qsumg = (double) pix[p2].sumg/sumg;

		      /* empirical correlation coefficient
			 from shape and brightness parameters */
		      corr = (4*qsumg + 2*qn + qnx + qny);
		      /* create a tendency to prefer those matches
			 with brighter targets */
		      corr *= ((double) (sumg + pix[p2].sumg));

		      if (qn>=cn && qnx>=cnx && qny>=cny && qsumg>csumg)
			{
				if ( *count < maxcand) {
			  cand[*count].pnr = j;
			  cand[*count].tol = d;
			  cand[*count].corr = corr;
			  (*count)++;
		  } else { printf("in find_candidate_plus: count > maxcand\n");}
			}
		    }
		}
	    }
	}
    }

  else  *count = -1;	 	/* out of sensor area */
}




void find_candidate_plus_msg (crd, pix, num, xa,ya,xb,yb,eps, n, nx, ny, sumg,
							  cand, count, i12)

/*  binarized search in a x-sorted coord-set, exploits shape information  */
/*  gives messages (in examination)  */

coord_2d	crd[];
target		pix[];
int    		num, *count, i12;
double		xa, ya, xb, yb, eps;
int    		n, nx, ny, sumg;
/*
candidate	cand[3];
*/
candidate	cand[];

{
  register int	j;
  int	       	j0, dj, p2;
  double        m, b, d, temp, qn, qnx, qny, qsumg, corr;
  double       	xmin, xmax, ymin, ymax;

  /* define sensor format for search interrupt */
  xmin = (-1) * pix_x * imx/2;	xmax = pix_x * imx/2;
  ymin = (-1) * pix_y * imy/2;	ymax = pix_y * imy/2;
  xmin -= I[i12].xh;	ymin -= I[i12].yh;
  xmax -= I[i12].xh;	ymax -= I[i12].yh;
  correct_brown_affin (xmin,ymin, ap[i12], &xmin,&ymin);
  correct_brown_affin (xmax,ymax, ap[i12], &xmax,&ymax);

  for (j=0; j<4; j++)
    {
      cand[j].pnr = -999;  cand[j].tol = 999;
    }
  m = (yb-ya)/(xb-xa);  b = ya - m*xa;   /* line equation: y = m*x + b */

  if (xa > xb)
    {
      temp = xa;  xa = xb;  xb = temp;
    }
  if (ya > yb)
    {
      temp = ya;  ya = yb;  yb = temp;
    }

  if ( (xb>xmin) && (xa<xmax) && (yb>ymin) && (ya<ymax)) /* sensor area */
    {
      /* binarized search for start point of candidate search */
      for (j0=num/2, dj=num/4; dj>1; dj/=2)
	{
	  if (crd[j0].x < (xa - eps))  j0 += dj;
	  else  j0 -= dj;
	}
      j0 -= 12;  if (j0 < 0)  j0 = 0;  	/* due to trunc */

      for (j=j0, *count=0; j<num; j++) 	/* candidate search */
	{
	  if (crd[j].x > xb+eps)  return;      	/* finish search */

	  if ((crd[j].y > ya-eps) && (crd[j].y < yb+eps))
	    {
	      if ((crd[j].x > xa-eps) && (crd[j].x < xb+eps))
		{
		  d = fabs ((crd[j].y - m*crd[j].x - b) / sqrt(m*m+1));
		  if (d < eps)
		    {
		      p2 = crd[j].pnr;
		      if (n  < pix[p2].n)      	qn  = (double) n/pix[p2].n;
		      else		       	qn  = (double) pix[p2].n/n;
		      if (nx < pix[p2].nx)	qnx = (double) nx/pix[p2].nx;
		      else		       	qnx = (double) pix[p2].nx/nx;
		      if (ny < pix[p2].ny)	qny = (double) ny/pix[p2].ny;
		      else		       	qny = (double) pix[p2].ny/ny;
		      if (sumg < pix[p2].sumg)
			qsumg = (double) sumg/pix[p2].sumg;
		      else	qsumg = (double) pix[p2].sumg/sumg;


		      /* empirical correlation coefficient
			 from shape and brightness parameters */
		      corr = (4*qsumg + 2*qn + qnx + qny);
		      /* create a tendency to prefer those matches
			 with brighter targets */
		      corr *= ((double) (sumg + pix[p2].sumg));

		      if (qn>=cn && qnx>=cnx && qny>=cny && qsumg>csumg)
			{
			  if (*count>=maxcand)
			    { printf("More candidates than (maxcand): %d\n",*count); return; }
			  cand[*count].pnr = p2;
			  cand[*count].tol = d;
 			  cand[*count].corr = corr;
			  (*count)++;
			  printf ("%d %3.0f/%3.1f \n", p2, corr, d*1000);
			}
		    }
		}
	    }
	}
      if (*count == 0)  puts ("- - -");
    }
  else  *count = -1;		       	       /* out of sensor area */

}





void crossprod (a,b,c)

double  a[3], b[3], c[3];

{
	c[0] = a[1] * b[2]  -  a[2] * b[1];
	c[1] = a[2] * b[0]  -  a[0] * b[2];
	c[2] = a[0] * b[1]  -  a[1] * b[0];
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                         sequence/image_processing.c                                                                         0000644 0001131 0001131 00000024104 10270234003 015000  0                                                                                                    ustar   amns                            amns                                                                                                                                                                                                                   /****************************************************************************

Routine:	       	image_processing.c

Author/Copyright:      	Hans-Gerd Maas

Address:	       	Institute of Geodesy and Photogrammetry
		       	ETH - Hoenggerberg
		       	CH - 8093 Zurich

Creation Date:	       	1988
	
Description:	       	different image processing routines ...
	
Routines contained:    	filter_3:	3*3 filter, reads matrix from filter.par
		       	lowpass_3:	3*3 local average with 9 pointers, fast
		       	lowpass_n:	n*n local average,, fast
		       			computation time independent from n
		       	histogram:	computes histogram
		       	enhance:	enhances gray value spectrum to 0..255,
		      			some extreme gray values are cut off
		       	mark_img:	reads image and pixel coordinate set,
			       		marks the image in a certain (monocomp)
			       		color and writes image
				histeq:     equalization of the histogram

****************************************************************************/

#include "ptv.h"


void filter_3 (img, img_lp)

unsigned char *img, *img_lp;

{
	register unsigned char	*ptr, *ptr1, *ptr2, *ptr3,
		             	*ptr4, *ptr5, *ptr6,
	                        *ptr7, *ptr8, *ptr9;
	int	       	end;
	float	       	m[9], sum;
	short	       	buf;
	register int	i;
	FILE	       	*fp;

	
	/* read filter elements from parameter file */
	fp = fopen_r ("filter.par");
	for (i=0, sum=0; i<9; i++)
	{
		fscanf (fp, "%f", &m[i]);
		sum += m[i];
	}
	fclose (fp);  if (sum == 0) exit(1);
	
	end = imgsize - 513;
	
	ptr  = img_lp + 513;
	ptr1 = img;				ptr2 = img + 1;			ptr3 = img + 2;
	ptr4 = img + imx;		ptr5 = img + imx + 1;	ptr6 = img + imx + 2;
	ptr7 = img + 2*imx;		ptr8 = img + 2*imx + 1;	ptr9 = img + 2*imx + 2;

	for (i=513; i<end; i++)
	{
		buf = m[0] * *ptr1++  +  m[1] * *ptr2++  +  m[2] * *ptr3++
			+ m[3] * *ptr4++  +  m[4] * *ptr5++  +  m[5] * *ptr6++
			+ m[6] * *ptr7++  +  m[7] * *ptr8++  +  m[8] * *ptr9++;
		buf /= sum;    if (buf > 255)  buf = 255;    if (buf < 8)  buf = 8;
		*ptr++ = buf;
	}
}





void enhance (img)

unsigned char	*img;

{
	register unsigned char	*ptr;
	unsigned char	       	*end, gmin = 255, gmax = 0, offs;
	float		       	diff, gain;
	int		       	i, sum, histo[256];
	
	void histogram ();
	
	end = img + imgsize;

	histogram (img, histo);
	for (i=0, sum=0; (i<255)&&(sum<imx); sum+=histo[i], i++);  gmin = i;	
	for (i=255, sum=0; (i>0)&&(sum<512); sum+=histo[i], i--);  gmax = i;	
	offs = gmin;  diff = gmax - gmin;  gain = 255 / diff;
	
	for (ptr=img; ptr<end; ptr++)
	{
		if (*ptr < gmin) *ptr = gmin;  else if (*ptr > gmax) *ptr = gmax;
		*ptr = (*ptr - offs) * gain;
		if (*ptr < 8) *ptr = 8;		/* due monocomp colors */
	}
}







void histogram (img, hist)

unsigned char	*img;
int	       	*hist;

{
	int	       	i;
	unsigned char  	*end;
	register unsigned char	*ptr;

	
	for (i=0; i<256; i++)  hist[i] = 0;
	
	end = img + imgsize;
	for (ptr=img; ptr<end; ptr++)
	{
		hist[*ptr]++;
	}
}






void lowpass_3 (img, img_lp)

unsigned char *img, *img_lp;

{
	register unsigned char	*ptr,*ptr1,*ptr2,*ptr3,*ptr4,
		       		*ptr5,*ptr6,*ptr7,*ptr8,*ptr9;
	short  	       		buf;
	register int   		i;
	
	ptr  = img_lp + 513;
	ptr1 = img;
	ptr2 = img + 1;
	ptr3 = img + 2;
	ptr4 = img + imx;
	ptr5 = img + imx+1;
	ptr6 = img + imx+2;
	ptr7 = img + 2*imx;
	ptr8 = img + 2*imx+1;
	ptr9 = img + 2*imx+2;

	for (i=0; i<imgsize; i++)
	{
		buf = *ptr5++ + *ptr1++ + *ptr2++ + *ptr3++ + *ptr4++
					  + *ptr6++ + *ptr7++ + *ptr8++ + *ptr9++;
		*ptr++ = buf/9;
	}

}






void lowpass_n (n, img, img_lp)

int	       	n;
unsigned char	*img, *img_lp;

{
	register unsigned char	*ptrl, *ptrr, *ptrz;
	short  		       	*buf1, *buf2, buf, *end;
	register short	       	*ptr, *ptr1, *ptr2, *ptr3;
	int    		       	k, n2, nq;
	register int	       	i;
	
	n2 = 2*n + 1;  nq = n2 * n2;
	
	buf1 = (short *) calloc (imgsize, sizeof(short));
	if ( ! buf1)
	{
		puts ("calloc for buf1 --> error");
		exit (1);
	}
	buf2 = (short *) calloc (imx, sizeof(short));


	/* --------------  average over lines  --------------- */
	end = buf1 + imgsize;  buf = 0;
	for (ptrr = img; ptrr < img + n2; ptrr ++)  buf += *ptrr;
	*(buf1 + n) = buf;
	
	for (ptrl=img, ptr = buf1+n+1; ptr<end; ptrl++, ptr++, ptrr++)
	{
		buf += (*ptrr - *ptrl);  *ptr = buf;
	}
	
	
	
	/* -------------  average over columns  -------------- */
	end = buf2 + imx;
	for (ptr1=buf1, ptrz=img_lp+imx*n, ptr3=buf2; ptr3<end;
		 ptr1++, ptrz++, ptr3++)
	{
		for (k=0, ptr2=ptr1; k<n2; k++, ptr2+=imx)  *ptr3 += *ptr2;
		*ptrz = *ptr3/nq;
	}
	for (i=n+1, ptr1=buf1, ptrz=img_lp+imx*(n+1), ptr2=buf1+imx*n2;
		 i<imy-n; i++)
	{
		for (ptr3=buf2; ptr3<end; ptr3++, ptr1++, ptrz++, ptr2++)
		{
			*ptr3 += (*ptr2 - *ptr1);
			*ptrz = *ptr3/nq;
		}
	}


	free (buf1);
}




void unsharp_mask (n, img0, img_lp)

int	      n;
unsigned char *img0, *img_lp;

{
	register unsigned char	*imgum, *ptrl, *ptrr, *ptrz;
	int  		       	*buf1, *buf2, buf, *end;
	register int	       	*ptr, *ptr1, *ptr2, *ptr3;
	int    		       	ii, n2, nq, m;
	register int	       	i;
	
	n2 = 2*n + 1;  nq = n2 * n2;
	

	imgum = (unsigned char *) calloc (imgsize, 1);
	if ( ! imgum)
	{
		puts ("calloc for imgum --> error");  exit (1);
	}

	buf1 = (int *) calloc (imgsize, sizeof(int));
	if ( ! buf1)
	{
		puts ("calloc for buf1 --> error");  exit (1);
	}
	buf2 = (int *) calloc (imx, sizeof(int));



	/* set imgum = img0 (so there cannot be written to the original image) */
	for (ptrl=imgum, ptrr=img0; ptrl<(imgum+imgsize); ptrl++, ptrr++)
	{
	  *ptrl = *ptrr;


	}	

	/* cut off high gray values (not in general use !)
	for (ptrz=imgum; ptrz<(imgum+imgsize); ptrz++) if (*ptrz > 160) *ptrz = 160; */




	/* --------------  average over lines  --------------- */

	for (i=0; i<imy; i++)
	{
		ii = i * imx;
		/* first element */
		buf = *(imgum+ii);  *(buf1+ii) = buf * n2;
		
		/* elements 1 ... n */
		for (ptr=buf1+ii+1, ptrr=imgum+ii+2, ptrl=ptrr-1, m=3;
			 ptr<buf1+ii+n+1; ptr++, ptrl+=2, ptrr+=2, m+=2)
		{
			buf += (*ptrl + *ptrr);
			*ptr = buf * n2 / m;
		}
		
		/* elements n+1 ... imx-n */
		for (ptrl=imgum+ii, ptr=buf1+ii+n+1, ptrr=imgum+ii+n2;
			 ptrr<imgum+ii+imx; ptrl++, ptr++, ptrr++)
		{
			buf += (*ptrr - *ptrl);
			*ptr = buf;
		}
		
		/* elements imx-n ... imx */
		for (ptrl=imgum+ii+imx-n2, ptrr=ptrl+1, ptr=buf1+ii+imx-n, m=n2-2;
			 ptr<buf1+ii+imx; ptrl+=2, ptrr+=2, ptr++, m-=2)
		{
			buf -= (*ptrl + *ptrr);
			*ptr = buf * n2 / m;
		}
	}
	
	free (imgum);


	/* -------------  average over columns  -------------- */

	end = buf2 + imx;

	/* first line */
	for (ptr1=buf1, ptr2=buf2, ptrz=img_lp; ptr2<end; ptr1++, ptr2++, ptrz++)
	{
		*ptr2 = *ptr1;
		*ptrz = *ptr2/n2;
	}
	
	/* lines 1 ... n */
	for (i=1; i<n+1; i++)
	{
		ptr1 = buf1 + (2*i-1)*imx;
		ptr2 = ptr1 + imx;
		ptrz = img_lp + i*imx;
		for (ptr3=buf2; ptr3<end; ptr1++, ptr2++, ptr3++, ptrz++)
		{
			*ptr3 += (*ptr1 + *ptr2);
			*ptrz = n2 * (*ptr3) / nq / (2*i+1);
		}
	}
	
	/* lines n+1 ... imy-n-1 */
	for (i=n+1, ptr1=buf1, ptrz=img_lp+imx*(n+1), ptr2=buf1+imx*n2;
		 i<imy-n; i++)
	{
		for (ptr3=buf2; ptr3<end; ptr3++, ptr1++, ptrz++, ptr2++)
		{
			*ptr3 += (*ptr2 - *ptr1);
			*ptrz = *ptr3/nq;
		}
	}
	
	/* lines imy-n ... imy */
	for (i=n; i>0; i--)
	{
		ptr1 = buf1 + (imy-2*i-1)*imx;
		ptr2 = ptr1 + imx;
		ptrz = img_lp + (imy-i)*imx;
		for (ptr3=buf2; ptr3<end; ptr1++, ptr2++, ptr3++, ptrz++)
		{
			*ptr3 -= (*ptr1 + *ptr2);
			*ptrz = n2 * (*ptr3) / nq / (2*i+1);
		}
	}
	
	
	free (buf1);

}






void zoom (img, zoomimg, xm, ym, zf)
unsigned char	*img, *zoomimg;
int	       	xm, ym, zf;    	/* zoom at col xm, line ym, factor zf */
{
  int          	i0, j0, sx, sy, i1, i2, j1, j2;
  register int	i,j,k,l;


  sx = imx/zf;  sy = imy/zf;  i0 = ym - sy/2;  j0 = xm - sx/2;
  
  /* lines = i, cols = j */
  
  for (i=0; i<sy; i++)  for (j=0; j<sx; j++)
    {
      i1 = i0 + i;  j1 = j0 + j;  i2 = zf*i;  j2 = zf*j;
      for (k=0; k<zf; k++)  for (l=0; l<zf; l++)
	{
	  *(zoomimg + imx*(i2+k) + j2+l) = *(img + imx*i1 + j1);
	}
    }

}


void zoom_new (img, zoomimg, xm, ym, zf, zimx, zimy)
unsigned char	*img, *zoomimg;
int	       	xm, ym, zf;    	/* zoom at col xm, line ym, factor zf */
int	       	zimx, zimy;    	/* zoom image size */
{
  int		      	xa, ya;
  register int	       	i;
  register unsigned char	*ptri, *ptrz;
  unsigned char	       	*end;
  
  xa = xm - zimx/(2*zf);
  ya = ym - zimy/(2*zf);
  ptri = img + ya*imx + xa;
  end = zoomimg + zimx*zimy;
  
  for (ptrz=zoomimg, i=0; ptrz<end; ptrz++)
    {
      *ptrz = *ptri;
      i++;
      if ((i%zimx) == 0)	ptri = img + (ya+(i/(zimx*zf)))*imx + xa;
      if ((i%zf) == 0)	ptri++;
    }
}

	
void split (img, field)

unsigned char	*img;
int	       	field;


{
  register int   		i, j;
  register unsigned char	*ptr;
  unsigned char	       	*end;
  
  switch (field)
    {
    case 0:  /* frames */
      return;	 break;
      
    case 1:  /* odd lines */
      for (i=0; i<imy/2; i++)  for (j=0; j<imx; j++)
	*(img + imx*i + j) = *(img + 2*imx*i + j + imx);  break;
      
    case 2:  /* even lines */
      for (i=0; i<imy/2; i++)  for (j=0; j<imx; j++)
	*(img + imx*i + j) = *(img + 2*imx*i + j);  break;
    }
  
  end = img + imgsize;
  for (ptr=img+imgsize/2; ptr<end; ptr++)  *ptr = 2;
}


void copy_images (img1, img2)
     unsigned char	*img1, *img2;
{
  register unsigned char 	*ptr1, *ptr2;
  unsigned char	       	*end;
  
  for (end=img1+imgsize, ptr1=img1, ptr2=img2; ptr1<end; ptr1++, ptr2++)
    *ptr2 = *ptr1;
}


void histeq (img, hist)
/* Actually this is histogram matching algorithm, and not equalization */

     unsigned char	*img;
int	       	*hist;

{
  int	       	i,j,tmpsum;
  register unsigned char	*ptr;
  int histogr[256]; 
  int cumhist1[256]={0}; 
  int cumhist2[256]={0}; 
  int map[256];
  
  histogram(img,histogr);
  
  cumhist1[0] = hist[0]; 
  cumhist2[0] = histogr[0];
  
  /* Compute cumulative histograms */
  for (i=1;i<256;i++)
    {
      cumhist1[i] = cumhist1[i-1] + hist[i];
      cumhist2[i] = cumhist2[i-1] + histogr[i];
    }
  
  for (i=1;i<256;i++)
    {
      tmpsum = 0;
      for (j=1;j<256;j++)
	{
	  if (cumhist1[j] >= cumhist2[i]) tmpsum++; 
	}
      map[i] = 256 - tmpsum;
    }
  
  for (ptr=img, i=0; i<imgsize;ptr++,i++)
    {
      *ptr = map[*ptr];
    }
  
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                            sequence/jw_ptv.c                                                                                   0000644 0001131 0001131 00000122604 10303615353 013007  0                                                                                                    ustar   amns                            amns                                                                                                                                                                                                                   /****************************************************************************
*****************************************************************************

Author/Copyright:      	Hans-Gerd Maas / Jochen Willneff

Address:	      	Institute of Geodesy and Photogrammetry
		       	ETH - Hoenggerberg
		       	CH - 8093 Zurich

Creation Date:	       	took a longer time ...

Description:	       	target detection, correspondences and
		       	positioning with tclTk display
		       	-> 4 camera version

Routines contained:    	many ...

Amended:                June 2005, Chris Bunney, University of Plymouth.
                        Removed all Tcl/Tk code.
                        Also, files can now be read/written
						in compressed form using zlib.

****************************************************************************/
#include "ptv.h"

#define nmax 5120
#define TCL_OK 0
#define TCL_ERROR 1

/*  global declarations for ptv  */
/*-------------------------------------------------------------------------*/

int	n_img;	       		      	/* no of images */
int	hp_flag=0;           	      	/* flag for highpass */
int	tiff_flag=0;           	      	/* flag for tiff header */
int oddevenflag=0;              /* flag for oddeven */
int	chfield;       		       	/* flag for field mode */
int	nfix;	       	       	       	/* no. of control points */
int	num[4];	       		       	/* no. of targets per image */
int numc[4];                        /* no. of targets in current image */
int nump[4];                        /* no. of targets in previous image */
int numn[4];                        /* no. of targets in next image */
int n_trac[4];	           	/* no. of tracks */
int	match=0;		      	/* no. of matches */
int	match2=0;	      	       	/* no. of matches in 2nd pass */
int	nr[4][4];		     	/* point numbers for man. ori */
int	imx, imy, imgsize;	      	/* image size */
int	zoom_x[4],zoom_y[4],zoom_f[4];  /* zoom parameters */
int	pp1=0, pp2=0, pp3=0, pp4=0;   	/* for man. orientation */
int	seq_first, seq_last;	       	/* 1. and last img of seq */
int	demo_nr;		      	/* for demo purposes */
int	examine = 0;		       	/* for more detailed output */
int	dump_for_rdb;		       	/* # of dumpfiles for rdb */
int cr_sz;                          /* size of crosses */
int display;                        /* display flag */
int corp, corc, corn;
int m[4];
int trackallocflag = 0;      /* checkflag if mega, c4, t4 already allocated */

double	pix_x, pix_y;			      	/* pixel size */
double	ro;			      	        /* 200/pi */
double	cn, cnx, cny, csumg, eps0, corrmin;	/* correspondences par */
double 	rmsX, rmsY, rmsZ, mean_sigma0;		/* a priori rms */
double  X_lay[2], Zmin_lay[2], Zmax_lay[2];	/* illu. layer data */

FILE	*fp1, *fp2, *fp3, *fp4, *fpp;
gzFile	fgz;

char	img_name[4][256];      	/* original image names */
char   	img_lp_name[4][256]; 	/* lowpass image names */
char   	img_hp_name[4][256];   	/* highpass image names */
char   	img_cal[4][128];       	/* calibrayion image names */
char   	img_ori[4][128];       	/* image orientation data */
char   	img_ori0[4][128];      	/* orientation approx. values */
char   	img_addpar[4][128];    	/* image additional parameters */
char   	img_addpar0[4][128];   	/* ap approx. values */
char   	seq_name[4][128];      	/* sequence names */
char   	track_dir[128];	       	/* directory with dap track data */
char    fixp_name[128];
char   	res_name[128];	      	/* result destination */
char   	filename[128];	      	/* for general use */
char   	buf[256], val[256];	       	/* buffer */

unsigned char	*img[4];      	/* image data */
unsigned char	*img0[4];      	/* image data for filtering etc */
unsigned char	*zoomimg;     	/* zoom image data */

Exterior       	Ex[4];	      	/* exterior orientation */
Interior       	I[4];	       	/* interior orientation */
ap_52	       	ap[4];	       	/* add. parameters k1,k2,k3,p1,p2,scx,she */
mm_np	       	mmp;	       	/* n-media parameters */
target	       	pix[4][nmax]; 	/* target pixel data */
target	       	pix0[4][4];    	/* pixel data for man_ori points */

target          *t4[4][4];
int             nt4[4][4];

coord_2d       	crd[4][nmax];  	/* (distorted) metric coordinates */
coord_2d       	geo[4][nmax];  	/* corrected metric coordinates */
coord_3d       	fix[4096];     	/* testfield points coordinates */
n_tupel	       	con[nmax];     	/* list of correspondences */

corres	       	*c4[4];
trackparameters tpar;           /* tracking parameters */

mm_LUT	       	mmLUT[4];     	/* LUT for multimedia radial displacement */
coord_3d        *p_c3d;
P *mega[4];

int				useCompression;	/* flag for using gz compression on output files */
int				verbose;

/***************************************************************************/

#if 1
int init_proc_c()
{
  int  i;
  const char *valp;


  if( verbose ) puts ("\nMultimedia Particle Positioning and Tracking\n");

  // ChrisB: Do we need this?
  //valp = Tcl_GetVar(interp, "examine",  TCL_GLOBAL_ONLY);
  //examine = atoi (valp);

  ro = 200/M_PI;

  /*  read from main parameter file  */
  fpp = fopen_r ("parameters/ptv.par");

  fscanf (fpp, "%d\n", &n_img);

  for (i=0; i<4; i++)
    {
      fscanf (fpp, "%s\n", img_name[i]);
      fscanf (fpp, "%s\n", img_cal[i]);
    }
  fscanf (fpp, "%d\n", &oddevenflag);
  fscanf (fpp, "%d\n", &hp_flag);
  fscanf (fpp, "%d\n", &tiff_flag);
  fscanf (fpp, "%d\n", &imx);
  fscanf (fpp, "%d\n", &imy);
  fscanf (fpp, "%lf\n", &pix_x);
  fscanf (fpp, "%lf\n", &pix_y);
  fscanf (fpp, "%d\n", &chfield);
  fscanf (fpp, "%lf\n", &mmp.n1);
  fscanf (fpp, "%lf\n", &mmp.n2[0]);
  fscanf (fpp, "%lf\n", &mmp.n3);
  fscanf (fpp, "%lf\n", &mmp.d[0]);
  fclose (fpp);

  /* read illuminated layer data */
  fpp = fopen_r ("parameters/criteria.par");
  fscanf (fpp, "%lf\n", &X_lay[0]);
  fscanf (fpp, "%lf\n", &Zmin_lay[0]);
  fscanf (fpp, "%lf\n", &Zmax_lay[0]);
  fscanf (fpp, "%lf\n", &X_lay[1]);
  fscanf (fpp, "%lf\n", &Zmin_lay[1]);
  fscanf (fpp, "%lf\n", &Zmax_lay[1]);
  fscanf (fpp, "%lf", &cnx);
  fscanf (fpp, "%lf", &cny);
  fscanf (fpp, "%lf", &cn);
  fscanf (fpp, "%lf", &csumg);
  fscanf (fpp, "%lf", &corrmin);
  fscanf (fpp, "%lf", &eps0);
  fclose (fpp);

  mmp.nlay = 1;

  /* read sequence parameters (needed for some demos) */

  fpp = fopen_r ("parameters/sequence.par");

  for (i=0; i<4; i++)		fscanf (fpp, "%s\n", seq_name[i]);
  fscanf (fpp,"%d\n", &seq_first);
  fscanf (fpp,"%d\n", &seq_last);
  fclose (fpp);

  /* initialize zoom parameters and image positions */
  for (i=0; i<n_img; i++)
    {
      num[i] = 0;
      zoom_x[i] = imx/2; zoom_y[i] = imy/2; zoom_f[i] = 1;
    }
  imgsize = imx*imy;

  /* allocate memory for images */
  for (i=0; i<n_img; i++)
    {
      img[i] = (unsigned char *) calloc (imgsize, 1);
      if ( ! img[i])
	{
	  printf ("calloc for img%d --> error\n", i);
	  exit (1);
	}
    }

  for (i=0; i<n_img; i++)
    {
      img0[i] = (unsigned char *) calloc (imgsize, 1);
      if ( ! img0[i])
	{
	  printf ("calloc for img0%d --> error\n", i);
	  exit (1);
	}
    }

  zoomimg = (unsigned char *) calloc (imgsize, 1);
  if ( ! zoomimg)
    {
      printf ("calloc for zoomimg --> error\n");
      return TCL_ERROR;
    }

  //parameter_panel_init(/*interp*/);
  // ChrisB: size of crosses - not important.
  cr_sz = 4;//atoi(Tcl_GetVar2(interp, "mp", "pcrossize",  TCL_GLOBAL_ONLY));

  display = 1;
  return TCL_OK;

}
#endif


int start_proc_c()
{
  int  i, k;

  /*  read from main parameter file  */
  fpp = fopen_r ("parameters/ptv.par");

  fscanf (fpp, "%d\n", &n_img);

  for (i=0; i<4; i++)
    {
      fscanf (fpp, "%s\n", img_name[i]);
      fscanf (fpp, "%s\n", img_cal[i]);
    }
  fscanf (fpp, "%d\n", &oddevenflag);
  fscanf (fpp, "%d\n", &hp_flag);
  fscanf (fpp, "%d\n", &tiff_flag);
  fscanf (fpp, "%d\n", &imx);
  fscanf (fpp, "%d\n", &imy);
  fscanf (fpp, "%lf\n", &pix_x);
  fscanf (fpp, "%lf\n", &pix_y);
  fscanf (fpp, "%d\n", &chfield);
  fscanf (fpp, "%lf\n", &mmp.n1);
  fscanf (fpp, "%lf\n", &mmp.n2[0]);
  fscanf (fpp, "%lf\n", &mmp.n3);
  fscanf (fpp, "%lf\n", &mmp.d[0]);
  fclose (fpp);

  /* read illuminated layer data */
  fpp = fopen_r ("parameters/criteria.par");
  fscanf (fpp, "%lf\n", &X_lay[0]);
  fscanf (fpp, "%lf\n", &Zmin_lay[0]);
  fscanf (fpp, "%lf\n", &Zmax_lay[0]);
  fscanf (fpp, "%lf\n", &X_lay[1]);
  fscanf (fpp, "%lf\n", &Zmin_lay[1]);
  fscanf (fpp, "%lf\n", &Zmax_lay[1]);
  fscanf (fpp, "%lf", &cnx);
  fscanf (fpp, "%lf", &cny);
  fscanf (fpp, "%lf", &cn);
  fscanf (fpp, "%lf", &csumg);
  fscanf (fpp, "%lf", &corrmin);
  fscanf (fpp, "%lf", &eps0);
  fclose (fpp);

  mmp.nlay = 1;

  /* read sequence parameters (needed for some demos) */

  fpp = fopen_r ("parameters/sequence.par");

  for (i=0; i<4; i++)		fscanf (fpp, "%s\n", seq_name[i]);
  fscanf (fpp,"%d\n", &seq_first);
  fscanf (fpp,"%d\n", &seq_last);
  fclose (fpp);

  /*  create file names  */
  for (i=0; i<n_img; i++)
    {
      strcpy (img_lp_name[i], img_name[i]); strcat (img_lp_name[i], "_lp");
      strcpy (img_hp_name[i], img_name[i]); strcat (img_hp_name[i], "_hp");
      strcpy (img_ori[i], img_cal[i]);  strcat (img_ori[i], ".ori");
      strcpy (img_addpar[i], img_cal[i]); strcat (img_addpar[i],".addpar");
    }

  /*  read orientation and additional parameters  */
  for (i=0; i<n_img; i++)
    {
      read_ori (&Ex[i], &I[i], img_ori[i]);
      rotation_matrix (Ex[i], Ex[i].dm);

      fp1 = fopen_r (img_addpar[i]);
      fscanf (fp1,"%lf %lf %lf %lf %lf %lf %lf",
	      &ap[i].k1, &ap[i].k2, &ap[i].k3, &ap[i].p1, &ap[i].p2,
	      &ap[i].scx, &ap[i].she);
      fclose (fp1);
    }

  /* read and display original images */
  for (i=0; i<n_img; i++)
    {
      /* reading */
      //sprintf(val, "camcanvas %d", i+1);
      //Tcl_Eval(interp, val);

      read_image (/*interp,*/ img_name[i], img[i]);
      //sprintf(val, "newimage %d", i+1);

      //Tcl_Eval(interp, val);
      //sprintf(val, "keepori %d", i+1);
      //Tcl_Eval(interp, val);
    }

  if (!trackallocflag)
    {
      for (i=0; i<4; i++)
	{
	  mega[i]=(P *) calloc(sizeof(P),M);
	  c4[i]=(corres *) calloc(sizeof(corres),M);
	  for (k=0; k<4; k++) { t4[i][k]=(target *) calloc(sizeof (target),M);}
	}
      trackallocflag=1;
    }

  return TCL_OK;

}

/* ********************************* */


int oddeven_c ()
{
  int i_img, sup=12;

  //Tk_PhotoHandle img_handle;
  //Tk_PhotoImageBlock img_block;

  if( verbose ) printf( "Odd/even");
  //Tcl_SetVar(interp, "tbuf", val, TCL_GLOBAL_ONLY);
  //Tcl_Eval(interp, ".text delete 2");
  //Tcl_Eval(interp, ".text insert 2 $tbuf");

  for (i_img=0; i_img<n_img; i_img++)
    {
      oddeven (img_name[i_img], img[i_img], img[i_img], sup, 0, chfield, i_img);

      if (display) {
#if 0
      img_handle = Tk_FindPhoto( interp, "temp");
      Tk_PhotoGetImage (img_handle, &img_block);
      tclimg2cimg (interp, img[i_img], &img_block);

      sprintf(val, "newimage %d", i_img+1);
      Tcl_GlobalEval(interp, val);
#endif
      }
    }

  if(verbose) printf( "...done\n");
  //Tcl_SetVar(interp, "tbuf", val, TCL_GLOBAL_ONLY);
  //Tcl_Eval(interp, ".text delete 3");
  //Tcl_Eval(interp, ".text insert 3 $tbuf");

  return TCL_OK;

}


/* ********************************* */


int pre_processing_c ()
{
  int i_img, sup;

  //Tk_PhotoHandle img_handle;
  //Tk_PhotoImageBlock img_block;

  if( verbose ) printf( "Filtering with Highpass");
  //Tcl_SetVar(interp, "tbuf", val, TCL_GLOBAL_ONLY);
  //Tcl_Eval(interp, ".text delete 2");
  //Tcl_Eval(interp, ".text insert 2 $tbuf");

  /* read support of unsharp mask */
  fpp = fopen ("parameters/unsharp_mask.par", "r");
  if ( fpp == 0) { sup = 12;}
  else	{ fscanf (fpp, "%d\n", &sup); fclose (fpp); }

  for (i_img=0; i_img<n_img; i_img++)
    {
      highpass (img_name[i_img], img[i_img], img[i_img], sup, 0, chfield, i_img);

      if (display) {
#if 0
      img_handle = Tk_FindPhoto( interp, "temp");
      Tk_PhotoGetImage (img_handle, &img_block);
      tclimg2cimg (interp, img[i_img], &img_block);

      sprintf(val, "newimage %d", i_img+1);
      Tcl_GlobalEval(interp, val);
#endif
      }
    }

  if(verbose)printf( "...done\n");
  //Tcl_SetVar(interp, "tbuf", val, TCL_GLOBAL_ONLY);
  //Tcl_Eval(interp, ".text delete 3");
  //Tcl_Eval(interp, ".text insert 3 $tbuf");

  return TCL_OK;

}


int detection_proc_c()
{
  int	       	i, i_img;
  int	       	xmin, pft_version=3;
  char val[256];

  /* process info */
  sprintf(val, "Detection of Particles");
  strcpy(val, "");

  /* xmin set to 10 so v_line is not included in detection, in future xmin should
     be set to 0, peakfitting has to be changed too */
  xmin=0;

  /*  read pft version  */
  fpp = fopen ("parameters/pft_version", "r");
  if (fpp)
    {
      fscanf (fpp, "%d\n", &pft_version);
      fclose (fpp);
    }


  /* reset zoom values */
  for (i_img=0; i_img<n_img; i_img++)
    {
      zoom_x[i_img] = imx/2; zoom_y[i_img] = imy/2;  zoom_f[i_img] = 1;
    }

  /*copy images because the target recognition will set greyvalues to 0*/
  for (i_img=0; i_img<n_img; i_img++)
    {
      copy_images (img[i_img], img0[i_img]);
    }

  /* target recognition */
  for (i_img=0; i_img<n_img; i_img++)
    {
      switch (pft_version)
	{
	case 3:	/* pft with profile and distance check */
	  /* newest version */
	  xmin=0; /* vertical line restriction */
	  num[i_img] = peak_fit_new (/*interp,*/ img[i_img],
				     "parameters/targ_rec.par",
				     xmin, imx, 1, imy, pix[i_img], i_img);
	  break;

	case 0:	/* without peak fitting technique */
	  simple_connectivity (/*interp,*/ img[i_img], img0[i_img],
			       "parameters/targ_rec.par",
			       xmin, imx, 1, imy, pix[i_img], i_img, &num[i_img]);
	  break;

	case 1:	/* with old (but fast) peak fitting technique */
	  targ_rec (/*interp,*/ img[i_img], img0[i_img],
		    "parameters/targ_rec.par",
		    xmin, imx, 1, imy, pix[i_img], i_img, &num[i_img]);
	  break;
	}

      sprintf (buf,"%d: %d,  ", i_img+1, num[i_img]);
      strcat(val, buf);

      /* proper sort of targets in y-direction for later binary search */
      /* and for dimitris' tracking */
      quicksort_target_y (pix[i_img], num[i_img]);

      /* reorganize target numbers */
      for (i=0; i<num[i_img]; i++)  pix[i_img][i].pnr = i;
    }

  sprintf (buf, "Number of detected particles per image");
  if( verbose ) printf("%s\n", val);
  return TCL_OK;
}



int correspondences_proc_c()
{
  int	i, i_img;
  double x,y;

  if( verbose )puts ("\nTransformation to metric coordinates\n");

  /* rearrange point numbers after manual deletion of points */
  for (i_img=0; i_img<n_img; i_img++)
    for (i=0; i<num[i_img]; i++)  pix[i_img][i].pnr = i;
  /* transformations pixel coordinates -> metric coordinates */
  /* transformations metric coordinates -> corrected metric coordinates */
  for (i_img=0; i_img<n_img; i_img++)
    {
      for (i=0; i<num[i_img]; i++)
	{
	  pixel_to_metric (pix[i_img][i].x, pix[i_img][i].y,
			   imx,imy, pix_x, pix_y,
			   &crd[i_img][i].x, &crd[i_img][i].y, chfield);
	  crd[i_img][i].pnr = pix[i_img][i].pnr;

	  x = crd[i_img][i].x - I[i_img].xh;
	  y = crd[i_img][i].y - I[i_img].yh;
	  correct_brown_affin (x, y, ap[i_img], &geo[i_img][i].x, &geo[i_img][i].y);

	  geo[i_img][i].pnr = crd[i_img][i].pnr;
	}
    }

  /* sort coordinates for binary search in correspondences_proc */
  for (i_img=0; i_img<n_img; i_img++)
    {
      quicksort_coord2d_x (geo[i_img], num[i_img]);
    }

  /* read illuminated layer data for demo version */
  fpp = fopen_r ("parameters/criteria.par");
  fscanf (fpp, "%lf\n", &X_lay[0]);
  fscanf (fpp, "%lf\n", &Zmin_lay[0]);
  fscanf (fpp, "%lf\n", &Zmax_lay[0]);
  fscanf (fpp, "%lf\n", &X_lay[1]);
  fscanf (fpp, "%lf\n", &Zmin_lay[1]);
  fscanf (fpp, "%lf\n", &Zmax_lay[1]);
  /* read criteria for accepted match (shape, tolerance), globals */
  fscanf (fpp, "%lf", &cnx);
  fscanf (fpp, "%lf", &cny);
  fscanf (fpp, "%lf", &cn);
  fscanf (fpp, "%lf", &csumg);
  fscanf (fpp, "%lf", &corrmin);
  fscanf (fpp, "%lf", &eps0);
  fclose (fpp);
  /* init multimedia radial displacement LUTs */
  /* ======================================== */

  if ( !mmp.lut && (mmp.n1 != 1 || mmp.n2[0] != 1 || mmp.n3 != 1))
    {
      if( verbose )puts ("Init multimedia displacement LUTs");
      for (i_img=0; i_img<n_img; i_img++) init_mmLUT(i_img);
      mmp.lut = 1;
    }

  correspondences_4 (/* interp*/);

  /* --------------- */
  /* save pixel coords for tracking */
  for (i_img=0; i_img<n_img; i_img++)
    {
		if( useCompression ) {
			// ChrisB: write compressed output:
			
			sprintf (filename, "%s_targets.gz", img_name[i_img]);
			fgz = gzopen (filename, "wb");

			gzprintf(fgz,"%d\n", num[i_img]);

	  
			for (i=0; i<num[i_img]; i++)
			{
				gzprintf (fgz, "%4d %9.4f %9.4f %5d %5d %5d %5d %5d\n", pix[i_img][i].pnr, pix[i_img][i].x,
						pix[i_img][i].y, pix[i_img][i].n ,
						pix[i_img][i].nx ,pix[i_img][i].ny,
						pix[i_img][i].sumg, pix[i_img][i].tnr);
			}
			gzclose( fgz );
		} else {
			// ChrisB: write uncompressed output:
			
			sprintf (filename, "%s_targets", img_name[i_img]);
			fp1 = fopen (filename, "w");
			fprintf(fp1,"%d\n", num[i_img]);

			for (i=0; i<num[i_img]; i++) {
				fprintf (fp1, "%4d %9.4f %9.4f %5d %5d %5d %5d %5d\n", pix[i_img][i].pnr, pix[i_img][i].x,
				pix[i_img][i].y, pix[i_img][i].n ,
					pix[i_img][i].nx ,pix[i_img][i].ny,
					pix[i_img][i].sumg, pix[i_img][i].tnr);
			}
			fclose (fp1);
		}
	}  
  return TCL_OK;
}


int determination_proc_c ()
{
  int  	i, j, n;
  int  	p[4];
  double  x[4], y[4], X,Y,Z;
  double  Zlo = 1e20, Zhi = -1e20;
  gzFile fileGz;

  if( verbose ) puts ("Determinate");

  sprintf (buf, "Point positioning (l.sq.)");

  if( useCompression ) {
	  
	  sprintf( filename, "%s.gz", res_name );
	  fileGz = gzopen( filename, "wb" );
	  
	  if ( ! fileGz)
	  {
		  sprintf(res_name,"res/dt_lsq.gz");
		  fileGz = gzopen (res_name, "wb");
	  }
	  if ( ! fileGz)
	  {
		  printf ("cannot find dir: res,  data written to dt_lsq in same dir\n");
		  sprintf (res_name, "dt_lsq.gz");
		  fileGz = gzopen (res_name, "wb");
	  }
  } else {
	  fp1 = fopen (res_name, "w");

	  if ( ! fp1)
	  {
		  sprintf(res_name,"res/dt_lsq");
		  fp1 = fopen (res_name, "w");
	  }
	  if ( ! fp1)
	  {
		  printf ("cannot find dir: res,  data written to dt_lsq in same dir\n");
		  sprintf (res_name, "dt_lsq");
		  fp1 = fopen (res_name, "w");
	  }
  }
  /* create dump file for rdb */
  if (examine == 4)
    {
      /* create filename for dumped dataset */
      sprintf (res_name, "dump_for_rdb");
      printf ("dataset dumped into %s\n", res_name);
      fp2 = fopen (res_name, "w");

      /* write # of points to file */
	  fprintf( stderr, "{%s} {%d} Writing to fp2 {%s}/n", __FILE__, __LINE__,res_name );
      fprintf (fp2, "%d\n", match);
    }
  /* first line to be updated in res_name file */
  if( useCompression )
	  gzprintf (fileGz, "%4d\n", match);
  else 
	  fprintf (fp1, "%4d\n", match);

  /* least squares determination for triplets */

  rmsX = 0; rmsY = 0; rmsZ = 0;	mean_sigma0 = 0;

  for (i=0; i<match; i++)
    {
      for (j=0; j<4; j++)
	if (con[i].p[j] >= 0)	p[j] = geo[j][con[i].p[j]].pnr;
	else		       	p[j] = -1;

      for (j=0, n=0; j<4; j++)
	{
	  if (p[j] > -1)
	    {
	      x[j] = crd[j][p[j]].x;	y[j] = crd[j][p[j]].y;
	      n++;
	    }
	  else
	    {
	      x[j] = -1e10;	y[j] = -1e10;
	      if (p[j] == -2)	n = -100;
	    }
	}

      /* take only points which are matched in all images */
      /* or triplets/quadruplets which result from object model */
      /* e.g.: quad -> n=4; model triplet -> n=3; model pair -> n=2;
	 unrestricted triplet -> n<0; unrestricted pair -> n<0 */
      /*     if (n_img > 2  &&  n < 3)	continue; */

      /* ################################# */
      /* take only points which are matched in all images */
      /* or triplets/quadruplets which result from object model */
      /* e.g.: quad -> n=4; model triplet -> n=3; model pair -> n=2;
	 unrestricted triplet -> n<0; unrestricted pair -> n<0 */
      if ((n_img > 2 && num[0]>64 && num[1]>64 && num[2]>64 && num[3]>64)
	  &&  n < 3)	continue;

/* hack due to problems with approx in det_lsq: */
X = 0.0; Y = 0.0; Z = (Zmin_lay[0]+Zmax_lay[0])/2.0;
for (j=0; j<n_img; j++) { X += Ex[j].x0; Y += Ex[j].y0; }
X /= n_img; Y /= n_img;
      /* ******************************** */


      det_lsq (Ex, I, ap, mmp,
	       x[0], y[0], x[1], y[1], x[2], y[2], x[3], y[3], &X, &Y, &Z);


      /* write a sequential point number,
	 sumg, if the point was used, and the 3D coordinates */
	  if( useCompression )
		  gzprintf (fileGz, "%4d", i+1);
	  else
		  fprintf (fp1, "%4d", i+1);


      /*
      if (p[0] > -1)	fprintf (fp1, "  %4d", pix[0][p[0]].sumg);
      else			fprintf (fp1, "   %4d", -1);
      if (p[1] > -1)	fprintf (fp1, "  %4d", pix[1][p[1]].sumg);
      else			fprintf (fp1, "  %4d", -1);
      if (p[2] > -1)	fprintf (fp1, "  %4d", pix[2][p[2]].sumg);
      else			fprintf (fp1, "  %4d", -1);
      if (p[3] > -1)	fprintf (fp1, "  %4d", pix[3][p[3]].sumg);
      else			fprintf (fp1, "  %4d", -1);
      */

	  if( useCompression ) {
		  gzprintf (fileGz, " %9.3f %9.3f %9.3f", X, Y, Z);
		  if (p[0] > -1)	gzprintf (fileGz, " %4d", pix[0][p[0]].pnr);
		  else			gzprintf (fileGz, " %4d", -1);
		  if (p[1] > -1)	gzprintf (fileGz, " %4d", pix[1][p[1]].pnr);
		  else			gzprintf (fileGz, " %4d", -1);
		  if (p[2] > -1)	gzprintf (fileGz, " %4d", pix[2][p[2]].pnr);
		  else			gzprintf (fileGz, " %4d", -1);
		  if (p[3] > -1)	gzprintf (fileGz, " %4d\n", pix[3][p[3]].pnr);
		  else			gzprintf (fileGz, " %4d\n", -1);
	  } else {
		  fprintf (fp1, " %9.3f %9.3f %9.3f", X, Y, Z);
		  if (p[0] > -1)	fprintf (fp1, " %4d", pix[0][p[0]].pnr);
		  else			fprintf (fp1, " %4d", -1);
		  if (p[1] > -1)	fprintf (fp1, " %4d", pix[1][p[1]].pnr);
		  else			fprintf (fp1, " %4d", -1);
		  if (p[2] > -1)	fprintf (fp1, " %4d", pix[2][p[2]].pnr);
		  else			fprintf (fp1, " %4d", -1);
		  if (p[3] > -1)	fprintf (fp1, " %4d\n", pix[3][p[3]].pnr);
		  else			fprintf (fp1, " %4d\n", -1);
	  }

      /* write data as new points to dump for rdb */
      if (examine == 4)
	{
	  fprintf (fp2, "%d %10.3f %10.3f %10.3f   %d    ", i, X, Y, Z, 3);
	  for (j=0; j<n_img; j++)
	    if (x[j] != -1e10)
	      fprintf (fp2, "%4d %8.5f %8.5f    ", i, x[j], y[j]);
	    else
	      fprintf (fp2, "%4d %8.5f %8.5f    ", -999, x[j], y[j]);
	  fprintf (fp2, "\n");
	  fclose (fp2);
	}

      if (Z < Zlo)  Zlo = Z;   if (Z > Zhi)  Zhi = Z;
    }

  if( useCompression )
	  gzclose( fileGz );
  else
	  fclose (fp1);

  rmsX = sqrt(rmsX/match); rmsY = sqrt(rmsY/match); rmsZ = sqrt(rmsZ/match);
  mean_sigma0 = sqrt (mean_sigma0/match);

  if( verbose )
	  printf ("Match: %d, => sigma0 = %4.2f micron, RMS = %5.3f/%5.3f/%5.3f mm", match, mean_sigma0*1000, rmsX, rmsY, rmsZ);

  /* sort coordinates for binary search in epi line segment drawing */
  for (i=0; i<n_img; i++)  quicksort_coord2d_x (geo[0], num[0]);

  if( verbose ) puts ("Determinate done\n");

  return TCL_OK;

}


int sequence_proc_c  ()
{
  int     i, j, ok, k;
  char    seq_ch[3], seq_name[4][128];


  fpp = fopen_r ("parameters/sequence.par");
  for (i=0; i<4; i++)
  fscanf (fpp, "%s\n", seq_name[i]);     /* name of sequence */
  fscanf (fpp,"%d\n", &seq_first);
  fscanf (fpp,"%d\n", &seq_last);
  fclose (fpp);

  // ChrisB: Main loop - processes each set of images:
  for (i=seq_first; i<seq_last+1; i++)
    {

		printf( "\rProcessing image set %03d        ", i );
		fflush( stdout );

	// ChrisB: why not just "%03d" ?????
      if (i < 10)             sprintf (seq_ch, "00%1d", i);
      else if (i < 100)       sprintf (seq_ch, "0%2d",  i);
      else       sprintf (seq_ch, "%3d",  i);

      for (j=0; j<n_img; j++)
	{
	  sprintf (img_name[j], "%s%s", seq_name[j], seq_ch);
	  sprintf (img_lp_name[j], "%s%s_lp", seq_name[j], seq_ch);
	  sprintf (img_hp_name[j], "%s%s_hp", seq_name[j], seq_ch);
	}

      if (chfield == 0)       sprintf (res_name, "res/rt_is.%s", seq_ch);
      else            sprintf (res_name, "res/rt_is.%s_%1d", seq_ch, chfield);

	  if( verbose ) {
		  sprintf (buf, "\nImages:");
		  for (j=0; j<n_img; j++) sprintf (buf, "%s  %s", buf, img_name[j]);
		  puts (buf);
	  }

      /* calling function for each sequence-n-tupel */
      /* read and display original images */

      for (k=0; k<n_img; k++)
	{
	  /* reading */
	  read_image (/*interp,*/ img_name[k], img[k]);
	}

	  if (oddevenflag) { 
		  oddeven_c();
	  if( verbose )puts("\nOddEven switched on\n");
	  } else { if( verbose )puts("\nOddEven switched off\n"); }
      if (hp_flag) {
	pre_processing_c ();
	if( verbose )puts("\nHighpass switched on\n");
      } else { if( verbose )puts("\nHighpass switched off\n"); }
      

	  /****************************************************************\
	  * Chris B : Main Routines:
	  \****************************************************************/

	  detection_proc_c ();
      correspondences_proc_c ();
      determination_proc_c ();

      /* asci */
      /*
      sprintf (filename, "corr_%s", seq_ch);
      fp1 = fopen (filename, "w");
      fprintf(fp1,"%d\n", match);
       for (j=0;j<match;j++)
	{
	  p1=p2=p3=p4=-2;
	  if (con[j].p[0] > -1) p1=geo[0][con[j].p[0]].pnr;
	  if (con[j].p[1] > -1) p2=geo[1][con[j].p[1]].pnr;
	  if (con[j].p[2] > -1) p3=geo[2][con[j].p[2]].pnr;
	  if (con[j].p[3] > -1) p4=geo[3][con[j].p[3]].pnr;
	  fprintf(fp1,"%4d  %4d  %4d  %4d  %4d\n", j+1,p1,p2,p3,p4 );
	}
      fclose (fp1);
      */

      /* delete unneeded files */

      for (j=0; j<n_img; j++)
	{
	  ok = remove (img_lp_name[j]);
	  ok = remove (img_hp_name[j]);
	  /*
	  sprintf (filename, "%s_targets", img_name[j]); ok = remove (filename);
	  */
	}
    }
  /* reset of display flag */
  display = 1;

  return TCL_OK;
}


#if 0
int calibration_proc_c (/*ClientData clientData, Tcl_Interp* interp,*/ int argc, const char** argv)
{
  int i, j, sel, i_img, k, n, sup;
  int intx1, inty1, intx2, inty2;
  coord_2d    	apfig1[11][11];	/* regular grid for ap figures */
  coord_2d     	apfig2[11][11];	/* ap figures */
  coord_3d     	fix4[4];       	/* object points for preorientation */
  coord_2d     	crd0[4][4];    	/* image points for preorientation */
  char	       	filename[256], val[256];
  const char *valp;

  //Tk_PhotoHandle img_handle;
  //Tk_PhotoImageBlock img_block;

  /* read support of unsharp mask */
  fp1 = fopen ("parameters/unsharp_mask.par", "r");
  if (! fp1)	sup = 12;
  else	{ fscanf (fp1, "%d\n", &sup); fclose (fp1); }

  /* Get Selection value from TclTk */

  // ChrisB: what does this do?? Set a value......
  //valp = Tcl_GetVar(interp, "sel",  TCL_GLOBAL_ONLY);
  //sel = atoi (valp);
  sel = 1;	// set a value....

  switch (sel)
    {
    case 1: /*  read calibration parameter file  */
      fp1 = fopen_r ("parameters/cal_ori.par");
      fscanf (fp1,"%s\n", fixp_name);
      for (i=0; i<4; i++)
	{
	  fscanf (fp1, "%s\n", img_name[i]);
	  fscanf (fp1, "%s\n", img_ori0[i]);
	}
      fscanf (fpp, "%d\n", &tiff_flag);
      fscanf (fp1, "%d\n", &chfield);
      fclose (fp1);

      /*  create file names  */
      for (i=0; i<n_img; i++)
	{
	  strcpy (img_ori[i], img_name[i]);
	  strcat (img_ori[i], ".ori");
	  strcpy (img_addpar0[i], img_name[i]);
	  strcat (img_addpar0[i], ".addpar0");
	  strcpy (img_addpar[i], img_name[i]);
	  strcat (img_addpar[i], ".addpar");
	  strcpy (img_hp_name[i], img_name[i]);
	  strcat (img_hp_name[i], "_hp");
	}

      for (i=0; i<n_img; i++)
	{

	  zoom_x[i] = imx/2, zoom_y[i] = imy/2, zoom_f[i] = 1;

	  read_image (/*interp,*/ img_name[i], img[i]);

	  sprintf(val, "camcanvas %d", i+1);
	  //Tcl_Eval(interp, val);

	  //img_handle = Tk_FindPhoto( interp, "temp");
	  //Tk_PhotoGetImage (img_handle, &img_block);
	  //tclimg2cimg (interp, img[i], &img_block);

	  sprintf(val, "newimage %d", i+1);
	  //Tcl_Eval(interp, val);
	}

      break;


    case 2: puts ("Detection procedure"); strcpy(val,"");

      /* Highpass Filtering */
      pre_processing_c (/*clientData, interp,*/ argc, argv);

      /* reset zoom values */
      for (i=0; i<n_img; i++)
	{
	  zoom_x[i] = imx/2; zoom_y[i] = imy/2; zoom_f[i] = 1;
	}

     /* copy images because the target recognition
	 will set greyvalues to zero */

     for (i=0; i<n_img; i++)
	{
	  copy_images (img[i], img0[i]);
	}


      /* target recognition */
      for (i=0; i<n_img; i++)
	{
	  targ_rec (/*interp,*/ img[i], img0[i], "parameters/detect_plate.par",
		    0, imx, 1, imy, pix[i], i, &num[i]);

	  sprintf (buf,"image %d: %d,  ", i+1, num[i]);
	  strcat(val, buf);

	  if (num[i] > nmax)  exit (1);
	}

      /* save pixel coord as approx. for template matching */
      if (examine)	for (i=0; i<n_img; i++)
	{
	  sprintf (filename, "%s_pix", img_name[i]);
	  fp1 = fopen (filename, "w");
	  for (j=0; j<num[i]; j++)
	    fprintf (fp1, "%4d  %8.3f  %8.3f\n",
		     pix[i][j].pnr, pix[i][j].x, pix[i][j].y);

	  fclose (fp1);
	}

      sprintf(buf,"Number of detected targets, interaction enabled");
      //Tcl_SetVar(interp, "tbuf", buf, TCL_GLOBAL_ONLY);
      //Tcl_Eval(interp, ".text delete 2");
      //Tcl_Eval(interp, ".text insert 2 $tbuf");
      //Tcl_SetVar(interp, "tbuf", val, TCL_GLOBAL_ONLY);
      //Tcl_Eval(interp, ".text delete 3");
      //Tcl_Eval(interp, ".text insert 3 $tbuf");
      break;


    case 3:	pp1=0;	pp2=0;	pp3=0;	pp4=0;

      for (i=0; i<n_img; i++)
	{
	  sprintf (buf, "%d targets remain", num[i]);
	  puts (buf);
	}
      fp1 = fopen_r ("parameters/man_ori.par");
      for (i=0; i<n_img; i++)
	{
	  fscanf (fp1, "%d %d %d %d\n", &nr[i][0], &nr[i][1], &nr[i][2], &nr[i][3]);
	}
      fclose (fp1);

      for (i=0; i<n_img; i++)
	{
	  sprintf(val, "measure %d %d %d %d %d", nr[i][0], nr[i][1], nr[i][2], nr[i][3], i+1);
	  //Tcl_Eval(interp, val);
#if 0
	  // ChrisB: do we need this?
	  valp = Tcl_GetVar(interp, "px0",  TCL_GLOBAL_ONLY);
	  pix0[i][0].x = atoi (valp);
	  valp = Tcl_GetVar(interp, "py0",  TCL_GLOBAL_ONLY);
	  pix0[i][0].y = atoi (valp);
	  valp = Tcl_GetVar(interp, "px1",  TCL_GLOBAL_ONLY);
	  pix0[i][1].x = atoi (valp);
	  valp = Tcl_GetVar(interp, "py1",  TCL_GLOBAL_ONLY);
	  pix0[i][1].y = atoi (valp);
	  valp = Tcl_GetVar(interp, "px2",  TCL_GLOBAL_ONLY);
	  pix0[i][2].x = atoi (valp);
	  valp = Tcl_GetVar(interp, "py2",  TCL_GLOBAL_ONLY);
	  pix0[i][2].y = atoi (valp);
	  valp = Tcl_GetVar(interp, "px3",  TCL_GLOBAL_ONLY);
	  pix0[i][3].x = atoi (valp);
	  valp = Tcl_GetVar(interp, "py3",  TCL_GLOBAL_ONLY);
	  pix0[i][3].y = atoi (valp);
#endif
	}

      /* write measured coordinates to file for next trial */
      fp1 = fopen ("man_ori.dat", "w");
      for (i=0; i<n_img; i++)
	for (j=0; j<4; j++)
	  fprintf (fp1, "%f %f\n", pix0[i][j].x, pix0[i][j].y);
      fclose (fp1);

      break;


    case 4: /* read pixel coordinates of older pre-orientation */

      /* read point numbers of pre-clicked points */
      fp1 = fopen_r ("parameters/man_ori.par");
      for (i=0; i<n_img; i++)
	{
	  fscanf (fp1, "%d %d %d %d\n",
		  &nr[i][0], &nr[i][1], &nr[i][2], &nr[i][3]);
	}
      fclose (fp1);

      /* read coordinates of pre-clicked points */
      fp1 = fopen ("man_ori.dat", "r");
      if (! fp1)	break;
      for (i_img=0; i_img<n_img; i_img++)	for (i=0; i<4; i++)
	{
#if 0
	  fscanf (fp1, "%lf %lf\n",
		  &pix0[i_img][i].x, &pix0[i_img][i].y);
	  drawcross (interp,  (int) pix0[i_img][i].x,
		     (int) pix0[i_img][i].y, cr_sz+2, i_img, "red");
	  draw_pnr (interp, (int) pix0[i_img][i].x, (int) pix0[i_img][i].y,
		    nr[i_img][i], i_img, "red");
#endif

	}
      fclose (fp1);

      break;


    case 5: puts ("Sort grid points");
      for (i=0; i<n_img; i++)
	{
	  /* read control point coordinates for man_ori points */
	  fp1 = fopen_r (fixp_name);
	  k = 0;
	  while ( fscanf (fp1, "%d %lf %lf %lf", &fix[k].pnr,
			  &fix[k].x, &fix[k].y, &fix[k].z) != EOF) k++;
	  fclose (fp1);
	  nfix = k;

	  /* take clicked points from control point data set */
	  for (j=0; j<4; j++)	for (k=0; k<nfix; k++)
	    {
	      if (fix[k].pnr == nr[i][j])	fix4[j] = fix[k];
	    }

	  /* get approx for orientation and ap */
	  read_ori (&Ex[i], &I[i], img_ori0[i]);
	  fp1 = fopen (img_addpar0[i], "r");
	  if (! fp1)  fp1 = fopen ("addpar.raw", "r");

	  if (fp1) {
	    fscanf (fp1, "%lf %lf %lf %lf %lf %lf %lf",
		    &ap[i].k1,&ap[i].k2,&ap[i].k3,
		    &ap[i].p1,&ap[i].p2,
		    &ap[i].scx,&ap[i].she);
	    fclose (fp1);} else {
	      printf("no addpar.raw\n");
	      ap[i].k1=ap[i].k2=ap[i].k3=ap[i].p1=ap[i].p2=ap[i].she=0.0;
	      ap[i].scx=1.0;
	    }


	  /* transform clicked points */
	  for (j=0; j<4; j++)
	    {
	      pixel_to_metric (pix0[i][j].x, pix0[i][j].y,
			       imx,imy, pix_x, pix_y,
			       &crd0[i][j].x, &crd0[i][j].y,
			       chfield);
	      correct_brown_affin (crd0[i][j].x, crd0[i][j].y, ap[i],
				   &crd0[i][j].x, &crd0[i][j].y);
	    }

	  /* raw orientation with 4 points */
	  raw_orient (Ex[i], I[i], ap[i], mmp, 4, fix4, crd0[i], &Ex[i]);
	  sprintf (filename, "raw%d.ori", i);
	  write_ori (Ex[i], I[i], filename);
	 
	  /* sorting of detected points by back-projection */
	  sortgrid_man (/*interp,*/ Ex[i], I[i], ap[i], mmp,
			imx,imy, pix_x,pix_y,
			nfix, fix, num[i], pix[i], chfield, i);

	  /* adapt # of detected points */
	  num[i] = nfix;

	  for (j=0; j<nfix; j++)
	    {
#if 0
	      if (pix[i][j].pnr < 0)	continue;
	      intx1 = (int) pix[i][j].x ;
	      inty1 = (int) pix[i][j].y ;

	      drawcross (interp, intx1, inty1, cr_sz, i, "white");
	      draw_pnr (interp, intx1, inty1, fix[j].pnr, i, "white");
#endif
	    }
	}

      /* dump dataset for rdb */
      if (examine == 4)
	{
	  /* create filename for dumped dataset */
	  sprintf (filename, "dump_for_rdb");
	  fp1 = fopen (filename, "w");

	  /* write # of points to file */
	  fprintf (fp1, "%d\n", nfix);

	  /* write point and image coord to file */
	  for (i=0; i<nfix; i++)
	    {
	      fprintf (fp1, "%4d %10.3f %10.3f %10.3f   %d    ",
		       fix[i].pnr, fix[i].x, fix[i].y, fix[i].z, 0);
	      for (i_img=0; i_img<n_img; i_img++)
		{
		  if (pix[i_img][i].pnr >= 0)
		    {
		      /* transform pixel coord to metric */
		      pixel_to_metric (pix[i_img][i].x,
				       pix[i_img][i].y, imx,imy, pix_x, pix_y,
				       &crd[i_img][i].x, &crd[i_img][i].y,
				       chfield);
		      fprintf (fp1, "%4d %8.5f %8.5f    ",
			       pix[i_img][i].pnr,
			       crd[i_img][i].x, crd[i_img][i].y);
		    }
		  else
		    {
		      fprintf (fp1, "%4d %8.5f %8.5f    ",
			       pix[i_img][i].pnr, 0.0, 0.0);
		    }
		}
	      fprintf (fp1, "\n");
	    }
	  fclose (fp1);
	  printf ("dataset dumped into %s\n", filename);
	}
      break;




    case 6: puts ("Orientation"); strcpy(buf, "");

      for (i_img=0; i_img<n_img; i_img++)
	{
	  for (i=0; i<nfix ; i++)
	    {
	      pixel_to_metric (pix[i_img][i].x, pix[i_img][i].y,
			       imx,imy, pix_x, pix_y,
			       &crd[i_img][i].x, &crd[i_img][i].y,
			       chfield);
	      crd[i_img][i].pnr = pix[i_img][i].pnr;
	    }

	  /* save data for special use of resection routine */
	  if (examine == 4)
	    {
	      printf ("try write resection data to disk\n");
	      /* point coordinates */
	      sprintf (filename, "resect_%s.fix", img_name[i_img]);
	      write_ori (Ex[i_img], I[i_img], img_ori[i_img]);
	      fp1 = fopen (filename, "w");
	      for (i=0; i<nfix; i++)
		fprintf (fp1, "%3d  %10.5f  %10.5f  %10.5f\n",
			 fix[i].pnr, fix[i].x, fix[i].y, fix[i].z);
	      fclose (fp1);

	      /* metric image coordinates */
	      sprintf (filename, "resect_%s.crd", img_name[i_img]);
	      fp1 = fopen (filename, "w");
	      for (i=0; i<nfix; i++)
		fprintf (fp1,
			 "%3d  %9.5f  %9.5f\n", crd[i_img][i].pnr,
			 crd[i_img][i].x, crd[i_img][i].y);
	      fclose (fp1);

	      /* orientation and calibration approx data */
	      write_ori (Ex[i_img], I[i_img], "resect.ori0");
	      fp1 = fopen ("resect.ap0", "w");
	      fprintf (fp1, "%f %f %f %f %f %f %f",
		       ap[i_img].k1, ap[i_img].k2, ap[i_img].k3,
		       ap[i_img].p1, ap[i_img].p2,
		       ap[i_img].scx, ap[i_img].she);
	      fclose (fp1);
	      printf ("resection data written to disk\n");
	    }


	  /* resection routine */
	  /* ================= */

	  if (examine != 4)
	    orient (/*interp,*/ Ex[i_img], I[i_img], ap[i_img], mmp,
		    nfix, fix, crd[i_img],
		    &Ex[i_img], &I[i_img], &ap[i_img], i_img);

	  /* ================= */


	  /* resection with dumped datasets */
	  if (examine == 4)
	    {

	      printf("Resection with dumped datasets? (y/n)");
	      scanf("%s",buf);
	      if (buf[0] != 'y')	continue;
	      strcpy (buf, "");

	      /* read calibration frame datasets */
	      for (n=0, nfix=0, dump_for_rdb=0; n<100; n++)
		{
		  sprintf (filename, "resect.fix%d", n);
		  fp1 = fopen (filename, "r");
		  if (! fp1)	continue;

		  printf("reading file: %s\n", filename);
		  printf ("reading dumped resect data #%d\n", n);
		  k = 0;
		  while ( fscanf (fp1, "%d %lf %lf %lf",
				  &fix[nfix+k].pnr, &fix[nfix+k].x,
				  &fix[nfix+k].y, &fix[nfix+k].z)
			  != EOF) k++;
		  fclose (fp1);
		  /* read metric image coordinates */
		  sprintf (filename, "resect_%d.crd%d", i_img, n);
		  printf("reading file: %s\n", filename);
		  fp1 = fopen (filename, "r");
		  for (i=nfix; i<nfix+k; i++)
		    fscanf (fp1, "%d %lf %lf",
			    &crd[i_img][i].pnr,
			    &crd[i_img][i].x, &crd[i_img][i].y);
		  nfix += k;
		}

	      /* resection */
	      orient (/*interp,*/ Ex[i_img], I[i_img], ap[i_img], mmp,
		      nfix, fix, crd[i_img],
		      &Ex[i_img], &I[i_img], &ap[i_img], i_img);
	    }


	  /* save orientation and additional parameters */
	  write_ori (Ex[i_img], I[i_img], img_ori[i_img]);
	  fp1 = fopen (img_addpar[i_img], "w");
	  fprintf (fp1, "%f %f %f %f %f %f %f",
		   ap[i_img].k1, ap[i_img].k2, ap[i_img].k3,
		   ap[i_img].p1, ap[i_img].p2,
		   ap[i_img].scx, ap[i_img].she);
	  fclose (fp1);
	}

      //Tcl_Eval(interp, ".text delete 3");
      //Tcl_Eval(interp, ".text delete 1");
      //Tcl_Eval(interp, ".text insert 1 \"Orientation and self calibration \"");
      //Tcl_Eval(interp, ".text delete 2");
      //Tcl_Eval(interp, ".text insert 2 \"...done, sigma0 for each image -> \"");
      //Tcl_SetVar(interp, "tbuf", buf, TCL_GLOBAL_ONLY);
      //Tcl_Eval(interp, ".text insert 3 $tbuf");

      break;

    case 7: checkpoint_proc (/*interp*/);
#if 0
      sprintf(val,"blue: planimetry,   yellow: height");
      Tcl_SetVar(interp, "tbuf", val, TCL_GLOBAL_ONLY);
      Tcl_Eval(interp, ".text delete 2");
      Tcl_Eval(interp, ".text insert 2 $tbuf");
      Tcl_SetVar(interp, "tbuf", buf, TCL_GLOBAL_ONLY);
      Tcl_Eval(interp, ".text delete 3");
      Tcl_Eval(interp, ".text insert 3 $tbuf");
#endif
      break;


    case 8: /* draw additional parameter figures */

      //Tcl_Eval(interp, "clearcam");

      /*  read orientation and additional parameters  */
      for (i=0; i<n_img; i++)	read_ori (&Ex[i], &I[i], img_ori[i]);
      for (i=0; i<n_img; i++)
	{
	  fp1 = fopen_r (img_addpar[i]);
	  fscanf (fp1,"%lf %lf %lf %lf %lf %lf %lf",
		  &ap[i].k1, &ap[i].k2, &ap[i].k3,
		  &ap[i].p1, &ap[i].p2, &ap[i].scx, &ap[i].she);
	  fclose (fp1);
	}
      for (i_img=0; i_img<n_img; i_img++)
	{
	  /* create undistorted grid */
	  for (i=0; i<11; i++)	for (j=0; j<11; j++)
	    {
	      apfig1[i][j].x = i * imx/10;
	      apfig1[i][j].y = j * imy/10;
	    }
	  /* draw undistorted grid */
	  for (i=0; i<10; i++)	for (j=0; j<10; j++)
	    {
	      intx1 = (int) apfig1[i][j].x;
	      inty1 = (int) apfig1[i][j].y;
	      intx2 = (int) apfig1[i+1][j].x;
	      inty2 = (int) apfig1[i][j+1].y;
	      //drawvector (interp, intx1, inty1, intx2, inty1, 1, i_img, "black");
	      //drawvector (interp, intx1, inty1, intx1, inty2, 1, i_img, "black");
	    }
	  for (j=0; j<10; j++)
	    {
	      intx1 = (int) apfig1[10][j].x;
	      inty1 = (int) apfig1[10][j].y;
	      inty2 = (int) apfig1[10][j+1].y;
	      //drawvector (interp, intx1, inty1, intx1, inty2, 1, i_img, "black");
	    }
	  for (i=0; i<10; i++)
	    {
	      intx1 = (int) apfig1[i][10].x;
	      inty1 = (int) apfig1[i][10].y;
	      intx2 = (int) apfig1[i+1][10].x;
	      //drawvector (interp, intx1, inty1, intx2, inty1, 1, i_img, "black");
	    }
	  /* distort grid */
	  for (i=0; i<11; i++)	for (j=0; j<11; j++)
	    {
	      /* transform to metric, distort and re-transform */
	      pixel_to_metric (apfig1[i][j].x, apfig1[i][j].y,
			       imx,imy, pix_x,pix_y,
			       &apfig2[i][j].x, &apfig2[i][j].y, chfield);
	      distort_brown_affin (apfig2[i][j].x, apfig2[i][j].y,
				   ap[i_img], &apfig2[i][j].x, &apfig2[i][j].y);
	      metric_to_pixel (apfig2[i][j].x, apfig2[i][j].y,
			       imx,imy, pix_x,pix_y,
			       &apfig2[i][j].x, &apfig2[i][j].y, chfield);
	      /* exaggerate distortion by factor 5 */
	      apfig2[i][j].x = 5*apfig2[i][j].x - 4*apfig1[i][j].x;
	      apfig2[i][j].y = 5*apfig2[i][j].y - 4*apfig1[i][j].y;

	    }
	  /* draw distorted grid */
	  for (i=0; i<10; i++)	for (j=0; j<10; j++)
	    {
	      intx1 = (int) apfig2[i][j].x;
	      inty1 = (int) apfig2[i][j].y;
	      intx2 = (int) apfig2[i+1][j].x;
	      inty2 = (int) apfig2[i+1][j].y;
	      //drawvector (interp, intx1, inty1, intx2, inty2, 3, i_img, "magenta");
	      intx2 = (int) apfig2[i][j+1].x ;
	      inty2 = (int) apfig2[i][j+1].y ;
	      //drawvector (interp, intx1, inty1, intx2, inty2, 3, i_img, "magenta");
	    }
	  for (j=0; j<10; j++)
	    {
	      intx1 = (int) apfig2[10][j].x;
	      inty1 = (int) apfig2[10][j].y;
	      intx2 = (int) apfig2[10][j+1].x;
	      inty2 = (int) apfig2[10][j+1].y;
	      //drawvector (interp, intx1, inty1, intx2, inty2, 3, i_img, "magenta");
	    }
	  for (i=0; i<10; i++)
	    {
	      intx1 = (int) apfig2[i][10].x;
	      inty1 = (int) apfig2[i][10].y;
	      intx2 = (int) apfig2[i+1][10].x;
	      inty2 = (int) apfig2[i+1][10].y ;
	      //drawvector (interp, intx1, inty1, intx2, inty2, 3, i_img, "magenta");
	    }
	}

      break;
    }
  return TCL_OK;
}
#endif


int quit_proc_c ()
{
  int i, k;

  for (i=0; i<n_img; i++) { free (img[i]); free (img0[i]); }
  free (zoomimg);

  /* delete unneeded files */
  for (i=0; i<n_img; i++)      	k = remove (img_lp_name[i]);
  return TCL_OK;
}

                                                                                                                            sequence/peakfitting.c                                                                              0000644 0001131 0001131 00000031205 10274656212 014005  0                                                                                                    ustar   amns                            amns                                                                                                                                                                                                                   /****************************************************************************

Routine:		       	peakfitting.c

Author/Copyright:		Hans-Gerd Maas

Address:		       	Institute of Geodesy and Photogrammetry
		      		ETH - Hoenggerberg
		       		CH - 8093 Zurich

Creation Date:			Feb. 1990

Description:

****************************************************************************/

#include "ptv.h"

int peak_fit_new ( img, par_file, xmin,xmax,ymin,ymax, pix, nr)

unsigned char	*img;		       	/* image data */
char	       	par_file[];	       	/* name of parameter file */
int	       	xmin,xmax,ymin,ymax;	/* search area */
target	       	pix[];		       	/* pixel coord array, global */
int	       	nr;		       	/* image number for display */

/*  newest peak fitting technique for particle coordinate determination  */
/*  labeling with discontinuity,
	reunification with distance and profile criteria  */
{

  int	       	n_peaks=0;	      /* # of peaks detected */
  int     	n_wait;	      	      /* size of waitlist for connectivity */
  int	       	x8[8], y8[8];  	      /* neighbours for connectivity */
  int	       	p2;	       	      /* considered point number */
  int	      	sumg_min, gvthres[4], thres, disco, nxmin,nxmax, nymin,nymax, nnmin, nnmax;
  /* parameters for target acceptance */
  int           pnr, sumg, xn, yn;	/* collecting variables for center of gravity */
  int         	n_target=0;	/* # of targets detected */
  int		intx1, inty1;		/* pixels for profile test and crosses */
  int          	unify;		       	/* flag for unification of targets */
  int	       	unified=0;	       	/* # of unified targets */
  int		non_unified=0;		/* # of tested, but not unified targets */
  register int	i, j, k, l, m, n;     /* loop variables */
  unsigned char	gv, gvref;     	/* current and reference greyvalue */
  unsigned char	gv1, gv2;	       	/* greyvalues for profile test */
  void	       	check_touch ();		/* marks touch events */
  double       	x1,x2,y1,y2,s12;	/* values for profile test */
  double       	x, y;	              /* preliminary coordinates */
  short	    	*label_img;           /* target number labeling */
  peak	       	*peaks, *ptr_peak;    /* detected peaks */
  targpix       waitlist[2048];     /* pix to be tested for connectivity */
  FILE   	*fpp;	       	/* parameter file pointer */

  /* read image name, threshold and shape limits from parameter file */

/*------------------------------------------------------------------------*/

  fpp = fopen_r (par_file);
  fscanf (fpp, "%d", &gvthres[0]);      /* threshold for binarization 1.image */
  fscanf (fpp, "%d", &gvthres[1]);      /* threshold for binarization 2.image */
  fscanf (fpp, "%d", &gvthres[2]);      /* threshold for binarization 3.image */
  fscanf (fpp, "%d", &gvthres[3]);      /* threshold for binarization 4.image */
  fscanf (fpp, "%d", &disco);		/* max discontinuity */
  fscanf (fpp, "%d  %d", &nnmin, &nnmax);	/* min. and max. number of */
  fscanf (fpp, "%d  %d", &nxmin, &nxmax);	/* pixels per target,  	*/
  fscanf (fpp, "%d  %d", &nymin, &nymax);	/* abs, in x, in y    	*/
  fscanf (fpp, "%d", &sumg_min);		       	/* min. sumg */
  fscanf (fpp, "%d", &cr_sz);		       	/* size of crosses */
  fclose (fpp);

  /* give thres value refering to image number */
  thres=gvthres[nr];

  /*------------------------------------------------------------------------*/

  /* allocate memory */

  /*------------------------------------------------------------------------*/

  label_img = (short *) calloc (imgsize, sizeof(short));
  peaks = (peak *) malloc (4*nmax * sizeof(peak));
  ptr_peak = peaks;

/*------------------------------------------------------------------------*/

  /*------------------------------------------------------------------------*/

  /* 1.: connectivity analysis with peak search and discontinuity criterion */

  /*------------------------------------------------------------------------*/

  if( verbose ) puts("Searching local maxima, connectivity analysis, peak factor 2 set");

  for (i=ymin; i<ymax; i++)  for (j=xmin; j<xmax; j++)
    {
      n = i*imx + j;

      /* compare with threshold */
      gv = *(img + n);   if (gv <= 2*thres)	continue;

      /* skip already labeled pixel */
      if (*(label_img + n) != 0)	continue;

      /* check, wether pixel is a local maximum */
      if ( gv >= *(img + n-1)
	   &&	gv >= *(img + n+1)
	   &&	gv >= *(img + n-imx)
	   &&	gv >= *(img + n+imx)
	   &&	gv >= *(img + n-imx-1)
	   &&	gv >= *(img + n+imx-1)
	   &&	gv >= *(img + n-imx+1)
	   &&	gv >= *(img + n+imx+1) )
	{
	  /* label peak in label_img, initialize peak */
	  n_peaks++;
	  *(label_img + n) = n_peaks;
	  ptr_peak->pos = n;
	  ptr_peak->status = 1;
	  ptr_peak->xmin = j;	ptr_peak->xmax = j;
	  ptr_peak->ymin = i;	ptr_peak->ymax = i;
	  ptr_peak->unr = 0;
	  ptr_peak->n = 0;
	  ptr_peak->sumg = 0;
	  ptr_peak->x = 0;
	  ptr_peak->y = 0;
	  ptr_peak->n_touch = 0;
	  for (k=0; k<4; k++)	ptr_peak->touch[k] = 0;
	  ptr_peak++;

	  waitlist[0].x = j;  waitlist[0].y = i;  n_wait = 1;

	  while (n_wait > 0)
	    {
	      gvref = *(img + imx*(waitlist[0].y) + (waitlist[0].x));

	      x8[0] = waitlist[0].x - 1;	y8[0] = waitlist[0].y;
	      x8[1] = waitlist[0].x + 1;	y8[1] = waitlist[0].y;
	      x8[2] = waitlist[0].x;		y8[2] = waitlist[0].y - 1;
	      x8[3] = waitlist[0].x;		y8[3] = waitlist[0].y + 1;
	      /*
		x8[4] = waitlist[0].x - 1;	y8[4] = waitlist[0].y - 1;
		x8[5] = waitlist[0].x + 1;	y8[5] = waitlist[0].y - 1;
		x8[6] = waitlist[0].x - 1;	y8[6] = waitlist[0].y + 1;
		x8[7] = waitlist[0].x + 1;	y8[7] = waitlist[0].y + 1;
		*/

	      /*
		for (k=0; k<8; k++)
		*/
	      for (k=0; k<4; k++)
		{
		  yn = y8[k];
		  xn = x8[k];
		  if (xn<0 || xn>imx || yn<0 || yn>imy)	continue;
		  n = imx*yn + xn;
		  if (*(label_img + n) != 0)	continue;
		  gv = *(img + n);

		  /* conditions for threshold, discontinuity, image borders */
		  /* and peak fitting */
		  if (   (gv > thres)
			 && (xn>=xmin)&&(xn<xmax) && (yn>=ymin)&&(yn<ymax)
			 && (gv <= gvref+disco)
			 && (gvref + disco >= *(img + imx*(yn-1) + xn))
			 && (gvref + disco >= *(img + imx*(yn+1) + xn))
			 && (gvref + disco >= *(img + imx*yn + (xn-1)))
			 && (gvref + disco >= *(img + imx*yn + (xn+1)))
			 /*
			 && (gvref + disco >= *(img + imx*(yn-1) + xn-1))
			 && (gvref + disco >= *(img + imx*(yn-1) + xn+1))
			 && (gvref + disco >= *(img + imx*(yn+1) + (xn-1)))
			 && (gvref + disco >= *(img + imx*(yn+1) + (xn+1)))  */
			 )
		    {
		      *(label_img + imx*yn + xn) = n_peaks;

		      waitlist[n_wait].x = xn;	waitlist[n_wait].y = yn;
		      n_wait++;
		    }
		}

	      n_wait--;
	      for (m=0; m<n_wait; m++)  waitlist[m] = waitlist[m+1];
	      waitlist[n_wait].x = 0;  waitlist[n_wait].y = 0;
	    }	/*  end of while-loop  */
	}

    }



  /*------------------------------------------------------------------------*/

  /* 2.:	process label image */
  /* 		(collect data for center of gravity, shape and brightness parameters) */
  /*		get touch events */

  /*------------------------------------------------------------------------*/

  if( verbose ) puts("Collecting peak data");

  for (i=ymin; i<ymax; i++)  for (j=xmin; j<xmax; j++)
    {
      n = i*imx + j;

      if (*(label_img+n) > 0)
	{
	  /* process pixel */
	  pnr = *(label_img+n);
	  gv = *(img+n);
	  ptr_peak = peaks + pnr - 1;
	  ptr_peak->n++;
	  ptr_peak->sumg += gv;
	  ptr_peak->x += (j * gv);
	  ptr_peak->y += (i * gv);
	  if (j < ptr_peak->xmin)	ptr_peak->xmin = j;
	  if (j > ptr_peak->xmax)	ptr_peak->xmax = j;
	  if (i < ptr_peak->ymin)	ptr_peak->ymin = i;
	  if (i > ptr_peak->ymax)	ptr_peak->ymax = i;


	  /* get touch events */

	  if (i>0 && j>1)      	check_touch (ptr_peak, pnr, *(label_img+n-imx-1));
	  if (i>0)	       	check_touch (ptr_peak, pnr, *(label_img+n-imx));
	  if (i>0 && j<imy-1)   check_touch (ptr_peak, pnr, *(label_img+n-imx+1));

	  if (j>0)     		check_touch (ptr_peak, pnr, *(label_img+n-1));
	  if (j<imy-1)		check_touch (ptr_peak, pnr, *(label_img+n+1));

	  if (i<imx-1 && j>0)	check_touch (ptr_peak, pnr, *(label_img+n+imx-1));
	  if (i<imx-1)		check_touch (ptr_peak, pnr, *(label_img+n+imx));
	  if (i<imx-1 && j<imy-1) check_touch (ptr_peak, pnr, *(label_img+n+imx+1));
	}
    }

  /*------------------------------------------------------------------------*/

  /* 3.:	reunification test: profile and distance */

  /*------------------------------------------------------------------------*/

  if( verbose ) puts("Unifying targets");

  for (i=0; i<n_peaks; i++)
    {

      if (peaks[i].n_touch == 0)	continue;	/* no touching targets */
      if (peaks[i].unr != 0)		continue;      	/* target already unified */

      /* profile criterion */
      /* point 1 */
      x1 = peaks[i].x / peaks[i].sumg;
      y1 = peaks[i].y / peaks[i].sumg;
      gv1 = *(img + peaks[i].pos);

      /* consider all touching points */
      for (j=0; j<peaks[i].n_touch; j++)
	{
	  p2 = peaks[i].touch[j] - 1;

	  if (p2 >= n_peaks) continue; /* workaround memory overwrite problem */
	  if ( p2 <0) continue;  /*  workaround memory overwrite problem */
	  if (peaks[p2].unr != 0)      	continue; /* target already unified */

	  /* point 2 */
	  x2 = peaks[p2].x / peaks[p2].sumg;
	  y2 = peaks[p2].y / peaks[p2].sumg;

	  gv2 = *(img + peaks[p2].pos);

	  s12 = sqrt ((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));

	  /* consider profile dot for dot */
	  /* if any points is by more than disco below profile, do not unify */
	  if (s12 < 2.0)	unify = 1;
	  else for (unify=1, l=1; l<s12; l++)
	    {
	      intx1 = (int) (x1 + l * (x2 - x1) / s12);
	      inty1 = (int) (y1 + l * (y2 - y1) / s12);
	      gv = *(img + inty1*imx + intx1) + disco;
	      if (gv < (gv1+l*(gv2-gv1)/s12) || gv<gv1 || gv<gv2)	unify = 0;
	      if (unify == 0)	break;
	    }
	  if (unify == 0)
	    {
	      non_unified++;
	      continue;
	    }

	  /* otherwise unify targets */
	  unified++;
	  peaks[i].unr = p2;
	  peaks[p2].x += peaks[i].x;
	  peaks[p2].y += peaks[i].y;
	  peaks[p2].sumg += peaks[i].sumg;
	  peaks[p2].n += peaks[i].n;
	  if (peaks[i].xmin < peaks[p2].xmin)	peaks[p2].xmin = peaks[i].xmin;
	  if (peaks[i].ymin < peaks[p2].ymin)	peaks[p2].ymin = peaks[i].ymin;
	  if (peaks[i].xmax > peaks[p2].xmax)	peaks[p2].xmax = peaks[i].xmax;
	  if (peaks[i].ymax > peaks[p2].ymax)	peaks[p2].ymax = peaks[i].ymax;
	}
    }

  /*------------------------------------------------------------------------*/

  /* 4.:	process targets */

  /*------------------------------------------------------------------------*/

  if( verbose ) puts("Calculate centers of gravity");

  for (i=0; i<n_peaks; i++)
    {
      /* check whether target touches image borders */
      if (peaks[i].xmin == xmin  &&  (xmax-xmin) > 32)	continue;
      if (peaks[i].ymin == ymin  &&  (xmax-xmin) > 32)	continue;
      if (peaks[i].xmax == xmax-1  &&  (xmax-xmin) > 32)	continue;
      if (peaks[i].ymax == ymax-1  &&  (xmax-xmin) > 32)	continue;

      if (   peaks[i].unr == 0
	     && peaks[i].sumg > sumg_min
	     && (peaks[i].xmax - peaks[i].xmin + 1) >= nxmin
	     && (peaks[i].ymax - peaks[i].ymin + 1) >= nymin
	     && (peaks[i].xmax - peaks[i].xmin) < nxmax
	     && (peaks[i].ymax - peaks[i].ymin) < nymax
	     && peaks[i].n >= nnmin
	     && peaks[i].n <= nnmax)
	{
	  sumg = peaks[i].sumg;

	  /* target coordinates */
	  pix[n_target].x = 0.5 + peaks[i].x / sumg;
	  pix[n_target].y = 0.5 + peaks[i].y / sumg;

	  /* target shape parameters */
	  pix[n_target].sumg = sumg;
	  pix[n_target].n = peaks[i].n;
	  pix[n_target].nx = peaks[i].xmax - peaks[i].xmin + 1;
	  pix[n_target].ny = peaks[i].ymax - peaks[i].ymin + 1;
	  pix[n_target].tnr = -1;
	  pix[n_target].pnr = n_target++;
	}
    }



  /* get number of touch events */
  if (examine==10)
    {
      for (x=0,i=0; i<n_target; i++)
	{
	  x += pix[i].n;
	}
      x /= n_target;
      if( verbose ) printf ("Average number of pix per target: %6.3f\n", x);

      for (sumg=0,i=0; i<n_peaks; i++)
	{
	  sumg += peaks[i].n_touch;
	}
      if( verbose ) printf ("Number of touch events: %d\n", sumg/2);

      y = 2*n_target*n_target*x/imgsize;
      if( verbose ) printf ("expected number of touch events: %6.0f\n", y);
    }

  free (label_img);
  free (peaks);

  return (n_target);

}


void check_touch (tpeak, p1, p2)

int    	p1, p2;
peak	*tpeak;

/* check wether p1, p2 are already marked as touching and mark them otherwise */

{
  int	m, done;


  if (p2 == 0)	return;		/* p2 not labeled */
  if (p2 == p1)	return;		/* p2 belongs to p1 */


  /* check wether p1, p2 are already marked as touching */
  for (done=0, m=0; m<tpeak->n_touch; m++)
    {
      if (tpeak->touch[m] == p2)	done = 1;
    }

  /* mark touch event */
  if (done == 0)
    {
      tpeak->touch[tpeak->n_touch] = p2;
      tpeak->n_touch++;
      /* don't allow for more than 4 touchs */
      if (tpeak->n_touch > 3)	tpeak->n_touch = 3;
    }
}
                                                                                                                                                                                                                                                                                                                                                                                           sequence/ptv.h                                                                                      0000644 0001131 0001131 00000000464 10273536514 012322  0                                                                                                    ustar   amns                            amns                                                                                                                                                                                                                   #include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

//#include <tcl.h>
//#include <tk.h>

#include "tiff.h"
#include "tiffio.h"

#include "typedefs.h"
#include "globals.h"

#include <zlib.h>

// Chris B: some defines to keep the code compiling:
#define TCL_OK 0
#define TCL_ERROR 1
                                                                                                                                                                                                            sequence/segmentation.c                                                                             0000644 0001131 0001131 00000033056 10252671241 014176  0                                                                                                    ustar   amns                            amns                                                                                                                                                                                                                   /****************************************************************************

Routine:				segmentation.c

Author/Copyright:		Hans-Gerd Maas

Address:		       	Institute of Geodesy and Photogrammetry
				       	ETH - Hoenggerberg
				       	CH - 8093 Zurich

Creation Date:			1988/89

Description:	  target recognition with adaptive threshold
		  based on high pass filter and connectivity analysis
						with peak fitting technique

Routines contained:		unsharp_mask () ...

****************************************************************************/

#include "ptv.h"

void highpass (pic_name, img, img_hp, dim_lp, filter_hp, field, nr)
char	        pic_name[256];  /* image name */
unsigned char  *img;
unsigned char  *img_hp;       	/* highpass filtered image */
int             dim_lp;	       	/* dimension of subtracted lowpass image */
int	        filter_hp;     	/* flag for additional filtering of _hp */
int    		field;	       	/* field to be used */
int	      	nr;	       	/* image number for display */

{
  register int	i;
  FILE	       	*fp;
  unsigned char *img_lp;
  char	       	lp_name[256], hp_name[256];
  register unsigned char *ptr1, *ptr2, *ptr3;
  void 	unsharp_mask ();

  sprintf (lp_name, "%s_lp", pic_name);
  sprintf (hp_name, "%s_hp", pic_name);

  /* allocate memory */

  img_lp = (unsigned char *) calloc (imgsize, 1);
  if ( ! img_lp)
    {
      puts ("calloc for img_lp --> error");
      exit (1);
    }

  unsharp_mask (dim_lp, img, img_lp);

  if (examine == 3)
    {
      fp = fopen (lp_name, "w");  /*  save lowpass image */
      if (tiff_flag) { write_tiff (lp_name, img_lp, imx, imy); }
      else { fwrite (img_lp, 1, imgsize, fp); }
      printf("low pass: %s will be deleted when quitting ptv\n", lp_name);
      fclose (fp);
    }

  /*  subtract lowpass from original  (=> highpass)  */
  for (ptr1=img, ptr2=img_lp, ptr3=img_hp, i=0; i<imgsize;
       ptr1++, ptr2++, ptr3++, i++)
    {
      if (*ptr1 > *ptr2)  *ptr3 = *ptr1 - *ptr2;
      else  *ptr3 = 0;
    }


  /* consider field mode */
  if (field == 1 || field == 2)  split (img_hp, field);


  /* filter highpass image, if wanted */
  switch (filter_hp)
    {
    case 0: break;
    case 1: lowpass_3 (img_hp, img_hp);	break;
    case 2: filter_3 (img_hp, img_hp);	break;
    }

  /* save highpass image for later use */
  if (examine == 3)
    {  fp = fopen (hp_name, "w");
  if (tiff_flag) { write_tiff (hp_name, img_hp, imx, imy); }
  else { fwrite (img_hp, 1, imgsize, fp);}
  fclose (fp);
  }

  free (img_lp);
}


void oddeven (pic_name, img, img_hp, dim_lp, filter_hp, field, nr)
char	        pic_name[256];  /* image name */
unsigned char  *img;
unsigned char  *img_hp;       	/* good lines */
int             dim_lp;	       	/* dimension of subtracted lowpass image */
int	        filter_hp;     	/* flag for additional filtering of _hp */
int    		field;	       	/* field to be used */
int	      	nr;	       	/* image number for display */


{

  register int	i,j;
  unsigned char *img_lp;			/* bad lines */
  int			sumodd=0, sumeven=0;		/* buffer */
  int			histo[256];
  void 	unsharp_mask ();
  void histogram ();
  void histeq ();

   /* allocate memory */
  img_lp = (unsigned char *) calloc (imgsize, 1);
  if ( ! img_lp)
    {
      puts ("calloc for img_lp --> error");
      exit (1);
    }


/* Check what lines should be replaced */
  /* Copy file img_hp = B, img_lp = C */
   copy_images(img,img_lp);
   copy_images(img,img_hp); 

 /* Sum values of odd and even lines */
/* Average over odd lines */
for (i=0; i<imy; i= i+2)
{
	for (j=0; j<imx; j++)
	{
		sumodd = sumodd + img[i*imx + j];
	} /* printf("%d,%d\n",i*imx+j,img[i*imx +j]); */
}
		/* printf("Sum odd lines\t %d",sumodd);*/
/* Average over even lines */
for (i=1; i<imy; i+=2)
{
	for (j=0; j<imx; j++)
	{
	/* sumodd[i] = sumodd[i] + img[i][j]; */
	sumeven = sumeven + img[i*imx + j];
	}
}

/* printf("Sum even lines\t %d",sumeven); */

if (sumodd < sumeven)
{
/* Remove odd lines in B */
	for (i=0; i<imy; i+=2)
{
	for (j=0; j<imx; j++)
	{
		img_hp[i*imx+j] = 0;
	}
}

	/* puts("removed odd lines in img_hp\n"); */

/* Calculate histogram of the even lines in B*/
histogram (img_hp, histo);

/* puts("histogram");

for (i=0;i<256;i++) printf("%d",histo[i]);

puts("\n"); */

/* Remove even lines in C */
	for (i=1; i<imy; i+=2)
{
	for (j=0; j<imx; j++)
	{
		img_lp[i*imx+j] = 0;
	}
}

/* Histogram equalization of C toward the desired histogram of B */
	histeq(img_lp,histo);

/* Copy modified lines from C to B */
	for (i=0; i<imy; i+=2)
{
	for (j=0; j<imx; j++)
	{
		img_hp[i*imx+j] = img_lp[i*imx+j];
	}
}

} /* end of if (sumodd < sumeven)  */
else
{
	{
/* Remove even lines in B */
	for (i=1; i<imy; i+=2)
{
	for (j=0; j<imx; j++)
	{
		img_hp[i*imx+j] = 0;
	}
}
/* Calculate histogram of the odd lines in B*/
histogram (img_hp, histo);

/* puts(" even > odd, histogram");

for (i=0;i<256;i++) printf("%d\t",histo[i]);

puts("\n"); */

/* Remove odd lines in C */
	for (i=0; i<imy; i+=2)
{
	for (j=0; j<imx; j++)
	{
		img_lp[i*imx+j] = 0;
	}
}

/* Histogram equalization of C toward the desired histogram of B */
	histeq(img_lp,histo);

/* Copy modified lines from C to B */
	for (i=1; i<imy; i+=2)
{
	for (j=0; j<imx; j++)
	{
		img_hp[i*imx+j] = img_lp[i*imx+j];
	}
}

} /* end of if (sumodd < sumeven)  */
}

  free (img_lp);
}

void targ_rec (img0, img, par_file, xmin,xmax,ymin,ymax, pix, nr, num)

unsigned char	*img, *img0;   	/* image data, image to be set to zero */
char	       	par_file[];    	/* name of parameter file */
int    		xmin,xmax,ymin,ymax;	/* search area */
target	       	pix[];		       	/* pixel coord array, global */
int	       	nr;		       	/* image number for display */
int	       	*num;	       		/* number of detections */

/*  thresholding and center of gravity with a peak fitting technique  */
/*  uses 4 neighbours for connectivity and 8 to find local maxima     */

{
  register int	i, j, m;
  int	      	n=0, n_wait=0, n_targets=0, sumg, sumg_min;
  int	        numpix;
  int	       	thres, gvthres[4], disco;
  int	       	cr_sz;			       /* size of crosses to be drawn */
  int	       	nnmin,nnmax, nx,ny, nxmin,nxmax, nymin,nymax;
  int	       	xa,ya,xb,yb, x4[4],y4[4], xn,yn;
  double	       	x, y;
  FILE	       	*fpp;

  register unsigned char	gv, gvref;

  targpix	       	waitlist[2048];

  /* read image name, filter dimension and threshold from parameter file */
  fpp = fopen_r (par_file);
  fscanf (fpp, "%d", &gvthres[0]);      /* threshold for binarization 1.image */
  fscanf (fpp, "%d", &gvthres[1]);      /* threshold for binarization 2.image */
  fscanf (fpp, "%d", &gvthres[2]);      /* threshold for binarization 3.image */
  fscanf (fpp, "%d", &gvthres[3]);      /* threshold for binarization 4.image */
  fscanf (fpp, "%d", &disco);    	/* threshold value for discontinuity */
  fscanf (fpp, "%d  %d", &nnmin, &nnmax);  /* min. and max. number of  */
  fscanf (fpp, "%d  %d", &nxmin, &nxmax);	 /* pixels per target, */
  fscanf (fpp, "%d  %d", &nymin, &nymax);  /* abs, in x, in y    */
  fscanf (fpp, "%d", &sumg_min);	       	/* min. sumg	       */
  fscanf (fpp, "%d", &cr_sz);    	    	/* size of crosses     */
  fclose (fpp);

  /* give thres value refering to image number */
  thres=gvthres[nr];

  /*  thresholding and connectivity analysis in image  */

  for (i=ymin; i<ymax; i++)  for (j=xmin; j<xmax; j++)
    {
      gv = *(img + i*imx + j);
      if ( gv > thres)
	if (	gv >= *(img + i*imx + j-1)
		&&	gv >= *(img + i*imx + j+1)
		&&	gv >= *(img + (i-1)*imx + j)
		&&	gv >= *(img + (i+1)*imx + j)
		&&	gv >= *(img + (i-1)*imx + j-1)
		&&	gv >= *(img + (i+1)*imx + j-1)
		&&	gv >= *(img + (i-1)*imx + j+1)
		&&	gv >= *(img + (i+1)*imx + j+1) )

	  /* => local maximum, 'peak' */
	  {
	    yn=i;  xn=j;
	    xn = xn;
	    sumg = gv;  *(img + i*imx + j) = 0;
	    xa = xn;  xb = xn;  ya = yn;  yb = yn;
	    gv -= thres;
	    x = (xn) * gv;
	    y = yn * gv;
	    numpix = 1;
	    waitlist[0].x = j;  waitlist[0].y = i;  n_wait = 1;

	    while (n_wait > 0)
	      {
		gvref = *(img0 + imx*(waitlist[0].y) + (waitlist[0].x));

		x4[0] = waitlist[0].x - 1;  y4[0] = waitlist[0].y;
		x4[1] = waitlist[0].x + 1;  y4[1] = waitlist[0].y;
		x4[2] = waitlist[0].x;  y4[2] = waitlist[0].y - 1;
		x4[3] = waitlist[0].x;  y4[3] = waitlist[0].y + 1;

		for (n=0; n<4; n++)
		  {
		    xn = x4[n];  yn = y4[n];
		    xn = xn;
		    gv = *(img + imx*yn + xn);

		    /* conditions for threshold, discontinuity, image borders */
		    /* and peak fitting */
		    if (   (gv > thres)
			   && (xn>=xmin)&&(xn<xmax) && (yn>=ymin)&&(yn<ymax)
			   && (gv <= gvref+disco)
			   && (gvref + disco >= *(img0 + imx*(yn-1) + xn))
			   && (gvref + disco >= *(img0 + imx*(yn+1) + xn))
			   && (gvref + disco >= *(img0 + imx*yn + (xn-1)))
			   && (gvref + disco >= *(img0 + imx*yn + (xn+1)))  )
		      {
			sumg += gv;  *(img + imx*yn + xn) = 0;
			if (xn < xa)	xa = xn;	if (xn > xb)	xb = xn;
			if (yn < ya)	ya = yn;	if (yn > yb)	yb = yn;
			waitlist[n_wait].x = xn;	waitlist[n_wait].y = yn;
			x = x + (xn) * (gv - thres);
			y = y + yn * (gv - thres);
			numpix++;	n_wait++;
		      }
		  }

		n_wait--;
		for (m=0; m<n_wait; m++)  waitlist[m] = waitlist[m+1];
		waitlist[n_wait].x = 0;  waitlist[n_wait].y = 0;

	      }	/*  end of while-loop  */


	    /* check whether target touches image borders */
	    if (xa==xmin || ya==ymin || xb==xmax-1 || yb==ymax-1)	continue;


	    /* get targets extensions in x and y */
	    nx = xb - xa + 1;  ny = yb - ya + 1;

	    if (   (numpix >= nnmin) && (numpix <= nnmax)
		   && (nx >= nxmin) && (nx <= nxmax)
		   && (ny >= nymin) && (ny <= nymax)
		   && (sumg > sumg_min)			 )
	      {
		pix[n_targets].n = numpix;
		pix[n_targets].nx = nx;
		pix[n_targets].ny = ny;
		pix[n_targets].sumg = sumg;
		sumg -= (numpix*thres);
		x /= sumg;	x += 0.5;	y /= sumg;	y += 0.5;
		pix[n_targets].x = x;
		pix[n_targets].y = y;
		pix[n_targets].pnr = n_targets++;

		xn = x;  yn = y;
		//drawcross (interp, (int) xn, (int) yn, cr_sz, nr, "Blue");

	      }
	  }	/*  end of if-loop  */
    }
  *num = n_targets;
}





void simple_connectivity (img0, img, par_file, xmin,xmax,ymin,ymax, pix, nr, num)
unsigned char	*img, *img0;   	/* image data, image to be set to zero */
char	       	par_file[];    	/* name of parameter file */
int	       	xmin,xmax,ymin,ymax;	/* search area */
target	       	pix[];        	/* pixel coord array, global */
int    	       	nr;    	       	/* image number for display */
int	       	*num;	       	/* number of detections */

/*  thresholding and center of gravity with a peak fitting technique  */
/*  uses 4 neighbours for connectivity and 8 to find local maxima     */

{
  register int	i, j, m;
  int         	n=0, n_wait=0, n_targets=0, sumg, sumg_min;
  int	       	numpix;
  int	       	thres;
  int	      	cr_sz; 	      	/* size of crosses to be drawn */
  int          	nnmin,nnmax, nx,ny, nxmin,nxmax, nymin,nymax;
  int	       	xa,ya,xb,yb, x4[4],y4[4], xn,yn;
  double       	x, y;
  FILE	       	*fpp;

  register unsigned char  gv, gvref;

  targpix	waitlist[2048];

  /* read image name, threshold and shape limits from parameter file */
  fpp = fopen_r (par_file);
  fscanf (fpp, "%d", &thres);    	/* threshold for binarization  	 */
  fscanf (fpp, "%d", &n);		/* threshold value for discontinuity */
  fscanf (fpp, "%d  %d", &nnmin, &nnmax);	 /* min. and max. number of  */
  fscanf (fpp, "%d  %d", &nxmin, &nxmax);	 /* pixels per target, 	*/
  fscanf (fpp, "%d  %d", &nymin, &nymax);	 /* abs, in x, in y   	*/
  fscanf (fpp, "%d", &sumg_min);	       	 /* min. sumg 		*/
  fscanf (fpp, "%d", &cr_sz);		 /* size of crosses	*/
  fclose (fpp);


  /*  thresholding and connectivity analysis in image  */

  for (i=ymin; i<ymax; i++)  for (j=xmin; j<xmax; j++)
    {
      gv = *(img + i*imx + j);
      if ( gv > 2*thres)
	{
	  yn=i;  xn=j;
	  xn = xn;
	  sumg = gv;  *(img + i*imx + j) = 0;
	  xa = xn;  xb = xn;  ya = yn;  yb = yn;
	  gv -= thres;
	  x = (xn) * gv;
	  y = yn * gv;
	  numpix = 1;
	  waitlist[0].x = j;  waitlist[0].y = i;  n_wait = 1;

	  while (n_wait > 0)
	    {
	      gvref = *(img0 + imx*(waitlist[0].y) + (waitlist[0].x));

	      x4[0] = waitlist[0].x - 1;  y4[0] = waitlist[0].y;
	      x4[1] = waitlist[0].x + 1;  y4[1] = waitlist[0].y;
	      x4[2] = waitlist[0].x;  y4[2] = waitlist[0].y - 1;
	      x4[3] = waitlist[0].x;  y4[3] = waitlist[0].y + 1;

	      for (n=0; n<4; n++)
		{
		  xn = x4[n];  yn = y4[n];
		  xn = xn;
		  gv = *(img + imx*yn + xn);

		  /* conditions for threshold, discontinuity, image borders */
		  /* and peak fitting */
		  if (   (gv > thres)
			 && (xn>=xmin)&&(xn<xmax) && (yn>=ymin)&&(yn<ymax)  )
		    {
		      sumg += gv;  *(img + imx*yn + xn) = 0;
		      if (xn < xa)	xa = xn;	if (xn > xb)	xb = xn;
		      if (yn < ya)	ya = yn;	if (yn > yb)	yb = yn;
		      waitlist[n_wait].x = xn;	waitlist[n_wait].y = yn;
		      x = x + (xn ) * (gv - thres);
		      y = y + yn * (gv - thres);
		      numpix++;	n_wait++;
		    }
		}

	      n_wait--;
	      for (m=0; m<n_wait; m++)  waitlist[m] = waitlist[m+1];
	      waitlist[n_wait].x = 0;  waitlist[n_wait].y = 0;

	    }	/*  end of while-loop  */

	  nx = xb - xa + 1;  ny = yb - ya + 1;

	  if (   (numpix >= nnmin) && (numpix <= nnmax)
		 && (nx >= nxmin) && (nx <= nxmax)
		 && (ny >= nymin) && (ny <= nymax)
		 && (sumg > sumg_min)			 )
	    {
	      pix[n_targets].n = numpix;
	      pix[n_targets].nx = nx;
	      pix[n_targets].ny = ny;
	      pix[n_targets].sumg = sumg;
	      sumg -= (numpix*thres);
	      x /= sumg;	x += 0.5;	y /= sumg;	y += 0.5;
	      pix[n_targets].x = x;
	      pix[n_targets].y = y;
	      pix[n_targets].tnr = -1;
	      pix[n_targets].pnr = n_targets++;

	      xn =  x;  yn = y;
	      //drawcross (xn, yn, cr_sz, 8);
	    }
	}	/*  end of if-loop  */
    }

  *num = n_targets;
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  sequence/sequence_main.c                                                                            0000644 0001131 0001131 00000003523 10303615427 014312  0                                                                                                    ustar   amns                            amns                                                                                                                                                                                                                   /**********************************************************************\
 * Name:           track_main 
 *
 * Description:    Entry point for sequencing module of the jw_ptv
 *                 particle tracking utility. Originally, sequencing and
 *                 tracking were performed by the same executable under
 *                 a Tcl/Tk front end.
 *
 *                 This code is part of an executable that contains only
 *                 the sequencing code in command line form.
 *               
 * Author:         Chris Bunney, University of Plymouth
 *
 * Created:        July 2005
\**********************************************************************/

#include "ptv.h"
#include <unistd.h>

int main( int argc, char **argv )
{
	char opt;
	
	useCompression = 0;
	verbose = 0;

	while( ( opt = getopt( argc, argv, "cvh" ) ) != -1 ) {
		switch( opt ) {
			case 'c':
			fprintf( stderr, "Enabling compression\n" );
			useCompression = 1;
			break;

			case 'v':
			fprintf( stderr, "Printing extra verbose output\n" );
			verbose = 1;
			break;
			
			case 'h':
			default:
			printf( "\nUsage: sequence [-cvh]\n\n"
					"Options:\n"
					"   -c\n"
				 	"        Enable gzip compression of output files.\n"
					"        This includes all files written to the res/\n"
					"        directory and img/*_target files.\n\n"
					"        If you run 'sequence' using the -c flag,\n"
					"        make sure you also run 'track' with the -c flag.\n\n"
					"        Note: Reading of gzipped image files is done\n"
					"        implicitly and is not effected by this flag.\n\n"
					"   -v\n"
					"        Prints extra verbose output.\n\n"
					"   -h\n"
					"        Prints this usage informaton.\n\n"
				  );

			return 0;
		}
	}
	
	init_proc_c();
	start_proc_c();
	sequence_proc_c();

	fprintf( stderr, "\n\nComplete\n\n" );

	quit_proc_c();
	return 0;
}
                                                                                                                                                                             track/                                                                                              0000755 0001131 0001131 00000000000 10421530664 010623  5                                                                                                    ustar   amns                            amns                                                                                                                                                                                                                   track/dynamic_track.c                                                                               0000644 0001131 0001131 00000005113 10303614021 013564  0                                                                                                    ustar   amns                            amns                                                                                                                                                                                                                   /**********************************************************************\
 * Name:           dynamicTrack
 *
 * Description:    Reads dynamic tracking parameters used by jw_ptv
 *                 particle tracking.
 *               
 * Author:         Chris Bunney, University of Plymouth
 *
 * Created:        July 2005
\**********************************************************************/


#include "ptv.h"
#include "dynamic_track.h"

void loadTrackingParameters( TParList *tp, int seq_first, int seq_last )
{
	FILE *fp;					// File pointer to dynamic tracking parameters file
	int nTpSize;				// number of tracking parameters to be read from file
	int nRead;					// number of fileds read in line
	int nLine = 0;				// line number in file
	int nCnt = 0;				// number of lines found that we actually want
	int idx;					// image index of current line
	
	if( staticTracking )
		return;
	
	fprintf( stderr, "Loading dynamic tracking file.\n" );

	if( ( fp = fopen( szDynTParFilename, "r" ) ) == NULL ) {
		fprintf( stderr, "Failed to open file tracking parameters file '%s'\n",
				szDynTParFilename );

		return;
	}

	// allocate memory for tracking data array:
	nTpSize = seq_last - seq_first + 1;
	*tp = (trackparameters*) calloc( nTpSize, sizeof( trackparameters ) );

	while( fgets( buf, 256, fp ) ) {

		nLine++;

		// check if we want this line
		sscanf( buf, "%d", &idx );
		if( idx >= seq_first && idx <= seq_last ) {
			nCnt++;
			nRead = sscanf( buf, "%*d %lf %lf %lf %lf %lf %lf %lf %lf %d\n",
					&(*tp)[idx].dvxmin,
					&(*tp)[idx].dvxmax,
					&(*tp)[idx].dvymin,
					&(*tp)[idx].dvymax,
					&(*tp)[idx].dvzmin,
					&(*tp)[idx].dvzmax,
					&(*tp)[idx].dangle,
					&(*tp)[idx].dacc,
					&(*tp)[idx].add
					);

			// check for malformed line:
			if( nRead != 9 )
			{
				fprintf( stderr, "Malformed line in file {%s} at line %d\n\n",
						szDynTParFilename,
						nLine );
				exit( -1 );
			}
		}
	}	

	// check we read the right amount of parameter lines:
	if( nTpSize != nCnt ) {
		fprintf( stderr, "Error! Expected to read %d lines from %s, actually read %d.\n\n",
				nTpSize,
				szDynTParFilename,
				nCnt
			   );
		exit( -1 );
	}
}

void getCurrentTPar( TParList tplist, trackparameters *tp, int step )
{
	int i = step - seq_first;
	
	tp->dvxmin = tplist[i].dvxmin;
	tp->dvxmax = tplist[i].dvxmax;
	tp->dvymin = tplist[i].dvymin;
	tp->dvymax = tplist[i].dvymax;
	tp->dvzmin = tplist[i].dvzmin;
	tp->dvzmax = tplist[i].dvzmax;
	tp->dangle = tplist[i].dangle;
	tp->dacc = tplist[i].dacc;
	tp->add = tplist[i].add;
}

void freeTrackingParameters( TParList tp )
{
	if( !staticTracking )
		free( tp );
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                     track/dynamic_track.h                                                                               0000644 0001131 0001131 00000000461 10274647516 013620  0                                                                                                    ustar   amns                            amns                                                                                                                                                                                                                   #ifndef DYNAMIC_TRACK_H
#define DYNAMIC_TRACK_H

#include "typedefs.h"

typedef trackparameters* TParList;

void loadTrackingParameters( TParList *tp, int seq_first, int seq_last );
void getCurrentTPar( TParList tplist, trackparameters *tp, int step );
void freeTrackingParameters( TParList tp );

#endif
                                                                                                                                                                                                               track/jw_ptv_track.c                                                                                0000644 0001131 0001131 00000025553 10303615021 013464  0                                                                                                    ustar   amns                            amns                                                                                                                                                                                                                   /****************************************************************************
*****************************************************************************

Author/Copyright:      	Hans-Gerd Maas / Jochen Willneff

Address:	            Institute of Geodesy and Photogrammetry
		                ETH - Hoenggerberg
		                CH - 8093 Zurich

Creation Date:	       	took a longer time ...

Description:	       	target detection, correspondences and
		                positioning with tclTk display
					    -> 4 camera version

Routines contained:    	many ...

Amended:                June 2005, Chris Bunney, University of Plymouth.
                        Removed all Tcl/Tk code and implemented a
                        facility for reading a dynamic tracking parameters
                        input file. Also, files can now be read/written
						in compressed form using zlib.

****************************************************************************/
#include "ptv.h"

#define nmax 5120
#define TCL_OK 0
#define TCL_ERROR 1

/*  global declarations for ptv  */
/*-------------------------------------------------------------------------*/

int	n_img;	       		      	/* no of images */
int	hp_flag=0;           	      	/* flag for highpass */
int	tiff_flag=0;           	      	/* flag for tiff header */
int oddevenflag=0;              /* flag for oddeven */
int	chfield;       		       	/* flag for field mode */
int	nfix;	       	       	       	/* no. of control points */
int	num[4];	       		       	/* no. of targets per image */
int numc[4];                        /* no. of targets in current image */
int nump[4];                        /* no. of targets in previous image */
int numn[4];                        /* no. of targets in next image */
int n_trac[4];	           	/* no. of tracks */
int	match=0;		      	/* no. of matches */
int	match2=0;	      	       	/* no. of matches in 2nd pass */
int	nr[4][4];		     	/* point numbers for man. ori */
int	imx, imy, imgsize;	      	/* image size */
int	zoom_x[4],zoom_y[4],zoom_f[4];  /* zoom parameters */
int	pp1=0, pp2=0, pp3=0, pp4=0;   	/* for man. orientation */
int	seq_first, seq_last;	       	/* 1. and last img of seq */
int	demo_nr;		      	/* for demo purposes */
int	examine = 0;		       	/* for more detailed output */
int	dump_for_rdb;		       	/* # of dumpfiles for rdb */
int cr_sz;                          /* size of crosses */
int display;                        /* display flag */
int corp, corc, corn;
int m[4];
int trackallocflag = 0;      /* checkflag if mega, c4, t4 already allocated */

double	pix_x, pix_y;			      	/* pixel size */
double	ro;			      	        /* 200/pi */
double	cn, cnx, cny, csumg, eps0, corrmin;	/* correspondences par */
double 	rmsX, rmsY, rmsZ, mean_sigma0;		/* a priori rms */
double  X_lay[2], Zmin_lay[2], Zmax_lay[2];	/* illu. layer data */

FILE	*fp1, *fp2, *fp3, *fp4, *fpp;
gzFile	fgz;

char	img_name[4][256];      	/* original image names */
char   	img_lp_name[4][256]; 	/* lowpass image names */
char   	img_hp_name[4][256];   	/* highpass image names */
char   	img_cal[4][128];       	/* calibrayion image names */
char   	img_ori[4][128];       	/* image orientation data */
char   	img_ori0[4][128];      	/* orientation approx. values */
char   	img_addpar[4][128];    	/* image additional parameters */
char   	img_addpar0[4][128];   	/* ap approx. values */
char   	seq_name[4][128];      	/* sequence names */
char   	track_dir[128];	       	/* directory with dap track data */
char    fixp_name[128];
char   	res_name[128];	      	/* result destination */
char   	filename[128];	      	/* for general use */
char   	buf[256], val[256];	       	/* buffer */

unsigned char	*img[4];      	/* image data */
unsigned char	*img0[4];      	/* image data for filtering etc */
unsigned char	*zoomimg;     	/* zoom image data */

Exterior       	Ex[4];	      	/* exterior orientation */
Interior       	I[4];	       	/* interior orientation */
ap_52	       	ap[4];	       	/* add. parameters k1,k2,k3,p1,p2,scx,she */
mm_np	       	mmp;	       	/* n-media parameters */
target	       	pix[4][nmax]; 	/* target pixel data */
target	       	pix0[4][4];    	/* pixel data for man_ori points */

target          *t4[4][4];
int             nt4[4][4];

coord_2d       	crd[4][nmax];  	/* (distorted) metric coordinates */
coord_2d       	geo[4][nmax];  	/* corrected metric coordinates */
coord_3d       	fix[4096];     	/* testfield points coordinates */
n_tupel	       	con[nmax];     	/* list of correspondences */

corres	       	*c4[4];
trackparameters tpar;           /* tracking parameters */

mm_LUT	       	mmLUT[4];     	/* LUT for multimedia radial displacement */
coord_3d        *p_c3d;
P *mega[4];

/* ChrisB added: */
char szDynTParFilename[128];		/* Filename for dynamic tracking parameters*/
int useCompression;					/* Compressed input/output flag. */
int staticTracking;					/* Static/dynamic tracking flag. */
int verbose;						/* Verbosity flag */

/***************************************************************************/

int init_proc_c(int argc, const char** argv)
{
  int  i;
  const char *valp;

  if( verbose ) puts ("\nMultimedia Particle Positioning and Tracking\n");

  // ChrisB: Do we need this? no....
  //valp = Tcl_GetVar(interp, "examine",  TCL_GLOBAL_ONLY);
  //examine = atoi (valp);

  ro = 200/M_PI;

  /*  read from main parameter file  */
  fpp = fopen_r ("parameters/ptv.par");

  fscanf (fpp, "%d\n", &n_img);

  for (i=0; i<4; i++)
    {
      fscanf (fpp, "%s\n", img_name[i]);
      fscanf (fpp, "%s\n", img_cal[i]);
    }
  fscanf (fpp, "%d\n", &oddevenflag);
  fscanf (fpp, "%d\n", &hp_flag);
  fscanf (fpp, "%d\n", &tiff_flag);
  fscanf (fpp, "%d\n", &imx);
  fscanf (fpp, "%d\n", &imy);
  fscanf (fpp, "%lf\n", &pix_x);
  fscanf (fpp, "%lf\n", &pix_y);
  fscanf (fpp, "%d\n", &chfield);
  fscanf (fpp, "%lf\n", &mmp.n1);
  fscanf (fpp, "%lf\n", &mmp.n2[0]);
  fscanf (fpp, "%lf\n", &mmp.n3);
  fscanf (fpp, "%lf\n", &mmp.d[0]);
  fclose (fpp);

  /* read illuminated layer data */
  fpp = fopen_r ("parameters/criteria.par");
  fscanf (fpp, "%lf\n", &X_lay[0]);
  fscanf (fpp, "%lf\n", &Zmin_lay[0]);
  fscanf (fpp, "%lf\n", &Zmax_lay[0]);
  fscanf (fpp, "%lf\n", &X_lay[1]);
  fscanf (fpp, "%lf\n", &Zmin_lay[1]);
  fscanf (fpp, "%lf\n", &Zmax_lay[1]);
  fscanf (fpp, "%lf", &cnx);
  fscanf (fpp, "%lf", &cny);
  fscanf (fpp, "%lf", &cn);
  fscanf (fpp, "%lf", &csumg);
  fscanf (fpp, "%lf", &corrmin);
  fscanf (fpp, "%lf", &eps0);
  fclose (fpp);

  mmp.nlay = 1;

  /* read sequence parameters (needed for some demos) */

  fpp = fopen_r ("parameters/sequence.par");

  for (i=0; i<4; i++)		fscanf (fpp, "%s\n", seq_name[i]);
  fscanf (fpp,"%d\n", &seq_first);
  fscanf (fpp,"%d\n", &seq_last);
  fclose (fpp);

  /* initialize zoom parameters and image positions */
  for (i=0; i<n_img; i++)
    {
      num[i] = 0;
      zoom_x[i] = imx/2; zoom_y[i] = imy/2; zoom_f[i] = 1;
    }

  imgsize = imx*imy;

  /* allocate memory for images */
  for (i=0; i<n_img; i++)
    {
      img[i] = (unsigned char *) calloc (imgsize, 1);
      if ( ! img[i])
	{
	  printf ("calloc for img%d --> error\n", i);
	  exit (1);
	}
    }

  for (i=0; i<n_img; i++)
    {
      img0[i] = (unsigned char *) calloc (imgsize, 1);
      if ( ! img0[i])
	{
	  printf ("calloc for img0%d --> error\n", i);
	  exit (1);
	}
    }

  // TODO: remove references to zooming some time as we do not need it.
  zoomimg = (unsigned char *) calloc (imgsize, 1);
  if ( ! zoomimg)
    {
      printf ("calloc for zoomimg --> error\n");
      return TCL_ERROR;
    }

  //parameter_panel_init(/*interp*/);
  // ChrisB: size of crosses - not important.
  cr_sz = 4;//atoi(Tcl_GetVar2(interp, "mp", "pcrossize",  TCL_GLOBAL_ONLY));

  display = 1;
  return TCL_OK;

}

#if 1	// we seemingly do need this for tracking.........
int start_proc_c(/*ClientData clientData, Tcl_Interp* interp,*/ int argc, const char** argv)
{
  int  i, k;

  /*  read from main parameter file  */
  fpp = fopen_r ("parameters/ptv.par");

  fscanf (fpp, "%d\n", &n_img);

  for (i=0; i<4; i++)
    {
      fscanf (fpp, "%s\n", img_name[i]);
      fscanf (fpp, "%s\n", img_cal[i]);
    }
  fscanf (fpp, "%d\n", &oddevenflag);
  fscanf (fpp, "%d\n", &hp_flag);
  fscanf (fpp, "%d\n", &tiff_flag);
  fscanf (fpp, "%d\n", &imx);
  fscanf (fpp, "%d\n", &imy);
  fscanf (fpp, "%lf\n", &pix_x);
  fscanf (fpp, "%lf\n", &pix_y);
  fscanf (fpp, "%d\n", &chfield);
  fscanf (fpp, "%lf\n", &mmp.n1);
  fscanf (fpp, "%lf\n", &mmp.n2[0]);
  fscanf (fpp, "%lf\n", &mmp.n3);
  fscanf (fpp, "%lf\n", &mmp.d[0]);
  fclose (fpp);

  /* read illuminated layer data */
  fpp = fopen_r ("parameters/criteria.par");
  fscanf (fpp, "%lf\n", &X_lay[0]);
  fscanf (fpp, "%lf\n", &Zmin_lay[0]);
  fscanf (fpp, "%lf\n", &Zmax_lay[0]);
  fscanf (fpp, "%lf\n", &X_lay[1]);
  fscanf (fpp, "%lf\n", &Zmin_lay[1]);
  fscanf (fpp, "%lf\n", &Zmax_lay[1]);
  fscanf (fpp, "%lf", &cnx);
  fscanf (fpp, "%lf", &cny);
  fscanf (fpp, "%lf", &cn);
  fscanf (fpp, "%lf", &csumg);
  fscanf (fpp, "%lf", &corrmin);
  fscanf (fpp, "%lf", &eps0);
  fclose (fpp);

  mmp.nlay = 1;

  /* read sequence parameters (needed for some demos) */

  fpp = fopen_r ("parameters/sequence.par");

  for (i=0; i<4; i++)		fscanf (fpp, "%s\n", seq_name[i]);
  fscanf (fpp,"%d\n", &seq_first);
  fscanf (fpp,"%d\n", &seq_last);
  fclose (fpp);

  /*  create file names  */
  for (i=0; i<n_img; i++)
    {
      strcpy (img_lp_name[i], img_name[i]); strcat (img_lp_name[i], "_lp");
      strcpy (img_hp_name[i], img_name[i]); strcat (img_hp_name[i], "_hp");
      strcpy (img_ori[i], img_cal[i]);  strcat (img_ori[i], ".ori");
      strcpy (img_addpar[i], img_cal[i]); strcat (img_addpar[i],".addpar");
    }

  /*  read orientation and additional parameters  */
  for (i=0; i<n_img; i++)
    {
      read_ori (&Ex[i], &I[i], img_ori[i]);
      rotation_matrix (Ex[i], Ex[i].dm);

      fp1 = fopen_r (img_addpar[i]);
      fscanf (fp1,"%lf %lf %lf %lf %lf %lf %lf",
	      &ap[i].k1, &ap[i].k2, &ap[i].k3, &ap[i].p1, &ap[i].p2,
	      &ap[i].scx, &ap[i].she);
      fclose (fp1);
    }

  /* read and display original images */
  // ChrisB: or in our case, just read them.
  for (i=0; i<n_img; i++)
    {
      /* reading */
      //sprintf(val, "camcanvas %d", i+1);
      //Tcl_Eval(interp, val);

      //read_image (/*interp,*/ img_name[i], img[i]);
      //sprintf(val, "newimage %d", i+1);

      //Tcl_Eval(interp, val);
      //sprintf(val, "keepori %d", i+1);
      //Tcl_Eval(interp, val);
    }

  if (!trackallocflag)
    {
      for (i=0; i<4; i++)
	{
	  mega[i]=(P *) calloc(sizeof(P),M);
	  c4[i]=(corres *) calloc(sizeof(corres),M);
	  for (k=0; k<4; k++) { t4[i][k]=(target *) calloc(sizeof (target),M);}
	}
      trackallocflag=1;
    }

  return TCL_OK;

}
#endif

/* ********************************* */


int quit_proc_c ( )
{
	int i, k;

	for (i=0; i<n_img; i++) { free (img[i]); free (img0[i]); }
	free (zoomimg);

	/* delete unneeded files */
	for (i=0; i<n_img; i++)      	k = remove (img_lp_name[i]);
	return TCL_OK;
}
                                                                                                                                                     track/orientation.c                                                                                 0000644 0001131 0001131 00000031540 10274653331 013330  0                                                                                                    ustar   amns                            amns                                                                                                                                                                                                                   /*******************************************************************

Routine:      	orientation.c

Author/Copyright:      	Hans-Gerd Maas

Address:      	Institute of Geodesy and Photogrammetry
                ETH - Hoenggerberg
	       	CH - 8093 Zurich

Creation Date:	summer 1988

Description:   	computes 6 parameters of exterior orientation
	     	and different sets of additional parameters
	      	from given approximate values, fixpoint- and image
	    	coordinates in ASCII files

Functiones used:   	img_affin, rotation_matrix, adjlib.a

Routines contained:

Related routines:

******************************************************************/
#include "ptv.h"

void orient (/*interp,*/ Ex0, I0, ap0, mm, nfix, fix, crd, Ex, I, ap, nr)
//Tcl_Interp*     interp;
Exterior	Ex0, *Ex;	/* exterior orientation, approx and result */
Interior	I0, *I;		/* interior orientation, approx and result */
ap_52		ap0, *ap;	/* add. parameters, approx and result */
mm_np		mm;    		/* multimedia parameters */
int	       	nfix;		/* # of object points */
coord_3d	fix[];		/* object point data */
coord_2d	crd[];		/* image coordinates */
int	       	nr;  		/* image number for residual display */
{
  int  	i,j,n, itnum, stopflag, n_obs=0;
  int  	useflag, ccflag, scxflag, sheflag, xhflag, yhflag,
    k1flag, k2flag, k3flag, p1flag, p2flag;
  int  	intx1, intx2, inty1, inty2;
  double       	dm = 0.00001,  drad = 0.0000001;
  double       	X[1800][16], Xh[1800][16], y[1800], yh[1800], ident[10],
    XPX[16][16], XPy[16], beta[16], Xbeta[1800],
    resi[1800], omega=0, sigma0, sigmabeta[16],
    P[1800], p, sumP, pixnr[3600];
  double 	Xp, Yp, Zp, xp, yp, xpd, ypd, r, qq;
  FILE 	*fp1;

  /* read, which parameters shall be used */
  fp1 = fopen_r ("parameters/orient.par");
  fscanf (fp1,"%d", &useflag);
  fscanf (fp1,"%d", &ccflag);
  fscanf (fp1,"%d", &xhflag);
  fscanf (fp1,"%d", &yhflag);
  fscanf (fp1,"%d", &k1flag);
  fscanf (fp1,"%d", &k2flag);
  fscanf (fp1,"%d", &k3flag);
  fscanf (fp1,"%d", &p1flag);
  fscanf (fp1,"%d", &p2flag);
  fscanf (fp1,"%d", &scxflag);
  fscanf (fp1,"%d", &sheflag);
  fclose (fp1);


  /* init X, y (set to zero) */
  for (i=0; i<1800; i++)
    {
      for (j=0; j<16; j++)  X[i][j] = 0;
      y[i] = 0;  P[i] = 0;
    }

  /* init identities */
  ident[0] = I0.cc;  ident[1] = I0.xh;  ident[2] = I0.yh;
  ident[3]=ap0.k1; ident[4]=ap0.k2; ident[5]=ap0.k3;
  ident[6]=ap0.p1; ident[7]=ap0.p2;
  ident[8]=ap0.scx; ident[9]=ap0.she;

  /* main loop, program runs through it, until none of the beta values
     comes over a threshold and no more points are thrown out
     because of their residuals */

  if( verbose ) puts ("\n\nbegin of iterations");
  itnum = 0;  stopflag = 0;
  while ((stopflag == 0) && (itnum < 20))
    {
      if( verbose ) printf ("\n\n%2d. iteration\n", ++itnum);

      for (i=0, n=0; i<nfix; i++)  if (crd[i].pnr == fix[i].pnr)
	{
	  /* use only certain points as control points */
	  switch (useflag)
	    {
	    case 1: if ((fix[i].pnr % 2) == 0)  continue;  break;
	    case 2: if ((fix[i].pnr % 2) != 0)  continue;  break;
	    case 3: if ((fix[i].pnr % 3) == 0)  continue;  break;
	    }

	  /* check for correct correspondence */
	  if (crd[i].pnr != fix[i].pnr)	continue;

	  /* do not use the corner points of plate 85 */
	  if (nfix == 85  &&  fix[i].pnr == 1)	continue;
	  if (nfix == 85  &&  fix[i].pnr == 7)	continue;
	  if (nfix == 85  &&  fix[i].pnr == 43)	continue;
	  if (nfix == 85  &&  fix[i].pnr == 49)	continue;


	  pixnr[n/2] = i;		/* for drawing residuals */
	  Xp = fix[i].x;  Yp = fix[i].y;  Zp = fix[i].z;
	  rotation_matrix (Ex0, Ex0.dm);
	  img_coord (Xp,Yp,Zp, Ex0,I0, ap0, mm, &xp,&yp);


	  /* derivatives of add. parameters */

	  r = sqrt (xp*xp + yp*yp);

	  X[n][7] = ap0.scx;
	  X[n+1][7] = sin(ap0.she);

	  X[n][8] = 0;
	  X[n+1][8] = 1;

	  X[n][9] = ap0.scx * xp * r*r;
	  X[n+1][9] = yp * r*r;

	  X[n][10] = ap0.scx * xp * pow(r,4.0);
	  X[n+1][10] = yp * pow(r,4.0);

	  X[n][11] = ap0.scx * xp * pow(r,6.0);
	  X[n+1][11] = yp * pow(r,6.0);

	  X[n][12] = ap0.scx * (2*xp*xp + r*r);
	  X[n+1][12] = 2 * xp * yp;

	  X[n][13] = 2 * ap0.scx * xp * yp;
	  X[n+1][13] = 2*yp*yp + r*r;

	  qq =  ap0.k1*r*r; qq += ap0.k2*pow(r,4.0);
	  qq += ap0.k3*pow(r,6.0);
	  qq += 1;
	  X[n][14] = xp * qq + ap0.p1 * (r*r + 2*xp*xp) + 2*ap0.p2*xp*yp;
	  X[n+1][14] = 0;

	  X[n][15] = -cos(ap0.she) * yp;
	  X[n+1][15] = -sin(ap0.she) * yp;



	  /* numeric derivatives */

	  Ex0.x0 += dm;
	  img_coord (Xp,Yp,Zp, Ex0,I0, ap0, mm, &xpd,&ypd);
	  X[n][0]      = (xpd - xp) / dm;
	  X[n+1][0] = (ypd - yp) / dm;
	  Ex0.x0 -= dm;

	  Ex0.y0 += dm;
	  img_coord (Xp,Yp,Zp, Ex0,I0, ap0, mm, &xpd,&ypd);
	  X[n][1]      = (xpd - xp) / dm;
	  X[n+1][1] = (ypd - yp) / dm;
	  Ex0.y0 -= dm;

	  Ex0.z0 += dm;
	  img_coord (Xp,Yp,Zp, Ex0,I0, ap0, mm, &xpd,&ypd);
	  X[n][2]      = (xpd - xp) / dm;
	  X[n+1][2] = (ypd - yp) / dm;
	  Ex0.z0 -= dm;

	  Ex0.omega += drad;
	  rotation_matrix (Ex0, Ex0.dm);
	  img_coord (Xp,Yp,Zp, Ex0,I0, ap0, mm, &xpd,&ypd);
	  X[n][3]      = (xpd - xp) / drad;
	  X[n+1][3] = (ypd - yp) / drad;
	  Ex0.omega -= drad;

	  Ex0.phi += drad;
	  rotation_matrix (Ex0, Ex0.dm);
	  img_coord (Xp,Yp,Zp, Ex0,I0, ap0, mm, &xpd,&ypd);
	  X[n][4]      = (xpd - xp) / drad;
	  X[n+1][4] = (ypd - yp) / drad;
	  Ex0.phi -= drad;

	  Ex0.kappa += drad;
	  rotation_matrix (Ex0, Ex0.dm);
	  img_coord (Xp,Yp,Zp, Ex0,I0, ap0, mm, &xpd,&ypd);
	  X[n][5]      = (xpd - xp) / drad;
	  X[n+1][5] = (ypd - yp) / drad;
	  Ex0.kappa -= drad;

	  I0.cc += dm;
	  rotation_matrix (Ex0, Ex0.dm);
	  img_coord (Xp,Yp,Zp, Ex0,I0, ap0, mm, &xpd,&ypd);
	  X[n][6]      = (xpd - xp) / dm;
	  X[n+1][6] = (ypd - yp) / dm;
	  I0.cc -= dm;

	  y[n]   = crd[i].x - xp;
	  y[n+1] = crd[i].y - yp;

	  n += 2;
	}
      n_obs = n;


      /* identities */

      for (i=0; i<10; i++)  X[n_obs+i][6+i] = 1;

      y[n_obs+0] = ident[0] - I0.cc;		y[n_obs+1] = ident[1] - I0.xh;
      y[n_obs+2] = ident[2] - I0.yh;		y[n_obs+3] = ident[3] - ap0.k1;
      y[n_obs+4] = ident[4] - ap0.k2;		y[n_obs+5] = ident[5] - ap0.k3;
      y[n_obs+6] = ident[6] - ap0.p1;		y[n_obs+7] = ident[7] - ap0.p2;
      y[n_obs+8] = ident[8] - ap0.scx;	y[n_obs+9] = ident[9] - ap0.she;



      /* weights */
      for (i=0; i<n_obs; i++)  P[i] = 1;
      if ( ! ccflag)  P[n_obs+0] = 1e20;
      if ( ! xhflag)  P[n_obs+1] = 1e20;
      if ( ! yhflag)  P[n_obs+2] = 1e20;
      if ( ! k1flag)  P[n_obs+3] = 1e20;
      if ( ! k2flag)  P[n_obs+4] = 1e20;
      if ( ! k3flag)  P[n_obs+5] = 1e20;
      if ( ! p1flag)  P[n_obs+6] = 1e20;
      if ( ! p2flag)  P[n_obs+7] = 1e20;
      if ( ! scxflag) P[n_obs+8] = 1e20;
      if ( ! sheflag) P[n_obs+9] = 1e20;


      n_obs += 10;  sumP = 0;
      for (i=0; i<n_obs; i++)	       	/* homogenize */
	{
	  p = sqrt (P[i]);
	  for (j=0; j<16; j++)  Xh[i][j] = p * X[i][j];
	  yh[i] = p * y[i];  sumP += P[i];
	}



      /* Gauss Markoff Model */

      ata (Xh, XPX, n_obs, 16);

      matinv (XPX, 16);

      atl (XPy, Xh, yh, n_obs, 16);

      matmul (beta, XPX, XPy, 16,16,1);

      stopflag = 1;
      if( verbose ) puts ("\n==> beta :\n");
      for (i=0; i<16; i++)
	{
	  if( verbose ) printf ("%10.6f  ",beta[i]);
	  if (fabs (beta[i]) > 0.01)  stopflag = 0;	/* more iterations */
	}
      if( verbose ) printf ("\n\n");

      Ex0.x0 += beta[0];  Ex0.y0 += beta[1];  Ex0.z0 += beta[2];
      Ex0.omega += beta[3];  Ex0.phi += beta[4];  Ex0.kappa += beta[5];
      I0.cc += beta[6];  I0.xh += beta[7];  I0.yh += beta[8];
      ap0.k1 += beta[9];  ap0.k2 += beta[10];  ap0.k3 += beta[11];
      ap0.p1 += beta[12];  ap0.p2 += beta[13];
      ap0.scx += beta[14];  ap0.she += beta[15];
    }



  /* compute residuals etc. */

  matmul ( Xbeta, X, beta, n_obs, 16, 1);

  omega = 0;
  for (i=0; i<n_obs; i++)
    {
      resi[i] = Xbeta[i] - y[i];  omega += resi[i] * P[i] * resi[i];
    }
  sigma0 = sqrt (omega / (n_obs - 16));

  for (i=0; i<16; i++)  sigmabeta[i] = sigma0 * sqrt(XPX[i][i]);


  /* correlations between parameters */
  if (examine)	for (i=0; i<16; i++)
    {
      for (j=0; j<16; j++)
		if( verbose ) { 
			printf ("%6.2f",
			XPX[i][j] / (sqrt(XPX[i][i]) * sqrt(XPX[j][j])));
		  printf ("\n");
		}
    }


  /* print results */
  if( verbose ) {
	  printf ("\n|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||");
	  printf ("\n\nResults after %d iterations:\n\n", itnum);
	  printf ("sigma0 = %6.2f micron\n", sigma0*1000);
	  printf ("X0 =    %8.3f   +/- %8.3f\n", Ex0.x0, sigmabeta[0]);
	  printf ("Y0 =    %8.3f   +/- %8.3f\n", Ex0.y0, sigmabeta[1]);
	  printf ("Z0 =    %8.3f   +/- %8.3f\n", Ex0.z0, sigmabeta[2]);
	  printf ("omega = %8.4f   +/- %8.4f\n", Ex0.omega*ro, sigmabeta[3]*ro);
	  printf ("phi   = %8.4f   +/- %8.4f\n", Ex0.phi*ro, sigmabeta[4]*ro);
	  printf ("kappa = %8.4f   +/- %8.4f\n", Ex0.kappa*ro, sigmabeta[5]*ro);
	  printf ("camera const  = %8.5f   +/- %8.5f\n", I0.cc, sigmabeta[6]);
	  printf ("xh            = %8.5f   +/- %8.5f\n", I0.xh, sigmabeta[7]);
	  printf ("yh            = %8.5f   +/- %8.5f\n", I0.yh, sigmabeta[8]);
	  printf ("k1            = %8.5f   +/- %8.5f\n", ap0.k1, sigmabeta[9]);
	  printf ("k2            = %8.5f   +/- %8.5f\n", ap0.k2, sigmabeta[10]);
	  printf ("k3            = %8.5f   +/- %8.5f\n", ap0.k3, sigmabeta[11]);
	  printf ("p1            = %8.5f   +/- %8.5f\n", ap0.p1, sigmabeta[12]);
	  printf ("p2            = %8.5f   +/- %8.5f\n", ap0.p2, sigmabeta[13]);
	  printf ("scale for x'  = %8.5f   +/- %8.5f\n", ap0.scx, sigmabeta[14]);
	  printf ("shearing      = %8.5f   +/- %8.5f\n", ap0.she*ro, sigmabeta[15]*ro);
  }

  /* show original images with residual vectors (requires globals) */
  sprintf (val, "%d: %5.2f micron, ", nr+1, sigma0*1000);
  strcat(buf,val);

  read_image (/*interp,*/ img_name[nr], img[nr]);
  sprintf(val, "newimage %d", nr+1);
  //Tcl_Eval(interp, val);

  if( verbose ) puts (buf);


  for (i=0; i<n_obs-10; i+=2)
    {
      n = pixnr[i/2];
      intx1 = (int) pix[nr][n].x;
      inty1 = (int) pix[nr][n].y;
      intx2 = intx1 + resi[i]*5000;
      inty2 = inty1 + resi[i+1]*5000;

      //drawcross (interp, intx1, inty1, 3, nr , "orange");
      //drawvector (interp, intx1, inty1, intx2, inty2, 1, nr , "red");
    }


  if (stopflag)
    {
      rotation_matrix (Ex0, Ex0.dm);
      *Ex = Ex0;	*I = I0;	*ap = ap0;
    }
  else{
  	  if( verbose ) puts ("orientation does not converge");
  }
}




void raw_orient (Ex0, I, ap, mm, nfix, fix, crd, Ex)
Exterior  Ex0, *Ex;
Interior  I;
ap_52	  ap;
mm_np	  mm;
int	  nfix;
coord_3d  fix[];
coord_2d  crd[];
{
  double		X[8][6], y[8],
    XPX[6][6], XPy[6], beta[6];
  double 		Xp, Yp, Zp, xp, yp, xpd, ypd;
  int     	i,j,n, itnum, stopflag, n_obs=0;
  double		dm = 0.0001,  drad = 0.000001;

  /* init X, y (set to zero) */
  for (i=0; i<8; i++)
    {
      for (j=0; j<6; j++)  X[i][j] = 0;
      y[i] = 0;
    }

  ap.k1 = 0;	ap.k2 = 0;	ap.k3 = 0;	ap.p1 = 0;	ap.p2 = 0;
  ap.scx = 1; ap.she = 0;


  /* main loop, program runs through it, until none of the beta values
     comes over a threshold and no more points are thrown out
     because of their residuals */

  itnum = 0;  stopflag = 0;
  while ((stopflag == 0) && (itnum < 20))
    {
      ++itnum;

      for (i=0, n=0; i<nfix; i++)  if (crd[i].x != -999)
	{
	  Xp = fix[i].x;  Yp = fix[i].y;  Zp = fix[i].z;
	  rotation_matrix (Ex0, Ex0.dm);

	  img_coord (Xp,Yp,Zp, Ex0,I, ap, mm, &xp,&yp);

	  /* numeric derivatives */

	  Ex0.x0 += dm;
	  img_coord (Xp,Yp,Zp, Ex0,I, ap, mm, &xpd,&ypd);
	  X[n][0]      = (xpd - xp) / dm;
	  X[n+1][0] = (ypd - yp) / dm;
	  Ex0.x0 -= dm;

	  Ex0.y0 += dm;
	  img_coord (Xp,Yp,Zp, Ex0,I, ap, mm, &xpd,&ypd);
	  X[n][1]	  = (xpd - xp) / dm;
	  X[n+1][1] = (ypd - yp) / dm;
	  Ex0.y0 -= dm;

	  Ex0.z0 += dm;
	  img_coord (Xp,Yp,Zp, Ex0,I, ap, mm, &xpd,&ypd);
	  X[n][2]	  = (xpd - xp) / dm;
	  X[n+1][2] = (ypd - yp) / dm;
	  Ex0.z0 -= dm;

	  Ex0.omega += drad;
	  rotation_matrix (Ex0, Ex0.dm);
	  img_coord (Xp,Yp,Zp, Ex0,I, ap, mm, &xpd,&ypd);
	  X[n][3]	  = (xpd - xp) / drad;
	  X[n+1][3] = (ypd - yp) / drad;
	  Ex0.omega -= drad;

	  Ex0.phi += drad;
	  rotation_matrix (Ex0, Ex0.dm);
	  img_coord (Xp,Yp,Zp, Ex0,I, ap, mm, &xpd,&ypd);
	  X[n][4]	  = (xpd - xp) / drad;
	  X[n+1][4] = (ypd - yp) / drad;
	  Ex0.phi -= drad;

	  Ex0.kappa += drad;
	  rotation_matrix (Ex0, Ex0.dm);
	  img_coord (Xp,Yp,Zp, Ex0,I, ap, mm, &xpd,&ypd);
	  X[n][5]	  = (xpd - xp) / drad;
	  X[n+1][5] = (ypd - yp) / drad;
	  Ex0.kappa -= drad;

	  y[n]   = crd[i].x - xp;
	  y[n+1] = crd[i].y - yp;

	  n += 2;
	}
      n_obs = n;

      /* Gauss Markoff Model */

      ata (X, XPX, n_obs, 6);
      matinv (XPX, 6);
      atl (XPy, X, y, n_obs, 6);
      matmul (beta, XPX, XPy, 6,6,1);

      stopflag = 1;
      for (i=0; i<6; i++)  if (fabs (beta[i]) > 0.1)  stopflag = 0;

      Ex0.x0 += beta[0];  Ex0.y0 += beta[1];  Ex0.z0 += beta[2];
      Ex0.omega += beta[3];  Ex0.phi += beta[4];
      Ex0.kappa += beta[5];
    }

  if (stopflag)
    {
      *Ex = Ex0;
      rotation_matrix (*Ex, Ex->dm);
    }
  else { if(verbose) puts ("raw orientation impossible"); }
}
                                                                                                                                                                track/ptv.c                                                                                         0000644 0001131 0001131 00000110174 10303614743 011604  0                                                                                                    ustar   amns                            amns                                                                                                                                                                                                                   /*******************************************************************************
**
** Title: ptv
**
** Author: Heinrich Stueer
**
** Description: Main modul of track.
**Dies ist eine abgespeckte Version vom Malik und Papantoniou (allerdings mit
**ein paar Anderungen)
** Created: 12.02.1998
** Changes: July 2005, Chris Bunney, University of Plymouth.
**          Removed all Tcl/Tk code and implemented facility for reading and
**          writing compressed input/output using zlib to save disk space.
**
*******************************************************************************/
#include "ptv.h"

//int seq_track_proc_c(ClientData clientData, Tcl_Interp* interp, int argc, const char** argv)
int seq_track_proc_c( int argc, const char** argv)
{
  int step, i, k;

  /*Alloc space*/
  for (i=0; i<4; i++)
    mega[i]=(P *) calloc(sizeof(P),M);

  for (i=0; i<4; i++)
    c4[i]=(corres *) calloc(sizeof(corres),M);

  for (i=0; i<4; i++)
    for (k=0; k<n_img; k++)
      t4[i][k]=(target *) calloc(sizeof (target),M);

  readseqtrackcrit ();
  /*load again first data sets*/
  step = seq_first;
  read_ascii_data(step);
  rotate_dataset();
  read_ascii_data(step+1);
  rotate_dataset();
  read_ascii_data(step+2);

  for(step = (seq_first+2); step < seq_last; step++)
    {
      tracking(/*clientData, interp,*/ argc, argv);
      rotate_dataset();
      write_ascii_data(step-2);
      read_ascii_data(step+1);
    }

  /*write last data_sets*/

  tracking(/*clientData, interp,*/ argc, argv);
  rotate_dataset();
  write_ascii_data(step-2);
  rotate_dataset();
  write_ascii_data(step-1);


  for (i=0; i<4; i++)
    { free (mega[i]);free (c4[i]);
    for (k=0; k<n_img; k++) free (t4[i][k]);
    }

  return 0;
}


void read_ascii_data(int filenumber)
{
  FILE	*FILEIN;
  gzFile fileGz;
  char	filein[256];
  int	i, j;
  int   dumy;

  for(i=0;i<M;i++)
    {
      /*reset all other variable to default value*/
      mega[3][i].prev = -1;
      mega[3][i].next = -2;
      mega[3][i].prio = 4;
      mega[3][i].inlist = 0;
      mega[3][i].finaldecis = 1000000.0;
      c4[3][i].p[0]=-1;
      c4[3][i].p[1]=-1;
      c4[3][i].p[2]=-1;
      c4[3][i].p[3]=-1;
    }

  // ChrisB: ??? why not just do: spintf( filein, "res/rt_is.%03ld", filenumber ) ????!!!
  if (filenumber < 10)        sprintf (filein, "res/rt_is.00%1d", filenumber);
  else if (filenumber < 100)  sprintf (filein, "res/rt_is.0%2d",  filenumber);
  else       sprintf (filein, "res/rt_is.%3d", filenumber);

  if( useCompression ) {
	  sprintf( filename, "%s.gz", filein );
	  fileGz = gzopen (filename, "rb");
	  if (! fileGz) { 
		  fprintf( stderr, "Can't open gzip file: %s\n", filein);
		  fflush( stderr );
	  }

	  i=0;
	  m[3]=0;

	  gzgets( fileGz, buf, 256 );
	  sscanf(buf, "%d\n", &dumy); /* read # of 3D points on dumy */

	  do
	  {
		  /*read dataset row by row, x,y,z and correspondences */
		  gzgets( fileGz, buf, 256 );
		  sscanf( buf, "%d %f %f %f %d %d %d %d\n",
				  &dumy, &mega[3][i].x[0], &mega[3][i].x[1], &mega[3][i].x[2],
				  &c4[3][i].p[0], &c4[3][i].p[1], &c4[3][i].p[2], &c4[3][i].p[3]);

		  c4[3][i].nr=i;

		  for(j=0; j<POSI; j++){
			  mega[3][i].decis[j] = 0.0;
			  mega[3][i].linkdecis[j] = -999;
		  }
		  i++;
		  m[3]++;
	  }while(!gzeof(fileGz));

	  gzclose(fileGz);
  } else {
	  FILEIN = fopen (filein, "r");
	  if (! FILEIN) { 
		  fprintf( stderr, "Can't open ascii file: %s\n", filein);
		  fflush( stderr );
	  }

	  i=0;
	  m[3]=0;

	  fscanf(FILEIN, "%d\n", &dumy); /* read # of 3D points on dumy */
	  do
	  {
		  /*read dataset row by row, x,y,z and correspondences */
		  fscanf(FILEIN, "%d %f %f %f %d %d %d %d\n",
				  &dumy, &mega[3][i].x[0], &mega[3][i].x[1], &mega[3][i].x[2],
				  &c4[3][i].p[0], &c4[3][i].p[1], &c4[3][i].p[2], &c4[3][i].p[3]);

		  c4[3][i].nr=i;

		  for(j=0; j<POSI; j++){
			  mega[3][i].decis[j] = 0.0;
			  mega[3][i].linkdecis[j] = -999;
		  }
		  i++;
		  m[3]++;
	  }while(!feof(FILEIN));

	  fclose(FILEIN);
  }
  /* read targets of each camera */
  for (i=0; i<n_img; i++)
    {
      nt4[3][i]=0;

	  if( useCompression ) {
		  compose_name_plus_nr_str (seq_name[i], "_targets.gz", filenumber, filein);
		  
		  if( ( fileGz = gzopen( filein, "rb" ) ) == NULL ) {
		  	fprintf(stderr, "Can't open gzip file: %s\n", filein);
			exit( 1 );
		  }

		  gzgets( fileGz, buf, 256 );
		  sscanf( buf, "%d\n", &nt4[3][i] );
		  
		  for (j=0; j<nt4[3][i]; j++)
		  {
			  gzgets( fileGz, buf, 256 );
			  sscanf (buf, "%4d %lf %lf %d %d %d %d %d\n",
					  &t4[3][i][j].pnr, &t4[3][i][j].x,
					  &t4[3][i][j].y, &t4[3][i][j].n ,
					  &t4[3][i][j].nx ,&t4[3][i][j].ny,
					  &t4[3][i][j].sumg, &t4[3][i][j].tnr);
		  }
		  gzclose (fileGz);
		  
	  } else {
		  compose_name_plus_nr_str (seq_name[i], "_targets", filenumber, filein);

		  FILEIN= fopen (filein, "r");
		  if (! FILEIN) printf("Can't open ascii file: %s\n", filein);

		  fscanf (FILEIN, "%d\n", &nt4[3][i]);
		  for (j=0; j<nt4[3][i]; j++)
		  {
			  fscanf (FILEIN, "%4d %lf %lf %d %d %d %d %d\n",
					  &t4[3][i][j].pnr, &t4[3][i][j].x,
					  &t4[3][i][j].y, &t4[3][i][j].n ,
					  &t4[3][i][j].nx ,&t4[3][i][j].ny,
					  &t4[3][i][j].sumg, &t4[3][i][j].tnr);
		  }
		  fclose (FILEIN);
	  }
	}

}

/**********************************************************************/
void write_ascii_data(int filenumber)
{
  FILE	*FILEOUT;
  gzFile fileGz;
  char	fileout[256];
  int	i, set, j;

  set = 0;

  if (filenumber < 10)       sprintf (fileout, "res/ptv_is.00%1d", filenumber);
  else if (filenumber< 100)  sprintf (fileout, "res/ptv_is.0%2d",  filenumber);
  else       sprintf (fileout, "res/ptv_is.%3d",  filenumber);

  /*  printf ("write file: %s\n",fileout); */
  if( useCompression ) {
	  sprintf( filename, "%s.gz", fileout );
	  fileGz = gzopen (filename, "wb");
	  if (! fileGz ) printf("Can't open gzip file for writing\n");

	  gzprintf( fileGz, "%d\n", m[set]);

	  for(i=0; i<m[set]; i++)
	  {
		  /*read dataset row by row*/
		  gzprintf( fileGz, "%4d %4d %10.3f %10.3f %10.3f\n",
				  mega[set][i].prev, mega[set][i].next, mega[set][i].x[0],
				  mega[set][i].x[1], mega[set][i].x[2]); /* Alex 22.11.03 */
	  }
	  gzclose( fileGz);
  } else {
	  FILEOUT = fopen (fileout, "w");
	  if (! FILEOUT) printf("Can't open ascii file for writing\n");

	  fprintf(FILEOUT, "%d\n", m[set]);

	  for(i=0; i<m[set]; i++)
	  {
		  /*read dataset row by row*/
		  fprintf(FILEOUT, "%4d %4d %10.3f %10.3f %10.3f\n",
				  mega[set][i].prev, mega[set][i].next, mega[set][i].x[0],
				  mega[set][i].x[1], mega[set][i].x[2]); /* Alex 22.11.03 */
	  }
	  fclose(FILEOUT);
  }

  /* create/update of new targets- and new rt_is-files */

  if (filenumber < 10)        sprintf (fileout, "res/rt_is.00%1d", filenumber);
  else if (filenumber< 100)   sprintf (fileout, "res/rt_is.0%2d",  filenumber);
  else       sprintf (fileout, "res/rt_is.%3d",  filenumber);

  /*  printf ("write file: %s\n",fileout); */
  if( useCompression ) {
	  sprintf( filename, "%s.gz", fileout );
	  fileGz = gzopen (filename, "wb");
	  if (! fileGz) printf("Can't open ascii file for writing\n");

	  gzprintf(fileGz, "%d\n", m[set]);

	  for(i=0; i<m[set]; i++)
	  {
		  gzprintf( fileGz, "%4d %9.3f %9.3f %9.3f %4d %4d %4d %4d\n",
				  i+1, mega[set][i].x[0], mega[set][i].x[1], mega[set][i].x[2],
				  c4[set][i].p[0], c4[set][i].p[1], c4[set][i].p[2], c4[set][i].p[3]);
	  }

	  gzclose(fileGz);
  } else {
	  FILEOUT = fopen (fileout, "w");
	  if (! FILEOUT) printf("Can't open ascii file for writing\n");

	  fprintf(FILEOUT, "%d\n", m[set]);

	  for(i=0; i<m[set]; i++)
	  {
		  fprintf(FILEOUT, "%4d %9.3f %9.3f %9.3f %4d %4d %4d %4d\n",
				  i+1, mega[set][i].x[0], mega[set][i].x[1], mega[set][i].x[2],
				  c4[set][i].p[0], c4[set][i].p[1], c4[set][i].p[2], c4[set][i].p[3]);
	  }

	  fclose(FILEOUT);
  }

  /* write targets of each camera */
  for (i=0; i<n_img; i++)
  {
	  if( useCompression ) {
		  compose_name_plus_nr_str (seq_name[i], "_targets.gz",
				  filenumber, fileout);

		  fileGz= gzopen (fileout, "wb");
		  if (! fileGz ) printf("Can't open gzip file: %s\n", fileout);

		  gzprintf (fileGz, "%d\n", nt4[set][i]);
		  for (j=0; j<nt4[set][i]; j++)
		  {
			  gzprintf (fileGz, "%4d %9.4f %9.4f %5d %5d %5d %5d %5d\n",
					  t4[set][i][j].pnr, t4[set][i][j].x,
					  t4[set][i][j].y, t4[set][i][j].n ,
					  t4[set][i][j].nx ,t4[set][i][j].ny,
					  t4[set][i][j].sumg, t4[set][i][j].tnr);
		  }
		  gzclose (fileGz);
	  } else {
		  compose_name_plus_nr_str (seq_name[i], "_targets",
				  filenumber, fileout);

		  FILEOUT= fopen (fileout, "w");
		  if (! FILEOUT) printf("Can't open ascii file: %s\n", fileout);

		  fprintf (FILEOUT, "%d\n", nt4[set][i]);
		  for (j=0; j<nt4[set][i]; j++)
		  {
			  fprintf (FILEOUT, "%4d %9.4f %9.4f %5d %5d %5d %5d %5d\n",
					  t4[set][i][j].pnr, t4[set][i][j].x,
					  t4[set][i][j].y, t4[set][i][j].n ,
					  t4[set][i][j].nx ,t4[set][i][j].ny,
					  t4[set][i][j].sumg, t4[set][i][j].tnr);
		  }
		  fclose (FILEOUT);
	  }
  }
}


/* XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX */
void write_added(int filenumber)
{
  FILE	*FILEOUT;
  gzFile fileGz;
  char	fileout[256];
  int	i, set;

  set = 0;

  if (filenumber < 10)       sprintf (fileout, "res/added.00%1d", filenumber);
  else if (filenumber< 100)  sprintf (fileout, "res/added.0%2d",  filenumber);
  else       sprintf (fileout, "res/added.%3d",  filenumber);

  /*  printf ("write file: %s\n",fileout); */
  if( useCompression ) {
	  sprintf( filename, "%s.gz", fileout );
	  fileGz = gzopen (filename, "wb");
	  if (! fileGz) printf("Can't open ascii file for writing\n");

	  gzprintf(fileGz, "%d\n", m[set]);

	  for(i=0; i<m[set]; i++)
	  {
		  /*read dataset row by row*/
		  gzprintf(fileGz, "%4d %4d %10.3f %10.3f %10.3f %d %10.3f\n",
				  mega[set][i].prev, mega[set][i].next, mega[set][i].x[0],
				  mega[set][i].x[1], mega[set][i].x[2], mega[set][i].prio, mega[set][i].finaldecis); /* Alex 24.11.03 */
	  }
	  gzclose(fileGz);
  } else {
	  FILEOUT = fopen (fileout, "w");
	  if (! FILEOUT) printf("Can't open ascii file for writing\n");

	  fprintf(FILEOUT, "%d\n", m[set]);

	  for(i=0; i<m[set]; i++)
	  {
		  /*read dataset row by row*/
		  fprintf(FILEOUT, "%4d %4d %10.3f %10.3f %10.3f %d %10.3f\n",
				  mega[set][i].prev, mega[set][i].next, mega[set][i].x[0],
				  mega[set][i].x[1], mega[set][i].x[2], mega[set][i].prio, mega[set][i].finaldecis); /* Alex 24.11.03 */
	  }
	  fclose(FILEOUT);
  }

}
/* XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX */
/**********************************************************************/
void write_addedback(int filenumber)
{
  FILE	*FILEOUT;
  gzFile fileGz;
  char	fileout[256];
  int	i, set;

  set = 0;

  if (filenumber < 10)       sprintf (fileout, "res/added.00%1d", filenumber);
  else if (filenumber< 100)  sprintf (fileout, "res/added.0%2d",  filenumber);
  else       sprintf (fileout, "res/added.%3d",  filenumber);

  /*  printf ("write file: %s\n",fileout); */
  if( useCompression ) {
	  sprintf( filename, "%s.gz", fileout );
	  fileGz = gzopen (filename, "wb");
	  if (! fileGz) printf("Can't open ascii file for writing\n");

	  gzprintf(fileGz, "%d\n", m[set]);

	  for(i=0; i<m[set]; i++)
	  {
		  /*read dataset row by row, prev/next order changed because backwards*/
		  gzprintf(fileGz, "%4d %4d %10.3f %10.3f %10.3f %d %10.3f\n",
				  mega[set][i].prev, mega[set][i].next, mega[set][i].x[0],
				  mega[set][i].x[1], mega[set][i].x[2], mega[set][i].prio, mega[set][i].finaldecis); /* Alex 24.11.03 */
	  }
	  gzclose(fileGz);
  } else {
	  FILEOUT = fopen (fileout, "w");
	  if (! FILEOUT) printf("Can't open ascii file for writing\n");

	  fprintf(FILEOUT, "%d\n", m[set]);

	  for(i=0; i<m[set]; i++)
	  {
		  /*read dataset row by row, prev/next order changed because backwards*/
		  fprintf(FILEOUT, "%4d %4d %10.3f %10.3f %10.3f %d %10.3f\n",
				  mega[set][i].prev, mega[set][i].next, mega[set][i].x[0],
				  mega[set][i].x[1], mega[set][i].x[2], mega[set][i].prio, mega[set][i].finaldecis); /* Alex 24.11.03 */
	  }
	  fclose(FILEOUT);
  }



}
/* ************************************************************* */

/* ************************************************************* */

void read_ascii_datanew(int filenumber)
{
  FILE	*FILEIN;
  gzFile fileGz;
  char	filein[256];
  int	i, j;
  int   dumy;
  double fdumy;

  for(i=0;i<M;i++)
    {
      /*reset all other variable to default value*/
      mega[3][i].prev = -1;
      mega[3][i].next = -2;
      mega[3][i].prio = 4;
      mega[3][i].inlist = 0;
      mega[3][i].finaldecis = 1000000.0;
      c4[3][i].p[0]=-1;
      c4[3][i].p[1]=-1;
      c4[3][i].p[2]=-1;
      c4[3][i].p[3]=-1;
    }

  if (filenumber < 10)        sprintf (filein, "res/rt_is.00%1d", filenumber);
  else if (filenumber < 100)  sprintf (filein, "res/rt_is.0%2d",  filenumber);
  else       sprintf (filein, "res/rt_is.%3d", filenumber);

  if( useCompression ) {
	  sprintf( filename, "%s.gz", filein );
	  fileGz = gzopen (filename, "rb");
	  if (! fileGz) printf("Can't open ascii file: %s\n", filein);

	  m[3]=0;

	  gzgets( fileGz, buf, 256 );
	  sscanf( buf, "%d\n", &m[3]);

	  for(i=0; i<=m[3]; i++)
	  {
		  /*read dataset row by row, x,y,z and correspondences */
		  gzgets( fileGz, buf, 256 );

		  sscanf(buf, "%d %f %f %f %d %d %d %d\n",
				  &dumy, &mega[3][i].x[0], &mega[3][i].x[1], &mega[3][i].x[2],
				  &c4[3][i].p[0], &c4[3][i].p[1], &c4[3][i].p[2], &c4[3][i].p[3]);

		  c4[3][i].nr=i;

		  /*reset other variables to default value*/
		  mega[3][i].inlist = 0;
		  mega[3][i].finaldecis = 1000000.0;

		  for(j=0; j<POSI; j++){
			  mega[3][i].decis[j] = 0.0;
			  mega[3][i].linkdecis[j] = -999;
		  }
	  }

	  gzclose(fileGz);
  } else {
	  FILEIN = fopen (filein, "r");
	  if (! FILEIN) printf("Can't open ascii file: %s\n", filein);

	  m[3]=0;

	  fscanf(FILEIN, "%d\n", &m[3]);

	  for(i=0; i<=m[3]; i++)
	  {
		  /*read dataset row by row, x,y,z and correspondences */
		  fscanf(FILEIN, "%d %f %f %f %d %d %d %d\n",
				  &dumy, &mega[3][i].x[0], &mega[3][i].x[1], &mega[3][i].x[2],
				  &c4[3][i].p[0], &c4[3][i].p[1], &c4[3][i].p[2], &c4[3][i].p[3]);

		  c4[3][i].nr=i;

		  /*reset other variables to default value*/
		  mega[3][i].inlist = 0;
		  mega[3][i].finaldecis = 1000000.0;

		  for(j=0; j<POSI; j++){
			  mega[3][i].decis[j] = 0.0;
			  mega[3][i].linkdecis[j] = -999;
		  }
	  }

	  fclose(FILEIN);
  }

  /* read ptv_is-file for prev and next info */

  if (filenumber < 10)       sprintf (filein, "res/ptv_is.00%1d", filenumber);
  else if (filenumber< 100)  sprintf (filein, "res/ptv_is.0%2d",  filenumber);
  else       sprintf (filein, "res/ptv_is.%3d",  filenumber);

  if( useCompression ) {
	  sprintf( filename, "%s.gz", filein );
	  fileGz = gzopen (filename, "rb");
	  if (! fileGz) printf("Can't open ascii file for reading\n");

	  gzgets( fileGz, buf, 256 );
	  sscanf(buf, "%d\n", &dumy);

	  for(i=0; i<=m[3]; i++)
	  {
		  /*read dataset row by row*/
		  gzgets( fileGz, buf, 256 );
		  sscanf(buf, "%4d %4d %lf %lf %lf\n", &mega[3][i].prev, &mega[3][i].next, &fdumy, &fdumy, &fdumy);
	  }
	  gzclose(fileGz);
  } else {

	  FILEIN = fopen (filein, "r");
	  if (! FILEIN) printf("Can't open ascii file for reading\n");

	  fscanf(FILEIN, "%d\n", &dumy);

	  for(i=0; i<=m[3]; i++)
	  {
		  /*read dataset row by row*/
		  fscanf(FILEIN, "%4d %4d %lf %lf %lf\n", &mega[3][i].prev, &mega[3][i].next, &fdumy, &fdumy, &fdumy);
	  }
	  fclose(FILEIN);
  }
  /* end of read ptv_is-file for prev and next info */


  /* read added-file for prio info */

  if (filenumber < 10)       sprintf (filein, "res/added.00%1d", filenumber);
  else if (filenumber< 100)  sprintf (filein, "res/added.0%2d",  filenumber);
  else       sprintf (filein, "res/added.%3d",  filenumber);

  if( useCompression ) {
	  sprintf( filename, "%s.gz", filein );
	  fileGz = gzopen (filename, "rb");
	  if (! fileGz) printf("Can't open ascii file for reading\n");

	  gzgets( fileGz, buf, 256 );
	  sscanf(buf, "%d\n", &dumy);

	  for(i=0; i<=m[3]; i++)
	  {
		  /*read dataset row by row*/
		  gzgets( fileGz, buf, 256 );
		  sscanf(buf, "%4d %4d %lf %lf %lf %d %lf\n", &dumy, &dumy, &fdumy, &fdumy, &fdumy, &mega[3][i].prio, &fdumy); /* Alex 24.11.03 */
	  }
	  gzclose(fileGz);
  } else {
	  FILEIN = fopen (filein, "r");
	  if (! FILEIN) printf("Can't open ascii file for reading\n");

	  fscanf(FILEIN, "%d\n", &dumy);

	  for(i=0; i<=m[3]; i++)
	  {
		  /*read dataset row by row*/
		  fscanf(FILEIN, "%4d %4d %lf %lf %lf %d %lf\n", &dumy, &dumy, &fdumy, &fdumy, &fdumy, &mega[3][i].prio, &fdumy); /* Alex 24.11.03 */
	  }
	  fclose(FILEIN);
  }
  /* end of read added-file for prio info */


  /* read targets of each camera */
  for (i=0; i<n_img; i++)
  {
	  nt4[3][i]=0;

	  if( useCompression ) {
		  compose_name_plus_nr_str (seq_name[i], "_targets.gz",
				  filenumber, filein);

		  fileGz= gzopen (filein, "rb");
		  if (! fileGz ) printf("Can't open gzip file: %s\n", filein);

		  gzgets( fileGz , buf, 256 );
		  sscanf (buf, "%d\n", &nt4[3][i]);
		  for (j=0; j<nt4[3][i]; j++)
		  {
			  gzgets( fileGz , buf, 256 );
			  
			  sscanf (buf, "%4d %lf %lf %d %d %d %d %d\n",
					  &t4[3][i][j].pnr, &t4[3][i][j].x,
					  &t4[3][i][j].y, &t4[3][i][j].n ,
					  &t4[3][i][j].nx ,&t4[3][i][j].ny,
					  &t4[3][i][j].sumg, &t4[3][i][j].tnr);
		  }
		  gzclose (fileGz );
	  } else {
		  compose_name_plus_nr_str (seq_name[i], "_targets",
				  filenumber, filein);

		  FILEIN= fopen (filein, "r");
		  if (! FILEIN) printf("Can't open ascii file: %s\n", filein);

		  fscanf (FILEIN, "%d\n", &nt4[3][i]);
		  for (j=0; j<nt4[3][i]; j++)
		  {
			  fscanf (FILEIN, "%4d %lf %lf %d %d %d %d %d\n",
					  &t4[3][i][j].pnr, &t4[3][i][j].x,
					  &t4[3][i][j].y, &t4[3][i][j].n ,
					  &t4[3][i][j].nx ,&t4[3][i][j].ny,
					  &t4[3][i][j].sumg, &t4[3][i][j].tnr);
		  }
		  fclose (FILEIN);
	  }
  }

}

/**********************************************************************/
void write_ascii_datanew(int filenumber)
{
  FILE	*FILEOUT;
  gzFile fileGz;
  char	fileout[256];
  int	i, set, j;

  set = 0;

  if (filenumber < 10)       sprintf (fileout, "res/ptv_is.00%1d", filenumber);
  else if (filenumber< 100)  sprintf (fileout, "res/ptv_is.0%2d",  filenumber);
  else       sprintf (fileout, "res/ptv_is.%3d",  filenumber);

  /*  printf ("write file: %s\n",fileout); */
  if( useCompression ) {
	  sprintf( filename, "%s.gz", fileout );
	  fileGz = gzopen (filename, "wb");
	  if (! fileGz) printf("Can't open ascii file for writing\n");

	  gzprintf(fileGz, "%d\n", m[set]);

	  for(i=0; i<m[set]; i++)
	  {
		  /*read dataset row by row, prev/next order changed because backwards*/
		  gzprintf(fileGz, "%4d %4d %10.3f %10.3f %10.3f\n",
				  mega[set][i].prev, mega[set][i].next, mega[set][i].x[0],
				  mega[set][i].x[1], mega[set][i].x[2]);
	  }
	  gzclose(fileGz);
  } else {
	  FILEOUT = fopen (fileout, "w");
	  if (! FILEOUT) printf("Can't open ascii file for writing\n");

	  fprintf(FILEOUT, "%d\n", m[set]);

	  for(i=0; i<m[set]; i++)
	  {
		  /*read dataset row by row, prev/next order changed because backwards*/
		  fprintf(FILEOUT, "%4d %4d %10.3f %10.3f %10.3f\n",
				  mega[set][i].prev, mega[set][i].next, mega[set][i].x[0],
				  mega[set][i].x[1], mega[set][i].x[2]);
	  }
	  fclose(FILEOUT);
  }

  /* update of targets- and rt_is-files */

  if (filenumber < 10)        sprintf (fileout, "res/rt_is.00%1d", filenumber);
  else if (filenumber< 100)   sprintf (fileout, "res/rt_is.0%2d",  filenumber);
  else       sprintf (fileout, "res/rt_is.%3d",  filenumber);

  /*  printf ("write file: %s\n",fileout); */
  if( useCompression ) {
	  sprintf( filename, "%s.gz", fileout );
	  fileGz = gzopen (filename, "wb");
	  if (! fileGz) printf("Can't open ascii file for writing\n");

	  gzprintf(fileGz, "%d\n", m[set]);

	  for(i=0; i<m[set]; i++)
	  {
		  /*write dataset row by row*/
		  gzprintf(fileGz, "%4d %9.3f %9.3f %9.3f %4d %4d %4d %4d\n",
				  i+1, mega[set][i].x[0], mega[set][i].x[1], mega[set][i].x[2],
				  c4[set][i].p[0], c4[set][i].p[1], c4[set][i].p[2], c4[set][i].p[3]);
	  }

	  gzclose(fileGz);
  } else {
	  FILEOUT = fopen (fileout, "w");
	  if (! FILEOUT) printf("Can't open ascii file for writing\n");

	  fprintf(FILEOUT, "%d\n", m[set]);

	  for(i=0; i<m[set]; i++)
	  {
		  /*write dataset row by row*/
		  fprintf(FILEOUT, "%4d %9.3f %9.3f %9.3f %4d %4d %4d %4d\n",
				  i+1, mega[set][i].x[0], mega[set][i].x[1], mega[set][i].x[2],
				  c4[set][i].p[0], c4[set][i].p[1], c4[set][i].p[2], c4[set][i].p[3]);
	  }

	  fclose(FILEOUT);
  }

  /* write targets of each camera */
  for (i=0; i<n_img; i++)
  {
	  if( useCompression ) {
		  compose_name_plus_nr_str (seq_name[i], "_targets.gz",
				  filenumber, fileout);

		  fileGz= gzopen (fileout, "wb");
		  if (! fileGz) printf("Can't open gzip file: %s\n", fileout);

		  gzprintf (fileGz, "%d\n", nt4[set][i]);
		  for (j=0; j<nt4[set][i]; j++)
		  {
			  gzprintf (fileGz, "%4d %9.4f %9.4f %5d %5d %5d %5d %5d\n",
					  t4[set][i][j].pnr, t4[set][i][j].x,
					  t4[set][i][j].y, t4[set][i][j].n ,
					  t4[set][i][j].nx ,t4[set][i][j].ny,
					  t4[set][i][j].sumg, t4[set][i][j].tnr);
		  }
		  gzclose (fileGz);
	  } else {
		  compose_name_plus_nr_str (seq_name[i], "_targets",
				  filenumber, fileout);

		  FILEOUT= fopen (fileout, "w");
		  if (! FILEOUT) printf("Can't open ascii file: %s\n", fileout);

		  fprintf (FILEOUT, "%d\n", nt4[set][i]);
		  for (j=0; j<nt4[set][i]; j++)
		  {
			  fprintf (FILEOUT, "%4d %9.4f %9.4f %5d %5d %5d %5d %5d\n",
					  t4[set][i][j].pnr, t4[set][i][j].x,
					  t4[set][i][j].y, t4[set][i][j].n ,
					  t4[set][i][j].nx ,t4[set][i][j].ny,
					  t4[set][i][j].sumg, t4[set][i][j].tnr);
		  }
		  fclose (FILEOUT);
	  }
  }

}
/* ************************************************************* */





//int tracking(ClientData clientData, Tcl_Interp* interp, int argc, const char** argv)
int tracking( int argc, const char** argv)
{
 level1();
 level2();
 level3();

 return 0;
}

void level1(void)
{
  int i, ii, j, k, l;
  float trad[3];
  int liste[M], inliste, n2liste[POSI], inn2liste, n3liste[POSI], inn3liste;
  float seekx[3], esti, acc;
  int finish, flag;

  /* Define some varibles. This is done every time tracking is called
     (my be not necessary but is not a big problem
     trad is radius of correlation neigbourhood, trad1
     is search radius for next timestep with link*/

    trad[0]= tpar.dvxmax;
    trad[1]= tpar.dvymax;
    trad[2]= tpar.dvzmax;

  /* BEGIN TRACKING*/

  /* First start with highest priority this is if particle has already
     link to previous
     timstep current timstep is t[1]*/
  /*first search for tracks with previous links*/
  /*links named -1 or -2 are no links*/

  for(inliste=0, i=0; i<m[1]; i++)
    if (mega[1][i].prev > -1) {
      liste[inliste]=i;
      inliste++;
    }
  /*calculate possible tracks for t2 and t3 and calculate decision criteria*/
  if(inliste > 0) {
    for(i=0; i < inliste; i++) {
      for(j=0; j < 3; j++)
	seekx[j] = 2*mega[1][liste[i]].x[j]-mega[0][mega[1][liste[i]].prev].x[j];

      /*find neighbours in next timestep t = 2*/
      inn2liste = 0;
      neighbours(seekx, trad, n2liste, &inn2liste, 2);
      /* if no neighour is found no link will be established*/

      /*calculate decision criteria*/
      if(inn2liste > 0) {
	for(k=0; k<inn2liste; k++) {
	  for(j=0; j < 3; j++)
	    seekx[j] = 2*mega[2][n2liste[k]].x[j]-mega[1][liste[i]].x[j];


          /*find neigbours in next timestep t = 3*/
          inn3liste = 0;
          neighbours(seekx, trad, n3liste, &inn3liste, 3);


	  if(inn3liste == 0) {
	    /*if no neighour in t3 is found, give decision criteria artifical
	      value (100000) accelaration can be considered
	      as unbelivible large*/
  	    mega[1][liste[i]].decis[k] = 1000000.0;
	    mega[1][liste[i]].linkdecis[k] = n2liste[k];
	  }
	  else {
	    for(esti = 1000000.0, l=0; l<inn3liste; l++) {
	      /*calculate for estimates the decision value*/
	      for(acc=0.0, ii=0; ii<3; ii++)
		acc += sqr(mega[1][liste[i]].x[ii] - 2*mega[2][n2liste[k]].x[ii]
			   + mega[3][n3liste[l]].x[ii]);

	      acc=sqrt(acc);
	      if(esti > acc)
		esti = acc;
	    }/*for(l....)*/
	    mega[1][liste[i]].decis[k] = esti;
	    mega[1][liste[i]].linkdecis[k] = n2liste[k];
	  } /*else(inn2liste >0*/
	}/*for (k...)*/
	mega[1][liste[i]].inlist = inn2liste;
	if(inn2liste > 1)
	  sort(inn2liste, mega[1][liste[i]].decis, mega[1][liste[i]].linkdecis);

      }/*if(inn1liste > 0)*/

    }/*for(i=0....)*/

    /*establish links by streaming completly through the data*/

    do {
      finish = 0;

      for(i=0; i<inliste; i++) {

	if(mega[1][liste[i]].next < 0) {
	  if(mega[1][liste[i]].inlist > 0) {
	    /*in the following is a sorted list of decis assumed*/
	    flag = 1;
	    j = 0;
	    do {

	      if(mega[2][mega[1][liste[i]].linkdecis[j]].prev < 0) {
		/*found possible link*/
		mega[1][liste[i]].next = mega[1][liste[i]].linkdecis[j];
		mega[2][mega[1][liste[i]].linkdecis[j]].prev = liste[i];
		mega[2][mega[1][liste[i]].linkdecis[j]].prio = 0;
		mega[1][liste[i]].finaldecis = mega[1][liste[i]].decis[j];
		flag = 0;
	      }/*if(p2 == -1)*/

	      /*test exiting link if would be better*/
	      else if(mega[1][mega[2][mega[1][liste[i]].linkdecis[j]].prev].finaldecis
		      > mega[1][liste[i]].decis[j])
		{
		  /*current link is better and reset other link*/
		  mega[1][mega[2][mega[1][liste[i]].linkdecis[j]].prev].next = -2;

		  mega[1][liste[i]].next = mega[1][liste[i]].linkdecis[j];
		  mega[2][mega[1][liste[i]].linkdecis[j]].prev = liste[i];
		  mega[2][mega[1][liste[i]].linkdecis[j]].prio = 0;
		  mega[1][liste[i]].finaldecis = mega[1][liste[i]].decis[j];
		  flag = 0;
		  finish = 1;
		}

	      else {
             j++; /* if first choice is not possible then try next */
	      }
	    }while((j < mega[1][liste[i]].inlist) && flag);

	  }/*if(mega[1]....)*/
	  else {
	    mega[1][liste[i]].next=-1; /*No link could be established*/
	  }/*else*/
	}/*if(mega[1]  .next < 0)*/
      }/*for(i=0....)*/

    }while(finish);

  }/*if(inlist >0)*/

  /*END OF FIRST TRAIL*/
}

/*second if no previous link but in neigbouhood exist previous links*/

void level2(void)
{
  int i, ii, j, k, l;
  float trad[3];
  int liste[M], inliste, n1liste[POSI], inn1liste;
  int n2liste[POSI], inn2liste, n3liste[POSI], inn3liste;
  float seekx[3], esti, acc, vel[3];
  int finish, flag, nvel;


  /* Define some varibles. This is done every time tracking is called
     (my be not necessary but is not a big problem*/


    trad[0]= tpar.dvxmax;
    trad[1]= tpar.dvymax;
    trad[2]= tpar.dvzmax;

  /* BEGIN TRACKING*/
  /* Secondly start with second priority this is if particle has already no link to
     previous timstep but in Particles in neigbourhoud have. Current timstep is t[1]*/

  /*first search for tracks with no previous links ancd no next link*/
  /*links named -1 or -2 are no links*/

  for(inliste=0, i=0; i<m[1]; i++)
    if (mega[1][i].next < 0 && mega[1][i].prev < 0) {
      /*check if neighbours wihtin correlation have link*/
      for(j=0; j < 3; j++)
	seekx[j] = mega[1][i].x[j];
      /* search points in neigbourhood within coorelation lenght*/
      inn1liste = 0;
      neighbours(seekx, trad, n1liste, &inn1liste, 1);
      /*check if neighbours have previous link*/
      /*n1liste must be greater than 1 because neigbours will find the point i itself*/
      if(inn1liste > 1) {
	for(vel[0]=0.0, vel[1]=0.0, vel[2]=0.0, nvel=0, j=0; j<inn1liste; j++) {
	  if(n1liste[j]!=i)
	    if(mega[1][n1liste[j]].prev > -1){
	      for(l=0; l<3; l++)
		vel[l] += mega[1][n1liste[j]].x[l]- mega[0][mega[1][n1liste[j]].prev].x[l];
	      nvel++;
	    }
	}

	if(nvel > 0) {
	  /*intermediate storage of center of position in next frame */
	  for(l=0; l<3; l++)
	    mega[1][i].decis[l] = vel[l]/(float)nvel;
	  liste[inliste]=i;
	  inliste++;
	}
      }
    }

  /*calculate possible tracks for t2 and t3 and calculate decision criteria*/
  if(inliste > 0) {
    for(i=0; i < inliste; i++) {
      for(j=0; j < 3; j++)
	seekx[j] = mega[1][liste[i]].x[j] + mega[1][liste[i]].decis[j];

      /*find neighbours in next timestep t = 2*/
      inn2liste = 0;
      neighbours(seekx, trad, n2liste, &inn2liste, 2);
      /* if no neighour is found no link will be established*/

      /*calculate decision criteria*/
      if(inn2liste > 0) {
	for(k=0; k<inn2liste; k++) {
	  for(j=0; j < 3; j++)
	    seekx[j] = 2*mega[2][n2liste[k]].x[j]-mega[1][liste[i]].x[j];

	  /*find neigbours in next timestep t = 3*/
	  inn3liste = 0;
	  neighbours(seekx, trad, n3liste, &inn3liste, 3);

	  if(inn3liste == 0) {
	    /*if no neighour in t3 is found, give decision criteria artifical value (100000)
	      accelaration can be considered as unbelivible large*/
	    mega[1][liste[i]].decis[k] = 1000000.0;
	    mega[1][liste[i]].linkdecis[k] = n2liste[k];
	  }
	  else {
	    for(esti = 1000000.0, l=0; l<inn3liste; l++) {
	      /*calculate for estimates the decision value*/
	      for(acc=0.0, ii=0; ii<3; ii++)
		acc += sqr(mega[1][liste[i]].x[ii] - 2*mega[2][n2liste[k]].x[ii]
			   + mega[3][n3liste[l]].x[ii]);


	      acc=sqrt(acc);
	      if(esti > acc)
		esti = acc;
	    }/*for(l....)*/
	    mega[1][liste[i]].decis[k] = esti;
	    mega[1][liste[i]].linkdecis[k] = n2liste[k];
	  } /*else(inn2liste >0*/
	}/*for (k...)*/
	mega[1][liste[i]].inlist = inn2liste;
	if(inn2liste > 1)
	  sort(inn2liste, mega[1][liste[i]].decis, mega[1][liste[i]].linkdecis);

      }/*if(inn1liste > 0)*/

    }/*for(i=0....)*/

    /*establish links by streaming completly through the data*/

    do {
      finish = 0;

      for(i=0; i<inliste; i++) {
	if(mega[1][liste[i]].next < 0) {

	  if(mega[1][liste[i]].inlist > 0) {
	    /*in the following is a sorted list of decis assumed*/
	    flag = 1;
	    j = 0;
	    do {

	      if(mega[2][mega[1][liste[i]].linkdecis[j]].prev < 0) {
		/*found possible link*/
		mega[1][liste[i]].next = mega[1][liste[i]].linkdecis[j];
		mega[2][mega[1][liste[i]].linkdecis[j]].prev = liste[i];
		mega[2][mega[1][liste[i]].linkdecis[j]].prio = 1;
		mega[1][liste[i]].finaldecis = mega[1][liste[i]].decis[j];
		flag = 0;
	      }/*if(p2 == -1)*/

	      /*test exiting link if would be better*/
	      else if((mega[1][mega[2][mega[1][liste[i]].linkdecis[j]].prev].finaldecis
		       > mega[1][liste[i]].decis[j])
		      && (mega[2][mega[1][liste[i]].linkdecis[j]].prio >= 1))
		{
		  /*current link is better and reset other link*/
		  mega[1][mega[2][mega[1][liste[i]].linkdecis[j]].prev].next = -2;

		  mega[1][liste[i]].next = mega[1][liste[i]].linkdecis[j];
		  mega[2][mega[1][liste[i]].linkdecis[j]].prev = liste[i];
		  mega[2][mega[1][liste[i]].linkdecis[j]].prio = 1;
		  mega[1][liste[i]].finaldecis = mega[1][liste[i]].decis[j];
		  flag = 0;
		  finish = 1;
		}

	      else {
		j++; /* if first choice is not possible then try next */
	      }

	    }while((j < mega[1][liste[i]].inlist) && flag);

	  }/*if(mega[1]....)*/
	  else {
	    mega[1][liste[i]].next=-1; /*No link could be established*/
	  }/*else*/
	}/*if(mega[1]  .next<0)*/
      }/*for(i=0....)*/

    }while(finish);

  }/*if(inlist >0)*/

  /*END OF second TRAIL*/
}


/*Third if no previous link nor in neigbouhood exist previous links*/

void level3(void)
{
  int i, ii, j, k, l;
  float trad[3];
  int liste[M], inliste, n2liste[POSI], inn2liste, n3liste[POSI], inn3liste;
  float seekx[3], esti, acc;
  int finish, flag;


  /* Define some varibles. This is done every time tracking is called
     (my be not necessary but is not a big problem*/

    trad[0]= tpar.dvxmax;
    trad[1]= tpar.dvymax;
    trad[2]= tpar.dvzmax;


  /* BEGIN TRACKING*/


  /* Thirdly start with third priority this is if particle has no link to previous
     timstep and in Particles in neigbourhoud have not. Current timstep is t[1]*/

  /*first search for tracks with no previous links and no next link*/
  /*links named -1 or -2 are no links*/

  for(inliste=0, i=0; i<m[1]; i++)
    if (mega[1][i].next < 0 && mega[1][i].prev < 0) {
      liste[inliste]=i;
      inliste++;
    }

  /*calculate possible tracks for t2 and t3 and calculate decision criteria*/
  if(inliste > 0) {
    for(i=0; i < inliste; i++) {
      for(j=0; j < 3; j++)
	seekx[j] = mega[1][liste[i]].x[j];

      /*find neighbours in next timestep t = 2*/
      inn2liste = 0;
      neighbours(seekx, trad, n2liste, &inn2liste, 2);
      /* if no neighour is found no link will be established*/

      /*calculate decision criteria*/
      if(inn2liste > 0) {
	for(k=0; k<inn2liste; k++) {
	  for(j=0; j < 3; j++)
	    seekx[j] = 2*mega[2][n2liste[k]].x[j]-mega[1][liste[i]].x[j];
	  inn3liste = 0;
	  neighbours(seekx, trad, n3liste, &inn3liste, 3);
	  if(inn3liste == 0) {
	    /*if no neighour in t3 is found, give decision criteria artifical value (100000)
	      accelaration can be considered as unbelivible large*/
	    mega[1][liste[i]].decis[k] = 1000000.0;
	    mega[1][liste[i]].linkdecis[k] = n2liste[k];
	  }
	  else {
	    for(esti = 1000000.0, l=0; l<inn3liste; l++) {
	      /*calculate estimates the decision value*/
	      for(acc=0.0, ii=0; ii<3; ii++)
		acc += sqr(mega[1][liste[i]].x[ii] - 2*mega[2][n2liste[k]].x[ii]
			   + mega[3][n3liste[l]].x[ii]);


	      acc=sqrt(acc);
	      if(esti > acc)
		esti = acc;
            }/*for(l....)*/
	    mega[1][liste[i]].decis[k] = esti;
	    mega[1][liste[i]].linkdecis[k] = n2liste[k];
	  } /*else(inn2liste >0*/
	}/*for (k...)*/
	mega[1][liste[i]].inlist = inn2liste;
	if(inn2liste > 1)
	  sort(inn2liste, mega[1][liste[i]].decis, mega[1][liste[i]].linkdecis);

      }/*if(inn1liste > 0)*/

    }/*for(i=0....)*/

    /*establish links by streaming completly through the data*/

    do {
      finish = 0;
      for(i=0; i<inliste; i++) {

	if(mega[1][liste[i]].next < 0) {
	  if(mega[1][liste[i]].inlist > 0) {
	    /*in the following is a sorted list of decis assumed*/
	    flag = 1;
	    j = 0;
	    do {

	      if(mega[2][mega[1][liste[i]].linkdecis[j]].prev < 0) {
		/*found possible link*/
		mega[1][liste[i]].next = mega[1][liste[i]].linkdecis[j];
		mega[2][mega[1][liste[i]].linkdecis[j]].prev = liste[i];
		mega[2][mega[1][liste[i]].linkdecis[j]].prio = 2;
		mega[1][liste[i]].finaldecis = mega[1][liste[i]].decis[j];
		flag = 0;
	      }/*if(p2 == -1)*/

	      /*test exiting link if would be better*/
	      else if((mega[1][mega[2][mega[1][liste[i]].linkdecis[j]].prev].finaldecis
		       > mega[1][liste[i]].decis[j])
		      && (mega[2][mega[1][liste[i]].linkdecis[j]].prio >= 2))
		{
		  /*current link is better and reset other link*/
		  mega[1][mega[2][mega[1][liste[i]].linkdecis[j]].prev].next = -2;

		  mega[1][liste[i]].next = mega[1][liste[i]].linkdecis[j];
		  mega[2][mega[1][liste[i]].linkdecis[j]].prev = liste[i];
		  mega[2][mega[1][liste[i]].linkdecis[j]].prio = 2;
		  mega[1][liste[i]].finaldecis = mega[1][liste[i]].decis[j];
		  flag = 0;
		  finish = 1;
		}

	      else {
		j++; /* if first choice is not possible then try next */
	      }

	    }while((j < mega[1][liste[i]].inlist) && flag);

	  }/*if(mega[1]....)*/
	  else {
	    mega[1][liste[i]].next=-1; /*No link could be established*/
	  }/*else*/
	}/*if(mega[1]  < 0)*/
      }/*for(i=0....)*/
    }while(finish);
  }/*if(inlist >0)*/
  /*END OF THIRD TRAIL*/
}



/***SORTING ALGORIHTMUS****/

void sort(int n, float a[], int b[])
{
  int flag = 0, i, itemp;
  float ftemp;

  do {
    flag = 0;
    for(i=0; i<(n-1); i++)
      if(a[i] > a[i+1]) {
	ftemp =  a[i];
	itemp =  b[i];
	a[i] = a[i+1];
	b[i] = b[i+1];
	a[i+1] = ftemp;
	b[i+1] = itemp;
        flag = 1;
      }
  }while(flag);
}


void rotate_dataset(void)
{
  void * tmp;
  void * tmp2;
  int i;

  /*rotate dataset by changeing pointer*/
  tmp = mega[0];
  mega[0] = mega[1];
  mega[1] = mega[2];
  mega[2] = mega[3];
  mega[3] =tmp;

  /*rotate counter*/
  m[0] = m[1];
  m[1] = m[2];
  m[2] = m[3];

  tmp = c4[0];
  c4[0] = c4[1];
  c4[1] = c4[2];
  c4[2] = c4[3];
  c4[3] =tmp;

  for(i=0; i<4; i++) {
  tmp2 = t4[0][i];
  t4[0][i] = t4[1][i];
  t4[1][i] = t4[2][i];
  t4[2][i] = t4[3][i];
  t4[3][i] =tmp2;

  nt4[0][i] = nt4[1][i];
  nt4[1][i] = nt4[2][i];
  nt4[2][i] = nt4[3][i];
  }
}



void neighbours(float seekx[], float radi[], int nliste[], int *innliste, int set)
{
  int i;
  /*search for points in srearch radius. No sorted list is supported,
    although sorted in z would increase speed*/

  for(i=0; i< m[set]; i++) {
    if(fabs(seekx[0] - mega[set][i].x[0]) < radi[0])
      if(fabs(seekx[1] - mega[set][i].x[1]) < radi[1])
        if(fabs(seekx[2] - mega[set][i].x[2]) < radi[2]){
          nliste[*innliste]=i;
          (*innliste)++;
          if(*innliste > POSI)
	    printf("More Points found than can be supported! Reduce search area or increase POSI\n");
        }
  }
}
                                                                                                                                                                                                                                                                                                                                                                                                    track/ptv.h                                                                                         0000644 0001131 0001131 00000002132 10252332172 011577  0                                                                                                    ustar   amns                            amns                                                                                                                                                                                                                   #include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

//#include <tcl.h>
//#include <tk.h>

#include "tiff.h"
#include "tiffio.h"

#include "typedefs.h"
#include "globals.h"
/*
extern Tk_PhotoImageFormat tkImgFmtTIF;

extern Tcl_CmdProc init_proc_c;
extern Tcl_CmdProc start_proc_c;
extern Tcl_CmdProc done_proc_c;
extern Tcl_CmdProc detection_proc_c;
extern Tcl_CmdProc pre_processing_c;
extern Tcl_CmdProc correspondences_proc_c;
extern Tcl_CmdProc determination_proc_c;
extern Tcl_CmdProc sequence_proc_c;
extern Tcl_CmdProc calibration_proc_c;
extern Tcl_CmdProc quit_proc_c;
extern Tcl_CmdProc mouse_proc_c;
extern Tcl_CmdProc flow_demo_c;
extern Tcl_CmdProc mark_track_c;
extern Tcl_CmdProc trackcorr_c;
extern Tcl_CmdProc trackback_c;
extern Tcl_CmdProc nearestinnext_c;
extern Tcl_CmdProc tracksequence_c;
extern Tcl_CmdProc tracking;
extern Tcl_CmdProc trajectories_c;
extern Tcl_CmdProc vrmltracks_c;
extern Tcl_CmdProc trajectories_c;
extern Tcl_CmdProc vrmldetections_c;
extern Tcl_CmdProc vrmldettracks_c;
extern Tcl_CmdProc seq_track_proc_c;
extern Tcl_CmdProc oddeven_c;
*/

                                                                                                                                                                                                                                                                                                                                                                                                                                      track/track.c                                                                                       0000644 0001131 0001131 00000111002 10303614602 012060  0                                                                                                    ustar   amns                            amns                                                                                                                                                                                                                   /*******************************************************************

Routine:					track.c

Author/Copyright:		 	Jochen Willneff

Address:				 	Institute of Geodesy and Photogrammetry
							ETH - Hoenggerberg
							CH - 8093 Zurich

Creation Date:				Beginning: February '98
							End: far away

Description:	 			Tracking of particles in image- and objectspace

Routines contained:			trackcorr_c

Amended:                    June 2005, Chris Bunney, University of Plymouth.
                            Removed all Tcl/Tk code and implemented a
                            facility for reading a dynamic tracking parameters
                            input file.

 *******************************************************************/
#include "ptv.h"
#include "globals.h"
#include "dynamic_track.h"

void write_added();
void write_addedback();

/*int trackcorr_c (ClientData clientData, Tcl_Interp* interp, int argc, const char** argv)*/

int trackcorr_c( )
{
	char	val[256], buf[256];
	int i, j, h, k, mm, kk, step, okay=0, invol=0;
	int zaehler1, zaehler2,philf[4][4];
	int count1=0, count2=0, count3=0, lost =0, zusatz=0;
	int intx0, intx1, inty0, inty1;
	int intx2, inty2,intx3, inty3;
	int quali=0;
	double x1[4], y1[4], x2[4], y2[4], angle, acc, angle0, acc0, lmax, dl;
	double xr[4], xl[4], yd[4], yu[4], angle1, acc1;
	double X1, Y1, Z1, X0, Y0, Z0, X2, Y2, Z2;
	double X3, Y3, Z3, X4, Y4, Z4, X5, Y5, Z5, X6, Y6, Z6;
	double xp[4], yp[4], xc[4], yc[4], xn[4], yn[4];
	double rr, Ymin=0, Ymax=0;
	double npart=0, nlinks=0;

	foundpix *w, *wn, p16[16];

	trackparameters *tplist;		// array of tracking parameters for sequence
									// (used in dynamic tracking only).
	

	/* read paramaters */
	readseqtrackcrit ();

	// ChrisB: if we are using dynamic tracking - get the relavent lines from
	// the dynamic tracking file:
	if( !staticTracking ) {
		loadTrackingParameters( &tplist, seq_first, seq_last );
		if( !tplist ) {
			fprintf( stderr, "Failed to load tracking parameters\n" );
			return -1;
		}
	}
			
	//debug:
#if 0
	for( i = 0; i <seq_last-seq_first+1; i++ ) {
		fprintf( stderr, "Dynamic TPar index %d {%lf %lf %lf %lf %lf %lf %lf %lf %d}\n",
				i,
				tplist[i].dvxmin,
				tplist[i].dvxmax,
				tplist[i].dvymin,
				tplist[i].dvymax,
				tplist[i].dvzmin,
				tplist[i].dvzmax,
				tplist[i].dangle,
				tplist[i].dacc,
				tplist[i].add
			   );
	}
#endif

	/*Alloc space, if checkflag for mega, c4, t4 is zero */
	if (!trackallocflag)
	{
		for (i=0; i<4; i++)
		{
			mega[i]=(P *) calloc(sizeof(P),M);
			c4[i]=(corres *) calloc(sizeof(corres),M);
			for (k=0; k<4; k++) {
				t4[i][k]=(target *) calloc(sizeof (target),M);
			}
		}
		trackallocflag=1;
	}

	/*load again first data sets*/
	/* ChrisB: This loads in the rt_is files for the first 3 images.*/
	step = seq_first;
	read_ascii_data(step);
	rotate_dataset();
	read_ascii_data(step+1);
	rotate_dataset();
	read_ascii_data(step+2);

	// this is now moved inside the processing loop as it needs the tpar values:
#if 0
	lmax=sqrt((tpar.dvxmin-tpar.dvxmax)*(tpar.dvxmin-tpar.dvxmax)
			+(tpar.dvymin-tpar.dvymax)*(tpar.dvymin-tpar.dvymax)
			+(tpar.dvzmin-tpar.dvzmax)*(tpar.dvzmin-tpar.dvzmax));

#endif
	volumedimension (&X_lay[1], &X_lay[0], &Ymax, &Ymin, &Zmax_lay[1], &Zmin_lay[0]);



	/**********************************************************/
	/* MAIN TRACKING LOOP.                                    */
	/* Now contains code to read dynamic tracking parameters. */
	/**********************************************************/
	
	for (step = seq_first; step < seq_last; step++)
	{
		if( verbose )
			;//printf ( "Time step: %d, seqnr: %d, Particle info:\n", step- seq_first, step);
		else {
			printf( "\rProcessing: step %03d of %03d         ",
					step - seq_first,
					seq_last - seq_first - 1 );
			fflush(stdout);
		}

		/*Tcl_SetVar(interp, "tbuf", buf, TCL_GLOBAL_ONLY);
		  Tcl_Eval(interp, ".text delete 2");
		  Tcl_Eval(interp, ".text insert 2 $tbuf");*/

		count1=0; lost =0; zusatz=0;

		if( !staticTracking ) {
			// ChrisB: read the dynamic tracking parameters for this timestep:
			getCurrentTPar( tplist, &tpar, step );

			
#if 0	// old method:
			if( fgets( buf, 256, fDynTPar ) == NULL ) {
				fprintf( stderr, "Fatal Error: Unexpected end of file {%s} at line %d\n\n",
						szDynTParFilename,
						step );
				exit( -1 );
			}

			nRead = sscanf( buf, "%lf %lf %lf %lf %lf %lf %lf %lf %d\n",
					&tpar.dvxmin,
					&tpar.dvxmax,
					&tpar.dvymin,
					&tpar.dvymax,
					&tpar.dvzmin,
					&tpar.dvzmax,
					&tpar.dangle,
					&tpar.dacc,
					&tpar.add
					);

			// check for malformed line:
			if( nRead != 9 )
			{
				fprintf( stderr, "Malformed line in file {%s} at line %d\n\n",
						szDynTParFilename,
						step );
				exit( -1 );
			}
#endif
		}	
			

#if 0
		// ChrisB; debugging:
		fprintf( stderr, "Dynamic TPar line %d {%lf %lf %lf %lf %lf %lf %lf %lf %d}\n",
				step,
				tpar.dvxmin,
				tpar.dvxmax,
				tpar.dvymin,
				tpar.dvymax,
				tpar.dvzmin,
				tpar.dvzmax,
				tpar.dangle,
				tpar.dacc,
				tpar.add
			   );
#endif
		
		// ChrisB: This was moved in from outside the loop as it needs the tpar values
		// which are now dynamically loaded each timestep:
		lmax=sqrt((tpar.dvxmin-tpar.dvxmax)*(tpar.dvxmin-tpar.dvxmax)
				+(tpar.dvymin-tpar.dvymax)*(tpar.dvymin-tpar.dvymax)
				+(tpar.dvzmin-tpar.dvzmax)*(tpar.dvzmin-tpar.dvzmax));

		
		/* try to track correspondences from previous 0 - corp, variable h */
		for (h=0; h<m[1]; h++)
		{
			X1=Y1=Z1=X0=Y0=Z0=X2=Y2=Z2=X5=Y5=Z5=X3=Y3=Z3=X4=Y4=Z4=X6=Y6=Z6=-999;
			mega[1][h].inlist=0;

			for (i=0; i<16;i++)
			{
				p16[i].ftnr=-1;
				p16[i].freq=0;
				for(j=0;j<n_img;j++) p16[i].whichcam[j] =0;
			}
			/* 3D-position */
			X1=mega[1][h].x[0];
			Y1=mega[1][h].x[1];
			Z1=mega[1][h].x[2];

			/* use information from previous to locate new search position
			   and to calculate values for search area */
			if (mega[1][h].prev>=0) {
				X0=mega[0][mega[1][h].prev].x[0];
				Y0=mega[0][mega[1][h].prev].x[1];
				Z0=mega[0][mega[1][h].prev].x[2];
				X2=2*X1-X0;
				Y2=2*Y1-Y0;
				Z2=2*Z1-Z0;

				for (j=0; j<n_img; j++) {
					img_coord (X2, Y2, Z2, Ex[j],I[j], ap[j], mmp, &xn[j], &yn[j]);
					metric_to_pixel (xn[j], yn[j], imx,imy, pix_x,pix_y, &xn[j], &yn[j], chfield);
					x1[j]=xn[j];
					y1[j]=yn[j];
				}
			} else {	X2=X1; Y2=Y1; Z2=Z1;
				for (j=0;j<n_img;j++) {
					if (c4[1][h].p[j] == -1) {
						img_coord (X2, Y2, Z2, Ex[j],I[j], ap[j], mmp, &xn[j], &yn[j]);
						metric_to_pixel (xn[j], yn[j], imx,imy, pix_x,pix_y, &xn[j], &yn[j], chfield);
						x1[j]=xn[j];
						y1[j]=yn[j];
					} else { x1[j]=t4[1][j][c4[1][h].p[j]].x; y1[j]=t4[1][j][c4[1][h].p[j]].y; }
				}
			}

			/* calculate searchquader and reprojection in image space */
			searchquader(X2, Y2, Z2, &xr, &xl, &yd, &yu);

			/* mark search quader in image */
			/*
			   for (j=0;j<n_img;j++)
			   {
			   intx0 = (int)(imx/2+zoom_f[j]*(x1[j]-xl[j]-zoom_x[j]));
			   inty0 = (int)(imy/2+zoom_f[j]*(y1[j]+yd[j]-zoom_y[j]));
			   intx1 = (int)(imx/2+zoom_f[j]*(x1[j]-xl[j]-zoom_x[j]));
			   inty1 = (int)(imy/2+zoom_f[j]*(y1[j]-yu[j]-zoom_y[j]));
			   intx2 = (int)(imx/2+zoom_f[j]*(x1[j]+xr[j]-zoom_x[j]));
			   inty2 = (int)(imy/2+zoom_f[j]*(y1[j]-yu[j]-zoom_y[j]));
			   intx3 = (int)(imx/2+zoom_f[j]*(x1[j]+xr[j]-zoom_x[j]));
			   inty3 = (int)(imy/2+zoom_f[j]*(y1[j]+yd[j]-zoom_y[j]));

			   drawvector (interp, intx0, inty0, intx1, inty1, 1, j, "white");
			   drawvector (interp, intx1, inty1, intx2, inty2, 1, j, "white");
			   drawvector (interp, intx3, inty3, intx2, inty2, 1, j, "white");
			   drawvector (interp, intx0, inty0, intx3, inty3, 1, j, "white");
			   }
			 */
			/* search in pix for candidates in next time step */
			for (j=0;j<n_img;j++)
			{
				zaehler1 = candsearch_in_pix (t4[2][j], nt4[2][j], x1[j], y1[j],
						xl[j], xr[j], yu[j], yd[j], &philf[j]);

				for(k=0; k<4; k++)
				{
					p16[j*4+k].ftnr=t4[2][j][philf[j][k]].tnr;
					if(philf[j][k] != -999) p16[j*4+k].whichcam[j]=1;
					if(philf[j][k] == -999) p16[j*4+k].ftnr=-1;
				}
			}

			/* end of search in pix */

			/* fill and sort candidate struct */
			sortwhatfound(&p16, &zaehler1);
			w = (foundpix *) calloc (zaehler1, sizeof (foundpix));

			if (zaehler1 > 0) count2++;
			for (i=0; i<zaehler1;i++)
			{
				w[i].ftnr = p16[i].ftnr;
				w[i].freq = p16[i].freq;
				for (j=0; j<n_img; j++) w[i].whichcam[j] = p16[i].whichcam[j];
			}
			/*end of candidate struct */

			/* ******************************************************* */
			/* check for what was found */
			for (mm=0; mm<zaehler1;mm++) /* zaehler1-loop */
			{
				/* search for found corr of current the corr in next
				   with predicted location */

				/*reset p16 value for new search */
				for (i=0; i<16;i++)
				{
					p16[i].ftnr=-1;
					p16[i].freq=0;
					for(j=0;j<n_img;j++) p16[i].whichcam[j] =0;
				}

				/* found 3D-position */
				X3=mega[2][w[mm].ftnr].x[0];
				Y3=mega[2][w[mm].ftnr].x[1];
				Z3=mega[2][w[mm].ftnr].x[2];

				if (mega[1][h].prev>=0)
				{
					X5=0.5*(5.0*X3-4.0*X1+X0);
					Y5=0.5*(5.0*Y3-4.0*Y1+Y0);
					Z5=0.5*(5.0*Z3-4.0*Z1+Z0);
				} else {
					X5=2*X3-X1;
					Y5=2*Y3-Y1;
					Z5=2*Z3-Z1;
				}

				searchquader(X5, Y5, Z5, &xr, &xl, &yd, &yu);

				for (j=0;j<n_img;j++)
				{
					img_coord (X5, Y5, Z5, Ex[j],I[j], ap[j], mmp, &xn[j], &yn[j]);
					metric_to_pixel (xn[j], yn[j], imx,imy, pix_x,pix_y, &xn[j], &yn[j], chfield);
					x2[j]=xn[j];
					y2[j]=yn[j];
				}

				/* search for candidates in next time step */
				for (j=0;j<n_img;j++)
				{
					zaehler2 = candsearch_in_pix (t4[3][j], nt4[3][j], x2[j], y2[j],
							xl[j], xr[j], yu[j], yd[j], &philf[j]);

					for(k=0; k<4; k++)
					{
						if( t4[3][j][philf[j][k]].tnr != -1)
						{
							p16[j*4+k].ftnr=t4[3][j][philf[j][k]].tnr;
							if(philf[j][k] != -999) p16[j*4+k].whichcam[j]=1;
							if(philf[j][k] == -999) p16[j*4+k].ftnr=-1;
						}
					}
				}

				/* end of search in pix */
				/* fill and sort candidate struct */

				sortwhatfound(&p16, &zaehler2);
				wn = (foundpix *) calloc (zaehler2, sizeof (foundpix));
				if (zaehler2 > 0) count3++;

				for (i=0; i<zaehler2;i++)
				{
					wn[i].ftnr = p16[i].ftnr;
					wn[i].freq = p16[i].freq;
					for (j=0; j<n_img; j++) wn[i].whichcam[j] = p16[i].whichcam[j];
				}

				/*end of candidate struct */
				/* ************************************************ */
				for (kk=0; kk < zaehler2; kk++)	 /* zaehler2-loop */
				{
					X4=mega[3][wn[kk].ftnr].x[0];
					Y4=mega[3][wn[kk].ftnr].x[1];
					Z4=mega[3][wn[kk].ftnr].x[2];

					okay=0; rr=1000000; quali=0; dl=0;
					acc=2*tpar.dacc;angle=2*tpar.dangle;
					acc0=2*tpar.dacc;angle0=2*tpar.dangle;
					acc1=2*tpar.dacc;angle1=2*tpar.dangle;

					/* displacement check */
					if ( tpar.dvxmin < (X4-X3) && (X4-X3) < tpar.dvxmax &&
							tpar.dvymin < (Y4-Y3) && (Y4-Y3) < tpar.dvymax &&
							tpar.dvzmin < (Z4-Z3) && (Z4-Z3) < tpar.dvzmax )
					{
						okay=1;
						/* end displacement check */

						if ( okay ==1 )
						{
							dl=(sqrt((X1-X3)*(X1-X3)+(Y1-Y3)*(Y1-Y3)+(Z1-Z3)*(Z1-Z3))
									+sqrt((X4-X3)*(X4-X3)+(Y4-Y3)*(Y4-Y3)+(Z4-Z3)*(Z4-Z3)))/2;

							angle_acc(X3, Y3, Z3, X4, Y4, Z4, X5, Y5, Z5, &angle1, &acc1);

							if (mega[1][h].prev>=0)
							{
								angle_acc(X1, Y1, Z1, X2, Y2, Z2, X3, Y3, Z3, &angle0, &acc0);
							} else {
								acc0=acc1; angle0=angle1;
							}

							acc=(acc0+acc1)/2; angle=(angle0+angle1)/2;
							quali=wn[kk].freq+w[mm].freq;
							rr=1000000;

							if ( (acc<tpar.dacc && angle<tpar.dangle) ||	(acc<tpar.dacc/10) )
							{
								rr =(dl/lmax+acc/tpar.dacc + angle/tpar.dangle)/(quali);
								mega[1][h].decis[mega[1][h].inlist]=rr;
								mega[1][h].linkdecis[mega[1][h].inlist]=w[mm].ftnr;
								mega[1][h].inlist++;
								/*
								   printf("alt mit kk h: %d, X3: %6.3f %6.3f %6.3f, dl: %5.3f, ac:	%5.3f, an: %5.3f, quali: %d, rr: %5.3f\n",
								   h, X3, Y3, Z3, dl, acc, angle, quali, rr);
								 */
							}
							okay=0;
						}
					}
				}/* end of zaehler2-loop */
				okay=0;

				/* creating new particle position */
				/* *************************************************************** */

				for (j=0;j<n_img;j++) {
					img_coord (X5, Y5, Z5, Ex[j],I[j], ap[j], mmp, &xn[j], &yn[j]);
					metric_to_pixel (xn[j], yn[j], imx,imy, pix_x,pix_y, &xn[j], &yn[j], chfield);
				}

				/* reset img coord because of n_img smaller 4 */
				for (j=0;j<4;j++)
				{
					x2[j]=-1e10; y2[j]=-1e10;
				}

				/* search for unused candidates in next time step */
				for (j=0;j<n_img;j++)
				{
					/* use fix distance to define xl, xr, yu, yd instead of searchquader */
					xl[j]= xr[j]= yu[j]= yd[j] = 3.0;

					zaehler2 = candsearch_in_pixrest (t4[3][j], nt4[3][j], xn[j], yn[j],
							xl[j], xr[j], yu[j], yd[j], &philf[j]);
					if(zaehler2>0 ) {
						x2[j]=t4[3][j][philf[j][0]].x; y2[j]= t4[3][j][philf[j][0]].y;
					}
				}
				quali=0;

				for (j=0;j<n_img;j++)
				{
					if (x2[j] !=-1e10 && y2[j] != -1e10) {
						pixel_to_metric (x2[j],y2[j], imx,imy, pix_x,pix_y, &x2[j],&y2[j], chfield); quali++;
					}
				}

				if ( quali >= 2)
				{

					X4 = X5; Y4 =Y5; Z4 = Z5;
					invol=0; okay=0;

					det_lsq (Ex, I, ap, mmp,
							x2[0], y2[0], x2[1], y2[1], x2[2], y2[2], x2[3], y2[3], &X4, &Y4, &Z4);

					/* volume check */
					if ( X_lay[0] < X4 && X4 < X_lay[1] &&
							Ymin < Y4 && Y4 < Ymax &&
							Zmin_lay[0] < Z4 && Z4 < Zmax_lay[1])
					{
						invol=1;
					}

					/* displacement check */
					if ( invol==1 &&
							tpar.dvxmin < (X3-X4) && (X3-X4) < tpar.dvxmax &&
							tpar.dvymin < (Y3-Y4) && (Y3-Y4) < tpar.dvymax &&
							tpar.dvzmin < (Z3-Z4) && (Z3-Z4) < tpar.dvzmax ) { okay=1;
						/* end displacement check */
						if (okay == 1)
						{
							rr=1000000; dl=0;
							acc=2*tpar.dacc;angle=2*tpar.dangle;
							angle_acc(X3, Y3, Z3, X4, Y4, Z4, X5, Y5, Z5, &angle, &acc);
							dl=(sqrt((X1-X3)*(X1-X3)+(Y1-Y3)*(Y1-Y3)+(Z1-Z3)*(Z1-Z3))
									+sqrt((X4-X3)*(X4-X3)+(Y4-Y3)*(Y4-Y3)+(Z4-Z3)*(Z4-Z3)))/2;

							if ( (acc<tpar.dacc && angle<tpar.dangle) ||	(acc<tpar.dacc/10) )
							{
								rr =(dl/lmax+acc/tpar.dacc + angle/tpar.dangle)/(quali+w[mm].freq);
								/*
								   printf("neu ohne prev h: %d, X3: %6.3f %6.3f %6.3f, dl: %5.3f, ac:	%5.3f, an: %5.3f, quali: %d, rr: %5.3f\n",
								   h, X4, Y4, Z4, dl, acc, angle, quali+w[mm].freq, rr);
								 */

								mega[1][h].decis[mega[1][h].inlist]=rr;
								mega[1][h].linkdecis[mega[1][h].inlist]=w[mm].ftnr;
								mega[1][h].inlist++;

								if (tpar.add)
								{
									mega[3][m[3]].x[0]=X4;
									mega[3][m[3]].x[1]=Y4;
									mega[3][m[3]].x[2]=Z4;
									mega[3][m[3]].prev= -1;
									mega[3][m[3]].next= -2;
									mega[3][m[3]].prio= 2;




									for (j=0;j<n_img;j++)
									{
										c4[3][m[3]].p[j]=-1;
										if(philf[j][0]!=-999) {
											t4[3][j][philf[j][0]].tnr=m[3];
											c4[3][m[3]].p[j]= philf[j][0];
											c4[3][m[3]].nr=m[3];
										}
									}
									m[3]++; zusatz++; 

								}
							}
						}
						okay=0;
					}
					invol=0;
				}

				quali=0;

				/* end of creating new particle position */
				/* *************************************************************** */


				/* try to link if kk is not found/good enough and prev exist */
				if ( mega[1][h].inlist == 0 && mega[1][h].prev>=0 )
				{
					acc=2*tpar.dacc;angle=2*tpar.dangle;

					if ( tpar.dvxmin < (X3-X1) && (X3-X1) < tpar.dvxmax &&
							tpar.dvymin < (Y3-Y1) && (Y3-Y1) < tpar.dvymax &&
							tpar.dvzmin < (Z3-Z1) && (Z3-Z1) < tpar.dvzmax )
					{
						okay=1;

						if ( okay ==1 )
						{
							rr=1000000; quali=0;
							quali=w[mm].freq;
							angle_acc(X1, Y1, Z1, X2, Y2, Z2, X3, Y3, Z3, &angle, &acc);
							dl=(sqrt((X1-X0)*(X1-X0)+(Y1-Y0)*(Y1-Y0)+(Z1-Z0)*(Z1-Z0))
									+sqrt((X1-X3)*(X1-X3)+(Y1-Y3)*(Y1-Y3)+(Z1-Z3)*(Z1-Z3)))/2;

							if ( (acc<tpar.dacc && angle<tpar.dangle) ||	(acc<tpar.dacc/10) )
							{
								rr =(dl/lmax+acc/tpar.dacc + angle/tpar.dangle)/(quali);
								mega[1][h].decis[mega[1][h].inlist]=rr;
								mega[1][h].linkdecis[mega[1][h].inlist]=w[mm].ftnr;
								mega[1][h].inlist++;
								/*				
												printf("alt ohne kk h: %d, X3: %6.3f %6.3f %6.3f, dl: %5.3f, ac:	%5.3f, an: %5.3f, quali: %d, rr: %5.3f\n",
												h, X3, Y3, Z3, dl, acc, angle, quali, rr);
								 */		 
							}
						}
					}
				}
				okay=0;

				free(wn);
			} /* end of zaehler1-loop */

			/* ******************************************************************************/
			/* begin of inlist still zero */
			if (tpar.add) {
				if ( mega[1][h].inlist == 0 && mega[1][h].prev>=0 )
				{
					/*
					   printf("h: %d\n", h);
					   printf("X0: %6.3f	%6.3f	%6.3f, prev: %d\n", X0, Y0, Z0, mega[1][h].prev);
					   printf("X1: %6.3f	%6.3f	%6.3f, h: %d\n", X1, Y1, Z1, h);
					   printf("X2: %6.3f	%6.3f	%6.3f\n", X2, Y2, Z2);
					   printf("X3: %6.3f	%6.3f	%6.3f, ftnr: %d\n", X3, Y3, Z3, w[mm].ftnr);
					 */
					for (j=0;j<n_img;j++)
					{
						img_coord (X2, Y2, Z2, Ex[j],I[j], ap[j], mmp, &xn[j], &yn[j]);
						metric_to_pixel (xn[j], yn[j], imx,imy, pix_x,pix_y, &xn[j], &yn[j], chfield);
					}

					/* reset img coord because of n_img smaller 4 */
					for (j=0;j<4;j++)
					{
						x2[j]=-1e10; y2[j]=-1e10;
					}

					/* search for unused candidates in next time step */
					for (j=0;j<n_img;j++)
					{
						/* use fix distance to define xl, xr, yu, yd instead of searchquader */
						xl[j]= xr[j]= yu[j]= yd[j] = 3.0;

						zaehler2 = candsearch_in_pixrest (t4[2][j], nt4[2][j], xn[j], yn[j],
								xl[j], xr[j], yu[j], yd[j], &philf[j]);
						if(zaehler2>0 )
						{
							x2[j]=t4[2][j][philf[j][0]].x; y2[j]= t4[2][j][philf[j][0]].y;
						}
					}
					quali=0;

					for (j=0;j<n_img;j++)
					{
						if (x2[j] !=-1e10 && y2[j] != -1e10) {
							pixel_to_metric (x2[j],y2[j], imx,imy, pix_x,pix_y, &x2[j],&y2[j], chfield); quali++;
						}
					}

					if (quali>=2) {
						X3 = X2; Y3 =Y2; Z3 = Z2;
						invol=0; okay=0;

						det_lsq (Ex, I, ap, mmp,
								x2[0], y2[0], x2[1], y2[1], x2[2], y2[2], x2[3], y2[3], &X3, &Y3, &Z3);

						/* in volume check */
						if ( X_lay[0] < X3 && X3 < X_lay[1] &&
								Ymin < Y3 && Y3 < Ymax &&
								Zmin_lay[0] < Z3 && Z3 < Zmax_lay[1])
						{
							invol=1;
						}

						/* displacement check */
						if ( invol==1 &&
								tpar.dvxmin < (X2-X3) && (X2-X3) < tpar.dvxmax &&
								tpar.dvymin < (Y2-Y3) && (Y2-Y3) < tpar.dvymax &&
								tpar.dvzmin < (Z2-Z3) && (Z2-Z3) < tpar.dvzmax )
						{
							okay=1;
							/* end displacement check */
							if (okay == 1) {
								rr=1000000; dl=0;
								acc=2*tpar.dacc;angle=2*tpar.dangle;
								angle_acc(X1, Y1, Z1, X2, Y2, Z2, X3, Y3, Z3, &angle, &acc);
								dl=(sqrt((X1-X0)*(X1-X0)+(Y1-Y0)*(Y1-Y0)+(Z1-Z0)*(Z1-Z0))
										+sqrt((X1-X3)*(X1-X3)+(Y1-Y3)*(Y1-Y3)+(Z1-Z3)*(Z1-Z3)))/2;

								if ( (acc<tpar.dacc && angle<tpar.dangle) ||	(acc<tpar.dacc/10) )
								{
									rr =(dl/lmax+acc/tpar.dacc + angle/tpar.dangle)/(quali);

									mega[2][m[2]].x[0]=X3;
									mega[2][m[2]].x[1]=Y3;
									mega[2][m[2]].x[2]=Z3;
									mega[2][m[2]].prev= -1;
									mega[2][m[2]].next= -2;
									mega[2][m[2]].prio= 2;

									mega[1][h].decis[mega[1][h].inlist]=rr;
									mega[1][h].linkdecis[mega[1][h].inlist]=m[2];
									mega[1][h].inlist++;



									for (j=0;j<n_img;j++)
									{
										c4[2][m[2]].p[j]=-1;
										if(philf[j][0]!=-999) {
											t4[2][j][philf[j][0]].tnr=m[2];
											c4[2][m[2]].p[j]= philf[j][0];
											c4[2][m[2]].nr=m[2];
										}
									}
									m[2]++; zusatz++;
								}
							}
							okay=0;
						}
						invol=0;
					}
				}
			}
			/* end of inlist still zero */
			/***********************************/

			free(w);
		} /* end of h-loop */

		/* sort decis and give preliminary "finaldecis"	*/
		for (h=0;h<m[1];h++)
		{
			if(mega[1][h].inlist > 0 )
			{
				sort(mega[1][h].inlist, &mega[1][h].decis, &mega[1][h].linkdecis);
				mega[1][h].finaldecis=mega[1][h].decis[0];
				mega[1][h].next=mega[1][h].linkdecis[0];
			}
		}

		/* create links with decision check */
		for (h=0;h<m[1];h++)
		{
			if(mega[1][h].inlist > 0 ) {
				/* best choice wasn't used yet, so link is created */
				if ( mega[2][mega[1][h].next].prev == -1) {	mega[2][mega[1][h].next].prev=h; }

				/* best choice was already used by mega[2][mega[1][h].next].prev */
				else {
					/* check which is the better choice */
					if ( mega[1][mega[2][mega[1][h].next].prev].finaldecis > mega[1][h].finaldecis)
					{
						/*
						   printf("h ist besser, h: %4d jetzt: %5.3f vorher: %d %5.3f\n",
						   h, mega[1][h].finaldecis, mega[1][mega[2][mega[1][h].next].prev].next,
						   mega[1][mega[2][mega[1][h].next].prev].finaldecis); */

						if (mega[1][mega[2][mega[1][h].next].prev].inlist>1) {
							/*
							   printf("zweite Wahl fuer %d waere: %5.3f\n",
							   mega[1][mega[2][mega[1][h].next].prev].linkdecis[1],
							   mega[1][mega[2][mega[1][h].next].prev].decis[1]); */ }

							/* remove link with prev */
							mega[1][mega[2][mega[1][h].next].prev].next= -2;
						mega[2][mega[1][h].next].prev=h;
					}
					else {
						/*
						   printf("h ist schlechter, h: %4d jetzt: %5.3f vorher: %d %5.3f\n",
						   h, mega[1][h].finaldecis, mega[1][mega[2][mega[1][h].next].prev].next,
						   mega[1][mega[2][mega[1][h].next].prev].finaldecis); */
						if (mega[1][h].inlist>1) {
							/*
							   printf("zweite Wahl fuer %d waere: %5.3f\n",
							   mega[1][h].linkdecis[1],
							   mega[1][h].decis[1]);	*/ }

							mega[1][h].next=-2;}
				}
			}
			if (mega[1][h].next != -2 ) count1++;
		} /* end of creation of links with decision check */

		/* ******** Draw links now ******** */
		/* ChrisB: can probably comment out this whole block:*/
#if 0
		if (display)
		{
			for (h=0;h<m[1];h++)
			{
				if(mega[1][h].next != -2 ) {

					strcpy(buf,"");
					sprintf(buf ,"green");

					for (j=0;j<n_img;j++)
					{
						if(c4[1][h].p[j]>0 && c4[2][mega[1][h].next].p[j]>0) {
							xp[j]=t4[1][j][c4[1][h].p[j]].x;
							yp[j]=t4[1][j][c4[1][h].p[j]].y;
							xc[j]=t4[2][j][c4[2][mega[1][h].next].p[j]].x;
							yc[j]=t4[2][j][c4[2][mega[1][h].next].p[j]].y;
							predict (xp[j], yp[j], xc[j], yc[j], &xn[j], &yn[j]);

							if ( ( fabs(xp[j]-zoom_x[j]) < imx/(2*zoom_f[j]))
									&& (fabs(yp[j]-zoom_y[j]) < imy/(2*zoom_f[j])))
							{
								strcpy(val,"");
								sprintf(val ,"orange");

								intx0 = (int)(imx/2+zoom_f[j]*(xp[j]-zoom_x[j]));
								inty0 = (int)(imy/2+zoom_f[j]*(yp[j]-zoom_y[j]));
								intx1 = (int)(imx/2+zoom_f[j]*(xc[j]-zoom_x[j]));
								inty1 = (int)(imy/2+zoom_f[j]*(yc[j]-zoom_y[j]));
								intx2 = (int)(imx/2+zoom_f[j]*(xn[j]-zoom_x[j]));
								inty2 = (int)(imy/2+zoom_f[j]*(yn[j]-zoom_y[j]));

								/*drawcross(interp,intx0,inty0,cr_sz,j,"green");
								  drawcross(interp,intx1,inty1,cr_sz+1,j,"yellow");
								  drawcross(interp,intx2,inty2,cr_sz+1,j,"white");
								  drawvector (interp, intx0, inty0, intx1, inty1, 2, j, buf);
								  drawvector (interp, intx1, inty1, intx2, inty2, 1, j, "white");*/

								if (mega[1][h].finaldecis> 0.2) {
									/*draw_pnr ( interp, intx0, inty0, h, j, "white");
									  draw_pnr ( interp, intx0, inty0+10, mega[1][h].next, j, val);
									  draw_value (interp, intx0, inty0 + 20,mega[1][h].finaldecis, j, val); */
								}
							}
						}
					}
				}
			}
		}
#endif

		/* ******** End of Draw links now ******** */
		if( verbose ) {
			printf ( "step: %d, curr: %d, next: %d, links: %d, lost: %d, add: %d\n",
					step, m[1], m[2], count1, m[1]-count1, zusatz);
		}

		/* for the average of particles and links */
		npart = npart + m[1];
		nlinks = nlinks + count1;
		
		

		/*Tcl_SetVar(interp, "tbuf", buf, TCL_GLOBAL_ONLY);
		  Tcl_Eval(interp, ".text delete 3");
		  Tcl_Eval(interp, ".text insert 3 $tbuf");

		  Tcl_Eval(interp, "update idletasks");*/

		rotate_dataset();
		write_ascii_data(step);
		write_added(step);
		if(step< seq_last-2)
		{
			read_ascii_data(step+3);
		}


	} /* end of sequence loop */

	printf( "\r" );

	// close dynamic tracking file:
	if( !staticTracking ) {
		freeTrackingParameters( tplist );
		//fclose( fDynTPar );
	}

	/* average of all steps */
	npart /= (seq_last-seq_first);
	nlinks /= (seq_last-seq_first);
	printf ("Average over sequence, particles: %5.1f, links: %5.1f, lost: %5.1f\n",
			npart, nlinks, npart-nlinks);


	rotate_dataset();
	write_ascii_data(step);
	write_added(step);

	/* reset of display flag */
	display = 1;

	return 0;
}






/* track backwards */

/*int trackback_c (ClientData clientData, Tcl_Interp* interp,
  int argc, const char** argv)*/

int trackback_c(  )
{
	char	buf[256];
	int i, j, h, k, step, okay=0, invol=0;
	int zaehler1, philf[4][4];
	int count1=0, count2=0, zusatz=0;
	int quali=0;
	double x2[4], y2[4], angle, acc, lmax, dl;
	double xr[4], xl[4], yd[4], yu[4];
	double X1, Y1, Z1, X0, Y0, Z0, X2, Y2, Z2, X5, Y5, Z5;
	double X3, Y3, Z3, X4, Y4, Z4, X6, Y6, Z6;
	double xn[4], yn[4];
	double rr, Ymin=0, Ymax=0;
	double npart=0, nlinks=0;
	foundpix *w, p16[16];

	trackparameters *tplist;		// array of tracking parameters for sequence
									// (used in dynamic tracking only).

	// load the dynamic tracking parameters into a buffer:

	//Tcl_Eval(interp, ".text delete 2");
	//Tcl_Eval(interp, ".text insert 2 \"Track established correspondences\"");

	/* read data */
	readseqtrackcrit ();
	
	// ChrisB: if we are using dynamic tracking - get the relavent lines from
	// the dynamic tracking file:
	if( !staticTracking ) {
		loadTrackingParameters( &tplist, seq_first, seq_last );
		if( !tplist ) {
			fprintf( stderr, "Failed to load tracking parameters\n" );
			return -1;
		}
	}

	/*Alloc space, if checkflag for mega, c4, t4 is zero */
	if (!trackallocflag)
	{
		for (i=0; i<4; i++)
		{
			mega[i]=(P *) calloc(sizeof(P),M);
			c4[i]=(corres *) calloc(sizeof(corres),M);
			for (k=0; k<4; k++) { t4[i][k]=(target *) calloc(sizeof (target),M);}
		}
		trackallocflag=1;
	}

	/*load again first data sets*/
	step = seq_last;
	read_ascii_datanew(step);
	rotate_dataset();
	read_ascii_datanew(step-1);
	rotate_dataset();
	read_ascii_datanew(step-2);
	rotate_dataset();
	read_ascii_datanew(step-3);

	// this needs to be in main loop as uses data from tpar struct.
#if 0
	lmax=sqrt((tpar.dvxmin-tpar.dvxmax)*(tpar.dvxmin-tpar.dvxmax)
			+(tpar.dvymin-tpar.dvymax)*(tpar.dvymin-tpar.dvymax)
			+(tpar.dvzmin-tpar.dvzmax)*(tpar.dvzmin-tpar.dvzmax));
#endif

	volumedimension (&X_lay[1], &X_lay[0], &Ymax, &Ymin, &Zmax_lay[1], &Zmin_lay[0]);

	/* sequence loop */
	for (step = seq_last-1; step > seq_first; step--)
	{
		if( verbose ) {
			;//printf ( "Time step: %d, seqnr: %d, Particle info:\n", step- seq_first, step);
		} else {
			printf( "\rProcessing: step %03d of %03d         ",
					step - seq_first,
					seq_last - seq_first - 1 );
			fflush(stdout);
		}
		//Tcl_SetVar(interp, "tbuf", buf, TCL_GLOBAL_ONLY);
		//Tcl_Eval(interp, ".text delete 2");
		//Tcl_Eval(interp, ".text insert 2 $tbuf");

		if( !staticTracking ) {
			// ChrisB: read the dynamic tracking parameters for this timestep:
			getCurrentTPar( tplist, &tpar, step );
		}	

#if 0
		// ChrisB; debugging:
		fprintf( stderr, "Dynamic TPar line %d {%lf %lf %lf %lf %lf %lf %lf %lf %d}\n",
				step,
				tpar.dvxmin,
				tpar.dvxmax,
				tpar.dvymin,
				tpar.dvymax,
				tpar.dvzmin,
				tpar.dvzmax,
				tpar.dangle,
				tpar.dacc,
				tpar.add
			   );
#endif
		
		
		// ChrisB: move this block from outside loop as it needs the dynamic tracking parameters
		// (that are read each iteration of the loop):
		lmax=sqrt((tpar.dvxmin-tpar.dvxmax)*(tpar.dvxmin-tpar.dvxmax)
				+(tpar.dvymin-tpar.dvymax)*(tpar.dvymin-tpar.dvymax)
				+(tpar.dvzmin-tpar.dvzmax)*(tpar.dvzmin-tpar.dvzmax));

		for (h=0; h<m[1]; h++)
		{
			if (mega[1][h].next>=0 && mega[1][h].prev==-1) {
				X1=Y1=Z1=X0=Y0=Z0=X2=Y2=Z2=X5=Y5=Z5=X3=Y3=Z3=X4=Y4=Z4=X6=Y6=Z6=-999;

				mega[1][h].inlist=0;
				for (i=0; i<16;i++)
				{
					p16[i].ftnr=-1;
					p16[i].freq=0;
					for(j=0;j<n_img;j++) p16[i].whichcam[j] =0;
				}
				/* 3D-position */
				X1=mega[1][h].x[0];
				Y1=mega[1][h].x[1];
				Z1=mega[1][h].x[2];

				/* use information from previous to locate new search position
				   and to calculate values for search area */
				X0=mega[0][mega[1][h].next].x[0];
				Y0=mega[0][mega[1][h].next].x[1];
				Z0=mega[0][mega[1][h].next].x[2];
				X2=2*X1-X0;
				Y2=2*Y1-Y0;
				Z2=2*Z1-Z0;

				for (j=0; j<n_img; j++)
				{
					img_coord (X2, Y2, Z2, Ex[j],I[j], ap[j], mmp, &xn[j], &yn[j]);
					metric_to_pixel (xn[j], yn[j], imx,imy, pix_x,pix_y, &xn[j], &yn[j], chfield);
				}

				/* calculate searchquader and reprojection in image space */
				searchquader(X2, Y2, Z2, &xr, &xl, &yd, &yu);

				/* search in pix for candidates in next time step */
				for (j=0; j<n_img; j++)
				{
					/* xl[j]/=5; xr[j]/=5; yu[j]/=5; yd[j]/=5; */ /* reduced search area */
					zaehler1 = candsearch_in_pix (t4[2][j], nt4[2][j], xn[j], yn[j],
							xl[j], xr[j], yu[j], yd[j], &philf[j]);
					for(k=0; k<4; k++)
					{
						if( zaehler1>0) {
							p16[j*4+k].ftnr=t4[2][j][philf[j][k]].tnr;
							if(philf[j][k] != -999) p16[j*4+k].whichcam[j]=1;
							if(philf[j][k] == -999) p16[j*4+k].ftnr=-1;
						}
					}
				}

				/* fill and sort candidate struct */
				sortwhatfound(&p16, &zaehler1);
				w = (foundpix *) calloc (zaehler1, sizeof (foundpix));

				/*end of candidate struct */
				if (zaehler1 > 0) count2++;
				for (i=0; i<zaehler1;i++)
				{
					w[i].ftnr = p16[i].ftnr;
					w[i].freq = p16[i].freq;
					for (j=0; j<n_img; j++) w[i].whichcam[j] = p16[i].whichcam[j];
				}

				if (zaehler1 > 0) for (i=0; i<zaehler1;i++) {
					X3=mega[2][w[i].ftnr].x[0];
					Y3=mega[2][w[i].ftnr].x[1];
					Z3=mega[2][w[i].ftnr].x[2];

					okay=0; acc=2*tpar.dacc;angle=2*tpar.dangle;rr=1000000;quali=0; dl=0;

					/* displacement check */
					if ( tpar.dvxmin < (X1-X3) && (X1-X3) < tpar.dvxmax &&
							tpar.dvymin < (Y1-Y3) && (Y1-Y3) < tpar.dvymax &&
							tpar.dvzmin < (Z1-Z3) && (Z1-Z3) < tpar.dvzmax ) { okay=1;
						/* end displacement check */

						if ( okay ==1 ) {
							dl=(sqrt((X1-X0)*(X1-X0)+(Y1-Y0)*(Y1-Y0)+(Z1-Z0)*(Z1-Z0))
									+sqrt((X1-X3)*(X1-X3)+(Y1-Y3)*(Y1-Y3)+(Z1-Z3)*(Z1-Z3)))/2;

							quali=w[i].freq;
							angle_acc(X1, Y1, Z1, X2, Y2, Z2, X3, Y3, Z3, &angle, &acc);

							/* *********************check link *****************************/
							if ( (acc<tpar.dacc && angle<tpar.dangle) ||	(acc<tpar.dacc/10) )
							{
								rr =(dl/lmax+acc/tpar.dacc + angle/tpar.dangle)/quali;

								mega[1][h].decis[mega[1][h].inlist]=rr;
								mega[1][h].linkdecis[mega[1][h].inlist]=w[i].ftnr;
								mega[1][h].inlist++;
								/*
								   printf("h: %d, old and ok: X3: %6.3f %6.3f %6.3f, ftnr: %d, prev: %d, next: %d, angle: %6.3f, acc: %6.3f, dl: %6.3f, rr: %6.3f, quali: %d\n", h, X3, Y3, Z3, w[i].ftnr, mega[2][w[i].ftnr].prev, mega[2][w[i].ftnr].next,angle, acc, dl, rr, quali);
								 */
							}
						}
					} okay=0;
				}

				free(w);
				/******************/
				quali=0;
				/* reset img coord because of n_img smaller 4 */
				for (j=0;j<4;j++) { x2[j]=-1e10; y2[j]=-1e10;}

				/* if old wasn't found try to create new particle position from rest */
				if (tpar.add) {
					if ( mega[1][h].inlist == 0)
					{
						for (j=0;j<n_img;j++)
						{
							/* use fix distance to define xl, xr, yu, yd instead of searchquader */
							xl[j]= xr[j]= yu[j]= yd[j] = 3.0;

							zaehler1 = candsearch_in_pixrest (t4[2][j], nt4[2][j], xn[j], yn[j],
									xl[j], xr[j], yu[j], yd[j], &philf[j]);
							if(zaehler1>0 ) { x2[j]=t4[2][j][philf[j][0]].x; y2[j]=t4[2][j][philf[j][0]].y; }
						}

						for (j=0;j<n_img;j++)
						{
							if (x2[j] !=-1e10 && y2[j] != -1e10) {
								pixel_to_metric (x2[j],y2[j], imx,imy, pix_x,pix_y, &x2[j],&y2[j], chfield); quali++;
							}
						}

						if (quali>=2) {
							X3 = X2; Y3 =Y2; Z3 = Z2;
							invol=0; okay=0;

							det_lsq (Ex, I, ap, mmp,
									x2[0], y2[0], x2[1], y2[1], x2[2], y2[2], x2[3], y2[3], &X3, &Y3, &Z3);

							/* volume check */
							if ( X_lay[0] < X3 && X3 < X_lay[1] &&
									Ymin < Y3 && Y3 < Ymax &&
									Zmin_lay[0] < Z3 && Z3 < Zmax_lay[1]) {invol=1;}

							okay=0; acc=2*tpar.dacc;angle=2*tpar.dangle;rr=1000000; dl=0;

							/* displacement check */
							if ( invol==1 &&
									tpar.dvxmin < (X1-X3) && (X1-X3) < tpar.dvxmax &&
									tpar.dvymin < (Y1-Y3) && (Y1-Y3) < tpar.dvymax &&
									tpar.dvzmin < (Z1-Z3) && (Z1-Z3) < tpar.dvzmax ) { okay=1;
								/* end displacement check */

								if ( okay ==1 ) {
									dl=(sqrt((X1-X0)*(X1-X0)+(Y1-Y0)*(Y1-Y0)+(Z1-Z0)*(Z1-Z0))
											+sqrt((X1-X3)*(X1-X3)+(Y1-Y3)*(Y1-Y3)+(Z1-Z3)*(Z1-Z3)))/2;

									angle_acc(X1, Y1, Z1, X2, Y2, Z2, X3, Y3, Z3, &angle, &acc);

									if ( (acc<tpar.dacc && angle<tpar.dangle) || (acc<tpar.dacc/10) )
									{
										rr =(dl/lmax+acc/tpar.dacc + angle/tpar.dangle)/(quali);

										mega[2][m[2]].x[0]= X3;
										mega[2][m[2]].x[1]= Y3;
										mega[2][m[2]].x[2]= Z3;
										mega[2][m[2]].prev= -1;
										mega[2][m[2]].next= -2;
										mega[2][m[2]].prio= 2;
										mega[1][h].decis[mega[1][h].inlist]=rr;
										mega[1][h].linkdecis[mega[1][h].inlist]=m[2];

										for (j=0;j<n_img;j++)
										{
											c4[2][m[2]].p[j]=-1;
											if(philf[j][0]!=-999)
											{
												t4[2][j][philf[j][0]].tnr=m[2];
												c4[2][m[2]].p[j]= philf[j][0];
												c4[2][m[2]].nr=m[2];
											}
										}
										mega[1][h].inlist++;
										m[2]++;
									}
								} okay =0; }
								invol=0;
						}
					}
				} /* end of if old wasn't found try to create new particle position from rest */
			}
		} /* end of h-loop */

		/* sort decis	*/
		for (h=0;h<m[1];h++)
		{
			if(mega[1][h].inlist > 0 ) { sort(mega[1][h].inlist, &mega[1][h].decis, &mega[1][h].linkdecis); }
		}

		/* create links with decision check */
		count1=0; zusatz=0;
		for (h=0;h<m[1];h++)
		{
			if (mega[1][h].inlist > 0 ) {
				/* if old/new and unused prev == -1 and next == -2 link is created */
				if ( mega[2][mega[1][h].linkdecis[0]].prev == -1 && mega[2][mega[1][h].linkdecis[0]].next == -2 )
				{
					mega[1][h].finaldecis=mega[1][h].decis[0];
					mega[1][h].prev=mega[1][h].linkdecis[0];
					mega[2][mega[1][h].prev].next=h;
					zusatz++;
				}

				/* old which link to prev has to be checked */
				if ( mega[2][mega[1][h].linkdecis[0]].prev != -1 && mega[2][mega[1][h].linkdecis[0]].next == -2 )
				{
					X0=mega[0][mega[1][h].next].x[0];
					Y0=mega[0][mega[1][h].next].x[1];
					Z0=mega[0][mega[1][h].next].x[2];

					X1=mega[1][h].x[0];
					Y1=mega[1][h].x[1];
					Z1=mega[1][h].x[2];

					X3=mega[2][mega[1][h].linkdecis[0]].x[0];
					Y3=mega[2][mega[1][h].linkdecis[0]].x[1];
					Z3=mega[2][mega[1][h].linkdecis[0]].x[2];

					X4=mega[3][mega[2][mega[1][h].linkdecis[0]].prev].x[0];
					Y4=mega[3][mega[2][mega[1][h].linkdecis[0]].prev].x[1];
					Z4=mega[3][mega[2][mega[1][h].linkdecis[0]].prev].x[2];

					X5=0.5*(5.0*X3-4.0*X1+X0);
					Y5=0.5*(5.0*Y3-4.0*Y1+Y0);
					Z5=0.5*(5.0*Z3-4.0*Z1+Z0);

					acc=2*tpar.dacc;angle=2*tpar.dangle;
					angle_acc(X3, Y3, Z3, X4, Y4, Z4, X5, Y5, Z5, &angle, &acc);
					/*
					   printf("check X0: %6.3f %6.3f %6.3f\n", X0, Y0, Z0);
					   printf("check X1: %6.3f %6.3f %6.3f\n", X1, Y1, Z1);
					   printf("check X3: %6.3f %6.3f %6.3f\n", X3, Y3, Z3);
					   printf("check X4: %6.3f %6.3f %6.3f\n", X4, Y4, Z4);
					   printf("check X5: %6.3f %6.3f %6.3f\n", X5, Y5, Z5);
					   printf("check old, new: X3: %6.3f, Y3: %6.3f, Z3: %6.3f, angle: %6.3f, acc: %6.3f\n", X3, Y3, Z3, angle, acc);
					 */
					if ( (acc<tpar.dacc && angle<tpar.dangle) ||	(acc<tpar.dacc/10) )
					{
						mega[1][h].finaldecis=mega[1][h].decis[0];
						mega[1][h].prev=mega[1][h].linkdecis[0];
						mega[2][mega[1][h].prev].next=h;
						zusatz++;
					}
				}
			}

			if (mega[1][h].prev != -1 ) count1++;
		} /* end of creation of links with decision check */

		if( verbose ) {
			printf ( "step: %d, curr: %d, next: %d, links: %d, lost: %d, add: %d\n",
					step, m[1], m[2], count1, m[1]-count1, zusatz);
		}

		/* for the average of particles and links */
		npart = npart + m[1];
		nlinks = nlinks + count1;
		/*
		   printf("%s\n", buf);
		 */

		/*Tcl_SetVar(interp, "tbuf", buf, TCL_GLOBAL_ONLY);
		  cl_Eval(interp, ".text delete 3");
		  Tcl_Eval(interp, ".text insert 3 $tbuf");
		  Tcl_Eval(interp, "update idletasks");*/

		rotate_dataset();
		write_ascii_datanew(step);
		write_addedback(step);
		if(step> seq_first+2) { read_ascii_datanew(step-3); }

	} /* end of sequence loop */

	printf( "\r" );
	freeTrackingParameters( tplist );

	
	/* average of all steps */
	npart /= (seq_last-seq_first-1);
	nlinks /= (seq_last-seq_first-1);
	printf ("Average over sequence, particles: %5.1f, links: %5.1f, lost: %5.1f\n",
			npart, nlinks, npart-nlinks);

	rotate_dataset();
	write_ascii_datanew(step);
	write_addedback(step);

	/* reset of display flag */
	display = 1;
	return 0;
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              track/track_main.c                                                                                  0000644 0001131 0001131 00000006103 10303614263 013074  0                                                                                                    ustar   amns                            amns                                                                                                                                                                                                                   /**********************************************************************\
 * Name:           track_main 
 *
 * Description:    Entry point for tracking module of the jw_ptv
 *                 particle tracking utility. Originally, sequencing and
 *                 tracking were performed by the same executable under
 *                 a Tcl/Tk front end.
 *
 *                 This code is part of an executable that contains only
 *                 the tracking code in command line form.
 *               
 * Author:         Chris Bunney, University of Plymouth
 *
 * Created:        July 2005
\**********************************************************************/
#include "ptv.h"

int main( int argc, char **argv )
{
	char opt;
	extern char *optarg;
	int trackb = 0;		// 0 = forwards only, 1 = forward + back, 2 = back only
	
	strcpy( szDynTParFilename, "parameters/dynamic_track.par" );
	useCompression = 0;
	staticTracking = 0;
	verbose = 0;
		
	while( ( opt = getopt( argc, argv, "bBcp:svh" ) ) != -1 ) {
		switch( opt ) {
			case 'c':
			fprintf( stderr, "Enabling compression.\n" );
			useCompression = 1;
			break;

			case 'p':
			fprintf( stderr, "Using tracking file {%s}.\n", optarg );
			strcpy( szDynTParFilename, optarg );
			break;

			case 's':
			fprintf( stderr, "Using static tracking parameters.\n" );
			staticTracking = 1;
			break;
			
			case 'b':
			fprintf( stderr, "Enabling tracking backwards.\n" );
			trackb = 1;
			break;

			case 'B':
			fprintf( stderr, "Running backwards tacking ONLY.\n" );
			trackb = 2;
			break;

			case 'v':
			fprintf( stderr, "Producing verbose output.\n" );
			verbose = 1;
			break;

			case 'h':
			default:
			printf( "\nUsage: track -[bBcpsvh]\n\n"
					"Options:\n"
					"   -b\n"
					"        Run backwards tracking after forward track.\n\n"
					"   -B\n"
					"        Run backwards tracking ONLY.\n\n"
					"   -c\n"
					"        Enable gzip compression of output files.\n"
					"        This includes all files written to the res/\n"
					"        directory and img/*_target files.\n"
					"        Note: any input file already written by\n"
					"        'sequence' are also expected to be in gzip format.\n\n"
					"   -p <parameter file>\n"
					"        Use the dynamic tracking parameter file specified\n"
					"        in <parameter file> instead of the default file.\n"
					"        Default file is: 'parameters/dynamic_track.par'.\n"
					"        Flag has no effect is -s flag is specified.\n\n"
					"   -s\n"
					"        Use static tracking parameters as defined in the\n"
					"        file 'parameters/track.par'. Overrides -p flag.\n\n"
					"   -v\n"
					"        Print extra verbose output\n\n"
					"   -h\n"
					"        Prints this usage informaton.\n\n"
				  );

			return 0;
		}
	}

	printf( "\n" );

	init_proc_c();
	start_proc_c();	// this is obviously still needed

	if( trackb < 2 ) {
		fprintf( stderr, "Tracking forwards\n" );
		trackcorr_c();
	}

	// track backwards if -b flag set:
	if( trackb > 0 ) {
		fprintf( stderr, "Tracking backwards\n" );
		trackback_c();
	}

	// free up memory and quit:
	quit_proc_c();
	return 0;
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                             track/ttools.c                                                                                      0000644 0001131 0001131 00000024352 10303615155 012317  0                                                                                                    ustar   amns                            amns                                                                                                                                                                                                                   /****************************************************************************

Author/Copyright:      	Jochen Willneff

Address:	      	Institute of Geodesy and Photogrammetry
		       	ETH - Hoenggerberg
		       	CH - 8093 Zurich

Creation Date:	       	end of 99 ...going on

Description:	       	different search routines and track tools

Routines contained:     pix_in_next, candsearch_in_pix, searchposition,
                        predict, readseqtrackcrit, readtrackdata,
                        searchquader

Amedned:                July 2005, Chris Bunney, University of Plymouth.
                        Implemented facility to read a file containing
                        dynamic tracking parameters.
****************************************************************************/
#include "ptv.h"

int pix_in_next (next, num, x, y, dl, dr, du, dd, found)
target  next[];
int     num;
double  x, y, dl, dr, du, dd;
int found[POSI];
{
  /* search of POSI near candidates in targetlist */

  int  j, j0, dj;
  int  zaehler=0;
  double  d, xmin, xmax, ymin, ymax;
  double dcand[POSI];
  int cand[POSI];
  xmin = x - dl;  xmax = x + dr;  ymin = y - du;  ymax = y + dd;


  /* binarized search for start point of candidate search */
  for (j0=num/2, dj=num/4; dj>1; dj/=2)
    {
      if (next[j0].y < ymin) j0 += dj;
      else j0 -= dj;
    }

  j0 -= 12;  if (j0 < 0)  j0 = 0;	       	/* due to trunc */
  for (j=j0; j<num; j++)		       	/* candidate search */
    {
      if (next[j].y > ymax)  break;	       	/* finish search */
      if (next[j].x > xmin  &&  next[j].x < xmax && next[j].y > ymin  &&  next[j].y < ymax && next[j].tnr>0)
	{
	  d = sqrt ((x-next[j].x)*(x-next[j].x) + (y-next[j].y)*(y-next[j].y));
	  cand[zaehler]=next[j].tnr; dcand[zaehler]=d;
	  zaehler++;
	}
    }

  return (zaehler);
}


int candsearch_in_pix (next, num, x, y, dl, dr, du, dd, p)
target  next[];
int     num;
double  x, y, dl, dr, du, dd;
int p[4];
{
  /* search of four near candidates in targetlist */

  int  	  j, j0, dj, pnr = -999;
  int  zaehler=0, p1, p2, p3, p4;
  double  d, dmin=1e20, xmin, xmax, ymin, ymax;
  double d1, d2, d3, d4;
  xmin = x - dl;  xmax = x + dr;  ymin = y - du;  ymax = y + dd;

  if(xmin<0.0) xmin=0.0;
  if(xmax>imx) xmax=imx;
  if(ymin<0.0) ymin=0.0;
  if(ymax>imy) ymax=imy;

  if(x<0.0) x=0.0;
  if(x>imx) x=imx;
  if(y<0.0) y=0.0;
  if(y>imy) y=imy;

  p1 = p2 = p3 = p4 = -999;
  d1 = d2 = d3 = d4 = dmin;

  if (x>=0.0 && x<=imx ) { if (y>=0.0 && y<=imy ) {

  /* binarized search for start point of candidate search */
  for (j0=num/2, dj=num/4; dj>1; dj/=2)
    {
      if (next[j0].y < ymin) j0 += dj;
      else j0 -= dj;
    }

  j0 -= 12;  if (j0 < 0)  j0 = 0;	       	/* due to trunc */
  for (j=j0; j<num; j++)		       	/* candidate search */
    {
      if (next[j].tnr != -1 ) {
	if (next[j].y > ymax )  break;	       	/* finish search */
	if (next[j].x > xmin  &&  next[j].x < xmax && next[j].y > ymin  &&  next[j].y < ymax )
	  {
	    d = sqrt ((x-next[j].x)*(x-next[j].x) + (y-next[j].y)*(y-next[j].y));

	    if (d < dmin) { dmin = d; pnr = j;
	    }
	    if ( d < d1 )
	      {
		p4=p3; p3=p2; p2=p1; p1=j;
		d4=d3; d3=d2; d2=d1; d1=d;
	      }
	    else if ( d1 < d &&  d < d2 )
	      {
		p4=p3; p3=p2; p2=j;
		d4=d3; d3=d2; d2=d;
	      }
	    else if ( d2 < d && d < d3 )
	      {
		p4=p3; p3=j;
		d4=d3; d3=d;
	      }
	    else if ( d3 < d && d < d4 )
	      {
		p4=j;
		d4=d;
	      }
	  }
      }
    }

  p[0]=p1;
  p[1]=p2;
  p[2]=p3;
  p[3]=p4;
  for (j=0; j<4; j++) if ( p[j] != -999 ) {zaehler++; }
  } }
  return (zaehler);
}


int candsearch_in_pixrest (next, num, x, y, dl, dr, du, dd, p)
target  next[];
int     num;
double  x, y, dl, dr, du, dd;
int p[4];
{
  /* search of four near candidates in targetlist */

  int  	  j, j0, dj;
  int  zaehler=0, p1, p2, p3, p4;
  double  d, dmin=1e20, xmin, xmax, ymin, ymax;
  xmin = x - dl;  xmax = x + dr;  ymin = y - du;  ymax = y + dd;

  if(xmin<0.0) xmin=0.0;
  if(xmax>imx) xmax=imx;
  if(ymin<0.0) ymin=0.0;
  if(ymax>imy) ymax=imy;

  if(x<0.0) x=0.0;
  if(x>imx) x=imx;
  if(y<0.0) y=0.0;
  if(y>imy) y=imy;

  p1 = p2 = p3 = p4 = -999;

  /* binarized search for start point of candidate search */
  for (j0=num/2, dj=num/4; dj>1; dj/=2)
    {
      if (next[j0].y < ymin) j0 += dj;
      else j0 -= dj;
    }

  j0 -= 12;  if (j0 < 0)  j0 = 0;	       	/* due to trunc */
  for (j=j0; j<num; j++)		       	/* candidate search */
    {
      if (next[j].tnr == -1 ) {
	if (next[j].y > ymax )  break;	       	/* finish search */
	if (next[j].x > xmin  &&  next[j].x < xmax && next[j].y > ymin  &&  next[j].y < ymax )
	  {
	    d = sqrt ((x-next[j].x)*(x-next[j].x) + (y-next[j].y)*(y-next[j].y));
	    if (d < dmin) { dmin = d; p1 = j; }
	  }
      }
    }

  p[0]=p1;
  p[1]=p2;
  p[2]=p3;
  p[3]=p4;
  for (j=0; j<4; j++) if ( p[j] != -999 ) {zaehler++; }
  return (zaehler);
}



void predict (x1, y1, x2, y2, x3, y3)
double x1, y1, x2, y2, *x3, *y3;
{
  *x3 = 2*x2 - x1;
  *y3 = 2*y2 - y1;
  return;
}


void readseqtrackcrit ()
{
	// ChrisB: When implementing dynamic tracking, we will want to read only
	// the sequence name and illuminated layer data from this routine.
	// The tracking parameters will be read from a seperate file containing
	// processed ADV data.
	
  int i_img;
  /* reads pixfiles and try to track particles in imagespace
     over the sequence */

  fpp = fopen_r ("parameters/sequence.par");
  for (i_img=0; i_img<4; i_img++) {
    fscanf (fpp, "%s\n", seq_name[i_img]);
  }
  /* name of sequence */
  fscanf (fpp,"%d\n", &seq_first);
  fscanf (fpp,"%d\n", &seq_last);
  fclose (fpp);

  // ChrisB: This section (at least some of it) will be read from a seperate
  // file containing processed ADV data unless we are using static tracking,
  // in which case it will be read once before the main tracking loop starts.
  if( staticTracking ) {
	  fpp = fopen_r ("parameters/track.par");
	  fscanf (fpp, "%lf\n", &tpar.dvxmin);
	  fscanf (fpp, "%lf\n", &tpar.dvxmax);
	  fscanf (fpp, "%lf\n", &tpar.dvymin);
	  fscanf (fpp, "%lf\n", &tpar.dvymax);
	  fscanf (fpp, "%lf\n", &tpar.dvzmin);
	  fscanf (fpp, "%lf\n", &tpar.dvzmax);
	  fscanf (fpp, "%lf\n", &tpar.dangle);
	  fscanf (fpp, "%lf\n", &tpar.dacc);
	  fscanf (fpp,"%d\n", &tpar.add);
	  /*
		 fscanf (fpp,"%d\n", &tpar.dsumg);
		 fscanf (fpp,"%d\n", &tpar.dn);
		 fscanf (fpp,"%d\n", &tpar.dnx);
		 fscanf (fpp,"%d\n", &tpar.dny);
	   */
	  fclose (fpp);
  }

  /* read illuminated layer data */
  fpp = fopen_r ("parameters/criteria.par");
  fscanf (fpp, "%lf\n", &X_lay[0]);
  fscanf (fpp, "%lf\n", &Zmin_lay[0]);
  fscanf (fpp, "%lf\n", &Zmax_lay[0]);
  fscanf (fpp, "%lf\n", &X_lay[1]);
  fscanf (fpp, "%lf\n", &Zmin_lay[1]);
  fscanf (fpp, "%lf\n", &Zmax_lay[1]);
  fclose (fpp);
}



void searchquader(X, Y, Z, xr, xl, yd, yu)
double X, Y, Z, xr[4], xl[4], yd[4], yu[4];
{
  int k, i;
  double x, y, xz, yz;
  coord_3d quader[8], point;

  /* project quader in image space to define search area */
  for (k=0; k<8; k++)
    {
      quader[k].pnr=k;
    }
  /* calculation of quader points */
  point.pnr=0; point.x=X; point.y=Y; point.z=Z;

  quader[0].x=X+tpar.dvxmin; quader[0].y=Y+tpar.dvymin; quader[0].z=Z+tpar.dvzmin; /* --- */
  quader[1].x=X+tpar.dvxmax; quader[1].y=Y+tpar.dvymin; quader[1].z=Z+tpar.dvzmin; /* +-- */
  quader[2].x=X+tpar.dvxmin; quader[2].y=Y+tpar.dvymax; quader[2].z=Z+tpar.dvzmin; /* -+- */
  quader[3].x=X+tpar.dvxmax; quader[3].y=Y+tpar.dvymin; quader[3].z=Z+tpar.dvzmax; /* --+ */
  quader[4].x=X+tpar.dvxmax; quader[4].y=Y+tpar.dvymax; quader[4].z=Z+tpar.dvzmin; /* ++- */
  quader[5].x=X+tpar.dvxmax; quader[5].y=Y+tpar.dvymin; quader[5].z=Z+tpar.dvzmax; /* +-+ */
  quader[6].x=X+tpar.dvxmin; quader[6].y=Y+tpar.dvymax; quader[6].z=Z+tpar.dvzmax; /* -++ */
  quader[7].x=X+tpar.dvxmax; quader[7].y=Y+tpar.dvymax; quader[7].z=Z+tpar.dvzmax; /* +++ */

  /* calculation of search area */
  for (i=0; i<n_img; i++)
    {
      xr[i]=0;
      xl[i]=imx;
      yd[i]=0;
      yu[i]=imy;
      img_coord (point.x, point.y, point.z, Ex[i], I[i], ap[i], mmp, &xz,&yz);
      metric_to_pixel (xz,yz, imx,imy, pix_x,pix_y, &xz,&yz, chfield);

      for (k=0; k<8; k++)
	{
	  img_coord (quader[k].x, quader[k].y, quader[k].z, Ex[i], I[i], ap[i], mmp, &x,&y);
	  metric_to_pixel (x,y, imx,imy, pix_x,pix_y, &x,&y, chfield);

	  if (x <xl[i] ) xl[i]=x;
	  if (y <yu[i] ) yu[i]=y;
	  if (x >xr[i] ) xr[i]=x;
	  if (y >yd[i] ) yd[i]=y;
	}
      if (xl[i] < 0 ) xl[i]=0;
      if (yu[i] < 0 ) yu[i]=0;
      if (xr[i] > imx) xr[i]=imx;
      if (yd[i] > imy) yd[i]=imy;

      xr[i]=xr[i]-xz;
      xl[i]=xz-xl[i];
      yd[i]=yd[i]-yz;
      yu[i]=yz-yu[i];
    }
  return;

}



void sortwhatfound (foundpix item[16], int *zaehler)
{
  int i,j,m, different;
  foundpix temp;

  different=0;

  /* where what was found */
  for (i=0; i<16; i++)
    for (j=0; j<4; j++)
      for (m=0; m<4; m++)
	if(item[i].ftnr == item[4*j+m].ftnr)
	  {
	    item[i].whichcam[j]=1;
	  }

  /* how often was ftnr found */
  for (i=0; i<16; i++)
    for (j=0; j<n_img; j++)
      if (item[i].whichcam[j] == 1 && item[i].ftnr !=-1) item[i].freq++;

  /* sort freq */
  for (i=1; i<16; ++i)  for (j=16-1; j>=i; --j)
    {
      if ( item[j-1].freq < item[j].freq )
	{
	  temp = *(item+j-1); *(item+j-1) = *(item+j); *(item+j) = temp;
	}
    }

  for (i=0; i<16; i++)
    for (j=i+1; j<16; j++)
      {
	if (item[i].ftnr == item[j].ftnr || item[j].freq <2)
	  {
	    item[j].freq=0;
	    item[j].ftnr=-1;
	  }
      }

  /* sort freq */
  for (i=1; i<16; ++i)  for (j=16-1; j>=i; --j)
    {
      if ( item[j-1].freq < item[j].freq )
	{
	  temp = *(item+j-1); *(item+j-1) = *(item+j); *(item+j) = temp;
	}
    }
  for (i=0; i<16; ++i) if(item[i].freq != 0) different++;
  *zaehler=different;

}

void angle_acc( X0, Y0, Z0,X1, Y1, Z1, X2, Y2, Z2, angle, acc )
double X0, Y0, Z0,X1, Y1, Z1, X2, Y2, Z2, *angle, *acc;
{
  double ds, da;
  coord_3d v0, v1, v2;

  /* calculation of angle and acceleration */
  v0.x=X1-X0; v0.y=Y1-Y0; v0.z=Z1-Z0;
  v1.x=X2-X0; v1.y=Y2-Y0; v1.z=Z2-Z0;
  v2.x=v1.x-v0.x; v2.y=v1.y-v0.y; v2.z=v1.z-v0.z;

  ds= sqrt( v2.x*v2.x+v2.y*v2.y+v2.z*v2.z);
  /* special case 200 gon */
  if (v1.x ==-v0.x && v1.y ==-v0.y && v1.z ==-v0.z)
    { *angle =200; } else {
  da=acos ((v0.x*v1.x+v0.y*v1.y+v0.z*v1.z)/
	   (sqrt( v1.x*v1.x+v1.y*v1.y+v1.z*v1.z)*
	    sqrt( v0.x*v0.x+v0.y*v0.y+v0.z*v0.z)));
  *angle=da*ro; }
  *acc=ds;

  return;
}