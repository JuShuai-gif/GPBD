#include "Common.h"
#include <vector>

namespace PBD
{
    /*
    使用Welzl算法计算点集的最小封闭球体是一种常用的计算几何问题。
    通过这种算法，可以找到包含给定点集中所有点的最小圆盘或者球体。
    在计算机图形学中，Welzl算法可以用来寻找3D模型的最小外接球体.
    */

   class BoundingSphere
   {
    public:
        BoundingSphere():m_x(Vector3r::Zero()),m_r(0.0f){}
        BoundingSphere(Vector3r x,Real r):m_x(x),m_r(r){}
        BoundingSphere(const Vector3r& a){
            m_x = a;
            m_r = 0.0f;
        }

        // 两点确定一个圆
        BoundingSphere(const Vector3r& a,const Vector3r& b){
            const Vector3r ba = b - a;
            m_x = (a + b) * static_cast<Real>(0.5f);
            m_r = static_cast<Real>(0.5f) * ba.norm();
        }
        // 三点确定一个外接球
        BoundingSphere(const Vector3r& a,const Vector3r& b,const Vector3r& c)
        {
            const Vector3r ba = b - a;
            const Vector3r ca = c - a;
            const Vector3r baxca = ba.cross(ca);
            Vector3r r;
            Matrix3r T;
            T << ba[0],ba[1],ba[2],
                ca[0],ca[1],ca[2],
                baxca[0],baxca[1],baxca[2];

            r[0] = static_cast<Real>(0.5f) * ba.squaredNorm();
            r[1] = static_cast<Real>(0.5f) * ba.squaredNorm();
            r[2] = static_cast<Real>(0.0f);

            m_x = T.inverse() * r;
            m_r = m_x.norm();
            m_x += a;
        }

        BoundingSphere(const Vector3r& a,const Vector3r& b,const Vector3r& c,const Vector3r& d)
        {
            const Vector3r ba = b - a;
            const Vector3r ca = c - a;
            const Vector3r da = d - a;

            Vector3r r;
            Matrix3r T;
            T << ba[0],ba[1],ba[2],
                ca[0],ca[1],ca[2],
                da[0],da[1],da[2];
            r[0] = static_cast<Real>(0.5) * ba.squaredNorm();
			r[1] = static_cast<Real>(0.5) * ca.squaredNorm();
			r[2] = static_cast<Real>(0.5) * da.squaredNorm();
			m_x = T.inverse() * r;
			m_r = m_x.norm();
			m_x += a;
        }

        BoundingSphere(const std::vector<Vector3r>& p)
        {
            m_r = 0;
            m_x.setZero();
            setPoints(p);
        }

        Vector3r const& x() const { return m_x; }
        Vector3r& x() { return m_x; }
        Real r() const { return m_r; }
        Real& r() { return m_r; } 

        void setPoints(const std::vector<Vector3r>& p)
        {
            // 删除重复项
            std::vector<Vector3r> v(p);
            std::sort(v.begin(),v.end(),[](const Vector3r& a,const Vector3r& b){
                if(a[0] < b[0]) return true;
                if(a[0] > b[0]) return false;
                if(a[1] < b[1]) return true;
                if(a[1] > b[1]) return false;
                return (a[2] < b[2]);
            });
            // std::unique()函数，用于从容器v中移除相邻的重复元素
            // Lambda表达式，使用Eigen库的isApprox()方法比较两个向量是否近似相等
            v.erase(std::unique(v.begin(),v.end(),[](Vector3r& a,Vector3r& b){
                return a.isApprox(b);
            }),v.end());

            Vector3r d;
            const int n = int(v.size());

            // 生成点的随机排列并通过epsilon对点进行排列以避免拐角情况
            const double epsilon = 1.0e-6;
            for (int i = n - 1; i > 0; i--)
            {
                const Vector3r epsilon_vec = epsilon * Vector3r::Random();
                const int j = static_cast<int>(floor(i * double(rand()) / RAND_MAX));
                d = v[i] + epsilon_vec;
                v[i] = v[j] - epsilon_vec;
                v[j] = d;
            }

            BoundingSphere S = BoundingSphere(v[0],v[1]);

            for (int i = 2; i < n; ++i)
            {
                d = v[i] - S.x();
                if (d.squaredNorm() > S.r() * S.r())
                    S = ses1(i,v,v[i]);
            }

            m_x = S.m_x;
            // 加上epsilon以确保所有未改变的点都在球体内
            m_r = S.m_r + static_cast<Real>(epsilon);
        }

        bool overlaps(BoundingSphere const& other) const
        {
            double rr = m_r + other.m_r;
            return (m_x - other.m_x).squaredNorm() < rr * rr;
        }

        bool contains(BoundingSphere const& other)const
        {
            double rr = r() - other.r();
            return (x() - other.x()).squaredNorm() < rr * rr;
        }

		bool contains(Vector3r const& other) const
		{
			return (x() - other).squaredNorm() < m_r * m_r;
		}

    private:
		BoundingSphere ses3(int n, std::vector<Vector3r>& p, Vector3r& q1, Vector3r& q2, Vector3r& q3)
		{
            // 构造一个球体
			BoundingSphere S(q1, q2, q3);
            
            // 遍历所有点是否在球面上
			for (int i = 0; i < n; i++)
			{
				Vector3r d = p[i] - S.x();
                // 点不在球内，继续外扩球，让它包括该点
				if (d.squaredNorm() > S.r()* S.r())
					S = BoundingSphere(q1, q2, q3, p[i]);
			}
			return S;
		}

		BoundingSphere ses2(int n, std::vector<Vector3r>& p, Vector3r& q1, Vector3r& q2)
		{
			BoundingSphere S(q1, q2);

			for (int i = 0; i < n; i++)
			{
				Vector3r d = p[i] - S.x();
				if (d.squaredNorm() > S.r()* S.r())
					S = ses3(i, p, q1, q2, p[i]);
			}
			return S;
		}

		BoundingSphere ses1(int n, std::vector<Vector3r>& p, Vector3r& q1)
		{
			BoundingSphere S(p[0], q1);

			for (int i = 1; i < n; i++)
			{
				Vector3r d = p[i] - S.x();
				if (d.squaredNorm() > S.r()* S.r())
					S = ses2(i, p, q1, p[i]);
			}
			return S;
		}

        Vector3r m_x;
        Real m_r;
   };
   
}