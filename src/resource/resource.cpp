#include "ce/resource/resource.h"
#include "ce/geometry/triangle.h"
#include <fstream>
#include <windows.h>

static byte_t* GetWord(byte_t* p_start, byte_t* buff, size_t buff_size)
{
    for (size_t i = 0; i < buff_size; ++i)
    {
        if (p_start[i] == '\n' || p_start[i] == ' ' || p_start[i] == '\t' || p_start[i] == '\0')
        {
            buff[i] = '\0';
            return buff;
        }
        buff[i] = p_start[i];
    }
    buff[buff_size - 1] = '\0';
    return buff;
}

static void MovePToNextSpace(byte_t** p, byte_t* p_end)
{
    while (**p != '\n' && **p != ' ' && **p != '\t' && **p != '\0')
    { 
        if (*p >= p_end)
            throw std::out_of_range("Out of range.");
        ++(*p);
    }
}

std::string Resource::GetExeDirectory()
{
    char buff[256];
    int bytes = GetModuleFileName(NULL, buff, 256);
    if (bytes == -1)
        throw std::runtime_error("Failed to get directory.");
    for (int i = bytes - 1; i >= 0; --i)
    {
        if (buff[i] == '/' || buff[i] == '\\')
        {
            buff[i] = '\0';
            break;
        }
    }
    return std::string(buff);
}

byte_t* Resource::LoadFile(const std::string& p_path)
{
    std::ifstream file(p_path.c_str(), std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("Failed to open file");
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    byte_t* data = new byte_t[size];
    file.read(reinterpret_cast<char*>(data), size);
    file.close();
    return data;
}

byte_t* Resource::LoadFile(const std::string& p_path, size_t& p_size)
{
    std::ifstream file(p_path.c_str(), std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("Failed to open file");
    file.seekg(0, std::ios::end);
    p_size = file.tellg();
    file.seekg(0, std::ios::beg);
    byte_t* data = new byte_t[p_size + 10]; // 10 bytes for safety
    file.read(reinterpret_cast<char*>(data), p_size);
    file.close();
    return data;
}

byte_t* Resource::LoadFile(const std::string& p_path, byte_t* p_buffer, size_t p_buff_size)
{
    std::ifstream file(p_path.c_str(), std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("Failed to open file");
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    for (size_t i = 0; i < size; i++)
    {
        if (i >= p_buff_size)
            break;
        file.read(reinterpret_cast<char*>(p_buffer + i), 1);
    }
    return p_buffer;
}

byte_t* Resource::LoadFile(const std::string& p_path, byte_t* p_buffer, size_t p_buff_size, size_t& p_size)
{
    std::ifstream file(p_path.c_str(), std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("Failed to open file");
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    for (size_t i = 0; i < size; i++)
    {
        if (i >= p_buff_size)
            break;
        file.read(reinterpret_cast<char*>(p_buffer + i), 1);
    }
    p_size = min(p_buff_size, size);
    return p_buffer;
}

void Resource::LoadTris(const std::string& p_path, std::vector<Triangle*>& p_result)
{
    size_t file_size;
    std::unique_ptr<byte_t[]> data = std::unique_ptr<byte_t[]>(LoadFile(p_path.c_str(), file_size));
    byte_t* p = data.get();
    byte_t buff[256];
    GetWord(p, buff, 256);
    size_t tri_count = (size_t)std::atoi(buff);
    MovePToNextSpace(&p, data.get() + file_size);
    ++p;
    
    Vec4f temp = Pos();
    p_result.reserve(tri_count);
    for (size_t i = 0; i < tri_count; ++i)
    {
        Triangle* current_tri = new Triangle;
        for (size_t j = 0; j < 3; ++j)
        {
            for (size_t k = 0; k < 3; ++k)
            {
                GetWord(p, buff, 256);
                temp[k] = std::atof(buff);
                MovePToNextSpace(&p, data.get() + file_size);
                ++p;
            }
            current_tri->GetVertex(j)->SetPosition(temp);
        }
        for (size_t j = 0; j < 3; ++j)
            current_tri->GetVertex(j)->ResetNormal();
        p_result.push_back(current_tri);
        ++p;
    }
}

std::vector<Triangle*> Resource::LoadTris(const std::string& p_path)
{
    std::vector<Triangle*> result;
    LoadTris(p_path, result);
    return std::move(result);
}

void Resource::LoadTrisWithNormal(const std::string& p_path, std::vector<Triangle*>& p_result)
{
    size_t file_size;
    std::unique_ptr<byte_t[]> data = std::unique_ptr<byte_t[]>(LoadFile(p_path.c_str(), file_size));
    byte_t* p = data.get();
    byte_t buff[256];
    GetWord(p, buff, 256);
    size_t tri_count = (size_t)std::atoi(buff);
    MovePToNextSpace(&p, data.get() + file_size);
    ++p;
    
    Vec4f temp_pos = Pos();
    Vec4f temp_normal = Vec4f();
    p_result.reserve(tri_count);
    for (size_t i = 0; i < tri_count; ++i)
    {
        Triangle* current_tri = new Triangle;
        for (size_t j = 0; j < 3; ++j)
        {
            for (size_t k = 0; k < 3; ++k)
            {
                GetWord(p, buff, 256);
                temp_pos[k] = std::atof(buff);
                MovePToNextSpace(&p, data.get() + file_size);
                ++p;
            }
            for (size_t k = 0; k < 3; ++k)
            {
                GetWord(p, buff, 256);
                temp_normal[k] = std::atof(buff);
                MovePToNextSpace(&p, data.get() + file_size);
                ++p;
            }
            current_tri->GetVertex(j)->Position() = temp_pos;
            current_tri->GetVertex(j)->Normal() = temp_normal;
        }
        p_result.push_back(current_tri);
        ++p;
    }
}

float* Resource::CreateModelVertexArray(const std::initializer_list<Triangle*>& p_triangles, float* p_buffer, size_t p_buffer_size)
{
    if (p_triangles.size() * Triangle::TRIANGLE_ARRAY_SIZE > p_buffer_size)
        throw std::out_of_range("The buffer size is too small.");
    for (size_t i = 0; i < p_triangles.size(); ++i)
        (*(p_triangles.begin() + i))->GetVertexArray(p_buffer + i * Triangle::TRIANGLE_ARRAY_SIZE, Triangle::TRIANGLE_ARRAY_SIZE);
    return p_buffer;
}

float* Resource::CreateModelVertexArray(const std::vector<Triangle*>& p_triangles, float* p_buffer, size_t p_buffer_size)
{
    if (p_triangles.size() * Triangle::TRIANGLE_ARRAY_SIZE > p_buffer_size)
        throw std::out_of_range("The buffer size is too small.");
    for (size_t i = 0; i < p_triangles.size(); ++i)
        (*(p_triangles.begin() + i))->GetVertexArray(p_buffer + i * Triangle::TRIANGLE_ARRAY_SIZE, Triangle::TRIANGLE_ARRAY_SIZE);
    return p_buffer;
}