#pragma once
#include <string>
#include <vector>
#include <map>
namespace Soarscape
{
	struct Vertex
	{
        float px;
        float py;
        float pz;
        float nx;
        float ny;
        float nz;
        float u;
        float v;
	};

    class Mesh
    {
    public:
        Mesh() = default;
        Mesh(const std::string& path);
        virtual void update();
        virtual void updateBuffer() {}
        virtual ~Mesh();
    public:
        std::vector<Vertex> m_V;
        std::vector<uint32_t> m_I;
        std::string m_Path;

        bool operator==(const Mesh& other) const { return other.m_Path == m_Path; }
    };
}