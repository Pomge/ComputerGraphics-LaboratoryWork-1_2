#include "Render.h"
#include <sstream>
#include <iostream>
#include <windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>
#include "MyOGL.h"
#include "Camera.h"
#include "Light.h"
#include "Primitives.h"
#include "GUItextRectangle.h"

//===============================================================================
// Texture Params
int textureId = 0;
int textureIndex = 0;
std::string textureNames[] = {
	"texture_test.bmp",
	"texture2.bmp",
	"texture3.bmp"
};
unsigned int textures[sizeof(textureNames) / sizeof(std::string)];

// Figure Params
const int steps = 15;		// do not touch (i don't get it how it's works)
double alpha = 0.75;
const double height = 5.0;	// retexturing required
bool textureMode = false;
bool lightMode = true;
bool alphaMode = false;
bool bulgeMode = true;
bool cutoutMode = true;
bool animationMode = true;

int partColor = 0;
int chooseRGB = 0;
bool colorMode = false;
std::vector <std::vector <double>> figureColors = {
// Figure Top		[0]
	{ 1, 0, 0 },
// Figure Bottom	[1]
	{ 1, 0, 0 },
// Figure Walls		[2]
	{ 0, 1, 0 },
// Circle 1 Top		[3]
	{ 0, 0, 1 },
// Circle 1 Bottom	[4]
	{ 0, 0, 1 },
// Circle 1 Walls	[5]
	{ 0, 0, 1 },
// Circle 2 Top		[6]
	{ 0, 0, 1 },
// Circle 2 Bottom	[7]
	{ 0, 0, 1 },
// Circle 2 Walls	[8]
	{ 0, 0, 1 }
};

// First Circle Params
double center_1[3];
double radius_1;
double k_1;

// Second Circle Params
double center_2[3];
double center_3[3];
double radius_2;
double k_2;
double k_3;
const double M[] = { 2, 6, 0 };

// Animation Params
bool isSwitched = false;
const double animationSpeed = 0.01;
//double lighting[] = { 0.0, -0.6, -1.5, -0.3, -0.9, -1.2 };
double lighting[] = { -1.0, -1.6, -2.5, -1.3, -1.9, -2.2 };

double isGrowing[] = { true, true, true, true, true, true };

// Optimisation Params
std::vector <std::vector <double>> points;

// Textures Coordinates
double firstCircle_center_X = 0.3348918760958503798947983635301;
double firstCircle_center_Y = 1.0 - 0.89189189189189189189189189189189;
double firstCircle_radius_X = 0.06779661016949152542372881355932;
double firstCircle_radius_Y = 0.10810810810810810810810810810811;

double firstCircle_startCoord_X = 0.78725891291642314436002337814144;
double firstCircle_endCoord_X = 1.0;
double firstCircle_startCoord_Y = 1.0;
double firstCircle_endCoord_Y = 1.0 - 0.25442684063373718546132339235788;

double secondCircle_center_X_1 = 0.12799532437171244886031560490941;
double secondCircle_center_Y_1 = 1.0 - 0.71388630009319664492078285181733;
double secondCircle_center_X_2 = -0.1145528930450029222676797194623;
double secondCircle_center_Y_2 = 1.0 - 0.58527493010251630941286113699907;
double secondCircle_radius_X = 0.18398597311513734658094681472823;
double secondCircle_radius_Y = 0.29338303821062441752096924510718;

double secondCircle_startCoord_X = 0.64114552893045002922267679719462;
double secondCircle_endCoord_X = 0.85388661601402688486265341905319;
double secondCircle_startCoord_Y = 1.0 - 0.25535880708294501397949673811743;
double secondCircle_endCoord_Y = 1.0 - 0.5097856477166821994408201304753;

std::vector <std::vector <std::vector<double>>> textureCoords_Triangles = {
// Bottom
// Triangle 0
{
	{ 0.0, 									1.0 - 0.81547064305684995340167753960857 },
	{ 0.0642898889538281706604324956166, 	1.0 - 0.51071761416589002795899347623486 },
	{ 0.12799532437171244886031560490941, 	1.0 - 0.71388630009319664492078285181733 }
},
// Triangle 1
{
	{ 0.12799532437171244886031560490941, 	1.0 - 0.71388630009319664492078285181733 },
	{ 0.19170075978959672706019871420222, 	1.0 - 0.51071761416589002795899347623486 },
	{ 0.25540619520748100526008182349503, 	1.0 - 0.86672879776328052190121155638397 }
},
// Triangle 2
{
	{ 0.25540619520748100526008182349503, 	1.0 - 0.86672879776328052190121155638397 },
	{ 0.28696668614845119812974868497954, 	1.0 - 0.66356011183597390493942218080149 },
	{ 0.38223261250730566919929865575687, 	1.0 - 0.81547064305684995340167753960857 }
},
// Triangle 3
{
	{ 0.0, 									1.0 - 0.81547064305684995340167753960857 },
	{ 0.12799532437171244886031560490941, 	1.0 - 0.71388630009319664492078285181733 },
	{ 0.25540619520748100526008182349503, 	1.0 - 0.86672879776328052190121155638397 }
},
// Triangle 4
{
	{ 0.0, 									1.0 - 0.81547064305684995340167753960857 },
	{ 0.25540619520748100526008182349503, 	1.0 - 0.86672879776328052190121155638397 },
	{ 0.28696668614845119812974868497954, 	1.0 - 0.96831314072693383038210624417521 }
},
// Triangle 5
{
	{ 0.25540619520748100526008182349503, 	1.0 - 0.86672879776328052190121155638397 },
	{ 0.28696668614845119812974868497954, 	1.0 - 0.96831314072693383038210624417521 },
	{ 0.38223261250730566919929865575687, 	1.0 - 0.81547064305684995340167753960857 }
},

// Top
// Triangle 0
{
	{ 0.40268848626534190531852717708942, 	1.0 - 0.81547064305684995340167753960857 },
	{ 0.46639392168322618351841028638223, 	1.0 - 0.51071761416589002795899347623486 },
	{ 0.52951490356516656925774400935126, 	1.0 - 0.71388630009319664492078285181733 }
},
// Triangle 1
{
	{ 0.52951490356516656925774400935126, 	1.0 - 0.71388630009319664492078285181733 },
	{ 0.59380479251899473991817650496786, 	1.0 - 0.51071761416589002795899347623486 },
	{ 0.65751022793687901811805961426067, 	1.0 - 0.86672879776328052190121155638397 }
},
// Triangle 2
{
	{ 0.65751022793687901811805961426067, 	1.0 - 0.86672879776328052190121155638397 },
	{ 0.68965517241379310344827586206897, 	1.0 - 0.66356011183597390493942218080149 },
	{ 0.78492109877264757451782583284629, 	1.0 - 0.81547064305684995340167753960857 }
},
// Triangle 3
{
	{ 0.40268848626534190531852717708942, 	1.0 - 0.81547064305684995340167753960857 },
	{ 0.52951490356516656925774400935126, 	1.0 - 0.71388630009319664492078285181733 },
	{ 0.65751022793687901811805961426067, 	1.0 - 0.86672879776328052190121155638397 }
},
// Triangle 4
{
	{ 0.40268848626534190531852717708942, 	1.0 - 0.81547064305684995340167753960857 },
	{ 0.65751022793687901811805961426067, 	1.0 - 0.86672879776328052190121155638397 },
	{ 0.68965517241379310344827586206897, 	1.0 - 0.96831314072693383038210624417521 }
},
// Triangle 5
{
	{ 0.65751022793687901811805961426067,	1.0 - 0.86672879776328052190121155638397 },
	{ 0.68965517241379310344827586206897, 	1.0 - 0.96831314072693383038210624417521 },
	{ 0.78492109877264757451782583284629, 	1.0 - 0.81547064305684995340167753960857 }
}
};

