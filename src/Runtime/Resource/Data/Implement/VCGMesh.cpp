#include "VCGMesh.h"
#include "Core/Base/macro.h"
#include <wrap/io_trimesh/import_obj.h>
#include <wrap/io_trimesh/import_ply.h>

#include <osgUtil/SmoothingVisitor>

namespace Soarscape
{
	VCGMesh::VCGMesh(const std::string& filename)
	{
		init();
		readFile(filename);
		update();
		smoothGeometry();
	}
	void VCGMesh::update()
	{
		updateVertices();
		updateFace();
	}
	void VCGMesh::readFile(const std::string& filename)
	{
		auto pointPos = filename.find_last_of(".");
		auto lPos = filename.find_last_of("/");
		std::string meshName = filename.substr(lPos + 1, pointPos - 1 - lPos);
		std::string extendName = filename.substr(pointPos + 1);
		LOG_INFO("mesh Name = {0}", meshName);
		LOG_INFO("mesh extenName = {0}", extendName);
		if (extendName == "obj")
		{
			vcg::tri::io::ImporterOBJ<BaseTriMesh>::Info info;
			if ((vcg::tri::io::ImporterOBJ<BaseTriMesh>::Open(m_Mesh, filename.c_str(), info) == vcg::tri::io::ImporterOBJ<BaseTriMesh>::E_NOERROR))
			{
				LOG_INFO("Input mesh: {0}", meshName);
				LOG_INFO("Mesh file type: {0}", extendName);
			}
			else
			{
				LOG_ERROR("Can't read FIle : {0}", filename);
			}
		}
		else if (extendName == "ply")
		{
			if (vcg::tri::io::ImporterPLY<BaseTriMesh>::Open(m_Mesh, filename.c_str()) == 0)
			{
				LOG_INFO("Input mesh: {0}", meshName);
				LOG_INFO("Mesh file type: {0}", extendName);
			}
			else
			{
				LOG_INFO("Can't read FIle : {0}", filename);
			}
		}

		else
		{
			LOG_ERROR("Editor can't suport format :{0}", extendName);
		}
		m_NumFaces =  m_Mesh.face.size();
		m_NumVerts = m_Mesh.vert.size();
		m_NeedUpdateBuffer = true;
	}
	void VCGMesh::reserverFace()
	{
		std::vector<uint32_t> rindeices;
		rindeices.reserve(m_DrawElements->size());
		std::copy(m_DrawElements->rbegin(), m_DrawElements->rend(), std::back_inserter(rindeices));
		std::memcpy((void*)(m_DrawElements->getDataPointer()), (void*)(rindeices.data()), rindeices.size() * 4);
	}
	void VCGMesh::smoothGeometry()
	{
		osgUtil::SmoothingVisitor::smooth(*m_Geometry);
	}
	void VCGMesh::deleteFace(uint32_t index)
	{
		ASSERT(index < m_Mesh.face.size(), "face index greater than face max size!");
		m_Mesh.face[index].SetD();
	}
	void VCGMesh::updateFace()
	{
		m_DrawElements->clear();
		m_DrawElements->reserveElements(m_NumFaces * 3);
		m_DrawElements->reserve(m_NumFaces * 3);
		for (auto f : m_Mesh.face)
		{
			if (!f.IsD())
			{
				for (size_t i = 0; i < 3; i++)
				{
					m_DrawElements->push_back(vcg::tri::Index(m_Mesh, f.V(i)));
				}
			}
		}
		m_Geometry->dirtyDisplayList();
	}
	void VCGMesh::updateVertices()
	{
		m_Vertices->clear();
		m_Vertices->reserve(m_NumVerts);
		for (auto v : m_Mesh.vert)
		{
			if (!v.IsD())
			{
				float x = v.P().X(), y = v.P().Y(), z = v.P().Z();
				m_Vertices->push_back({ x, y ,z });
			}
		}
		m_Geometry->dirtyDisplayList();
	}
	void VCGMesh::init()
	{
		if (m_Geode != nullptr ||
			m_Geometry != nullptr ||
			m_Vertices != nullptr)
		{
			return;
		}
		m_Geode = new osg::Geode;
		m_Geometry = new osg::Geometry;
		m_Vertices = new osg::Vec3Array;
		m_Geometry->setVertexArray(m_Vertices);
		m_Geode->addChild(m_Geometry);
		m_DrawElements = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES);
		m_Geometry->addPrimitiveSet(m_DrawElements);
	}
}