#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(std::string name): ShaderProgram(name, NOTHING)
{
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(programID);
}

ShaderProgram::ShaderProgram(std::string name, int sophistication)
{

	std::string vertexFile(path + name + ".vs");
	std::string fragmentFile(path + name + ".fs");
	programID = glCreateProgram();
	createShader(vertexID, GL_VERTEX_SHADER, vertexFile);
	createShader(fragmentID, GL_FRAGMENT_SHADER, fragmentFile);

	unsigned int geometryID=-1, tessControlID=-1, tessEvalID=-1;

	if (sophistication == GEO)
	{
		std::string geometryFile(path + name + ".gs");
		createShader(geometryID, GL_GEOMETRY_SHADER, geometryFile);
		glAttachShader(programID, geometryID);
	}
	else if (sophistication == TESS)
	{
		std::string tessControlFile(path + name + ".tesc");
		std::string tessEvalFile(path + name + ".tese");
		createShader(tessControlID, GL_TESS_CONTROL_SHADER, tessControlFile);
		createShader(tessEvalID, GL_TESS_EVALUATION_SHADER, tessEvalFile);
		glAttachShader(programID, tessControlID);
		glAttachShader(programID, tessEvalID);
	}
	else if (sophistication == GEOTESS)
	{
		std::string tessComputeFile(path + name + ".tesc");
		std::string tessEvalFile(path + name + ".tese");
		std::string geometryFile(path + name + ".gs");
		createShader(geometryID, GL_GEOMETRY_SHADER, geometryFile);
		createShader(tessControlID, GL_TESS_CONTROL_SHADER, tessComputeFile);
		createShader(tessEvalID, GL_TESS_EVALUATION_SHADER, tessEvalFile);
		glAttachShader(programID, tessControlID);
		glAttachShader(programID, tessEvalID);
		glAttachShader(programID, geometryID);
	}
		 

	glAttachShader(programID, vertexID);
	glAttachShader(programID, fragmentID);

	int success;
	char infoLog[256];
	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(programID, 512, NULL, infoLog);
		std::cerr << "Failed to link " << name << " \n " << infoLog << std::endl;
	}

	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
	
	if (sophistication == GEO)
	{
		glDeleteShader(geometryID);
	}
	else if (sophistication == TESS)
	{
		glDeleteShader(tessControlID);
		glDeleteShader(tessEvalID);
	}
	else if (sophistication == GEOTESS)
	{
		glDeleteShader(tessControlID);
		glDeleteShader(tessEvalID);
		glDeleteShader(geometryID);
	}


}


void ShaderProgram::createShader(unsigned int& shaderID, int shaderType, std::string filename)
{
	std::ifstream file(filename);
	if(file.bad())
	{
		std::cerr << filename << " not found " << std::endl;
		return;
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string shaderCode = buffer.str();
	buffer.clear();
	file.close();

	shaderID = glCreateShader(shaderType);
	const char* cshaderCode = shaderCode.c_str();
	glShaderSource(shaderID, 1, &cshaderCode, NULL);
	glCompileShader(shaderID);

	int success;
	char infoLog[256];
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderID, 256, NULL, infoLog);
		std::cerr << "Failed to compile " << filename << " \n " << infoLog << std::endl;
	}

}

void ShaderProgram::use()
{
	glUseProgram(programID);
}

void ShaderProgram::unuse()
{
	glUseProgram(0);
}

void ShaderProgram::mapUniform(std::string uniformName)
{
	const char* cUniform = uniformName.c_str();
	int uniformLoc = glGetUniformLocation(programID, cUniform);
	uniformMap.insert(std::pair <std::string,int> (uniformName, uniformLoc));
}

void ShaderProgram::mapCameraUniform(std::string camName)
{
    std::string spinName = camName + ".spin";
    std::string posName = camName + ".pos";
    const char* cSpinName = spinName.c_str();
    const char* cPosName = posName.c_str();
    int spinLoc = glGetUniformLocation(programID, cSpinName);
    int posLoc = glGetUniformLocation(programID, cPosName);
    uniformMap.insert({spinName, spinLoc});
    uniformMap.insert({posName, posLoc});
}

void ShaderProgram::mapDirectionalLightUniform(std::string lightName)
{
	std::string colName = lightName + ".col";
	std::string dirName = lightName + ".dir";
	const char* cColName = colName.c_str();
	const char* cDirName = dirName.c_str();
	int colLoc = glGetUniformLocation(programID, cColName);
	int dirLoc = glGetUniformLocation(programID, cDirName);
	uniformMap.insert({colName, colLoc});
	uniformMap.insert({dirName, dirLoc});
}


void ShaderProgram::setUniform(std::string uniformName, float x, float y)
{
	glUniform2f(uniformMap[uniformName], x, y);
}

void ShaderProgram::setUniform(std::string uniformName, int value)
{
	glUniform1i(uniformMap[uniformName], value);
}

void ShaderProgram::setUniform(std::string name, Vec2& v)
{
	glUniform2f(uniformMap[name], v[0], v[1]);
}

void ShaderProgram::setUniform(std::string name, Vec2&& v)
{
  glUniform2f(uniformMap[name], v[0], v[1]);
}

void ShaderProgram::setUniform(std::string name, Vec3& v)
{
	glUniform3f(uniformMap[name], v[0], v[1], v[2]);
}

void ShaderProgram::setUniform(std::string name, Vec3&& v)
{
	glUniform3f(uniformMap[name], v[0], v[1], v[2]);
}

