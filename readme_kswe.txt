1. GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_LEFT
src/windows/include/GL/glut.h
src/libs/tgfclient/guimenu.cpp - line 55

2. src/libs/tgfclient/control.cpp - line 70
key mapping

3. export/includes/car.h - line 337
struct tCarCtrl
int raceCmd
tdble steer, accelCmd, brakeCmd

4. src/drivers/human/human.cpp - line 470
common_drive() (called by drive_at() or drive_mt())
updateKeys()
s->currentTime

5. src/libs/raceengineclient/raceengine.cpp - line 517
tSituation, ReInfo->s

6. src/libs/raceengineclient/raceenging.cpp - line 619
call ReOneStep(RCM_MAX_DT_SIMU), RCM_MAX_DT_SIMU: 20ms

7. src/libs/raceengineclient/racemanmenu.cpp - line 268
New Race menu -> ReStartNewRace() -> ReStateManage() -> ReUpdate() -> ...
   src/libs/raceengineclient/racestate.cpp - line 56
ReStateManage(void): main loop function - busy waiting
line 97: ReUpdate() -> ReOneStep() -> rbDrive(): drive_at() -> common_drive()

8. src/libs/raceengineclient/raceengine.cpp - line 568
ReStart(): refresh current time (real time)
	called by reScreenActivate() or ReBoardInfo()

9. src/modules/simu/simuv2/simu.cpp - line 314
SimUpdate(): update dynamics (expected)
	called by src/libs/raceengineclient/raceengine.cpp - line 571 update()

10. src/libs/raceengineclient/raceengine.cpp - line 524
ReRaceBigMsgSet(): show message largely
