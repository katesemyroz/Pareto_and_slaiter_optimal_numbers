#include <gl/glut.h>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#define nums 7			//must be = numers.size()


//vector <int> numbers = { 05, 04, 33, 99, 34, 19, 15, 35, 87, 53, 69, 50, 87, 02, 37, 62, 89, 10, 05, 60 };
vector <int> numbers = { 04, 11, 57, 29, 03, 16, 92, 21, 22, 05, 43, 79, 61, 28, 78, 47, 51, 45, 82, 87 };
//vector <int> numbers = { 99, 51, 89, 86, 53, 26, 48, 94, 36, 06, 07, 92, 17, 64, 21, 20, 80, 66, 94, 54 };
//vector <int> numbers = { 05, 04, 33, 99, 34, 19, 15, 35, 87, 53, 69, 50, 87, 02, 37, 62, 89, 10, 05, 60,
//04, 11, 57, 29, 03, 16, 92, 21, 22, 05, 43, 79, 61, 28, 78, 47, 51, 45, 82, 87,
//99, 51, 89, 86, 53, 26, 48, 94, 36, 06, 07, 92, 17, 64, 21, 20, 80, 66, 94, 54 };
vector <int> x, y;
int xScale[20], yScale[20], allPointsArr[nums * 2], slaiterArr[nums * 2], paretoArr[nums * 2];
static int s,p,a;

vector<int> sortForMakingLines(vector<int> numbers)
{
	for (int i = 0; i < numbers.size() - 1; i++)
	{
		for (int j = i + 1; j < numbers.size(); j++)
		{
			if ((numbers[i] / 10 == numbers[j] / 10) && (numbers[i] % 10 < numbers[j] % 10))
			{
				swap(numbers[i], numbers[j]);
			}
			else if (numbers[i] / 10 < numbers[j] / 10)
				break;
		}
	}
	return numbers;
}

void fromVectorToArr(vector<int> numbers, int arr[])
{
	for (int i = 0; i < numbers.size(); i++)
	{
		arr[i * 2] = numbers[i] / 10;
		arr[i * 2 + 1] = numbers[i] % 10;
	}
}

static vector <int> pareto(vector <int> numbers){
	for (int i = 0; i < numbers.size(); i++){
		for (int j = i + 1; j < numbers.size(); j++){
			if (((numbers[i] / 10) >= (numbers[j] / 10)) && ((numbers[i] % 10) >= (numbers[j] % 10))){
				numbers.erase(numbers.begin() + j);
				j--;
			}
			else  if (((numbers[j] / 10) >= (numbers[i] / 10)) && ((numbers[j] % 10) >= (numbers[i] % 10))){
				numbers.erase(numbers.begin() + i);
				j = i;
			}
		}
	}
	return numbers;
}

static vector <int> slaiter(vector <int> numbers){
	for (int i = 0; i < numbers.size(); i++){
		for (int j = i + 1; j < numbers.size(); j++){
			if (((numbers[i] / 10) >(numbers[j] / 10)) && ((numbers[i] % 10) >(numbers[j] % 10))){
				numbers.erase(numbers.begin() + j);
				j--;
			}
			else  if (((numbers[j] / 10) >(numbers[i] / 10)) && ((numbers[j] % 10) >(numbers[i] % 10))){
				numbers.erase(numbers.begin() + i);
				j = i + 1;
			}
		}
	}
	cout << endl;
	return numbers;
}

//open gl functions 

void Display(void)
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glScalef(45, 45, 40);
	glTranslated(-5, -5, 0);
	glBegin(GL_LINES);
	glColor3ub(0, 255, 0); //green lines
	glVertex2i(0, 0);
	glVertex2i(0, 10);
	glVertex2i(0, 0);
	glVertex2i(10, 0);
	glEnd();

	//Scales
	glPointSize(3);
	glColor3ub(255, 255, 255);	//white
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_INT, 0, xScale);
	glDrawArrays(GL_POINTS, 0, 10);
	glVertexPointer(2, GL_INT, 0, yScale);
	glDrawArrays(GL_POINTS, 0, 10);
	glDisableClientState(GL_VERTEX_ARRAY);

	
	//Points 
	glPointSize(5);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_INT, 0, allPointsArr);
	glColor3ub(255, 0, 0);	//red lines
	glDrawArrays(GL_POINTS, 0, a);
	glDisableClientState(GL_VERTEX_ARRAY);

	//Slaiter lines
	glLineWidth(3);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_INT, 0, slaiterArr);
	glColor4ub(0, 0, 255, 100); //blue
	glDrawArrays(GL_LINE_STRIP, 0, s);
	glDisableClientState(GL_VERTEX_ARRAY);

	//Pareto lines
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_INT, 0, paretoArr);
	glColor4ub(100, 0, 100, 100); //violet
	glDrawArrays(GL_LINE_STRIP, 0, p);
	glDisableClientState(GL_VERTEX_ARRAY);

	glFinish();
}

void Reshape(GLint w, GLint h)
{
	/* установлюємо розміри області відображення */
	glViewport(0, 0, w, h);

	/* ортографічна проекція */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-(w / 2), w / 2, -(h / 2), h / 2, -500, 500);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}	

void Keyboard(unsigned char key, int x, int y)
{
#define ESCAPE '\033'
	if (key == ESCAPE)
		exit(0);
}


int main(int argc, char *argv[])
{
	//scales
	for (int i = 0; i < 10; i++)
	{
		xScale[i * 2] = i;
		xScale[i * 2 + 1] = 0;
		yScale[i * 2] = 0;
		yScale[i * 2 + 1] = i;
	}
	///////////////////////////////
	//All points
	fromVectorToArr(numbers, allPointsArr);

	///////////////////////////////
	//Slaiter numbers and slaiter lines

	sort(numbers.begin(), numbers.end());		//sort all numbers from smaller to bigger
	a = numbers.size();
	numbers = slaiter(numbers);
	numbers = sortForMakingLines(numbers);		//sort in order to receive lines we needed at the plot
	fromVectorToArr(numbers, slaiterArr);
	s = numbers.size();

	//Pareto numbers and pareto lines
	numbers = pareto(numbers);
	fromVectorToArr(numbers, paretoArr);
	p = numbers.size();

	//////////////////////////
	GLint MaxWindowWidth = 1350, MaxWindowHeight = 690;
	GLint Width = 1350, Height = 690;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(Width, Height);
	glutInitWindowPosition((MaxWindowWidth - Width) / 2, (MaxWindowHeight - Height) / 2);
	glutCreateWindow("TPR-1");

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);

	glutMainLoop();
	return 0;
}