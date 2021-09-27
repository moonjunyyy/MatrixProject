#include <iostream>
#include <iomanip>
#include <cmath>
#include <exception>
using namespace std;
class Matrix_Exception : public exception
{
private:
	char message[50];
public:
	Matrix_Exception(char *w) { strcpy_s(message, w); message[49] = '\0'; }
	const char* what() { return message; }
};
template<class Type>
class Matrix
{
private:
	int width,
		height;
	Type* _matrixBody;

public:
	Matrix() { this->width = 0, this->height = 0, this->_matrixBody = nullptr; }
	Matrix(int w, int h)
	{
		this->width = w;
		this->height = h;
		this->_matrixBody = new Type[width * height];
		for (int i = 0; i < width * height; i++) _matrixBody[i] = 0;
	}
	Matrix(int w, int h, float m[])
	{
		this->width = w;
		this->height = h;
		this->_matrixBody = new Type[width * height];
		for (int i = 0; i < width * height; i++) _matrixBody[i] = m[i];
	}
	Matrix(const Matrix& O)
	{
		this->width = O.width;
		this->height = O.height;
		this->_matrixBody = new Type[width * height];
		for (int i = 0; i < width * height; i++)_matrixBody[i] = O._matrixBody[i];
	}
	~Matrix() { delete[] _matrixBody; }
	void showmatrix()
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				cout << setw(8) << _matrixBody[i * width + j];
			}
			cout << endl;
		}
	}
	void shape() { cout << "(" << width << ", " << height << ")" << endl; }
	void setMat(int w, int h, Type dat)
	{
		if (w >= width, h >= height) { char M[] = "Matrix Access Error"; throw Matrix_Exception(M); }
		_matrixBody[h * width + w] = dat;
	}
	Type& getMat(int w, int h)
	{
		if (w >= width, h >= height) { char M[] = "Matrix Access Error"; throw Matrix_Exception(M); }
		return _matrixBody[h * width + w];
	}
	friend ostream& operator<<(ostream& os, const Matrix M)
	{
		for (int i = 0; i < M.height; i++)
		{
			for (int j = 0; j < M.width; j++)
			{
				os << setw(8) << M._matrixBody[i * M.width + j];
			}
			cout << endl;
		}
		return os;
	}
	Matrix operator* (Matrix M)
	{
		if (this->width != M.height) { char M[] = "Multiply Size Error"; throw Matrix_Exception(M); }

		Matrix Out(M.width, this->height);
		for (int y = 0; y < this->height; y++)
		{
			for (int x = 0; x < M.width; x++)
			{
				for (int iter = 0; iter < this->width; iter++)
				{
					Out.getMat(x, y) += this->getMat(iter, y) * M.getMat(x, iter);
				}
			}
		}
		return Out;
	}
};

int main(int argc, char* argv[])
{
	try
	{
		float A[] = { 1,2,3,4,5,6 }, B[] = { 1,2,3,4 }, C[6];
		Matrix<float> M23(2, 3, A), M32(3, 2, A);                          
		Matrix<float> M22(2, 2, B);
		M23.showmatrix();
		cout << endl;
		M32.showmatrix();
		cout << endl;

		Matrix<float> M_mul = M23 * M22;
		cout << M_mul << endl;
		cout << endl;
	}
	catch (Matrix_Exception E)
	{
		cout << E.what() << endl;
		return 1;
	}
	catch(exception E)
	{
		cout << E.what() << endl;
		return 400;
	}
	return 0;
}