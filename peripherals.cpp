#include <stdlib.h>

#include "miniat/miniat.h"
#include "peripherals.h"


#define P_THROTTLE (0x4000)
#define P_DIST_LEFT (0x4001)
#define P_DIST_RIGHT (0x4002)
#define P_DIST_FRONT (0x4003)
#define P_STEERING (0x4010)


void peripherals_cleanup() {

	/* nothing yet */

	return;
}

void peripherals_clock(miniat *m, vehicle &car) {

	m_bus bus;

	if(!m) {
		fprintf(stderr, "NULL MiniAT on peripheral clock\n");
		exit(EXIT_FAILURE);
	}

	bus = miniat_pins_bus_get(m);

	if(bus.req == M_HIGH && bus.ack == M_LOW) {
		/*
		 * The MiniAT is waiting for a peripheral acknowledgement
		 */
		bool handled = true;
		if(bus.rW == M_HIGH) {

			/* STOR to peripheral */

			switch(bus.address) {
			case P_THROTTLE:
			    car.set_vel(bus.data);
			    //printf("STORed %d to the P_THROTTLE peripheral\n", (signed)bus.data);
				break;
			case P_DIST_LEFT:
				//printf("STORed %d to the P_TURN_ANGLE peripheral\n", (signed)bus.data);
				break;
			case P_DIST_RIGHT:
				//printf("STORed %d to the P_OFFSET_TURN_ANGLE peripheral\n", (signed)bus.data);
				break;
			case P_DIST_FRONT:
				//printf("STORed %d to the P_WEAPON peripheral\n", (signed)bus.data);
				break;
			case P_STEERING:
				//printf("STORed %d to the P_WEAPON_TURN peripheral\n", (signed)bus.data);
				car.set_angle(bus.data);
				break;
			default:
				fprintf(stderr, "There is no peripheral accepting writes at 0x%04X\n", bus.address);
				/*
				 * Here, it would be good to set "handled" to false so the ACK bus
				 * line isn't set high automatically.  It may hang the system, but
				 * devices that aren't there shouldn't be communicated with.
				 */
				break;
			}
		}
		else {

			/* LOAD from peripheral */

			switch(bus.address) {
			case P_THROTTLE:
			    bus.data=car.vel_f();
			    //printf("ring ring function calls back: %f \n", car.vel_f());
				//printf("LOADed from P_THROTTLE\n");
				//printf("\n'Dat number is %d\n", (signed)bus.data);
				break;
			case P_STEERING:
				bus.data=car.angle_f();
				printf("MiniAT: LOADed from P_TURN_ANGLE\n");
				break;
			case P_DIST_LEFT:
				bus.data=car.spaceLeft_f();
				printf("MiniAT: LOADed from P_TURN_LEFT Dist: %d\n", car.spaceLeft_f());
				break;
			case P_DIST_RIGHT:
				bus.data=car.spaceRight_f();
				printf("MiniAT: LOADed from P_TURN_RIGHT Dist: %d\n", car.spaceRight_f());
				break;
			case P_DIST_FRONT:
				bus.data=car.spaceUp_f();
				printf("MiniAT: LOADed from P_TURN_FRONT Dist: %d\n", car.spaceUp_f());
				break;
			default:
				fprintf(stderr, "There is no peripheral accepting reads at 0x%04X\n", bus.address);
				break;
			}
		}

		if(handled) {
			bus.ack = M_HIGH;
		}
	}
	else if(bus.ack) {
		/*
		 * The MiniAT is waiting for the peripheral to lower the ACK pin.
		 * Devices intended to take longer than normal should add delay
		 * "case"s here.
		 */
		switch(bus.address) {
		default:
			bus.ack = M_LOW;
			break;
		}
	}

	miniat_pins_bus_set(m, bus);

	return;
}
