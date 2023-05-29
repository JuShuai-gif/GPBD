#include "BoundingSphereHierarchy.h"

#include <iostream>
#include <unordered_set>
#include <set>

using namespace Eigen;
using pool_set = std::set<unsigned int>;
using namespace PBD;

PointCloudBSH::PointCloudBSH()
    :super(0,10)
{}


Vector3r const& PointCloudBSH::entity_position(unsigned int i)const
{
    return m_vertices[i];
}

// 计算最小凸包
void PointCloudBSH::compute_hull(unsigned int b,unsigned int n,BoundingSphere& hull)const
{
    auto vertices_subset = std::vector<Vector3r>(n);
    for (unsigned int i = b; i < b + n; ++i)
        vertices_subset[i-b]=m_vertices[m_lst[i]];
    const BoundingSphere s(vertices_subset);

    hull.x() = s.x();
    hull.r() = s.r();
}

void PointCloudBSH::compute_hull_approx(unsigned int b,unsigned int n,BoundingSphere& hull)const
{
    Vector3r x;
    x.setZero();
    for(unsigned int i = b;i < b+n;++i)
        x += m_vertices[m_lst[i]];
    x /= (Real)n;

    Real radius2 = 0.0f;
    for (unsigned int i = b;i < b + n; ++i)
    {
        radius2 = std::max(radius2, (x - m_vertices[m_lst[i]]).squaredNorm());
    }
    
    hull.x() = x;
    hull.r() = sqrt(radius2);
}

void PointCloudBSH::init(const Vector3r* vertices,const unsigned int numVertices)
{
    m_lst.resize(numVertices);
    m_vertices = vertices;
    m_numVertices = numVertices;
}


TetMeshBSH::TetMeshBSH():super(0)
{}

Vector3r const& TetMeshBSH::entity_position(unsigned int i)const
{
    return m_com[i];
}

void TetMeshBSH::compute_hull(unsigned int b,unsigned int n,BoundingSphere& hull)const
{
    compute_hull_approx(b,n,hull);
}

void TetMeshBSH::compute_hull_approx(unsigned int b,unsigned int n,BoundingSphere& hull)const
{
    Vector3r x;
    x.setZero();
    for (unsigned int i = b; i < b + n; ++i)
    {
        const unsigned int tet = m_lst[i];
        x += m_vertices[m_indices[4 * tet]];
        x += m_vertices[m_indices[4 * tet + 1]];
        x += m_vertices[m_indices[4 * tet + 2]];
        x += m_vertices[m_indices[4 * tet + 3]];
    }
    x /= ((Real)4.0f * (Real)n);

    Real radius2 = 0.0f;
    for (unsigned int i = b; i < b + n; ++i)
    {
        const unsigned int tet = m_lst[i];
        radius2 = std::max(radius2, (x - m_vertices[m_indices[4 * tet]]).squaredNorm());
		radius2 = std::max(radius2, (x - m_vertices[m_indices[4 * tet+1]]).squaredNorm());
		radius2 = std::max(radius2, (x - m_vertices[m_indices[4 * tet+2]]).squaredNorm());
		radius2 = std::max(radius2, (x - m_vertices[m_indices[4 * tet+3]]).squaredNorm());
    }
    
    hull.x() = x;
    hull.r() = sqrt(radius2) + m_tolerance;
}