void ShaderProgram::setUniform(std::string name, Quat& q)
{
    glUniform4f(uniformMap[name], q[1], q[2], q[3], q[0]);
}


void ShaderProgram::setUniform(std::string name, Quat&& q)
{
    glUniform4f(uniformMap[name], q[1], q[2], q[3], q[0]);
}

void ShaderProgram::setUniform(std::string name, Mat4& m)
{	
	glUniformMatrix4fv(uniformMap[name], 1, true, m[0]);
}

void ShaderProgram::setUniform(std::string name, Camera& cam)
{
	std::string spin = name + ".spin";
	std::string pos = name + ".pos";

	glUniform4f(uniformMap[spin],  cam.spin[1], cam.spin[2], cam.spin[3], cam.spin[0]);
	glUniform3f(uniformMap[pos], cam.position[0], cam.position[1], cam.position[2]);	
}


void ShaderProgram::setUniform(std::string name, DirectionalLight& light)
{
	std::string dirName = name + ".dir";
	std::string colName = name + ".col";
	Vec3 dir = light.getDirection();
	Vec3 col = light.getColor();

	glUniform3f(uniformMap[dirName],  dir[0], dir[1], dir[2]);
	glUniform3f(uniformMap[colName], col[0], col[1], col[2]);	
}



void ShaderProgram::setCameraUniform(std::string name, Quat& sunSpin, Vec3& sunPos)
{
	std::string spin = name + ".spin";
	std::string pos = name + ".pos";

	glUniform4f(uniformMap[spin],  sunSpin[1], sunSpin[2], sunSpin[3], sunSpin[0]);
	glUniform3f(uniformMap[pos], sunPos[0], sunPos[1], sunPos[2]);	
}


void ShaderProgram::setUniform(std::string name, Vec4& v)
{
	glUniform4f(uniformMap[name], v[0], v[1], v[2], v[3]);
}

void ShaderProgram::setUniform(std::string name, Vec4&& v)
{
	glUniform4f(uniformMap[name], v[0], v[1], v[2], v[3]);
}

void ShaderProgram::mapJointDataArrayUniform(std::string name)
{
  for(unsigned int i=0; i<MeshAnimated::max_joints; i++)
  {
    std::string baseName = name + "[" + std::to_string(i) + "]";
    std::string scaName = baseName + ".scale";
    std::string rotName = baseName + ".rotation";
    std::string posName = baseName + ".translation";

    int rotLoc = glGetUniformLocation(programID, rotName.c_str());
    int posLoc = glGetUniformLocation(programID, posName.c_str());
    int scaLoc = glGetUniformLocation(programID, scaName.c_str());

    uniformMap.insert({rotName, rotLoc});
    uniformMap.insert({posName, posLoc});
    uniformMap.insert({scaName, scaLoc});
  }
}

void ShaderProgram::setJointDataArrayUniform(std::string name, std::vector<JointData>& currentFrame)
{
  unsigned int i=0;
  for(; i<currentFrame.size(); i++)
  {
    std::string baseName = name + "[" + std::to_string(i) + "]";
    std::string scaName = baseName + ".scale";
    std::string rotName = baseName + ".rotation";
    std::string posName = baseName + ".translation";
    
    glUniform4f(uniformMap[rotName],  currentFrame[i].rotation[1], currentFrame[i].rotation[2], currentFrame[i].rotation[3], currentFrame[i].rotation[0]);
    glUniform3f(uniformMap[posName], currentFrame[i].translation[0], currentFrame[i].translation[1], currentFrame[i].translation[2]);	
    glUniform3f(uniformMap[scaName], currentFrame[i].scale[0], currentFrame[i].scale[1], currentFrame[i].scale[2]);	
  }
  
  // might comment this loop idk
  for(; i<MeshAnimated::max_joints; i++)
  {
    std::string baseName = name + "[" + std::to_string(i) + "]";
    std::string scaName = baseName + ".scale";
    std::string rotName = baseName + ".rotation";
    std::string posName = baseName + ".translation";
    
    glUniform4f(uniformMap[rotName],  0,0,0,1);
    glUniform3f(uniformMap[posName], 0,0,0);	
    glUniform3f(uniformMap[scaName], 1,1,1);	
  }
}



void ShaderProgram::mapJointMatricesArrayUniform(std::string name)
{
  for(unsigned int i=0; i<MeshAnimated::max_joints; i++)
  {
    std::string baseName = name + "[" + std::to_string(i) + "]";

    int uniformLoc = glGetUniformLocation(programID, baseName.c_str());
    uniformMap.insert({baseName, uniformLoc});
  }
}

void ShaderProgram::setJointMatricesArrayUniform(std::string name, std::vector<Mat4>& currentFrame)
{
  unsigned int i=0;
  for(; i<currentFrame.size(); i++)
  {
    std::string baseName = name + "[" + std::to_string(i) + "]";
	  glUniformMatrix4fv(uniformMap[baseName], 1, true, currentFrame[i][0]);
  }
  
  // might comment this loop idk
  for(; i<MeshAnimated::max_joints; i++)
  {
    std::string baseName = name + "[" + std::to_string(i) + "]";
    Mat4 identity(1,0,0,0,
                  0,1,0,0,
                  0,0,1,0,
                  0,0,0,1);
    glUniformMatrix4fv(uniformMap[baseName], 1, true, identity[0]);
  }
}
