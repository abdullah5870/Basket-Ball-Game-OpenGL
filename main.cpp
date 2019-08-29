/**
ballX  => ball's X coordinate
ballY  => ball's Y coordinate
ballR  => ball's Radius
meterA => throwing angle
meterAX => throwing angle meter X coordinate
meterAY => throwing angle meter Y coordinate
meterAR => throwing angle meter radius
meterBX => speed meter X coordinate
meterBY => speed meter Y coordinate
basketX => basket X coordinate
**/

#include <windows.h>
#include <GL/glut.h>
#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;

int restart = 0;
float ballX = 230,ballY = 160,ballR = 10;
int meterA = 72, speed = 60;
int meterAX = 50, meterAY = 50, meterAR = 30;
int meterBX = 100, meterBY = 50;
float basketX = 600, basketY = 300;
clock_t timeStart;
bool startGame = false;
bool success = false, crossX = false, crossY = false, crossDisplay = false;
float successX = 0;
float currentX= 0, currentState = 0;
float degree = 0;
int bestScore = 0;

int life = 5, score = 0;


void drawBall(float x, float y, float r);
void moveBall(float x, float y, float r);
void drawBasket(int basket);
void drawAngleMeter(int degree);
void drawSpeedMeter(int height);
void drawBackground();
void myInit (void);
void myDisplay(void);
void keyListener(int key,int, int);
void draw_object();
void draw_circle(float x, float y, float w, float h);




void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius){
	int i;
	int triangleAmount = 20; //# of triangles used to draw circle

	//GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * 3.1416;

	glBegin(GL_TRIANGLE_FAN);
		glVertex2f(x, y); // center of circle
		for(i = 0; i <= triangleAmount;i++) {
			glVertex2f(
		            x + (radius * cos(i *  twicePi / triangleAmount)),
			    y + (radius * sin(i * twicePi / triangleAmount))
			);
		}
	glEnd();
}

void draw_object()
{
    //sky
    glColor3f(0.0,0.9,0.9);
    glBegin(GL_POLYGON);
    glVertex2f(0,250);
    glVertex2f(0,700);
    glVertex2f(1100,700);
    glVertex2f(1100,250);
    glEnd();



    //grass
    glColor3f(0.0,0.9,0.0);
    glBegin(GL_POLYGON);
    glVertex2f(0,0);
    glVertex2f(0,250);
    glVertex2f(1200,250);
    glVertex2f(1100,0);
    glEnd();

    //compound
    glColor3f(0.7,0.7,0.7);
    glBegin(GL_POLYGON);
    glVertex2f(basketX-15,basketY-145);
    glVertex2f(basketX-15,basketY-160);
    glVertex2f(basketX+15,basketY-160);
    glVertex2f(basketX+15,basketY-145);
    glEnd();

    glColor3f(1.0,1.0,1.0);

    //cloud1
    drawFilledCircle(50.0,400.0,20.0);
    drawFilledCircle(70.0,420.0,20.5);
    drawFilledCircle(80.0,390.0,21.5);
    drawFilledCircle(100.0,400.0,20.0);
    drawFilledCircle(120.0,420.0,20.5);
    drawFilledCircle(125.0,390.0,21.5);

    //cloud2
    drawFilledCircle(200.0,400.0,20.0);
    drawFilledCircle(220.0,420.0,20.5);
    drawFilledCircle(230.0,390.0,21.5);
    drawFilledCircle(250.0,400.0,20.0);
    drawFilledCircle(270.0,420.0,20.5);
    drawFilledCircle(280.0,390.0,21.5);

    //cloud3
    drawFilledCircle(400.0,400.0,20.0);
    drawFilledCircle(420.0,420.0,20.5);
    drawFilledCircle(430.0,390.0,21.5);
    drawFilledCircle(450.0,400.0,20.0);
    drawFilledCircle(470.0,420.0,20.5);
    drawFilledCircle(480.0,390.0,21.5);
}


