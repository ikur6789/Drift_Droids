#ifndef __PERIPHERALS_H__
#define __PERIPHERALS_H__

#include "miniat/miniat.h"
#include "vehicle.h"


extern int keyb_update_counter;


extern void peripherals_cleanup();
extern void peripherals_clock(miniat *m, vehicle &car);


#endif /*  __PERIPHERALS_H__ */
