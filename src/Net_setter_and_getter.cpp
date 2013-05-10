#ifndef NET_SETTER_AND_GETTER_H_20130507
#define NET_SETTER_AND_GETTER_H_20130507
#include"Net.hpp"

void Net::setLearnNum( int n )
{
	param.num_learn = n;
}

void Net::setHiddenNodesNum( int n )
{
	param.num_hidden = n;
}
void Net::setSigmoidGain( double gain )
{
	param.s_gain = gain;
}
void Net::setLearningCoefficient( double eps )
{
	param.epsilon = eps;
}

void Net::setThresholdError( double err )
{
	param.threshold_error = err;
}

int Net::getLearnNum()
{
	return param.num_learn;
}

int Net::getInputNum()
{
	return param.num_input;
}

int Net::getHiddenNodesNum()
{
	return param.num_hidden;
}

double Net::getSigmoidGain()
{
	return param.s_gain;
}
double Net::getLearningCoefficient()
{
	return param.epsilon;
}

double Net::getThresholdError()
{
	return param.threshold_error;
}

bool Net::isTrained()
{
	return param.is_trained;
}

#endif
