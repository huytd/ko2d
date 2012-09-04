#include "Model.h"
#include "FileSystem.h"

Model::Model(void)
{
}


Model::~Model(void)
{
}

int count_char(string txt, const char c)
{
	int count = 0;
	for (int i = 0; i < txt.length(); i++)
	{
		if (txt[i] == c) count++;
	}
	return count;
}

bool Model::init(const char* fileName)
{
	frM.max_x = 0;
	frM.max_y = 0;
	frM.max_z = 0;
	frM.min_x = 0;
	frM.min_y = 0;
	frM.min_z = 0;
	LOGI("\n[Model] File to open: '%s'\n", fileName);
	int nV = 0, nT = 0, nF = 0;
	char* fileToRead = (char*)fileName;
	#ifdef _ANDROID
	LOGI("\nAndroid detected\n");
	fileToRead[strlen(fileToRead)-1] = '\0';
	#endif
	LOGI("File to open: '%s' \n", fileToRead);
	LOGI("Last char: %d (%c)\n", fileToRead[strlen(fileToRead) - 1], fileToRead[strlen(fileToRead) - 1]);
	ifstream fc(fileToRead);
	if (fc)
	{
		string s;
		// count V
		while (!fc.eof())
		{
			getline(fc, s);
			string sub = s.substr(0, 2);
			if (sub == "v ") nV++;
			if (sub == "vt") nT++;
			if (sub == "f ") nF++;
		}
		fc.close();
	}else{
		LOGI("Canot open file for measuring\n");
	}
	LOGI("\nReaded %d %d %d\n", nV, nT, nF);
	ifstream f(fileToRead);
	if (f)
	{
		string sLine;
		Vector3 *vArr = new Vector3[nV]; int vIndex = 0;
		Vector2 *tArr = new Vector2[nT]; int tIndex = 0;
		FACE_LINE *fArr = new FACE_LINE[nF]; int fIndex = 0;
		while(!f.eof())
		{
			getline(f, sLine);
			string type = sLine.substr(0, 2);
			if (type == "v ") 
			{
				sscanf(sLine.data(), "v %f %f %f", &vArr[vIndex].x, &vArr[vIndex].y, &vArr[vIndex].z);
				if ( frM.max_x < vArr[vIndex].x ) frM.max_x = vArr[vIndex].x;
				if ( frM.min_x > vArr[vIndex].x ) frM.min_x = vArr[vIndex].x;
				if ( frM.max_y < vArr[vIndex].y ) frM.max_y = vArr[vIndex].y;
				if ( frM.min_y > vArr[vIndex].y ) frM.min_y = vArr[vIndex].y;
				if ( frM.max_z < vArr[vIndex].z ) frM.max_z = vArr[vIndex].z;
				if ( frM.min_z > vArr[vIndex].z ) frM.min_z = vArr[vIndex].z;
				vIndex++;
			}
			if (type == "vt")
			{
				float tmp;
				int spcs = count_char(sLine, ' ');
				if (spcs == 3)
					sscanf(sLine.data(), "vt %f %f %f", &tArr[tIndex].x, &tArr[tIndex].y, &tmp);
				else sscanf(sLine.data(), "vt %f %f", &tArr[tIndex].x, &tArr[tIndex].y);
				tIndex++;
			}
			if (type == "f ")
			{
				sscanf(sLine.data(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &fArr[fIndex].v1, &fArr[fIndex].t1, &fArr[fIndex].n1, &fArr[fIndex].v2, &fArr[fIndex].t2, &fArr[fIndex].n2, &fArr[fIndex].v3, &fArr[fIndex].t3, &fArr[fIndex].n3);
				fIndex++;
			}
		}
		Vertex *vData = new Vertex[fIndex*3];
		int count = 0;
		for (int i = 0; i < fIndex; i++) 
		{
			vData[count].pos = vArr[fArr[i].v1 - 1];
			vData[count].tex = tArr[fArr[i].t1 - 1];
			count++;
			vData[count].pos = vArr[fArr[i].v2 - 1];
			vData[count].tex = tArr[fArr[i].t2 - 1];
			count++;
			vData[count].pos = vArr[fArr[i].v3 - 1];
			vData[count].tex = tArr[fArr[i].t3 - 1];
			count++;
		}
		LOGI("Total: %d vertices\n", count);
		LOGI("Generating vertex buffer...\n");
		//Khởi tạo vertex buffer
		glGenBuffers(1, &vboID);
		// Đẩy dữ liệu từ mảng vData vào buffer
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * (fIndex * 3), vData, GL_STATIC_DRAW);
		// Đóng buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		LOGI("Vertex buffer created with ID: %d\n", vboID);
		delete[] vData;
		delete[] vArr;
		delete[] tArr;
		delete[] fArr;
		numCount = fIndex * 3;
		f.close();
		return true;
	}
	else
	{
		LOGI("\nCannot open file !\n");
		return false;
	}
	return false;
}