#ifndef NET_SETTER_AND_GETTER_H_20130507
#define NET_SETTER_AND_GETTER_H_20130507
#include"Net.hpp"

void Net::set_learn_num( int n )
{
	param.num_learn = n;
}

void Net::set_hidden_nodes_num( int n )
{
	param.num_hidden = n;
}
void Net::set_s_gain( double gain )
{
	param.s_gain = gain;
}
void Net::set_epsilon( double eps )
{
	param.epsilon = eps;
}

void Net::set_threshold_error( double err )
{
	param.threshold_error = err;
}

int Net::get_learn_num()
{
	return param.num_learn;
}

int Net::get_input_num()
{
	return param.num_input;
}

int Net::get_hidden_nodes_num()
{
	return param.num_hidden;
}

double Net::get_s_gain()
{
	return param.s_gain;
}
double Net::get_epsilon()
{
	return param.epsilon;
}

double Net::get_threshold_error()
{
	return param.threshold_error;
}

bool Net::is_trained()
{
	return param.is_trained;
}

#endif
