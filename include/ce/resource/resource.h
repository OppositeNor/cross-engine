#pragma once
#include "ce/defs.hpp"
#include <vector>
#include "ce/geometry/triangle.h"

class Resource
{
public:

    
    /**
     * @brief Get the current directory of the executable file.
     * @throw std::runtime_error Failed to get directory.
     * @return std::string The current directory
     */
    static std::string GetExeDirectory();

    /**
     * @brief Load a file from path.
     * @note The returned data must be manually freed.
     * @param p_path The path to the file.
     * @throw std::runtime_error Failed to open file.
     * @return byte_t* The data of the file.
     */
    static byte_t* LoadFile(const std::string& p_path);

    /**
     * @brief Load a file from path.
     * @note The returned data must be manually freed.
     * @param p_path The path to the file.
     * @param p_size This will be set to the size of the file.
     */
    static byte_t* LoadFile(const std::string& p_path, size_t& p_size);

    /**
     * @brief Load a file from path to a buffer.
     * 
     * @param p_path The path to load the data from.
     * @param p_buffer The buffer to load the data to.
     * @param p_size The size of the buffer.
     * @return byte_t* The buffer.
     */
    static byte_t* LoadFile(const std::string& p_path, byte_t* p_buffer, size_t p_buff_size);
    
    /**
     * @brief Load a file from path to a buffer.
     * 
     * @param p_path The path to load the data from.
     * @param p_buffer The buffer to load the data to.
     * @param p_size This will be set to the size of the file.
     * @return byte_t* The buffer.
     */
    static byte_t* LoadFile(const std::string& p_path, byte_t* p_buffer, size_t p_buff_size, size_t& p_size);


    /**
     * @brief Load the triangles from a Tris file.
     * 
     * @param p_path The path of the tris file.
     * @param p_result The result triangles. The triangles will be pushed back to this vector.
     */
    static void LoadTris(const std::string& p_path, std::vector<Triangle*>& p_result);

    /**
     * @brief Load the triangles from a tris with normal file.
     * 
     * @param p_path The path of the tris with normal file.
     * @return std::vector<Triangle*> The result triangles. The triangles will be pushed back to this vector.
     */
    static void LoadTrisWithNormal(const std::string& p_path, std::vector<Triangle*>& p_result);

    /**
     * @brief Load the triangles from a model file.
     * 
     * @param p_path The path of the model file.
     * @param p_result The result triangles. The triangles will be pushed back to this vector.
     */
    static void LoadModel(const std::string& p_path, std::vector<Triangle*>& p_result);

    /**
     * @brief Load the triangles from a obj file.
     * 
     * @param p_path The path of the obj file.
     * @param p_result The result triangles. The triangles will be pushed back to this vector.
     */
    static void LoadObjModel(const std::string& p_path, std::vector<Triangle*>& p_result);

    /**
     * @brief Load the triangles from a Tris file.
     * 
     * @param p_path The path of the tris file.
     * @return The result triangles. The triangles will be pushed back to this vector.
     */
    static std::vector<Triangle*> LoadTris(const std::string& p_path);

    /**
     * @brief Load the triangles from a tris with normal file.
     * 
     * @param p_path The path of the tris with normal file.
     * @return The result triangles. The triangles will be pushed back to this vector.
     */
    static std::vector<Triangle*> LoadTrisWithNormal(const std::string& p_path);

    /**
     * @brief Load the triangles from a model file.
     * 
     * @param p_path The path of the model file.
     * @return The result triangles. The triangles will be pushed back to this vector.
     */
    static std::vector<Triangle*> LoadModel(const std::string& p_path);

    /**
     * @brief Load the triangles from a obj file.
     * 
     * @param p_path The path of the obj file.
     * @return The result triangles. The triangles will be pushed back to this vector.
     */
    static std::vector<Triangle*> LoadObjModel(const std::string& p_path);

    /**
     * @brief Create triangle vertices array.
     * 
     * @param p_triangles The triangles to get the vertices array from.
     * @param p_buffer The buffer to put the triangle array.
     * @param p_buffer_size The size of the buffer.
     * 
     * @throw std::out_of_range The buffer size is too small.
     * 
     * @return float* The triangle vertices array buffer.
     */
    static float* CreateModelVertexArray(const std::initializer_list<Triangle*>& p_triangles, float* p_buffer, size_t p_buffer_size);

    
    /**
     * @brief Create triangle vertices array.
     * 
     * @param p_triangles The triangles to get the vertices array from.
     * @param p_buffer The buffer to put the triangle array.
     * @param p_buffer_size The size of the buffer.
     * 
     * @throw std::out_of_range The buffer size is too small.
     * 
     * @return float* The triangle vertices array buffer.
     */
    static float* CreateModelVertexArray(const std::vector<Triangle*>& p_triangles, float* p_buffer, size_t p_buffer_size);

};