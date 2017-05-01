# peripheral constants

at robots
.const P_DIST_LEFT 0x4001 # dist in pixels. <0 if off road
.const P_DIST_RIGHT 0x4002 # dist in pixels. <0 if off road
.const P_DIST_FRONT 0x4003 # dist in pixels. <0 if off road

.const P_STEERING 0x4010 # -60 -- 60 (assume initially at 0)

!main
	movi r100 = 50
	stor [P_THROTTLE] = r100
	load r100 = [P_THROTTLE]
	sub r100, 5
	sub r100, 5
	sub r100, 5
	sub r100, 5
	sub r100, 5
	sub r100, 5
	sub r100, 5
	sub r100, 5
	sub r100, 5
	sub r100, 5

!inf
	bra [!inf]

