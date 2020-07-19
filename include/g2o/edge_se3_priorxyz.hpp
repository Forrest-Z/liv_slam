#ifndef KKL_G2O_EDGE_SE3_PRIORXYZ_HPP
#define KKL_G2O_EDGE_SE3_PRIORXYZ_HPP

#include <g2o/types/slam3d/types_slam3d.h>
#include <g2o/types/slam3d_addons/types_slam3d_addons.h>

namespace g2o {
 // GPS获取到经纬高后的边   继承单边
class EdgeSE3PriorXYZ : public g2o::BaseUnaryEdge<3, Eigen::Vector3d, g2o::VertexSE3> {
	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
		EdgeSE3PriorXYZ()
		: g2o::BaseUnaryEdge<3, Eigen::Vector3d, g2o::VertexSE3>()
		{}
              // 计算残差  
		void computeError() override {
		  // 获取该边连接的节点
			const g2o::VertexSE3* v1 = static_cast<const g2o::VertexSE3*>(_vertices[0]);
                       // 获取该节点状态的估计值
			Eigen::Vector3d estimate = v1->estimate().translation();
			_error = estimate - _measurement;    // 状态值 - 测量值 = 残差
		}
               // 即GPS的值
		void setMeasurement(const Eigen::Vector3d& m) override {
			_measurement = m;
		}

		virtual bool read(std::istream& is) override {
			Eigen::Vector3d v;
			is >> v(0) >> v(1) >> v(2);
			setMeasurement(v);
			for (int i = 0; i < information().rows(); ++i)
				for (int j = i; j < information().cols(); ++j) {
					is >> information()(i, j);
					if (i != j)
						information()(j, i) = information()(i, j);
				}
			return true;
		}
		virtual bool write(std::ostream& os) const override {
			Eigen::Vector3d v = _measurement;
			os << v(0) << " " << v(1) << " " << v(2) << " ";
			for (int i = 0; i < information().rows(); ++i)
				for (int j = i; j < information().cols(); ++j)
					os << " " << information()(i, j);
			return os.good();
		}
	};
}

#endif