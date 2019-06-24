#include <bits/stdc++.h>

using namespace std;

int **distanceMatrix, size=-1, pos=0;

int calculateTourDistance(int *tour)//calculate the distance of a tour
{
	int dist=0;
	for(int i=0; i<size-1; i++)
	{
		dist+=distanceMatrix[tour[i]][tour[i+1]];
	}
	dist+=distanceMatrix[tour[size-1]][tour[0]];
	return dist;
}

vector<string> split(const string& str, const string& delim)
{
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();
        string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    return tokens;
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
                s[s.size()-1] = '\0';
            if(('\r' == s[s.size()-1]))//in some files there is a carriage return at the end, don't know why. This command removes it
                s[s.size()-1]=0;

            vector<string> splitted = split(s, " ");
            string value1 = splitted[0], value2 = splitted[1], value3 = splitted[2];

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
                cout << value1 << "'" << value3 << "'"<< endl;
                fprintf(stderr,"\nERROR! tsp file is not of type TSP, aborting!!\n");
                exit(1);
            }
            else if(value1 == "DIMENSION")
            {//read the dimension from the header and allocate memory for the cities
                cout << "VALUE3 = " << value3 << endl;
                size = stoi(value3);
                distanceMatrix = (int**)malloc(size * sizeof(int*));
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
			}
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
    return 0;
}
