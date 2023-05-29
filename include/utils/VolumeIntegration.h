#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "Common.h"
/*
计算物体惯性张量的程序，用于模拟物体运动时的惯性效应，通过对物体的顶点和面进行积分来计算物体的质量、质心、惯性张量
惯性张量是描述物体抵抗转动的特殊属性
*/
namespace Utils
{
    class VolumeIntegration
    {
    private:
        int A;
        int B;
        int C;
        // 投影积分
        Real P1,Pa,Pb,Paa,Pab,Pbb,Paaa,Paab,Pabb,Pbbb;
        // 面积分
		Real Fa, Fb, Fc, Faa, Fbb, Fcc, Faaa, Fbbb, Fccc, Faab, Fbbc, Fcca;
        // 体积分
        Real T0;
        Real T1[3];
        Real T2[3];
        Real TP[3];

    public:
        VolumeIntegration(const unsigned int nVertices, const unsigned int nFaces, Vector3r * const vertices, const unsigned int* indices);
        void compute_inertia_tensor(Real density);

        // 获取质量
		Real getMass() const { return m_mass; }

        // 获取体积
		Real getVolume() const { return m_volume; }
		// 惯性张量
		Matrix3r const& getInertia() const { return m_theta; }
		// 质心
		Vector3r const& getCenterOfMass() const { return m_r; }
    private:
        // 物体的体积积分
        void volume_integrals();
        // 面积积分
        void face_integrals(unsigned int i);
        // 投影积分
        void projection_integrals(unsigned int i);

        std::vector<Vector3r> m_face_normals;
        std::vector<Real> m_weights;
        unsigned int m_nVertices;
        unsigned int m_nFaces;
        std::vector<Vector3r> m_vertices;
        const unsigned int* m_indices;

        Real m_mass,m_volume;
        Vector3r m_r;
        Vector3r m_x;
        Matrix3r m_theta;
    };
    
}