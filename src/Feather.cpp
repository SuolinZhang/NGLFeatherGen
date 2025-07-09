#include "Feather.h"

#include <qconfig.h>

#include "Curve.h"

void Feather::setSampleNum(const int _num) noexcept
{
    m_sample = _num;
}

void Feather::ensureRachisExists() const
{
    if (!m_rachis) {
        m_rachis = std::make_unique<BezierCurve>();
    }
}

void Feather::GenerateRachis(const ngl::Vec3& p0, const ngl::Vec3& p1, const ngl::Vec3& p2, const ngl::Vec3& p3) const
{
    ensureRachisExists();
    m_rachis->addPoint(p0);
    m_rachis->addPoint(p1);
    m_rachis->addPoint(p2);
    m_rachis->addPoint(p3);
    m_rachis->setLOD(m_sample);
}

void Feather::generateRachis() const
{
    GenerateRachis(m_rachisP0, m_rachisP1, m_rachisP2, m_rachisP3);
}

// Method to update rachis control points from UI
void Feather::setRachisControlPoints(const ngl::Vec3& p0, const ngl::Vec3& p1, 
                                    const ngl::Vec3& p2, const ngl::Vec3& p3)
{
    m_rachisP0 = p0;
    m_rachisP1 = p1;
    m_rachisP2 = p2;
    m_rachisP3 = p3;

    // Clear existing rachis and regenerate
    m_rachis.reset();
    generateRachis();
}

std::vector<ngl::Vec3> Feather::getRachisControlPoints() const
{
    return {m_rachisP0, m_rachisP1, m_rachisP2, m_rachisP3};
}

void Feather::generateCurves() const
{
    generateRachis();
}

void Feather::drawRachis() const
{
    ensureRachisExists();
    if (m_rachis) {
        m_rachis->draw();
        m_rachis->drawControlPoints();
        m_rachis->drawHull();
    }
}

void Feather::draw() const
{
    drawRachis();
}






/*
void Feather::GenerateOutlines(ngl::Vec3& p1, ngl::Vec3& p2, ngl::Vec3& p3) const
{
    std::vector<ngl::Vec3> rachisPts = rachis->getSamplePoints();
    ngl::Vec3 p0 = rachisPts[static_cast<int>(F0 * sample-1.0)];

    ngl::Vec3 l_p1=p1;
    ngl::Vec3 l_p2=p2;
    ngl::Vec3 r_p1(-p1.m_x, p1.m_y, p1.m_z);
    ngl::Vec3 r_p2(-p2.m_x, p2.m_y, p2.m_z);

    leftOutline->addPoint(p0);
    leftOutline->addPoint(l_p1);
    leftOutline->addPoint(l_p2);
    leftOutline->addPoint(p3);

    rightOutline->addPoint(p0);
    rightOutline->addPoint(r_p1);
    rightOutline->addPoint(r_p2);
    rightOutline->addPoint(p3);

    unsigned int lod = static_cast<int>(sample * (1-F0));
    leftOutline->setLOD(lod);
    rightOutline->setLOD(lod);

}
*/
/*
std::vector<std::unique_ptr<BezierCurve>> Feather::GenerateBarbs(ngl::Real k, ngl::Real q, ngl::Real d) const
{
    std::vector<ngl::Vec3> rachisPts=getSamplePoints(rachis);
    std::vector<ngl::Vec3> outlinePts;
    unsigned int lod = static_cast<int>(sample * (1-F0));

    std::vector<std::unique_ptr<BezierCurve>> vec(lod);

    for (unsigned int i=sample-lod-1; i<sample; i++)
    {
        std::vector<ngl::Vec3> p;
        ngl::Vec3 cp1=rachisPts[i];
        ngl::Vec3 cp3;
        ngl::Real x1=cp1.m_x - k;
        vec.push_back(std::make_unique<BezierCurve>());
    }

}
*/
