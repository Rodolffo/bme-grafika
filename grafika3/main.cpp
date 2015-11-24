//=============================================================================================
// Szamitogepes grafika hazi feladat keret. Ervenyes 2014-tol.          
// A //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sorokon beluli reszben celszeru garazdalkodni, mert a tobbit ugyis toroljuk. 
// A beadott program csak ebben a fajlban lehet, a fajl 1 byte-os ASCII karaktereket tartalmazhat. 
// Tilos:
// - mast "beincludolni", illetve mas konyvtarat hasznalni
// - faljmuveleteket vegezni (printf is fajlmuvelet!)
// - new operatort hivni az onInitialization fßggvÊnyt kivÊve, a lefoglalt adat korrekt felszabadítåsa nÊlkßl 
// - felesleges programsorokat a beadott programban hagyni
// - tovabbi kommenteket a beadott programba irni a forrasmegjelolest kommentjeit kiveve
// ---------------------------------------------------------------------------------------------
// A feladatot ANSI C++ nyelvu forditoprogrammal ellenorizzuk, a Visual Studio-hoz kepesti elteresekrol
// es a leggyakoribb hibakrol (pl. ideiglenes objektumot nem lehet referencia tipusnak ertekul adni)
// a hazibeado portal ad egy osszefoglalot.
// ---------------------------------------------------------------------------------------------
// A feladatmegoldasokban csak olyan gl/glu/glut fuggvenyek hasznalhatok, amelyek
// 1. Az oran a feladatkiadasig elhangzottak ES (logikai AND muvelet)
// 2. Az alabbi listaban szerepelnek:  
// Rendering pass: glBegin, glVertex[2|3]f, glColor3f, glNormal3f, glTexCoord2f, glEnd, glDrawPixels
// Transzformaciok: glViewport, glMatrixMode, glLoadIdentity, glMultMatrixf, gluOrtho2D, 
// glTranslatef, glRotatef, glScalef, gluLookAt, gluPerspective, glPushMatrix, glPopMatrix,
// Illuminacio: glMaterialfv, glMaterialfv, glMaterialf, glLightfv
// Texturazas: glGenTextures, glBindTexture, glTexParameteri, glTexImage2D, glTexEnvi, 
// Pipeline vezerles: glShadeModel, glEnable/Disable a kovetkezokre:
// GL_LIGHTING, GL_NORMALIZE, GL_DEPTH_TEST, GL_CULL_FACE, GL_TEXTURE_2D, GL_BLEND, GL_LIGHT[0..7]
//
// NYILATKOZAT
// ---------------------------------------------------------------------------------------------
// Nev    : Tran Thanh Long
// Neptun : K4H9ZB
// ---------------------------------------------------------------------------------------------
// ezennel kijelentem, hogy a feladatot magam keszitettem, es ha barmilyen segitseget igenybe vettem vagy 
// mas szellemi termeket felhasznaltam, akkor a forrast es az atvett reszt kommentekben egyertelmuen jeloltem. 
// A forrasmegjeloles kotelme vonatkozik az eloadas foliakat es a targy oktatoi, illetve a 
// grafhazi doktor tanacsait kiveve barmilyen csatornan (szoban, irasban, Interneten, stb.) erkezo minden egyeb 
// informaciora (keplet, program, algoritmus, stb.). Kijelentem, hogy a forrasmegjelolessel atvett reszeket is ertem, 
// azok helyessegere matematikai bizonyitast tudok adni. Tisztaban vagyok azzal, hogy az atvett reszek nem szamitanak
// a sajat kontribucioba, igy a feladat elfogadasarol a tobbi resz mennyisege es minosege alapjan szuletik dontes.  
// Tudomasul veszem, hogy a forrasmegjeloles kotelmenek megsertese eseten a hazifeladatra adhato pontokat 
// negativ elojellel szamoljak el es ezzel parhuzamosan eljaras is indul velem szemben.
//=============================================================================================

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>

#if defined(__APPLE__)                                                                                                                                                                                                            
#include <OpenGL/gl.h>                                                                                                                                                                                                            
#include <OpenGL/glu.h>                                                                                                                                                                                                           
#include <GLUT/glut.h>                                                                                                                                                                                                            
#else                                                                                                                                                                                                                             
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)                                                                                                                                                                       
#include <windows.h>                                                                                                                                                                                                              
#endif                                                                                                                                                                                                                            
#include <GL/gl.h>                                                                                                                                                                                                                
#include <GL/glu.h>                                                                                                                                                                                                               
#include <GL/glut.h>                                                                                                                                                                                                              
#endif          


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Innentol modosithatod...

