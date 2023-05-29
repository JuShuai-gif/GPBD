#pragma once

#include "Common.h"
#include "utils/IndexedFaceMesh.h"
#include "utils/IndexedTetMesh.h"
#include "ParticleData.h"
#include <vector>


namespace PBD
{
    class TetModel
    {
    public:
        TetModel();
        virtual ~TetModel();

        typedef Utils::IndexedFaceMesh SurfaceMesh;
        typedef Utils::IndexedTetMesh ParticleMesh;

        struct Attachment
        {
            unsigned int m_index;
            unsigned int m_triIndex;
            Real m_bary[3];
            Real m_dist;
            Real m_minError;
        };

        Vector3r& getInitialX() { return m_initialX; }
		void setInitialX(const Vector3r &val) { m_initialX = val; }
		Matrix3r& getInitialR() { return m_initialR; }
		void setInitialR(const Matrix3r &val) { m_initialR = val; }
		Vector3r& getInitialScale() { return m_initialScale; }
		void setInitialScale(const Vector3r &val) { m_initialScale = val; }
    
    protected:
		// 偏移量，必须添加该偏移量才能在粒子阵列中获得正确的索引
        unsigned int m_indexOffset;
		// 表示模拟模型的粒子Tet网格
		ParticleMesh m_particleMesh;			
		SurfaceMesh m_surfaceMesh;
		VertexData m_visVertices;
		SurfaceMesh m_visMesh;
		Real m_restitutionCoeff;
		Real m_frictionCoeff;
		std::vector<Attachment> m_attachments;
		Vector3r m_initialX;
		Matrix3r m_initialR;
		Vector3r m_initialScale;
		
		void createSurfaceMesh();
 		void solveQuadraticForZero(const Vector3r& F, const Vector3r& Fu, 
 			const Vector3r& Fv, const Vector3r& Fuu,
 			const Vector3r&Fuv, const Vector3r& Fvv, 
 			Real& u, Real& v);
 		bool pointInTriangle(const Vector3r& p0, const Vector3r& p1, const Vector3r& p2, 
 			const Vector3r& p, Vector3r& inter, Vector3r &bary);
    public:
        SurfaceMesh &getSurfaceMesh();
		VertexData &getVisVertices();
		SurfaceMesh &getVisMesh();
		ParticleMesh& getParticleMesh() { return m_particleMesh; }
		const ParticleMesh& getParticleMesh() const { return m_particleMesh; }
		void cleanupModel();

		unsigned int getIndexOffset() const;

		void initMesh(const unsigned int nPoints, const unsigned int nTets, const unsigned int indexOffset, unsigned int* indices);
		void updateMeshNormals(const ParticleData &pd);
        // 将可视化网格附着到身体表面
        // 提示：调用此函数之前必须更新法线通过 updateMeshNormals(). 
 		void attachVisMesh(const ParticleData &pd);

        // 更新实体的可视化网格
        // 提示：调用此函数之前必须更新法线通过 updateMeshNormals(). 
		void updateVisMesh(const ParticleData &pd);

		FORCE_INLINE Real getRestitutionCoeff() const
		{
			return m_restitutionCoeff;
		}

		FORCE_INLINE void setRestitutionCoeff(Real val)
		{
			m_restitutionCoeff = val;
		}

		FORCE_INLINE Real getFrictionCoeff() const
		{
			return m_frictionCoeff;
		}

		FORCE_INLINE void setFrictionCoeff(Real val)
		{
			m_frictionCoeff = val;
		}
    };
    

    
}