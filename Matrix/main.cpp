//
//  main.cpp
//  Matrix
//
//  Created by Ruben Cuadra on 2/22/16.
//  Copyright © 2016 Algebra. All rights reserved.
//

#include <iostream>
int GetMinor(float **first, float **last, int row, int col, int s)
{               //Nos da la menor basandonos en la matrix que se pasa y lo guarda en
                //la matrix last
    // indices
    int col_i=0,row_i=0;
    for(int i = 0; i < s; i++ )
    {
        if( i != row )
        {
            col_i = 0;
            for(int j = 0; j < s; j++ )
            {
                if( j != col )
                {
                    last[row_i][col_i] = first[i][j];
                    col_i++;
                }
            }
            row_i++;
        }
    }
    return 1;
}

// Calculate the determinant recursively.
double CalcDeterminant( float **mat, int s)
{
    //Señal que detiene la recursividad
    if( s == 1 )
        return mat[0][0];
    
    float det = 0;
    
    // Generamos espacio temporal para almacenar la menor
    float **minor;
    minor = new float*[s-1];
    for(int i=0;i<s-1;i++)
        minor[i] = new float[s-1];
    //Siempre obtiene la determinante usando la primer fila
    for(int i = 0; i < s; i++ )
    {
        GetMinor( mat, minor, 0, i , s); //Sobreescribe la matriz menor
        //Aqui obtenemos el cofactor, multiplicamos por el elemento en la posicion i y ahora por la determinante
        det += (i%2==1?-1.0:1.0) * mat[0][i] * CalcDeterminant(minor,s-1);
    }
    
    // Quitamos el espacio usado
    for(int i=0;i<s-1;i++)
        delete [] minor[i];
    delete [] minor;

    return det;
}

float ** MatrixInversion(float **original, int s)
{
    //Reservamos memoria para guardar la inversa que regresaremos al final(Solo apuntador inicial)
    float**Invertida= new float*[s];
    for(int i =0;i<s;++i)
        Invertida[i]=new float[s];
        
    //Sacamos la inversa de la determinante, que luego usaremos para multiplicar
    double det = 1.0/CalcDeterminant(original,s);
    
    //Reservamos una matriz que se sobreescribe con los valores de la menor
    //dependiendo de la celda en la que nos encontremos iterando
    float *cols = new float[(s-1)*(s-1)];
    float **menor = new float*[s-1];
    for(int i=0;i<s-1;i++)
        menor[i] = cols+(i*(s-1));
    
    //Iteramos para generar la matriz de cofactores en A'
    for(int j=0;j<s;j++)
    {
        for(int i=0;i<s;i++)
        {
            //La funcion recibe celda, tamaño total y el espacio temporal. Asi como matriz original
            GetMinor(original,menor,j,i,s);
            //Aqui ya tenemos la menor almacenada, obtenemos su determinante y eso nos daria la celda
            //i,j de la matriz de cofactores
            Invertida[i][j] = CalcDeterminant(menor,s-1);
            //Ahora la multiplicamos por 1/det original (Aprovechar el ciclo para multiplicar por escalar)
            Invertida[i][j] *= det;
            //Ahora le agregamos el signo del cofactor basandonos en si 1+j=par el signo debe ser +
            Invertida[i][j] *= ((i+j)%2?-1:1);
        }
    }
    
    // Liberamos el espacio de la menor, el de la invertida se regresa.
    // si se desea liberar debe ser despues de llamar a la funcion
    delete [] cols;
    delete [] menor;
    return Invertida;
}

int main(int argc, const char * argv[])
{
    int size=3; //Definir tamaño
    float **matrix = new float*[size];
    for(int i =0;i<size;++i)
        matrix[i]=new float[size];
    
    matrix[0][0]=0; matrix[0][1]=2; matrix[0][2]=0;
    matrix[1][0]=2; matrix[1][1]=1; matrix[1][2]=1;
    matrix[2][0]=2; matrix[2][1]=1; matrix[2][2]=2; //LLENAMOS LA MATRIX COMO EL EJEMPLO DEL PDF
    
    std::cout<<CalcDeterminant(matrix, size)<<"\n"; //PRINTEAMOS DETERMINANTE
   
    float **inv=MatrixInversion(matrix, size);  //El apuntador Inv se llena aqui
   
    
    std::cout<<"\nOriginal\n";
    //PRINTEAR MATRIX
    for(int i =0;i<size;++i)
    {
        for(int j =0;j<size;++j)
            std::cout<<matrix[i][j]<<"\t";
        std::cout<<"\n";
    }
    //PRINTEAR MATRIX INV
    std::cout<<"\nInversa\n";
    for(int i =0;i<size;++i)
    {
        for(int j =0;j<size;++j)
            std::cout<<inv[i][j]<<"\t";
        std::cout<<"\n";
    }
    
    return 0;
}