std::vector <std::vector <std::vector<double>>> textureCoords_Quads = {
// Wall 0
{
	{ 0.50496785505552308591466978375219, 	1.0 - 0.5097856477166821994408201304753 },
	{ 0.50496785505552308591466978375219, 	1.0 - 0.25535880708294501397949673811743 },
	{ 0.30333138515488018702513150204559, 	1.0 - 0.25535880708294501397949673811743 },
	{ 0.30333138515488018702513150204559, 	1.0 - 0.5097856477166821994408201304753 }
},
// Wall 1
{
	{ 0.14260666277030976037405026300409, 	1.0 - 0.25442684063373718546132339235788 },
	{ 0.14260666277030976037405026300409, 	1.0 - 0.0 },
	{ 0.0, 									1.0 - 0.0 },
	{ 0.0, 									1.0 - 0.25442684063373718546132339235788 }
},
// Wall 2
{
	{ 0.28579777907656341320864991233197, 	1.0 - 0.25442684063373718546132339235788 },
	{ 0.28579777907656341320864991233197, 	1.0 - 0.0 },
	{ 0.14319111630625365283459964932788, 	1.0 - 0.0 },
	{ 0.14319111630625365283459964932788, 	1.0 - 0.25442684063373718546132339235788 }
},
// Wall 3
{
	{ 0.5184102863822326125073056691993, 	1.0 - 0.25442684063373718546132339235788 },
	{ 0.5184102863822326125073056691993, 	1.0 - 0.0 },
	{ 0.28638223261250730566919929865576, 	1.0 - 0.0 },
	{ 0.28638223261250730566919929865576, 	1.0 - 0.25442684063373718546132339235788 }
},
// Wall 4
{
	{ 0.65049678550555230859146697837522, 	1.0 - 0.25442684063373718546132339235788 },
	{ 0.65049678550555230859146697837522, 	1.0 - 0.0 },
	{ 0.51899473991817650496785505552309, 	1.0 - 0.0 },
	{ 0.51899473991817650496785505552309, 	1.0 - 0.25442684063373718546132339235788 }
},
// Wall 5
{
	{ 0.78667445938047925189947399181765, 	1.0 - 0.25442684063373718546132339235788 },
	{ 0.78667445938047925189947399181765, 	1.0 - 0.0 },
	{ 0.65108123904149620105201636469901, 	1.0 - 0.0 },
	{ 0.65108123904149620105201636469901, 	1.0 - 0.25442684063373718546132339235788 }
},
// Wall 6
{
	{ 0.64056107539450613676212741087084, 	1.0 - 0.5097856477166821994408201304753 },
	{ 0.64056107539450613676212741087084, 	1.0 - 0.25535880708294501397949673811743 },
	{ 0.50555230859146697837521917007598, 	1.0 - 0.25535880708294501397949673811743 },
	{ 0.50555230859146697837521917007598, 	1.0 - 0.5097856477166821994408201304753 }
},
// Wall 7
{
	{ 0.3027469316189362945645821157218, 	1.0 - 0.5097856477166821994408201304753 },
	{ 0.3027469316189362945645821157218, 	1.0 - 0.25535880708294501397949673811743 },
	{ 0.0, 									1.0 - 0.25535880708294501397949673811743 },
	{ 0.0, 									1.0 - 0.5097856477166821994408201304753 }
}
};


void loadTexture(std::string textureName) {
	RGBTRIPLE *textureArray;
	char *textureCharArray;
	int textureWidth, textureHeight;

	OpenGL::LoadBMP(textureName.c_str(), &textureWidth, &textureHeight, &textureArray);
	OpenGL::RGBtoChar(textureArray, textureWidth, textureHeight, &textureCharArray);

	glBindTexture(GL_TEXTURE_2D, textures[textureId]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0,
				 GL_RGBA, GL_UNSIGNED_BYTE, textureCharArray);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	free(textureCharArray);
	free(textureArray);
	textureId++;
}

