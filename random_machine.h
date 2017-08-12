#pragma once

#include "rntype.h"
#include <boost/noncopyable.hpp>
#include <boost/random.hpp>

class RandomInt : boost::noncopyable
{
public:
	RandomInt(int sno, int eno);
	~RandomInt() {}

	int operator()()
	{
		return dice_();
	}

private:
	boost::mt19937			random_actor_;
	boost::uniform_int<>	uniform_;
	boost::variate_generator<boost::mt19937, boost::uniform_int<> > dice_;

private:
	static unsigned int inc_seed_;
};

class RandomReal : boost::noncopyable
{
public:
	RandomReal(float sno, float eno);
	~RandomReal() {}

	float operator()()
	{
		return (float)dice_();
	}

private:
	boost::mt19937			random_actor_;
	boost::uniform_real<>	uniform_;
	boost::variate_generator<boost::mt19937, boost::uniform_real<> > dice_;

private:
	static unsigned int inc_seed_;
};


class RnadomNumberGenerator
{
public:
	RnadomNumberGenerator();
	RnadomNumberGenerator(const tUINT seed);
	~RnadomNumberGenerator();

	tUINT randU();														// [0,2^32-1]
	tUINT randU(const tUINT n);										// [0,n]
	tUINT randU(const tUINT min, const tUINT max);					// [min,max]

	tINT randI();														// [0,2^16-1]
	tINT randI(const tINT n);											// [0,n]
	tINT randI(const tINT min, const tINT max);						// [min,max]

	tFLOAT randF();														// [0,1]
	tFLOAT randF(const tFLOAT n);										// [0,n]
	tFLOAT randF(const tFLOAT min, const tFLOAT max);					// [min,max]
	tDOUBLE randD();													// [0,1]
	tDOUBLE randD(const tDOUBLE n);									// [0,n]
	tDOUBLE randD(const tDOUBLE min, const tDOUBLE max);				// [min,max]

	tFLOAT randNormF(const tFLOAT mean = 0.0, const tFLOAT variance = 0.0);
	tDOUBLE randNormD(const tDOUBLE mean = 0.0, const tDOUBLE variance = 0.0);

	void seed(const tUINT seed) { random_actor_.seed(seed); }

private:
	boost::mt19937			random_actor_;

};
