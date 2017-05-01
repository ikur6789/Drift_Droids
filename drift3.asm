# peripheral constants

.const P_THROTTLE 0x4000
.const P_DIST_LEFT 0x4001 # dist in pixels. <0 if off road
.const P_DIST_RIGHT 0x4002 # dist in pixels. <0 if off road
.const P_DIST_FRONT 0x4003 # dist in pixels. <0 if off road

.const P_STEERING 0x4010 # -60 -- 60 (assume initially at 0)

!main
	movi r100 = 50
	stor [P_THROTTLE] = r100
	load r100 = [P_THROTTLE]
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	movi r100 = 45
	stor [P_THROTTLE] = r100
	load r100 = [P_THROTTLE]
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	movi r100 = 40
	stor [P_THROTTLE] = r100
	load r100 = [P_THROTTLE]
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	movi r100 = 35
	stor [P_THROTTLE] = r100
	load r100 = [P_THROTTLE]
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	movi r100 = 38
	stor [P_THROTTLE] = r100
	load r100 = [P_THROTTLE]
	stor [P_THROTTLE] = r100
	load r100 = [P_THROTTLE]
	stor [P_THROTTLE] = r100
	load r100 = [P_THROTTLE]
	stor [P_THROTTLE] = r100
	load r100 = [P_THROTTLE]
	stor [P_THROTTLE] = r100
	load r100 = [P_THROTTLE]
	stor [P_THROTTLE] = r100
	load r100 = [P_THROTTLE]
	stor [P_THROTTLE] = r100
	load r100 = [P_THROTTLE]
	stor [P_THROTTLE] = r100
	load r100 = [P_THROTTLE]
	stor [P_THROTTLE] = r100
	load r100 = [P_THROTTLE]
	movi r100 = 39
	stor [P_THROTTLE] = r100
	load r100 = [P_THROTTLE]
	NOP
	movi r100 = 25
	stor [P_THROTTLE] = r100
	load r100 = [P_THROTTLE]
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	movi r100 = 20
	stor [P_THROTTLE] = r100
	load r100 = [P_THROTTLE]
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	movi r104 = 90
	stor [P_STEERING] = r104
	stor [P_STEERING] = r104
	stor [P_STEERING] = r104
	stor [P_STEERING] = r104
	stor [P_STEERING] = r104
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	movi r104 = -45
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	stor [P_STEERING] = r104
	stor [P_STEERING] = r104
	stor [P_STEERING] = r104
	stor [P_STEERING] = r104
	stor [P_STEERING] = r104
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	movi r104 = 0
	stor [P_STEERING] = r104
	load r101 = [P_DIST_LEFT]
	load r102 = [P_DIST_RIGHT]
	load r103 = [P_DIST_FRONT]

!inf
	bra [!inf]

