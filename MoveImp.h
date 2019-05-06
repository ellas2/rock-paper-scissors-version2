/*
 * MoveImp.h
 *
 *  Created on: Apr 29, 2018
 *      Author: ellas2
 */

#ifndef MOVEIMP_H_
#define MOVEIMP_H_

#include "Move.h"
#include "PointImp.h"

class MoveImp: public Move {
public:
	MoveImp(const Point& from, const Point& to);
	const Point& getFrom() const override;
	const Point& getTo() const override;
	~MoveImp() = default;
private:
	const PointImp m_from;
	const PointImp m_to;
};

#endif /* MOVEIMP_H_ */
