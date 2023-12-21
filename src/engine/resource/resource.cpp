#include "ce/resource/resource.h"
#include "ce/geometry/polygon.h"
#include <memory>
#include <fstream>
#include <windows.h>
#include <functional>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

static FORCE_INLINE bool IsAlpha(byte_t p_char)
{
    return (p_char >= 'a' && p_char <= 'z') || (p_char >= 'A' && p_char <= 'Z');
}

static FORCE_INLINE bool IsDigit(byte_t p_char)
{
    return (p_char >= '0' && p_char <= '9') || p_char == '.' || p_char == '-';
}

static byte_t* GetWord(byte_t* p_start, byte_t* buff, size_t buff_size)
{
    for (size_t i = 0; i < buff_size; ++i)
    {
        if (!(IsAlpha(p_start[i]) || IsDigit(p_start[i]) || p_start[i] == '_'))
        {
            buff[i] = '\0';
            return buff;
        }
        buff[i] = p_start[i];
    }
    buff[buff_size - 1] = '\0';
    return buff;
}

static byte_t* GetLine(byte_t* p_start, byte_t* buff, size_t buff_size)
{
    for (size_t i = 0; i < buff_size; ++i)
    {
        if (p_start[i] == '\n' || p_start[i] == '\0')
        {
            buff[i] = '\0';
            return buff;
        }
        buff[i] = p_start[i];
    }
    buff[buff_size - 1] = '\0';
    return buff;
}

static byte_t* MovePToNextSpace(byte_t** p, byte_t* p_end)
{
    while (**p != '\n' && **p != ' ' && **p != '\t' && **p != '\0')
    { 
        if (*p >= p_end)
            throw std::out_of_range("Out of range.");
        ++(*p);
    }
    return *p;
}

static byte_t* MovePToNextWord(byte_t** p, byte_t* p_end)
{
    while (IsAlpha(**p) || IsDigit(**p) || **p == '_')
    {
        if (*p >= p_end)
            throw std::out_of_range("Out of range.");
        ++(*p);
    }
    return *p;
}