void initializeTextures() {
	glGenTextures(sizeof(textureNames) / sizeof(std::string), textures);
	for (int i = 0; i < sizeof(textureNames) / sizeof(std::string); i++) loadTexture(textureNames[i]);
}


void calculateNormal(double *firstPoint, double *secondPoint, double *generalPoint) {
	double x_1 = firstPoint[0];
	double y_1 = firstPoint[1];
	double z_1 = firstPoint[2];

	double x_2 = secondPoint[0];
	double y_2 = secondPoint[1];
	double z_2 = secondPoint[2];

	double x_3 = generalPoint[0];
	double y_3 = generalPoint[1];
	double z_3 = generalPoint[2];

	double line_1[] = { x_3 - x_1, y_3 - y_1, z_3 - z_1 };
	double line_2[] = { x_3 - x_2, y_3 - y_2, z_3 - z_2 };

	double n_x = line_1[1] * line_2[2] - line_2[1] * line_1[2];
	double n_y = line_1[0] * line_2[2] - line_2[0] * line_1[2];
	double n_z = line_1[0] * line_2[1] - line_2[0] * line_1[1];

	double length = sqrt(pow(n_x, 2) + pow(n_y, 2) + pow(n_z, 2));
	double result[] = { n_x / length, -n_y / length, n_z / length };

	glNormal3dv(result);
}

double calculateAngularCoefficient(double *firstPoint, double *secondPoint) {
	double param_A = firstPoint[1] - secondPoint[1];
	double param_B = secondPoint[0] - firstPoint[0];

	return -param_A / param_B;
}

void calculateFirstCircleParams(double *firstPoint, double *secondPoint) {
	double max_x = max(firstPoint[0], secondPoint[0]);
	double min_x = min(firstPoint[0], secondPoint[0]);
	double max_y = max(firstPoint[1], secondPoint[1]);
	double min_y = min(firstPoint[1], secondPoint[1]);

	center_1[0] = (max_x - min_x) / 2 + min_x;
	center_1[1] = (max_y - min_y) / 2 + min_y;
	radius_1 = sqrt(pow(max_x - min_x, 2) + pow(max_y - min_y, 2)) / 2;
	k_1 = calculateAngularCoefficient(firstPoint, secondPoint);
}

void calculateSecondCircleParams(double *firstPoint, double *secondPoint, const double *thirdPoint, double *fourthPoint) {
	double x_1 = firstPoint[0];
	double y_1 = firstPoint[1];
	double x_2 = secondPoint[0];
	double y_2 = secondPoint[1];
	double x_3 = thirdPoint[0];
	double y_3 = thirdPoint[1];
	double x_4 = fourthPoint[0];
	double y_4 = fourthPoint[1];

	double sum_1 = pow(x_2, 2) + pow(y_2, 2) - pow(x_3, 2) - pow(y_3, 2);
	double sum_2 = pow(x_3, 2) + pow(y_3, 2) - pow(x_1, 2) - pow(y_1, 2);
	double sum_3 = pow(x_1, 2) + pow(y_1, 2) - pow(x_2, 2) - pow(y_2, 2);
	double divider = x_1 * (y_2 - y_3) + x_2 * (y_3 - y_1) + x_3 * (y_1 - y_2);

	center_2[0] = -0.5 * (y_1 * sum_1 + y_2 * sum_2 + y_3 * sum_3) / divider;
	center_2[1] = 0.5 * (x_1 * sum_1 + x_2 * sum_2 + x_3 * sum_3) / divider;
	center_3[0] = fourthPoint[0];
	center_3[1] = fourthPoint[1];
	center_3[2] = fourthPoint[2];

	radius_2 = sqrt(pow(x_3 - center_2[0], 2) + pow(y_3 - center_2[1], 2));
	k_2 = calculateAngularCoefficient(firstPoint, center_2);
	k_3 = calculateAngularCoefficient(secondPoint, center_2);
}

void calculateStaticParams() {
	points = { { 0, 3, 0 },	{ 2, 9, 0 },
			  { 4, 5, 0 }, { 6, 9, 0 },
			  { 8, 2, 0 }, { 9, 6, 0 },
			  { 12, 3, 0 }, { 9, 0, 0 } };
	initializeTextures();
	if (bulgeMode) calculateFirstCircleParams(&points[6][0], &points[7][0]);
	if (cutoutMode) calculateSecondCircleParams(&points[0][0], &points[1][0], M, &points[2][0]);
}


void splitFirstCircleTexture() {
	int partIndex = 0;
	double firstCirclePart = (firstCircle_endCoord_X - firstCircle_startCoord_X) / double(steps);

	for (int i = 0; i < steps; i++) {
		double startOffset_X = firstCircle_startCoord_X + (partIndex + 1.0) * firstCirclePart;
		double endOffset_X = firstCircle_startCoord_X + partIndex * firstCirclePart;

		std::vector <double> lowerLeft = { startOffset_X, firstCircle_endCoord_Y };
		std::vector <double> upperLeft = { startOffset_X, firstCircle_startCoord_Y };
		std::vector <double> upperRight = { endOffset_X, firstCircle_startCoord_Y };
		std::vector <double> lowerRight = { endOffset_X, firstCircle_endCoord_Y };
		std::vector <std::vector <double>> quad = { lowerLeft, upperLeft, upperRight, lowerRight };
		textureCoords_Quads.push_back(quad);
		partIndex++;
	}

	double angle = atan(k_1) * 180.0 / PI;
	double step = 180.0 / steps;

	for (int k = 0; k < 2; k++) {
		if (k > 0) firstCircle_center_X = 0.73699590882524839275277615429573;

		for (double i = angle - 180.0; i < angle; i += step) {
			std::vector <double> point_1 = { firstCircle_center_X, firstCircle_center_Y };

			double dx = firstCircle_radius_X * cos(i * PI / 180.0);
			double dy = firstCircle_radius_Y * sin(i * PI / 180.0);
			std::vector <double> point_2 = { firstCircle_center_X + dx, firstCircle_center_Y + dy };

			dx = firstCircle_radius_X * cos((i + step) * PI / 180.0);
			dy = firstCircle_radius_Y * sin((i + step) * PI / 180.0);
			std::vector <double> point_3 = { firstCircle_center_X + dx, firstCircle_center_Y + dy };

			std::vector <std::vector <double>> triangle = { point_1, point_2, point_3 };
			textureCoords_Triangles.push_back(triangle);
		}
	}
}

