/*
 * PointImp.cpp
 *
 *  Created on: May 2, 2018
 *      Author: ellas2
 */

#include "PointImp.h"

PointImp::PointImp(int x, int y) : m_x(x), m_y(y){}

int PointImp::getX() const
{
	return m_x;
}
int PointImp::getY() const
{
	return m_y;
}

bool PointImp::operator==(const Point& rhs) const
{
	if ((m_x == rhs.getX()) && (m_y == rhs.getY()))
		return true;
	return false;
}


void PointImp::setX(int new_x)
{
	m_x = new_x;
}

void PointImp::setY(int new_y)
{
	m_y = new_y;
}

PointImp::PointImp(const PointImp& pointImp)
{
	m_x = pointImp.getX();
	m_y = pointImp.getY();
}

PointImp& PointImp::operator=(const Point& rhs)
{
	if (this == &rhs)
		return *this;
	m_x = rhs.getX();
	m_y = rhs.getY();
	return *this;
}

bool operator==(const Point& lhs, const Point& rhs)
{
	if ((lhs.getX() == rhs.getX()) && (rhs.getY() == rhs.getY()))
			return true;
	return false;
}


PointImp::PointImp(const Point& point) : m_x(point.getX()), m_y(point.getY()){}
