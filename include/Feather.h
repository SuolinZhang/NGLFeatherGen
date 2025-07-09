#ifndef FEATHER_H_
#define FEATHER_H_

#include "ngl/Vec3.h"
#include <ngl/Text.h>
#include <vector>
#include <memory>
#include "Curve.h"

/**
 * @brief Feather class for generating procedural feather geometry
 * 
 * This class manages the generation of feather components including:
 * - Rachis (main shaft)
 * - Left and right outlines
 * - Individual barbs
 */
class Feather
{
public:
    // ===== Constructors and Destructors =====
    Feather() = default;
    ~Feather() = default;

    // ===== Core Generation Methods =====
    /*
    /// @brief Generate all feather curves (rachis, outlines, barbs)
    void generateCurves() const;
    */

    /// @brief Generate the main rachis curve
    /// @param p0 Start point
    /// @param p1 First control point
    /// @param p2 Second control point  
    /// @param p3 End point
    void GenerateRachis(const ngl::Vec3& p0, const ngl::Vec3& p1, 
                       const ngl::Vec3& p2, const ngl::Vec3& p3) const;
    
    /// @brief Set the sample number (LOD) for rachis curve
    /// @param num Number of samples
    void setSampleNum(const int _num) noexcept;
    
    void GenerateOutlines(ngl::Vec3 &p1, ngl::Vec3 &p2, ngl::Vec3 &p3) const;
    
    ngl::Vec3 getStartPoint(ngl::Real _t);
    
    /// @brief Draw the complete feather (rachis, outlines, all barbs)
    void drawRachis() const;
    void draw() const;
    
    // ===== UI Control Methods =====
    /// @brief Set rachis control points for UI control
    /// @param p0 Start point
    /// @param p1 First control point
    /// @param p2 Second control point
    /// @param p3 End point
    void setRachisControlPoints(const ngl::Vec3& p0, const ngl::Vec3& p1, 
                               const ngl::Vec3& p2, const ngl::Vec3& p3);
    
    /// @brief Get current rachis control points
    /// @return Vector of 4 control points [p0, p1, p2, p3]
    std::vector<ngl::Vec3> getRachisControlPoints() const;

    // ===== Internal Methods =====
    /// @brief Generate rachis using current control points
    void generateRachis() const;
    
    void generateCurves() const;
    /*
        /// @brief generate all barb pointers and store in a vector
        /// @param k factor determine barb's second or third cp's x value
        /// @param q factor determine barb's second or third cp's y value
        /// @param d distance from the first cp to the end cp of a barb
        std::vector<std::unique_ptr<BezierCurve>> GenerateBarbs(ngl::Real k, ngl::Real q, ngl::Real d) const;
    */
    /// ====================Core Curve Components===================
    mutable std::unique_ptr<BezierCurve> m_rachis;
    mutable std::unique_ptr<BezierCurve> m_leftOutline;
    mutable std::unique_ptr<BezierCurve> m_rightOutline;
    mutable std::unique_ptr<BezierCurve> m_leftBarb;
    mutable std::unique_ptr<BezierCurve> m_rightBarb;

private:
    
    /// ====================Feather Parameters===================
    /// @brief the LOD of rachies curve, number of barbs
    unsigned int m_sample=200;
    /// @brief the LOD of barb curves, number of babules
    unsigned int m_numBarbules=100;
    /// @brief factor determining where barbs start on Rachies
    ngl::Real m_F0=0.25f;

    /// ====================Rachis Control Points===================
    ngl::Vec3 m_rachisP0 = ngl::Vec3(0.0f, 0.0f, 0.0f);
    ngl::Vec3 m_rachisP1 = ngl::Vec3(0.3f, 2.0f, 0.0f);
    ngl::Vec3 m_rachisP2 = ngl::Vec3(0.5f, 4.0f, 0.0f);
    ngl::Vec3 m_rachisP3 = ngl::Vec3(0.2f, 6.0f, 0.0f);

    ngl::Vec3 l_barb_endP;
    ngl::Vec3 r_barb_endP;
    
    /// ====================Helper Methods===================
    /// @brief Ensure rachis curve exists
    void ensureRachisExists() const;
};

#endif