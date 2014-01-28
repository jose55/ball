#include <allegro.h>
#include <math.h>



BITMAP *bufferLand;
BITMAP *finalBuffer;
int  moving=0;

char soil[]={'h','u','h','d','h','h','h','h','u','h','h','h','u','h','h','h','d','d','d','h','h','h','u','h','d','h','h','h','h','u','h','h','h','u','h','h','h','d','d','d','h','h','h','u','h','u','h','h','h','h','u','h','h','h','u','h','h','h','d','h','u','h','d','h','h','h','u','h','d','h','h','h','h','u','h','h','h','u','h','h','h','d','u','h','d','h','h','f'};

namespace prota
{
  int x,y;
};

class Soil
{
  int x;
  int y;
 
  void  draw_horiz(int&,int&);
  void draw_up(int&,int&);
  void draw_down(int&,int&);
  void check(int&,int&);
  
public:
  Soil(){x=100;y=400;}
  void draw_soil();
};


class Thing
{
protected:

  int x,y;
  int find_y(int);//in function of x..  down to x

};


class Control:public Thing
{
protected:
  bool isDead;
  float angle;
  float  littleBall[2];//here we store the two coordenates
  int jumpState;
  int check();
  bool fallingDown;
  float* trigo(float []);
  void drawBall();
  
public:
  virtual void jump()=0;//virtual because first we want to do a basic jump 
  virtual void Move()=0;
  virtual void dead()=0;

};
class Control1:public Control
{
  void jump();

  void dead();
  void Move();//basic moves in this class
public:
  Control1(int);
  
};
class Control2:public Control
{
  void jump(){};
  void dead(){};
  
  void Move(){};
public:
  Control2(int){};

};

Control1::Control1(int x)
{
  this->x=x;
  this->y=358;//it means it doesn't have value yet
  jumpState=0;
  fallingDown=false;
  littleBall[0]=0;//therefor [1]=1  trigonometry
  angle=0.0f;
  isDead=false;
}

class IA:public Thing
{
  int range[2];
  int direction;

public:
 virtual void Move();
  IA();
  void values(int);//int = number of enemy
  bool collision(int,int);
};

class IA2:public IA
{
  void Move();
};
IA::IA()
{
  direction=0;
  y=0;
}
void IA::values(int n)
{
  int index=0;
  int pixels=0;
  int count=0;
  
  do{
    if(soil[index++]!='h' && soil[index]=='h')count++;
    pixels+=50;    
  }
  while(count<n);

  this->x=100+pixels;//we have just worked out the x-coordenate
  count=0;
  while(soil[index++]=='h')count++;
  range[0]=x;
  range[1]=x+count*50;

}



int main()
{

  allegro_init();
  install_keyboard();
  set_color_depth(16);
  set_gfx_mode(GFX_AUTODETECT_WINDOWED,1200,600,0,0);
  
  finalBuffer=create_bitmap(1200,600);
  bufferLand=create_bitmap(1200,600);

    while(key[KEY_ESC]==0)
      {
	Soil soil;
	Control1 me(300);
	Control2 m2(300);
	Control *me_base;
	me_base=&me;
	//me_base=&me2;//polimorphism
	
	IA enemy[20];

	for(int x=0;x<20;x++)
      	enemy[x].values(x+1);

	bool game=true;

	while(key[KEY_ESC]==0 && game==true)
	  {
	    clear_bitmap(bufferLand);
	    clear_bitmap(finalBuffer);
	    
	    
	    soil.draw_soil();
	    
	    me_base->Move();
	    
	    
	    
	    for(int x=0;x<20;x++)      
	      {
		enemy[x].Move();
		if(enemy[x].collision(prota::x,prota::y))
		  {
		    me_base->dead();
		  }
	      }      
	    if(prota::y<-99)game=false;
	    
	    draw_sprite(finalBuffer,bufferLand,0,0);
	    
	    draw_sprite(screen,finalBuffer,0,0);
	  }//end while esc
      }

  allegro_exit();

  return 0;
}


void Soil::draw_soil()
{
  int x=this->x;
  int y=this->y;
  int index=0;
  while(soil[index]!='f')
    {
      //(x>1000)? break:x=2;
      if(x>1200)break;
      switch(soil[index++])
	{
	case 'h':
	  draw_horiz(x,y);
	  break;
	case 'u':
	  draw_up(x,y);
	  break;
	case 'd':
	  draw_down(x,y);
	  
	  break;

	  
	}//end switch
    }//end while
  //      rectfill(bufferLand,x,y,x+10,y+10,makecol(255,0,0));
  check(this->x,this->y);
  //  if(!moving)  this->x-=5;
  //  this->y++;
}

