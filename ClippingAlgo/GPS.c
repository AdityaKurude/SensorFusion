#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const long double PI = 3.1415926535;
const long double d = 500; //Distance in feet
const long double R = 20903520; //Mean Radius of the earth in feet (note, earths radius varies from 3947 to 3968 miles)

void getNewGPS(long double lat1, long double lon1, long double* lat2, long double* lon2,int heading, int viewAngle) {

      long double Th;

	//convert to radians
      lon1 =   (lon1*PI/180) ;
      lat1 =  (lat1*PI/180) ;

      Th = PI * (heading + viewAngle) / 180;      //Must convert heading in degrees to radians


      *lat2 = asin( sin(lat1) * cos(d/R) + cos(lat1) * sin(d/R) * cos(Th) );
      *lon2 = lon1 + atan2( sin(Th) * sin(d/R) * cos(lat1), cos(d/R) - sin(lat1) * sin(*lat2) );
	
	*lat2 = ( (*lat2) *180/PI);
	*lon2 = ( (*lon2) *180/PI);	

	//printf("In function new coordinates : \n Lat,Lon : %Lf,%Lf \n",*lat2,*lon2);

}

int main() {
      long double lat2;
      long double lon2;


	int heading = 160;
	getNewGPS(51.235909, 6.731039, &lat2, &lon2,heading, +50);

	printf(" 2 new coordinates : \n Lat,Lon : %Lf,%Lf \n",lat2,lon2);




	getNewGPS(51.235909, 6.731039, &lat2, &lon2,heading, -50);

	printf("new coordinates : \n Lat,Lon : %Lf,%Lf \n",lat2,lon2);

return 0;
}
	

