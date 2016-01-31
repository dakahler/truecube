// All data necessary for one object
// call: ObjectClass objname(world_x,world_y,world_z);
class ObjectClass {
public:
	ObjectClass();                              // Default (don't use)
	ObjectClass(int w_x,int w_y,int w_z);       // Standard call method
	void cam_rot_x(int rot);                    // Rotate obj w/ cam values X
	void cam_rot_y(int rot);                    // Rotate obj w/ cam values Y
	void cam_rot_z(int rot);                    // Rotate obj w/ cam values Z
	void cam_translate(int x,int y,int z);      // Translate obj via cam
	void world_trans(void);                     // Local->world
	void clippoly(void);                        // Screen level clipping
	int  plg_import(char filename[20]);         // Import .PLG
	void generate_master(void);                 // poly->disppoly convert
	void BFC(void);                             // Backface culling
	void pcorrect(void);                        // Perspective correction
	void initLLo(void);                         // Init obj's fullpoly list
	void addscreen(void);                       // Add screen coords
	void setvars(void);                         // Reset some vars..
	void create_world_to_cam(int cam_x,int cam_y,int cam_z,int cam_rot_x,int cam_rot_y,int cam_rot_z);             // Just what it says
	void world_to_cam(void);
	void matrix4x4X1x4(double m1[4][4],double m2[1][4],double value[1][4]);// Multiply two matrices
	void matrix4x4X4x4(double m1[4][4],double m2[4][4],double value[4][4]);// Same
	void setidentity(double matrix[4][4]);        // Make iden. matrix
	double dsin(int angle);
	double dcos(int angle);

private:
	int world_x,world_y,world_z,obj_cam_x,obj_cam_y,obj_cam_z;
	poly *polystart,*polyend;
	double camtrans[4][4];
};

// Default (don't use)
ObjectClass::ObjectClass() {}

// Standard objinit call
ObjectClass::ObjectClass(int w_x,int w_y,int w_z):
world_x(w_x),
world_y(w_y),
world_z(w_z)
{
	obj_cam_x=world_x;
	obj_cam_y=world_y;
	obj_cam_z=world_z;
}

// Can be used to reset some vars (may require mod) TODO: recurse
void ObjectClass::setvars(void)
{
	for (poly *trav=polystart->next;trav!=NULL;) {
		for (int count2=0;count2<3;count2++) {
			trav->vertices[count2].x_rot=trav->vertices[count2].x;
			trav->vertices[count2].y_rot=trav->vertices[count2].y;
			trav->vertices[count2].z_rot=trav->vertices[count2].z;
		}
		trav=trav->next;
	}
}

// Rotate object X into position using cam angle values TODO: recurse
void ObjectClass::cam_rot_x(int rot)
{
	for (poly *trav=polystart->next;trav!=NULL;) {
		for (int count2=0;count2<3;count2++) {
			trav->vertices[count2].y_rot=(trav->vertices[count2].y_wor*(dcos(rot)))-(trav->vertices[count2].z_wor*(dsin(rot)));
			trav->vertices[count2].z_rot=(trav->vertices[count2].y_wor*(dsin(rot)))+(trav->vertices[count2].z_wor*(dcos(rot)));
		}
		trav=trav->next;
	}
}

// Rotate object Y into position using cam angle values TODO: recurse
void ObjectClass::cam_rot_y(int rot)
{
	for (poly *trav=polystart->next;trav!=NULL;) {
		for (int count2=0;count2<3;count2++) {
			trav->vertices[count2].x_rot=(trav->vertices[count2].x_wor*(dcos(rot)))+(trav->vertices[count2].z_wor*(dsin(rot)));
			trav->vertices[count2].z_rot=(-trav->vertices[count2].x_wor*(dsin(rot)))+(trav->vertices[count2].z_wor*(dcos(rot)));
		}
		trav=trav->next;
	}
}

// Rotate object Z into position using cam angle values TODO: recurse
void ObjectClass::cam_rot_z(int rot)
{
	for (poly *trav=polystart->next;trav!=NULL;) {
		for (int count2=0;count2<3;count2++) {
			trav->vertices[count2].x_rot=(trav->vertices[count2].x_wor*(dcos(rot)))-(trav->vertices[count2].y_wor*(dsin(rot)));
			trav->vertices[count2].y_rot=(trav->vertices[count2].x_wor*(dsin(rot)))+(trav->vertices[count2].y_wor*(dcos(rot)));
		}
		trav=trav->next;
	}
}

