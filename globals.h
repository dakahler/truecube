/* globals.h -> Globals for truecube.c */


// holds vertices (needs cleanup)
struct polyvertices {
	double x,y,z,x_per,y_per;
	double x_rot,y_rot,z_rot,x_trans,y_trans,z_trans;
	double x_temp,y_temp,x_wor,y_wor,z_wor;
};

// holds ALL polys defined for object
struct poly {
	int backface;
	int num_sides;
	int clipped;
	unsigned int color_r,color_g,color_b;
	polyvertices vertices[4];
	poly *next;
};

// holds all RENDERED polys for object
struct disppoly {
	int num_sides;
	unsigned int color_r,color_g,color_b;
	polyvertices vertices[3];
	disppoly *next;
};
disppoly *renderstart,*renderend;

// Z-buffer data
struct z_buffer {
		unsigned int color_r,color_g,color_b;
		double depth;
};
z_buffer buffer[640][480];

// camera position and rotation globals

