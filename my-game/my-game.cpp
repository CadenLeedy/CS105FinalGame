// Game Name: Cosmic Climb
// Written by: Caden Leedy
// Date: 12/06/2022
//
// Instructions: Try to climb as high as possible without falling by bouncing off the white platforms. Hover over the screen
// with the mouse and move it left or right to move the ball. 
//

#include "graphics.h"
#include <iostream>
#include <string>
#include <random>
#include <cmath>
#include <stdio.h>
#include <string>


using namespace std;

const int BACKGROUND_COLOR = BLUE;
const int TEXT_COLOR = WHITE;
const int ARRAYSIZE = 9;
const int INITVEL = -26;

random_device myEngine;
uniform_real_distribution<double> randomReal(0.0, 1.0);


//enumerated types:

//defines enumerated type for the different types of platforms
enum PlatformType { STATIC, BROKEN, MOVING };

//defines enumerated type for different scenarios of the ball landing on a platform
enum Landed { LEFT, MIDDLE, RIGHT };

//set gravity
const double GRAVITY = 1.00;

//constant variables for starting position of ball for each new game
const double RESETX = 250;
const double RESETY = 750;


//Structure definitions:

//defines structure for the player which is the ball
struct Player {
	// Variables to keep track of position, previous position, and velocity
	double x, y, prevX, prevY, velX, velY, gravity = GRAVITY;
	int color;
	bool alive;



	//a "constructor" function
	Player(int c) {
		color = c;
		alive = true;

		// Set Initial Position
		x = RESETX;
		y = RESETY;

		//set previous position to be current position
		prevX = x;
		prevY = y;

		// Set Initial Velocity
		velX = 0;
		velY = INITVEL;
	} // end Player() constructor function

	void move() {

		//set previous position equal to current position to keep track of old position
		prevX = x;
		prevY = y;

		//erase old position
		setcolor(BLUE);
		setfillstyle(SOLID_FILL, BLUE);
		fillellipse(prevX, prevY, 20, 20);

		//move ball and redraw it
		x += velX;
		y += velY;

		//if ball goes off one side of the screen, makes it re-enter on the other side
		if (x <= -15) {
			x = 515;
		}
		else if (x >= 515) {
			x = -15;
		}


		setcolor(color);
		setfillstyle(SOLID_FILL, color);
		fillellipse(x, y, 20, 20);



		velY += gravity; //change the velocity  by the acceleration due to gravity


		velX = (static_cast<double>((250 - mousex()) * (-1))) / 10;

	}
};


//defines structure for the platforms
struct Platform {
	double x, y, prevX, prevY, velX, velY;
	PlatformType type;
	bool alive;
	bool spring;


	//function to calculate distance from player center to top left corner of platform
	// input the player.x and player.y
	double Ldistance(double ballx, double bally) {
		double distance = sqrt(pow(ballx - (x - 40), 2) + pow(bally - (y), 2));
		return distance;
	}
	//function to calculate distance from player center to top right corner of platform
	// input the ball.x and ball.y
	double Rdistance(double ballx, double bally) {
		double distance = sqrt(pow(ballx - (x + 40), 2) + pow(bally - (y), 2));
		return distance;
	}

	//function that determines if the ball landed on the plaform, 
	//and returns whether it hit the left rounded edge, somewhere in the middle, or the right rounded edge of the platform
	//input ball.x, ball.y, and ball.velY
	Landed landedOn(double ballx, double bally, double prevbally, double vely) {
		if (vely > 0) {
			if (((ballx - 15) <= (x + 40)) && ((ballx + 15) >= (x - 40)) && ((bally + 20) >= (y - 5)) && ((prevbally + 20) <= (y - 5))) {
				//if ((ballx <= (x + 40)) && (ballx >= (x - 40)) && ((bally) >= (y + 5)) && ((y-bally) <=25)) {
				return MIDDLE;
			}
			/*else if ((Ldistance(ballx, bally) <= 25) && ballx < (x - 40)) {
				return LEFT;
			}
			else if ((Rdistance(ballx, bally) <= 25) && ballx > (x + 40)) {
				return RIGHT;
			}
			*/
		}
	}

	void move(bool moveY) {
		//erase old
		setcolor(BLUE);
		setfillstyle(SOLID_FILL, BLUE);
		bar(x - 40, y + 5, x + 40, y - 5);
		pieslice(x - 40, y, 90, 270, 5);
		pieslice(x + 40, y, 270, 90, 5);

		if (type == MOVING) {
			if (x >= 450 || x <= 50) {
				velX *= -1;
			}
			x += velX;
		}
		if (moveY)
			y -= velY;

		//draw new
		setcolor(WHITE);
		setfillstyle(SOLID_FILL, WHITE);
		bar(x - 40, y + 5, x + 40, y - 5);
		pieslice(x - 40, y, 90, 270, 5);
		pieslice(x + 40, y, 270, 90, 5);
	}


};
//end structure definitions



