#include "Shader.h"

#include <fstream>

using namespace std;

// Basics
Shader::Shader()
	: m_bIsBound(false),
	m_Program(0),
	m_hVertShader(0),
	m_hFragShader(0) {
}

Shader::~Shader() {
	Unbind();
}

// These static "factory" methods construct a smart pointer to a shader
// object, because I'd prefer to have one of these active at a time
// (although does it really matter, as long as one Shader runs CompileAndLink()?)
/*static*/ ShaderPtr Shader::FromSource(string v, string f) {
	if (v.empty() || f.empty())
		return nullptr;

	ShaderPtr ret(new Shader);

	ret->m_VertShaderSrc = v;
	ret->m_FragShaderSrc = f;
	int err = ret->CompileAndLink();

	// Exit on this, just because
	if (err)
		exit(err);

	return ret;
}

/*static*/ ShaderPtr Shader::FromFile(string v, string f) {
	if (v.empty() || f.empty())
		return nullptr;

	ifstream vIn(v), fIn(f);
	string vSrc((istreambuf_iterator<char>(vIn)), istreambuf_iterator<char>());
	string fSrc((istreambuf_iterator<char>(fIn)), istreambuf_iterator<char>());

	return FromSource(vSrc, fSrc);
}

// Managing bound state
bool Shader::Bind() {
	if (!m_bIsBound) {
		glUseProgram(m_Program);
		m_bIsBound = true;
	}
	return m_bIsBound == true;
}

bool Shader::Unbind() {
	if (m_bIsBound) {
		glUseProgram(0);
		m_bIsBound = false;
	}
	return m_bIsBound == false;
}

bool Shader::IsBound() const {
	return m_bIsBound;
}

int Shader::CompileAndLink() {
	// Check if the shader op went ok
	auto check = [](GLuint id, GLuint type) {
		GLint status(GL_FALSE);
		if (type == GL_COMPILE_STATUS)
			glGetShaderiv(id, type, &status);
		if (type == GL_LINK_STATUS)
			glGetProgramiv(id, type, &status);
		return status == GL_TRUE;
	};

	// Error codes to return
	const int ERR_N(0), ERR_V(1), ERR_F(2), ERR_L(4);

	// Weird thing that helps with glShaderSource
	const GLchar * shaderSrc[] = { m_VertShaderSrc.c_str(), m_FragShaderSrc.c_str() };

	// Compile Vertex Shader
	m_hVertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_hVertShader, 1, &(shaderSrc[0]), 0);
	glCompileShader(m_hVertShader);
	if (!check(m_hVertShader, GL_COMPILE_STATUS)) {
		cout << "Unable to compile vertex shader." << endl;
		PrintLog_V();
		return ERR_V;
	}

	// Compile Frag Shader
	m_hFragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_hFragShader, 1, &(shaderSrc[1]), 0);
	glCompileShader(m_hFragShader);
	if (!check(m_hFragShader, GL_COMPILE_STATUS)) {
		cout << "Unable to compile fragment shader." << endl;
		PrintLog_F();
		return ERR_F;
	}

	// Create and Link Program
	m_Program = glCreateProgram();
	glAttachShader(m_Program, m_hVertShader);
	glAttachShader(m_Program, m_hFragShader);
	glLinkProgram(m_Program);
	if (!check(m_Program, GL_LINK_STATUS)) {
		cout << "Unable to link shader program." << endl;
		return ERR_L;
	}

	return ERR_N;
}

// Accessor for shader handles
GLint Shader::getHandle(const string idx) {
	// If we have the handle, return it
	if (m_Handles.find(idx) != m_Handles.end())
		return m_Handles.find(idx)->second;

	// Otherwise we have to find it, so client must bind
	if (!m_bIsBound) {
		cout << "Error: shader queried for untested variable " << idx
			<< " while shader was not bound" << endl;
		return -1;
	}

	// Try and get handle, first as attr then as uniform
	GLint handle = glGetAttribLocation(m_Program, idx.c_str());
	if (handle < 0)
		handle = glGetUniformLocation(m_Program, idx.c_str());

	// valid handles begin at 0
	if (handle >= 0)
		m_Handles[idx] = handle;
	else {
		// We queried something bad, print the log
		cout << "Invalid variable " << idx << " queried in shader" << endl;
		PrintLog_V();
		PrintLog_F();
	}

	return handle;
}

// Print Logs
int Shader::PrintLog_V() const {
	const int max(1024);
	int len(0);
	char log[max];
	glGetShaderInfoLog(m_hVertShader, max, &len, log);
	cout << "Vertex Shader Log: \n\n" << log << "\n\n" << endl;

	return len;
}

int Shader::PrintLog_F() const {
	const int max(1024);
	int len(0);
	char log[max];
	glGetShaderInfoLog(m_hFragShader, max, &len, log);
	cout << "Fragment Shader Log: \n\n" << log << "\n\n" << endl;

	return len;
}

// Print Source
int Shader::PrintSrc_V() const {
	cout << "Vertex Shader Source: \n\n" << m_VertShaderSrc << "\n\n" << endl;
	return m_VertShaderSrc.length();
}

int Shader::PrintSrc_F() const {
	cout << "Fragment Shader Source: \n\n" << m_FragShaderSrc << "\n\n" << endl;
	return m_FragShaderSrc.length();
}