#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#define TIMER_ID 1
#define TIMER_INTERVAL 20
#define PI 3.1415926535

static float window_width, window_height;
static float camera_angle=PI;
static int camera_position=2;
static int movement_direction=0;
static float projectile_times[]={0,0,0}; //Proteklo vreme za sva 3 projektila
static int projectile_active[]={0,0,0};//Aktivnost sva 3 projektila
static int projectile_position[]={0,0,0};//Tunel u kojem je ispaljen projektil
static const float projectile_limits[]={-1,1,0,2,-1,31, -1,31,0,2,-1,1, -1,1,0,2,-31,1, -31,1,0,2,-1,1}; //Limiti projektila za sva 4 tunela
static float projectile_angles[]={0,0,0,0,0,0}; //x i y uglovi za sva 3 projektila
static float projectile_coordinates[]={0,0,0, 0,0,0, 0,0,0}; //x, y, z kooridante projektila
static int projectile_number=0;
static float enemy_times[]={0,0,0,0,0,0,0,0,0,0}; 
static int enemy_active[]={0,0,0,0,0,0,0,0,0,0};
static int enemy_position[]={0,0,0,0,0,0,0,0,0,0};
static float enemy_coordinates[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

static void wave1();
static int wave_number=1;
static int wave_time=0;
static int health=3;
static int cooldown=0;
static int cooldown_color=1;
static int special_position=0;
static float special_time=0;
static int special_active=0;
static void destroy_projectile(int number);
static void enemy_timer(int id);
static int game_over_time=0;
static int game_over_screen=0;

/* Nivoi */
void spawn_enemy_help(float x, float y, int position, int time)
{
    int number;
    if(wave_time==time)
    {
        for(int i=0;i<10;i++)
        {
            if(enemy_active[i]==0)
            {
                number=i;
                break;
            }
        }
    enemy_active[number]=1;
    enemy_position[number]=position;
    enemy_coordinates[number*2]=x;
    enemy_coordinates[number*2+1]=y;
    glutTimerFunc(TIMER_INTERVAL, enemy_timer, number);
    }
}
static void wave1()
{
    spawn_enemy_help(0.3,1.3,2,15);
    spawn_enemy_help(0.5,0.8,2,30);
    spawn_enemy_help(0.2,1.2,1,50);
    spawn_enemy_help(-0.5,1,1,80);
    spawn_enemy_help(0.4,1.5,1,80);
    spawn_enemy_help(0.6,1.7,3,110);
    spawn_enemy_help(-0.2,0.3,0,130);
    spawn_enemy_help(-0.5,0.5,0,150);
    spawn_enemy_help(0,1.4,1,180);
    spawn_enemy_help(-0.7,0.3,1,185);
}
static void wave2()
{
    spawn_enemy_help(0.3,0.8,2,10);
    spawn_enemy_help(-0.2,1.3,2,20);
    spawn_enemy_help(0,1.2,1,30);
    spawn_enemy_help(-0.6,0.8,1,40);
    spawn_enemy_help(0.6,0.8,1,40);
    spawn_enemy_help(-0.6,1.5,3,70);
    spawn_enemy_help(0.6,1.6,3,75);
    spawn_enemy_help(-0.5,1.3,2,89);
    spawn_enemy_help(0.3,0.8,3,105);
    spawn_enemy_help(0.5,0.6,0,130);
    spawn_enemy_help(-0.5,0.4,0,150);
    spawn_enemy_help(0.5,1.6,0,170);
    spawn_enemy_help(-0.2,0.8,0,170);
}
static void wave3()
{
    spawn_enemy_help(-0.5,0.8,0,15);
    spawn_enemy_help(0,1.5,0,15);
    spawn_enemy_help(-0.5,1,2,53);
    spawn_enemy_help(0.1,0.4,2,53);
    spawn_enemy_help(0.7,1,2,53);
    spawn_enemy_help(0.6,0.3,2,70);
    spawn_enemy_help(-0.2,1.2,2,70);
    spawn_enemy_help(0,1.2,3,100);
    spawn_enemy_help(-0.2,1.2,0,115);
    spawn_enemy_help(0.6,1,0,115);
    spawn_enemy_help(-0.4,1,1,140);
    spawn_enemy_help(0,0.8,1,150);
    spawn_enemy_help(0.3,1,1,155);
    spawn_enemy_help(0,1.2,1,160);
    spawn_enemy_help(-0.3,1,1,165);
}
static void wave4()
{
    spawn_enemy_help(-0.5,0.8,0,15);
    spawn_enemy_help(0,1.5,0,15);
    spawn_enemy_help(0.5,0.8,0,15);
    spawn_enemy_help(-0.5,1,1,45);
    spawn_enemy_help(0.1,0.4,1,45);
    spawn_enemy_help(0.7,1,1,45);
    spawn_enemy_help(-0.3,1.5,1,48);
    spawn_enemy_help(0.6,0.3,2,70);
    spawn_enemy_help(-0.2,1.2,2,70);
    spawn_enemy_help(0.2,1.6,2,80);
    spawn_enemy_help(0,1.2,3,90);
    spawn_enemy_help(0.5,1.2,3,100);
    spawn_enemy_help(-0.2,1.6,3,115);
    spawn_enemy_help(0.6,0.5,3,115);
    spawn_enemy_help(-0.4,1.2,1,140);
    spawn_enemy_help(0,0.4,1,150);
    spawn_enemy_help(0.6,1,1,155);
    spawn_enemy_help(0,1.4,1,160);
    spawn_enemy_help(-0.7,1,1,165);
}
static void wave5()
{
    spawn_enemy_help(-0.6,1.5,3,20);
    spawn_enemy_help(0.1,0.4,3,20);
    spawn_enemy_help(0.7,1.5,3,20);
    spawn_enemy_help(-0.3,1.6,2,35);
    spawn_enemy_help(-0.3,0.3,2,35);
    spawn_enemy_help(0.2,1,3,50);
    spawn_enemy_help(-0.7,1.6,3,50);
    spawn_enemy_help(0,1,0,70);
    spawn_enemy_help(0.1,1.6,1,90);
    spawn_enemy_help(-0.7,0.3,1,90);
    spawn_enemy_help(0.7,1.6,1,90);
    spawn_enemy_help(0.3,1.7,0,110);
    spawn_enemy_help(-0.6,1.2,0,115);
    spawn_enemy_help(0.1,1,0,120);
    spawn_enemy_help(0.7,1,0,130);
    spawn_enemy_help(0.1,1,3,140);
    spawn_enemy_help(0.4,1.6,0,150);
    spawn_enemy_help(-0.7,1.4,1,160);
    spawn_enemy_help(0.5,1.2,1,160);
    spawn_enemy_help(-0.5,1,1,165);
    spawn_enemy_help(0.3,0.8,1,170);
    spawn_enemy_help(-0.5,0.6,1,170);
    spawn_enemy_help(0,0.4,1,175);
    spawn_enemy_help(0.6,0.6,1,180);
    spawn_enemy_help(-0.6,1,1,190);
}


/* Tajmeri */
static void game_over_timer(int id)
{
    if(game_over_time<30)
    {
        game_over_time++;
        glutPostRedisplay();
        glutTimerFunc(50,game_over_timer,TIMER_ID);
    }
    else
    {
        game_over_screen=0;
        game_over_time=0;
        glutPostRedisplay();
    }
}
static void wave_timer(int id)
{
    wave_time+=1;
    switch(wave_number)
    {
        case 1:
            wave1();
            break;
        case 2:
            wave2();
            break;
        case 3:
            wave3();
            break;
        case 4:
            wave4();
            break;
        case 5:
            wave5();
            break;
    }
    if(wave_time==200)
    {
        wave_number++;
        wave_time=0;
    }
    if(health<=0)
    {
        game_over_screen=1;
    glutTimerFunc(50,game_over_timer,TIMER_ID);
     wave_number=1;
     wave_time=0;
     for(int i=0;i<10;i++)
     {
         enemy_active[i]=0;
         enemy_position[i]=0;
         
     }
     cooldown=0;
     cooldown_color=1;
     health=3;
     return;
    }
    if(wave_number==6)
        return;
    glutPostRedisplay();
    glutTimerFunc(100, wave_timer, TIMER_ID);
}
static void enemy_timer(int id)
{
    enemy_times[id]+=0.2;
    if(enemy_active[id]==1)
    {
        glutTimerFunc(TIMER_INTERVAL, enemy_timer, id);
        glutPostRedisplay();
    }
}
static void projectile_timer(int id)
{
    if(id<0 || id>2)
        return;
    /* Uvecava se vreme projektila za 1, ako je i dalje aktivan, poziva se ponovo */
    projectile_times[id]+=0.5;
    if(projectile_times[id]>30)
    {
        destroy_projectile(id);
    }
    
    glutPostRedisplay();
    if(projectile_active[id])
        glutTimerFunc(TIMER_INTERVAL, projectile_timer, id);
}
static void movement_timer(int id)
{
    /* Proverava se u kom smeru je kretanje kamere i pozicija se resetuje da bi vrednost uvek bila manja od 2*PI i veca od 0 */
    if(id != TIMER_ID)
        return;
    if(movement_direction>0)
    {
        if(camera_position*PI/2>camera_angle)
            camera_angle+=PI/16;
        else
        {
            if(camera_position==4)
                camera_position=0;
            camera_angle=camera_position*PI/2;
            movement_direction=0;
        }
    }
    if(movement_direction<0)
    {
        if(camera_position*PI/2<camera_angle)
            camera_angle-=PI/16;
        else
        {
            if(camera_position==-1)
                camera_position=3;
            camera_angle=camera_position*PI/2;
            movement_direction=0;
        }
    }
    glutPostRedisplay();
    /* Timer se pokrece ponovo ako kretanje nije zavrseno */
    if(movement_direction!=0)
        glutTimerFunc(TIMER_INTERVAL, movement_timer, TIMER_ID);
}
static void special_cooldown_timer(int id)
{
    if(cooldown>0)
    {
        cooldown-=1;
        glutPostRedisplay();
        glutTimerFunc(1000, special_cooldown_timer, TIMER_ID);
    }
    else
    {
        cooldown_color=!(cooldown_color);
        glutPostRedisplay();
        glutTimerFunc(1000, special_cooldown_timer, TIMER_ID);
    }
}
static void special_timer(int id)
{
    special_time+=0.5;
    if(special_active)
    {
        glutTimerFunc(TIMER_INTERVAL, special_timer, id);
        glutPostRedisplay();
    }
}

/* Crtanje objekata */
static void destroy_projectile(int number)
{
    projectile_active[number]=0;
    projectile_times[number]=0; 
    projectile_coordinates[number*3]=0;
    projectile_coordinates[number*3+1]=0;
    projectile_coordinates[number*3+2]=0; 
    glutPostRedisplay();
}
static void draw_projectile(int number)
{
    
    /* Podesavanje materijala */
    GLfloat ambient_coeffs[] = { 0.3, 0.3, 0, 1 };
    GLfloat diffuse_coeffs[] = { 0.8, 0.8, 0, 1 };
    GLfloat specular_coeffs[] = { 1, 1, 1, 1 };
    GLfloat shininess = 40;
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    
    /* Vrsi se transformacija za projektil sa odrednjenim brojem po njegovim parametrima */
    glPushMatrix();
    float projectile_x=projectile_times[number]*sin(projectile_angles[2*number]);
    float projectile_y=1-projectile_times[number]*sin(projectile_angles[2*number+1]);
    float projectile_z=projectile_times[number]*cos(projectile_angles[2*number]);
    
    /* Ako projektil stigne do nekog limita unistava se */
    if(projectile_x-0.1<projectile_limits[projectile_position[number]*6])
    {
        destroy_projectile(number);
    }
    
    if(projectile_x+0.1>projectile_limits[projectile_position[number]*6+1])
    {
        destroy_projectile(number);
    }
    if(projectile_y-0.1<projectile_limits[projectile_position[number]*6+2])
    {
        destroy_projectile(number);
    }
    if(projectile_y+0.1>projectile_limits[projectile_position[number]*6+3])
    {
        destroy_projectile(number);
    }
    if(projectile_z-0.1<projectile_limits[projectile_position[number]*6+4])
    {
        destroy_projectile(number);
    }
    if(projectile_z+0.1>projectile_limits[projectile_position[number]*6+5])
    {
        destroy_projectile(number);
    }
    
    glTranslatef(projectile_x,projectile_y,projectile_z);
    glutSolidSphere(0.1,10,10);
    
    /* Pamte se koordinate projektila zbog kolozije sa neprijateljima */
    projectile_coordinates[number*3]=projectile_x;
    projectile_coordinates[number*3+1]=projectile_y;
    projectile_coordinates[number*3+2]=projectile_z;
    glPopMatrix();
}
static void draw_enemy(int number, float local_x, float local_y)
{
    /* Podesavanje materijala */
    GLfloat ambient_coeffs[] = { 0.1, 0.1, 0.1, 1 };
    GLfloat diffuse_coeffs[] = { 0.4, 0.4, 0.4, 1 };
    GLfloat specular_coeffs[] = { 0.1, 0.1, 0.1, 1 };
    GLfloat shininess = 10;
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    
    /* Izracunava se pozicija projektila u zavisnosti od pozicije u kojoj se nalazi */
    float enemy_x, enemy_y, enemy_z;
    switch(enemy_position[number])
    {
        case 0:
            enemy_x=local_x;
            enemy_y=local_y;
            enemy_z=31-enemy_times[number];
            break;
        case 1:
            enemy_x=31-enemy_times[number];
            enemy_y=local_y;
            enemy_z=local_x;;
            break;
        case 2:
            enemy_x=local_x;
            enemy_y=local_y;
            enemy_z=-31+enemy_times[number];
            break;
        case 3:
            enemy_x=-31+enemy_times[number];
            enemy_y=local_y;
            enemy_z=local_x;;
            break;
    }
    
    /* Provera se da li je doslo do kolizije samo ako se projektil nalazi u istom tunelu */
    for(int i=0;i<3;i++)
    {
        if(projectile_position[i]==enemy_position[number])
        {
            if((projectile_coordinates[3*i]>(enemy_x-0.3)) &&
                (projectile_coordinates[3*i]<(enemy_x+0.3)) &&
                (projectile_coordinates[3*i+1]>(enemy_y-0.3)) &&
                (projectile_coordinates[3*i+1]<(enemy_y+0.3)) &&
                (projectile_coordinates[3*i+2]>(enemy_z-0.3)) &&
                (projectile_coordinates[3*i+2]<(enemy_z+0.3)))
            {
                enemy_active[number]=0;
                enemy_times[number]=0;
                destroy_projectile(i);
            }
        }
    }
    
    /* Provera se da li je doslo do kolizije sa specijalnom sposobnoscu */
    if(special_active)
        if(special_position==enemy_position[number])
        {
            switch(special_position)
            {
                case 0:
                    if(special_time>=enemy_z)
                    {
                        enemy_active[number]=0;
                        enemy_times[number]=0;
                    }
                    break;
                case 1:
                    if(special_time>=enemy_x)
                    {
                        enemy_active[number]=0;
                        enemy_times[number]=0;
                    }
                    break;
                case 2:
                    if(-special_time<=enemy_z)
                    {
                        enemy_active[number]=0;
                        enemy_times[number]=0;
                    }
                    break;
                case 3:
                    if(-special_time<=enemy_x)
                    {
                        enemy_active[number]=0;
                        enemy_times[number]=0;
                    }
                    break;

            }
            
        }
    
    /* Provera se da li je neprijatelj udario u igraca */
        switch(enemy_position[number])
        {
            case 0:
                if(enemy_z<1)
                {
                    health-=1;
                    enemy_active[number]=0;
                    enemy_times[number]=0;
                    glutPostRedisplay();
                }
                break;
            case 1:
                if(enemy_x<1)
                {
                    health-=1;
                    enemy_active[number]=0;
                    enemy_times[number]=0;
                    glutPostRedisplay();
                }
                break;
            case 2:
                if(enemy_z>-1)
                {
                    health-=1;
                    enemy_active[number]=0;
                    enemy_times[number]=0;
                    glutPostRedisplay();
                }
                break;
            case 3:
                if(enemy_x>-1)
                {
                    health-=1;
                    enemy_active[number]=0;
                    enemy_times[number]=0;
                    glutPostRedisplay();
                }
                break;
        }
    glPushMatrix();
    glTranslatef(enemy_x,enemy_y,enemy_z);
    glutSolidCube(0.5);
    glPopMatrix();
}
void draw_hud()
{
    /* Prelazi se na ortogonalnu projekciju i crta se mapa */
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,window_width,0,window_height,-1,1);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
   /* Ako je kraj igre pojavljuje se crni ekran */
        if(game_over_screen==1)
     {
     glColor4f(0,0,0,1);
     glBegin(GL_POLYGON);
     glVertex3f(0,0,0);
     glVertex3f(0,window_height,0);
     glVertex3f(window_width, window_height, 0);
     glVertex3f(window_width,0,0);
     glEnd();
     }
    /* Izracunava se centar i poluprecnik kruga */
    float map_radius=window_height/10;
    float map_center=window_height/9;
    
    /* Pravi se ceo krug kao pozadina mape */
    glBegin(GL_POLYGON);
    glColor4f(0,0,0.5,1);
    
    for(float i=0;i<=2*PI;i+=PI/30)
    {
        glVertex3f(map_center+map_radius*sin(PI-i),map_center+map_radius*cos(PI-i),0);
    }
    glEnd();

    /* Pravi se deo kruga koji prikazuje gde je kamera usmerena */
    glBegin(GL_POLYGON);
    glColor4f(0,0,1,1);
    
    glVertex3f(map_center,map_center,0);
    for(float i=camera_angle-PI/6;i<=camera_angle+PI/6;i+=PI/30)
    {
        glVertex3f(map_center+map_radius*sin(PI-i),map_center+map_radius*cos(PI-i),0);
    }
    glEnd();
    
    /* Crtaju se slobodni projektili */
    int number_of_free_projectiles=0;
    for(int i=0;i<3;i++)
    {
        if(projectile_active[i]==0)
            number_of_free_projectiles+=1;
    }
    
    glLineWidth(2);
    glColor4f(0,0,0,1);
    float projectile_length=map_radius/3;
    for(int i=1;i<=number_of_free_projectiles;i++)
    {
        glBegin(GL_LINES);
        glVertex3f(map_center+map_radius+(i*projectile_length/2),projectile_length,0);
        glVertex3f(map_center+map_radius+(i*projectile_length/2),2*projectile_length,0);
        glEnd();
    }

    /* Crtaju se neprijatelji na mapi */
    float enemy_x;
    float enemy_z;
    glColor4f(1,1,1,1);
    glPointSize(10);
    glBegin(GL_POINTS);
    for(int i=0;i<10;i++)
    {
        if(enemy_active[i])
        {
            /* Pronalaze se koordinate i skaliraju prema mapi */
            switch(enemy_position[i])
            {
                case 0:
                    enemy_x=enemy_coordinates[i*2];
                    enemy_z=-31+enemy_times[i];
                    break;
                case 1:
                    enemy_x=31-enemy_times[i];
                    enemy_z=enemy_coordinates[i*2];
                    break;
                case 2:
                    enemy_x=enemy_coordinates[i*2];
                    enemy_z=31-enemy_times[i];
                    break;
                case 3:
                    enemy_x=-31+enemy_times[i];
                    enemy_z=enemy_coordinates[i*2];
                    break;
            }
            enemy_x=enemy_x*map_radius/31;
            enemy_z=enemy_z*map_radius/31;
            glVertex3f(map_center+enemy_x,map_center+enemy_z,0);
        }
    }
    glEnd();
    
    /* Crta se progres u igri */
    float progress=((float)wave_time/200.+wave_number-1)/5.;
    progress*=window_width;
    glLineWidth(10);
    glColor4f(1-0.2*wave_number,1-0.2*wave_number,1-0.2*wave_number,1);
    glBegin(GL_LINES);
    glVertex3f(0,window_height-20,0);
    glVertex3f(progress,window_height-20,0);
    glEnd();
    
    /* Crtaju se zivoti */
    float health_radius=map_radius/5;
    float health_y=map_center+map_radius+health_radius*2;
    float health_x=map_center-map_radius+health_radius*2;
    for(int j=0;j<health;j++)
    {
        glColor4f(1,0,0,1);
        glBegin(GL_POLYGON);
        for(float i=0;i<=2*PI;i+=PI/30)
        {
            glVertex3f(health_x+(3*health_radius)*j+health_radius*sin(PI-i),health_y+health_radius*cos(PI-i),0);
        }
        glEnd();
    }
    
    /* Crta se da li je spremna specijalna sposobnost */
    glBegin(GL_POLYGON);
    if(!cooldown_color)
        glColor4f(1,1,1,1);
    else
        glColor4f(0,0.8,0,1);
    for(float i=0;i<=2*PI;i+=PI/30)
    {
        glVertex3f(2*map_center+projectile_length*sin(PI-i),2*map_center+projectile_length*cos(PI-i),0);
    }
    glEnd();
    float cooldown_percent=2*PI/20*cooldown;
    glBegin(GL_POLYGON);
    glVertex3f(2*map_center,2*map_center,0);
    glColor4f(0,0.2,0,1);
    for(float i=0;i<=cooldown_percent;i+=PI/30)
    {
        glVertex3f(2*map_center+projectile_length*sin(PI-i),2*map_center+projectile_length*cos(PI-i),0);
    }
    glEnd();
    
    /* Vracaju se prethodna podesavanja */
    glPopMatrix();
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    
}
void draw_tunnel()
{
    
    /* Podesavanje materijala */
    GLfloat ambient_coeffs[] = { 0.4, 0, 0, 1 };
    GLfloat diffuse_coeffs[] = { 0.9, 0, 0, 1 };
    GLfloat specular_coeffs[] = { 1, 1, 1, 1 };
    GLfloat shininess = 40;

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    
    /* Pravljenje tunela */
    for(int i=1;i<=30;i++)
    {
        /* Prednji tunel */
        glBegin(GL_POLYGON);
        glNormal3f(0,1,0);
        glVertex3f(-1, 0, -i);
        glVertex3f(1, 0, -i);
        glVertex3f(1, 0, -i-1);
        glVertex3f(-1, 0, -i-1);
        glEnd();
        
         glBegin(GL_POLYGON);
        glNormal3f(-1,0,0);
        glVertex3f(1, 0, -i);
        glVertex3f(1, 2, -i);
        glVertex3f(1, 2, -i-1);
        glVertex3f(1, 0, -i-1);
        glEnd();
        
        glBegin(GL_POLYGON);
        glNormal3f(1,0,0);
        glVertex3f(-1, 0, -i);
        glVertex3f(-1, 2, -i);
        glVertex3f(-1, 2, -i-1);
        glVertex3f(-1, 0, -i-1);
        glEnd();
        
        glBegin(GL_POLYGON);
        glNormal3f(0,-1,0);
        glVertex3f(-1, 2, -i);
        glVertex3f(1, 2, -i);
        glVertex3f(1, 2, -i-1);
        glVertex3f(-1, 2, -i-1);
        glEnd();
        
        glBegin(GL_POLYGON);
        glNormal3f(0, 0, -1);
        glVertex3f(-1, 0, -31);
        glVertex3f(1, 0, -31);
        glVertex3f(1, 2, -31);
        glVertex3f(-1, 2, -31);
        glEnd();
        
        /* Desni tunel */
        glBegin(GL_POLYGON);
        glNormal3f(0,1,0);
        glVertex3f(i, 0, -1);
        glVertex3f(i+1, 0, -1);
        glVertex3f(i+1, 0, 1);
        glVertex3f(i, 0, 1);
        glEnd();
        
         glBegin(GL_POLYGON);
        glNormal3f(0,-1,0);
        glVertex3f(i, 2, -1);
        glVertex3f(i+1, 2, -1);
        glVertex3f(i+1, 2, 1);
        glVertex3f(i, 2, 1);
        glEnd();
        
        glBegin(GL_POLYGON);
        glNormal3f(0,0,1);
        glVertex3f(i, 0, -1);
        glVertex3f(i+1, 0, -1);
        glVertex3f(i+1, 2, -1);
        glVertex3f(i, 2, -1);
        glEnd();
        
        glBegin(GL_POLYGON);
        glNormal3f(0,0,-1);
        glVertex3f(i, 0, 1);
        glVertex3f(i+1, 0, 1);
        glVertex3f(i+1, 2, 1);
        glVertex3f(i, 2, 1);
        glEnd();
        
        glBegin(GL_POLYGON);
        glNormal3f(1, 0, 0);
        glVertex3f(31, 0, -1);
        glVertex3f(31, 0, 1);
        glVertex3f(31, 2, 1);
        glVertex3f(31, 2, -1);
        glEnd();
        
        /* Zadnji tunel */
        glBegin(GL_POLYGON);
        glNormal3f(0,1,0);
        glVertex3f(-1, 0, i);
        glVertex3f(1, 0, i);
        glVertex3f(1, 0, i+1);
        glVertex3f(-1, 0, i+1);
        glEnd();
        
         glBegin(GL_POLYGON);
        glNormal3f(-1,0,0);
        glVertex3f(1, 0, i);
        glVertex3f(1, 2, i);
        glVertex3f(1, 2, i+1);
        glVertex3f(1, 0, i+1);
        glEnd();
        
        glBegin(GL_POLYGON);
        glNormal3f(1,0,0);
        glVertex3f(-1, 0, i);
        glVertex3f(-1, 2, i);
        glVertex3f(-1, 2, i+1);
        glVertex3f(-1, 0, i+1);
        glEnd();
        
        glBegin(GL_POLYGON);
        glNormal3f(0,-1,0);
        glVertex3f(-1, 2, i);
        glVertex3f(1, 2, i);
        glVertex3f(1, 2, i+1);
        glVertex3f(-1, 2, i+1);
        glEnd();
        
        glBegin(GL_POLYGON);
        glNormal3f(0, 0, 1);
        glVertex3f(-1, 0, 31);
        glVertex3f(1, 0, 31);
        glVertex3f(1, 2, 31);
        glVertex3f(-1, 2, 31);
        glEnd();
        
        /* Levi tunel */
        glBegin(GL_POLYGON);
        glNormal3f(0,1,0);
        glVertex3f(-i, 0, -1);
        glVertex3f(-i-1, 0, -1);
        glVertex3f(-i-1, 0, 1);
        glVertex3f(-i, 0, 1);
        glEnd();
        
         glBegin(GL_POLYGON);
        glNormal3f(0,-1,0);
        glVertex3f(-i, 2, -1);
        glVertex3f(-i-1, 2, -1);
        glVertex3f(-i-1, 2, 1);
        glVertex3f(-i, 2, 1);
        glEnd();
        
        glBegin(GL_POLYGON);
        glNormal3f(0,0,1);
        glVertex3f(-i, 0, -1);
        glVertex3f(-i-1, 0, -1);
        glVertex3f(-i-1, 2, -1);
        glVertex3f(-i, 2, -1);
        glEnd();
        
        glBegin(GL_POLYGON);
        glNormal3f(0,0,-1);
        glVertex3f(-i, 0, 1);
        glVertex3f(-i-1, 0, 1);
        glVertex3f(-i-1, 2, 1);
        glVertex3f(-i, 2, 1);
        glEnd();
        
        glBegin(GL_POLYGON);
        glNormal3f(-1, 0, 0);
        glVertex3f(-31, 0, -1);
        glVertex3f(-31, 0, 1);
        glVertex3f(-31, 2, 1);
        glVertex3f(-31, 2, -1);
        glEnd();
        
    }
 
}
void draw_special()
{
    float light_x = 0;
    float light_z = 0;
    glEnable(GL_LIGHT1);
    switch(special_position)
    {
        case 0:
            light_z=special_time;
            if(light_z>31)
            {
                special_active=0;
                special_time=0;
                glDisable(GL_LIGHT1);
            }
            break;
        case 1:
            light_x=special_time;
            if(light_x>31)
            {
                special_active=0;
                special_time=0;
                glDisable(GL_LIGHT1);
            }
            break;
        case 2:
            light_z=-special_time;
            if(light_z<-31)
            {
                special_active=0;
                special_time=0;
                glDisable(GL_LIGHT1);
            }
            break;
        case 3:
            light_x=-special_time;
            if(light_x<-31)
            {
                special_active=0;
                special_time=0;
                glDisable(GL_LIGHT1);
            }
            break;
    }
    GLfloat light_position[] = {light_x, 1, light_z, 1};
    GLfloat light_ambient[] = { 0, 0, 0, 1 };
    GLfloat light_diffuse[] = { 0.9, 0.9, 0.9, 1 };
    GLfloat light_specular[] = {0.5, 0.5, 0.5, 1 };
    glLightfv(GL_LIGHT1, GL_POSITION, light_position);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
}


