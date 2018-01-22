#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//51°14'09.3"N 6°43'51.7"E
// 51.235909, 6.731039 


int main() {
      long double lat1;
      long double lon1;
      long double lat2;
      long double lon2;
      long double Th;
      long double d;
      long double R;

      const long double PI = 3.1415926535;

      lon1 =   (6.731039*PI/180)  ;
      lat1 =  (51.235909*PI/180) ;

      Th = PI * 50 / 180;      //Must convert Bearing in degrees to radians
      d = 250;                        //Distance in feet, so...
      R = 3959 * 5280;      //Mean Radius of the earth in feet (note, earths radius varies from 3947 to 3968 miles)

      lat2 = asin( sin(lat1) * cos(d/R) + cos(lat1) * sin(d/R) * cos(Th) );
      lon2 = lon1 + atan2( sin(Th) * sin(d/R) * cos(lat1), cos(d/R) - sin(lat1) * sin(lat2) );
	
	lat2 = (lat2*180/PI);
	lon2 = (lon2*180/PI);	

	printf("new coordinates : \n Lat,Lon : %Lf,%Lf \n",lat2,lon2);
return 0;
}
	

