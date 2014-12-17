//
//  barrels.c
//  maze2_barrel
//
//  Created by Ben Kerestan on 12/7/14.
//  Copyright (c) 2014 Ben Kerestan. All rights reserved.
//

#include <stdio.h>
#include <math.h>
#include "barrels.h"

#define M_PI 3.14

void make_barrel(float height, int sides) {
    int rad=1.3;
    float theta = 0.0f;
    float delta_theta = (2*M_PI)/30;
    float theta_next;
    
    float angletest;
    float epsilon = 0.0000000000001f;
    
    //glLightModelf(GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR);
    
    glBegin(GL_TRIANGLE_STRIP);
    for (angletest=0.0f; angletest<=1.0f+epsilon; angletest+=1.0f/30) {
        float rads = angletest*M_PI*2.0f;
        float cosr = cos(rads);
        float sinr = sin(rads);
        
        glTexCoord2f(angletest, 0.0f);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50);
        glNormal3f(cosr,0.0f,sinr);
        glVertex3f(cosr*rad, 0.0f, sinr*rad);
        //glTexCoord2f((angletest+1/sides), 0.0f);
        //glVertex3f(cos_next, height*-0.5f, sin_next);
        glTexCoord2f(angletest, 0.66f);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50);
        glNormal3f(cosr, 3, sinr);
        glVertex3f(cosr*rad, 3, sinr*rad);
        
        /*
         glTexCoord2f(angletest+1/sides, 0.0f);
         glVertex3f(cos_next, height*-0.5f, sin_next);
         glTexCoord2f((angletest+1/sides), 0.66f);
         glVertex3f(cos_next, height*0.5f, sin_next);
         glTexCoord2f(angletest, 0.66f);
         glVertex3f(cosr, height*0.5, sinr);
         */
    }
    glTexCoord2f(angletest, 0.0f);
    glNormal3f(cos(0.0f), 0.0f, sin(0.0f));
    glVertex3f(cos(0.0f)*rad, 0.0f, sin(0.0f)*rad);
    glTexCoord2f(angletest, 0.66f);
    glNormal3f(cos(0.0f), 3, sin(0.0f));
    glVertex3f(cos(0.0f)*rad, 3, sin(0.0f)*rad);
    
    glEnd();
    
   // glLightModelf(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);

    
    glBegin(GL_TRIANGLE_STRIP);
    for (angletest=0.0f; angletest<=1.0f+epsilon; angletest+=1.0f/30) {
        float rads = angletest*M_PI*2.0f;
        float cosr = cos(rads);
        float sinr = sin(rads);
        
        glTexCoord2f(0.165+0.165*cosr, 0.825+0.165*sinr);
        glVertex3f(cosr*rad,3,sinr*rad);
        glTexCoord2f(0.165, 0.825);
        glVertex3f(0.0f,3.0f,0.0f);
    }
    glEnd();
    
}