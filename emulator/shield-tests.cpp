#include <cassert>
#include <iostream>
#include "sketch.generated.cpp"
void tick(int ms,int mask=0){buttonMask=mask;for(int i=0;i<ms;i++){clockMs++;loop();}}
void scene(int level){resetShield();wifiMode=false;btnWhiteHeld=false;bossProjectiles.clear();shots.clear();startLevelIntro(level);tick(4032);}
void breach(){enemyFrontIndex=config_homebase_size;tick(16);}
int main(){try{setup();}catch(Reboot&){restarting=false;setup();}
scene(1);currentScore=4321;int oldLast=lastGames[0];auto shotsBefore=stat_totalShots,killsBefore=stat_totalKills;
breach();assert(shieldPoints==2&&currentState==STATE_PLAYING&&shieldRecovering());assert(enemyFrontIndex>config_homebase_size);assert(lastGames[0]==oldLast&&currentScore==4321);assert(feedbackKind==FB_SHIELD);
assert(leds[ledStartOffset].r>0&&leds[ledStartOffset+1].r>0&&leds[ledStartOffset+2].r==0);
// An overlapping contact in the recovery window cannot drain more points.
breach();assert(shieldPoints==2&&currentState==STATE_PLAYING);tick(1000);breach();assert(shieldPoints==1&&currentState==STATE_PLAYING);
assert(leds[ledStartOffset].r>0&&leds[ledStartOffset+1].r==0&&leds[ledStartOffset+2].r==0);
tick(1000);breach();assert(shieldPoints==0&&currentState==STATE_BASE_DESTROYED&&lastGames[0]==4321);int secondLast=lastGames[1];damageShield();triggerBaseDestruction();assert(lastGames[0]==4321&&lastGames[1]==secondLast);tick(2100);assert(currentState==STATE_GAMEOVER);
assert(stat_totalShots==shotsBefore&&stat_totalKills==killsBefore);
// A new run refills; normal level and bonus transitions do not.
startLevelIntro(config_start_level);assert(shieldPoints==3&&!shieldRecovering());tick(4032);breach();assert(shieldPoints==2);startLevelIntro(2);assert(shieldPoints==2);tick(4032);assert(shieldPoints==2);
bonusReturnLevel=3;startBonusGame();bonusLives=0;tick(16);assert(currentState==STATE_INTRO&&shieldPoints==2&&currentLevel==3);
startSimonBonus();simonLives=0;tick(16);assert(shieldPoints==2);
startTraining();setTrainingStage(8);trainingFront=3;tick(16);assert(currentState==STATE_TRAINING&&shieldPoints==2);assert(!damageShield()&&shieldPoints==2);
setTrainingStage(9);tick(32);tick(16,1);assert(currentState==STATE_INTRO&&shieldPoints==3);
// Projectile breaches apply equally to every boss; armor/phase state survives.
for(int level:{3,6,10}){
 scene(level);auto armor=bossSegments.size();int hp=bossSegments[0].hp;currentScore=900;
 bossProjectiles={{config_homebase_size+.01f,1},{config_homebase_size+.01f,2},{config_homebase_size+.01f,3}};
 tick(16);assert(shieldPoints==2&&currentState==STATE_BOSS_PLAYING&&bossProjectiles.empty());assert(bossSegments.size()==armor&&bossSegments[0].hp==hp&&currentScore==900);
 // Incoming shots during recovery disappear, without consuming another point.
 bossProjectiles={{2,1},{2,2}};tick(16);assert(shieldPoints==2&&bossProjectiles.empty());tick(1000);
 bossProjectiles={{2,1}};tick(16);assert(shieldPoints==1&&currentState==STATE_BOSS_PLAYING);tick(1000);
 bossProjectiles={{2,1},{2,2}};tick(16);assert(shieldPoints==0&&currentState==STATE_BASE_DESTROYED);
}
// Boss body breaches also use the shield, with knockback rather than repeated contact.
scene(6);auto armor=bossSegments.size();breach();assert(shieldPoints==2&&currentState==STATE_BOSS_PLAYING&&enemyFrontIndex>3&&bossSegments.size()==armor);
// Wrong-color mistakes are separate from base damage.
scene(1);enemies={{2,0,false}};enemyFrontIndex=60;shots={{60,1}};tick(16);assert(shieldPoints==3&&enemies.size()==2);
// A one-pixel base displays fractional brightness instead of hiding the charge.
config_homebase_size=1;shieldPoints=1;feedbackUntil=0;baseInDanger=false;drawGameFeedback();assert(leds[ledStartOffset].r>0&&leds[ledStartOffset].r<100);
std::cout<<"PASS: three breaches end a run; recovery prevents burst damage; all bosses and body hits use shield; wave knockback and projectile cleanup; shield carries across levels/bonuses; training isolated; restart refills; game-end statistics recorded once; base LEDs show charge.\n";
}
