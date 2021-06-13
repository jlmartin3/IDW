# IDW

A common geostatstical technique used to interpolate values across a study area based on a number of explicit sample locations. I had trouble finding a package in the R repository that implemented this technique in a simple way which proved to be a problem when using IDW to interpolate air pollution concentrations at millions of locations. To make it computationally feasible I coded IDW and an accuracy metric as c++ functions and included them in a package.
