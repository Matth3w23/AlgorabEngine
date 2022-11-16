#include "model.h"

Model::Model(std::string path) {
    addModel(path);
}

Model::Model(std::vector<Mesh>& mshs) : //not supported/correct
    meshes(mshs) {
    currentlyDefault = false;
}

void Model::addModel(std::string path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "Error when importing model: " << importer.GetErrorString() << std::endl;
        return;
    }

    if (currentlyDefault) {
        meshes = {};
        currentlyDefault = false;
    }

    currentModelDirectory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
    
}

void Model::resetMeshes() {
    furthestVertexDist = 0.0f;
    meshes = std::vector<Mesh>{};
    addModel(defaultModelPath);
    currentlyDefault = true;
    
}

void Model::processNode(aiNode* node, const aiScene* scene) {

    //process nodes meshes
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    //process child nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }

}

//
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<TexturedVertex> vertices; //all imported meshes will be textures, coloured is for my own use
    //TODO: split importing meshes into other class?
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        TexturedVertex vertex;
        
        glm::vec3 vector; //for converting between assimp data types and glm

        //position
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;
        
        if (glm::length(vector) >= furthestVertexDist) {
            furthestVertexDist = glm::length(vector);
        }

        //normals
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;

        //texture coords
        if (mesh->mTextureCoords[0]) {
            glm::vec2 vector2;
            vector2.x = mesh->mTextureCoords[0][i].x;
            vector2.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vector2;
        } else {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    //indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse"); //will only have diffuse to begin with
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);

        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++) {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }

        if (!skip) {
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), currentModelDirectory);
            texture.type = typeName;
            texture.path = str.C_Str(); //may be incorrect
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }

    return textures;
}

//helper function from learnOpenGL
unsigned int Model::TextureFromFile(const char* path, const std::string& directory, bool gamma) { //TODO: Credit learnopengl where necessary
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format = GL_RED;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

std::vector<Mesh>& Model::getMeshes() {
    return meshes;
}

float Model::getfurVertDist() {
    return furthestVertexDist;
}
