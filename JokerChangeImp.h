/*
 * JokerChangeImp.h
 *
 *  Created on: May 2, 2018
 *      Author: ellas2
 */

#ifndef JOKERCHANGEIMP_H_
#define JOKERCHANGEIMP_H_

#include "JokerChange.h"
#include "PointImp.h"

class JokerChangeImp : public JokerChange{
public:
	JokerChangeImp();
	JokerChangeImp(const PointImp& jokerChangePosition, char newRep);
	const Point& getJokerChangePosition() const override;
	char getJokerNewRep() const override; // R, P, S or B (but NOT J and NOT F)
	void setJokerNewRep(char newRep);
	JokerChangeImp& operator=(const JokerChangeImp& rhs);
	~JokerChangeImp() = default; 
private:
	PointImp m_jokerChangePosition;
	char m_newRep;
};

#endif /* JOKERCHANGEIMP_H_ */