//--------------------------------------------------------
// 3D Vektor
//--------------------------------------------------------
struct Vector {
	float x, y, z;

	Vector() {
		x = y = z = 0;
	}
	Vector(float x0, float y0, float z0 = 0) {
		x = x0; y = y0; z = z0;
	}
	Vector operator*(float a) {
		return Vector(x * a, y * a, z * a);
	}
	Vector operator/(float a) {
		return Vector(x / a, y / a, z / a);
	}
	Vector operator+(float a) {
		return Vector(x + a, y + a, z + a);
	}
	Vector operator+(const Vector& v) {
		return Vector(x + v.x, y + v.y, z + v.z);
	}
	Vector operator-(const Vector& v) {
		return Vector(x - v.x, y - v.y, z - v.z);
	}
	float operator*(const Vector& v) { //dot
		return (x * v.x + y * v.y + z * v.z);
	}
	Vector operator%(const Vector& v) { //cross
		return Vector(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
	}
	float Length() { return sqrt(x * x + y * y + z * z); }
	Vector norm() {
		return Vector(x / Length(), y / Length(), z / Length());
	}
};

//--------------------------------------------------------
// Spektrum illetve szin
//--------------------------------------------------------
struct Color {
	float r, g, b;

	Color() {
		r = g = b = 0;
	}
	Color(float r0, float g0, float b0) {
		r = r0; g = g0; b = b0;
	}
	Color operator*(float a) {
		return Color(r * a, g * a, b * a);
	}
	Color operator*(const Color& c) {
		return Color(r * c.r, g * c.g, b * c.b);
	}
	Color operator+(const Color& c) {
		return Color(r + c.r, g + c.g, b + c.b);
	}
};

float toRad(float deg) {
	return deg * (M_PI / 180.0f);
}

float toDeg(float rad) {
	return rad * (180.0f / M_PI);
}

/*struct Camera {
	static const int XM = 600;
	static const int YM = 600;

	Vector eye, lookat, up;

	Camera() :eye(Vector()), lookat(Vector()), up(Vector()) {}

	Camera(Vector eye, Vector lookat, Vector up) :eye(eye), lookat(lookat), up(up) {}
};*/

enum ControllKeys { W, A, S, D, Q, E, keys_num };
bool keys_down[keys_num];

struct Camera {
	static const int XM = 600;
	static const int YM = 600;
	Vector fwd, pos;
	const float speed, mouse_speed;

	Camera(float speed = 5, float mouse_speed = 0.002f) : fwd(Vector(9, 0, 4).norm()), pos(-9, 5, -4), speed(speed), mouse_speed(mouse_speed) { }

	void updatePos(float dt) {
		Vector up = Vector(0, 1, 0), right = (fwd % up).norm();
		up = (right % fwd).norm();

		if (keys_down[W] && !keys_down[S]) {
			pos = pos + fwd * speed * dt;
		}
		else if (keys_down[S] && !keys_down[W]) {
			pos = pos - fwd * speed * dt;
		}

		if (keys_down[D] && !keys_down[A]) {
			pos = pos + right * speed * dt;
		}
		else if (keys_down[A] && !keys_down[D]) {
			pos = pos - right * speed * dt;
		}

		if (keys_down[Q] && !keys_down[E]) {
			pos = pos + Vector(0, 1, 0) * speed * dt;
		}
		else if (keys_down[E] && !keys_down[Q]) {
			pos = pos - Vector(0, 1, 0) * speed * dt;
		}
	}

	void updateDir(int dx, int dy) {
		Vector y_axis = Vector(0, 1, 0), right = (fwd % y_axis).norm();
		Vector up = (right % fwd).norm();

		// Ha teljesen felfele / lefele nĂŠznĂŠnk, akkor ne forduljon ĂĄt a kamera
		float dot_up_fwd = (y_axis * fwd);
		if (dot_up_fwd > 0.95f && dy > 0) {
			dy = 0;
		}
		if (dot_up_fwd < -0.95f && dy < 0) {
			dy = 0;
		}

		// MĂłdosĂ­tsuk az nĂŠzeti irĂĄnyt
		fwd = fwd + (right * dx + up * dy) * mouse_speed;
		fwd = fwd.norm();
	}

	void applyMatrix() const {
		gluLookAt(pos.x, pos.y, pos.z, pos.x + fwd.x, pos.y + fwd.y, pos.z + fwd.z, 0, 1, 0);
	}
} camera;

struct Cylinder {
	float r, m;
	int slices;

