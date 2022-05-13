#pragma once
#include <string>

#include "Resource/Data/Interface/Mesh.h"
namespace Soarscape
{
    template<typename T>
    class MeshComponent
    {
    public:
        MeshComponent(const std::string& filename)
            : m_Mesh(new T(filename))
        {}

        ~MeshComponent() {  }
        
        T* m_Mesh;
    };
} // namespace kanjing
