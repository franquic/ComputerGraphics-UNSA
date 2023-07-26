#include <iostream>
#include <fstream>
#include <sstream>
#include "loadObj.hpp"

// Función para cargar un objeto en formato OBJ y extraer sus vértices
// agregar color
bool loadObject(const std::string &filename, std::vector<glm::vec3> &vertices)
{
    std::ifstream objFile(filename);
    if (!objFile.is_open())
    {
        std::cout << "Error al abrir el archivo: " << filename << std::endl;
        return false;
    }

    std::vector<glm::vec3> objVertices;
    std::string line;
    while (std::getline(objFile, line))
    {
        std::istringstream iss(line);
        std::string firstSymbol;
        iss >> firstSymbol;

        if (firstSymbol == "v")
        {
            glm::vec3 position;
            iss >> position.x >> position.y >> position.z;
            objVertices.push_back(position);
        }
        else if (firstSymbol == "f")
        {
            std::string vertexIndices;
            for (int i = 0; i < 3; i++)
            {
                iss >> vertexIndices;
                std::istringstream viss(vertexIndices);
                unsigned int index;
                viss >> index;
                vertices.push_back(objVertices[index - 1]);
            }
        }
    }

    objFile.close();
    return true;
}