void draw_circle(float x, float y, float w, float h){
    glBegin(GL_TRIANGLE_FAN);
    for(int i=0; i<360; i++){
        float degree = i * 3.1416 / 180.0;
        glVertex2f(w*cos(degree)+x, h*sin(degree)+y);
    }
    glEnd();
}


void drawBackground(){
    draw_object();
}

void drawBall(float x, float y, float r){
    if((x > basketX-50 && x< basketX - 25) && (y> basketY-2 && y < basketY+2)){
        if(success == false){
            life++;
            score++;
            if(score>bestScore){
                bestScore = score;
            }
            system("cls");
            cout<<"score->"<<score<<"\n";
            cout<<"Best score->"<<bestScore<<"\n";
        }
        successX = x;
        success = true;
    }
    if(success){
        x = successX;
        glColor3f (1.0, 0.0, 0.0);
    }
    else{
        glColor3f (1.0, 0.65, 0.0);
    }
    glBegin(GL_TRIANGLE_FAN);
    for(int i=0; i<360; i++){
        degree = i * 3.1416 / 180.0;
        glVertex2f(r*cos(degree)+x, r*sin(degree)+y);
    }
    glEnd();
}

void moveBall(float x, float y, float r){
    if(life<=0){
        return;
    }
    if(!startGame){
        float angle = 0;
        glBegin(GL_TRIANGLE_FAN);
        glColor3f (1.0, 0.65, 0.0);
        if(!startGame){
            for(int i=0; i<360; i++){
                angle = i * 3.1416 / 180.0;
                glVertex2f(ballR*cos(angle)+ballX, ballR*sin(angle)+ballY);
            }
        }
        glEnd();
        return;
    }
    float duration = (clock() - timeStart) / (float) CLOCKS_PER_SEC * 5;
    float prX = (speed+20) * cos(meterA * 3.1416 / 180.0) * duration;
    float prY = (speed+20) * sin(meterA * 3.1416 / 180.0) * duration - (0.5 * 9.8 * duration * duration);
    float backX = (ballX+prX) - (basketX - 20);
    if(!crossX && !crossY){
        drawBall(ballX+prX, ballY+prY, r);
    }else if(crossX && !crossY){
        drawBall( basketX - backX, ballY+prY, r);
    }else if(!crossX && crossY){
        if(ballY+prY < basketY-145){
            meterA = 0;
            speed =0;
            life--;
            startGame = false;
        }
        else{
            drawBall(currentState, basketY-145, r);
        }
    }else{
        life--;
        startGame = false;
        drawBall(currentX, basketY-145, r);
        meterA = 0;
        speed =0;
    }
    if((ballX+prX < basketX - 8) && (ballX+prX > basketX - 13) && (ballY+prY < basketY + 51)){
        crossX = true;
    }
    if(ballY+prY < basketY-145){
        crossY = true;
        timeStart = clock();
        currentState = ballX + prX;
        currentX= basketX - backX;
    }
    if(ballX+prX < basketX+20 && ballY+prY < basketY-145){
        life--;
        startGame = false;
        meterA = 0;
        speed =0;
    }
    if(ballX+prX > basketX+25 && !crossX){
        life--;
        startGame = false;
        meterA = 0;
        speed =0;
    }
}

