#include <Rcpp.h>
#include <cstdlib>
#include <vector>
#include <numeric>
#include <cmath> // for sqrt() 

using namespace Rcpp;

// [[Rcpp::export]]

double RMSE( NumericVector observation_x,  // observation longs
             NumericVector observation_y,  // observation lats
             NumericVector observation_values,// observation values
                   double power) { // power for IDW
  
  int v = observation_values.size(); // number monitors for the day
  
  NumericVector preds (v); // new vector to store predictions
  NumericVector square_errorz (v); // vector for (pred-obs)^2
  
  for(int i = 0; i < v; ++i){ // for each observation i
    
    double observation_xi = observation_x[i]; // extract coords and value
    double observation_yi = observation_y[i];
    double observation_valuesi = observation_values[i]; // actual value at observation i 
    
    NumericVector observation_xj = observation_x; // make copy of input vectors
    NumericVector observation_yj = observation_y;
    NumericVector observation_valuesj = observation_values;
    
    observation_xj.erase(i); // remove i values from input vectors 
    observation_yj.erase(i); //    to make vectors for j observation
    observation_valuesj.erase(i);
    
    // create variables to store calculations
    NumericVector dists    ((v-1)) ;  // distance from i to j 
    NumericVector val_dist ((v-1)) ;  // value divided by distance
    NumericVector dist_inv ((v-1)) ;  // one divided by distance
    
    for(int j = 0; j < (v-1); ++j) { // for each observation j
      
      // calculate the distance from observation i to observation j
      dists[j] = sqrt( pow( (observation_xi - observation_xj[j]), 2.0 ) + 
                       pow( (observation_yi - observation_yj[j]), 2.0 )   ) ;
      
      dists[j] = pow( dists[j], power ) ; // power of IDW is 2
      
      val_dist[j] = observation_values[j]/dists[j] ; // part one calculations
      dist_inv[j]  = 1/dists[j] ;      // part two calulations 
    }
      
    
    // calculate IDW
    double sum_dv = std::accumulate(val_dist.begin(),  // sum part one
                                    val_dist.end(),  0.0) ; 
    double sum_di = std::accumulate(dist_inv.begin(),  // sum part two
                                    dist_inv.end(),   0.0)  ;
    
    preds[i] =  sum_dv/sum_di ; // IDW result for i
    
    preds[i] = exp(preds[i]);  // un log
    observation_valuesi    = exp(observation_valuesi);     
   
    // (observed - predicted)^2
    square_errorz[i] = pow((observation_valuesi - preds[i]), 2.0) ;
  }
  
  // root mean square error for the day
  double rmse = sqrt( mean(square_errorz) );
  
  return rmse ;
}

/*** R
RMSE( observation_x = c(4,-1,2), observation_y = c(-2,0,1), 
            observation_values = c(50,60,70),
            power = 2.0) 
*/

