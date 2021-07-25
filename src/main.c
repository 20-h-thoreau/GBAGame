#include "defs.h"
 u16 const  down = 0x07;
 u16 const  up = 0x06;
 u16 const  left = 0x05;
 u16 const  right = 0x04;
 u16 const  b = 0x01;
 u16 const  start = 0x03;


 u8 const ScreenWidth=240;
 u8 const ScreenHieght=160;//these were orginally u8 constances


#include <tonc_oam.h>
#include <tonc_core.h>
#include <tonc_types.h>
#include <tonc_bios.h>
#include <tonc_math.h>


#include "enemy.h"
#include "graphics.h"
#include "gameoverscreen.h"
#include "spritepositionfunctions.h"

#include "sprite_bin.h"
#include "background.h"
#include "bullet_bin.h"
#include "playerpallete_bin.h"
#include "enemypallete_bin.h"
#include "font_bin.h"
#include "exsplosion_bin.h"

#include "background.h"




u8 level;//this makes the enemy more likely to shoot



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


int main(){
	u16 bulletwaittimer=0;
	bool openfire=false;
	u8 level=startinglevel;
    vsync();
	OAM_CLEAR();
    DisplayController=0x1001;
    tonccpy(&Mem_Tile[4][1],sprite_bin,sprite_bin_size);
    tonccpy(Mem_Obj_Palette,playerpallete_bin,playerpallete_bin_size);
	tonccpy(Mem_Obj_Palette+16,enemypallete_bin,enemypallete_bin_size-16);
	tonccpy(BGPaletteMem,bgPal,bgPalLen);
    //tonccpy(Mem_Obj_Palette[16],enemyPal,enemyPalLen);
    
    tonccpy(&Mem_Tile[4][4],bullet_bin,bullet_bin_size);
	tonccpy(&Mem_Tile[4][5],exsplosion_bin,exsplosion_bin_size);
    
	tonccpy (&Mem_Tile[4][0x20],font_bin,font_bin_size);
    struct ObjectAttribute SpriteAttribute={0b0010000000000000, 0b0000000000000000, 0b0000000010000000};
    tonccpy(Obj_Attributes,&SpriteAttribute,12);


	
	struct Ship Player;

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
    
	struct Ship Enemy;
    
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
    
	struct BulletAttribute Bullet;

	Bullet.ally=true;
	Bullet.x=Player.x;
	Bullet.y=250<<8;
	Bullet.BulletTimer=0;
	Bullet.BulletSpeed=0x301;
	Bullet.angle=0;
	
	struct BulletAttribute Bullet1;

	Bullet1.ally=false;
	Bullet1.x=Enemy.x;
	Bullet1.y=250<<8;
	Bullet1.BulletTimer=0;
	Bullet1.BulletSpeed=0x301;
	Bullet1.angle=0;

	
	startscreen();
    
    
    
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
        
			
		
		
		
		if ((Player.bulletactive &0x80) !=0){
			xyout=anglespeed(Bullet.angle, Bullet.BulletSpeed, Bullet.x, Bullet.y);
			Bullet.x=(xyout>>16);
			Bullet.y=(xyout&0xffff);
			Bullet.BulletTimer=((Bullet.BulletTimer+1)&0x3f);
		}
		
		if (Bullet.BulletTimer==0){
			Bullet.y=250<<8;
			Player.bulletactive=Player.bulletactive&0x7f;
			
		}
		
		
		
   
        
        if (Enemy.turntimer==0x00){
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
        
        
		



			
		
        if ((Enemy.bulletactive &0x80) !=0){
            xyout=anglespeed(Bullet1.angle, Bullet1.BulletSpeed, Bullet1.x, Bullet1.y);
            Bullet1.x=(xyout>>16);
            Bullet1.y=(xyout&0xffff);
            Bullet1.BulletTimer=((Bullet.BulletTimer+1)&0x3f);
        }
        
        if (Bullet1.BulletTimer==0){
            Bullet1.y=250<<8;//not sure this is nessisary
			Enemy.bulletactive=Enemy.bulletactive&0x7f;
            
        }
            
        
		
		
		
		if ( (((ABS(Bullet.x-Enemy.x))<0x600  && (ABS(Bullet.y-Enemy.y))<0x600  )) && Player.bulletactive!=0){
			Enemy.hit=true;
			gameover=true;
			if ( (ABS(Player.x-Enemy.x))<0x800  && (ABS(Player.y-Enemy.y))<0x800  ){
				Player.hit=true;
			}
		}
		
      
		if ( (ABS(Player.x-Enemy.x))<0x400  && (ABS(Player.y-Enemy.y))<0x400  ){
			gameover=true; //this ends the game in the event of a collision
			Enemy.hit=true;
			Player.hit=true;
			}
		
		

        
        if ( (((ABS(Bullet1.x-Player.x))<0x300  && (ABS(Bullet1.y-Player.y))<0x300  )) && Enemy.bulletactive!=0){
            Player.hit=true;
            gameover=true;
        }
        
        
        

		
        vsync();
		OAM_CLEAR();

		
		
		if (!Player.hit){
			createship(Player.x,Player.y,0,Player.angle,Player.ally);
		}
		
		if (!Enemy.hit) {
			createship(Enemy.x,Enemy.y,32,Enemy.angle,Enemy.ally);
		}
        /*if (turntimer&0x01){
            createbullet(x,y,255,0,true); //this creates a jet stream
        }
         */
        if (!(Player.button & 1<<1) && (Player.turntimer&0x03)==0 && openfire){//1=b
			Bullet.x=Player.x;
			Bullet.y=Player.y;
			Bullet.angle=Player.angle;
			Bullet.BulletTimer=0;
			
            createbullet(Player.x,Player.y,1,Player.angle,Player.ally);
            Player.bulletactive=Player.bulletactive | 0x80;
            }
        
        else {
            if ((Player.bulletactive&0x80)!=0){
            createbullet(Bullet.x,Bullet.y,1,Bullet.angle,true);
            }
        }
        
        if ((Enemy.button & 1<<1) && (Enemy.turntimer&0x03)==0 && openfire) {
			Bullet1.x=Enemy.x;
			Bullet1.y=Enemy.y;
			Bullet1.angle=Enemy.angle;
			Bullet1.BulletTimer=0;
			
            createbullet(Enemy.x,Enemy.y,33,Enemy.angle,Enemy.ally);
            Enemy.bulletactive=Enemy.bulletactive | 0x80;
        }
        else{
            if ((Enemy.bulletactive&0x80)!=0){
            createbullet(Bullet1.x,Bullet1.y,33,Bullet1.angle,false);//if we used a pointer that would be helpfull here my dude instead of having to seperate fucntions
            }
        }
        
    }

	
	
	deathanimation(Player.hit,Enemy.hit, Player.x,Player.y, Player.angle,Enemy.x,Enemy.y,Enemy.angle);
    gameoverscreen();
    SoftReset();
	
	
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
		OAM_CLEAR();
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