void drawLifeBar(){
    int l = life;
    glColor3f(1,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2i(50, 450);
    glVertex2i(200, 450);
    glVertex2i(200, 460);
    glVertex2i(50, 460);
    glEnd();
    glBegin(GL_POLYGON);
    if(life>5){
        l = 5;
    }
    glVertex2i(50, 450);
    glVertex2i(50+l*30, 450);
    glVertex2i(50+l*30, 460);
    glVertex2i(50, 460);
    glEnd();
    glColor3f(1,1,1);
    glBegin(GL_LINES);
    for(int i=1; i<5; i++){
        glVertex2i(50+i*30, 460);
        glVertex2i(50+i*30, 450);
    }
    glEnd();
}

void drawBasket(){
    glColor3f (0.3333, 0.3333, 0.3333);
    glBegin(GL_POLYGON);
    glVertex2i(basketX, basketY+3);
    glVertex2i(basketX-20, basketY+3);
    glVertex2i(basketX-20, basketY-2);
    glVertex2i(basketX, basketY-2);
    glVertex2i(basketX, basketY-150);
    glVertex2i(basketX+5, basketY-150);
    glVertex2i(basketX+5, basketY+50);
    glVertex2i(basketX, basketY+50);
    glEnd();
    float degree = 0;
    glBegin(GL_POINTS);
    for(int i=0; i<360; i++){
        degree = i * 3.1416 / 180.0;
        glVertex2f(20*cos(degree)+basketX-39, 7*sin(degree)+basketY);
    }
    glEnd();
}

void drawAngleMeter(int degree){
    glColor3f (1.0, 0.0, 0.0);
    float angle = 0;
    glBegin(GL_POINTS);
    for(int i=0; i<180; i++){
        angle = i * 3.1416 / 180.0;
        glVertex2f(meterAR*cos(angle)+meterAX, meterAR*sin(angle)+meterAY);
    }
    glEnd();
    float rad = degree * 3.1416 / 180.0;
    float x = meterAR*cos(rad) + meterAX;
    float y = meterAR*sin(rad) + meterAY;
    glBegin(GL_LINES);
    glVertex2f(meterAX,meterAY);
    glVertex2f(x, y);
    glEnd();
}

void drawSpeedMeter(int height){
    glColor3f (1.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(meterBX ,meterBY);
    glVertex2f(meterBX + 10,meterBY);
    glVertex2f(meterBX + 10,meterBY + 100);
    glVertex2f(meterBX ,meterBY + 100);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(meterBX ,meterBY );
    glVertex2f(meterBX + 10,meterBY);
    glVertex2f(meterBX + 10,meterBY + height);
    glVertex2f(meterBX , meterBY + height);
    //glVertex2f(meterBX , meterBY);
    glEnd();
}

void myInit (void)
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 640.0, 0.0, 480.0);
}

void myDisplay(void)
{
    glClear (GL_COLOR_BUFFER_BIT);
    drawBackground();

    drawBasket();
    moveBall(ballX,ballY,ballR);
    drawAngleMeter(meterA);
    drawSpeedMeter(speed);
    drawLifeBar();
    glFlush();
    glutPostRedisplay();
}

void restartGame(unsigned char key, int ,int){
    switch(key){
        case 'r':{
            if(!life){
                life = 5;
                score = 0;
                startGame = false;
                crossX = crossY = success = false;
            }
            break;
        }

        case ' ' : {
            if(!startGame){
                startGame = true;
                timeStart = clock();
                crossX = crossY = success = false;
                break;
            }
        }
    }
}

void keyListener(int key,int, int){
    switch(key){
        case GLUT_KEY_LEFT: {
            if(!startGame){
                meterA++;
                if(meterA > 90){
                    meterA=90;
                }
            }
            break;
        }
        case GLUT_KEY_RIGHT:{
            if(!startGame){
                meterA--;
                if(meterA < 0){
                    meterA=0;
                }
            }
            break;
        }
        case GLUT_KEY_UP: {
            if(!startGame){
                speed++;
                if(speed>100){
                    speed = 100;
                }
            }
            break;
        }
        case GLUT_KEY_DOWN: {
            if(!startGame){
                speed--;
                if(speed<0){
                    speed = 0;
                }
            } break;
        }
        case GLUT_KEY_END: {
            if(life<=0){
                exit(0);
            }
        }
    }
}

int main(int argc, char** argv)
{
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (1080, 810);
    glutInitWindowPosition (200, 200);
    glutCreateWindow ("Basketball");
    myInit();
    glutDisplayFunc(myDisplay);
    glutSpecialFunc(keyListener);
    glutKeyboardFunc(restartGame);
    glutMainLoop();
}
