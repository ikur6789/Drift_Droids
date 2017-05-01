#include <stdbool.h>

#include "miniat/miniat.h"
#include "ports.h"


/*
 * Here are some fake condition booleans for determining pin
 * values.  In the real application, you'd have to track this
 * somehow.
 */
bool still_moving = true;
bool still_turning = true;
bool still_taxiing = true;


void ports_cleanup() {

	/* nothing yet */

	return;
}

void ports_clock(miniat *m) {

	static m_wyde port_a = { .u = 0 };

	port_a.bits.bit0 = still_moving ? 1 : 0;
	port_a.bits.bit1 = still_turning ? 1 : 0;
	port_a.bits.bit2 = still_taxiing ? 1 : 0;

	miniat_pins_set_gpio_port(m, m_gpio_id_A, port_a);

	return;
}