//Function prototypes
void welcomeScreen();
void drawHoop(int, int);
void initPlatform(Platform&, double, double, int);
void bounceball(Landed, Player&, Platform&);
void drawPlatform(double, double);
void displayScore(int, int, int);
void setCosmos(int[], int[]);
void drawCosmos(int [], int[]);

int main()
{
	bool keepGoing = true;
	bool movingPlatforms = false;
	double platformVel = 0;
	char keyPressed;
	int score = 0;
	int xArray[500];
	int yArray[500];



	initwindow(500, 800, (char*)"Cosmic Climb", 10, 10);

  	setCosmos(xArray, yArray);
	drawCosmos(xArray, yArray);

	welcomeScreen();

	bool playAgain = true;
	

















	//define types of platforms using enumerated type


	int landedi;
	int landedY;


	//
	// -------- Main "Play Again" loop
	//
	do {
		drawCosmos(xArray, yArray);
		score = 0;

		

		//defines the ball/player
		Player ball(LIGHTRED);

		//draw ball
		ball.x = RESETX;
		ball.y = RESETY;
		ball.velY = INITVEL;
		ball.velX = 0;
		fillellipse(ball.x, ball.y, 20, 20);



		//intialize the array for the platforms with a for loop, giving each platform/element a random type and location. Weight the types 
		//so that static is most common (random 1-10 where static is 1-5, moving 6-8,etc), and then broken and then moving. For the locations, 
		//make the randomizer choose a random y between two values, and each time make the previous max y the new min y. Maybe randomize a 
		//number 1 - 3 for how many platforms can be between each step of y values

		//defines an array of structures to hold the locations of every platform on the screen

		int numarray[ARRAYSIZE];
		for (int i = 0; i < ARRAYSIZE; i++) {

		}
		Platform platforms[ARRAYSIZE];

		//initializes the locations of all the platforms, and if one is made too close to another, it is moved
		//bool spotValid = true;
		double maxy;
		double miny;
		for (int i = 0; i < ARRAYSIZE; i++) {
			if (i < 3) {
				maxy = 775;
				miny = 525;
			}
			else if (i >= 3 && i < 6) {
				maxy = 515;
				miny = 275;
			}
			else if (i < 9) {
				maxy = 265;
				miny = 25;
			}




			initPlatform(platforms[i], maxy, miny, score);
			/*do {
				for (int n = 0; n < i; n++) {
					if (abs(platforms[i].x - platforms[n].x) <= 90 && abs(platforms[i].y - platforms[n].y) <= 10) {
						initPlatform(platforms[i]);
						spotValid = false;
					}
				}
			} while (spotValid == false);
			*/
		}

		//checks if platform bumped into edge of the screen and if so reverses its direction


		//draws all the platforms
		for (int i = 0; i < ARRAYSIZE; i++) {
			drawPlatform(platforms[i].x, platforms[i].y);
		}


		// 
		// -------- Main "Keep Going" loop
		// 
		keepGoing = true;
		while (keepGoing) {
			if (score < 5000)
				delay(1);
			else if (score >= 5000 && score < 10000)
				delay(0);
			else if (score >= 10000)
				delay(0);

			drawCosmos(xArray, yArray);


			//temporary
			//Platform platform1 = { 200, 500, 200, 500, 0, 0, STATIC };
			//drawPlatform(platform1.x, platform1.y);

			//draws all the platforms
			for (int i = 0; i < ARRAYSIZE; i++) {

				platforms[i].move(false);
			}



			ball.move();


			// ball falls through bottom of screen, end game
			if (ball.y > 800)
				keepGoing = false;



			//iterate through every platform and check if it was landed on, then make ball bounce if one was


			for (int i = 0; i < ARRAYSIZE; i++) {
				Landed spotLanded = platforms[i].landedOn(ball.x, ball.y, ball.prevY, ball.velY);
				if (spotLanded == MIDDLE) {
					landedi = i;
					landedY = platforms[landedi].y;
				}
				bounceball(spotLanded, ball, platforms[i]);
			}

			//if ball reaches goes past the max height, make it stop moving and move all the platforms down instead
			if (ball.y <= 375) {
				movingPlatforms = true;
				platformVel = ball.velY;
				setcolor(BLUE);
				setfillstyle(SOLID_FILL, BLUE);
				fillellipse(ball.x, ball.y, 20, 20);
				ball.y = 376;
				ball.velY = 0;
				ball.gravity = 0;
			}

			//make all the platforms move down
			if (movingPlatforms) {


				for (int i = 0; i < ARRAYSIZE; i++) {
					platforms[i].velY = platformVel;
					platforms[i].move(movingPlatforms);
					score -= platformVel / 10; //increase the score
					//if one of the platforms goes off the bottom of the screen, replace it with a random new one at the top of the screen
					if (platforms[i].y >= 800) {
						initPlatform(platforms[i], platforms[landedi].y - landedY + 25, platforms[landedi].y - landedY - 225, score);
						drawPlatform(platforms[i].x, platforms[i].y);
					}
				}

				//stop moving the platforms once they have moved down enough
				if (platforms[landedi].y >= (RESETY)) {
					movingPlatforms = false;
					ball.gravity = GRAVITY;
					platformVel = 0;
				}


			}

			//draw the score
			displayScore(score, 10, 10);



			//redraw rims
			/*setfillstyle(SOLID_FILL, RED);
			setcolor(RED);
			bar(300 - 28, 100 + 20, 300 + 28, 100 + 23); // rim
			bar(100 - 28, 250 + 20, 100 + 28, 250 + 23);
			bar(300 - 28, 600 + 20, 300 + 28, 600 + 23);
			*/




			// -------- Check to see if a key has been pressed
			if (kbhit()) {
				keyPressed = getch();

				// KEY: Q or ESC pressed
				if (keyPressed == 'q' || keyPressed == 'Q' || keyPressed == 0x1b) {
					keepGoing = false;
				}
			}

			// -------- Check to see if a mouse was clicked 
			if (ismouseclick(WM_LBUTTONUP)) {
				int mouseX, mouseY;
				getmouseclick(WM_LBUTTONUP, mouseX, mouseY);
			}

		} // end while(keepGoing)

		// Game has ended
		setcolor(TEXT_COLOR);
		outtextxy(10, 10, (char*)"GAME OVER, Play Again (Y/N)");
		displayScore(score, 175, 350);
		keyPressed = getch();
		if (keyPressed == 'y' || keyPressed == 'Y') {
			welcomeScreen();
		}
		else {
			playAgain = false;
		}

	} while (playAgain == true);  // Main "Play Again" loop

	return 0;
} // end main()

