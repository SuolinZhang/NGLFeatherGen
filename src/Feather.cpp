#include "Feather.h"

#include <qconfig.h>

#include "Curve.h"

void Feather::setSampleNum(const int _num) noexcept
{
    m_sample = _num;
}

void Feather::setF0(const ngl::Real _f0) noexcept
{
    m_F0 = _f0;
}

void Feather::setFn(const ngl::Real _fn) noexcept
{
    m_Fn = _fn;
}

// ===== Barb Generation Methods =====
void Feather::setNumBarbs(unsigned int numBarbs)
{
    m_numBarbs = numBarbs;
}

void Feather::setBarbsOutlineFactor(ngl::Real left_factor, ngl::Real right_factor)
{
    m_leftBarbOutlineFactor = left_factor;
    m_rightBarbOutlineFactor = right_factor;
}

void Feather::setFb(const ngl::Real _fb) noexcept
{
    m_Fb = _fb;
}

void Feather::setBarbLOD(unsigned int lod)
{
    m_numBarbules = lod;
}

std::unique_ptr<BezierCurve> Feather::GenerateSingleBarb(const ngl::Vec3& p0,
                                                        const ngl::Vec3& p3,
                                                        ngl::Real p1XFactor,
                                                        ngl::Real p1YFactor,
                                                        ngl::Real p2XFactor,
                                                        ngl::Real p2YFactor,
                                                        bool isLeftSide) const
{
    // Calculate distance between p0 and p3
    ngl::Real d = (p3 - p0).length();

    // Calculate p1 position based on factors
    // For left side: p1.x = p0.x + v1 (where -Fb*d < v1 < 0)
    // For right side: p1.x = p0.x + v1 (where 0 < v1 < Fb*d) - inverted
    // p1.y = p0.y + v2 (where -Fb*d < v2 < Fb*d) - same for both sides
    ngl::Real v1, v3;
    if (isLeftSide) {
        v1 = -m_Fb * d * p1XFactor; // Maps 0-1 to (-Fb*d, 0) for left
        v3 = m_Fb * d * p2XFactor;  // Maps 0-1 to (0, Fb*d) for left
    } else {
        v1 = m_Fb * d * p1XFactor;  // Maps 0-1 to (0, Fb*d) for right - inverted
        v3 = -m_Fb * d * p2XFactor; // Maps 0-1 to (-Fb*d, 0) for right - inverted
    }
    
    ngl::Real v2 = (2.0f * p1YFactor - 1.0f) * m_Fb * d; // Maps 0-1 to (-Fb*d, Fb*d)
    ngl::Real v4 = (2.0f * p2YFactor - 1.0f) * m_Fb * d; // Maps 0-1 to (-Fb*d, Fb*d)

    ngl::Vec3 p1(p0.m_x + v1, p0.m_y + v2, p0.m_z);
    ngl::Vec3 p2(p3.m_x + v3, p3.m_y + v4, p0.m_z);

    // Create and return the barb curve
    auto barb = std::make_unique<BezierCurve>();
    barb->addPoint(p0);
    barb->addPoint(p1);
    barb->addPoint(p2);
    barb->addPoint(p3);
    barb->setLOD(m_numBarbules);

    return barb;
}

void Feather::setBarbControlFactors(ngl::Real p1XFactor, 
                                   ngl::Real p1YFactor,
                                   ngl::Real p2XFactor,
                                   ngl::Real p2YFactor)
{
    m_p1XFactor = p1XFactor;
    m_p1YFactor = p1YFactor;
    m_p2XFactor = p2XFactor;
    m_p2YFactor = p2YFactor;
}

void Feather::generateTemplateBarbs() const
{
    // Ensure rachis and outlines exist
    if (!m_rachis) {
        generateRachis();
    }
    if (!m_leftOutline || !m_rightOutline) {
        // Need to generate outlines first
        return;
    }
    
    // Calculate p0 (starting point on rachis at middle position of barb region)
    // Barb region starts at m_F0 and goes to 1.0
    // Middle position = m_F0 + (1.0 - m_F0) * 0.5
    ngl::Real middleFactor = m_F0 + (1.0f - m_F0) * 0.5f;
    ngl::Vec3 p0 = m_rachis->getPointOnCurve(middleFactor);
    
    // Calculate p3 for left barb (end point on left outline)
    ngl::Vec3 leftP3 = m_leftOutline->getPointOnCurve(m_leftBarbOutlineFactor);
    
    // Calculate p3 for right barb (end point on right outline)  
    ngl::Vec3 rightP3 = m_rightOutline->getPointOnCurve(m_rightBarbOutlineFactor);
    
    // Generate left template barb using member variables
    m_leftBarb = GenerateSingleBarb(p0, leftP3, m_p1XFactor, m_p1YFactor, m_p2XFactor, m_p2YFactor, true);
    
    // Generate right template barb using member variables
    m_rightBarb = GenerateSingleBarb(p0, rightP3, m_p1XFactor, m_p1YFactor, m_p2XFactor, m_p2YFactor, false);

}

