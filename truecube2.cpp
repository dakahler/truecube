/*  Description:    3-space program
	Last Modified:  05.23.99
	Author:         Dave Kahler      */

//  Current features:
//  1) Scaling
//  2) Translation
//  3) Rotation
//  4) Perspective Correction
//  5) Backface Culling
//  6) Solid Polygon Faces
//  7) Rotation of any shape object
//  8) Displays both triangles and quads
//  9) Reads .PLG files
// 10) Z-buffer

//  Current bugs:
//  1) .PLG reader doesn't support color importing (need new .??? importer!)

#include <glut.h>
#include <math.h>
#include <stdio.h>
#include <fstream.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include "mathfunc.h"
#include "globals.h"

#define  BFC_ON         // Calc BFC
#define  PCORRECT_ON    // Calc Perspective Correction
#define  ZBUFFER_ON     // Calc Z-buffer
//#define  SHADING_ON     // Calc Shading
#define  CLIP_ON        // Calc Minimal Clipping


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

// Clipping
void clippoly()
{
	int count;

	for (count=0;polys[count].stop==0;count++) {
		if (polys[count].vertices[0].x_per>640 || polys[count].vertices[0].x_per<0 ||
			polys[count].vertices[0].y_per>480 || polys[count].vertices[0].y_per<0 ||
			polys[count].vertices[1].x_per>640 || polys[count].vertices[1].x_per<0 ||
			polys[count].vertices[1].y_per>480 || polys[count].vertices[1].y_per<0 ||
			polys[count].vertices[2].x_per>640 || polys[count].vertices[2].x_per<0 ||
			polys[count].vertices[2].y_per>480 || polys[count].vertices[2].y_per<0)
			polys[count].clipped=1;
		else polys[count].clipped=0;
	}
}

// Shading
void shadepoly()
{
	/* Polygon shading code here */
}