// Functions
void welcomeScreen() {
	setbkcolor(BACKGROUND_COLOR);
	cleardevice();

	setcolor(TEXT_COLOR);
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
	outtextxy(80, 300, (char*)"Welcome to Cosmic Climb");
	outtextxy(80, 400, (char*)"Press any key to begin");

	getch();
	cleardevice();
}

void drawHoop(int x, int y) {
	setfillstyle(SOLID_FILL, DARKGRAY);
	bar(x - 50, y - 35, x + 50, y + 35); //outline of whole backboard
	setfillstyle(SOLID_FILL, WHITE);
	bar(x - 45, y - 30, x + 45, y + 30); //whole white part of backboard
	setfillstyle(SOLID_FILL, DARKGRAY);
	bar(x - 25, y - 15, x + 25, y + 20); //outline of whole inner box
	setfillstyle(SOLID_FILL, WHITE);
	bar(x - 20, y - 10, x + 20, y + 15); //whole white part of inner box
	setfillstyle(SOLID_FILL, RED);
	setcolor(RED);
	bar(x - 28, y + 20, x + 28, y + 23); // rim
	setfillstyle(SOLID_FILL, DARKGRAY);
	setfillstyle(SOLID_FILL, RED);
	bar(x - 10, y + 20, x + 10, y + 27); //red square behind rim

}

void initPlatform(Platform& plat, double maxY, double minY, int score) {
	//pick random coordinates for the platform within the alotted section of screen
	plat.x = 50 + randomReal(myEngine) * 400;
	plat.y = minY + randomReal(myEngine) * (maxY - minY); // picks a y value between the minimum value and the maximum value
	//choose a random platform type
	double rand1 = randomReal(myEngine);
	if (score < 6000) {
		if (rand1 > 0 && rand1 <= 0.6)
			plat.type = STATIC;
		else if (rand1 > 0.6 && rand1 <= 0.8)
			plat.type = BROKEN;
		else
			plat.type = MOVING;
	}
	else if (score >= 5000 && score < 8000) {
		if (rand1 > 0 && rand1 <= 0.4)
			plat.type = STATIC;
		else if (rand1 > 0.4 && rand1 <= 0.6)
			plat.type = BROKEN;
		else
			plat.type = MOVING;
	}
	else if (score >= 8000 && score < 10000) {
		if (rand1 > 0 && rand1 <= 0.2)
			plat.type = STATIC;
		else if (rand1 > 0.2 && rand1 <= 0.4)
			plat.type = BROKEN;
		else
			plat.type = MOVING;
	}
	else if (score >= 10000) {
		if (rand1 > -1 && rand1 < 0)
			plat.type = STATIC;
		else if (rand1 > 0 && rand1 <= 0.1)
			plat.type = BROKEN;
		else
			plat.type = MOVING;
	}



	if (plat.type == MOVING) {
		if (score < 5000)
			plat.velX = 3;
		else if (score >= 5000 && score < 10000)
			plat.velX = 4;
		else if (score >= 10000 && score < 15000)
			plat.velX = 5;
		else if (score >= 15000 && score)
			plat.velX = 6;
	}


	else
		plat.velX = 0;
	//choose randomly if the platform will have a spring on it
	double rand2 = randomReal(myEngine);
	if (rand2 >= 0 && rand2 < 80)
		plat.spring = false;
	else
		plat.spring = true;

}

