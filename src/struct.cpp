#include"struct.hpp"

Params::Params()
{
		num_input = num_hidden = num_output = 0;
		num_learn = 50000;
		num_hidden = 10;
		s_gain = 1.0;
		learning_coefficient = 0.07;
		interia_coefficient = 0.05;
		threshold_error = 0.00001;
		is_empty = true;
		is_trained = false;
}