	Cylinder(float r, float m, int slices): r(r), m(m), slices(slices) {}

	void draw() {
		//Bottom face
		glBegin(GL_TRIANGLE_FAN);
		glNormal3f(0, -1, 0);
		glVertex3f(0, 0, 0);
		for (float i = 0; i <= slices; i++)
		{
			float angle = 2*M_PI / slices * i;

			glNormal3f(0, -1, 0);
			glVertex3f(sinf(angle) * r, 0, cosf(angle) * r);
		}
		glEnd();

		//Top face
		glBegin(GL_TRIANGLE_FAN);
		glNormal3f(0, 1, 0);
		glVertex3f(0, m, 0);
		for (float i = 0; i <= slices; i++)
		{
			float angle = 2 * M_PI / slices * i;

			glNormal3f(0, 1, 0);
			glVertex3f(sinf(angle) * r, m, cosf(angle) * r);
		}
		glEnd();

		//Sides
		glBegin(GL_TRIANGLE_STRIP);
		for (float i = 0; i <= slices; i++)
		{
			float angle = 2 * M_PI / slices * i;

			glNormal3f(sinf(angle), 0, cosf(angle));
			glVertex3f(sinf(angle) * r, 0, cosf(angle) * r);
			glVertex3f(sinf(angle) * r, m, cosf(angle) * r);
		}
		glEnd();
	}
};

struct Cone {
	float r, m;
	int slices;

	Cone() : r(1), m(1), slices(16) {}

	Cone(float r, float m, int slices) : r(r), m(m), slices(slices) {}

	void draw() {
		float a = cosf(atanf(m / r)) * r;

		//Bottom face
		glBegin(GL_TRIANGLE_FAN);
		glNormal3f(0, -1, 0);
		glVertex3f(0, 0, 0);
		for (float i = 0; i <= slices; i++)
		{
			float angle = 2 * M_PI / slices * i;

			glNormal3f(0, -1, 0);
			glVertex3f(sinf(angle) * r, 0, cosf(angle) * r);
		}
		glEnd();

		//Sides
		glBegin(GL_TRIANGLE_STRIP);
		for (float i = 0; i <= slices; i++)
		{
			float angle = 2 * M_PI / slices * i;

			Vector vr = Vector(sinf(angle) * r, 0, cosf(angle) * r);
			Vector normal = vr + (Vector(0, m, 0) - vr).norm() * a;

			glNormal3f(normal.x, normal.y, normal.z);
			glVertex3f(0, m, 0);
			glVertex3f(sinf(angle) * r, 0, cosf(angle) * r);
		}
		glEnd();
	}
};

struct Sphere {
	float r;
	int slices, stacks;

	Sphere() : r(1), slices(16), stacks(16) {}

	Sphere(float r, int slices, int stacks) : r(r), slices(slices), stacks(stacks) {}

	void draw() {
		glBegin(GL_TRIANGLE_STRIP);

		for (int i = 0; i < stacks; i++)
		{
			float b1 = M_PI / stacks * i - M_PI / 2;
			float b2 = M_PI / stacks * (i+1) - M_PI / 2;

			for (int j = 0; j < slices; j++)
			{
				float a = 2 * M_PI / slices * j;
				Vector point1 = Vector(sinf(a) * cosf(b1) * r, sinf(b1) * r, cosf(a) * cosf(b1) * r);
				Vector point2 = Vector(sinf(a) * cosf(b2) * r, sinf(b2) * r, cosf(a) * cosf(b2) * r);

				glNormal3f(point1.x, point1.y, point1.z);
				glVertex3f(point1.x, point1.y, point1.z);

				glNormal3f(point2.x, point2.y, point2.z);
				glVertex3f(point2.x, point2.y, point2.z);
			}
		}

		glEnd();
	}
};

struct CatmullRom {
	Vector points[10];
	float t[10];
	int size;

	CatmullRom() : size(0) {}

	void addControlPoint(Vector point, float t) {
		if (size == 10) 
			return;

		this->points[size] = point;
		this->t[size++] = t;
	}

	Vector getHermiteCurvePoint(int i, float t) {
		if (t > (this->t)[i + 1]) {
			i++;
		}
		float deltaT = t - (this->t)[i];
		return
			a3(i) * powf(deltaT, 3) +
			a2(i) * powf(deltaT, 2) +
			a1(i) * deltaT +
			a0(i);
	}