void Feather::generateAllBarbs() const
{
    // Clear existing barb collections
    m_leftBarbs.clear();
    m_rightBarbs.clear();
    
    // Ensure rachis and outlines exist
    if (!m_rachis) {
        generateRachis();
    }
    if (!m_leftOutline || !m_rightOutline) {
        // Need to generate outlines first
        return;
    }
    
    // Calculate barb distribution along rachis
    // Barbs start at F0 position and distribute towards tip (but not all the way to 1.0)
    ngl::Real barbStart = m_F0;
    ngl::Real barbEnd = m_Fn; // Stop before the very tip
    ngl::Real barbRegionLength = std::clamp(barbEnd - barbStart, 0.0f, 1.0f);
    
    for (unsigned i = 0; i < m_numBarbs; ++i) {
        // Position along rachis
        const ngl::Real tRachis = barbStart +
            (static_cast<ngl::Real>(i) / static_cast<ngl::Real>(m_numBarbs - 1)) *
            (barbEnd - barbStart);

        // Map to outline position
        const ngl::Real tOutline = m_outlineMappingStart +
            ((tRachis - barbStart) / (barbRegionLength)) *
            (m_outlineMappingEnd - m_outlineMappingStart);

        // Get points
        const ngl::Vec3 p0 = m_rachis->getPointOnCurve(tRachis);
        const ngl::Vec3 leftP3 = m_leftOutline->getPointOnCurve(tOutline);
        const ngl::Vec3 rightP3 = m_rightOutline->getPointOnCurve(tOutline);

        // Generate left barb
        if (auto leftBarb = GenerateSingleBarb(p0, leftP3,
            m_p1XFactor, m_p1YFactor, m_p2XFactor, m_p2YFactor, true)) {
            leftBarb->setLOD(m_numBarbules);
            leftBarb->createVAO();
            m_leftBarbs.push_back(std::move(leftBarb));
            }

        // Generate right barb
        if (auto rightBarb = GenerateSingleBarb(p0, rightP3,
            m_p1XFactor, m_p1YFactor, m_p2XFactor, m_p2YFactor, false)) {
            rightBarb->setLOD(m_numBarbules);
            rightBarb->createVAO();
            m_rightBarbs.push_back(std::move(rightBarb));
            }
    }
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
}

std::vector<ngl::Vec3> Feather::getRachisControlPoints() const
{
    return {m_rachisP0, m_rachisP1, m_rachisP2, m_rachisP3};
}

ngl::Vec3 Feather::getStartPointOnRachis(ngl::Real _t)
{
    return m_rachis->getPointOnCurve(_t);
}


void Feather::setOutlineControlPoints(const ngl::Vec3 &leftP1, const ngl::Vec3 &leftP2,
                                     const ngl::Vec3 &rightP1, const ngl::Vec3 &rightP2,
                                     const ngl::Vec3 &endP3)
{
    m_outlineP1 = leftP1;
    m_outlineP2 = leftP2;
    m_rightOutlineP1 = rightP1;
    m_rightOutlineP2 = rightP2;
    m_outlineP3 = endP3;
    m_outlineSymmetric = false;
}

void Feather::setSymmetricOutlineControlPoints(const ngl::Vec3 &p1, const ngl::Vec3 &p2, const ngl::Vec3 &p3)
{
    m_outlineP1 = p1;
    m_outlineP2 = p2;
    m_outlineP3 = p3;
    m_outlineSymmetric = true;
}

void Feather::setOutlineMappingRange(ngl::Real start, ngl::Real end) noexcept {
    m_outlineMappingStart = std::clamp(start, 0.0f, 1.0f);
    m_outlineMappingEnd = std::clamp(end, 0.0f, 1.0f);
    // make sure end >= start
    if(m_outlineMappingEnd < m_outlineMappingStart) {
        m_outlineMappingEnd = m_outlineMappingStart;
    }
}

