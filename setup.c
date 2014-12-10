#include "setup.h"

void my_setup(void) {
	glShadeModel(GL_FLAT);
    lighting_setup();
    return;
}

void my_timer(int val) {
    glutPostRedisplay() ;
    return;
}

/* This function sets up the initial states of OpenGL related enivornment */
void gl_setup(void) {
    
    /* specifies a background color: black in this case */
    glClearColor(0,0,0,0) ;
    
    glEnable(GL_DEPTH_TEST);
    
    /* NEW: setup for 3d projection */
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( 80.0, 1.0, 1.0, 320.0);
    glMatrixMode(GL_MODELVIEW) ;
    glLoadIdentity();
    glShadeModel(GL_SMOOTH);
    texture_setup();
    return;
}



void my_reshape(int w, int h) {
	
    /* define viewport -- x, y, (origin is at lower left corner) width, height */
    glViewport (0, 0, min(w,h), min(w,h));
    return;
}

void lighting_setup () {
    
    GLfloat light0_amb[]      = {0, 0, 0, 1};
    GLfloat light0_diffuse[]  = {0, 1, 0, 1};
    GLfloat light0_specular[] = {0, 1, 0, 1};
    GLfloat light0_exponent = 0;
	GLfloat globalAmb[]       = {0, 0, 0, 1};
    //GLfloat globalAmb[]       = {0.005, 0.005, 0.005, 1};

    GLfloat mat_shininess[] = { 50.0 };
	GLfloat mat_amb_diff[]  = {.6,.6,.6,1};
	GLfloat mat_specular[]  = {0.6, 0.6, 0.6,1};
	GLfloat mat_high_shininess[] = {100};
	GLfloat mat_emission[]  = {0,0,0, 0};

    //glShadeModel (GL_SMOOTH);
    //glLightfv(GL_LIGHT0,GL_SPECULAR,mat_specular);
    /*glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);
	 glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_high_shininess);*/

    // Definig spotlight attributes
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_amb);
	glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,80);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 1);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
    //glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 1.0f);
    //glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 1.0f);


    //enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
	//glEnable(GL_DEPTH_TEST);

    
    // reflective properites -- global ambiant light
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmb);
    
}

void texture_setup() {
    // set pixel storage mode
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    // load images from files
    //  load_bmp("brick.bmp",  wall_img, smallWidth, &tex_names[WALLIMG_TEX]) ;
    // load_bmp("soup.bmp",    soup_img,  smallWidth, &tex_names[SOUPIMG_TEX]) ;
}