	Vector getDerivative(int i, float t) {
		if (t > (this->t)[i + 1]) {
			i++;
		}
		float deltaT = t - (this->t)[i];
		return
			a3(i) * 3 * powf(deltaT, 2) +
			a2(i) * 2 * deltaT +
			a1(i);
	}

	int getSize() { return size; }

	float getT(int i) { return t[i]; }

	Vector getPoint(int i) { return points[i]; }

private:
	Vector getVelocity(int i) {
		if (i == 0 || i == size) {
			return Vector();
		}

		return (((points[i + 1] - points[i]) / (t[i + 1] - t[i])) +
			((points[i] - points[i - 1]) / (t[i] - t[i - 1]))) / 2;
	}

	Vector a3(int i) {
		float deltaT = t[i + 1] - t[i];
		return
			(((points[i] - points[i + 1]) * 2) / powf(deltaT, 3)) +
			((getVelocity(i + 1) + getVelocity(i)) / powf(deltaT, 2));
	}

	Vector a2(int i) {
		float deltaT = t[i + 1] - t[i];
		return
			(((points[i + 1] - points[i]) * 3) / powf(deltaT, 2)) -
			((getVelocity(i + 1) + getVelocity(i) * 2) / deltaT);
	}

	Vector a1(int i) {
		return getVelocity(i);
	}

	Vector a0(int i) {
		return points[i];
	}
};

struct BezierCurve {
	Vector points[10];
	int size;

	BezierCurve() : size(0) {}

	void addControlPoint(Vector point) {
		if (size == 10)
			return;

		this->points[size++] = point;
	}

	Vector getBezierCruvePoint(float t) {
		Vector r = Vector();

		for (int i = 0; i < size; i++)
		{
			r = r + (points[i] * B(i, t));
		}

		return r;
	}

	Vector getDerivative(float t) {
		if (t == 0) {
			t += 0.0001f;
		}
		Vector r = Vector();

		for (int i = 0; i < size; i++)
		{
			r = r + (points[i] * dB(i, t));
		}

		return r;
	}

private:
	float B(int i, float t) {
		int n = size - 1;

		float choose = 1;

		for (int j = 1; j <= i; j++)
		{
			choose *= (((float)n - (float)j + 1.0f) / (float)j);
		}

		return choose * powf(t, i) * powf(1 - t, n - i);
	}

	float dB(int i, float t) {
		int n = size - 1;

		float choose = 1;

		for (int j = 1; j <= i; j++)
		{
			choose *= (((float)n - (float)j + 1.0f) / (float)j);
		}

		return choose * (i*powf(t, i - 1) * powf(1 - t, n - i) - powf(t, i) * powf(1 - t, n - i - 1) * (n - i));
	}
};

struct Object {
	Vector position;

	bool attached;

	virtual void draw() = 0;

	void fling(Vector dir) {

	}
};

static const float HEAD_RADIUS = 0.5f;
static const float EYE_RADIUS = 0.05f;

//a CSIRGURU szeme
struct CsirguruEye : public Object {

	Sphere eyeBall;

	CsirguruEye() {
		eyeBall.r = EYE_RADIUS;
		eyeBall.slices = 16;
		eyeBall.stacks = 16;
	}

	void draw() {
		glColor3f(0, 0, 0);
		eyeBall.draw();
	}
};

//a CSIRGURU csőre
struct CsirguruBeak : public Object {

	Cone beak;

	CsirguruBeak() {
		beak.m = 0.2f;
		beak.r = 0.1f;
		beak.slices = 16;
	}

	void draw() {
		glColor3f(1, 0.647f, 0);
		beak.draw();
	}
};

//a CSIRGURU taraja
struct CsirguruComb : public Object {

	Cone comb;

	CsirguruComb() {
		comb.m = 0.4f;
		comb.r = 0.1f;
		comb.slices = 16;
	}

	void draw() {
		glColor3f(1, 0, 0);
		comb.draw();
	}
};

//a CSIGURU teste
struct CsirguruBody : public Object {

	CatmullRom spine;

	CsirguruBody() {
		spine.addControlPoint(Vector(0, 0, 0), 0);
		spine.addControlPoint(Vector(0, -0.5f, -0.1f), 1);
		spine.addControlPoint(Vector(0, -1.3f, -0.4f), 2);
		spine.addControlPoint(Vector(0, -1.4f, -1.4f), 3);
		spine.addControlPoint(Vector(0, -0.5f, -1.8f), 4);
	}