static byte_t* MovePToNextLine(byte_t** p, byte_t* p_end)
{
    while (**p != '\n' && **p != '\0')
    { 
        if (*p >= p_end)
            throw std::out_of_range("Out of range.");
        ++(*p);
    }
    return *p;
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
        throw std::runtime_error("Failed to open file: \"" + p_path + "\".");
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
        throw std::runtime_error("Failed to open file: \"" + p_path + "\".");
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
        throw std::runtime_error("Failed to open file: \"" + p_path + "\".");
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
        throw std::runtime_error("Failed to open file: \"" + p_path + "\".");
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
    auto data = std::unique_ptr<byte_t[]>(LoadFile(p_path.c_str(), file_size));
    byte_t* p = data.get();
    byte_t buff[256];
    GetWord(p, buff, 256);
    size_t tri_count = (size_t)std::atoi(buff);
    MovePToNextSpace(&p, data.get() + file_size);
    ++p;
    
    Math::Vec4f temp = Math::Pos();
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

std::vector<Triangle*> Resource::LoadTrisWithNormal(const std::string& p_path)
{
    std::vector<Triangle*> result;
    LoadTrisWithNormal(p_path, result);
    return std::move(result);
}

std::vector<Triangle*> Resource::LoadModel(const std::string& p_path)
{
    std::vector<Triangle*> result;
    LoadModel(p_path, result);
    return std::move(result);
}

std::vector<Triangle*> Resource::LoadObjModel(const std::string& p_path)
{
    std::vector<Triangle*> result;
    LoadObjModel(p_path, result);
    return std::move(result);
}

void Resource::GetImageSize(const std::string& p_path, size_t& p_width, size_t& p_height, size_t& p_channels)
{
    int width, height, channels;
    stbi_info(p_path.c_str(), &width, &height, &channels);
    p_width = width;
    p_height = height;
    p_channels = channels;
}

ubyte_t* Resource::LoadTextureImage(const std::string& p_path, ubyte_t* p_buffer, size_t p_buffer_size,
        size_t& p_width, size_t& p_height, size_t& p_channels)
{
    auto result = std::unique_ptr<ubyte_t[], std::function<decltype(stbi_image_free)>>(
            stbi_load(p_path.c_str(), (int*)&p_width, (int*)&p_height, (int*)&p_channels, 0), stbi_image_free);
    if (result.get() == nullptr)
        throw std::runtime_error("Failed to load image at path: " + p_path + ".");
    
    if (p_buffer == nullptr)
        p_buffer = new ubyte_t[p_width * p_height * p_channels];
    else if (p_buffer_size < p_width * p_height * p_channels)
        throw std::out_of_range("The buffer size is too small.");
    
    memcpy(p_buffer, result.get(), p_width * p_height * p_channels);
    return p_buffer;
}

void Resource::LoadTrisWithNormal(const std::string& p_path, std::vector<Triangle*>& p_result)
{
    size_t file_size;
    auto data = std::unique_ptr<byte_t[]>(LoadFile(p_path.c_str(), file_size));
    byte_t* p = data.get();
    byte_t buff[256];
    GetWord(p, buff, 256);
    size_t tri_count = (size_t)std::atoi(buff);
    MovePToNextSpace(&p, data.get() + file_size);
    ++p;
    
    Math::Vec4f temp_pos = Math::Pos();
    Math::Vec4f temp_normal =Math::Vec4f();
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

void Resource::LoadModel(const std::string& p_path, std::vector<Triangle*>& p_result)
{
    std::string ext = p_path.substr(p_path.find_last_of('.') + 1);
    if (ext == "tris")
        LoadTris(p_path, p_result);
    else if (ext == "norm")
        LoadTrisWithNormal(p_path, p_result);
    else if (ext == "obj")
        LoadObjModel(p_path, p_result);
}

void Resource::LoadObjModel(const std::string& p_path, std::vector<Triangle*>& p_result)
{
    size_t file_size;
    auto data = std::unique_ptr<byte_t[]>(LoadFile(p_path.c_str(), file_size));
    byte_t* p = data.get();

    std::vector<Math::Vec4> positions;
    std::vector<Math::Vec4> normals;
    std::vector<Math::Vec2> tex_coords;
    byte_t line_buff[1024];
    byte_t word_buff[256];
    size_t counter = 0;
    while (p < data.get() + file_size) {
        GetLine(p, line_buff, 1024);
        auto *pp = line_buff;
        if (pp[0] == 'v' && pp[1] == ' ')
        {
            pp += 2;
            Math::Vec4 pos = Math::Pos();
            for (size_t i = 0; i < 3; ++i)
            {
                GetWord(pp, word_buff, 256);
                pos[i] = std::atof(word_buff);
                MovePToNextSpace(&pp, line_buff + 1024);
                ++pp;
            }
            positions.push_back(pos);
        }
        else if (pp[0] == 'v' && pp[1] == 't')
        {
            pp += 3;
            Math::Vec2 tex_coord;
            for (size_t i = 0; i < 2; ++i)
            {
                GetWord(pp, word_buff, 256);
                tex_coord[i] = std::atof(word_buff);
                MovePToNextSpace(&pp, line_buff + 1024);
                ++pp;
            }
            tex_coords.push_back(tex_coord);
        }
        else if (pp[0] == 'v' && pp[1] == 'n')
        {
            pp += 3;
            Math::Vec4 norm =Math::Vec4();
            for (size_t i = 0; i < 3; ++i)
            {
                GetWord(pp, word_buff, 256);
                norm[i] = std::atof(word_buff);
                MovePToNextSpace(&pp, line_buff + 1024);
                ++pp;
            }
            normals.push_back(norm);
        }
        else if (pp[0] == 'f' && pp[1] == ' ')
        {
            ++pp;
            PolygonN poly;
            while (*pp != '\0')
            {
                ++pp;
                Vertex* vert = new Vertex;
                if (std::atoi(GetWord(pp, word_buff, 256)) - 1 >= positions.size())
                    throw std::runtime_error("Invalid obj file.");
                vert->Position() = positions[std::atoi(GetWord(pp, word_buff, 256)) - 1];
                MovePToNextWord(&pp, line_buff + 1024);
                ++pp;
                auto temp = atoi(GetWord(pp, word_buff, 256)) - 1;
                vert->UV() = tex_coords[std::atoi(GetWord(pp, word_buff, 256)) - 1];
                MovePToNextWord(&pp, line_buff + 1024);
                ++pp;
                if (std::atoi(GetWord(pp, word_buff, 256)) - 1 >= normals.size())
                    throw std::runtime_error("Invalid obj file.");
                vert->Normal() = normals[std::atoi(GetWord(pp, word_buff, 256)) - 1];
                MovePToNextWord(&pp, line_buff + 1024);
                poly.AddVertex(poly.GetVertexCount(), vert);
            }
            std::vector<Triangle*> temp_triangles;
            poly.Triangulate(temp_triangles);
            for (size_t i = 0; i < temp_triangles.size(); ++i)
            {
                p_result.push_back(temp_triangles[i]);
            }
        }
        MovePToNextLine(&p, data.get() + file_size);
        ++p;
    }
}