void splitSecondCircleTexture() {
	int partIndex = 0;
	double secondCirclePart = (secondCircle_endCoord_X - secondCircle_startCoord_X) / double(steps);

	for (int i = 0; i < steps; i++) {
		double startOffset_X = secondCircle_endCoord_X - (partIndex + 1.0) * secondCirclePart;
		double endOffset_X = secondCircle_endCoord_X - partIndex * secondCirclePart;

		std::vector <double> lowerLeft = { startOffset_X, secondCircle_endCoord_Y };
		std::vector <double> upperLeft = { startOffset_X, secondCircle_startCoord_Y };
		std::vector <double> upperRight = { endOffset_X, secondCircle_startCoord_Y };
		std::vector <double> lowerRight = { endOffset_X, secondCircle_endCoord_Y };
		std::vector <std::vector <double>> quad = { lowerRight, upperRight, upperLeft, lowerLeft };
		textureCoords_Quads.push_back(quad);
		partIndex++;
	}

	double angle_1 = atan(k_2) * 180.0 / PI;
	double angle_2 = atan(k_3) * 180.0 / PI;
	double step = (abs(angle_1) + abs(angle_2)) / double(steps);

	for (int k = 0; k < 2; k++) {
		if (k > 0) {
			secondCircle_center_X_1 = 0.52951490356516656925774400935126;
			secondCircle_center_X_2 = 0.28813559322033898305084745762712;
		}

		for (double i = angle_1; i < angle_2; i += step) {
			std::vector <double> point_1 = { secondCircle_center_X_1, secondCircle_center_Y_1 };

			double dx = secondCircle_radius_X * cos(i * PI / 180.0);
			double dy = secondCircle_radius_Y * sin(i * PI / 180.0);
			double point[] = { center_2[0] + dx, center_2[1] + dy, 0 };
			std::vector <double> point_2 = { secondCircle_center_X_2 + dx, secondCircle_center_Y_2 + dy };

			dx = secondCircle_radius_X * cos((i + step) * PI / 180.0);
			dy = secondCircle_radius_Y * sin((i + step) * PI / 180.0);
			std::vector <double> point_3 = { secondCircle_center_X_2 + dx, secondCircle_center_Y_2 + dy };

			std::vector <std::vector <double>> triangle = { point_1, point_2, point_3 };
			textureCoords_Triangles.push_back(triangle);
		}
	}
}


void drawTriangle(int *texturePartIndex, double *point_1, double *point_2, double *point_3) {
	glBegin(GL_TRIANGLES);
	glTexCoord2dv(&textureCoords_Triangles[*texturePartIndex][0][0]);
	glVertex3dv(point_1);
	glTexCoord2dv(&textureCoords_Triangles[*texturePartIndex][1][0]);
	glVertex3dv(point_2);
	glTexCoord2dv(&textureCoords_Triangles[*texturePartIndex][2][0]);
	glVertex3dv(point_3);
	glEnd();

	(*texturePartIndex)++;
}

void drawQuad(int *texturePartIndex, double *lowerPoint_1, double *upperPoint_1, double *upperPoint_2, double *lowerPoint_2) {
	calculateNormal(upperPoint_1, lowerPoint_2, lowerPoint_1);

	glBegin(GL_QUADS);
	glTexCoord2dv(&textureCoords_Quads[*texturePartIndex][0][0]);
	glVertex3dv(lowerPoint_1);
	glTexCoord2dv(&textureCoords_Quads[*texturePartIndex][1][0]);
	glVertex3dv(upperPoint_1);
	glTexCoord2dv(&textureCoords_Quads[*texturePartIndex][2][0]);
	glVertex3dv(upperPoint_2);
	glTexCoord2dv(&textureCoords_Quads[*texturePartIndex][3][0]);
	glVertex3dv(lowerPoint_2);
	glEnd();

	(*texturePartIndex)++;
}


void getColorForTriangles(bool isTopDrawing, int k) {
	int index = isTopDrawing ? 0 : 1;
	double color[] = { figureColors[index][0] - lighting[k], figureColors[index][1] - lighting[k], figureColors[index][2] - lighting[k] };
	isTopDrawing &&alphaMode ? glColor4d(color[0], color[1], color[2], alpha) : glColor3dv(color);
}

void drawFigureBottom(bool isTopDrawing) {
	int k = 0;
	isTopDrawing ? glNormal3d(0, 0, 1) : glNormal3d(0, 0, -1);

	int texturePartIndex = 0;
	if (isTopDrawing) texturePartIndex = 6;
	if (cutoutMode) texturePartIndex++;

	for (int i = cutoutMode ? 2 : 0; i < int(points.size() - 2); i += 2) {
		getColorForTriangles(isTopDrawing, k);
		drawTriangle(&texturePartIndex, &points[i][0], &points[i + 1][0], &points[i + 2][0]);
		k++;
	}

	//isTopDrawing && alphaMode ? glColor4d(lighting[k], 0, 0, alpha) : glColor3d(lighting[k], 0, 0);
	getColorForTriangles(isTopDrawing, k);
	drawTriangle(&texturePartIndex, &points[0][0], &points[2][0], &points[4][0]);
	k++;

	getColorForTriangles(isTopDrawing, k);
	drawTriangle(&texturePartIndex, &points[0][0], &points[4][0], &points[7][0]);
	k++;

	getColorForTriangles(isTopDrawing, k);
	drawTriangle(&texturePartIndex, &points[4][0], &points[7][0], &points[6][0]);
}

