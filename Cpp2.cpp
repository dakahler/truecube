#include <fstream.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void plg_import(char filename[20])
{
	ifstream file_ptr;
	char string[50],temp[50];
	int count,vertnum,faces,tempint,vert1,vert2,vert3,vert4;

	typedef struct verttype {
		int x,y,z;
	} vert;
	vert vertlist[100];

	srand( (unsigned)time( NULL ) );

	file_ptr.open(filename,ios::in);

	file_ptr.getline(string,80);

	sscanf(string,"%s %d %d",temp,&vertnum,&faces);

	polys[faces+1].stop=1;

	for (count=1;count<=vertnum;count++) {
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
}

void main()
{
	plg_import("test.txt");
}