#include "defs.h"
 u16 const  down = 0x07;
 u16 const  up = 0x06;
 u16 const  left = 0x05;
 u16 const  right = 0x04;
 u16 const  b = 0x01;
 u16 const  start = 0x03;
 u16 const  a = 0x00;

 u8 const ScreenWidth=240;
 u8 const ScreenHieght=160;//these were orginally u8 constances
 const struct Ship *shipptr[4];//these break everything if put into defs
 u8 shipcount;

#include <tonc_oam.h>
#include <tonc_core.h>
#include <tonc_types.h>
#include <tonc_bios.h>
#include <tonc_math.h>

#include "enemy.h"
#include "graphics.h"
#include "gameoverscreen.h"
#include "spritepositionfunctions.h"
#include "music.h"
#include "score.h"
#include "bg.h"
//#include "interupts.h" we will work on this later.

#include "ship_bin.h"
#include "shipprop_bin.h"
#include "numbers_bin.h"
#include "background.h"
#include "bullet_bin.h"
#include "font_bin.h"
#include "exsplosion_bin.h"
#include "underline_bin.h"


#include "clouds_bin.h"


#include "playerpallete_bin.h"
#include "enemypallete_bin.h"
#include "backgroundpallete_bin.h"

u8 level;//this makes the enemy more likely to shoot
u16 score;
//sprite position functions
	   
// start screen will get ts own file:
void startscreen(void);
			   
static inline void waitforstart(){
    u8 startpressed=0;
    vu16 button=0xff;
    while(startpressed==0){
        qran_range(0,0xffff);
        button=*Button_Reg & 0x03ff;
        if (button==((((0xffff)^(1<<start))) &0x03ff )){
            startpressed=1;
        }
    }

}
    

        


bool gameover=false;
u32 xyout; //this is jsut used to get x y out of functions where it is stored together as one long value
u8 exsplosion=0;

