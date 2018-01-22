#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

const long double deg_to_rad = 3.1415926535/180; // PI = 3.1415926535;
const long double rad_to_deg = 180/3.1415926535;
const long double d = 500; //Distance in feet
const long double R = 20903520; //Mean Radius of the earth in feet (note, earths radius varies from 3947 to 3968 miles)
const int angle_of_view = 50;

typedef struct { double x, y; } vec_t, *vec;
 

vec_t C_Bldg[] = {
		{51.236585,6.730659}, {51.236317,6.730826},
		{51.235933,6.731958}, {51.235975,6.732461}};


vec_t B_Bldg[] = {
		{51.236460,6.730068}, {51.236565,6.730461},
		{51.235143,6.731474}, {51.235040,6.731099}};


double dot(vec a, vec b)
{
	return a->x * b->x + a->y * b->y;
}
 
double cross(vec a, vec b)
{
	return a->x * b->y - a->y * b->x;
}
 
vec vsub(vec a, vec b, vec res)
{
	res->x = a->x - b->x;
	res->y = a->y - b->y;
	return res;
}
 
/* tells if vec c lies on the left side of directed edge a->b
 * 1 if left, -1 if right, 0 if colinear
 */
int left_of(vec a, vec b, vec c)
{
	vec_t tmp1, tmp2;
	double x;
	vsub(b, a, &tmp1);
	vsub(c, b, &tmp2);
	x = cross(&tmp1, &tmp2);
	return x < 0 ? -1 : x > 0;
}
 
int line_sect(vec x0, vec x1, vec y0, vec y1, vec res)
{
	vec_t dx, dy, d;
	vsub(x1, x0, &dx);
	vsub(y1, y0, &dy);
	vsub(x0, y0, &d);
	/* x0 + a dx = y0 + b dy ->
	   x0 X dx = y0 X dx + b dy X dx ->
	   b = (x0 - y0) X dx / (dy X dx) */
	double dyx = cross(&dy, &dx);
	if (!dyx) return 0;
	dyx = cross(&d, &dx) / dyx;
	if (dyx <= 0 || dyx >= 1) return 0;
 
	res->x = y0->x + dyx * dy.x;
	res->y = y0->y + dyx * dy.y;
	return 1;
}
 
/* === polygon stuff === */
typedef struct { int len, alloc; vec v; } poly_t, *poly;
 
poly poly_new()
{
	return (poly)calloc(1, sizeof(poly_t));
}
 
void poly_free(poly p)
{
	free(p->v);
	free(p);
}
 
void poly_append(poly p, vec v)
{
	if (p->len >= p->alloc) {
		p->alloc *= 2;
		if (!p->alloc) p->alloc = 4;
		p->v = (vec)realloc(p->v, sizeof(vec_t) * p->alloc);
	}
	p->v[p->len++] = *v;
}
 
/* this works only if all of the following are true:
 *   1. poly has no colinear edges;
 *   2. poly has no duplicate vertices;
 *   3. poly has at least three vertices;
 *   4. poly is convex (implying 3).
*/
int poly_winding(poly p)
{
	return left_of(p->v, p->v + 1, p->v + 2);
}
 
void poly_edge_clip(poly sub, vec x0, vec x1, int left, poly res)
{
	int i, side0, side1;
	vec_t tmp;
	vec v0 = sub->v + sub->len - 1, v1;
	res->len = 0;
 
	side0 = left_of(x0, x1, v0);
	if (side0 != -left) poly_append(res, v0);
 
	for (i = 0; i < sub->len; i++) {
		v1 = sub->v + i;
		side1 = left_of(x0, x1, v1);
		if (side0 + side1 == 0 && side0)
			/* last point and current straddle the edge */
			if (line_sect(x0, x1, v0, v1, &tmp))
				poly_append(res, &tmp);
		if (i == sub->len - 1) break;
		if (side1 != -left) poly_append(res, v1);
		v0 = v1;
		side0 = side1;
	}
}
 
poly poly_clip(poly sub, poly clip)
{
	int i;
	poly p1 = poly_new(), p2 = poly_new(), tmp;
 
	int dir = poly_winding(clip);
	poly_edge_clip(sub, clip->v + clip->len - 1, clip->v, dir, p2);
	for (i = 0; i < clip->len - 1; i++) {
		tmp = p2; p2 = p1; p1 = tmp;
		if(p1->len == 0) {
			p2->len = 0;
			break;
		}
		poly_edge_clip(p1, clip->v + i, clip->v + i + 1, dir, p2);
	}
 
	poly_free(p1);
	return p2;
}

void getNewGPS(long double lat1, long double lon1, long double* lat2, long double* lon2,int heading, int viewAngle) {

      long double Th;

	//convert to radians
      lon1 =   (lon1*deg_to_rad) ;
      lat1 =  (lat1*deg_to_rad) ;

      Th = (heading + viewAngle)* deg_to_rad;      //Must convert heading in degrees to radians


      *lat2 = asin( sin(lat1) * cos(d/R) + cos(lat1) * sin(d/R) * cos(Th) );
      *lon2 = lon1 + atan2( sin(Th) * sin(d/R) * cos(lat1), cos(d/R) - sin(lat1) * sin(*lat2) );
	
	*lat2 = ( (*lat2) *rad_to_deg);
	*lon2 = ( (*lon2) *rad_to_deg);	

	//printf("In function new coordinates : \n Lat,Lon : %Lf,%Lf \n",*lat2,*lon2);

}

bool isB_Bldg_in_view(long double lat1, long double lon1,long double lat2, long double lon2,long double lat3, long double lon3, int heading) {

	bool result = false;
 
	vec_t c[] = {{lat1, lon1}, {lat2, lon2},  {lat3, lon3}};

#define clen (sizeof(c)/sizeof(vec_t))
#define slen (sizeof(B_Bldg)/sizeof(vec_t))

	poly_t clipper = {clen, 0, c};
	poly_t subject = {slen, 0, B_Bldg};

	poly res = poly_clip(&subject, &clipper);
 
	if(res->len > 0)result = true;

//#if 0
	int i;
	for (i = 0; i < res->len; i++)
		printf("%g %g\n", res->v[i].x, res->v[i].y);
//#endif

return result;
}	

void filterClasses( long double lat1, long double lon1, int heading) {

      	long double lat2;
      	long double lon2;
      	long double lat3;
      	long double lon3;
	bool result = 0;

	getNewGPS(lat1, lon1, &lat2, &lon2,heading, (angle_of_view/2) );
	printf(" 2 new coordinates : \n Lat,Lon : %Lf,%Lf \n",lat2,lon2);

	getNewGPS(lat1, lon1, &lat3, &lon3,heading, (-1* (angle_of_view/2)));
	printf(" 3 new coordinates : \n Lat,Lon : %Lf,%Lf \n",lat3,lon3);

	result = isB_Bldg_in_view(lat1,lon1,lat2,lon2,lat3,lon3,heading);
	printf(" is B Building in View = %d \n",result);

	result = false;
}

int main(int argc, char **argv)
{
	bool result = true;
/*
    if (0 == strcmp(argv[0], "gps")){
        average(argc, argv);
    } else if (0 == strcmp(argv[1], "yolo")){
*/
	
      	long double lat1 = atof(argv[1]);//51.235909;
      	long double lon1 = atof(argv[2]);//6.731039;

	int heading = atoi(argv[3]);//(260);

	printf("received  coordinates : \n Lat,Lon : %Lf,%Lf : heading %d \n",lat1, lon1, heading);

	filterClasses( lat1, lon1, heading);

	return 0;
}	

