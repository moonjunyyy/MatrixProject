#include <iostream>
#include <iomanip>
#include <cmath>
#include <exception>
#include <fstream>
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
	Matrix(int w, int h, float *m)
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
	Matrix reshape(int w, int h)
	{
		if (width * height == w * h) { return Matrix(w, h, _matrixBody); }
		Type* N = new Type[w * h];
		for (int i = 0; i < w * h; i++)
		{
			if (i >= width * height) { N[i] = 0; }
			else { N[i] = _matrixBody[i]; }
		}
		return Matrix(w, h, N);
	}
	Matrix transpose()
	{
		if (width * height == 0) { return Matrix(); }
		Type* N = new Type[height * width];
		for (int x = 0; x < height; x++)
		{
			for (int y = 0; y < width; y++)
			{
				N[y * height + x] = getData(y, x);
			}
		}
		return Matrix(height, width, N);
	}
	void setData(int w, int h, Type dat)
	{
		if (w >= width || h >= height) { char M[] = "Matrix Access Error"; throw Matrix_Exception(M); }
		_matrixBody[h * width + w] = dat;
	}
	Type& getData(int w, int h)
	{
		if (w >= width || h >= height) { char M[] = "Matrix Access Error"; throw Matrix_Exception(M); }
		return _matrixBody[h * width + w];
	}
	Type* getRow(int h)
	{
		if (h >= height) { char M[] = "Matrix Access Error"; throw Matrix_Exception(M); }
		return (_matrixBody + h * width);
	}
	void writeMatrixtoBinary(fstream& fs, char* filename)
	{
		fs.open(filename, ios::out | ios::binary);
		
		fs.write((char*)&width, sizeof(width));
		fs.write((char*)&height, sizeof(height));
		for (int i = 0; i < width * height; i++)
		{
			fs.write((char*)(_matrixBody + i), sizeof(Type));
		}

		fs.close();
	}
	void readMatrixtoBinary(fstream& fs, char* filename)
	{
		fs.open(filename, ios::in | ios::binary);

		fs.read((char*)&width, sizeof(width));
		fs.read((char*)&height, sizeof(height));

		delete[] _matrixBody;
		_matrixBody = new Type[width * height];
		for (int i = 0; i < width * height; i++)
		{
			fs.read((char*)(_matrixBody + i), sizeof(Type));
		}

		fs.close();
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
	Matrix<Type> operator* (Matrix<Type> M)
	{
		if (this->width != M.height) { char M[] = "Multiply Size Error"; throw Matrix_Exception(M); }

		Matrix Out(M.width, this->height);
		for (int y = 0; y < this->height; y++)
		{
			for (int x = 0; x < M.width; x++)
			{
				for (int iter = 0; iter < this->width; iter++)
				{
					Out.getData(x, y) += this->getData(iter, y) * M.getData(x, iter);
				}
			}
		}
		return Out;
	}
	Matrix<Type> operator= (Matrix<Type> M)
	{
		delete[] _matrixBody;
		this->width = M.width, this->height = M.height, this->_matrixBody = new Type[width * height];
		for (int y = 0; y < M.height; y++)
		{
			for (int x = 0; x < M.width; x++)
			{
				this->setData(x, y, M.getData(x, y));
			}
		}
		return *this;
	}
};