void drawFigureTop() {
	glTranslated(0, 0, height);
	drawFigureBottom(true);
	glTranslated(0, 0, -height);
}

void drawFigureWalls() {
	int startIndex = cutoutMode ? 1 : 0;
	int texturePartIndex = startIndex;
	int endIndex = bulgeMode ? (points.size() - 2) : (points.size() - 1);

	double colorOffset = 5.0;
	double divider = double(endIndex) + colorOffset;

	for (int i = startIndex; i < endIndex; i++) {
		double color[] = {
			figureColors[2][0] * double(i + colorOffset) / divider,
			figureColors[2][1] * double(i + colorOffset) / divider,
			figureColors[2][2] * double(i + colorOffset) / divider };
		glColor3dv(color);

		double lowerRightPoint[] = { points[i][0], points[i][1], points[i][2] };
		double upperRightPoint[] = { points[i][0], points[i][1], points[i][2] + height };
		double upperLeftPoint[] = { points[i + 1][0], points[i + 1][1], points[i + 1][2] + height };
		double lowerLeftPoint[] = { points[i + 1][0], points[i + 1][1], points[i + 1][2] };
		drawQuad(&texturePartIndex, lowerRightPoint, upperRightPoint, upperLeftPoint, lowerLeftPoint);
	}

	texturePartIndex = 7;
	int size = points.size() - 1;
	glColor3dv(&figureColors[2][0]);

	double lowerRightPoint[] = { points[size][0], points[size][1], points[size][2] };
	double upperRightPoint[] = { points[size][0], points[size][1], points[size][2] + height };
	double upperLeftPoint[] = { points[0][0], points[0][1], points[0][2] + height };
	double lowerLeftPoint[] = { points[0][0], points[0][1], points[0][2] };
	drawQuad(&texturePartIndex, lowerRightPoint, upperRightPoint, upperLeftPoint, lowerLeftPoint);
}

void drawFigure() {
	drawFigureBottom(false);
	drawFigureTop();
	drawFigureWalls();
}


void drawFirstCircleBottom(bool isTopDrawing) {
	isTopDrawing ? glNormal3d(0, 0, 1) : glNormal3d(0, 0, -1);
	double *color = &figureColors[isTopDrawing ? 3 : 4][0];
	isTopDrawing &&alphaMode ? glColor4d(color[0], color[1], color[2], alpha) : glColor3dv(color);

	int texturePartIndex = 12;
	if (isTopDrawing) texturePartIndex += steps + 1;

	double angle = atan(k_1) * 180.0 / PI;
	double step = 180.0 / steps;

	for (double i = angle - 180.0; i < angle - step; i += step) {
		double dx = radius_1 * cos(i * PI / 180.0);
		double dy = radius_1 * sin(i * PI / 180.0);
		double point_1[] = { center_1[0] + dx, center_1[1] + dy, 0 };

		dx = radius_1 * cos((i + step) * PI / 180.0);
		dy = radius_1 * sin((i + step) * PI / 180.0);
		double point_2[] = { center_1[0] + dx, center_1[1] + dy, 0 };

		drawTriangle(&texturePartIndex, center_1, point_1, point_2);
	}
}

void drawFirstCircleTop() {
	glTranslated(0, 0, height);
	drawFirstCircleBottom(true);
	glTranslated(0, 0, -height);
}

void drawFirstCircleWalls() {
	int texturePartIndex = 8;
	double color[] = { 0, 0, 0 };
	double angle = atan(k_1) * 180.0 / PI;
	double step = 180.0 / steps;

	for (double i = angle - 180.0; i < angle; i += step) {
		glColor3dv(color);
		for (int c = 0; c < figureColors[5].size(); c++) color[c] += figureColors[5][c] / steps;

		double dx = radius_1 * cos(i * PI / 180.0);
		double dy = radius_1 * sin(i * PI / 180.0);
		double lowerRightPoint[] = { center_1[0] + dx, center_1[1] + dy, 0 };
		double upperRightPoint[] = { center_1[0] + dx, center_1[1] + dy, height };

		dx = radius_1 * cos((i + step) * PI / 180.0);
		dy = radius_1 * sin((i + step) * PI / 180.0);
		double upperLeftPoint[] = { center_1[0] + dx, center_1[1] + dy, height };
		double lowerLeftPoint[] = { center_1[0] + dx, center_1[1] + dy, 0 };

		drawQuad(&texturePartIndex, lowerLeftPoint, upperLeftPoint, upperRightPoint, lowerRightPoint);
	}
}

void drawFirstCircle() {
	drawFirstCircleBottom(false);
	drawFirstCircleTop();
	drawFirstCircleWalls();
}