void Soil::draw_horiz(int &x,int &y)
{
  for(int t=0;t<5;t++)
    {
      if(x>0)     rectfill(bufferLand,x,y,x+10,y+10,makecol(255,0,0));
     x+=10;
    }
}

void Soil::draw_up(int &x,int &y)
{
  for(int t=0;t<5;t++)
    {
      if(x>0)rectfill(bufferLand,x,y,x+10,y+10,makecol(255,0,0));
      x+=10;
      y-=10;
    }
}
void Soil::draw_down(int &x,int &y)
{
  for(int t=0;t<5;t++)
    {
      if(x>0)rectfill(bufferLand,x,y,x+10,y+10,makecol(255,0,0));
      x+=10;
      y+=10;
    }
}

int Thing::find_y(int x)
{
  //Here we'll work out the y-coordenate when the
  //object is next to the screen

  return 360;
}

void Control1::Move()
{
  //puts values en namespace
  prota::x=x;
  prota::y=y;

  if(!isDead)
    {
      if(key[KEY_RIGHT] && !jumpState)
	{
	  switch (check()){
	  case 0:
	    moving=0;
	    
	    break;
	    
	  case 1:
	    //	moving=1 going up
	    y-=5;
	    moving=0;
	    
	    break;
	    
	  case 3:
	    y+=3;
	    moving=10;
	    
	    break;
	    
	  }
	}
      if(!key[KEY_RIGHT] && !jumpState)moving=10;
      
      if(check()==3)
	{
	  moving=10;
	  y+=3;
	} 
      if(check()==2 && !jumpState)
	{
	  fallingDown=true;
	  moving=0;
	  y+=5;

	}
      else fallingDown=false;

      if((key[KEY_UP] || jumpState) && !fallingDown)jump();
    }
  else{

    moving=0;
    y-=10;
    if(y<-100)isDead=false;
  }
  
  drawBall();

}


int Control::check()
{
  //returns value:
  //0 ...  nothing ahead
  //1 ... ramp going up
  //2 ... ramp going down
  //3 ... nothing down
  int r;
  if(getpixel(bufferLand,x+27,y+39)==makecol(255,0,0) || getpixel(bufferLand,x+27,y+35)==makecol(255,0,0))r=1;
    else  if(getpixel(bufferLand,x-10,y+42)==0 && getpixel(bufferLand,x+30,y+42)==0)r=2;


  else r=0;




  return r;
}

void Soil::check(int &x,int &y)
{
  //if(!moving)  this->x-=5;

  switch (moving)
    {
    case 0:
      x-=5;
      break;
    case 1:

      break;

    }
}

void Control1::jump()
{
  if (!jumpState)jumpState=10;
  if(jumpState>4)y-=5;
  else jumpState=1;
  moving=0;
  jumpState--;

}

void Control::drawBall()
{
  circlefill(bufferLand,x,y,40,makecol(0,0,255));

  float* array=trigo(littleBall);
  circlefill(bufferLand,*array,*(array+1),10,makecol(0,255,0));

}

float* Control::trigo(float coord[])
{
  //works out absolute coordenate

  coord[0]=x+(cos(angle)*30);
  coord[1]=y+(sin(angle)*30);
  if(!moving)angle+=0.08;
  if(angle>400)angle=0;
  return coord;
}

void IA::Move()
{

      if(x>-100 && x<1199)
	{
	  if(!y)
	    {
	      int y=0,pixel;
	      do{
		pixel=getpixel(bufferLand,x+1,y++);
	      }
	      while(!pixel);
	      this->y=y-10;
	    }
	  
	  rectfill(bufferLand,x,this->y,x+10,(this->y)+10,makecol(0,255,0));
	}
      else y=0;
      if(!moving)
	{
	  x-=5;
	  range[0]-=5;
	  range[1]-=5;
	}
      if(!direction)
	{
	  x++;
	  if(x>range[1])direction=1;
	}
      else
	{
	  x--;
	  if(x<range[0])direction=0;
	  
	}

  
}

void IA2::Move()
{
}

bool IA::collision(int px,int py)//protagonist x  = px
{
  
  return (x>px-10 && x<px+10 && y>py && y<py+40)? true:false;
  
}
void Control1::dead()
{
  isDead=true;

}