int main(int argc, char* argv[])
{
	try
	{
		//Sprint 1
		cout << "Sprint 1" << endl << endl;
		float A[] = { 1,2,3,4,5,6 }, B[] = { 1,2,3,4 }, C[6];
		Matrix<float> M23(2, 3, A);
		Matrix<float> M32 = M23.reshape(3, 2);
		Matrix<float> M22(2, 2, B);
		M23.showmatrix();
		cout << endl;
		M32.showmatrix();
		cout << endl;

		//Sprint 2
		cout << "Sprint 2" << endl << endl;
		Matrix<float> M_mul = M23 * M22;
		cout << M_mul << endl;
		cout << endl;

		//Sprint 3
		cout << "Sprint 3" << endl << endl;
		float o[] = { 0.5,0.5,0.5,0.5,  0.5,0.5,-0.5,-0.5, 0.5,-0.5,-0.5,0.5,  0.5,-0.5,0.5,-0.5 };
		Matrix<float> Delta(4, 4);

		Matrix<float> c[4];
		c[0] = Matrix<float>(4, 1, o);
		c[1] = Matrix<float>(4, 1, o + 4);
		c[2] = Matrix<float>(4, 1, o + 8);
		c[3] = Matrix<float>(4, 1, o + 12);

		for (int i = 0; i < 4; i++)
		{
			cout << c[i];
		}
		cout << endl;
		for (int x = 0; x < 4; x++)
		{
			for (int y = 0; y < 4; y++)
			{
				Delta.setData(x, y, (c[x] * c[y].reshape(1, 4)).getData(0, 0));
			}
		}
		cout << Delta << endl;

		//Sprint 4
		cout << "Sprint 4" << endl << endl;
		Matrix<float> F(4, 4, o);
		Matrix<float> FT = F.transpose();

		cout << F << endl << FT << endl;
		cout << F * FT << endl;

		//Sprint 5
		cout << "Sprint 5" << endl << endl;
		float a[] = { 100,120,-4,2,100,20,120,32,10,120,108,8 };

		Matrix<float> a_vec[3];
		for (int i = 0; i < 3; i++) { a_vec[i] = Matrix<float>(1, 4, (a + (i * 4))); }
		for (int i = 0; i < 3; i++) { cout << "a" << i + 1 << endl << a_vec[i] << endl; }

		Matrix<float> A_transeform[3];
		for (int i = 0; i < 3; i++) { A_transeform[i] = F * a_vec[i]; }
		for (int i = 0; i < 3; i++) { cout << "A" << i + 1 << endl << A_transeform[i] << endl; }

		//Sprint 6
		cout << "Sprint 6" << endl << endl;
		Matrix<float> A_inverse[3];
		for (int i = 0; i < 3; i++) { A_inverse[i] = FT * A_transeform[i]; }
		for (int i = 0; i < 3; i++) { cout << "a" << i + 1 << endl << A_inverse[i] << endl; }

		//Sprint 7
		cout << "Sprint 7" << endl << endl;
		Matrix<float> A_TT[3];
		for (int i = 0; i < 3; i++) { A_TT[i] = A_transeform[i]; }
		for (int i = 0; i < 3; i++)
		{
			
		}
		for (int i = 0; i < 3; i++)
		{
			char sort[4] = { 0,0,0,0 };
			for (int j = 0; j < 4; j++)
			{
				for (int k = 0; k < 4; k++)
				{
					if (abs(A_TT[i].getData(0, j)) > abs(A_TT[i].getData(0, k))) sort[j]++;
				}
			}
			for (int j = 0; j < 4; j++) if (sort[j] == 0 || sort[j] == 1) A_TT[i].setData(0, j, 0.);
		}
		for (int i = 0; i < 3; i++) { cout << "A" << i + 1 << endl << A_TT[i] << endl; }

		Matrix<float> A_New_inverse[3];
		for (int i = 0; i < 3; i++) { A_New_inverse[i] = FT * A_TT[i]; }
		for (int i = 0; i < 3; i++) { cout << "a" << i + 1 << endl << A_New_inverse[i] << endl; }

		//Sprint 8
		cout << "Sprint 8" << endl << endl;
		fstream fio;
		char fileName[3][50] = { "A1.bin", "A2.bin", "A3.bin"};
		for (int i = 0; i < 3; i++)
		{
			A_transeform[i].writeMatrixtoBinary(fio, fileName[i]);
		}

		//Sprint 9
		cout << "Sprint 9" << endl << endl;
		for (int i = 0; i < 3; i++)
		{
			A_transeform[i].readMatrixtoBinary(fio, fileName[i]);
			cout << "A" << i + 1 << endl << A_transeform[i] << endl;
		}

		Matrix<float> A_Neo_inverse[3];
		for (int i = 0; i < 3; i++) { A_Neo_inverse[i] = FT * A_transeform[i]; }
		for (int i = 0; i < 3; i++) { cout << "a" << i + 1 << endl << A_Neo_inverse[i] << endl; }
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