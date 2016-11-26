/*
 * Random.h
 *
 *  Created on: 11.11.2016
 *      Author: tobias
 */

#ifndef HELPER_RANDOMIZER_RANDOM_H_
#define HELPER_RANDOMIZER_RANDOM_H_

#include <random>
#include <utility>

namespace helper {
namespace randomizer {

static inline int random(const int& beginn, const int& end) {
	if (beginn >= end)
		return 0;

	std::random_device generator;
	std::uniform_int_distribution<int> distribution(beginn, end);
	int dice_roll = distribution(generator);
	return dice_roll;
}

}
}



#endif /* HELPER_RANDOMIZER_RANDOM_H_ */
