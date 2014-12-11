#include "setup.h"

void my_setup(void) {
	glShadeModel(GL_SMOOTH);
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
    GLfloat light0_diffuse[]  = {1, 1, 1, 1};
    GLfloat light0_specular[] = {1, 1, 1, 1};
    GLfloat light0_exponent = 0;
	GLfloat globalAmb[]       = {0.3, 0.3, 0.3, 1};
    //GLfloat globalAmb[]       = {0.005, 0.005, 0.005, 1};

    GLfloat mat_shininess[] = { 50.0 };
	GLfloat mat_amb_diff[]  = {0.6,0.6,0.6,1};
	GLfloat mat_specular[]  = {0.6, 0.6, 0.6,1};
	GLfloat mat_high_shininess[] = {100};
	GLfloat mat_emission[]  = {0,0,0,0};

    //enable lighting
    
    glShadeModel (GL_SMOOTH);
    //glLightfv(GL_LIGHT0,GL_SPECULAR,mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_high_shininess);

    // Definig spotlight attributes
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_amb);
	glLightf(GL_LIGHT0,GL_SPOT_CUTOFF, 40);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 1);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 2.0f);
    //glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 1.0f);
    //glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 1.0f);

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);     //two sided lighting
    //enable lighting
    glEnable(GL_LIGHTING);
  //  glEnable(GL_NORMALIZE);
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
      load_bmp("brick.bmp",  wall_img, smallWidth, &tex_names[WALLIMG_TEX]) ;
    // load_bmp("soup.bmp",    soup_img,  smallWidth, &tex_names[SOUPIMG_TEX]) ;
}

void bmp2rgb(GLubyte img[], int size) {
    int i;
    GLubyte temp;
    
    for (i=0; i<size; i+=3) {
        temp = img[i+2];
        img[i+2] = img[i+1];
        img[i+1] = temp;
        
    }
}

void load_bmp(char *fname, GLubyte img[], int size, GLuint *ptname) {
    FILE *fp;
    
    fp = fopen(fname,"rb") ;
    if(fp == NULL) {
        fprintf(stderr,"unable to open texture file %s\n", fname) ;
        exit(1) ;
    }
    
    fseek(fp,8,SEEK_SET) ;
    fread(img,size*size*3,1,fp) ;
    bmp2rgb(img, size*size*3);
    fclose(fp) ;
    
    if (ptname) {
        // initialize the texture
        glGenTextures(1, ptname) ;
        glBindTexture(GL_TEXTURE_2D,*ptname);
        
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT) ;
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT) ;
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR) ;
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR) ;
        
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,
                     size,size,
                     0,GL_RGB,GL_UNSIGNED_BYTE,img) ;
    }
}
