#ifndef _COLLISION_H_
#define _COLLISION_H_

#include <math.h>

double getDistance(double x, double y, double x1, double y1, 
                    double x2, double y2){
    double dx,dy,a,b,t,tx,ty;
    double distance;
    dx = (x2 - x1); dy = (y2 - y1);
    a = dx*dx + dy*dy;
    b = dx * (x1 - x) + dy * (y1 - y);
    t = -b / a;
    if (t < 0) t = 0;
    if (t > 1) t = 1;
    tx = x1 + dx * t;
    ty = y1 + dy * t;
    distance = sqrt((x - tx)*(x - tx) + (y - ty)*(y - ty));
    return distance;
}

bool checkPointCollision(float ox, float oy, float px, float py, double r){
    double dx = ox - px, dy = oy - py;
    if(dx*dx + dy*dy < r*r) return true;
    else                    return false;
}

bool checkCollision(
	float ox, float oy, float r,
	float ax, float ay,
	float vx, float vy
) {
	  // “_‚Æ“_
	  if (checkPointCollision(ox, oy, ax, ay, r) || checkPointCollision(ox, oy, vx, vy, r)) {
		  return true;
	  }
	  // ü•ª‚Æ“_
	  if (getDistance(ox, oy, ax, ay, vx, vy) < r) {
		  return true;
	  }

	  return false;
}

#endif