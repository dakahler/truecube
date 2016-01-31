class ObjectClass {
public:
	ObjectClass();
	ObjectClass(int w_x,int w_y,int w_z);
	void cam_rot_x(int rot);
	void cam_rot_y(int rot);
	void cam_rot_z(int rot);
	void cam_translate(int x,int y,int z);
	void world_trans(void);
	void clippoly(void);
	int plg_import(char filename[20]);
	void generate_master(void);
	void BFC(void);
	void pcorrect(void);

private:
	int world_x,world_y,world_z,obj_cam_x,obj_cam_y,obj_cam_z;
	poly *polystart,*polyend;
};

ObjectClass::ObjectClass() {}

ObjectClass::ObjectClass(int w_x,int w_y,int w_z):
world_x(w_x),
world_y(w_y),
world_z(w_z)
{
	obj_cam_x=world_x;
	obj_cam_y=world_y;
	obj_cam_z=world_z;
}

void ObjectClass::cam_rot_x(int rot)
{
	int count2;

	for (poly *trav=polystart->next;trav!=NULL;) {
		for (count2=0;count2<3;count2++) {
			trav->vertices[count2].y_rot=(trav->vertices[count2].y*(dcos(rot)))-(trav->vertices[count2].z*(dsin(rot)));
			trav->vertices[count2].z_rot=(trav->vertices[count2].y*(dsin(rot)))+(trav->vertices[count2].z*(dcos(rot)));
		}
		trav=trav->next;
	}
}

void ObjectClass::cam_rot_y(int rot)
{
	int count2;

	for (poly *trav=polystart->next;trav!=NULL;) {
		for (count2=0;count2<3;count2++) {
			trav->vertices[count2].x_rot=(trav->vertices[count2].x*(dcos(rot)))+(trav->vertices[count2].z*(dsin(rot)));
			trav->vertices[count2].z_rot=(-trav->vertices[count2].x*(dsin(rot)))+(trav->vertices[count2].z*(dcos(rot)));
		}
		trav=trav->next;
	}
}

void ObjectClass::cam_rot_z(int rot)
{
	int count2;

	for (poly *trav=polystart->next;trav!=NULL;) {
		for (count2=0;count2<3;count2++) {
			trav->vertices[count2].x_rot=(trav->vertices[count2].x*(dcos(rot)))-(trav->vertices[count2].y*(dsin(rot)));
			trav->vertices[count2].y_rot=(trav->vertices[count2].x*(dsin(rot)))+(trav->vertices[count2].y*(dcos(rot)));
		}
		trav=trav->next;
	}
}

void ObjectClass::cam_translate(int x,int y,int z)
{
	obj_cam_x=world_x;
	obj_cam_y=world_y;
	obj_cam_z=world_z;

	obj_cam_x-=x;
	obj_cam_y-=y;
	obj_cam_z-=z;
}

void ObjectClass::world_trans()
{
	for (poly *trav=polystart->next;trav!=NULL;) {
		for (int count=0;count<3;count++) {
			trav->vertices[count].x+=world_x;
			trav->vertices[count].y+=world_y;
			trav->vertices[count].z+=world_z;
		}
		trav=trav->next;
	}
}

void ObjectClass::clippoly()
{
	for (poly *trav=polystart;trav!=NULL;) {
		if (trav->vertices[0].x_per>640 || trav->vertices[0].x_per<0 ||
			trav->vertices[0].y_per>480 || trav->vertices[0].y_per<0 ||
			trav->vertices[1].x_per>640 || trav->vertices[1].x_per<0 ||
			trav->vertices[1].y_per>480 || trav->vertices[1].y_per<0 ||
			trav->vertices[2].x_per>640 || trav->vertices[2].x_per<0 ||
			trav->vertices[2].y_per>480 || trav->vertices[2].y_per<0)
			trav->clipped=1;
		else trav->clipped=0;

		trav=trav->next;
	}
}

