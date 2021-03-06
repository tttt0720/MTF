#ifndef MTF_ISOMETRY_H
#define MTF_ISOMETRY_H

#include "ProjectiveBase.h"
#include "SSMEstimator.h"
#include "SSMEstimatorParams.h"

_MTF_BEGIN_NAMESPACE

struct IsometryParams : SSMParams{
	int pt_based_sampling;
	IsometryParams(const SSMParams *ssm_params, int _pt_based_sampling);
	IsometryParams(const IsometryParams *params = nullptr);
};

class IsometryEstimator : public SSMEstimator{
public:
	IsometryEstimator(int modelPoints, bool _use_boost_rng);

	int runKernel(const CvMat* m1, const CvMat* m2, CvMat* model) override;
	bool refine(const CvMat* m1, const CvMat* m2,
		CvMat* model, int maxIters) override;
protected:
	void computeReprojError(const CvMat* m1, const CvMat* m2,
		const CvMat* model, CvMat* error) override;
};

class Isometry : public ProjectiveBase{
public:
	typedef IsometryParams ParamType;

	Isometry( const ParamType *params_in = nullptr);

	void setState(const VectorXd &ssm_state) override;
	void compositionalUpdate(const VectorXd& state_update) override;

	void getInitPixGrad(Matrix2Xd &ssm_grad, int pix_id) override;
	void getCurrPixGrad(Matrix2Xd &ssm_grad, int pix_id) override;

	void cmptInitPixJacobian(MatrixXd &jacobian_prod, const PixGradT &am_jacobian) override;
	void cmptPixJacobian(MatrixXd &jacobian_prod, const PixGradT &am_jacobian) override;
	void cmptApproxPixJacobian(MatrixXd &jacobian_prod,
		const PixGradT &pix_jacobian) override;
	void cmptWarpedPixJacobian(MatrixXd &dI_dp,	const PixGradT &dI_dx) override;
	void cmptInitPixHessian(MatrixXd &pix_hess_ssm, const PixHessT &pix_hess_coord,
		const PixGradT &pix_grad) override;
	void cmptPixHessian(MatrixXd &pix_hess_ssm, const PixHessT &pix_hess_coord,
		const PixGradT &pix_grad) override;
	void cmptWarpedPixHessian(MatrixXd &d2I_dp2, const PixHessT &d2I_dw2,
		const PixGradT &dI_dw) override;

	void estimateWarpFromCorners(VectorXd &state_update, const Matrix24d &in_corners,
		const Matrix24d &out_corners) override;
	void estimateWarpFromPts(VectorXd &state_update, vector<uchar> &mask,
		const vector<cv::Point2f> &in_pts, const vector<cv::Point2f> &out_pts,
		const EstimatorParams &est_params) override;

	//void invertState(VectorXd& inv_state, const VectorXd& state) override {
	//	inv_state = -state;
	//}
	void updateGradPts(double grad_eps) override;
	void updateHessPts(double hess_eps) override;

	void applyWarpToCorners(Matrix24d &warped_corners, const Matrix24d &orig_corners,
		const VectorXd &state_update) override;
	void applyWarpToPts(Matrix2Xd &warped_pts, const Matrix2Xd &orig_pts,
		const VectorXd &state_update) override;
	void generatePerturbation(VectorXd &perturbation) override;

	void getWarpFromState(Matrix3d &warp_mat, const VectorXd& ssm_state) override;
	void getStateFromWarp(VectorXd &state_vec, const Matrix3d& warp_mat) override;

#ifndef DISABLE_SPI
	bool supportsSPI() override{ return true; }
#endif

private:
	ParamType params;
	double getAngleOfRotation(double sin_theta, double cos_theta);

	cv::Mat estimateIsometry(cv::InputArray _points1, cv::InputArray _points2,
		cv::OutputArray _mask, const SSMEstimatorParams &est_params);
	int	estimateIsometry(const CvMat* in_pts, const CvMat* out_pts,
		CvMat* __H, CvMat* mask, const SSMEstimatorParams &est_params);

};



_MTF_END_NAMESPACE

#endif
