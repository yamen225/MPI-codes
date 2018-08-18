#include <stdio.h>

float f (float x){
  float return_value;
  return_value = x*x;
  return return_value;
}

int main(int argc, char const *argv[]) {
  /* code */
  float integral; /*store result of the integral */
  float a,b; /* left and ight end points */
  int n; /* Number of trapazoids */
  float h; /* trapazoids bas width */
  float x;
  int i;

  float f(float x); /* Function we are integrating */
  printf("Enter a, b, and n\n");
  scanf("%f %f %d\n", &a, &b, &n );

  h = (b-a)/n;
  integral = (f(a) + f(b))/2.0;
  x = a;
  for (i = 1; i <= n-1; i++) {
    /* code */
    x = x + h;
    integral += f(x);
  }
  integral *= h;
  printf("with n = %d trapazoids, our estimate \n",n );
  printf("of the integral from %f to %f = %f\n", a, b, integral );
  return 0;
}
