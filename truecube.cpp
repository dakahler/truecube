/*  Description:    3-space program
	Last Modified:  06.08.99
	Author:         Dave Kahler      */

//  Current features:
//  1) Scaling
//  2) Translation
//  3) Rotation
//  4) Perspective Correction
//  5) Backface Culling
//  6) Solid Polygon Faces
//  7) Rotation of any shape object
//  8) Imports both triangles and quads
//  9) Reads .PLG files
// 10) Z-buffer
// 11) Linked Lists
// 12) World Object System??

//  To do:
//  1) .PLG reader doesn't support color importing (need new .??? importer!)
//  2) Needs to be optimized!
//  3) world->cam doesn't quite work (probably because I don't quite
//     understand how it works myself ;)

#include <stdlib.h>
#include "glut.h"
#include <math.h>
#include <stdio.h>
#include <fstream>
#include <time.h>
#include <conio.h>
#include <iostream>

using namespace std;

#include "mathfunc.h"
#include "globals.h"
#include "ObjectClass.h"



// define an object
ObjectClass object(0,0,50);
//ObjectClass object2(0,0,-50);

// Initialize Linked Lists
void initLLr(void)
{
	renderstart=new disppoly;
	renderend=renderstart;
}

// Debug File Generator
void debugToFile(double value)
{
	ofstream file_ptr;

	file_ptr.open("debug.dat",ios::app);
	file_ptr << value << '\n';
	file_ptr.close();
}

void debugToFile(int value)
{
	ofstream file_ptr;

	file_ptr.open("debug.dat",ios::app);
	file_ptr << value << '\n';
	file_ptr.close();
}

// Z-buffer
void zbuffer()
{
	int v1t,v3t,count,vertex1,vertex2,vertex3,start_count_h,start_count_v,y_inc,x_l,x_r;
	double norm_i,norm_j,norm_k,slope_l,slope_r,ztemp;

	for (start_count_h=0,start_count_v=0;start_count_v<480;start_count_h++) {
		buffer[start_count_h][start_count_v].depth=-1;
		if (start_count_h==639) {
			start_count_h=0;
			start_count_v++;
		}
	}
	for (disppoly *trav=renderstart->next;trav!=NULL;) {
		count=0;
		if (trav->vertices[0].y_per>trav->vertices[1].y_per && trav->vertices[1].y_per>trav->vertices[2].y_per) {
			vertex1=0;
			vertex2=1;
			vertex3=2;
		}
		if (trav->vertices[1].y_per>trav->vertices[0].y_per && trav->vertices[0].y_per>trav->vertices[2].y_per) {
			vertex1=1;
			vertex2=0;
			vertex3=2;
		}
		if (trav->vertices[2].y_per>trav->vertices[0].y_per && trav->vertices[0].y_per>trav->vertices[1].y_per) {
			vertex1=2;
			vertex2=0;
			vertex3=1;
		}
		if (trav->vertices[0].y_per>trav->vertices[2].y_per && trav->vertices[2].y_per>trav->vertices[1].y_per) {
			vertex1=0;
			vertex2=2;
			vertex3=1;
		}
		if (trav->vertices[1].y_per>trav->vertices[2].y_per && trav->vertices[2].y_per>trav->vertices[0].y_per) {
			vertex1=1;
			vertex2=2;
			vertex3=0;
		}
		if (trav->vertices[2].y_per>trav->vertices[1].y_per && trav->vertices[1].y_per>trav->vertices[0].y_per) {
			vertex1=2;
			vertex2=1;
			vertex3=0;
		}
		if (trav->vertices[0].y_per>trav->vertices[1].y_per && trav->vertices[1].y_per==trav->vertices[2].y_per) {
			vertex1=0;
			vertex2=1;
			vertex3=2;
		}
		if (trav->vertices[1].y_per>trav->vertices[0].y_per && trav->vertices[0].y_per==trav->vertices[2].y_per) {
			vertex1=1;
			vertex2=0;
			vertex3=2;
		}
		if (trav->vertices[2].y_per>trav->vertices[1].y_per && trav->vertices[1].y_per==trav->vertices[0].y_per) {
			vertex1=2;
			vertex2=0;
			vertex3=1;
		}
		if (trav->vertices[0].y_per==trav->vertices[1].y_per && trav->vertices[0].y_per>trav->vertices[2].y_per) {
			vertex1=2;
			vertex2=0;
			vertex3=1;
			count=1;
		}
		if (trav->vertices[0].y_per==trav->vertices[2].y_per && trav->vertices[0].y_per>trav->vertices[1].y_per) {
			vertex1=1;
			vertex2=0;
			vertex3=2;
			count=1;
		}
		if (trav->vertices[1].y_per==trav->vertices[2].y_per && trav->vertices[1].y_per>trav->vertices[0].y_per) {
			vertex1=0;
			vertex2=2;
			vertex3=1;
			count=1;
		}
		cross(trav->vertices[0].x_per-trav->vertices[1].x_per,
			  trav->vertices[0].y_per-trav->vertices[1].y_per,
			  trav->vertices[0].z-trav->vertices[1].z,
			  trav->vertices[2].x_per-trav->vertices[1].x_per,
			  trav->vertices[2].y_per-trav->vertices[1].y_per,
			  trav->vertices[2].z-trav->vertices[1].z,
			  &norm_i,&norm_j,&norm_k);
		while (count<2) {
			slope_l=(trav->vertices[vertex1].y_per-trav->vertices[vertex2].y_per)/
					(trav->vertices[vertex1].x_per-trav->vertices[vertex2].x_per);
			slope_r=(trav->vertices[vertex1].y_per-trav->vertices[vertex3].y_per)/
					(trav->vertices[vertex1].x_per-trav->vertices[vertex3].x_per);
			if (count==0)
				y_inc=floor(trav->vertices[vertex1].y_per);
			else y_inc=ceil(trav->vertices[vertex1].y_per);
			// X point solver
			while (1) {
				if (count==0) {
					if (y_inc<trav->vertices[vertex2].y_per)
						break;
				}
				else {
					if (y_inc>trav->vertices[vertex2].y_per)
						break;
				}
				x_l=((y_inc-trav->vertices[vertex1].y_per)/slope_l)+trav->vertices[vertex1].x_per;
				x_r=((y_inc-trav->vertices[vertex1].y_per)/slope_r)+trav->vertices[vertex1].x_per;
				if (x_l<x_r && x_l>0 && x_l<640 && y_inc>0 && y_inc<480) {
					while (x_l<x_r) {
						ztemp=-((norm_i*x_l)+(norm_j*y_inc)+(-((norm_i*trav->vertices[1].x_per)+(norm_j*trav->vertices[1].y_per)+(norm_k*(trav->vertices[1].z)))))/(norm_k);
						if ((1/ztemp)>buffer[x_l][y_inc].depth) {
							buffer[x_l][y_inc].depth=(1/ztemp);
							buffer[x_l][y_inc].color_r=trav->color_r;
							buffer[x_l][y_inc].color_g=trav->color_g;
							buffer[x_l][y_inc].color_b=trav->color_b;
						}
						x_l++;
					}
				}
				if (x_r<x_l && x_r>0 && x_r<640 && y_inc>0 && y_inc<480) {
					while (x_r<x_l) {
						ztemp=-((norm_i*x_r)+(norm_j*y_inc)+(-((norm_i*trav->vertices[1].x_per)+(norm_j*trav->vertices[1].y_per)+(norm_k*(trav->vertices[1].z)))))/(norm_k);
						if ((1/ztemp)>buffer[x_r][y_inc].depth) {
							buffer[x_r][y_inc].depth=(1/ztemp);
							buffer[x_r][y_inc].color_r=trav->color_r;
							buffer[x_r][y_inc].color_g=trav->color_g;
							buffer[x_r][y_inc].color_b=trav->color_b;
						}
						x_r++;
					}
				}
				if (count==0) {
					if (y_inc>=trav->vertices[vertex2].y_per)
						y_inc--;
					else break;
				}
				else {
					if (y_inc<=trav->vertices[vertex2].y_per)
						y_inc++;
					else break;
				}
			}
			v1t=vertex1;
			v3t=vertex3;
			
			vertex1=v3t;
			vertex3=v1t;

			count++;

			if (trav->vertices[vertex2].y_per==trav->vertices[vertex3].y_per)
				break;
		}
		trav=trav->next;
	}
}

