#include "ResourcesManager.h"
#include "FileSystem.h"
#include "Vertex.h"

ResourcesManager::ResourcesManager(void)
{
	loadingCount = 0;
	loadingTotal = 0;
	LOGI("\n[RM] Resource Manager initialized\n");
}


ResourcesManager::~ResourcesManager(void)
{
	LOGI("\n[RM] Resource Manager destroyed\n");
	delete[] _modelList;
	delete[] _textureList;
}

void ResourcesManager::LoadResouces(const char* fileName)
{
	string section_name;
	int _num;
	int _current, _count;
	
	DATA_LINE* _rline = NULL;
	int _curLine;

	char* fileToRead = (char*)fileName;
	#ifdef _ANDROID
	LOGI("\nAndroid detected\n");
	//fileToRead[strlen(fileToRead)-1] = '\0';
	LOGI("File to open: '%s' \n", fileToRead);
	LOGI("Last char: %d (%c)\n", fileToRead[strlen(fileToRead) - 1], fileToRead[strlen(fileToRead) - 1]);
	#endif
	ifstream f(FileSystem::GetInstance()->GetPath(fileToRead).data());
	if (f)
	{
		while (!f.eof())
		{
			string line;
			getline(f, line);
			if (line.substr(0, 1) == "#")
			{
				// Begin read section
				int split_pos = line.find(':');	
				section_name = line.substr(1, split_pos - 1);
				LOGI("\n[RM] Parsing \t%s \n", section_name.data());
				
				string section_num_data = line.substr(split_pos + 2);
				LOGI("[RM]   Count \t%s \n", section_num_data.data());

				_current = 0;
				_count = 0;
				_num	 = atoi(section_num_data.data());
				
				if (section_name == "Models")
				{
					_modelList = new Model[_num];
					_modelCount = 0;
					if (_rline != NULL) delete[] _rline;
					_rline = new DATA_LINE[2];
					_curLine = 0;
				}
				if (section_name == "Textures")
				{
					_textureList = new Texture[_num];
					_textureCount = 0;
					if (_rline != NULL) delete[] _rline;
					_rline = new DATA_LINE[3];
					_curLine = 0;
				}
				if (section_name == "Shaders")
				{
					_shaderList = new Shaders[_num];
					_shaderCount = 0;
					if (_rline != NULL) delete[] _rline;
					_rline = new DATA_LINE[6];
					_curLine = 0;
				}

				LOGI("[RM] Start reading\n");
				
			}
			else
			{
				if (_current < _num)
				{
					_count++;
					// Process data for Models
					if (section_name == "Models")
					{
						int space_pos = line.find(' ');
						DATA_LINE ln;
						ln.key = line.substr(0, space_pos);
						ln.value = line.substr(space_pos + 1);
						_rline[_curLine].key = line.substr(0, space_pos);
						_rline[_curLine].value = line.substr(space_pos + 1);
						_curLine++;
						if (_count >= 2)
						{
							_modelList[_modelCount].init( FileSystem::GetInstance()->GetPath( _rline[1].value.data() ).c_str() );
							_modelCount++;

							_count = 0;
							_current++;
							_curLine = 0;
						}
					}
					// Process data for Textures
					if (section_name == "Textures")
					{
						int space_pos = line.find(' ');
						DATA_LINE ln;
						ln.key = line.substr(0, space_pos);
						ln.value = line.substr(space_pos + 1);
						_rline[_curLine].key = line.substr(0, space_pos);
						_rline[_curLine].value = line.substr(space_pos + 1);
						_curLine++;
						if (_count >= 3)
						{
							_textureList[_textureCount].init( FileSystem::GetInstance()->GetPath( _rline[1].value.data() ).c_str(), _rline[2].value.data() );
							_textureCount++;

							_count = 0;
							_current++;
							_curLine = 0;

						}
					}
					// Process data for Shaders
					if (section_name == "Shaders")
					{
						int space_pos = line.find(' ');
						DATA_LINE ln;
						ln.key = line.substr(0, space_pos);
						ln.value = line.substr(space_pos + 1);
						_rline[_curLine].key = line.substr(0, space_pos);
						_rline[_curLine].value = line.substr(space_pos + 1);
						_curLine++;
						if (_count >= 6)
						{
							_shaderList[_shaderCount].Init( FileSystem::GetInstance()->GetPath( _rline[1].value.data() ).c_str(),
															FileSystem::GetInstance()->GetPath( _rline[2].value.data() ).c_str());
							_shaderCount++;

							_count = 0;
							_current++;
							_curLine = 0;

						}
					}
				}
			}
		}
		f.close();
	}
	loadingCount++;
}

Model* ResourcesManager::getModel(int id)
{
	if (id < _modelCount)
	{
		return &_modelList[id];
	}
	return NULL;
}
frameModel * ResourcesManager::getFrameModel ( int id )
{
	if (id < _modelCount)
	{
		return &_modelList[id].frM;
	}
	return NULL;
}

Texture* ResourcesManager::getTexture(int id)
{
	if (id < _textureCount)
	{
		return &_textureList[id];
	}
	return NULL;
}

Shaders* ResourcesManager::getShader(int id)
{
	if (id < _shaderCount)
	{
		return &_shaderList[id];
	}
	return NULL;
}