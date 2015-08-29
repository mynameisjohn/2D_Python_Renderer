#ifndef SHADER_H
#define SHADER_H


#include <memory>


#include "GL_Includes.h"
#include "Util.h"

// Useful for looking up shader variables and handles
using HandleMap = std::map < std::string, GLint >;

class Shader {
public:
	Shader();
	~Shader();

	// Initializer
	int CompileAndLink();

	// Python class registration
	static void PyRegister();

	// For the python scripting stuff, it helps to let
	// other people change the source. However this has
	// to blow out any previous shader info, so be careful
	void SetSource_V(std::string source); 
	void SetSourceFile_V(std::string fileName); 
	void SetSource_F(std::string source); 
	void SetSourceFile_F(std::string fileName); 

	// Bound status
	bool Bind();
	bool Unbind();
	bool IsBound() const;

	// Logging functions
	int PrintLog_V() const;
	int PrintSrc_V() const;
	int PrintLog_F() const;
	int PrintSrc_F() const;

	// Public Accessors
	GLint getHandle(const std::string idx);

	// Why not?
	inline HandleMap getHandleMap() { return m_Handles; }
private:
	// Bound status, program/shaders, source, handles
	bool m_bIsBound;
	GLuint m_Program;
	GLuint m_hVertShader;
	GLuint m_hFragShader;
	std::string m_VertShaderSrc;
	std::string m_FragShaderSrc;
	HandleMap m_Handles;

	// Public scoped bind class
	// binds shader for as long as it lives
public:
	class ScopedBind {
		friend class Shader;
	protected:
		Shader& m_Shader;
		ScopedBind(Shader& s) : m_Shader(s) { m_Shader.Bind(); }
	public:
		~ScopedBind() { m_Shader.Unbind(); }
	};
	inline ScopedBind ScopeBind() { return ScopedBind(*this); }
};

#endif