// Detect input type on keypress, call funcs
void keyboard(unsigned char keypress,int w,int h)
{
	static int cam_x=0,cam_y=0,cam_z=0,cam_rot_x=0,cam_rot_y=0,cam_rot_z=0;

	if (keypress=='a')
		cam_x-=20;
	if (keypress=='d')
		cam_x+=20;
	if (keypress=='w')
		cam_z+=20;
	if (keypress=='s')
		cam_z-=20;
	if (keypress==',')
		cam_rot_y-=20;
	if (keypress=='.')
		cam_rot_y+=20;

	object.setvars();
	object.world_trans();
	object.create_world_to_cam(cam_x,cam_y,cam_z,cam_rot_x,cam_rot_y,cam_rot_z);
	object.world_to_cam();
	object.pcorrect();
	object.BFC();
	object.addscreen();
	object.clippoly();
	object.generate_master();

	/*object2.setvars();
	object2.world_trans();
	object2.cam_translate(cam_x,cam_y,cam_z);
	object2.pcorrect();
	object2.cam_rot_x(-cam_rot_x);
	object2.cam_rot_y(-cam_rot_y);
	object2.cam_rot_z(-cam_rot_z);
	object2.BFC();
	object2.addscreen();
	object2.clippoly();
	object2.generate_master();*/

	zbuffer();

	glutPostRedisplay();

	// reset disppoly list
	if (renderstart!=renderend) {
		disppoly *del=renderstart->next,*tmp;
		while (del->next!=NULL) {
			tmp=del;
			del=del->next;
			delete tmp;
		}
		renderstart->next=del;
		renderend=renderstart;
	}
}
 
// Polygon Display
void display(void) 
{ 
	int count_h,count_v;

	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_POINTS);
	for (count_h=1,count_v=1;count_v<480;count_h++) {
		if (buffer[count_h][count_v].depth!=-1) {
			 glColor3ub(buffer[count_h][count_v].color_r,buffer[count_h][count_v].color_g,buffer[count_h][count_v].color_b);
			 glVertex2f(count_h,count_v);
		}
		if (count_h==639) {
			count_h=1;
			count_v++;
		}
	}
	glEnd();
	glutSwapBuffers();
} 

int main(int argc,char **argv) 
{
	initLLr();
	object.initLLo();
	//object2.initLLo();

	if (!object.plg_import("SPIDER.PLG"))
		return(1);

	//if (!object2.plg_import("diamond.plg"))
		//return(1);

	printf("Welcome to DK3D!\n");

	// Init OGL+GLUT
	glutInit(&argc,argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);

	glutInitWindowPosition(10,20);
	glutInitWindowSize(640,480);
	glutCreateWindow ("3D Object Transformations");

	glClearColor(256,256,256,0);
	glOrtho(0,640,0,480,-1,1);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return(0); 
}