void drawSecondCircleBottom(bool isTopDrawing) {
	isTopDrawing ? glNormal3d(0, 0, 1) : glNormal3d(0, 0, -1);
	double *color = &figureColors[isTopDrawing ? 7 : 8][0];
	isTopDrawing &&alphaMode ? glColor4d(color[0], color[1], color[2], alpha) : glColor3dv(color);

	int texturePartIndex = 12 + steps * 2 + 2;
	if (isTopDrawing) texturePartIndex += steps + 1;

	double angle_1 = atan(k_2) * 180.0 / PI;
	double angle_2 = atan(k_3) * 180.0 / PI;
	double step = (abs(angle_1) + abs(angle_2)) / double(steps);

	for (double i = angle_1; i < angle_2 - step; i += step) {
		double dx = radius_2 * cos(i * PI / 180.0);
		double dy = radius_2 * sin(i * PI / 180.0);
		double point_1[] = { center_2[0] + dx, center_2[1] + dy, 0 };

		dx = radius_2 * cos((i + step) * PI / 180.0);
		dy = radius_2 * sin((i + step) * PI / 180.0);
		double point_2[] = { center_2[0] + dx, center_2[1] + dy, 0 };

		drawTriangle(&texturePartIndex, center_3, point_1, point_2);
	}
}

void drawSecondCircleTop() {
	glTranslated(0, 0, height);
	drawSecondCircleBottom(true);
	glTranslated(0, 0, -height);
}

void drawSecondCircleWalls() {
	int texturePartIndex = 8 + steps;
	double color[] = { 0, 0, 0 };
	double angle_1 = atan(k_2) * 180.0 / PI;
	double angle_2 = atan(k_3) * 180.0 / PI;
	double step = (abs(angle_1) + abs(angle_2)) / double(steps);

	for (double i = angle_1; i < angle_2 - step; i += step) {
		glColor3dv(color);
		for (int c = 0; c < figureColors[8].size(); c++) color[c] += figureColors[8][c] / steps;

		double dx = radius_2 * cos(i * PI / 180.0);
		double dy = radius_2 * sin(i * PI / 180.0);
		double lowerRightPoint[] = { center_2[0] + dx, center_2[1] + dy, 0 };
		double upperRightPoint[] = { center_2[0] + dx, center_2[1] + dy, height };

		dx = radius_2 * cos((i + step) * PI / 180.0);
		dy = radius_2 * sin((i + step) * PI / 180.0);
		double upperLeftPoint[] = { center_2[0] + dx, center_2[1] + dy, height };
		double lowerLeftPoint[] = { center_2[0] + dx, center_2[1] + dy, 0 };

		drawQuad(&texturePartIndex, lowerRightPoint, upperRightPoint, upperLeftPoint, lowerLeftPoint);
	}
}

void drawSecondCircle() {
	drawSecondCircleBottom(false);
	drawSecondCircleTop();
	drawSecondCircleWalls();
}


void animationControl() {
	if (animationMode) {
		if (isSwitched) {
			lighting[0] = -1.0;
			lighting[1] = -1.6;
			lighting[2] = -2.5;
			lighting[3] = -1.3;
			lighting[4] = -1.9;
			lighting[5] = -2.2;
			isSwitched = false;
		}

		for (int i = 0; i < sizeof(lighting) / sizeof(lighting[0]); i++) {
			if (lighting[i] < 1 && isGrowing[i]) {
				lighting[i] += animationSpeed;
			} else {
				isGrowing[i] = false;
				lighting[i] -= animationSpeed;
				if (lighting[i] < 0.3) isGrowing[i] = true;
			}
		}
	} else {
		for (int i = 0; i < sizeof(lighting) / sizeof(lighting[0]); i++) {
			lighting[i] = 0;
			isGrowing[i] = true;
		}
	}
}


void myRender() {
	if (alphaMode) {
		drawFigureBottom(false);
		drawFigureWalls();

		if (bulgeMode) {
			drawFirstCircleWalls();
			drawFirstCircleBottom(false);
		}

		if (cutoutMode) {
			drawSecondCircleWalls();
			drawSecondCircleBottom(false);
		}

		drawFigureTop();
		if (bulgeMode) drawFirstCircleTop();
		if (cutoutMode) drawSecondCircleTop();
	} else {
		drawFigure();
		if (bulgeMode) drawFirstCircle();
		if (cutoutMode) drawSecondCircle();
	}
	animationControl();
}
//===============================================================================

// Класс для настройки камеры
class CustomCamera : public Camera {
public:
	//дистанция камеры
	double camDist;
	//углы поворота камеры
	double fi1, fi2;

	//значния масеры по умолчанию
	CustomCamera() {
		camDist = 15;
		fi1 = 1;
		fi2 = 1;
	}

	//считает позицию камеры, исходя из углов поворота, вызывается движком
	void SetUpCamera() {
		//отвечает за поворот камеры мышкой
		lookPoint.setCoords(0, 0, 0);

		pos.setCoords(camDist * cos(fi2) * cos(fi1),
					  camDist * cos(fi2) * sin(fi1),
					  camDist * sin(fi2));

		if (cos(fi2) <= 0) {
			normal.setCoords(0, 0, -1);
		} else normal.setCoords(0, 0, 1);

		LookAt();
	}

	void CustomCamera::LookAt() {
		//функция настройки камеры
		gluLookAt(pos.X(), pos.Y(), pos.Z(), lookPoint.X(), lookPoint.Y(), lookPoint.Z(), normal.X(), normal.Y(), normal.Z());
	}
}  camera; // создаем объект камеры

// Класс для настройки света
class CustomLight : public Light {
public:
	CustomLight() {
		//начальная позиция света
		pos = Vector3(1, 1, 3);
	}

	//рисует сферу и линии под источником света, вызывается движком
	void  DrawLightGhismo() {
		glDisable(GL_LIGHTING);

		glColor3d(0.9, 0.8, 0);
		Sphere s;
		s.pos = pos;
		s.scale = s.scale * 0.08;
		s.Show();

		if (OpenGL::isKeyPressed('G')) {
			glColor3d(0, 0, 0);
			//линия от источника света до окружности
			glBegin(GL_LINES);
			glVertex3d(pos.X(), pos.Y(), pos.Z());
			glVertex3d(pos.X(), pos.Y(), 0);
			glEnd();

			//рисуем окруность
			Circle c;
			c.pos.setCoords(pos.X(), pos.Y(), 0);
			c.scale = c.scale * 1.5;
			c.Show();
		}
	}

