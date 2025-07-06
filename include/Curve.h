/*
    This file is modified based on NGL::BezierCurve.
    Different from NGL::BezierCurve, the algorithm uesd to draw a
    curve is deCasteljau.
*/
#ifndef CURVE_H_
#define CURVE_H_
/// @file Curve.h
/// @brief basic BezierCurve using deCasteljau algorithm
// must include types.h first for Real and GLEW if required
#include "ngl/Types.h"
#include "ngl/Vec3.h"
#include <vector>
#include "ngl/VAOFactory.h"
#include "ngl/SimpleVAO.h"



class BezierCurve
{
public :
	/// @brief default ctor sets initial values for Curve to be used with AddPoint etc
	BezierCurve() =default;

  	/// @brief ctor passing in an array of points
  	/// @param[in] _p the array of CP values expressed as groups of Vec3
	BezierCurve(const std::vector<ngl::Vec3>  &_p) noexcept;
	/// @brief don't allow copies
	BezierCurve(const BezierCurve &_c) =delete;
	/// @brief destructor
	~BezierCurve() noexcept;
	/// @brief Draw method to draw the curve 
		void draw() const noexcept;
	/// @brief draw the control points
	void drawControlPoints() const noexcept;
	/// @brief Draw the control hull
	void drawHull() const noexcept;
  	/// @brief get a point on the curve in the range of 0 - 1 based on the control points
  	/// @param[in] _value the point to evaluate between 0 and 1
  	/// @returns the value of the point at t
	ngl::Vec3 getPointOnCurve(ngl::Real _value) noexcept;
	/// @brief add a control point to the Curve
	/// @param[in] &_p the point to add
	void addPoint(const ngl::Vec3 &_p) noexcept;
	/// @brief add a point to the curve using x,y,z values
	/// @param[in] _x x value of point
	/// @param[in] _y y value of point
	/// @param[in] _z z value of point
	void addPoint(ngl::Real _x, ngl::Real _y, ngl::Real _z) noexcept;
	///@brief caculate the point interpolated by two points
	///@param[in] _t the value betwween 0 and to evaluate the point
	///@param[in] p0 the first point to interpolate
	///@param[in] p1 the second point to interpolate
	ngl::Vec3 lerp(ngl::Real _t, ngl::Vec3& p0, ngl::Vec3& p1) noexcept;
	///@brief the baisc function to caculate the point on curve
	///this is a recursion function but have not worked well
	///@param[in] _t the value betwween 0 and to evaluate the point
	///@param[in] _n the degree of the Beizier curve to draw
	///@param[in] p_list array container include all control points
	ngl::Vec3 deCasteljau(ngl::Real _t, std::vector<ngl::Vec3>& p_list) noexcept;
  	/// @brief set the Level of Detail for Drawing
  	/// @note this will have no Effect if the createVAO has
  	/// been called before
  	/// @param[in] _lod the level of detail to use when creating the VAO for drawing the higher the number
  	/// the finer the drawing
	void setLOD(unsigned int lod) noexcept;
  	/// @brief create and set all VAOs for a Bezier Curve
  	void createVAO() noexcept;
	///	@brief get all samples of drawing the curve and reset m_samplePts variable
	std::vector<ngl::Vec3> getSamplePoints() noexcept;


protected :
  /// @brief The amount of Control Points in the Curve
  unsigned int m_numCP=0;
  /// @brief The degree of the curve, Calculated from the Number of Control Points
  unsigned int m_degree=0;
  /// @brief  the control points for the Bezier curve
  std::vector <ngl::Vec3> m_cp;
  /// @brief The level of detail used to calculate how much detail to draw
  unsigned int m_lod=30;
  /// @brief store all samples of drawing the curve
  std::vector<ngl::Vec3> m_samplePts;
  /// @brief control when recalculate m_samplePts	
  bool m_samplePtsDirty = true;
	/// @brief a vertex array object for our curve drawing
	std::unique_ptr<ngl::AbstractVAO> m_vaoCurve;
	/// @brief a vertex array object for our point drawing
	std::unique_ptr<ngl::AbstractVAO> m_vaoPoints;
}; // end class BezierCurve
#endif // end header file