// Z-buffer
void zbuffer()
{
	int v1t,v3t,count,vertex1,vertex2,vertex3,polycount,start_count_h,start_count_v,y_inc,x_l,x_r;
	double norm_i,norm_j,norm_k,slope_l,slope_r,ztemp;

	for (start_count_h=0,start_count_v=0;start_count_v<480;start_count_h++) {
		buffer[start_count_h][start_count_v].depth=0;
		if (start_count_h==639) {
			start_count_h=0;
			start_count_v++;
		}
	}
	for (polycount=0;renderpolys[polycount].stop==0;polycount++) {
		count=0;
		if (renderpolys[polycount].num_verts==3) {
			if (renderpolys[polycount].vertices[0].y_per>renderpolys[polycount].vertices[1].y_per && renderpolys[polycount].vertices[1].y_per>renderpolys[polycount].vertices[2].y_per) {
				vertex1=0;
				vertex2=1;
				vertex3=2;
			}
			if (renderpolys[polycount].vertices[1].y_per>renderpolys[polycount].vertices[0].y_per && renderpolys[polycount].vertices[0].y_per>renderpolys[polycount].vertices[2].y_per) {
				vertex1=1;
				vertex2=0;
				vertex3=2;
			}
			if (renderpolys[polycount].vertices[2].y_per>renderpolys[polycount].vertices[0].y_per && renderpolys[polycount].vertices[0].y_per>renderpolys[polycount].vertices[1].y_per) {
				vertex1=2;
				vertex2=0;
				vertex3=1;
			}
			if (renderpolys[polycount].vertices[0].y_per>renderpolys[polycount].vertices[2].y_per && renderpolys[polycount].vertices[2].y_per>renderpolys[polycount].vertices[1].y_per) {
				vertex1=0;
				vertex2=2;
				vertex3=1;
			}
			if (renderpolys[polycount].vertices[1].y_per>renderpolys[polycount].vertices[2].y_per && renderpolys[polycount].vertices[2].y_per>renderpolys[polycount].vertices[0].y_per) {
				vertex1=1;
				vertex2=2;
				vertex3=0;
			}
			if (renderpolys[polycount].vertices[2].y_per>renderpolys[polycount].vertices[1].y_per && renderpolys[polycount].vertices[1].y_per>renderpolys[polycount].vertices[0].y_per) {
				vertex1=2;
				vertex2=1;
				vertex3=0;
			}
			if (renderpolys[polycount].vertices[0].y_per>renderpolys[polycount].vertices[1].y_per && renderpolys[polycount].vertices[1].y_per==renderpolys[polycount].vertices[2].y_per) {
				vertex1=0;
				vertex2=1;
				vertex3=2;
			}
			if (renderpolys[polycount].vertices[1].y_per>renderpolys[polycount].vertices[0].y_per && renderpolys[polycount].vertices[0].y_per==renderpolys[polycount].vertices[2].y_per) {
				vertex1=1;
				vertex2=0;
				vertex3=2;
			}
			if (renderpolys[polycount].vertices[2].y_per>renderpolys[polycount].vertices[1].y_per && renderpolys[polycount].vertices[1].y_per==renderpolys[polycount].vertices[0].y_per) {
				vertex1=2;
				vertex2=0;
				vertex3=1;
			}
			if (renderpolys[polycount].vertices[0].y_per==renderpolys[polycount].vertices[1].y_per && renderpolys[polycount].vertices[0].y_per>renderpolys[polycount].vertices[2].y_per) {
				vertex1=2;
				vertex2=0;
				vertex3=1;
				count=1;
			}
			if (renderpolys[polycount].vertices[0].y_per==renderpolys[polycount].vertices[2].y_per && renderpolys[polycount].vertices[0].y_per>renderpolys[polycount].vertices[1].y_per) {
				vertex1=1;
				vertex2=0;
				vertex3=2;
				count=1;
			}
			if (renderpolys[polycount].vertices[1].y_per==renderpolys[polycount].vertices[2].y_per && renderpolys[polycount].vertices[1].y_per>renderpolys[polycount].vertices[0].y_per) {
				vertex1=0;
				vertex2=2;
				vertex3=1;
				count=1;
			}
			cross(renderpolys[polycount].vertices[0].x_per-renderpolys[polycount].vertices[1].x_per,
				  renderpolys[polycount].vertices[0].y_per-renderpolys[polycount].vertices[1].y_per,
				  renderpolys[polycount].vertices[0].z-renderpolys[polycount].vertices[1].z,
				  renderpolys[polycount].vertices[2].x_per-renderpolys[polycount].vertices[1].x_per,
				  renderpolys[polycount].vertices[2].y_per-renderpolys[polycount].vertices[1].y_per,
				  renderpolys[polycount].vertices[2].z-renderpolys[polycount].vertices[1].z,
				  &norm_i,&norm_j,&norm_k);
			while (count<2) {
				slope_l=(renderpolys[polycount].vertices[vertex1].y_per-renderpolys[polycount].vertices[vertex2].y_per)/
						(renderpolys[polycount].vertices[vertex1].x_per-renderpolys[polycount].vertices[vertex2].x_per);
				slope_r=(renderpolys[polycount].vertices[vertex1].y_per-renderpolys[polycount].vertices[vertex3].y_per)/
						(renderpolys[polycount].vertices[vertex1].x_per-renderpolys[polycount].vertices[vertex3].x_per);
				if (count==0)
					y_inc=floor(renderpolys[polycount].vertices[vertex1].y_per);
				else y_inc=ceil(renderpolys[polycount].vertices[vertex1].y_per);
				// X point solver
				while (1) {
					if (count==0) {
						if (y_inc<renderpolys[polycount].vertices[vertex2].y_per)
							break;
					}
					else {
						if (y_inc>renderpolys[polycount].vertices[vertex2].y_per)
							break;
					}
					x_l=((y_inc-renderpolys[polycount].vertices[vertex1].y_per)/slope_l)+renderpolys[polycount].vertices[vertex1].x_per;
					x_r=((y_inc-renderpolys[polycount].vertices[vertex1].y_per)/slope_r)+renderpolys[polycount].vertices[vertex1].x_per;
					if (x_l<x_r && x_l>0 && x_l<640 && y_inc>0 && y_inc<480) {
						while (x_l<x_r) {
							ztemp=-((norm_i*x_l)+(norm_j*y_inc)+(-((norm_i*renderpolys[polycount].vertices[1].x_per)+(norm_j*renderpolys[polycount].vertices[1].y_per)+(norm_k*(renderpolys[polycount].vertices[1].z+world_z)))))/(norm_k);
							if ((1/ztemp)>buffer[x_l][y_inc].depth) {
								buffer[x_l][y_inc].depth=(1/ztemp);
								buffer[x_l][y_inc].color_r=renderpolys[polycount].color_r;
								buffer[x_l][y_inc].color_g=renderpolys[polycount].color_g;
								buffer[x_l][y_inc].color_b=renderpolys[polycount].color_b;
							}
							x_l++;
						}
					}
					if (x_r<x_l && x_r>0 && x_r<640 && y_inc>0 && y_inc<480) {
						while (x_r<x_l) {
							ztemp=-((norm_i*x_r)+(norm_j*y_inc)+(-((norm_i*renderpolys[polycount].vertices[1].x_per)+(norm_j*renderpolys[polycount].vertices[1].y_per)+(norm_k*(renderpolys[polycount].vertices[1].z+world_z)))))/(norm_k);
							if ((1/ztemp)>buffer[x_r][y_inc].depth) {
								buffer[x_r][y_inc].depth=(1/ztemp);
								buffer[x_r][y_inc].color_r=renderpolys[polycount].color_r;
								buffer[x_r][y_inc].color_g=renderpolys[polycount].color_g;
								buffer[x_r][y_inc].color_b=renderpolys[polycount].color_b;
							}
							x_r++;
						}
					}
					if (count==0) {
						if (y_inc>=renderpolys[polycount].vertices[vertex2].y_per)
							y_inc--;
						else break;
					}
					else {
						if (y_inc<=renderpolys[polycount].vertices[vertex2].y_per)
							y_inc++;
						else break;
					}
				}
				v1t=vertex1;
				v3t=vertex3;
				
				vertex1=v3t;
				vertex3=v1t;

				count++;

				if (renderpolys[polycount].vertices[vertex2].y_per==renderpolys[polycount].vertices[vertex3].y_per)
					break;
			}
		}
	}
}