	void SetUpLight() {
		GLfloat amb[] = { 0.2, 0.2, 0.2, 0 };
		GLfloat dif[] = { 1.0, 1.0, 1.0, 0 };
		GLfloat spec[] = { .7, .7, .7, 0 };
		GLfloat position[] = { pos.X(), pos.Y(), pos.Z(), 1. };

		// параметры источника света
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		// характеристики излучаемого света
		// фоновое освещение (рассеянный свет)
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		// диффузная составляющая света
		glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
		// зеркально отражаемая составляющая света
		glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

		glEnable(GL_LIGHT0);
	}


} light; // создаем источник света


// старые координаты мыши
int mouseX = 0, mouseY = 0;

void mouseEvent(OpenGL *ogl, int mX, int mY) {
	int dx = mouseX - mX;
	int dy = mouseY - mY;
	mouseX = mX;
	mouseY = mY;

	//меняем углы камеры при нажатой левой кнопке мыши
	if (OpenGL::isKeyPressed(VK_RBUTTON)) {
		camera.fi1 += 0.01 * dx;
		camera.fi2 += -0.01 * dy;
	}

	//двигаем свет по плоскости, в точку где мышь
	if (OpenGL::isKeyPressed('G') && !OpenGL::isKeyPressed(VK_LBUTTON)) {
		LPPOINT POINT = new tagPOINT();
		GetCursorPos(POINT);
		ScreenToClient(ogl->getHwnd(), POINT);
		POINT->y = ogl->getHeight() - POINT->y;

		Ray r = camera.getLookRay(POINT->x, POINT->y);

		double z = light.pos.Z();
		double k = 0, x = 0, y = 0;

		if (r.direction.Z() == 0) {
			k = 0;
		} else k = (z - r.origin.Z()) / r.direction.Z();

		x = k * r.direction.X() + r.origin.X();
		y = k * r.direction.Y() + r.origin.Y();

		light.pos = Vector3(x, y, z);
	}

	if (OpenGL::isKeyPressed('G') && OpenGL::isKeyPressed(VK_LBUTTON)) {
		light.pos = light.pos + Vector3(0, 0, 0.02 * dy);
	}
}

void mouseWheelEvent(OpenGL *ogl, int delta) {
	if (delta < 0 && camera.camDist <= 1) return;
	if (delta > 0 && camera.camDist >= 100)	return;

	camera.camDist += 0.01 * delta;
}


void keyDownEvent(OpenGL *ogl, int key) {
	if (key == 'C') colorMode = !colorMode;
	if (key == 'T' && !alphaMode) textureMode = !textureMode;
	if (key == 'A' && !textureMode) alphaMode = !alphaMode;
	if (key == 'S') bulgeMode = !bulgeMode;
	if (key == 'D') cutoutMode = !cutoutMode;
	if (key == 'H') {
		animationMode = !animationMode;
		isSwitched = animationMode;
	}
	if (key == 'P' && alpha < 1) alpha += 0.05;
	if (key == 'O' && alpha > 0) alpha -= 0.05;
	if (key == 'B') {
		if (textureIndex < (sizeof(textures) / sizeof(unsigned int) - 1)) {
			textureIndex++;
		} else textureIndex = 0;
	}
	if (key == 'R') {
		camera.fi1 = 1;
		camera.fi2 = 1;
		camera.camDist = 15;

		light.pos = Vector3(1, 1, 3);
	}

	if (colorMode) {
		lightMode = false;
		if (key == '1') partColor = 0;
		if (key == '2') partColor = 1;
		if (key == '3') partColor = 2;
		if (key == '4') partColor = 3;
		if (key == '5') partColor = 4;
		if (key == '6') partColor = 5;
		if (key == '7') partColor = 6;
		if (key == '8') partColor = 7;
		if (key == '9') partColor = 8;

		if (key == 'K' && chooseRGB > 0) chooseRGB--;
		if (key == 'L' && chooseRGB < 2) chooseRGB++;

		if (key == 'N' && figureColors[partColor][chooseRGB] > 0) figureColors[partColor][chooseRGB] -= 1.0 / 255.0;
		if (key == 'M' && figureColors[partColor][chooseRGB] < 1) figureColors[partColor][chooseRGB] += 1.0 / 255.0;
	} else {
		if (key == 'L') lightMode = !lightMode;
		if (key == 'F') light.pos = camera.pos;
	}
}

void keyUpEvent(OpenGL *ogl, int key) { }


// Выполняется перед первым рендером
void initRender(OpenGL *ogl) {
	calculateStaticParams();
	splitFirstCircleTexture();
	splitSecondCircleTexture();

	//камеру и свет привязываем к "движку"
	ogl->mainCamera = &camera;
	ogl->mainLight = &light;

	// нормализация нормалей : их длины будет равна 1
	glEnable(GL_NORMALIZE);

	// устранение ступенчатости для линий
	glEnable(GL_LINE_SMOOTH);

	//  задать параметры освещения
	//  параметр GL_LIGHT_MODEL_TWO_SIDE - 
	//                0 -  лицевые и изнаночные рисуются одинаково(по умолчанию), 
	//                1 - лицевые и изнаночные обрабатываются разными режимами       
	//                соответственно лицевым и изнаночным свойствам материалов.    
	//  параметр GL_LIGHT_MODEL_AMBIENT - задать фоновое освещение, 
	//                не зависящее от сточников
	//  по умолчанию (0.2, 0.2, 0.2, 1.0)

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);

	camera.fi1 = -1.3;
	camera.fi2 = 0.8;
}