// Translate object world->cam coords (X,Y,Z) TODO: recurse
void ObjectClass::cam_translate(int x,int y,int z)
{
	for (poly *trav=polystart->next;trav!=NULL;) {
		for (int count=0;count<3;count++) {
			trav->vertices[count].x_trans=trav->vertices[count].x_wor-x;
			trav->vertices[count].y_trans=trav->vertices[count].y_wor-y;
			trav->vertices[count].z_trans=trav->vertices[count].z_wor-z;
		}
		trav=trav->next;
	}
}

// Translate object local->world coords (can be omitted with some mods..)
// TODO: recurse
void ObjectClass::world_trans()
{
	for (poly *trav=polystart->next;trav!=NULL;) {
		for (int count=0;count<3;count++) {
			trav->vertices[count].x_wor=world_x+trav->vertices[count].x;
			trav->vertices[count].y_wor=world_y+trav->vertices[count].y;
			trav->vertices[count].z_wor=world_z+trav->vertices[count].z;
		}
		trav=trav->next;
	}
}

// Perform screen-level clipping per poly TODO: recurse
void ObjectClass::clippoly()
{
	for (poly *trav=polystart->next;trav!=NULL;) {
		if (trav->vertices[0].x_rot>640 || trav->vertices[0].x_rot<0 ||
			trav->vertices[0].y_rot>480 || trav->vertices[0].y_rot<0 ||
			trav->vertices[1].x_rot>640 || trav->vertices[1].x_rot<0 ||
			trav->vertices[1].y_rot>480 || trav->vertices[1].y_rot<0 ||
			trav->vertices[2].x_rot>640 || trav->vertices[2].x_rot<0 ||
			trav->vertices[2].y_rot>480 || trav->vertices[2].y_rot<0)
			trav->clipped=1;
		else {
			trav->clipped=0;
			//printf("\nnot clipped");
		}
		//printf("\nCLIP:%f\n",trav->vertices[0].z_trans);

		trav=trav->next;
	}
}

// Throw appropriate polys into the renderpoly list TODO: recurse
void ObjectClass::generate_master()
{
	for (poly *trav=polystart->next;trav!=NULL;) {
		disppoly *make=new disppoly;
		if (trav->backface!=1 && trav->clipped!=1) {
			make->color_r=trav->color_r;
			make->color_g=trav->color_g;
			make->color_b=trav->color_b;
			for (int count3=0;count3<3;count3++) {
				make->vertices[count3].x_per=trav->vertices[count3].x_rot;
				make->vertices[count3].y_per=trav->vertices[count3].y_rot;
				make->vertices[count3].z=trav->vertices[count3].z_trans;
				//printf("<%f>",trav->vertices[count3].x_trans);
			}
			renderend->next=make;
			renderend=make;
		}
		trav=trav->next;
	}
	renderend->next=NULL;
}

// Perspective correction TODO: recurse
void ObjectClass::pcorrect()
{
	for (poly *trav=polystart->next;trav!=NULL;) {
		for (int count2=0;count2<3;count2++) {
			trav->vertices[count2].x_rot=((310*trav->vertices[count2].x_trans)/(trav->vertices[count2].z_trans));
			trav->vertices[count2].y_rot=((310*trav->vertices[count2].y_trans)/(trav->vertices[count2].z_trans));
		}
		count2--;
		//printf("%f %f %f | ",trav->vertices[count2].x_per,trav->vertices[count2].y_per,trav->vertices[count2].z);
		trav=trav->next;
	}
}

// Backface Culling TODO: recurse
void ObjectClass::BFC()
{
	int dotp;
	double norm_i=0,norm_j=0,norm_k=0;

	for (poly *trav=polystart->next;trav!=NULL;) {
		cross((trav->vertices[0].x_wor-trav->vertices[1].x_wor),
			  (trav->vertices[0].y_wor-trav->vertices[1].y_wor),
			  (trav->vertices[0].z_wor-trav->vertices[1].z_wor),
			  (trav->vertices[2].x_wor-trav->vertices[1].x_wor),
			  (trav->vertices[2].y_wor-trav->vertices[1].y_wor),
			  (trav->vertices[2].z_wor-trav->vertices[1].z_wor),
			   &norm_i,&norm_j,&norm_k);
		dotp=dot(norm_i,norm_j,norm_k,
			-(trav->vertices[1].x_wor),
			-(trav->vertices[1].y_wor),
			-(trav->vertices[1].z_wor));
		if (dotp<0) {
			trav->backface=1;
		}
		else {
			trav->backface=0;
		}

		trav=trav->next;
	}
}

// Import an object using .PLG
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
	polyend->next=NULL;

	return(1);
}

