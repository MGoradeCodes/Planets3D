#include "headers/ShaderClass.h"
#include <glad/gl.h>
#include <filesystem> 

using namespace std;

string getFileContents(const char* filename){
    ifstream in(filename, ios::binary);
    if(in){
        string contents;
        in.seekg(0, ios::end);
        contents.resize(in.tellg());
        in.seekg(0, ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return contents;
    }
    
    // Find where the program is executing from
    std::filesystem::path current_dir = std::filesystem::current_path();
    std::filesystem::path full_path = current_dir / filename;
    
    // Explicitly print to the console so it cannot be cut off by the runtime_error
    std::cout << "\n=== FILE ERROR INITIALIZED ===" << std::endl;
    std::cout << "Working Directory: " << current_dir.string() << std::endl;
    std::cout << "Looking for file: " << filename << std::endl;
    std::cout << "Full Absolute Path: " << full_path.string() << std::endl;
    std::cout << "==============================\n" << std::endl;
    
    throw std::runtime_error("Failed to read file.");
}

ShaderProgram::ShaderProgram(const char* vertexFile, const char* fragFile){
    
    string vertexCode = getFileContents(vertexFile);
    string fragmentCode = getFileContents(fragFile);

    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}


void ShaderProgram::Delete(){
    glDeleteProgram(ID);
}

void ShaderProgram::Activate(){
    glUseProgram(ID);
}

void ShaderProgram::SetVec3(const char* name, const glm::vec3& val){
    int location = glGetUniformLocation(ID, name);
    glUniform3f(location, val.x , val.y , val.z);
}
void ShaderProgram::SetMat4(const char* name, glm::mat4& value)
{
    glUniformMatrix4fv(
        glGetUniformLocation(ID, name),
        1,
        GL_FALSE,
        glm::value_ptr(value)
    );
}