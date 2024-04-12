#define OLC_PGE_APPLICATION
#define _USE_MATH_DEFINES

#include "olcPixelGameEngine.h"
//#include <iostream>
#include <math.h>
//#include <string>

using namespace std;

// Override base class with your custom functionality
class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		// Name your application
		sAppName = "2D Transformations";
	}

private:
	int counter = 0, t_count = 0, r_count = 0, s_count = 0, sh_count = 0, rf_count = 0, start = 0;
	int x, y, theta = 0, running = 0, translating = 0, rotating = 0, scaling = 0, shearing = 0, reflecting = 0;
	olc::vd2d A, B, C, D, t1, t2, t3, t4, center, txt_size;
	string text = "None", tl = "";

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::GREEN);
		
		if (GetKey(olc::Key::SPACE).bPressed && !start) {
			counter = 4;
			start = 1;

			A.x = t1.x = 100;
			A.y = t1.y = 100;

			B.x = t2.x = 100;
			B.y = t2.y = 300;

			C.x = t3.x = 300;
			C.y = t3.y = 300;

			D.x = t4.x = 300;
			D.y = t4.y = 100;
		}

		if (counter < 4){
			if (GetMouse(0).bPressed) {
				x = GetMouseX();
				y = GetMouseY();

				switch (counter) {
				case 0: A.x = x;
						A.y = y;

						// temporary A
						t1.x = x;
						t1.y = y;

						counter++;
						break;

				case 1: B.x = x;
						B.y = y;

						// temporary B
						t2.x = x;
						t2.y = y;

						counter++;
						break;

				case 2: C.x = x;
						C.y = y;

						// temporary C
						t3.x = x;
						t3.y = y;

						counter++;
						break;

				case 3: D.x = x;
						D.y = y;

						// temporary D
						t4.x = x;
						t4.y = y;

						counter++;
						break;
				}
			}
		}
		
		// main running block where everything that is to be displayed and updated will happen
		if (counter == 4) {
			DrawObj();

			if (GetKey(olc::Key::T).bPressed && !rotating && !scaling && !shearing && !reflecting) {
				translating = 1;
				t_count++;
			}

			else if (GetKey(olc::Key::R).bPressed && !translating && !scaling && !shearing && !reflecting) {
				rotating = 1;
				r_count++;
			}

			else if (GetKey(olc::Key::S).bPressed && !rotating && !translating && !shearing && !reflecting) {
				scaling = 1;
				s_count++;
			}

			else if (GetKey(olc::Key::H).bPressed && !rotating && !scaling && !translating && !reflecting) {
				shearing = 1;
				sh_count++;
			}

			else if (GetKey(olc::Key::F).bPressed && !rotating && !scaling && !translating && !shearing) {
				reflecting = 1;
				rf_count++;
			}

			if (translating) {
				translate();

				if (t_count % 2 == 0) {
					translating = 0;
					t_count = 0;

					// Updating the transformation text
					text = "None";
				}
			}

			if (rotating) {
				rotate();

				if (r_count % 2 == 0) {
					rotating = 0;
					r_count = 0;

					// Updating the transformation text
					text = "None";
				}
			}

			if (scaling) {
				scale();

				if (s_count % 2 == 0) {
					scaling = 0;
					s_count = 0;

					// Updating the transformation text
					text = "None";
				}
			}

			if (shearing) {
				shear();

				if (sh_count % 2 == 0) {
					shearing = 0;
					sh_count = 0;

					// Updating the transformation text
					text = "None";
				}
			}

			if (reflecting) {
				reflect();

				if (rf_count % 2 == 0) {
					reflecting = 0;
					rf_count = 0;

					// Updating the transformation text
					text = "None";
				}
			}
		}

		// function to reset the object and variables
		if (GetKey(olc::Key::NP0).bPressed) {
			A.x = t1.x;
			A.y = t1.y;

			B.x = t2.x;
			B.y = t2.y;

			C.x = t3.x;
			C.y = t3.y;

			D.x = t4.x;
			D.y = t4.y;

			theta = 0;
			translating = 0, rotating = 0, scaling = 0, shearing = 0, reflecting = 0;
			t_count = 0, r_count = 0, s_count = 0, sh_count = 0, rf_count = 0;

			text = "None";
		}

		// get the angle of point A anytime by pressing CAPS_LOCK
		if (GetKey(olc::Key::CAPS_LOCK).bPressed) {
			findAngle();
		}

		if (GetKey(olc::Key::M).bPressed) {
			translate(center.x, center.y);
		}


		// Displaying the current transformation's name on the top of the screen
		txt_size = GetTextSize(text);
		DrawString(ScreenWidth() / 2 - txt_size.x / 2, 20, text, olc::BLACK);

		return true;
	}

	void DrawObj() {
		string t1, t2, res;
		olc::vi2d t_size;

		DrawLine(A, B, olc::BLACK);
		DrawLine(B, C, olc::BLACK);
		DrawLine(C, D, olc::BLACK);
		DrawLine(D, A, olc::BLACK);

		// calculating the object's center
		center.x = (A.x + B.x + C.x + D.x) / 4;
		center.y = (A.y + B.y + C.y + D.y) / 4;

		// drawing center point of the object
		FillCircle(center, 1, olc::RED);

		// drawiing the coordinates for A
		t1 = to_string(int(A.x));
		t2 = to_string(int(A.y));
		res = t1 + "," + t2;

		t_size = GetTextSize(res);
		DrawString(A.x - t_size.x / 2, A.y, res, olc::BLACK);

		// drawiing the coordinates for B
		t1 = to_string(int(B.x));
		t2 = to_string(int(B.y));
		res = t1 + "," + t2;

		t_size = GetTextSize(res);
		DrawString(B.x - t_size.x / 2, B.y, res, olc::BLACK);

		// drawiing the coordinates for C
		t1 = to_string(int(C.x));
		t2 = to_string(int(C.y));
		res = t1 + "," + t2;

		t_size = GetTextSize(res);
		DrawString(C.x - t_size.x / 2, C.y, res, olc::BLACK);

		// drawiing the coordinates for D
		t1 = to_string(int(D.x));
		t2 = to_string(int(D.y));
		res = t1 + "," + t2;

		t_size = GetTextSize(res);
		DrawString(D.x - t_size.x / 2, D.y, res, olc::BLACK);

		theta %= 360;
		DrawString(100, 10, to_string(theta), olc::BLACK);
	}

	void translate(double centx = 0, double centy = 0) {
		// Updating the text according to the current transformation
		text = "Translation";

		// translating centroid to 0,0
		if (centx != 0 || centy != 0) {
			A.x += centx;
			A.y += centy;
			B.x += centx;
			B.y += centy;
			C.x += centx;
			C.y += centy;
			D.x += centx;
			D.y += centy; 
		}

		// up
		if (GetKey(olc::Key::UP).bHeld) {
			A.y--;
			B.y--;
			C.y--;
			D.y--;
		}

		// down
		if (GetKey(olc::Key::DOWN).bHeld) {
			A.y++;
			B.y++;
			C.y++;
			D.y++;
		}

		// left
		if (GetKey(olc::Key::LEFT).bHeld) {
			A.x--;
			B.x--;
			C.x--;
			D.x--;
		}

		// right
		if (GetKey(olc::Key::RIGHT).bHeld) {
			A.x++;
			B.x++;
			C.x++;
			D.x++;
		}
	}

	void rotate(double deg = 1) {
		double step, tx, ty, SIN, COS, cx, cy, sx, sy, I1, K1, I2, K2, tempX[4] = { A.x, B.x, C.x, D.x };

		text = "Rotation";
 
		tx = center.x;
		ty = center.y;

		SIN = sin(degrees_to_radian(abs(deg)));
		COS = cos(degrees_to_radian(abs(deg)));

		cx = COS * tx;
		cy = COS * ty;
		sx = SIN * tx;
		sy = SIN * ty;

		I1 = -cx + sy + tx;
		K1 = -sx - cy + ty;

		I2 = -cx - sy + tx;
		K2 = sx - cy + ty;

		if (deg == 1) {
			// right || CLOCKWISE
			if (GetKey(olc::Key::RIGHT).bHeld) {
				theta++;

				A.x = COS * A.x - SIN * A.y + I1;
				A.y = SIN * tempX[0] + COS * A.y + K1;

				B.x = COS * B.x - SIN * B.y + I1;
				B.y = SIN * tempX[1] + COS * B.y + K1;

				C.x = COS * C.x - SIN * C.y + I1;
				C.y = SIN * tempX[2] + COS * C.y + K1;

				D.x = COS * D.x - SIN * D.y + I1;
				D.y = SIN * tempX[3] + COS * D.y + K1;
			}

			// left || ANTI-CLOCKWISE
			if (GetKey(olc::Key::LEFT).bHeld) {
				theta--;

				A.x = COS * A.x + SIN * A.y + I2;
				A.y = -SIN * tempX[0] + COS * A.y + K2;

				B.x = COS * B.x + SIN * B.y + I2;
				B.y = -SIN * tempX[1] + COS * B.y + K2;

				C.x = COS * C.x + SIN * C.y + I2;
				C.y = -SIN * tempX[2] + COS * C.y + K2;

				D.x = COS * D.x + SIN * D.y + I2;
				D.y = -SIN * tempX[3] + COS * D.y + K2;
			}
		}

		else {
			if (deg > 0) {
				theta += deg;

				A.x = COS * A.x - SIN * A.y + I1;
				A.y = SIN * tempX[0] + COS * A.y + K1;

				B.x = COS * B.x - SIN * B.y + I1;
				B.y = SIN * tempX[1] + COS * B.y + K1;

				C.x = COS * C.x - SIN * C.y + I1;
				C.y = SIN * tempX[2] + COS * C.y + K1;

				D.x = COS * D.x - SIN * D.y + I1;
				D.y = SIN * tempX[3] + COS * D.y + K1;
			}

			if (deg < 0) {
				theta += deg;

				A.x = COS * A.x + SIN * A.y + I2;
				A.y = -SIN * tempX[0] + COS * A.y + K2;

				B.x = COS * B.x + SIN * B.y + I2;
				B.y = -SIN * tempX[1] + COS * B.y + K2;

				C.x = COS * C.x + SIN * C.y + I2;
				C.y = -SIN * tempX[2] + COS * C.y + K2;

				D.x = COS * D.x + SIN * D.y + I2;
				D.y = -SIN * tempX[3] + COS * D.y + K2;
			}
		}
	}

	void scale() {
		double temp_theta = 0;
		text = "Scaling";
		// up
		if (GetKey(olc::Key::UP).bPressed) {
			temp_theta = theta;
			rotate(-theta);
			translate(-center.x, -center.y);
			A.y *= 1.1;
			B.y *= 1.1;
			C.y *= 1.1;
			D.y *= 1.1;
			translate(center.x, center.y);
			rotate(temp_theta);
		}

		// down
		if (GetKey(olc::Key::DOWN).bPressed) {
			temp_theta = theta;
			rotate(-theta);
			translate(-center.x, -center.y);
			A.y *= 0.9;
			B.y *= 0.9;
			C.y *= 0.9;
			D.y *= 0.9;
			translate(center.x, center.y);
			rotate(temp_theta);
		}

		// left
		if (GetKey(olc::Key::LEFT).bPressed) {
			temp_theta = theta;
			rotate(-theta);
			translate(-center.x, -center.y);
			A.x *= 0.9;
			B.x *= 0.9;
			C.x *= 0.9;
			D.x *= 0.9;
			translate(center.x, center.y);
			rotate(temp_theta);
		}

		// right
		if (GetKey(olc::Key::RIGHT).bPressed) {
			temp_theta = theta;
			rotate(-theta);
			translate(-center.x, -center.y);
			A.x *= 1.1;
			B.x *= 1.1;
			C.x *= 1.1;
			D.x *= 1.1;
			translate(center.x, center.y);
			rotate(temp_theta);
		}
	}

	void shear() {
		double tempX[4] = { A.x, B.x, C.x, D.x }, shx = 0.001, shy = 0.001, temp_theta = 0;

		text = "Shearing";

		if (GetKey(olc::Key::RIGHT).bHeld) {
			temp_theta = theta;

			rotate(-theta);
			B.x += shx * B.y;
			C.x += shx * C.y;
			rotate(temp_theta);
		}

		if (GetKey(olc::Key::LEFT).bHeld) {
			temp_theta = theta;

			rotate(-theta);
			B.x -= shx * B.y;
			C.x -= shx * C.y;
			rotate(temp_theta);
		}

		if (GetKey(olc::Key::UP).bHeld) {
			temp_theta = theta;

			rotate(-theta);
			D.y -= shy * D.x;
			C.y -= shy * C.x;
			rotate(temp_theta);
		}

		if (GetKey(olc::Key::DOWN).bHeld) {
			temp_theta = theta;

			rotate(-theta);
			D.y += shy * D.x;
			C.y += shy * C.x;
			rotate(temp_theta);
		}
	}

	void reflect() {
		text = "Reflecting";

		olc::vi2d screen_center = {ScreenWidth() / 2, ScreenHeight() / 2};

		// drawing the x and y axes
		DrawLine(0, screen_center.y, ScreenWidth(), screen_center.y, olc::BLACK);		// x axis
		DrawLine(screen_center.x, 0, screen_center.x, ScreenHeight(), olc::BLACK);		// y axis

		// Y-axis reflection
		if (GetKey(olc::Key::RIGHT).bPressed || GetKey(olc::Key::LEFT).bPressed) {
			A.x = screen_center.x + (screen_center.x - A.x);
			B.x = screen_center.x + (screen_center.x - B.x);
			C.x = screen_center.x + (screen_center.x - C.x);
			D.x = screen_center.x + (screen_center.x - D.x);
		}

		// X-axis reflection
		if (GetKey(olc::Key::UP).bPressed || GetKey(olc::Key::DOWN).bPressed) {
			A.y = screen_center.y + (screen_center.y - A.y);
			B.y = screen_center.y + (screen_center.y - B.y);
			C.y = screen_center.y + (screen_center.y - C.y);
			D.y = screen_center.y + (screen_center.y - D.y);
		}
	}

	double degrees_to_radian(double deg)
	{
		return deg * M_PI / 180.0;
	}

	/*void getTopLeft() {
		vector <double> x_arr = { A.x, B.x, C.x, D.x };
		vector <double> y_arr = { A.y, B.y, C.y, D.y };

		sort(x_arr.begin(), x_arr.end());
		sort(y_arr.begin(), y_arr.end());

		cout << "X array" << endl;
		for (auto i = x_arr.begin(); i != x_arr.end(); ++i)
			cout << *i << " ";

		cout << "\nY array" << endl;
		for (auto i = y_arr.begin(); i != y_arr.end(); ++i)
			cout << *i << " ";

		if (A.x == x_arr[0] && A.y == y_arr[0]) {
			tl = "A";
		}
		else if (B.x == x_arr[0] && B.y == y_arr[0]) {
			tl = "B";
		}
		else if (C.x == x_arr[0] && C.y == y_arr[0]) {
			tl = "C";
		}
		else if (D.x == x_arr[0] && D.y == y_arr[0]) {
			tl = "D";
		}
		
		cout << "\nTL = " << tl << endl;
	}*/

	void findAngle() {
		double result = atan2(D.y - A.y, D.x - A.x) - atan2(B.y - A.y, B.x - A.x);

		result = result * 180 / M_PI;
		cout << "Result: " << result << endl;
	}
};

int main()
{
	Example demo;
	if (demo.Construct(600, 600, 1, 1))
		demo.Start();
	return 0;
}