#include "mathcompat.h"

#include <cstdio>

#include "mathcompat.h"
#include "common.h"

#include "partchunk.h"



////////////////////////////////////////////////////////////////////////////////
//
// theta - x
// phi   - y
//

const Float PartitionChunk::kEpsilon = 0.01;
const Float PartitionChunk::kThetaResolution = M_PI / PartitionChunk::kThetaSize;
const Float PartitionChunk::kPhiResolution   = M_PI / PartitionChunk::kPhiSize;


PartitionChunk::PartitionChunk() :
    m_rects(),
    m_knots(),
    m_minAngle(0.),
    m_maxAngle(0.),
    m_iterations(0),
    m_iterationStep(0.),
    m_root(NULL),
    m_data(NULL),
    m_cellIntegrals(NULL),
    m_knotsMap(),
    m_cellSquare(0.),
    m_fullIntegral(0.)
{
}


PartitionChunk::~PartitionChunk()
{
    cleanUp();
}

void PartitionChunk::cleanUp()
{
    delete m_root; m_root = NULL;

    if (m_cellIntegrals) {

        free2dArray(m_cellIntegrals);
        m_cellIntegrals = NULL;
    }
}

void PartitionChunk::setData(Float** const data, const Float& cellSquare)
{
    m_data = data;
    m_cellSquare = cellSquare;
    m_fullIntegral = 0.;

    for (int i = 0; i < kThetaSize-1; ++i)
        for (int j = 0; j < kPhiSize-1; ++j) {

            m_cellIntegrals[j][i] = approxIntegral(GreedRect(i, j, i+1, j+1));
            m_fullIntegral += m_cellIntegrals[j][i];
        }
}

void PartitionChunk::refine()
{
    refineNode(m_root);
}

void PartitionChunk::refineNode(Node* node)
{
    if (node->isLeaf()) {

        Float nodeIntegral    = integral(node->rect);
        Float rectMaxError    = nodeIntegral*kEpsilon;

        if (node->rect.canSplitX() && node->rect.canSplitY()) {

            Float xSplitError = rectError(node->rect.leftHalf()) +
                                rectError(node->rect.rightHalf());

            Float ySplitError = rectError(node->rect.topHalf()) +
                                rectError(node->rect.bottomHalf());


            if ((xSplitError > rectMaxError) && (xSplitError >= ySplitError)) {

                node->splitX();
            }
            else if ((ySplitError > rectMaxError) && (ySplitError > xSplitError)) {

                node->splitY();
            }
        }
        else if (node->rect.canSplitX()) {

            Float xSplitError = rectError(node->rect.leftHalf()) +
                                rectError(node->rect.rightHalf());


            if (xSplitError > rectMaxError) {

                node->splitX();
            }
        }
        else if (node->rect.canSplitY()) {

            Float ySplitError = rectError(node->rect.topHalf()) +
                                rectError(node->rect.bottomHalf());


            if (ySplitError > rectMaxError) {

                node->splitY();
            }
        }
    }

    if (!node->isLeaf()) {

        refineNode(node->pChild1);
        refineNode(node->pChild2);
    }
}

Float PartitionChunk::integral(const GreedRect& rect)
{
    Float res = 0.;

    for (int i = rect.x1; i < rect.x2; ++i)
        for (int j = rect.y1; j < rect.y2; ++j)
            res += m_cellIntegrals[j][i];

    return res;
}

Float PartitionChunk::approxIntegral(const GreedRect& rect)
{
    return 0.25 * ( m_data[rect.y1][rect.x1] +
                    m_data[rect.y2][rect.x1] +
                    m_data[rect.y1][rect.x2] +
                    m_data[rect.y2][rect.x2]) *
    (rect.square * m_cellSquare);
}

Float PartitionChunk::rectError(const GreedRect& rect)
{
    Float res = 0.;
    Float a, b, c, d;

    a = m_data[rect.y1][rect.x1] / rect.square;
    b = m_data[rect.y1][rect.x2] / rect.square;
    c = m_data[rect.y2][rect.x1] / rect.square;
    d = m_data[rect.y2][rect.x2] / rect.square;

    for (int i = rect.x1; i < rect.x2; ++i)
        for (int j = rect.y1; j < rect.y2; ++j) {

            res +=  fabs(m_data[j][i]  -  (a*(rect.x2 - i)*(rect.y2 - j)
                                         + b*(i - rect.x1)*(rect.y2 - j)
                                         + c*(rect.x2 - i)*(j - rect.y1)
                                         + d*(i - rect.x1)*(j - rect.y1)));
        }

    return res*m_cellSquare;
}


