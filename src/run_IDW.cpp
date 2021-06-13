#include <Rcpp.h>
#include <cstdlib>
#include <vector>
#include <numeric>
#include <cmath> // for sqrt() 

using namespace Rcpp;

// [[Rcpp::export]]

NumericVector run_IDW( NumericVector interpolate_x,  // interpolation longs
                   NumericVector interpolate_y,  // interpolation lats
                   NumericVector observation_x,  // observation longs
                   NumericVector observation_y,  // observation lats
                   NumericVector observation_values,  // observation values
                          double power) {      // power for IDW
  
  int p = interpolate_x.size(); // number of interpolation locations
  int v = observation_values.size();  // number of observation locations
  
  NumericVector pred(p); // vector to store predicted values
  
  for(int i = 0; i < p; ++i) {  // for each inerpolation location
    
    // create variables to store calculations
    NumericVector dists    (v) ;  // distance from interpolation to each observation
    NumericVector val_dist (v) ;  // value divided by distance
    NumericVector dist_inv (v) ;  // one divided by distance
    
    for(int j = 0; j < v; ++j) { // for each monitor
      
      // calculate the distance from the interpolation location to the observation
      dists[j] = sqrt( pow( (interpolate_x[i] - observation_x[j]), 2.0 ) + 
                       pow( (interpolate_y[i] - observation_y[j]), 2.0 )   ) ;
      
      dists[j] = pow( dists[j], power ) ; // power of IDW
      
      val_dist[j] = observation_values[j]/dists[j] ; // part one calculations
      dist_inv[j]  = 1/dists[j] ;        // part two calulations 
    }
    
    // calculate IDW
    double sum_dv = std::accumulate(val_dist.begin(),  // sum part one
                                    val_dist.end(),  0.0) ; 
    double sum_di = std::accumulate(dist_inv.begin(),  // sum part two
                                    dist_inv.end(),   0.0)  ;
    
    pred[i] = sum_dv/sum_di ; // IDW result for the interpolation location
    
  }
  
  return pred ; 
  
}

/*** R
run_IDW(c(5,4), c(3,1), 
   observation_x = c(4,-1,2), observation_y = c(-2,0,1), 
   observation_values = c(50,60,70), 2.0) 
*/