// Init object's fullpoly list
void ObjectClass::initLLo(void)
{
	polystart=new poly;
	polyend=polystart;
}

// Add screen coords to vertices TODO: recurse
void ObjectClass::addscreen(void)
{
	for (poly *trav=polystart->next;trav!=NULL;) {
		for (int count2=0;count2<3;count2++) {
			trav->vertices[count2].y_rot+=240;
			trav->vertices[count2].x_rot+=320;
		}
		count2--;
		printf("/%lf/",trav->vertices[count2].x_rot);
		trav=trav->next;
	}
}

// Create World to Camera Translation Matrix
void ObjectClass::create_world_to_cam(int cam_x,int cam_y,int cam_z,int cam_rot_x,int cam_rot_y,int cam_rot_z)
{
	double translate[4][4],           // translate[ROW][COLUMN]
		rotate_x[4][4],
		rotate_y[4][4],
		rotate_z[4][4],
		temp1[4][4],
		temp2[4][4];

	setidentity(translate);
	setidentity(rotate_x);
	setidentity(rotate_y);
	setidentity(rotate_z);

	translate[3][0]=-cam_x;
	translate[3][1]=-cam_y;
	translate[3][2]=-cam_z;

	if (cam_rot_x<0)
		cam_rot_x+=360;
	if (cam_rot_y<0)
		cam_rot_y+=360;
	if (cam_rot_z<0)
		cam_rot_z+=360;

	rotate_x[1][1]=dcos(cam_rot_x);
	rotate_x[1][2]=-1*(dsin(cam_rot_x));
	rotate_x[2][1]=dsin(cam_rot_x);
	rotate_x[2][2]=dcos(cam_rot_x);

	rotate_y[0][0]=dcos(cam_rot_y);
	rotate_y[0][2]=dsin(cam_rot_y);
	rotate_y[2][0]=-1*(dsin(cam_rot_y));
	rotate_y[2][2]=dcos(cam_rot_y);

	rotate_z[0][0]=dcos(cam_rot_z);
	rotate_z[0][1]=-1*(dsin(cam_rot_z));
	rotate_z[1][0]=dsin(cam_rot_z);
	rotate_z[1][1]=dcos(cam_rot_z);

	printf(" -- %d -- ",cam_rot_y);

	matrix4x4X4x4(translate,rotate_x,temp1);
	matrix4x4X4x4(temp1,rotate_y,temp2);
	matrix4x4X4x4(temp2,rotate_z,camtrans);
}

void ObjectClass::setidentity(double matrix[4][4])
{
	matrix[0][0]=matrix[1][1]=matrix[2][2]=matrix[3][3]=1;
	matrix[0][1]=matrix[0][2]=matrix[0][3]=matrix[1][0]=
	matrix[1][2]=matrix[1][3]=matrix[2][0]=matrix[2][1]=
	matrix[2][3]=matrix[3][0]=matrix[3][1]=matrix[3][2]=0;
}

void ObjectClass::matrix4x4X4x4(double m1[4][4],double m2[4][4],double value[4][4])
{
	int sum=0,i,j,k;

	for (i=0;i<4;i++) {
		for (j=0;j<4;j++) {
			sum=0;
			for (k=0;k<4;k++)
				sum+=m1[i][k]*m2[k][j];

			value[i][j]=sum;
		}
	}
}

void ObjectClass::world_to_cam()
{
	for (poly *trav=polystart->next;trav!=NULL;) {
		for (int count2=0;count2<3;count2++) {
			trav->vertices[count2].x_trans=
				trav->vertices[count2].x_wor*camtrans[0][0]+
				trav->vertices[count2].y_wor*camtrans[1][0]+
				trav->vertices[count2].z_wor*camtrans[2][0]+
											 camtrans[3][0];

			trav->vertices[count2].y_trans=
				trav->vertices[count2].x_wor*camtrans[0][1]+
				trav->vertices[count2].y_wor*camtrans[1][1]+
				trav->vertices[count2].z_wor*camtrans[2][1]+
											 camtrans[3][1];

			trav->vertices[count2].z_trans=
				trav->vertices[count2].x_wor*camtrans[0][2]+
				trav->vertices[count2].y_wor*camtrans[1][2]+
				trav->vertices[count2].z_wor*camtrans[2][2]+
											 camtrans[3][2];
		}
		cout << "&&" << trav->vertices[count2].x_trans << "&&";
		trav=trav->next;
	}
}

//degree sin
double ObjectClass::dsin(int angle)
{
	return(sin(angle*(PI/180)));
}

//degree cos
double ObjectClass::dcos(int angle)
{
	return(cos(angle*(PI/180)));
}