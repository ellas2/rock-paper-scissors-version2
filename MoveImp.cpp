/*
 * MoveImp.cpp
 *
 *  Created on: May 2, 2018
 *      Author: ellas2
 */


#include "MoveImp.h"

MoveImp::MoveImp(const Point& from, const Point& to) : m_from(from), m_to(to){}

const Point& MoveImp::getFrom() const
{
	return m_from;
}


const Point& MoveImp::getTo() const{
	return m_to;
}


