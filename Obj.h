#pragma once
#include <cstdlib>
#include <string>
#include <iostream>
#include <cmath>
#include <chrono>
#include <fstream>
#include <vector>
#include "Vec3.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <SOIL.h>

using namespace std;

class Obj
{
public:
	string objName;
	GLfloat* vertices;
	GLfloat* textures;
	GLfloat* normals;
	GLint* indices;

	int numOfFaces;
	vector<Vec3> tmpVecArray;
	vector<Vec2> tmpUVsArray;
	vector<Vec3> tmpNrmArray;
	vector<int> tmpIndxArray;

	Obj(const char* fileName)
	{
		ifstream pFile;
		pFile.open(fileName);
		string token;
		string line;
		string groupName;

		bool ignoreTexCoords;

		numOfFaces = 0;

		//int read;
		char ch;
		float s, u, v, x, y, z;
		int a, b, c;

		if(pFile.fail())
		{
			cout << "> file pointer failed, exiting" << endl;
			return;
		}

		//takes next available token
		pFile >> token;

		while( !(pFile.eof() || pFile.fail()) )
		{
			if(token == "#")
			{
				getline(pFile, line);
			}
			else if (token == "o")
			{
				pFile >> objName;
			}
			else if (token == "g")
			{
				getline(pFile, groupName);
			}
			else if(token == "s")
			{
				getline(pFile, line);
				//pFile >> s;
			}
			else if(token == "mtllib")
			{
				getline(pFile, line);
			}
			else if(token == "usemtl")
			{
				getline(pFile, line);
			}
			else if(token == "v")
			{
				pFile >> x >> y >> z;
				tmpVecArray.push_back(Vec3(x,y,z));

			}
			else if(token == "vt")
			{
				pFile >> u >> v;
				tmpUVsArray.push_back(Vec2(u,v));

			}
			else if(token == "vn")
			{
				pFile >> x >> y >> z;
				tmpNrmArray.push_back(Vec3(x,y,z));
			}
			else if(token == "f")
			{	//assusmes triangle faces (3 sides)

				//cout << "f";
				for(int i=0; i<3; i++)
				{
					
					pFile >> a >> ch >> b >> ch >> c;
					tmpIndxArray.push_back(a-1);
					
					//cout << " " << a << "/" << b << "/" << c;
				}
				numOfFaces++;
				//cout << endl;
			}
			else
				getline(pFile, line);


			//takes next available token
			pFile >> token;		
		}

		int p = 0;
		if (tmpVecArray.size() > 0)
		{
			vertices = new GLfloat[tmpVecArray.size() * 3];
			for (auto i = tmpVecArray.begin(); i != tmpVecArray.end(); ++i)
			{
				vertices[p++] = i->x;
				vertices[p++] = i->y;
				vertices[p++] = i->z;
			}
		}

		p = 0;
		if (tmpUVsArray.size() > 0)
		{
			textures = new GLfloat[tmpUVsArray.size() * 2];
			for (auto i = tmpUVsArray.begin(); i != tmpUVsArray.end(); ++i)
			{
				textures[p++] = i->x;
				textures[p++] = i->y;
			}
		}

		p = 0;
		if (tmpNrmArray.size() > 0)
		{
			normals = new GLfloat[tmpNrmArray.size() * 3];
			for (auto i = tmpNrmArray.begin(); i != tmpNrmArray.end(); ++i)
			{
				normals[p++] = i->x;
				normals[p++] = i->y;
				normals[p++] = i->z;
			}
		}

		p = 0;
		if (tmpIndxArray.size() > 0)
		{
			indices = new GLint[tmpIndxArray.size()];
			for (auto i = tmpIndxArray.begin(); i != tmpIndxArray.end(); ++i)
			{
				indices[p++] = *i;
			}
		}

		pFile.close();

	    //glVertexPointer(3, GL_FLOAT, 0, vertices);
	    //glTexCoordPointer(2, GL_FLOAT, 0, textures);
	    //glNormalPointer(GL_FLOAT, 0, normals);
	    //glColorPointer(3, GL_FLOAT, 0, colsPointer);
	}
	void draw()
	{
	    glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, vertices);

		if (textures != NULL)
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, 0, textures);
		}

		if (normals != NULL)
		{
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_FLOAT, 0, normals);
		}

	    //glEnableClientState(GL_COLOR_ARRAY);
	    glColor4f(1,1,1,1);

		glDrawElements(GL_TRIANGLES, numOfFaces*3, GL_UNSIGNED_INT, indices);

	    glDisableClientState(GL_VERTEX_ARRAY);  			  // disable vertex array
	    glDisableClientState(GL_TEXTURE_COORD_ARRAY);		 // disable texture array
	    glDisableClientState(GL_NORMAL_ARRAY);				// disable normal array
	    //glDisableClientState(GL_COLOR_ARRAY);
	}

	~Obj()
	{
		delete [] vertices;
		delete [] textures;
		delete [] normals;
		delete [] indices;
	}
};