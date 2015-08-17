#ifndef SHADER_H
#define SHADER_H

#include "GL_Includes.h"
#include "Util.h"

#include <memory>

// Useful for looking up shader variables and handles
using HandleMap = std::map < std::string, GLint >;

class Shader;
using ShaderPtr = std::shared_ptr < Shader >;

class Shader {
	// Can you expose enums? Only if python does
	// the same ordering i.e 0,1,2...
	enum Type {
		VERT,
		FRAG
	};
	// Private constructor to limit how these can be created (see static methods below)
	Shader();
public:
	// Initializer
	int CompileAndLink();

	static ShaderPtr FromSource(std::string v, std::string f);
	static ShaderPtr FromFile(std::string v, std::string f);

	~Shader();

	// For the python scripting stuff, it helps to let
	// other people change the source. However this has
	// to blow out any previous shader info, so be careful
	void SetSource(std::string source, Type t); // TODO should these
	void SetSourceFile(std::string fileName, Type t); // compile/link?

	// Bound status
	bool Bind();
	bool Unbind();
	bool IsBound() const;

	// Logging functions
	bool PrintError() const;
	int PrintLog(Type t) const;
	int PrintSrc(Type t) const;

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
