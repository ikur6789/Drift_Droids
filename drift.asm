# peripheral constants

.const P_THROTTLE 0x4000 # -100 -- 100 (assume initially at 0)
.const P_DIST_LEFT 0x4001 # dist in pixels. <0 if off road
.const P_DIST_RIGHT 0x4002 # dist in pixels. <0 if off road
.const P_DIST_FRONT 0x4003 # dist in pixels. <0 if off road

.const P_STEERING 0x4010 # -60 -- 60 (assume initially at 0)

.const FRONT_THRESHOLD 200

####################
# REGISTERS
#
# r1: front threshold
#
# r100: throttle
# r101: left dist reading
# r102: right dist reading
# r103: front dist reading
# r104: steering
####################

!main

	movi r1 = FRONT_THRESHOLD
    movi r100 = 100
    stor [P_THROTTLE] = r100
	load r100 = [P_THROTTLE]
	bra [!main]

!check_front
	load r103 = [P_DIST_FRONT]
	brag [!middle_steer], r103 > r1

# turn ahead
	movi r100 = 50
	stor [P_THROTTLE] = r100
	load r101 = [P_DIST_LEFT]
	load r102 = [P_DIST_RIGHT]
	bral [!turn_right], r101 < r102

# turn left
	movi r104 = -45
	stor [P_STEERING] = r104
	bra [!check_front]

!turn_right
	movi r104 = 45
	stor [P_STEERING] = r104
	bra [!check_front]

!middle_steer
	movi r100 = 100
	stor [P_THROTTLE] = r100
	load r101 = [P_DIST_LEFT]
	load r102 = [P_DIST_RIGHT]
	bral [!veer_right], r101 < r102

# veer left
	movi r104 = -5
	stor [P_STEERING] = r104
	bra [!check_front]

!veer_right
	movi r104 = 5
	stor [P_STEERING] = r104
	bra [!check_front]

!inf
	bra [!inf]
