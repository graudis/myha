#include "random_machine.h"
#include <time.h>

unsigned int RandomInt::inc_seed_ = 0;
unsigned int RandomReal::inc_seed_ = 0;

RandomInt::RandomInt(int sno, int eno)
	: random_actor_(time(0) + inc_seed_), uniform_(sno, eno), dice_(random_actor_, uniform_)
{
	inc_seed_ += 999999;
}

RandomReal::RandomReal(float sno, float eno)
	: random_actor_(time(0) + inc_seed_), uniform_(sno, eno), dice_(random_actor_, uniform_)
{
	inc_seed_ += 999999;
}



template <typename Distribution, typename Result>
Result generate(boost::mt19937& random_actor, const Result min, const Result max)
{
	Distribution dist(min, max);
	boost::variate_generator<boost::mt19937&, Distribution> vg(random_actor, dist);
	return vg();
}



RnadomNumberGenerator::RnadomNumberGenerator() : random_actor_()
{
}

RnadomNumberGenerator::RnadomNumberGenerator(const tUINT seed) : random_actor_(seed)
{
}

RnadomNumberGenerator::~RnadomNumberGenerator()
{
}

tUINT RnadomNumberGenerator::randU()
{
	return generate<boost::uniform_int<tUINT>, tUINT>(random_actor_, 0, UINT_MAX);
}

tUINT RnadomNumberGenerator::randU(const tUINT n)
{
	return generate<boost::uniform_int<tUINT>, tUINT>(random_actor_, 0, n);
}

tUINT RnadomNumberGenerator::randU(const tUINT min, const tUINT max)
{
	return generate<boost::uniform_int<tUINT>, tUINT>(random_actor_, min, max);
}

tINT RnadomNumberGenerator::randI()
{
	return generate<boost::uniform_int<tINT>, tINT>(random_actor_, 0, INT_MAX);
}

tINT RnadomNumberGenerator::randI(const tINT n)
{
	return generate<boost::uniform_int<tINT>, tINT>(random_actor_, 0, n);
}

tINT RnadomNumberGenerator::randI(const tINT min, const tINT max)
{
	return generate<boost::uniform_int<tINT>, tINT>(random_actor_, min, max);
}

tFLOAT RnadomNumberGenerator::randF()
{
	return generate<boost::uniform_real<tFLOAT>, tFLOAT>(random_actor_, 0.0f, 1.0f);
}

tFLOAT RnadomNumberGenerator::randF(const tFLOAT n)
{
	return generate<boost::uniform_real<tFLOAT>, tFLOAT>(random_actor_, 0.0f, n);
}

tFLOAT RnadomNumberGenerator::randF(const tFLOAT min, const tFLOAT max)
{
	return generate<boost::uniform_real<tFLOAT>, tFLOAT>(random_actor_, min, max);
}

tDOUBLE RnadomNumberGenerator::randD()
{
	return generate<boost::uniform_real<tDOUBLE>, tDOUBLE>(random_actor_, 0.0f, 1.0f);
}

tDOUBLE RnadomNumberGenerator::randD(const tDOUBLE n)
{
	return generate<boost::uniform_real<tDOUBLE>, tDOUBLE>(random_actor_, 0.0f, n);
}

tDOUBLE RnadomNumberGenerator::randD(const tDOUBLE min, const tDOUBLE max)
{
	return generate<boost::uniform_real<tDOUBLE>, tDOUBLE>(random_actor_, min, max);
}

tFLOAT RnadomNumberGenerator::randNormF(const tFLOAT mean, const tFLOAT variance)
{
	boost::normal_distribution<tFLOAT> dist(mean, variance);
	boost::variate_generator<boost::mt19937&, boost::normal_distribution<tFLOAT> > vg(random_actor_, dist);
	return vg();
}

tDOUBLE RnadomNumberGenerator::randNormD(const tDOUBLE mean, const tDOUBLE variance)
{
	boost::normal_distribution<tDOUBLE> dist(mean, variance);
	boost::variate_generator<boost::mt19937&, boost::normal_distribution<tDOUBLE> > vg(random_actor_, dist);
	return vg();
}