void ObjectClass::generate_master()
{
	int count3;

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
	for (poly *trav=polystart->next;trav!=NULL;) {
		disppoly *make=new disppoly;
		if (trav->backface!=1 && trav->clipped!=1) {
			make->color_r=trav->color_r;
			make->color_g=trav->color_g;
			make->color_b=trav->color_b;
			for (count3=0;count3<3;count3++) {
				make->vertices[count3].x=trav->vertices[count3].x_per;
				make->vertices[count3].y=trav->vertices[count3].y_per;
				make->vertices[count3].z=trav->vertices[count3].z;
			}
			renderend->next=make;
			renderend=make;
		}
		trav=trav->next;
	}
	renderend->next=NULL;
}

void ObjectClass::pcorrect()
{
	int count2;

	for (disppoly *trav=renderstart->next;trav!=NULL;) {
		for (count2=0;count2<3;count2++) {
			trav->vertices[count2].x_per=((310*trav->vertices[count2].x)/(trav->vertices[count2].z+world_z))+320;
			trav->vertices[count2].y_per=((310*trav->vertices[count2].y)/(trav->vertices[count2].z+world_z))+240;
		}
		trav=trav->next;
	}
}

int ObjectClass::plg_import(char filename[])
{
	ifstream file_ptr;

	char string[50],name[50];
	int count,vertnum,faces,tempint,vert1,vert2,vert3,vert4,verts;

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

	for (count=0;count<vertnum;count++) {
		file_ptr.getline(string,80);
		sscanf(string,"%d %d %d",&vertlist[count].x,&vertlist[count].y,&vertlist[count].z);
	}
	for (count=0;count<faces;count++) {
		poly *trav=new poly;
		file_ptr.getline(string,80);
		sscanf(string,"%d %d",&tempint,&verts);
		if (verts==3) {
			sscanf(string,"%d %d %d %d %d",&tempint,&verts,&vert1,&vert2,&vert3);
			trav->vertices[0].x=vertlist[vert1].x;
			trav->vertices[0].y=vertlist[vert1].y;
			trav->vertices[0].z=vertlist[vert1].z;
			trav->vertices[1].x=vertlist[vert2].x;
			trav->vertices[1].y=vertlist[vert2].y;
			trav->vertices[1].z=vertlist[vert2].z;
			trav->vertices[2].x=vertlist[vert3].x;
			trav->vertices[2].y=vertlist[vert3].y;
			trav->vertices[2].z=vertlist[vert3].z;
		}
		if (verts==4) {
			sscanf(string,"%d %d %d %d %d %d",&tempint,&verts,&vert1,&vert2,&vert3,&vert4);
			trav->vertices[0].x=vertlist[vert1].x;
			trav->vertices[0].y=vertlist[vert1].y;
			trav->vertices[0].z=vertlist[vert1].z;
			trav->vertices[1].x=vertlist[vert2].x;
			trav->vertices[1].y=vertlist[vert2].y;
			trav->vertices[1].z=vertlist[vert2].z;
			trav->vertices[2].x=vertlist[vert3].x;
			trav->vertices[2].y=vertlist[vert3].y;
			trav->vertices[2].z=vertlist[vert3].z;
			trav->color_r=rand()%256;
			trav->color_g=rand()%256;
			trav->color_b=rand()%256;
			count++; faces++;
			trav->vertices[0].x=vertlist[vert3].x;
			trav->vertices[0].y=vertlist[vert3].y;
			trav->vertices[0].z=vertlist[vert3].z;
			trav->vertices[1].x=vertlist[vert4].x;
			trav->vertices[1].y=vertlist[vert4].y;
			trav->vertices[1].z=vertlist[vert4].z;
			trav->vertices[2].x=vertlist[vert1].x;
			trav->vertices[2].y=vertlist[vert1].y;
			trav->vertices[2].z=vertlist[vert1].z;
		}
		trav->color_r=rand()%256;
		trav->color_g=rand()%256;
		trav->color_b=rand()%256;

		polyend->next=trav;
		polyend=trav;
	}
	oneobj.polyend->next=NULL;

	return(1);
}