void Render(OpenGL *ogl) {
	glEnable(GL_DEPTH_TEST);

	textureMode ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);
	lightMode ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
	alphaMode ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// настройка материала
	GLfloat amb[] = { 0.2, 0.2, 0.1, 1. };
	GLfloat dif[] = { 0.4, 0.65, 0.5, 1. };
	GLfloat spec[] = { 0.9, 0.8, 0.3, 1. };
	GLfloat sh = 0.1f * 256;

	// фоновая
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	// дифузная
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	// зеркальная
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	// размер блика
	glMaterialf(GL_FRONT, GL_SHININESS, sh);

	// чтоб было красиво, без квадратиков (сглаживание освещения)
	glShadeModel(GL_SMOOTH);
	//===================================
	// Прогать тут
	glBindTexture(GL_TEXTURE_2D, textures[textureIndex]);
	myRender();
	//===================================

   // Cообщение вверху экрана
	glMatrixMode(GL_PROJECTION);	// Делаем активной матрицу проекций. 
									// (всек матричные операции, будут ее видоизменять.)
	glPushMatrix();					// сохраняем текущую матрицу проецирования (которая описывает перспективную проекцию) в стек 				    
	glLoadIdentity();				// Загружаем единичную матрицу
	glOrtho(0, ogl->getWidth(), 0, ogl->getHeight(), 0, 1);	 // врубаем режим ортогональной проекции
	glMatrixMode(GL_MODELVIEW);		// переключаемся на модел-вью матрицу
	glPushMatrix();					// сохраняем текущую матрицу в стек (положение камеры, фактически)
	glLoadIdentity();				// сбрасываем ее в дефолт
	glDisable(GL_LIGHTING);


	GuiTextRectangle rec;			// классик моего авторства для удобной работы с рендером текста.
	rec.setSize(300, 400);
	rec.setPosition(10, ogl->getHeight() - 400 - 10);

	std::stringstream ss;
	std::string textureModeState = textureMode ? "[вкл]" : "[выкл]";
	std::string lightModeState = lightMode ? "[вкл]" : "[выкл]";
	std::string alphaModeState = alphaMode ? "[вкл]" : "[выкл]";
	std::string bulgeModeState = bulgeMode ? "[вкл]" : "[выкл]";
	std::string cutoutModeState = cutoutMode ? "[вкл]" : "[выкл]";
	std::string animationModeState = animationMode ? "[вкл]" : "[выкл]";

	ss << "T -> вкл/выкл текстур " << textureModeState << std::endl;
	if (!colorMode) ss << "L -> вкл/выкл освещение " << lightModeState << std::endl;
	if (!colorMode) ss << "F -> Свет из камеры" << std::endl;
	ss << "G -> двигать свет по горизонтали" << std::endl;
	ss << "G + ЛКМ -> двигать свет по вертекали" << std::endl;

	ss << "A -> вкл/выкл альфа наложение " << alphaModeState << std::endl;
	ss << "S -> вкл/выкл выпуклость " << bulgeModeState << std::endl;
	ss << "D -> вкл/выкл впуклость " << cutoutModeState << std::endl;
	ss << "H -> вкл/выкл анимацию " << animationModeState << std::endl;
	ss << "P -> увеличить альфа наложение" << std::endl;
	ss << "O -> уменьшить альфа наложение" << std::endl;
	ss << "B -> переключить текстуру" << std::endl;
	if (colorMode) {
		ss << "C -> выключить редактирование цветов ";
	} else ss << "C -> включить редактирование цветов ";
	ss << std::endl << std::endl;

	if (!colorMode) ss << "Light Coords: (" << light.pos.X() << ", " << light.pos.Y() << ", " << light.pos.Z() << ")" << std::endl;
	ss << "Cam Coords: (" << camera.pos.X() << ", " << camera.pos.Y() << ", " << camera.pos.Z() << ")" << std::endl;
	ss << "Cam Params: R = " << camera.camDist << ", fi1 = " << camera.fi1 << ", fi2 = " << camera.fi2 << std::endl;
	ss << "Alpha: " << alpha << std::endl;

	if (colorMode) {
		ss << std::endl << "Режим редактирования цветов" << std::endl;
		ss << "[1 - 9] -> выбор части фигуры" << std::endl;
		ss << "[K, L] -> выбор составляющей RGB" << std::endl;
		ss << "N -> уменьшить составляющую RGB" << std::endl;
		ss << "M -> увеличить составляющую RGB" << std::endl << std::endl;
		ss << "Смена цвета: ";

		switch (partColor) {
			case 0:
				ss << "Призма Верх";
				break;
			case 1:
				ss << "Призма Низ";
				break;
			case 2:
				ss << "Призма Стенки";
				break;
			case 3:
				ss << "Выпуклость Верх";
				break;
			case 4:
				ss << "Выпуклость Низ";
				break;
			case 5:
				ss << "Выпуклость Стенки";
				break;
			case 6:
				ss << "Вырез Верх";
				break;
			case 7:
				ss << "Вырез Низ";
				break;
			case 8:
				ss << "Вырез Стенки";
				break;
		}

		ss << std::endl << "RGB: ";
		double *color = &figureColors[partColor][0];
		int red = int(color[0] * 255);
		int green = int(color[1] * 255);
		int blue = int(color[2] * 255);

		switch (chooseRGB) {
			case 0:
				ss << "[" << red << "], ";
				ss << green << ", ";
				ss << blue << std::endl;
				break;
			case 1:
				ss << red << ", ";
				ss << "[" << green << "], ";
				ss << blue << std::endl;
				break;
			case 2:
				ss << red << ", ";
				ss << green << ", ";
				ss << "[" << blue << "]" << std::endl;
				break;
		}
		ss << std::endl;

	}

	rec.setText(ss.str().c_str());
	rec.Draw();

	glMatrixMode(GL_PROJECTION);	  // восстанавливаем матрицы проекции и модел-вью обратьно из стека.
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}