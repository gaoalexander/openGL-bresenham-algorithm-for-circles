#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
using namespace std;
#include <stdio.h>
#include <math.h>
#ifdef __APPLE__  // include Mac OS X verions of headers
#include <GLUT/glut.h>
#else // non-Mac OS X operating systems
#include <GL/glut.h>
#endif

#define XOFF          50
#define YOFF          50
#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT 600

void display_fromuser(void);
void display_fromfile(void);
void display_fromfile_animate(void);
void myinit(void);

/* Function to handle file input; modification may be needed */
void file_in(string pathname);
string pathname;
void calculate_scale_factor(void);

/* Functions which drive drawing of circles */
void circlePoint(int x, int y, int origin_x, int origin_y);
void draw_circle(int x, int y, int r);

/* Additional Global variable declarations */
int **input_circles_array;   // Multi-dimensional array data structure which will store parameters read from FILE, for circles to be drawn
int num_circles = 0;
float window_scale_factor = 0;
int x, y, r;  // GLOBAL X,Y,R
int j = 1;
int K = 240;  // Number of steps required for circles to grow to full size

/*-----------------
The main function
------------------*/
int main(int argc, char **argv)
{
    int user_choice = 0;
    cout << "Choose from the following options: "<<endl;
    cout << "1 - Input a custom origin(x,y) and circle radius."<<endl;
    cout << "2 - Read in circle data from a file and display circles statically."<<endl;
    cout << "3 - Read in circle data from a file and display animation cycle of circles growing."<<endl;
    cout << endl<< "Please enter the number corresponding to your choice:";
    cin >> user_choice;

    glutInit(&argc, argv);
    
    /* Use both double buffering and Z buffer */
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(XOFF, YOFF);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("CS6533/CS4533 Assignment 1");
    
    if((user_choice != 1)&&(user_choice != 2)&&(user_choice != 3)){
        cout << "You did not enter a valid choice. Goodbye."; }
    
    else if (user_choice == 1){
        /* Prompt user to input values: x, y, r */
        cout << "Please enter a starting X coordinate between 0 and "<<WINDOW_WIDTH<<":";
        cin >> x;
        ::x = x;
        cout << "Please enter a starting Y coordinate between 0 and "<<WINDOW_HEIGHT<<":";
        cin >> y;
        ::y = y;
        cout << "Please enter a circle radius: ";
        cin >> r;
        ::r = r;
        glutDisplayFunc(display_fromuser);
    }
    else if (user_choice == 2){
        cout << "Please enter the filepath of your document containing circle data (NO SPACES):";
        cin >> pathname;

        /* Function call to handle file input here */
        file_in(pathname);
        calculate_scale_factor();
        glutDisplayFunc(display_fromfile);
    }
    else if (user_choice == 3){
        cout << "Please enter the filepath of your document containing circle data (NO SPACES):";
        cin >> pathname;
        
        /* Function call to handle file input here */
        file_in(pathname);
        calculate_scale_factor();
        glutDisplayFunc(display_fromfile_animate);
    }
    
    myinit();
    glutMainLoop();

    return 0;
}

/*----------
file_in(): file input function. Modify here.
------------*/
void file_in(string pathname)
{
    ifstream input_circles;
    input_circles.open(pathname.c_str());

    if(input_circles.fail()){
        cout << "File failed to open.";
        exit(1);}
    
    input_circles >> num_circles;                   // Initialize num_circles
    input_circles_array = new int*[num_circles];    // Initialize size of input_circles_array data structure
    for(int i = 0; i < num_circles; i++){
        input_circles_array[i] = new int[3];}

    for(int i = 0; i < num_circles;i++){            // populate input_circles_array from data read from FILE
        input_circles >> input_circles_array[i][0] >> input_circles_array[i][1] >> input_circles_array[i][2];}
}

/*----------
 calculate_scale_factor(): based on all circles to be drawn, determine scale factor so all circles at full size fit within declared window size
 ------------*/
void calculate_scale_factor(void){
    int max_distance_from_origin = 0;
    int a = 0;
    int b = 0;
    for(int i = 0; i < num_circles;i++){
        a = abs(input_circles_array[i][0]) + input_circles_array[i][2];
        b = abs(input_circles_array[i][1]) + input_circles_array[i][2];
        if((a > b)&&(a > max_distance_from_origin)){
            max_distance_from_origin = a;
        }
        else if((b >= a)&&(b > max_distance_from_origin)){
            max_distance_from_origin = b;}}
    window_scale_factor = float(max_distance_from_origin) / (WINDOW_WIDTH/2);
}

