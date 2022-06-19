#include <iostream>
using namespace std;

class Matrix{
public:
    int height, width;
    int table[100][100];
    Matrix(){
        height = 0;
        width = 0;
        for(int i=0; i<height; i++)
            for(int j=0; j<width; j++)
                table[i][j] = 0;
    }
    Matrix(int n, int m){
        height = n;
        width = m;
        for(int i=0; i<height; i++)
            for(int j=0; j<width; j++)
                table[i][j] = 0;
    }
    void input(){
        for(int i=0; i<height; i++)
            for(int j=0; j<width; j++)
                cin >> table[i][j];
    }
    void output(){
        for(int i=0; i<height; i++) {
            for (int j = 0; j < width; j++)
                cout << table[i][j] << " ";
            cout << endl;
        }
    }
    Matrix operator = (Matrix m){
        height = m.height;
        width = m.width;
        for(int i=0; i<height; i++){
            for(int j=0; j<width; j++)
                table[i][j] = m.table[i][j];
        }
        return *this;
    }
    Matrix operator + (Matrix m){
        Matrix temp(height, width);
        if (height == m.height && width == m.width){
            for(int i=0; i<height; i++)
                for(int j=0; j<width; j++) {
                    temp.table[i][j] = table[i][j] + m.table[i][j];
                }
        }
        return temp;
    }
    Matrix operator - (Matrix m){
        if (height == m.height && width == m.width){
            Matrix temp(height, width);
            for(int i=0; i<height; i++)
                for(int j=0; j<width; j++)
                    temp.table[i][j] = table[i][j] - m.table[i][j];
            return temp;
        }
    }
    Matrix operator * (Matrix m){
        if (width == m.height){
            Matrix temp(height, m.width);
            for(int i=0; i<height; i++)
                for(int j=0; j<m.width; j++)
                    for(int k=0; k<width; k++)
                        temp.table[i][j] += table[i][k] * m.table[k][j];
            return temp;
        }
    }
    Matrix operator * (int a){
        Matrix temp(height, width);
        for(int i=0; i<height; i++)
            for(int j=0; j<width; j++)
                temp.table[i][j] = table[i][j] * a;
        return temp;
    }
};
int main(){
    Matrix a(3, 3), b(3, 3);
    a.input();
    b.input();
    Matrix c;
    c=a+b;
    c.output();
    cout << endl;
    c=a-b;
    c.output();
    cout << endl;
    c=a*b;
    c.output();
    cout << endl;
    c=a*3;
    c.output();
    cout << endl;
}
/*
 1 1 1
 2 2 2
 3 3 3
 4 4 4
 3 3 3
 2 2 2
 */