	void draw() { 

		glColor3f(0.9f, 0.9f, 0.9f);

		glBegin(GL_TRIANGLE_STRIP);
		for (int i = 0; i < spine.getSize() - 1; i++)
		{

			float step = (spine.getT(i + 1) - spine.getT(i)) / 20.0f;

			for (float t = spine.getT(i); t < spine.getT(i + 1); t += step)
			{
				//Az aktuális és a következő catmull-rom pont
				// TODO optimalizálni, így minden ponott kétszer számol ki...
				Vector curve1 = spine.getHermiteCurvePoint(i, t);
				Vector curve2 = spine.getHermiteCurvePoint(i, t + step);

				//Deriváljuk a catmull-romot
				Vector deriv1 = spine.getDerivative(i, t);
				Vector normal1 = Vector(deriv1.x, deriv1.z, -deriv1.y);

				Vector deriv2 = spine.getDerivative(i, t + step);
				Vector normal2 = Vector(deriv2.x, deriv2.z, -deriv2.y);

				//Léterhozzuk a bézier görbéket
				//Ezt is optimalizálni köll
				BezierCurve bcurve1 = BezierCurve();
				Vector cp1 = curve1 + normal1.norm() * 1;

				bcurve1.addControlPoint(curve1);
				bcurve1.addControlPoint(Vector(curve1.x - 0.5f, curve1.y, curve1.z));
				bcurve1.addControlPoint(Vector(curve1.x - 0.5f, cp1.y, cp1.z));
				bcurve1.addControlPoint(Vector(curve1.x + 0.5f, cp1.y, cp1.z));
				bcurve1.addControlPoint(Vector(curve1.x + 0.5f, curve1.y, curve1.z));
				bcurve1.addControlPoint(curve1);

				BezierCurve bcurve2 = BezierCurve();
				Vector cp2 = curve2 + normal2.norm() * 1;

				bcurve2.addControlPoint(curve2);
				bcurve2.addControlPoint(Vector(curve2.x - 0.5f, curve2.y, curve2.z));
				bcurve2.addControlPoint(Vector(curve2.x - 0.5f, cp2.y, cp2.z));
				bcurve2.addControlPoint(Vector(curve2.x + 0.5f, cp2.y, cp2.z));
				bcurve2.addControlPoint(Vector(curve2.x + 0.5f, curve2.y, curve2.z));
				bcurve2.addControlPoint(curve2);

				for (float tb = 0; tb <= 1; tb += 1.0f / 50.0f)
				{
					Vector curvePoint1 = bcurve1.getBezierCruvePoint(tb);
					Vector curvePoint2 = bcurve2.getBezierCruvePoint(tb);

					Vector normal1 = bcurve1.getDerivative(tb) % (curvePoint1 - curvePoint2);
					Vector normal2 = (curvePoint2 - curvePoint1) % bcurve2.getDerivative(tb);

					glNormal3f(normal1.x, normal1.y, normal1.z);
					glVertex3f(curvePoint1.x, curvePoint1.y, curvePoint1.z);

					glNormal3f(normal2.x, normal2.y, normal2.z);
					glVertex3f(curvePoint2.x, curvePoint2.y, curvePoint2.z);
				}

			}
		}

		glEnd();
	}
};

//a CSIRGURU lába
struct CsirguruLeg : public Object {

	void draw() {

	}
};

struct CsirguruHead : public Object {

	Sphere head;

	CsirguruHead() {
		head.r = HEAD_RADIUS;
		head.slices = 20;
		head.stacks = 20;
	}

	void explode(Vector center) {

	}

	void draw() {
		glColor3f(0.9f, 0.9f, 0.9f);
		head.draw();
	}
};

//sexy beast
struct Csirguru {
	CsirguruBody body;
	CsirguruLeg leg;
	CsirguruHead head;
	CsirguruEye eyeLeft, eyeRight;
	CsirguruBeak beak;
	CsirguruComb comb1, comb2, comb3, comb4, comb5, comb6, comb7;

	float angle;

	Csirguru() {
		angle = 0;
	}

	void jump() {
		//TODO random irány, fancy animation
	}

	void explode(Vector center) {
		//TODO fling all body parts
	}