// Import a .PLG file
int plg_import(char filename[20])
{
	ifstream file_ptr;

	char string[50],name[50];
	int count,vertnum,faces,tempint,vert1,vert2,vert3,vert4;

	typedef struct verttype {
		int x,y,z;
	} vert;
	vert vertlist[2000];

	srand((unsigned)time(NULL));

	file_ptr.open(filename,ios::in);
	if (!file_ptr) {
		printf("File Error");
		return(0);
	}

	file_ptr.getline(string,80);

	sscanf(string,"%s %d %d",name,&vertnum,&faces);

	polys[faces+1].stop=1;

	for (count=0;count<vertnum;count++) {
		file_ptr.getline(string,80);
		sscanf(string,"%d %d %d",&vertlist[count].x,&vertlist[count].y,&vertlist[count].z);
	}
	for (count=0;count<faces;count++) {
		file_ptr.getline(string,80);
		sscanf(string,"%d %d",&tempint,&polys[count].num_verts);
		if (polys[count].num_verts==3) {
			sscanf(string,"%d %d %d %d %d",&tempint,&polys[count].num_verts,&vert1,&vert2,&vert3);
			polys[count].vertices[0].x=vertlist[vert1].x;
			polys[count].vertices[0].y=vertlist[vert1].y;
			polys[count].vertices[0].z=vertlist[vert1].z;
			polys[count].vertices[1].x=vertlist[vert2].x;
			polys[count].vertices[1].y=vertlist[vert2].y;
			polys[count].vertices[1].z=vertlist[vert2].z;
			polys[count].vertices[2].x=vertlist[vert3].x;
			polys[count].vertices[2].y=vertlist[vert3].y;
			polys[count].vertices[2].z=vertlist[vert3].z;
		}
		if (polys[count].num_verts==4) {
			sscanf(string,"%d %d %d %d %d %d",&tempint,&polys[count].num_verts,&vert1,&vert2,&vert3,&vert4);
			polys[count].vertices[0].x=vertlist[vert1].x;
			polys[count].vertices[0].y=vertlist[vert1].y;
			polys[count].vertices[0].z=vertlist[vert1].z;
			polys[count].vertices[1].x=vertlist[vert2].x;
			polys[count].vertices[1].y=vertlist[vert2].y;
			polys[count].vertices[1].z=vertlist[vert2].z;
			polys[count].vertices[2].x=vertlist[vert3].x;
			polys[count].vertices[2].y=vertlist[vert3].y;
			polys[count].vertices[2].z=vertlist[vert3].z;
			polys[count].vertices[3].x=vertlist[vert4].x;
			polys[count].vertices[3].y=vertlist[vert4].y;
			polys[count].vertices[3].z=vertlist[vert4].z;
		}
		polys[count].color_r=rand()%256;
		polys[count].color_g=rand()%256;
		polys[count].color_b=rand()%256;
	}
	return(1);
}

