#ifndef __INTEGRAL_H__
#define __INTEGRAL_H__

#include <vector>
#include "utils.h"
#include "simulator.h"

void freqGpuSimulate(SimulateConfig&, TransferFunction&, vector<pair<double,complex<double> > >&);

#endif /* __INTEGRAL_H__ */