	void draw() {

		body.position = Vector(sinf(angle + M_PI) * HEAD_RADIUS, 0, cosf(angle + M_PI) * HEAD_RADIUS);
		glPushMatrix();
		glTranslatef(body.position.x, body.position.y, body.position.z);
		body.draw();
		glPopMatrix();


		glPushMatrix();
		glTranslatef(head.position.x, head.position.y, head.position.z);
		head.draw();
		glPopMatrix();

		eyeLeft.position = Vector(sinf(angle + M_PI / 4) * HEAD_RADIUS, 0, cosf(angle + M_PI / 4) * HEAD_RADIUS);
		glPushMatrix();
		glTranslatef(eyeLeft.position.x, eyeLeft.position.y, eyeLeft.position.z);
		eyeLeft.draw();
		glPopMatrix();

		eyeRight.position = Vector(sinf(angle - M_PI / 4) * HEAD_RADIUS, 0, cosf(angle - M_PI / 4) * HEAD_RADIUS);
		glPushMatrix();
		glTranslatef(eyeRight.position.x, eyeRight.position.y, eyeRight.position.z);
		eyeRight.draw();
		glPopMatrix();

		beak.position = Vector(sinf(angle) * (HEAD_RADIUS - 0.02f), 0, cosf(angle) * (HEAD_RADIUS - 0.02f));
		glPushMatrix();
		glTranslatef(beak.position.x, beak.position.y, beak.position.z);
		glRotatef(toDeg(angle), 0, 1, 0);
		glRotatef(90, 1, 0, 0);
		beak.draw();
		glPopMatrix();

		float comb1Angle = toRad(45);
		comb1.position = Vector(sinf(angle) * cosf(comb1Angle) * (HEAD_RADIUS - 0.02f), sinf(comb1Angle) * (HEAD_RADIUS - 0.02f), cosf(angle) * cosf(comb1Angle) * (HEAD_RADIUS - 0.02f));
		glPushMatrix();
		glTranslatef(comb1.position.x, comb1.position.y, comb1.position.z);
		glRotatef(toDeg(angle), 0, 1, 0);
		glRotatef(45, 1, 0, 0);
		comb1.draw();
		glPopMatrix();

		float comb2Angle = toRad(60);
		comb1.position = Vector(sinf(angle) * cosf(comb2Angle) * (HEAD_RADIUS - 0.02f), sinf(comb2Angle) * (HEAD_RADIUS - 0.02f), cosf(angle) * cosf(comb2Angle) * (HEAD_RADIUS - 0.02f));
		glPushMatrix();
		glTranslatef(comb1.position.x, comb1.position.y, comb1.position.z);
		glRotatef(toDeg(angle), 0, 1, 0);
		glRotatef(30, 1, 0, 0);
		comb1.draw();
		glPopMatrix();

		float comb3Angle = toRad(75);
		comb1.position = Vector(sinf(angle) * cosf(comb3Angle) * (HEAD_RADIUS - 0.02f), sinf(comb3Angle) * (HEAD_RADIUS - 0.02f), cosf(angle) * cosf(comb3Angle) * (HEAD_RADIUS - 0.02f));
		glPushMatrix();
		glTranslatef(comb1.position.x, comb1.position.y, comb1.position.z);
		glRotatef(toDeg(angle), 0, 1, 0);
		glRotatef(15, 1, 0, 0);
		comb1.draw();
		glPopMatrix();

		float comb4Angle = toRad(90);
		comb1.position = Vector(sinf(angle) * cosf(comb4Angle) * (HEAD_RADIUS - 0.02f), sinf(comb4Angle) * (HEAD_RADIUS - 0.02f), cosf(angle) * cosf(comb4Angle) * (HEAD_RADIUS - 0.02f));
		glPushMatrix();
		glTranslatef(comb1.position.x, comb1.position.y, comb1.position.z);
		comb1.draw();
		glPopMatrix();

		float comb5Angle = toRad(105);
		comb1.position = Vector(sinf(angle) * cosf(comb5Angle) * (HEAD_RADIUS - 0.02f), sinf(comb5Angle) * (HEAD_RADIUS - 0.02f), cosf(angle) * cosf(comb5Angle) * (HEAD_RADIUS - 0.02f));
		glPushMatrix();
		glTranslatef(comb1.position.x, comb1.position.y, comb1.position.z);
		glRotatef(toDeg(angle), 0, 1, 0);
		glRotatef(-15, 1, 0, 0);
		comb1.draw();
		glPopMatrix();

		float comb6Angle = toRad(120);
		comb1.position = Vector(sinf(angle) * cosf(comb6Angle) * (HEAD_RADIUS - 0.02f), sinf(comb6Angle) * (HEAD_RADIUS - 0.02f), cosf(angle) * cosf(comb6Angle) * (HEAD_RADIUS - 0.02f));
		glPushMatrix();
		glTranslatef(comb1.position.x, comb1.position.y, comb1.position.z);
		glRotatef(toDeg(angle), 0, 1, 0);
		glRotatef(-30, 1, 0, 0);
		comb1.draw();
		glPopMatrix();

		float comb7Angle = toRad(135);
		comb1.position = Vector(sinf(angle) * cosf(comb7Angle) * (HEAD_RADIUS - 0.02f), sinf(comb7Angle) * (HEAD_RADIUS - 0.02f), cosf(angle) * cosf(comb7Angle) * (HEAD_RADIUS - 0.02f));
		glPushMatrix();
		glTranslatef(comb1.position.x, comb1.position.y, comb1.position.z);
		glRotatef(toDeg(angle), 0, 1, 0);
		glRotatef(-45, 1, 0, 0);
		comb1.draw();
		glPopMatrix();
	}
};

struct Bomb : public Object {
	void explode() {
		//loop through csirgurus
	}
};

struct Field {
	void draw() {
		glBegin(GL_QUADS);

		glColor3f(1, 1, 0);
		glNormal3f(0, 1, 0);

		float f = 1;

		for (float i = 0; i < 100; i += f)
		{
			for (float j = 0; j < 100; j += f)
			{
				glVertex3f(i, 0, j);
				glVertex3f(i + f, 0, j);
				glVertex3f(i + f, 0, j + f);
				glVertex3f(i, 0, j + f);
			}
		}

		glEnd();
	}
};

struct Scene {
	static const Vector SUN_LIGHT;

