#ifndef BP_H_20130504
#define BP_H_20130504

#include"mylib.h"
#include"struct.h"
#include"Net.h"

void set_data( Params& param, std::vector< TrainingData >& target, const std::string filename );

void execute( const Params param, Net& net );

#endif
