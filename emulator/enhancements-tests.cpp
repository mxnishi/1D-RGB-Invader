#include <cassert>
#include <iostream>
#include "sketch.generated.cpp"
void tick(int ms,int mask=0,int frame=1){buttonMask=mask;for(int i=0;i<ms;i+=frame){clockMs+=frame;loop();}}
void wave(int count){config_num_leds=count;config_homebase_size=3;FastLED.addLeds<LED_TYPE,PIN_LED_DATA,COLOR_ORDER>(leds,count+1);shots.clear();bossProjectiles.clear();buttonsReleased=false;startLevelIntro(1);tick(4032);assert(currentState==STATE_PLAYING);}
void trainingFire(int mask){tick(160,0);tick(200,mask);tick(16,0);}
int main(){try{setup();}catch(Reboot&){restarting=false;setup();}
wave(100);currentScore=1234;highScore=5678;auto totalShots=stat_totalShots,totalKills=stat_totalKills;int lastShots=stat_lastGameShots;
startTraining();assert(currentState==STATE_TRAINING);trainingFire(4);for(int i=0;i<1500&&feedbackKind!=FB_MISTAKE;i++)tick(16);assert(trainingStage==0&&feedbackKind==FB_MISTAKE&&!trainingAdvanceAt);
int masks[]={1,2,4,4,3,5,6,7};
for(int stage=0;stage<8;stage++){
 assert(trainingStage==stage);trainingFire(masks[stage]);
 if(stage==3){for(int i=0;i<1000&&!trainingMistakeShown;i++)tick(16);assert(trainingMistakeShown&&trainingAddedColor==1);}
 for(int i=0;i<1000&&trainingStage==stage;i++)tick(16);
 assert(trainingStage==stage+1);
 assert(currentScore==1234&&highScore==5678&&stat_totalShots==totalShots&&stat_totalKills==totalKills&&stat_lastGameShots==lastShots);
}
for(int mask:{1,2,4}){int before=trainingHits;trainingFire(mask);for(int i=0;i<1000&&trainingStage==8&&trainingHits==before;i++)tick(16);}
for(int i=0;i<1000&&trainingStage==8;i++)tick(16);assert(trainingStage==9&&currentState==STATE_TRAINING);assert(currentScore==1234&&stat_totalShots==totalShots);tick(32,0);tick(16,1);assert(currentState==STATE_INTRO);
// An ignored moving target resets safely rather than ending practice.
startTraining();setTrainingStage(8);trainingFront=3.001f;tick(16);assert(currentState==STATE_TRAINING&&trainingFront>50);
// Short press restarts campaign; medium press starts practice, long press enters Wi-Fi.
wave(100);tick(1500,8);tick(32,0);assert(currentState==STATE_TRAINING);tick(3200,8);tick(32,0);assert(wifiMode);wifiMode=false;btnWhiteHeld=false;
// Equal real reaction time across different strip lengths and frame rates.
unsigned long shortest=1000000,longest=0;
for(int count:{100,240,600}){for(int frame:{1,32}){wave(count);unsigned long start=millis();for(int i=0;i<6000&&currentState==STATE_PLAYING&&shieldPoints==MAX_SHIELD;i++)tick(32,0,frame);unsigned long elapsed=millis()-start;assert(currentState==STATE_PLAYING&&shieldPoints==2);shortest=min(shortest,elapsed);longest=max(longest,elapsed);std::cout<<count<<" LEDs, scheduler "<<frame<<" ms: "<<elapsed<<" ms to base\n";}}
assert(longest-shortest<=64);
wave(100);enemies={{2,0,false},{3,0,false}};enemyFrontIndex=60;shots={{60,2}};tick(16);assert(feedbackKind==FB_HIT&&leds[ledStartOffset].g==255);
shots={{enemyFrontIndex,1}};tick(16);assert(feedbackKind==FB_MISTAKE&&enemies[0].color==1&&leds[ledStartOffset].r==255);
shots.clear();enemyFrontIndex=5;feedbackUntil=millis();tick(16);assert(baseInDanger&&feedbackKind==FB_DANGER);
startLevelIntro(3);tick(4032);shots={{enemyFrontIndex,1}};tick(16);assert(feedbackKind==FB_BLOCKED);
// Normalization must not cause fast shots to skip front boss segments.
config_num_leds=1100;FastLED.addLeds<LED_TYPE,PIN_LED_DATA,COLOR_ORDER>(leds,1101);startLevelIntro(6);tick(4032);int frontHp=bossSegments[0].hp;shots={{enemyFrontIndex-1.0f,3}};config_shot_speed_pct=150;tick(16);assert(bossSegments[0].hp==frontHp-1);
// Long-strip bonus projectiles must not tunnel past targets.
config_num_leds=1100;FastLED.addLeds<LED_TYPE,PIN_LED_DATA,COLOR_ORDER>(leds,1101);startBonusGame();currentScore=0;bonusReturnLevel=2;bonusEnemiesSpawned=200;bonusEnemies={{2,25,false}};bonusShots={{20,2}};config_shot_speed_pct=150;frameSeconds=.016;updateBonusGame();assert(currentScore==500&&bonusEnemies.empty());
std::cout<<"PASS: all nine training lessons, wrong attempts, penalty demo, unscored practice, moving-target retry, menu access, length/time normalization, hit/miss/blocked/danger feedback, and long-strip swept collisions.\n";
}
