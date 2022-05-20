#pragma once
#include <vcg/complex/complex.h>
#include <vector>
#include <Resource/Data/Interface/Mesh.h>
#include <string>

#include <osg/Geode>
#include <osg/Geometry>
namespace Soarscape
{
	class VCGVertex;
	class VCGEdge;
	class VCGFace;
	struct VCGUsedTypes : public vcg::UsedTypes<vcg::Use<VCGVertex>::AsVertexType,
		vcg::Use<VCGEdge>::AsEdgeType,
		vcg::Use<VCGFace>::AsFaceType>
	{
	};

	class VCGVertex : public vcg::Vertex<VCGUsedTypes, vcg::vertex::Coord3f, vcg::vertex::TexCoord2f, vcg::vertex::BitFlags, vcg::vertex::Normal3f, vcg::vertex::Mark, vcg::vertex::Color4b>
	{
	};
	class VCGFace : public vcg::Face<VCGUsedTypes, vcg::face::VertexRef, vcg::face::FFAdj, vcg::face::Mark, vcg::face::BitFlags, vcg::face::Normal3f>
	{
	};
	class VCGEdge : public vcg::Edge<VCGUsedTypes>
	{
	};

	typedef vcg::tri::TriMesh<std::vector<VCGVertex>, std::vector<VCGEdge>, std::vector<VCGFace>> BaseTriMesh;

	class VCGMesh
	{
	public:
		BaseTriMesh m_Mesh;
	public:
		VCGMesh() = default;
		VCGMesh(const std::string& filename);

		virtual ~VCGMesh() = default;

		osg::Geode* getOSGGeode() { return m_Geode; }

		void update();
		void readFile(const std::string& filename);

		void reserverFace();
		void smoothGeometry();

		void deleteFace(uint32_t index);

		void updateFace();
		void updateVertices();
	private:
		bool m_NeedUpdateBuffer = true;
		osg::DrawElementsUInt* m_DrawElements = nullptr;
		osg::Vec3Array* m_Vertices = nullptr;
		osg::Geode* m_Geode = nullptr;
		osg::Geometry* m_Geometry = nullptr;
		uint32_t m_NumFaces = 0;
		uint32_t m_NumVerts = 0;

		void init();
	};
}