static void on_mouse(int button, int state, int x, int y)
{
    /* Projektil se ispaljuje na klik levog dugmeta misa */
    if(button!=GLUT_LEFT_BUTTON || state!=GLUT_DOWN)
        return;
    
    /* Moguce je ispaliti do 3 projektila u isto vreme, trazi se slobodan projektil */
    int no_free_projectile=1;
    for(int i=0;i<3;i++)
    {
        if(projectile_active[i]==0)
        {
            projectile_number=i;
            no_free_projectile=0;
            break;
        }
    }
    /* Ako ne postoji slobodan projektil, izlazi se iz funkcije */
    if(no_free_projectile)
        return;
    /* Izracunava se mesto gde se desio klik */
    float x_angle=x/window_width;
    float y_angle=y/window_height;
    x_angle-=0.5;
    y_angle-=0.5;
    
    /* Uglovi vidnog polja */
    float horizontal_view_angle=60*(window_width/window_height);
    float vertical_view_angle=60;
    
    /* Uglovi projektila */
    float projectile_y_angle=y_angle*vertical_view_angle*PI/180;
    float projectile_x_angle=camera_angle-(x_angle*horizontal_view_angle*PI/180);
    projectile_angles[2*projectile_number]=projectile_x_angle;
    projectile_angles[2*projectile_number+1]=projectile_y_angle;
    
    
    /* Timer za projektil se poziva samo ako taj projektil nije aktivan */
    if(projectile_active[projectile_number]==0)
    {
        projectile_active[projectile_number]=1;
        projectile_position[projectile_number]=camera_position;
        glutTimerFunc(TIMER_INTERVAL, projectile_timer, projectile_number);
    }
   
}
static void on_reshape(int width, int height)
{
    /* Pamte se visina i sirina prozora */
    window_width = width;
    window_height = height;
}
static void on_keyboard(unsigned char key, int x, int y)
{
    if(wave_time==0 && wave_number==1)
            {
                cooldown=0;
                special_active=0;
                glutTimerFunc(100, wave_timer, TIMER_ID);
                glutTimerFunc(1000, special_cooldown_timer, TIMER_ID);
            }
    switch(key)
    {
        case 27:
            /* Na esc taster se izlazi iz programa */
            exit(0);
            break;
        case 'a':
            /* Ako se trenutno ne odvija kretanje pokrece se timer */
            if(movement_direction==0)
            {
                camera_position+=1;
                movement_direction=1;
                glutTimerFunc(TIMER_INTERVAL, movement_timer, TIMER_ID);
            }
            break;
        case 'd':
            /* Ako se trenutno ne odvija kretanje pokrece se timer */
            if(movement_direction==0)
            {
                camera_position-=1;
                movement_direction=-1;
                glutTimerFunc(TIMER_INTERVAL, movement_timer, TIMER_ID);
            }
            break;
        case 'f':
            if(special_active==0 && cooldown==0)
            {
                special_active=1;
                special_position=camera_position;
                cooldown=20;
                cooldown_color=1;
                glutTimerFunc(TIMER_INTERVAL, special_timer, TIMER_ID);
            }
            break;
    }
    
}
static void on_display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glViewport(0, 0, window_width,window_height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, window_width/window_height, 1, 50);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 1, 0,sin(camera_angle), 1, cos(camera_angle), 0, 1, 0);
    
    /* Glavno osvetljenje */
    GLfloat light_position[] = { 0, 1, 0, 1};
    GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1 };
    GLfloat light_diffuse[] = { 0.9, 0.9, 0.9, 1 };
    GLfloat light_specular[] = {0.7, 0.7, 0.7, 1 };
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
        
    for(int i=0;i<3;i++)
    {
        if(projectile_active[i])
            draw_projectile(i);
    }
    for(int i=0;i<10;i++)
    {
        if(enemy_active[i])
            draw_enemy(i, enemy_coordinates[2*i], enemy_coordinates[2*i+1]);
    }
    if(special_active)
        draw_special();
    draw_tunnel();
    draw_hud();
    
    glutSwapBuffers();
}

int main(int argc, char** argv)
{
    /* Inicijalizacija */
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(600,600);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Tunnel Defense");
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);
    glutMouseFunc(on_mouse);
    
    glClearColor(0,0,0,0);
    glEnable(GL_DEPTH_TEST);
    
    glutMainLoop();
    
    return 0;
}
