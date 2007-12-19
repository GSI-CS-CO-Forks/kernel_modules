/* $Id: cdcmTime.h,v 1.2 2007/12/19 09:02:05 ygeorgie Exp $ */
/**
 * @file cdcmTime.h
 *
 * @brief CDCM timing-related header file.
 *
 * @author Georgievskiy Yury, Alain Gagnaire. CERN AB/CO.
 *
 * @date June, 2006
 *
 * Many thanks to Julian Lewis and Nicolas de Metz-Noblat.
 *
 * @version 3.0  ygeorgie  14/03/2007  Production release, CVS controlled.
 * @version 2.0  ygeorgie  27/07/2006  First working release.
 * @version 1.0  ygeorgie  07/07/2006  Initial version.
 */
#ifndef _CDCM_TIME_H_INCLUDE_
#define _CDCM_TIME_H_INCLUDE_

#include "cdcmDrvr.h"
#include <linux/wait.h>
#include <asm/atomic.h>

#define __wait_event_exclusive(wq, condition)				\
do {									\
  DEFINE_WAIT(__wait);							\
  									\
  for (;;) {								\
    prepare_to_wait_exclusive(&wq, &__wait, TASK_UNINTERRUPTIBLE);	\
    if (condition)							\
      break;								\
    schedule();								\
  }									\
  finish_wait(&wq, &__wait);						\
} while (0)

#define wait_event_exclusive(wq, condition)	\
do {						\
  if (condition)				\
    break;					\
  __wait_event_exclusive(wq, condition);	\
} while (0)

#if 0
/* DEFINED IN wait.h */
#define __wait_event_interruptible_exclusive(wq, condition, ret)	\
do {									\
  DEFINE_WAIT(__wait);							\
									\
  for (;;) {								\
    prepare_to_wait_exclusive(&wq, &__wait, TASK_INTERRUPTIBLE);	\
    if (condition)							\
      break;								\
    if (!signal_pending(current)) {					\
      schedule();							\
      continue;								\
    }									\
    ret = -ERESTARTSYS;							\
    break;								\
  }									\
  finish_wait(&wq, &__wait);						\
} while (0)

#define wait_event_interruptible_exclusive(wq, condition)	\
({								\
  int __ret = 0;						\
  if (!(condition))						\
    __wait_event_interruptible_exclusive(wq, condition, __ret);	\
  __ret;							\
})
#endif

void cdcm_sema_cleanup_all(void);

#endif /* _CDCM_TIME_H_INCLUDE_ */
