#ifndef BackPropagation_SETTER_AND_GETTER_H_20130507
#define BackPropagation_SETTER_AND_GETTER_H_20130507
#include"BackPropagation.hpp"

void BackPropagation::setLearnNum( int n )
{
	param.num_learn = n;
}

void BackPropagation::setHiddenNodesNum( int n )
{
	param.num_hidden = n;
}
void BackPropagation::setSigmoidGain( double gain )
{
	param.s_gain = gain;
}
void BackPropagation::setLearningCoefficient( double eps )
{
	param.learning_coefficient = eps;
}

void BackPropagation::setThresholdError( double err )
{
	param.threshold_error = err;
}

int BackPropagation::getLearnNum()
{
	return param.num_learn;
}

int BackPropagation::getInputNum()
{
	return param.num_input;
}

int BackPropagation::getHiddenNodesNum()
{
	return param.num_hidden;
}

double BackPropagation::getSigmoidGain()
{
	return param.s_gain;
}
double BackPropagation::getLearningCoefficient()
{
	return param.learning_coefficient;
}

double BackPropagation::getThresholdError()
{
	return param.threshold_error;
}

bool BackPropagation::isTrained()
{
	return param.is_trained;
}

#endif