// Polygon Display Loop
void polygon_display()
{
	int count_h,count_v;

	glClear(GL_COLOR_BUFFER_BIT);

	for (count_h=1,count_v=1;count_v<480;count_h++) {
		/*if (renderpolys[count].num_verts==4) {
			glBegin(GL_QUADS);
			 glVertex2f(renderpolys[count].vertices[0].x_per,renderpolys[count].vertices[0].y_per);
			 glVertex2f(renderpolys[count].vertices[1].x_per,renderpolys[count].vertices[1].y_per);
			 glVertex2f(renderpolys[count].vertices[2].x_per,renderpolys[count].vertices[2].y_per);
			 glVertex2f(renderpolys[count].vertices[3].x_per,renderpolys[count].vertices[3].y_per);
			glEnd();
		}
		if (renderpolys[count].num_verts==3) {
			glBegin(GL_TRIANGLES);
			 glVertex2f(renderpolys[count].vertices[0].x_per,renderpolys[count].vertices[0].y_per);
			 glVertex2f(renderpolys[count].vertices[1].x_per,renderpolys[count].vertices[1].y_per);
			 glVertex2f(renderpolys[count].vertices[2].x_per,renderpolys[count].vertices[2].y_per);
			glEnd();
		}*/
		if (buffer[count_h][count_v].depth!=0) {
			glColor3ub(buffer[count_h][count_v].color_r,buffer[count_h][count_v].color_g,buffer[count_h][count_v].color_b);
			glBegin(GL_POINTS);
			 glVertex2f(count_h,count_v);
			glEnd();
		}
		if (count_h==639) {
			count_h=1;
			count_v++;
		}
	}
	glutSwapBuffers();
}

// Generate Master Polygon Display List
void generate_master()
{
	int count1,count2=0,count3;

	for (count1=0;polys[count1].stop!=1;count1++) {
		renderpolys[count2].stop=0;
		if (polys[count1].backface!=1 && polys[count1].clipped!=1) {
			renderpolys[count2].color_r=polys[count1].color_r;
			renderpolys[count2].color_g=polys[count1].color_g;
			renderpolys[count2].color_b=polys[count1].color_b;
			renderpolys[count2].num_verts=polys[count1].num_verts;
			for (count3=0;count3<renderpolys[count2].num_verts;count3++) {
				renderpolys[count2].vertices[count3].x_per=polys[count1].vertices[count3].x_per;
				renderpolys[count2].vertices[count3].y_per=polys[count1].vertices[count3].y_per;
				renderpolys[count2].vertices[count3].x=polys[count1].vertices[count3].x;
				renderpolys[count2].vertices[count3].y=polys[count1].vertices[count3].y;
				renderpolys[count2].vertices[count3].z=polys[count1].vertices[count3].z;
			}
			count2++;
		}
	}
	renderpolys[count2].stop=1;
}

// Backface Culling
void BFC()
{
	int count;
	double norm_i=0,norm_j=0,norm_k=0;

	for (count=0;polys[count].stop!=1;count++) {
		if (polys[count].num_verts==4) {
			cross((polys[count].vertices[2].x-polys[count].vertices[1].x),
				  (polys[count].vertices[2].y-polys[count].vertices[1].y),
				  (polys[count].vertices[2].z-polys[count].vertices[1].z),
				  (polys[count].vertices[0].x-polys[count].vertices[1].x),
				  (polys[count].vertices[0].y-polys[count].vertices[1].y),
				  (polys[count].vertices[0].z-polys[count].vertices[1].z),
				   &norm_i,&norm_j,&norm_k);
		}
		if (polys[count].num_verts==3) {
			cross((polys[count].vertices[0].x-polys[count].vertices[1].x),
				  (polys[count].vertices[0].y-polys[count].vertices[1].y),
				  (polys[count].vertices[0].z-polys[count].vertices[1].z),
				  (polys[count].vertices[2].x-polys[count].vertices[1].x),
				  (polys[count].vertices[2].y-polys[count].vertices[1].y),
				  (polys[count].vertices[2].z-polys[count].vertices[1].z),
				   &norm_i,&norm_j,&norm_k);
		}
		polys[count].dot=dot(norm_i,norm_j,norm_k,
			world_x-(polys[count].vertices[1].x+world_x),
			world_y-(polys[count].vertices[1].y+world_y),
			-(polys[count].vertices[1].z+world_z));
		if (polys[count].dot<0) {
			polys[count].backface=1;
		}
		else
			polys[count].backface=0;
	}
}

