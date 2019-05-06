/*
 * PointImp.h
 *
 *  Created on: May 2, 2018
 *      Author: ellas2
 */

#ifndef POINTIMP_H_
#define POINTIMP_H_

#include "Point.h"

class PointImp : public Point{
public:
	PointImp(const Point& point);
	PointImp(int x, int y);
	int getX() const override;
	int getY() const override;
	bool operator==(const Point& rhs)const;
	PointImp& operator=(const Point& rhs);
	void setX(int new_x);
	void setY(int new_y);
	PointImp(const PointImp& pointImp);
	~PointImp() = default;
	friend bool operator==(const Point& lhs, const Point& rhs);

private:
	int m_x;
	int m_y;
};

#endif /* POINTIMP_H_ */