	Csirguru* csirgurus; //láncolt lista kéne

	Csirguru testCs;

	Field field;

	float lightColor[4];
	float lightDir[4];
	//Camera camera;

	Scene() {
		field = Field();
		testCs = Csirguru();
		//camera = Camera();
	}

	void init() {
		lightColor[0] = 1;
		lightColor[1] = 1;
		lightColor[2] = 1;
		lightColor[3] = 1;

		lightDir[0] = 0;
		lightDir[1] = 1;
		lightDir[2] = 1;
		lightDir[3] = 0;
	}

	void render() {

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		camera.applyMatrix();

		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
		glLightfv(GL_LIGHT0, GL_POSITION, lightDir);
		glEnable(GL_LIGHT0);

		glPushMatrix();
		field.draw();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-5, 5, -5);
		testCs.draw();
		glPopMatrix();

		//Test cylinder
		glPushMatrix();
		glColor3f(1, 1, 1);
		Cylinder cylinder(2, 10, 16);
		glTranslatef(10, 5, 10);
		glRotatef(30, 1, 1, 1);
		cylinder.draw();
		glPopMatrix();

		//Test cone
		glPushMatrix();
		glColor3f(0, 0, 1);
		Cone cone(2, 5, 16);
		glTranslatef(10, 5, 20);
		glRotatef(30, 1, 1, 1);
		cone.draw();
		glPopMatrix();

		//Test sphere
		glPushMatrix();
		glColor3f(0, 1, 1);
		Sphere shpere(2.5f, 16, 16);
		glTranslatef(20, 5, 10);
		shpere.draw();
		glPopMatrix();

		//4x4 mtx, árnyék mtx
		//Síkra vetett árnyékok
		float shadow[4][4] = {
			1, 0, 0, 0,
			-lightDir[0] / lightDir[1], 0, -lightDir[2] / lightDir[1], 0,
			0, 0, 1, 0,
			0, 0.001f, 0, 1
		};
		glMultMatrixf(&shadow[0][0]);

		//Árnyékolás
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);

		glColor3f(0, 0, 0);
		//Test cylinder
		glPushMatrix();
		glTranslatef(10, 5, 10);
		glRotatef(30, 1, 1, 1);
		cylinder.draw();
		glPopMatrix();

		//Test cone
		glPushMatrix();
		glTranslatef(10, 5, 20);
		glRotatef(30, 1, 1, 1);
		cone.draw();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(20, 5, 10);
		shpere.draw();
		glPopMatrix();
	}

	void addCsirguru(Vector pos) {

	}

	void dropBomb(Vector pos) {

	}
};

const Vector Scene::SUN_LIGHT = Vector();

Color image[Camera::XM*Camera::YM];
Scene scene;

