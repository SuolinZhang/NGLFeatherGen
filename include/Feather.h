#ifndef FEATHER_H_
#define FEATHER_H_

#include "ngl/Vec3.h"
#include <ngl/Text.h>
#include <vector>
#include <memory>
#include "Curve.h"
#include <algorithm>

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
    
    /// @brief Set the F0 factor (outline start position on rachis)
    /// @param f0 Factor between 0.0 and 1.0
    void setF0(const ngl::Real _f0) noexcept;
    
    /// @brief Set the Fn factor (barb end position on rachis)
    /// @param fn Factor between 0.0 and 1.0
    void setFn(const ngl::Real _fn) noexcept;
    
    // ===== Barb Generation Methods =====
    /// @brief Set number of barbs to generate
    /// @param numBarbs Number of barbs on each side
    void setNumBarbs(unsigned int numBarbs);
    
    /// @brief Set outline endpoint factor for barbs (0.4-1.0)
    /// @param factor 0.5=middle of outline, 1.0=tip of outline
    void setBarbsOutlineFactor(ngl::Real left_factor, ngl::Real right_factor);
    
    /// @brief Generate a single barb between two points
    /// @param p0 Starting point (on rachis)
    /// @param p3 End point (on outline)
    /// @param p1XFactor Slider value (0-1) to control p1 X position
    /// @param p1YFactor Slider value (0-1) to control p1 Y position
    /// @param p2XFactor Slider value (0-1) to control p2 X position
    /// @param p2YFactor Slider value (0-1) to control p2 Y position
    /// @param isLeftSide true for left barb, false for right barb (affects v1, v3 direction)
    /// @return Unique pointer to generated barb curve
    std::unique_ptr<BezierCurve> GenerateSingleBarb(const ngl::Vec3& p0,
                                                   const ngl::Vec3& p3,
                                                   ngl::Real p1XFactor, 
                                                   ngl::Real p1YFactor,
                                                   ngl::Real p2XFactor,
                                                   ngl::Real p2YFactor,
                                                   bool isLeftSide = true) const;
    
    /// @brief Set the Fb factor (barb extreme shape control)
    /// @param fb Factor controlling barb shape extremes
    void setFb(const ngl::Real _fb) noexcept;
    
    /// @brief Set the barb LOD (number of barbules)
    /// @param lod Level of detail for barb curves
    void setBarbLOD(unsigned int lod);
    
    /// @brief Generate template barbs (left and right) and store in m_leftBarb and m_rightBarb
    void generateTemplateBarbs() const;

    /// @brief set outline mapping point position
    /// @param start (0.0-1.0)
    /// @param end (0.0-1.0)
    void setOutlineMappingRange(ngl::Real start, ngl::Real end) noexcept;

    /// @brief Generate all barbs distributed along the feather length
    void generateAllBarbs() const;
    
    /// @brief Set barb control factors
    /// @param p1XFactor Slider value (0-1) to control p1 X position
    /// @param p1YFactor Slider value (0-1) to control p1 Y position
    /// @param p2XFactor Slider value (0-1) to control p2 X position
    /// @param p2YFactor Slider value (0-1) to control p2 Y position
    void setBarbControlFactors(ngl::Real p1XFactor, 
                              ngl::Real p1YFactor,
                              ngl::Real p2XFactor,
                              ngl::Real p2YFactor);
    
    /// @brief Generate the left and right outlines of the feather
    /// @param p1 First control point for outline shape
    /// @param p2 Second control point for outline shape  
    /// @param p3 End point for outline shape
    void GenerateOutlines(ngl::Vec3 &p1, ngl::Vec3 &p2, ngl::Vec3 &p3) ;
    void generateOutlines();
    /// @brief Set whether outlines should be symmetrical
    /// @param symmetric true for symmetrical outlines, false for separate control
    void setOutlineSymmetric(bool symmetric) { m_outlineSymmetric = symmetric; }
    
    /// @brief Set whether to show outlines in full feather view
    /// @param show true to show outlines, false to hide them
    void setShowOutlines(bool show) { m_showOutlines = show; }
    
    /// @brief Get current outline symmetry setting
    /// @return true if outlines are symmetrical
    bool isOutlineSymmetric() const { return m_outlineSymmetric; }
    
    /// @brief Set separate control points for left and right outlines
    /// @param leftP1 Left outline first control point
    /// @param leftP2 Left outline second control point
    /// @param rightP1 Right outline first control point
    /// @param rightP2 Right outline second control point
    /// @param endP3 Common end point for both outlines
    void setOutlineControlPoints(const ngl::Vec3 &leftP1, const ngl::Vec3 &leftP2, 
                                const ngl::Vec3 &rightP1, const ngl::Vec3 &rightP2, 
                                const ngl::Vec3 &endP3);
    
    /// @brief Set symmetrical outline control points
    /// @param p1 First control point (will be mirrored for right side)
    /// @param p2 Second control point (will be mirrored for right side)
    /// @param p3 End point (shared by both sides)
    void setSymmetricOutlineControlPoints(const ngl::Vec3 &p1, const ngl::Vec3 &p2, const ngl::Vec3 &p3);

    /// get coordinate of start point of outlines
    ngl::Vec3 getStartPointOnRachis(ngl::Real _t);
    
    /// @brief Draw functions
    void drawRachis() const;
    void drawOutlines() const;
    void drawBarb() const;
    void drawAllBarbs() const;
    void draw() const;
    void drawFullFeather(bool showOutlines = true) const;
    
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
    void update();

