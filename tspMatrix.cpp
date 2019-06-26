#include <bits/stdc++.h>

using namespace std;

struct Caminho
{
    int dist = INT_MAX, ind;
};

struct Solucao
{
    vector<Caminho> caminhos;
    int distTotal = 0;
};

int **distanceMatrix, size=-1, pos=0;
Solucao *solucoes;

int calculateTourDistance(int *tour)//calculate the distance of a tour
{
	int dist = 0;
	for(int i = 0; i < size-1; i++)
	{
		dist += distanceMatrix[tour[i]][tour[i+1]];
	}
	dist += distanceMatrix[tour[size-1]][tour[0]];
	return dist;
}

vector<string> cpp_strtok(string src, string dlm)
{
    string::size_type p, start=0, len=src.length();
    vector<string> v;

    start = src.find_first_not_of(dlm);
    p = src.find_first_of(dlm, start);
    while(p != string::npos)
    {
        v.push_back(src.substr(start, p-start));
        start = src.find_first_not_of(dlm, p);
        p = src.find_first_of(dlm, start);
    }
    if(len > start)
        v.push_back(src.substr(start, len - start));

    return v;
}

Caminho menorDaLinha(int i, bool visitados[])
{
    Caminho caminho;
    for(int j = 0; j < size; ++j)
    {
        if(distanceMatrix[i][j] < caminho.dist && !visitados[j])
        {
            caminho.dist = distanceMatrix[i][j];
            caminho.ind = j;
        }
    }

    return caminho;
}

void gerarSolucoes()
{
    for(int i = 0; i < size; ++i)
    {
        bool visitados[size] = {false};
        int contVisitados = 1, j = i;
        visitados[i] = true;
        Caminho menor;
        while(contVisitados < size)
        {
            menor = menorDaLinha(j, visitados);
            j = menor.ind;
            solucoes[i].caminhos.push_back(menor);
            solucoes[i].distTotal += menor.dist;
            visitados[j] = true;
            contVisitados += 1;
        }
        //Retornar para a cidade de origem
        Caminho volta;
        volta.dist = menor.dist;
        volta.ind = i;
        solucoes[i].caminhos.push_back(volta);
        solucoes[i].distTotal += volta.dist;
    }
}


int main(const int argc, const char **inputFile)
{
	double *x, *y;
	string type = "";

	if(argc < 2)
	{ //verify if an argument was passed (the first argument in C is the name of the executable)
		fprintf(stderr,"use: tspMatrix <tsp file>\n\n");
		exit(1);
	}
	FILE *stream = fopen(inputFile[1], "r"); //read file (read-only permission)
	if( stream == NULL )
	{ // if the file was not read, error
		fprintf(stderr,"\nFail to Open tsp File!!\n");
		exit(1);
	}

	string s;
    ifstream file(inputFile[1]);
    if(file.is_open())
    {
        while(getline(file, s))
        {
            if(s.size()-1 > 0)
                s = s.substr(0, s.size()-1);

            if(('\r' == s[s.size()-1]))//in some files there is a carriage return at the end, don't know why. This command removes it
                s[s.size()-1] = 0;

            vector<string> splitted = cpp_strtok(s, " ");
            string value1 = splitted[0], value2, value3;
            if(splitted.size() == 3)
                value2 = splitted[1], value3 = splitted[2];

            if(value1 == "EDGE_WEIGHT_TYPE")
            {
                if(value3 != "EUC_2D" && value3 != "ATT" && value3 != "CEIL_2D")
                {
                    fprintf(stderr,"\nERROR! tsp file is not of type EUC_2D, ATT or CEIL_2D aborting!!\n");
                    exit(1);
                }
                else
                    type = value3;
            }
            else if(value1 == "TYPE" && value3 != "TSP")
            { //verify if the instance is of type TSP, the other types will not be considered
                fprintf(stderr,"\nERROR! tsp file is not of type TSP, aborting!!\n");
                exit(1);
            }
            else if(value1 == "DIMENSION")
            {//read the dimension from the header and allocate memory for the cities
                size = stoi(value3);
                distanceMatrix = (int**)malloc(size * sizeof(int*));
                solucoes = new Solucao[size];



                x = (double*)malloc(size * sizeof(double*));
                y = (double*)malloc(size * sizeof(double*));

                for(int i = 0; i < size; i++)
                    distanceMatrix[i] = (int*)malloc(size * sizeof(int));
            }
            try
            {
                if(stoi(value1))
                {//if the first substring is a number, the list of cities started
                    if(size == -1)
                    {//if the size was not set, it was not in the header, error
                        fprintf(stderr,"\nERROR! Dimension not set in file header!!\n");
                        exit(1);
                    }
                    x[pos] = stof(value2);//storing the values from the file in the arrays
                    y[pos] = stof(value3);
                    pos++;
                }
            }
            catch(invalid_argument ia)
            {
            }
        }
        file.close();
    }


    if(type == "EUC_2D")
	{
		for(int i = 0; i < size; i++)
		{
			for(int j = 0; j < size; j++)
			{
				double xd = x[i]-x[j];
				double yd = y[i]-y[j];
				double dist = sqrt(xd*xd+yd*yd);
				distanceMatrix[i][j] = (int)(dist+0.5);//calculating the euclidean distance, rounding to int and storing in the distance matrix
				cout << distanceMatrix[i][j] << " ";
			}
			cout << endl;
		}
	}
	else if(type == "CEIL_2D")
	{
		for(int i = 0; i < size; i++)
		{
			for(int j = 0; j < size; j++)
			{
				double xd = x[i]-x[j];
				double yd = y[i]-y[j];
				double dist = sqrt(xd*xd+yd*yd);
				distanceMatrix[i][j] = (int)(dist+0.000000001);
			}
		}
	}
	else if(type == "ATT")
	{
		for(int i = 0; i < size; i++)
		{
			for(int j = 0; j < size; j++)
			{
				double xd = x[i]-x[j];
				double yd = y[i]-y[j];
				double rij = sqrt((xd*xd+yd*yd)/10.0);
				double tij = (int)(rij+0.5);
				if(tij < rij)
					distanceMatrix[i][j] = tij+1;
				else
					distanceMatrix[i][j] = tij;
			}
		}
	}

	int tour[size];
	for(int i = 0; i < size; i++)
		tour[i] = i;

	printf("Comprimento da rota: %d\n", calculateTourDistance(tour));


    gerarSolucoes();


    for(int i = 0; i < size; ++i)
    {
        printf("Cidade %d | Total = %d\n", i, solucoes[i].distTotal);
        for(int j = 0; j < size; ++j)
            cout << solucoes[i].caminhos[j].ind << " ";
        cout << endl;
    }
    return 0;
}
