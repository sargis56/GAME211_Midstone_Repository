#ifndef SHADER_H
#define SHADER_H
#include "glew.h"
#include <unordered_map>

class Shader {
	Shader(const Shader&) = delete;
	Shader(Shader&&) = delete;
	Shader& operator = (const Shader&) = delete;
	Shader& operator = (Shader&&) = delete;

public:
	Shader(const char *vsFilename, const char *fsFilename);
	~Shader();

	inline GLuint getProgram() const { return shaderID;}
	GLuint getUniformID(std::string name);

private:
	unsigned int shaderID;
	unsigned int vertShaderID;
	unsigned int fragShaderID;
	std::unordered_map<std::string, GLuint > uniformMap;

	char* readTextFile(const char *fileName);	
	void readCompileAttach(const char *vsFilename, const char *fsFilename);
	void link();
	void setUniformLocations();

};
#endif