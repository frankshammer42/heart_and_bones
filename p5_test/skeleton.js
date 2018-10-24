let serial;
let portName = '/dev/cu.usbmodem1421'; 
let readData;
let prereadData = -1000;
let heartbeat=[0, 0, 0];
let skelton_heartbeat = 0;
let num=60;
let t=0;
let time=0;
let completionrate = 0;
let tr=100;
let timebeat;
let circles=[];
let circleNum = 250;
let heart_tr;
let left_up_tr;
let left_down_tr;
let left_fingers_tr;

let click;
let looper1;
let bpm = 0;


// Serial Data Part 
let connected_signifier_array = [false, false, false];
let total_connected_number = 0;
let current_bone_index = 0;


function preload() {
  // click = loadSound('heartbeat.wav');
}

function serialEvent() {
	let readData = Number(serial.read());
	console.log(readData);
	if (readData >= 0 && readData < 3){
		//Now we've got connection
		connected_signifier_array[readData] = true;
		current_bone_index = readData; 
	}
	if(readData >= 40 && readData <= 120){
		if (readData != prereadData){
			//Now we got new heart beat data
			heartbeat[current_bone_index] = readData;
		}
		//i+=1;
		//num+=20;
		prereadData = readData;
        //looper1.bpm = heatbeat[i];
        //click.rate(heartbeat[i]/60); 
        //looper1.maxIterations = 10;
        //looper1.start();
	 }
}

function setup() {
	createCanvas(1250, 1000);
	rectMode(CENTER);
	textFont('Permanent Marker');
	serial = new p5.SerialPort();
	serial.open(portName);
	serial.on('data', serialEvent);     

	looper1 = new p5.SoundLoop(function(timeFromNow){
    click.play(timeFromNow);
    }, "4n");

//  looper1.maxIterations = 10;
//  looper1.start();

	
	//	let skelton_heart0 = {bpm:skelton_heartbeat,tr:100,function:skelton_heart()};
	// let skelton_leftarm_up0 = {bpm:heartbeat[0],tr:100,function:skelton_leftarm_up()};
	// let skelton_leftarm_down0 = {bpm:heartbeat[1],tr:100,function:skelton_leftarm_down()};
	// let skelton_left_fingers0 = {bpm:heartbeat[2],tr:100,function:skelton_left_fingers()};
	for(let i=0;i<circleNum;i+=1){
		let x = random(width);
		let y = random(height);
		let z = random(40);
		let r = random(60,100);
		let color = {r:random(50,255),g:random(50,255),b:random(50,255),
					//tr:random(50,255)
					};
		circles[i]=new Circle(x,y,z,r,color,i);
	}
}

function draw() {
	let connected_progress_sum = 0;
	let heart_beat_sum = 0;
	for (let i=0; i<3; i++){
		connected_progress_sum += connected_signifier_array[i];
		heart_beat_sum += heartbeat[i];
	}
	completionrate = int((float(connected_progress_sum)/3)*100);
	if (connected_progress_sum == 0){
		skelton_heartbeat = 0;
	}
	else{
		skelton_heartbeat = heart_beat_sum / connected_progress_sum;
	}
	background(0);
	push();
	//scale(1.8,1.8);
	translate(2.2*width/5,50);
	skelton_heart();
	skelton_leftarm_up();
	skelton_leftarm_down();
	skelton_left_fingers();
	fill(255,255-num,255,tr+num);
	noStroke();
	skelton_neck();
	skelton_head();
	skelton_body();
	skelton_rightarm_up();
	skelton_rightarm_down();
	skelton_hips();
	skelton_leftleg_up();
	skelton_leftleg_down();
	skelton_rightleg_up();
	skelton_rightleg_down();
	translate(0,-timebeat*heartbeat/30);
	pop();
	//pumpkin();
	fill(255);
	stroke(255);
	textSize(60);
	text(floor(completionrate)+"% Completed!!!",width/3,height-100);
	time++;
	if(floor(time/60)%2==0){timebeat = time%120;}
	for(let i=0;i<circleNum;i+=1){
		circles[i].makeline();}
	for(let j=0;j<circleNum;j+=1){
		circles[j].move();}
	for(let jj=0;jj<circleNum;jj+=1){
		circles[jj].makepoint();}
}

// function blink(fff){
// 	if(this.bpm==0){
// 		this.tr=40;
// 		this.function();}
// 	else{
// 		if(floor(time*this.bpm/1800)%2==0){
// 				this.tr=120;
// 				this.function();}
// 		else{
// 				this.tr=40;
// 				this.function();
// }
// 	}
// }