void onInitialization() {
	glViewport(0, 0, Camera::XM, Camera::YM);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	scene = Scene();
	scene.init();

	//scene.camera = Camera(Vector(3, 2, -3), Vector(5, 0, 5), Vector(0, 1, 0));

	//Rendes 3d
	glEnable(GL_DEPTH_TEST);
	//Normál vaktorokat egység vektorokként kezelni
	glEnable(GL_NORMALIZE);
	//Világítás engedélyezése
	glEnable(GL_LIGHTING);


	glMatrixMode(GL_PROJECTION);
	gluPerspective(54, 1, 0.2, 200);
	glMatrixMode(GL_MODELVIEW);
	camera.applyMatrix();
	/*gluLookAt(scene.camera.eye.x, scene.camera.eye.y, scene.camera.eye.z,
		scene.camera.lookat.x, scene.camera.lookat.y, scene.camera.lookat.z,
		scene.camera.up.x, scene.camera.up.y, scene.camera.up.z);*/

	//Ambiens fény?
	glEnable(GL_COLOR_MATERIAL);
	//Irányfényforrás
	glLightfv(GL_LIGHT0, GL_DIFFUSE, scene.lightColor);
	glLightfv(GL_LIGHT0, GL_POSITION, scene.lightDir);
	glEnable(GL_LIGHT0);

	glEnable(GL_SMOOTH);
}

void onDisplay() {
	glClearColor(0, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	scene.render();

	glutSwapBuffers();

}

void onKeyboard(unsigned char key, int x, int y) {
	/*switch (key)
	{
	case 'a':
		//TODO move crosshair left
		break;
	case 'w':
		//TODO move crosshair up
		break;
	case 'd':
		//TODO move crosshair right
		break;
	case 'y''s':
		//TODO move crosshair down
		break;
	case ' ':
		//TODO drop da bomb
		break;
	default:
		//nothing
		break;
	}*/

	switch (key) {
	case 'w': case 'W':
		keys_down[W] = true;
		break;
	case 's': case 'S':
		keys_down[S] = true;
		break;
	case 'a': case 'A':
		keys_down[A] = true;
		break;
	case 'd': case 'D':
		keys_down[D] = true;
		break;
	case 'q': case 'Q':
		keys_down[Q] = true;
		break;
	case 'e': case 'E':
		keys_down[E] = true;
		break;
	}
}

void onKeyboardUp(unsigned char key, int x, int y) {
	switch (key) {
	case 'w': case 'W':
		keys_down[W] = false;
		break;
	case 's': case 'S':
		keys_down[S] = false;
		break;
	case 'a': case 'A':
		keys_down[A] = false;
		break;
	case 'd': case 'D':
		keys_down[D] = false;
		break;
	case 'q': case 'Q':
		keys_down[Q] = false;
		break;
	case 'e': case 'E':
		keys_down[E] = false;
		break;
	}
}

int last_x, last_y;
void onMouse(int button, int state, int x, int y) {
	last_x = x;
	last_y = y;

}

void onMouseMotion(int x, int y) {
	camera.updateDir(x - last_x, last_y - y);
	last_x = x;
	last_y = y;

}

void onIdle() {
	static float last_time = glutGet(GLUT_ELAPSED_TIME);
	float time = glutGet(GLUT_ELAPSED_TIME);
	float dt = (time - last_time) / 1000.0f;
	last_time = time;

	camera.updatePos(dt);
	glutPostRedisplay();
}

// ...Idaig modosithatod
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// A C++ program belepesi pontja, a main fuggvenyt mar nem szabad bantani
int main(int argc, char **argv) {
	glutInit(&argc, argv); 				// GLUT inicializalasa
	glutInitWindowSize(600, 600);			// Alkalmazas ablak kezdeti merete 600x600 pixel 
	glutInitWindowPosition(100, 100);			// Az elozo alkalmazas ablakhoz kepest hol tunik fel
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);	// 8 bites R,G,B,A + dupla buffer + melyseg buffer

	glutCreateWindow("Grafika hazi feladat");		// Alkalmazas ablak megszuletik es megjelenik a kepernyon

	glMatrixMode(GL_MODELVIEW);				// A MODELVIEW transzformaciot egysegmatrixra inicializaljuk
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);			// A PROJECTION transzformaciot egysegmatrixra inicializaljuk
	glLoadIdentity();

	onInitialization();					// Az altalad irt inicializalast lefuttatjuk

	glutDisplayFunc(onDisplay);				// Esemenykezelok regisztralasa
	glutMouseFunc(onMouse);
	glutIdleFunc(onIdle);
	glutKeyboardFunc(onKeyboard);
	glutKeyboardUpFunc(onKeyboardUp);
	glutMotionFunc(onMouseMotion);

	glutMainLoop();					// Esemenykezelo hurok

	return 0;
}
