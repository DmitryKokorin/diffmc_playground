#ifndef _PHOTON_H_
#define _PHOTON_H_

#ifdef WIN32
#include <random>
#else
#include <tr1/random>
#endif

#include <limits.h>

#include "vector3.h"
#include "matrix3.h"


class LinearInterpolation;
class Partition;
class PartitionChunk;
class Angle;


typedef std::vector<Float> ValuesVector;

typedef std::tr1::mt19937 RngEngine;


class Photon

{
public:

    Photon(RngEngine& rng_engine, const Vector3& s = Vector3(0., 0., 1.), const int channel_ = Optics::ECHANNEL);

    static void init(   LinearInterpolation*    oLength,
                        LinearInterpolation*    eLength,
                        Partition*              oePartition,
                        Partition*              eoPartition,
                        Partition*              eePartition,
                        LinearInterpolation*    eChannelProb);

    void move();
    void scatter();

    Vector3   pos;  //position
    Vector3   s_i;  //normalized wave vector

    int   scatterings;
    Float fullIntegral;
    int   channel;
    Float time;

private:

    void createTransformToPartitionCoords(Matrix3& mtx, Vector3& nn, Angle& a_i);

    template <class T>
    void calcPartitionValues(const Vector3& nn);

    void choosePointInRect(Float& x, Float& y, const int rectIdx, const Float randX, const Float randY);


    RngEngine& rng_engine;

    inline Float random() { return (Float)Photon::rng_engine() / UINT_MAX; }

    static LinearInterpolation* s_oLength;
    static LinearInterpolation* s_eLength;

    static Partition*           s_oePartition;
    static Partition*           s_eoPartition;
    static Partition*           s_eePartition;

    static LinearInterpolation* s_eChannelProb;

    //these are to simulate static behavior for a reference (without ugly pointer syntax)
    LinearInterpolation&    oLength;
    LinearInterpolation&    eLength;

    Partition&              oePartition;
    Partition&              eoPartition;
    Partition&              eePartition;

    LinearInterpolation&    eChannelProb;

    PartitionChunk *m_chunk; //current chunk

    ValuesVector    m_knotValues;
    ValuesVector    m_rectValues;

    //disable copying
    Photon(const Photon&);
    Photon& operator=(const Photon&);
};


#endif /* _PHOTON_H_ */