void PartitionChunk::createRectsList()
{
//    Rect::s_knots = &m_knots;
    processTreeNode(m_root);
}

void PartitionChunk::processTreeNode(Node* node)
{
    if (node->isLeaf()) {

        int  keys[4];
        int  indeces[4];
        Knot knots[4];

        keys[0] = node->rect.x1 + node->rect.y1*kThetaSize; //tl
        keys[1] = node->rect.x2 + node->rect.y1*kThetaSize; //tr
        keys[2] = node->rect.x1 + node->rect.y2*kThetaSize; //bl
        keys[3] = node->rect.x2 + node->rect.y2*kThetaSize; //br

        Float x1 = node->rect.x1 * kThetaResolution;
        Float x2 = node->rect.x2 * kThetaResolution;
        Float y1 = node->rect.y1 * kPhiResolution;
        Float y2 = node->rect.y2 * kPhiResolution;

        knots[0] = Knot(x1, y1);
        knots[1] = Knot(x2, y1);
        knots[2] = Knot(x1, y2);
        knots[3] = Knot(x2, y2);

        for (int i = 0; i < 4; ++i) {

            if (m_knotsMap.find(keys[i]) == m_knotsMap.end()) {

                indeces[i] = m_knots.size();
                m_knots.push_back(knots[i]);
                m_knotsMap[keys[i]] = indeces[i];
            }
            else {

                indeces[i] = m_knotsMap[keys[i]];
            }
        }

        m_rects.push_back(Rect(indeces[0], indeces[1], indeces[2], indeces[3], &m_knots));
    }
    else {

        processTreeNode(node->pChild1);
        processTreeNode(node->pChild2);
    }
}


bool PartitionChunk::load(FILE *file)
{
    m_knots.clear();
    m_rects.clear();

    if (   !fscanf(file, "%le", &m_minAngle)
        || !fscanf(file, "%le", &m_maxAngle))
        return false;

    int knotsNumber;
    if (!fscanf(file, "%d", &knotsNumber))
        return false;

    m_knots.reserve(knotsNumber);

    for (int i = 0; i < knotsNumber; ++i) {

        Float x, y;
        if (!fscanf(file, "%le\t%le", &x, &y))
            return false;

        m_knots.push_back(Knot(x, y));
    }


//    Rect::s_knots = &m_knots;

    unsigned long int rectsNumber, i;

    if (!fscanf(file, "%lu", &rectsNumber))
        return false;

    m_rects.reserve(rectsNumber);

    for (i = 0; i < rectsNumber; ++i) {

        int tl, tr, bl, br;
        if (!fscanf(file, "%d\t%d\t%d\t%d", &tl, &tr, &bl, &br))
            return false;

        m_rects.push_back(Rect(tl, tr, bl, br, &m_knots));
    }

    fprintf(stderr, "chunk loaded: %e -- %e\t%lu\n", m_minAngle, m_maxAngle, (unsigned long int)m_rects.size());
    return true;
}

bool PartitionChunk::save(FILE *file)
{
    //angles
    fprintf(file, "%.17e\n", m_minAngle);
    fprintf(file, "%.17e\n", m_maxAngle);

    //knots
    {
        KnotsVector::iterator i;

        fprintf(file, "%lu\n", (long unsigned int)m_knots.size());

        for (i = m_knots.begin(); i != m_knots.end(); ++i) {

            fprintf(file, "%.17e\t%.17e\n", (*i).x, (*i).y);
        }
    }

    //rects
    {
        RectsVector::iterator i;

        fprintf(file, "%lu\n", (long unsigned int)m_rects.size());

        for (i = m_rects.begin(); i != m_rects.end(); ++i) {

            fprintf(file, "%d\t%d\t%d\t%d\n", (*i).tl, (*i).tr, (*i).bl, (*i).br);
        }
    }


    return true;
}