void drawSpring(double x, double y) {
	double springx = randomReal(myEngine) * 60;
	springx = x - 40 + springx;
	double springy = y + 8;
	setcolor(BLUE);
	setfillstyle(SOLID_FILL, BLUE);
	bar(springx - 3, springy + 3, springx + 3, springy - 3);
	setcolor(BLACK);
	setfillstyle(SOLID_FILL, BLACK);
	bar(springx - 3, springy + 3, springx + 3, springy - 3);
	

}

void drawPlatform(double x, double y) {
	setcolor(WHITE);
	setfillstyle(SOLID_FILL, WHITE);
	bar(x - 40, y + 5, x + 40, y - 5);
	pieslice(x - 40, y, 90, 270, 5);
	pieslice(x + 40, y, 270, 90, 5);
}

void bounceball(Landed landtype, Player& ball, Platform& plat) {

	double velx;
	double vely;
	double theta;
	double base;
	double height;
	double collisionX;
	double collisionY;
	double rPlatform = 5;
	double rBall = 20;

	switch (landtype) {

	case LEFT:
		collisionX =
			((ball.x * rPlatform) + ((plat.x - 40) * rBall))
			/ (rBall + rPlatform);

		collisionY =
			((ball.y * rPlatform) + (plat.y * rBall))
			/ (rBall + rPlatform);
		base = abs(ball.x - collisionX);
		height = abs(ball.y - collisionY);
		theta = atan(height / base);
		velx = (ball.velX * cos(theta)) + (sin(theta) * ball.velY);
		vely = (-sin(theta) * ball.velX) + (cos(theta) * ball.velY);
		vely = -vely;
		ball.velX = (ball.velX * cos(theta)) + (-sin(theta) * ball.velY);
		ball.velY = (sin(theta) * ball.velX) + (cos(theta) * ball.velY);
		break;

	case RIGHT:
		collisionX =
			((ball.x * rPlatform) + ((plat.x + 40) * rBall))
			/ (rBall + rPlatform);

		collisionY =
			((ball.y * rPlatform) + (plat.y * rBall))
			/ (rBall + rPlatform);
		base = abs(ball.x - collisionX);
		height = abs(ball.y - collisionY);
		theta = atan(height / base);
		velx = (ball.velX * cos(theta)) + (sin(theta) * ball.velY);
		vely = (-sin(theta) * ball.velX) + (cos(theta) * ball.velY);
		vely = -vely;
		ball.velX = (ball.velX * cos(theta)) + (-sin(theta) * ball.velY);
		ball.velY = (sin(theta) * ball.velX) + (cos(theta) * ball.velY);
		break;

	case MIDDLE:
		//erase current position
		setcolor(BLUE);
		setfillstyle(SOLID_FILL, BLUE);
		fillellipse(ball.x, ball.y, 20, 20);
		//move ball so sits perfectly on plaform
		ball.y = plat.y - 25;
		//reverse ball's y velocity
		ball.velY = INITVEL;
		break;
	}

}

void displayScore(int score, int x, int y) {
	char s[20]; // An array of char, which is a string
	sprintf_s(s, "Score: %d", score); // "print" 400 into a string

	setcolor(WHITE);
	outtextxy(x, y, &s[0]); // Draw that string in the graphics window
}
   
void setCosmos(int xArray[], int yArray[]) {
	double randx;
	double randy;
	
	for (int i = 0; i < 500; i++) {
		randx = randomReal(myEngine);
		randy = randomReal(myEngine);
		xArray[i] = randx * 500;
		yArray[i] = randy * 800;
	}
}
void drawCosmos(int xArray[], int yArray[]) {
	
	for (int i = 0; i < 500; i++) {
		int randr = randomReal(myEngine) * 3;
		setcolor(WHITE);
		setfillstyle(SOLID_FILL, WHITE);
		fillellipse(xArray[i], yArray[i], randr, randr);
	}
}