function skelton_heart(){
	if(skelton_heartbeat==0){
		heart_tr=60;}
	else{
		if(floor(time*skelton_heartbeat/1800)%2==0){
				heart_tr=255;}
		else{heart_tr=60;}}
	push();
	noStroke();
	fill(255,0,0,heart_tr);
	arc(10,110,20,20,PI,2*PI);
	arc(30,110,20,20,PI,2*PI);
	triangle(0,110,40,110,20,130);
	pop();
}

function skelton_head(){
	push();
	translate(50);
	noStroke();
	arc(0,-8,70,80,5*PI/6,13*PI/6); //head
	quad(-30,-8,-25,24,25,24,30,-8); //face
	//arc(0,-1,50,50,0,PI) //gear;
	stroke(255);
	curve(0,24,-25,24,-20,49,0,49); 
	curve(-20,49,-20,49,20,49,20,49); 
	curve(0,49,20,49,25,24,0,24); 
	curve(45,24,25,24,-25,24,45,24); 
	//20,49,25,24,4);	//gear
	noStroke();
	//triangle(-20,49,20,49,0,54,4);	//gear
	fill(0);
	rect(-14,0,16,15,5);  //eye
	rect(14,0,16,15,5);  //eye
	quad(-4,6,-5,15,5,15,4,6);	//nose
	rect(12,26,8,8,2);
	rect(4,28,8,8,2);
	rect(-4,28,8,8,2);
	rect(-12,26,8,8,2);
	rect(0,32,16,6,2);
	pop();
}


function skelton_neck(){
	push();
	for(let i=0;i<4;i+=1){
		rect(0,8*i+55,10,6,2);
	}
	pop();
}

function skelton_body(){
	push();
	for(let i=0;i<20;i+=1){
		rect(0,5*i+90,8,2,2);
	}
	for(let i=0;i<10;i+=1){
		rect(-25,8*i+90,30,4,4);
	}
	for(let i=0;i<10;i+=1){
		rect(25,8*i+90,30,4,4);
	}
	pop();
}

function skelton_leftarm_up(){
	if(heartbeat[0]==0){
		left_up_tr=60;}
	else{
		if(floor(time*heartbeat[0]/1800)%2==0){
				left_up_tr=255;}
		else{left_up_tr=60;}}
	push();
	noStroke();
	fill(255,255-num,255,left_up_tr);
	if (connected_signifier_array[0] && heartbeat[0] > 0){
		textSize(25);
		text('left arm(up) BPM: '+ heartbeat[0],-400,100);
	}
	rotate(0.5);
	rect(0,110,15,60,5);
	pop();
}

function skelton_rightarm_up(){
	push();
	textSize(12);
	rotate(-0.5);
	rect(0,110,15,60,5);
	pop();
}

function skelton_leftarm_down(){
	if(heartbeat[1]==0){
		left_down_tr=60;}
	else{
		if(floor(time*heartbeat[1]/1800)%2==0){
				left_down_tr=255;}
		else{left_down_tr=60;}}
	push();
	noStroke();
	fill(255,255-num,255,left_down_tr);
	if (connected_signifier_array[1] && heartbeat[1] > 0){
		textSize(25);
		text('left arm(down) BPM: '+ heartbeat[1],-420,150);
	}
	rotate(0.1);
	rect(-55,160,15,60,5);
	pop();
}

function skelton_left_fingers(){
	if(heartbeat[2]==0){
		left_fingers_tr=60;}
	else{
		if(floor(time*heartbeat[2]/1800)%2==0){
				left_fingers_tr=255;}
		else{left_fingers_tr=60;}}
	push();
	noStroke();
	fill(255,255-num,255,left_fingers_tr);
	if (connected_signifier_array[2] && heartbeat[2] > 0){
		textSize(25);
		text('left hand fingers BPM: '+ heartbeat[2],-440,200);
	}
	for(let i=0;i<5;i+=1){
		rotate(0.025);
		rect(-60-i/4,200,5,20+i,1);
	}	
	pop();
}

function skelton_rightarm_down(){
	push();
	let angletime = time%240;
	translate(70,130);
	if(floor(time/120)%2==0){
		push();
		rotate(-2.6+0.01*angletime);
		rect(7.5,30,15,60,5);
	for(let i=0;i<5;i+=1){
		rotate(-0.08);
		rect(-10+i/4,70,5,20+i/2,1);
	}	
		pop();
	}
	else{
		push();
		rotate(-0.2-0.01*angletime);
		rect(7.5,30,15,60,5);
	for(let i=0;i<5;i+=1){
		rotate(-0.08);
		rect(-10+i/4,70,5,20+i/2,1);
	}	
		pop();
	}
	pop();
}

