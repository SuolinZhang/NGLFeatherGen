/*
	This file is modified based on NGL::BezierCurve.
	Different from NGL::BezierCurve, the algorithm uesd to draw a
	Beizier curver is deCasteljau.
*/

/// @file Curve.cpp
/// @brief basic BezierCurve using deCasteljau algorithm

#include "Curve.h"
#include <iostream>


BezierCurve::BezierCurve( const std::vector<ngl::Vec3> &_p) noexcept : m_cp{_p}
{
	m_numCP=static_cast<unsigned int>(m_cp.size());
	m_degree=static_cast<unsigned int>(m_cp.size());
}

BezierCurve::~BezierCurve() noexcept
{
	m_cp.clear();
	if(m_vaoCurve!=nullptr && m_vaoPoints!=nullptr)
	{
		m_vaoCurve->unbind();
		m_vaoCurve->removeVAO();
		m_vaoPoints->unbind();
		m_vaoPoints->removeVAO();
	}
}

ngl::Vec3 BezierCurve::lerp(ngl::Real _t, ngl::Vec3& p0, ngl::Vec3& p1) noexcept
{
	ngl::Vec3 p;
	p.m_x=(1-_t)*p0.m_x +_t*p1.m_x;
	p.m_y=(1-_t)*p0.m_y +_t*p1.m_y;
	p.m_z=(1-_t)*p0.m_z +_t*p1.m_z;
	// std::cout << p.m_x << ' ' << p.m_y << ' ' << p.m_z <<'\n';
	return p;
}

	ngl::Vec3 BezierCurve::deCasteljau(ngl::Real _t, std::vector<ngl::Vec3>& p_list) noexcept
{
	size_t n = p_list.size() - 1;
	if (n == 0)
		return p_list[0];

	std::vector<ngl::Vec3> new_points(n);
	for (size_t i = 0; i < n; ++i) {
		new_points[i] = lerp(_t, p_list[i], p_list[i + 1]);
	}
	return deCasteljau(_t, new_points);
}

void BezierCurve::addPoint( const ngl::Vec3 &_p ) noexcept
{
	m_cp.push_back(_p);
	++m_numCP;
	++m_degree;
	m_samplePtsDirty = true;
	#ifdef DEBUG
    std::cout <<"Added "<<m_numCP<<" m_degree "<<m_degree<<" m_numKnots"<<m_numKnots<<" m_order "<<m_order<<'\n';
	#endif
}


void BezierCurve::addPoint( ngl::Real _x,  ngl::Real _y, ngl::Real _z) noexcept
{
	m_cp.push_back(ngl::Vec3(_x,_y,_z));
	++m_numCP;
	++m_degree;
	m_samplePtsDirty = true;
	#ifdef DEBUG
    std::cout <<"Added "<<m_numCP<<" m_degree "<<m_degree<<" m_numKnots"<<m_numKnots<<" m_order "<<m_order<<'\n';
	#endif
}

ngl::Vec3 BezierCurve::getPointOnCurve( const ngl::Real _value ) noexcept
{
	ngl::Vec3 p;
	p = deCasteljau(_value, m_cp);
	return p;
}

void BezierCurve::setLOD(unsigned int lod) noexcept
{
	if(m_lod != lod){
		m_lod = lod;
		m_samplePtsDirty = true;
	}
}

std::vector<ngl::Vec3> BezierCurve::getSamplePoints() noexcept
{
	if (m_samplePtsDirty) {
		m_samplePts.resize(m_lod);
		for(unsigned int i = 0; i < m_lod; ++i) {
			ngl::Real t = static_cast<float>(i) / m_lod;
			m_samplePts[i] = getPointOnCurve(t);
		}
		m_samplePtsDirty = false;
	}
	return m_samplePts;
}

void BezierCurve::createVAO() noexcept
{
  if(m_vaoCurve!=nullptr && m_vaoPoints!=nullptr)
  {
    m_vaoCurve->unbind();
    m_vaoCurve->removeVAO();
    m_vaoPoints->unbind();
    m_vaoPoints->removeVAO();
  }

  m_vaoPoints=ngl::VAOFactory::createVAO("simpleVAO",GL_POINTS);
  m_vaoPoints->bind();
  std::vector <ngl::Vec3> points = m_cp;
  m_vaoPoints->setData(ngl::SimpleVAO::VertexData(m_numCP*sizeof(ngl::Vec3),points[0].m_x));
  m_vaoPoints->setNumIndices(m_numCP);
  m_vaoPoints->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
  m_vaoPoints->unbind();


  m_vaoCurve=ngl::VAOFactory::createVAO("simpleVAO",GL_LINE_STRIP);
  m_vaoCurve->bind();

  std::vector <ngl::Vec3> lines = getSamplePoints();
  m_vaoCurve->setData(ngl::SimpleVAO::VertexData(m_lod*sizeof(ngl::Vec3),lines[0].m_x));
  m_vaoCurve->setNumIndices(m_lod);
  m_vaoCurve->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
  m_vaoCurve->unbind();

}

void BezierCurve::drawControlPoints()const noexcept
{
  m_vaoPoints->bind();
  m_vaoPoints->setMode(GL_POINTS);
  m_vaoPoints->draw();
  m_vaoPoints->unbind();
}


void BezierCurve::drawHull()const noexcept
{
  m_vaoPoints->bind();
  m_vaoPoints->setMode(GL_LINE_STRIP);
  m_vaoPoints->draw();
  m_vaoPoints->unbind();
 }


void BezierCurve::draw()  noexcept
{
	// createVAO();
m_vaoCurve->bind();
m_vaoCurve->draw();
m_vaoCurve->unbind();
}