private:
    /// ====================Core Curve Components===================
    mutable std::unique_ptr<BezierCurve> m_rachis;
    mutable std::unique_ptr<BezierCurve> m_leftOutline;
    mutable std::unique_ptr<BezierCurve> m_rightOutline;
    mutable std::unique_ptr<BezierCurve> m_leftBarb;
    mutable std::unique_ptr<BezierCurve> m_rightBarb;
    
    /// ====================Full Feather Barb Collections===================
    mutable std::vector<std::unique_ptr<BezierCurve>> m_leftBarbs;
    mutable std::vector<std::unique_ptr<BezierCurve>> m_rightBarbs;
    /// ====================Feather Parameters===================
    /// @brief the LOD of rachies curve
    unsigned int m_sample=200;
    /// @brief the LOD of barb curves
    unsigned int m_numBarbules=20;
    /// @brief factor determining where barbs start on Rachies
    ngl::Real m_F0=0.25f;
    /// @brief factor determining where barbs end on Rachies
    ngl::Real m_Fn=0.99f;
    /// @brief whether outlines should be symmetrical
    bool m_outlineSymmetric=true;
    /// @brief whether to show outlines in full feather view
    bool m_showOutlines=true;
    
    /// ====================Barb Parameters===================
    /// @brief number of barbs on each side
    unsigned int m_numBarbs=100;
    /// @brief outline endpoint factor for barbs (0.4-1.0)
    ngl::Real m_leftBarbOutlineFactor=0.55f;
    ngl::Real m_rightBarbOutlineFactor=0.51f;
    /// @brief Fb factor controlling barb shape extremes
    ngl::Real m_Fb=0.50f;
    /// @brief Barb control factors (0-1 slider values)
    ngl::Real m_p1XFactor=0.3f;
    ngl::Real m_p1YFactor=1.0f;
    ngl::Real m_p2XFactor=0.1f;
    ngl::Real m_p2YFactor=0.0f;

    /// ====================Outline Mapping Parameters===================
    /// @brief outline mapping start point (0.0-1.0)
    ngl::Real m_outlineMappingStart = 0.0f;
    /// @brief outline mapping end point (0.0-1.0)
    ngl::Real m_outlineMappingEnd = 1.0f;

    /// ====================Rachis Control Points===================
    ngl::Vec3 m_rachisP0 = ngl::Vec3(0.0f, 0.0f, 0.0f);
    ngl::Vec3 m_rachisP1 = ngl::Vec3(0.3f, 2.0f, 0.0f);
    ngl::Vec3 m_rachisP2 = ngl::Vec3(0.5f, 4.0f, 0.0f);
    ngl::Vec3 m_rachisP3 = ngl::Vec3(0.2f, 9.5f, 0.0f);

    ngl::Vec3 l_barb_endP;
    ngl::Vec3 r_barb_endP;
    
    /// ====================Outline Control Points===================
    /// @brief Control points for left outline (or symmetric outline base)
    ngl::Vec3 m_outlineP0;
    ngl::Vec3 m_outlineP1 = ngl::Vec3(-1.8f, 3.5f, 0.0f);
    ngl::Vec3 m_outlineP2 = ngl::Vec3(-1.5f, 5.5f, 0.0f);
    ngl::Vec3 m_outlineP3 = ngl::Vec3(-0.2f, 9.5f, 0.0f);
    
    /// @brief Control points for right outline (used only when not symmetric)
    ngl::Vec3 m_rightOutlineP1 = ngl::Vec3(0.6f, 2.75f, 0.0f);
    ngl::Vec3 m_rightOutlineP2 = ngl::Vec3(0.5f, 10.0f, 0.0f);
    
    /// ====================Helper Methods===================
    /// @brief Ensure rachis curve exists
    void ensureRachisExists() const;
};

#endif