#pragma once
#include <vector>
#include <array>
#include "Common.h"

namespace Utils{
    class IndexedFaceMesh
    {
    public:
        // 边  一个边有两个面  两个顶点
        struct Edge
        {
            std::array<unsigned int,2> m_face;
            std::array<unsigned int,2> m_vert;
        };
        
    public:
        typedef std::vector<unsigned int> Faces;
        typedef std::vector<Vector3r> FaceNormals;
        typedef std::vector<Vector3r> VertexNormals;
        typedef std::vector<std::vector<unsigned int>> FacesEdges;
        typedef std::vector<Edge> Edges;
        typedef std::vector<std::vector<unsigned int>> VerticesEdges;
        typedef std::vector<std::vector<unsigned int>> VerticesFaces;
        typedef std::vector<unsigned int> UVIndices;
        typedef std::vector<Vector2r> UVs;
    
    protected:
        unsigned int m_numPoints;//点数量
        Faces m_indices;// 面索引
        Edges m_edges;// 边
        FacesEdges m_facesEdges;// 面 边
        bool m_closed;// 关闭
        UVIndices m_uvIndices;// uv索引
        UVs m_uvs;// uv值
        VerticesFaces m_verticesFaces;// 面顶点
        VerticesEdges m_verticesEdges;// 边顶点
        const unsigned int m_verticesPerFace = 3u;//一般都是3个
        FaceNormals m_normals;// 法线
        VertexNormals m_vertexNormals;// 顶点法线
        bool m_flatShading;//着色方式
    
    public:
        IndexedFaceMesh();
        IndexedFaceMesh(IndexedFaceMesh& other);
        IndexedFaceMesh& operator=(IndexedFaceMesh const& other);

        void release();
        bool isClosed()const;
        bool getFlatShading()const{
            return m_flatShading;
        }
        void setFlatShading(const bool v){
            m_flatShading = v;
        }
        void initMesh(const unsigned int nPoints,const unsigned int nEdges,const unsigned int nFaces);
        void addFace(const unsigned int* const indices);
        void addFace(const int* const indices);
        void addUV(const Real u,const Real v);
        void addUNIndex(const unsigned int index);

        const Faces& getFaces()const{
            return m_indices;
        }
        Faces& getFaces(){return m_indices;}
        const FaceNormals& getFaceNormals()const{
            return m_normals;
        }
        FaceNormals& getFaceNormals(){
            return m_normals;
        }

        const VertexNormals& getVertexNormals()const{
            return m_vertexNormals;
        }
        VertexNormals& getVertexNormals(){
            return m_vertexNormals;
        }

        Edges& getEdges(){return m_edges;}
        const Edges& getEdges()const{return m_edges;}

        const FacesEdges& getFacesEdges()const {
            return m_facesEdges;
        }

        const UVIndices& getUVIndices()const{
            return m_uvIndices;
        }
        const UVs& getUVs()const{return m_uvs;}

        const VerticesFaces& getVertexFaces()const{
            return m_verticesFaces;
        }
        const VerticesEdges& getVertexEdges()const{
            return m_verticesEdges;
        }

        unsigned int numVertices()const{
            return m_numPoints;
        }
        unsigned int numFaces()const{
            return (unsigned int)m_indices.size() / m_verticesPerFace;
        }
        unsigned int numEdges()const {
            return (unsigned int)m_edges.size();
        }
        unsigned int numUVs()const {return (unsigned int)m_uvs.size();}

        void copyUVs(const UVIndices& uvIndices,const UVs& uvs);
        
        void buildNeighbors();

        template<class PositionData>
		void updateNormals(const PositionData &pd, const unsigned int offset);

        template<class PositionData>
        void updateVertexNormals(const PositionData& pd);

        unsigned int getVerticesPerFace()const;
    };

    template<class PositionData>
    void IndexedFaceMesh::updateNormals(const PositionData& pd,const unsigned int offset)
    {
        m_normals.resize(numFaces());
        {
            for (int i = 0; i < (int)numFaces(); ++i)
            {
				const Vector3r &a = pd.getPosition(m_indices[m_verticesPerFace*i] + offset);
				const Vector3r &b = pd.getPosition(m_indices[m_verticesPerFace*i + 1] + offset);
				const Vector3r &c = pd.getPosition(m_indices[m_verticesPerFace*i + 2] + offset);

                Vector3r v1 = b - a;
                Vector3r v2 = c - a;

                m_normals[i] = v1.cross(v2);
                m_normals[i].normalize();
                if (m_normals[i].squaredNorm() < 1e-6)
                    m_normals[i] = Vector3r::UnitX();
            }
            
        }

    }
    
    template<class PositionData>
    void IndexedFaceMesh::updateVertexNormals(const PositionData& pd)
    {
        m_vertexNormals.resize(numVertices());

        for (unsigned int i = 0;i < numVertices(); ++i)
        {
            m_vertexNormals[i].setZero();
        }

        for (unsigned int i = 0u; i < numFaces(); ++i)
        {
            const Vector3r& n = m_normals[i];
            m_vertexNormals[m_indices[m_verticesPerFace*i]] += n;
			m_vertexNormals[m_indices[m_verticesPerFace*i + 1]] += n;
			m_vertexNormals[m_indices[m_verticesPerFace*i + 2]] += n;
        }
        for (unsigned int i = 0; i < numVertices(); i++)
		{
			m_vertexNormals[i].normalize();
		}
        
    }
    

}