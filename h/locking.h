/*
 * locking.h
 *
 *  Created on: Jul 8, 2018
 *      Author: OS1
 */

#ifndef _locking_h_
#define _locking_h_

extern volatile unsigned lockFlag;

#define lockCout lockFlag = 0;
#define unlockCout lockFlag = 1;

#endif /* HEDERI_LOCKING_H_ */
