/*
 * JokerChangeImp.cpp
 *
 *  Created on: May 2, 2018
 *      Author: ellas2
 */

#include "JokerChangeImp.h"

JokerChangeImp::JokerChangeImp(const PointImp& jokerChangePosition, char newRep) :
m_jokerChangePosition(jokerChangePosition), m_newRep(newRep){}
JokerChangeImp::JokerChangeImp() :
m_jokerChangePosition(PointImp(-1,-1)), m_newRep('E'){}

const Point& JokerChangeImp::getJokerChangePosition() const
{
	return m_jokerChangePosition;
}
char JokerChangeImp::getJokerNewRep() const // R, P, S or B (but NOT J and NOT F)
{
	return m_newRep;
}

void JokerChangeImp::setJokerNewRep(char newRep)
{
	m_newRep = newRep;
}

JokerChangeImp& JokerChangeImp::operator=(const JokerChangeImp& rhs)
{
	if (&rhs == this)
		return *this;
	m_newRep = rhs.getJokerNewRep();
	m_jokerChangePosition = rhs.getJokerChangePosition();
	return *this;
}