void Feather::GenerateOutlines( ngl::Vec3 &p1, ngl::Vec3 &p2, ngl::Vec3 &p3)
{
    if (!m_rachis) {
        generateRachis();
    }

    std::vector<ngl::Vec3> rachisPts = m_rachis->getSamplePoints();
    if (rachisPts.empty()) return;

    // Get starting point on rachis (at F0 position)
    int startIndex = static_cast<int>(m_F0 * (m_sample - 1));
    if (startIndex >= static_cast<int>(rachisPts.size())) startIndex = rachisPts.size() - 1;
    ngl::Vec3 p0 = rachisPts[startIndex];
    m_outlineP0 = p0;

    // Calculate LOD for outlines
    unsigned int outlineLOD = static_cast<unsigned int>(m_sample * (1.0f - m_F0));

    // Create left outline
    m_leftOutline = std::make_unique<BezierCurve>();
    m_rightOutline = std::make_unique<BezierCurve>();

    if (m_outlineSymmetric) {
        // Symmetrical outlines - mirror left side to right side
        ngl::Vec3 leftP1 = p1;
        ngl::Vec3 leftP2 = p2;
        ngl::Vec3 rightP1(-p1.m_x, p1.m_y, p1.m_z);  // Mirror X coordinate
        ngl::Vec3 rightP2(-p2.m_x, p2.m_y, p2.m_z);  // Mirror X coordinate

        // Add points to left outline
        m_leftOutline->addPoint(p0);
        m_leftOutline->addPoint(leftP1);
        m_leftOutline->addPoint(leftP2);
        m_leftOutline->addPoint(p3);

        // Add points to right outline
        m_rightOutline->addPoint(p0);
        m_rightOutline->addPoint(rightP1);
        m_rightOutline->addPoint(rightP2);
        m_rightOutline->addPoint(p3);
    } else {
        // Separate control points for left and right outlines
        // Add points to left outline
        m_leftOutline->addPoint(p0);
        m_leftOutline->addPoint(m_outlineP1);
        m_leftOutline->addPoint(m_outlineP2);
        m_leftOutline->addPoint(p3);

        // Add points to right outline
        m_rightOutline->addPoint(p0);
        m_rightOutline->addPoint(m_rightOutlineP1);
        m_rightOutline->addPoint(m_rightOutlineP2);
        m_rightOutline->addPoint(p3);
    }

    // Set LOD for both outlines
    m_leftOutline->setLOD(outlineLOD);
    m_rightOutline->setLOD(outlineLOD);
}

void Feather::generateOutlines()
{
       GenerateOutlines(m_outlineP1, m_outlineP2, m_outlineP3);
}

void Feather::drawRachis() const
{
    if (m_rachis) {
        m_rachis->draw();
    }
}

void Feather::drawOutlines() const
{
    if (m_leftOutline && m_rightOutline)
    {
        m_leftOutline->draw();
        m_rightOutline->draw();
    }
}

void Feather::drawBarb() const
{
    if (m_leftBarb && m_rightBarb)
    {
        m_leftBarb->drawControlPoints();
        m_leftBarb->draw();
        m_rightBarb->drawControlPoints();
        m_rightBarb->draw();
    }
}

void Feather::drawAllBarbs() const
{
    // Draw all left barbs
    for (const auto& barb : m_leftBarbs) {
        if (barb) {
            barb->draw();
        }
    }
    
    // Draw all right barbs
    for (const auto& barb : m_rightBarbs) {
        if (barb) {
            barb->draw();
        }
    }
}



void Feather::update()
{
    m_rachis.reset();
    generateRachis();
    m_rachis->createVAO();

    m_leftOutline.reset();
    m_rightOutline.reset();
    generateOutlines();
    m_leftOutline->createVAO();
    m_rightOutline->createVAO();

    m_leftBarb.reset();
    m_rightBarb.reset();
    generateTemplateBarbs();
    m_leftBarb->createVAO();
    m_rightBarb->createVAO();
    
    // Generate full feather barbs
    generateAllBarbs();
}


void Feather::draw() const
{
    drawRachis();
    if (m_showOutlines) {
        drawOutlines();
    }
    drawAllBarbs();
}