function skelton_hips(){
	push();
	rotate(-0.2);
	ellipse(-20,200,30,40);
	rotate(0.4);
	ellipse(20,200,30,40);
	pop();
}

function skelton_leftleg_up(){
	push();
	rotate(0.1);
	rect(0,250,20,60,5);
	pop();
};

function skelton_rightleg_up(){
	push();
	rotate(-0.1);
	rect(0,250,20,60,5);
	pop();
};

function skelton_leftleg_down(){
	push();
	rect(-25,310,20,60,5);
	translate(-20,310);
	rotate(-0.45);
	for(let i=0;i<5;i+=1){
		rotate(0.15);
		rect(-5+i/2,45,5,20,1);
	}	
	pop();
};

function skelton_rightleg_down(){
	push();
	rect(25,310,20,60,5);
	translate(20,310);
	rotate(0.45);
	for(let i=0;i<5;i+=1){
		rotate(-0.15);
		rect(5-i/2,45,5,20,1);
	}	
	pop();
};


function pumpkin(){
	push();
	translate(150,0);
	strokeWeight(8);
	stroke(0,180,0,220)
	curve(60,270,0,270,20,250,20,300)
	fill(255,180,0,220)
	stroke(110);
	strokeWeight(1);
	arc(-1,300,100,60,PI/2+0.1,3*PI/2-0.1);
	arc(-1,300,80,60,PI/2+0.1,3*PI/2-0.1)
	arc(0,300,50,60,PI/2+0.1,3*PI/2-0.1)
	arc(0,300,20,60,PI/2,3*PI/2)
	arc(0,300,100,60,3*PI/2,5*PI/2);
	arc(0,300,80,60,3*PI/2+0.1,5*PI/2-0.1)
	arc(-1,300,50,60,3*PI/2+0.1,5*PI/2-0.1)
	arc(-1,300,20,60,3*PI/2+0.1,5*PI/2-0.1)
	fill(0);
	triangle(-4,300,-16,285,-28,300,5);
	triangle(4,300,16,285,28,300,5);
	triangle(-6,310,6,310,0,300,5);
	arc(0,315,40,15,0,PI)
	pop();
}

class Circle{
	constructor(px,py,pz,pr,pcolor,pid){
		this.x = px;
		this.y = py;
		this.z = pz;
		this.r = pr;
		this.movex = random(-1,1); 
		this.movey = random(-1,1); 
		this.movez = random(-1,1); 
		this.color = pcolor;
		this.id = pid;	
		this.rr = sqrt(this.r*this.r-this.z*this.z);
	}
	
	move(){
		this.x+= this.movex;
		this.y+= this.movey;
		this.z+= this.movez;
		this.rr = sqrt(this.r*this.r-this.z*this.z);
		if(this.x>width + this.rr){this.x = -this.rr}
		else if(this.x<-this.rr){this.x = width + this.rr}
		if(this.y>height + this.rr){this.y = -this.rr}
		else if(this.y<-this.rr){this.y = height + this.rr}		
		if(this.z>this.r){this.z = -this.r}
		else if(this.z<-this.r){this.z = this.r}		
	}

	makepoint(){
		fill(this.color.r,this.color.g,
		            this.color.b, random(100,150));
		strokeWeight(3);
		stroke(this.color.r,this.color.g,
		            this.color.b, random(100,150));
		point(this.x, this.y);
	}

	makeline(){
	for(let p = this.id + 1 ; p < circleNum; p+=1){
    		circles[this.id].intersect(circles[p]);
	}
}

　　　　　　　intersect(other){
	let dx = other.x - this.x;
	let dx2 = dx*dx;
	let dy = other.y - this.y;
	let dy2 = dy*dy;	   
	let d2 = dx*dx+dy*dy;
	let d = sqrt(d2);
	let rr2 = this.rr * this.rr;
	let RR2 = other.rr * other.rr;

if(d < this.rr + other.rr && d > abs(this.rr - other.rr)){	 
	let K = rr2-RR2+d2; 
	let K2 = K * K;
	let h = sqrt(4 * rr2 * d2 - K2);
	let x1 = this.x + (dx * K + dy * h)/(2*d2);
	let x2 = this.x + (dx * K - dy * h)/(2*d2);
	let y1 = this.y + (dy * K - dx * h)/(2*d2);
	let y2 = this.y + (dy * K + dx * h)/(2*d2);
	strokeWeight(1);
	stroke((this.color.r+other.color.r)/2,
		   (this.color.g+other.color.g)/2,
		   (this.color.b+other.color.b)/2,
		   random(100,150));
	line(x1,y1,x2,y2);}
 }
}