/*---------------------------------------------------------------------
 display_fromuser(): This function is called once for _every_ frame.
 ---------------------------------------------------------------------*/
void display_fromuser(void)
{
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glColor3f(1.0, 0.84, 0);              /* draw in golden yellow */
    glPointSize(1.0);                     /* size of each point */
    
    glBegin(GL_POINTS);
    
    draw_circle(x-WINDOW_WIDTH/2,y-WINDOW_HEIGHT/2,r);
    
    glEnd();
    glFlush();                            /* render graphics */
    glutSwapBuffers();                    /* swap buffers */

}

/*---------------------------------------------------------------------
 display_fromfile(): This function is called once for _every_ frame.
 ---------------------------------------------------------------------*/
void display_fromfile(void)
{
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glColor3f(1.0, 0.84, 0);              /* draw in golden yellow */
    glPointSize(1.0);                     /* size of each point */
    
    glBegin(GL_POINTS);
    
    for (int i = 0; i<num_circles;i++){
        x = input_circles_array[i][0]/window_scale_factor;
        y = input_circles_array[i][1]/window_scale_factor;
        r = (input_circles_array[i][2]/window_scale_factor);
        draw_circle(x,y,r); }
    
    glEnd();
    glFlush();                            /* render graphics */
    glutSwapBuffers();                    /* swap buffers */

}

/*---------------------------------------------------------------------
display_fromfile_animate(): This function is called once for _every_ frame.
---------------------------------------------------------------------*/
void display_fromfile_animate(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glColor3f(1.0, 0.84, 0);              /* draw in golden yellow */
    glPointSize(1.0);                     /* size of each point */

    glBegin(GL_POINTS);

    for (int i = 0; i<num_circles;i++){
        x = input_circles_array[i][0]/window_scale_factor;
        y = input_circles_array[i][1]/window_scale_factor;
        r = ((input_circles_array[i][2]/window_scale_factor)*j)/K;
        draw_circle(x,y,r); }
    
    glEnd();
    glFlush();                            /* render graphics */
    glutSwapBuffers();                    /* swap buffers */

    j++;
    if (j > K){ j = 1;}
    
    glutPostRedisplay();
    
}

/*---------------------------------------------------------------------
myinit(): Set up attributes and viewing
---------------------------------------------------------------------*/
void myinit()
{
    glClearColor(0.0, 0.0, 0.92, 0.0);    /* blue background*/

    /* set up viewing */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT);
    glMatrixMode(GL_MODELVIEW);
}

/*---------------------------------------------------------------------
 draw_circle(): Set up attributes and viewing
 ---------------------------------------------------------------------*/
void draw_circle(int x, int y, int r) {
    float y_upperbound = y + (r/sqrt(2));
    int origin_x = x;
    int origin_y = y;
    int xi = r;
    int yi = 0;
    float d_old, d_new;
    
    circlePoint(xi, yi, origin_x, origin_y);
    
    d_new = 1 - r;
    while(yi + origin_y <= y_upperbound){
        if (d_new < 0) {        // CHOOSE N
            xi = xi;
            yi++;
            circlePoint(xi, yi, origin_x, origin_y);
            d_old = d_new;
            d_new = d_old + 2 * yi + 3;
        }
        else{                   // CHOOSE NW
            xi--;
            yi++;
            circlePoint(xi, yi, origin_x, origin_y);
            d_old = d_new;
            d_new = d_old + 2*yi - 2*xi + 5;
        }
    }
}

/*---------------------------------------------------------------------
 circlePoint(): Set up attributes and viewing
 ---------------------------------------------------------------------*/
void circlePoint(int x, int y, int origin_x, int origin_y) {

    int offset_x = origin_x + WINDOW_WIDTH/2;
    int offset_y = origin_y + WINDOW_HEIGHT/2;
    
    glVertex2i(x + offset_x, y + offset_y);
    glVertex2i(x + offset_x, -y + offset_y);
    glVertex2i(-x + offset_x, y + offset_y);
    glVertex2i(-x + offset_x, -y + offset_y);
    glVertex2i(y + offset_x, x + offset_y);
    glVertex2i(y + offset_x, -x + offset_y);
    glVertex2i(-y + offset_x, x + offset_y);
    glVertex2i(-y + offset_x, -x + offset_y);
}
