#include <cassert>
#include "sketch.generated.cpp"
void tick(int ms,int mask=0){buttonMask=mask;for(int i=0;i<ms;i++){clockMs++;loop();}}
void scene(int n){buttonMask=0;wifiMode=false;buttonsReleased=true;isWaitingForCombo=false;shots.clear();enemies.clear();bossSegments.clear();bossProjectiles.clear();startLevelIntro(n);tick(4032);}
int main(){try{setup();}catch(Reboot&){restarting=false;setup();}
scene(1);assert(currentState==STATE_PLAYING&&enemies.size()==15);
enemies={{2,0,false},{3,0,false}};enemyFrontIndex=60;shots={{60,2}};tick(16);assert(enemies.size()==1&&enemies[0].color==3);
shots={{enemyFrontIndex,1}};tick(16);assert(enemies.size()==2&&enemies[0].color==1);
scene(6);assert(currentState==STATE_BOSS_PLAYING&&currentBossType==1&&bossSegments.size()==9);int hp=bossSegments[0].hp;shots={{enemyFrontIndex-0.75f,3}};tick(16);assert(bossSegments[0].hp==hp-1);
for(int i=0;i<3;i++){bossProjectiles.clear();shots={{enemyFrontIndex,1}};tick(16);}assert(boss1RageMode);tick(250);assert(!bossProjectiles.empty());
scene(3);assert(currentBossType==2);for(int i=0;i<3000&&boss2State==B2_MOVE;i++)tick(16);assert(boss2State==B2_CHARGE);tick(boss2Cfg.shotFreq*100+32);assert(boss2State==B2_SHOOT);for(int i=0;i<120;i++){bossProjectiles.clear();tick(16);}assert(boss2Section==1&&boss2State==B2_MOVE);assert(bossSegments[0].active);
scene(10);assert(currentBossType==3&&bossSegments.size()==15);auto count=stat_totalShots;tick(100,1|2);tick(32,0);assert(stat_totalShots==count+1);assert(!shots.empty()&&shots.back().color==4);
shots.clear();bossProjectiles.clear();for(int i=0;i<3000&&boss3State==B3_MOVE;i++){bossProjectiles.clear();tick(16);}assert(boss3State==B3_PHASE_CHANGE);tick(4032);assert(boss3PhaseIndex==1);assert(boss3State==B3_BURST||boss3State==B3_WAIT);
bonusReturnLevel=2;startBonusGame();assert(currentState==STATE_BONUS_PLAYING);tick(1000);assert(bonusEnemiesSpawned>0);
startSimonBonus();assert(currentState==STATE_BONUS_SIMON&&simonFullSequence.size()==17);for(int i=0;i<1000&&simonState!=S_INPUT;i++)tick(16);assert(simonState==S_INPUT);int c=simonFullSequence[0];int m=c==1?4:c==2?1:c==3?2:c==4?3:c==5?5:6;tick(120,m);tick(120,0);assert(simonInputIdx==1);
scene(1);enemies={{2,0,false}};enemyFrontIndex=60;shots={{60,2}};tick(16);assert(currentState==STATE_LEVEL_COMPLETED&&currentScore>0&&autoBonusTrigger);tick(5100);assert(currentState==STATE_BONUS_INTRO);
scene(1);for(int i=0;i<3;i++){enemyFrontIndex=3;tick(16);if(i<2){assert(currentState==STATE_PLAYING&&shieldPoints==2-i);tick(1000);}}assert(currentState==STATE_BASE_DESTROYED);tick(2100);assert(currentState==STATE_GAMEOVER);
std::cout<<"PASS: normal collisions, penalties, Tank armor and rage, Masterblaster charge/fire phases, Overlord phases, mixed input, Beat Saber spawning, Simon playback/input, scoring and automatic bonus, base destruction.\n";
}
