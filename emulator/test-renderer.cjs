// Run with Node.js: node test-renderer.cjs
// Exercises the actual canvas renderer from index.html without a browser.
const fs=require('node:fs'),vm=require('node:vm'),assert=require('node:assert/strict');
const html=fs.readFileSync(__dirname+'/index.html','utf8');
const start=html.indexOf('function draw()'),end=html.indexOf('async function pump()',start);
assert.ok(start>=0&&end>start);
const source=html.slice(start,end);
const colors=[[255,0,0],[0,255,0],[0,0,255],[255,255,0],[255,0,255],[0,255,255],[255,255,255]];
for(const state of ['PLAYING','BOSS','BEAT SABER','SIMON SAYS','TRAINING']){
 for(const brightness of [25,127,255]){
  let previous=[];
  for(const pixel of colors){
   const fills=[],ctx={clearRect(){},fillRect(){fills.push(this.fillStyle)}};
   const context={rect:{width:700,height:105},ctx,snapshot:{state,brightness,leds:[pixel]},displayPixels:previous,$:()=>({checked:true}),requestAnimationFrame(){}};
   vm.runInNewContext(source+"\ndraw();",context);
   const expected='rgb('+pixel.map(c=>Math.round(c*brightness/255)).join(',')+')';
   assert.equal(fills.at(-1),expected,`${state} brightness ${brightness}, color ${pixel}`);
   previous=context.displayPixels;
  }
 }
}
let ctx={clearRect(){},fillRect(){}};
const transition={rect:{width:700,height:105},ctx,snapshot:{state:'BASE DESTROYED',brightness:255,leds:[[255,0,0]]},displayPixels:[],$:()=>({checked:true}),requestAnimationFrame(){}};
vm.runInNewContext(source+"\ndraw();",transition);assert.ok(transition.displayPixels[0][0]<255,'Transition smoothing still applies');
console.log('PASS: seven shot colors at three brightness levels in all four gameplay modes; rapid color changes have no dark fade or color trails; transition smoothing retained.');