// Perspective Correction
void pcorrect()
{
	int count1,count2;

	for (count1=0;polys[count1].stop!=1;count1++) {
		for (count2=0;count2<polys[count1].num_verts;count2++) {
			polys[count1].vertices[count2].x_per=((310*polys[count1].vertices[count2].x)/(polys[count1].vertices[count2].z+world_z))+world_x;
			polys[count1].vertices[count2].y_per=((310*polys[count1].vertices[count2].y)/(polys[count1].vertices[count2].z+world_z))+world_y;
		}
	}
}

// Scaling
void scale(double factor)
{
	int count1,count2;

	for (count1=0;polys[count1].stop!=1;count1++) {
		for (count2=0;count2<polys[count1].num_verts;count2++) {
			polys[count1].vertices[count2].x*=factor;
			polys[count1].vertices[count2].y*=factor;
			polys[count1].vertices[count2].z*=factor;
		}
	}
}

// Translate X
void trl_x(int dir)
{
	world_x+=dir;
}

// Translate Y
void trl_y(int dir)
{
	world_y+=dir;
}

// Translate Z
void trl_z(int dir)
{
	world_z+=dir;
}

// Rotate parallel to X
void rot_x(int dir)
{
	int count1,count2;

	for (count1=0;polys[count1].stop!=1;count1++) {
		for (count2=0;count2<polys[count1].num_verts;count2++) {
			polys[count1].vertices[count2].y_temp=(polys[count1].vertices[count2].y*(dcos(dir)))-(polys[count1].vertices[count2].z*(dsin(dir)));
			polys[count1].vertices[count2].z=(polys[count1].vertices[count2].y*(dsin(dir)))+(polys[count1].vertices[count2].z*(dcos(dir)));
			polys[count1].vertices[count2].y=polys[count1].vertices[count2].y_temp;
		}
	}
}

// Rotate parallel to Y
void rot_y(int dir)
{
	int count1,count2;

	for (count1=0;polys[count1].stop!=1;count1++) {
		for (count2=0;count2<polys[count1].num_verts;count2++) {
			polys[count1].vertices[count2].x_temp=(polys[count1].vertices[count2].x*(dcos(dir)))+(polys[count1].vertices[count2].z*(dsin(dir)));
			polys[count1].vertices[count2].z=(-polys[count1].vertices[count2].x*(dsin(dir)))+(polys[count1].vertices[count2].z*(dcos(dir)));
			polys[count1].vertices[count2].x=polys[count1].vertices[count2].x_temp;   //copy data from temp to real
		}
	}
}

// Rotate parallel to Z
void rot_z(int dir)
{
	int count1,count2;

	for (count1=0;polys[count1].stop!=1;count1++) {
		for (count2=0;count2<polys[count1].num_verts;count2++) {
			polys[count1].vertices[count2].x_temp=(polys[count1].vertices[count2].x*(dcos(dir)))-(polys[count1].vertices[count2].y*(dsin(dir)));
			polys[count1].vertices[count2].y=(polys[count1].vertices[count2].x*(dsin(dir)))+(polys[count1].vertices[count2].y*(dcos(dir)));
			polys[count1].vertices[count2].x=polys[count1].vertices[count2].x_temp;
		}
	}	
}

// Detect input type on keypress, call funcs
void keyboard(unsigned char keypress,int w,int h)
{

	if (keypress==',')
		rot_y(1);
	if (keypress=='.')
		rot_y(-1);
	if (keypress=='o')
		rot_x(1);
	if (keypress=='l')
		rot_x(-1);
	if (keypress=='[')
		rot_z(1);
	if (keypress==']')
		rot_z(-1);
	if (keypress=='a')
		trl_x(-1);
	if (keypress=='d')
		trl_x(1);
	if (keypress=='w')
		trl_y(1);
	if (keypress=='s')
		trl_y(-1);
	if (keypress=='r')
		trl_z(1);
	if (keypress=='f')
		trl_z(-1);
	if (keypress=='z')
		scale(.9);
	if (keypress=='x')
		scale(1.1);

#ifdef PCORRECT_ON
	pcorrect();
#endif

#ifdef BFC_ON
	BFC();
#endif

#ifdef CLIP_ON
	clippoly();
#endif

	generate_master();

#ifdef ZBUFFER_ON
	zbuffer();
#endif

#ifdef SHADING_ON
	shadepoly();
#endif

	//printf("A la KB");

	glutPostRedisplay();

}
 
// Polygon Display
void display(void) 
{ 
	polygon_display();   // Display polygons
} 

int main(int argc,char **argv) 
{
	if (!plg_import("Spider.plg"))
		return(1);

	printf("Welcome to DK3D!\n");

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