int main(){
	u16 bulletwaittimer=0;
	bool openfire=false;
	u8 level=startinglevel;
	initilizebuffervariables();
	
	initleaderboard();
	
	if (verifysave()){
		loadscore();
	}
	else{
		leaderboardpreset();
	}

	
	
    vsync();
	fillbuffer();
	switchoutdma();
	initmusicregisters();
	scrollbackground();
	
	
	OAM_CLEAR();
    DisplayController=0x1000;
    tonccpy(&Mem_Tile[4][1],ship_bin,ship_bin_size);
	tonccpy(&Mem_Tile[4][0x41],shipprop_bin,shipprop_bin_size);
	
    tonccpy(Mem_Obj_Palette,playerpallete_bin,playerpallete_bin_size);
	tonccpy(Mem_Obj_Palette+16,enemypallete_bin,enemypallete_bin_size);

    //tonccpy(Mem_Obj_Palette[16],enemyPal,enemyPalLen);
    
    tonccpy(&Mem_Tile[4][4],bullet_bin,bullet_bin_size);
	tonccpy(&Mem_Tile[4][5],exsplosion_bin,exsplosion_bin_size);
    
	tonccpy (&Mem_Tile[4][0x18],numbers_bin,numbers_bin_size);
	tonccpy (&Mem_Tile[4][0x20],font_bin,font_bin_size);
	
	tonccpy(&Mem_Tile[4][0x1f], underline_bin,underline_bin_size);
	
    struct ObjectAttribute SpriteAttribute={0b0010000000000000, 0b0000000000000000, 0b0000000010000000};
    tonccpy(Obj_Attributes,&SpriteAttribute,12);

	initbackground();
	
	loadbackground(); 
	
	//done with hardwareinit

	

    shipcount=2;
	struct Ship Player;
    shipptr[0]=&Player;
	struct Ship Enemy;
    shipptr[1]=&Enemy;
    
	volatile struct BulletAttribute ab0;
    Player.bullet[0]=&ab0;
    volatile struct BulletAttribute ab1;
    Player.bullet[1]=&ab1;
    volatile struct BulletAttribute ab2;
    Player.bullet[2]=&ab2;
    volatile struct BulletAttribute ab3;
    Player.bullet[3]=&ab3;
 
    
    
    volatile struct BulletAttribute eb0;
    Enemy.bullet[0]=&eb0;
    volatile struct BulletAttribute eb1;
    Enemy.bullet[1]=&eb1;
    volatile struct BulletAttribute eb2;
    Enemy.bullet[2]=&eb2;
    volatile struct BulletAttribute eb3;
    Enemy.bullet[3]=&eb3;
	
	startscreen();
	
	while (1){

		Player.x=8000;
		Player.y=8000;
		Player.speed=0x0100;
		Player.button=0xffff;
		Player.angle=0;
		Player.turntimer=0;
		Player.acceleration=01;
		Player.deceleration=02;
		Player.maxspeed= 0x0200;
		Player.minspeed=0x00c0;
		Player.bulletactive=0x00;
		Player.ally=true;
		Player.hit=false;
        Player.bulletcount=0;
        Player.bulletcooldown=0;
		
		
		
		Enemy.x=00;
		Enemy.y=000;
		Enemy.speed=0x0100;
		Enemy.button=0xffff;
		Enemy.angle=0;
		Enemy.turntimer=0;
		Enemy.acceleration=01;
		Enemy.deceleration=02;
		Enemy.maxspeed= 0x0200;
		Enemy.minspeed=0x00c0;
		Enemy.bulletactive=0x00;
		Enemy.ally=false;
		Enemy.hit=false;
        Enemy.bulletcount=0;
        Player.bulletcooldown=0;
		
/*
		Bullet.ally=true;
		Bullet.x=Player.x;
		Bullet.y=250<<8;
		Bullet.BulletTimer=0;
		Bullet.BulletSpeed=0x301;
		Bullet.angle=0;
		*/


		eb0.ally=false;
		eb0.x=Enemy.x;
		eb0.y=250<<8;
		eb0.BulletTimer=0;
		eb0.BulletSpeed=0x301;
		eb0.angle=0;

		bulletwaittimer=0;
		openfire=false;
		gameover=false;
		exsplosion=0;
		score=0;
		
		while(!gameover){
			++Player.turntimer;
			++Enemy.turntimer;
			++bulletwaittimer;
			if (bulletwaittimer==0x2f){ //later on we should make this the ammount of time it takes for the enemy to spawn
				openfire=true;
			}
			Player.turntimer= Player.turntimer & 0x07;
			Enemy.turntimer=Enemy.turntimer & 0x07;
			
			Player.button=*Button_Reg & 0x03ff;

			if (Player.turntimer==0x00){
				if (Player.button & 1<<left){//5=left
					++Player.angle;
				}
				if (Player.button & 1<<right){//4=right
					--Player.angle;
				}
				Player.angle=Player.angle & 0x03;
			}
			
			
			
			if (!(Player.button & 1<<up)){//7=down
				Player.speed=Player.speed+Player.acceleration;
				if (Player.speed>Player.maxspeed){
					Player.speed=Player.maxspeed;
				}
			}
			if (!(Player.button & 1<<down)){
				Player.speed=Player.speed-Player.deceleration;
				if (Player.speed<Player.minspeed){
					Player.speed=Player.minspeed;
				}
			}
			
			xyout= anglespeed(Player.angle,Player.speed,Player.x,Player.y);
			Player.x=(xyout>>16);
			Player.y=(xyout&0xffff);
			
				
			
			
			

			
			
			
	   
			if (Enemy.hit==false){

				if (Enemy.turntimer==0x00 ){
					Enemy.button=enemyai(Player.x,Player.y,Player.speed, Enemy.x,Enemy.y,Enemy.speed,Enemy.angle,Enemy.bulletactive, Player.angle);
					
					if (Enemy.button & 1<<5){//5=left
						++Enemy.angle;
					}
					if (Enemy.button & 1<<4){//4=right
						--Enemy.angle;
					}
					Enemy.angle=Enemy.angle & 0x03;
				}
				
				
				
				if (!(Enemy.button & 1<<up)){//7=down
					Enemy.speed=Enemy.speed+Enemy.acceleration;
					if (Enemy.speed>Enemy.maxspeed){
						Enemy.speed=Enemy.maxspeed;
					}
				}
				if (!(Enemy.button & 1<<down)){
					Enemy.speed=Enemy.speed-Enemy.deceleration;
					if (Enemy.speed<Enemy.minspeed){
						Enemy.speed=Enemy.minspeed;
					}
				}
				
				xyout= anglespeed(Enemy.angle,Enemy.speed,Enemy.x,Enemy.y);
				Enemy.x=(xyout>>16);
				Enemy.y=(xyout&0xffff);
				
				
				



					
				

					
				
				
				
				
				if ( (((ABS(ab0.x-Enemy.x))<0x600  && (ABS(ab0.y-Enemy.y))<0x600  )) && ab0.active){
					Enemy.hit=true;
					Enemy.button=0xffff;
					Enemy.bulletactive=0;
					
					/*if ( (ABS(Player.x-Enemy.x))<0x400  && (ABS(Player.y-Enemy.y))<0x400 && Enemy.hit==false){
						gameover=true; //this ends the game in the event of a collision even after being shot
						Enemy.hit=true;
						Player.hit=true;
						}
					 else{*/
					
					
					++score; //}
					
				}
				
				if ( (ABS(Player.x-Enemy.x))<0x800  && (ABS(Player.y-Enemy.y))<0x800 && Enemy.hit==false){
					Player.hit=true;
					Enemy.hit=true;
					
					gameover=true;
					
				}
			
			
		  
				/*if ( (ABS(Player.x-Enemy.x))<0x400  && (ABS(Player.y-Enemy.y))<0x400 && Enemy.hit==false){
					gameover=true; //this ends the game in the event of a collision
					Enemy.hit=true;
					Player.hit=true;
					}
				 else{*/
				
			

			
				if ( (((ABS(eb0.x-Player.x))<0x400  && (ABS(eb0.y-Player.y))<0x400  )) && Enemy.bulletactive!=0){
					Player.hit=true;
					gameover=true;
				}
				
			}
			

			
			vsync();
			fillbuffer();
			switchoutdma();
			OAM_CLEAR();
			scrollbackground();

			int directionadder=0;
			switch (Player.turntimer&0x4){
				case 0x00:
					directionadder=0x80;
					break;
			}
			
			if (!Player.hit){
				createship(Player.x,Player.y,0,Player.angle+directionadder,Player.ally);
			}
			directionadder=0;
			switch (Enemy.turntimer&0x4){
				case 0x00:
					directionadder=0x80;
					break;
			}
			
			
			if (!Enemy.hit) {
				createship(Enemy.x,Enemy.y,8,Enemy.angle+directionadder,Enemy.ally);
			}
			else{
				
				if (exsplosion<0x40){ //exsplosion will incriment each frame
					createship(Enemy.x,Enemy.y,8,(8+(exsplosion>>4)),true);//this can be changed to false once we fix the problem with the pallete generation, also exsplotion might need bitshifitn 7 times not 8
					
				}
				else{
					if (exsplosion>=0x80){
						Enemy.hit=false;
						Enemy.x=0;
						Enemy.y=0;
						Enemy.bulletactive=0;
						Enemy.speed=100;
						exsplosion=0; //resets the enemy and puts them onto the field we should add an animation here later on.
					}
				}
				++exsplosion;
				
			}
			/*if (turntimer&0x01){
				createbullet(x,y,255,0,true); //this creates a jet stream
			}
			 */
            
            //replace this with a struct pointer to ship struct for player then for enemy??
            for(u8 i=0; i<shipcount; ++i){
                struct Ship *ptr=shipptr[i];
                if (!(ptr->button & 1<<1) && (ptr->turntimer&0x03)==0 && openfire && ptr->bulletcooldown==0){//1=b
                    //creatr bullet
                    ptr->bullet[ptr->bulletcount&0x03]->x=ptr->x;
                    ptr->bullet[ptr->bulletcount&0x03]->y=ptr->y;
                    ptr->bullet[ptr->bulletcount&0x03]->angle=ptr->angle;
                    ptr->bullet[ptr->bulletcount&0x03]->BulletTimer=0x28;
                    ptr->bullet[ptr->bulletcount&0x03]->active=true;
                    ptr->bullet[ptr->bulletcount&0x03]->BulletSpeed=0x301;
                    ++ptr->bulletcount;
                    
                    ptr->bulletactive=ptr->bulletactive | 0x80;
                    ptr->bulletcooldown=0x10;
                }
                else if(ptr->bulletcooldown!=0){
                    --ptr->bulletcooldown;
                }
                
                /*else {
                    if ((Player.bulletactive&0x80)!=0){
                    createbullet(ptr->bullet[ptr->bulletcount&0x03]->x,ptr->bullet[ptr->bulletcount&0x03]->y,1,ptr->bullet[ptr->bulletcount&0x03]->angle,ptr->ally);
                    }
                 */
                for (u8 n=0; n<4;++n){
                    if (ptr->bullet[n]->active){
                        if (ptr->bullet[n]->BulletTimer!=0){
                            xyout=anglespeed(ptr->bullet[n]->angle, ptr->bullet[n]->BulletSpeed, ptr->bullet[n]->x, ptr->bullet[n]->y);
                            ptr->bullet[n]->x=(xyout>>16);
                            ptr->bullet[n]->y=(xyout&0xffff);
                            --ptr->bullet[n]->BulletTimer;
                            
                            createbullet(ptr->bullet[n]->x,ptr->bullet[n]->y,(i*8)+(n)+1,ptr->bullet[n]->angle,ptr->ally);
                        }
                        else{
                            ptr->bullet[n]->y=250<<8;
                            ptr->bullet[n]->active=false;
                        }
                    }
                }
                
            }
            
            
		
            
            
            
			displayscore();
			
			
			
		}

		
		
		deathanimation(Player.hit,Enemy.hit, Player.x,Player.y, Player.angle,Enemy.x,Enemy.y,Enemy.angle);
		gameoverscreen();
		
	}
	
	
	
	
	
	
	
	
	
	
	
    return 0;
}





    




void startscreen(void){
	bool startpressed=false;
	vu16 button=0xff;
	u8 timer=0;
	bool pressstart=false;
	char message[11]="PRESS START";

	
	while (startpressed==false){
		vsync();
		fillbuffer();
		switchoutdma();
		OAM_CLEAR();
		scrollbackground();
		if ((timer&0x1f)==0){
			pressstart=(!pressstart); //this flickers the press start text
		}
		if (pressstart==true){
			displaytext(message, 11, 0, 130,130,01);
		}
		qran_range(0,0xffff);
		button=*Button_Reg & 0x03ff;
		if (button==((((0xffff)^(1<<start))) &0x03ff )){
			startpressed=true;
			return;
		}
		++timer;
		
		
	}
	
}




