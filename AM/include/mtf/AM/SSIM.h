#ifndef MTF_SSIM_H
#define MTF_SSIM_H

#include "AppearanceModel.h"

_MTF_BEGIN_NAMESPACE

struct SSIMParams : AMParams{
	double k1, k2;

	SSIMParams(const AMParams *am_params,
		double _k1, double _k2);
	SSIMParams(const SSIMParams *params = nullptr);
};

struct SSIMDist : AMDist{
	typedef double ElementType;
	typedef double ResultType;
	SSIMDist(const string &_name, const double _c1, const double _c2) :
		AMDist(_name), c1(_c1), c2(_c2){}
	double operator()(const double* a, const double* b,
		size_t size, double worst_dist = -1) const override;
private:
	const double c1, c2;
};


// Structural Similarity
class SSIM : public AppearanceModel{
public:
	typedef SSIMParams ParamType;
	typedef SSIMDist DistType;

	SSIM(const ParamType *ssim_params = nullptr, const int _n_channels = 1);

	double getLikelihood() const override;

	//-------------------------------initialize functions------------------------------------//
	void initializeSimilarity() override;
	void initializeGrad() override;
	void initializeHess() override{ hess.resize(patch_size); }

	//-----------------------------------------------------------------------------------//
	//-------------------------------update functions------------------------------------//
	//-----------------------------------------------------------------------------------//
	void updateSimilarity(bool prereq_only = true) override;
	void updateInitGrad() override;
	// nothing is done here since curr_grad is same as and shares memory with  curr_pix_diff
	void updateCurrGrad() override;

	void cmptInitHessian(MatrixXd &init_hessian, const MatrixXd &init_pix_jacobian) override;
	void cmptCurrHessian(MatrixXd &curr_hessian, const MatrixXd &curr_pix_jacobian) override;

	void cmptInitHessian(MatrixXd &init_hessian, const MatrixXd &init_pix_jacobian,
		const MatrixXd &init_pix_hessian) override;
	void cmptCurrHessian(MatrixXd &curr_hessian, const MatrixXd &curr_pix_jacobian,
		const MatrixXd &curr_pix_hessian) override;

	void cmptSelfHessian(MatrixXd &self_hessian, const MatrixXd &curr_pix_jacobian) override;
	void cmptSelfHessian(MatrixXd &self_hessian, const MatrixXd &curr_pix_jacobian,
		const MatrixXd &curr_pix_hessian) override{
		cmptSelfHessian(self_hessian, curr_pix_jacobian);
	}

	/*Support for FLANN library*/
	const DistType* getDistFunc() override{
		return new DistType(name, c1, c2);
	}
	void updateDistFeat(double* feat_addr) override;
	void initializeDistFeat() override;
	void updateDistFeat() override;
	const double* getDistFeat() override{ return curr_feat_vec.data(); }
	unsigned int getDistFeatSize() override;

protected:
	ParamType params;

	//! mean, variance and standard deviation of the initial pixel values
	double init_pix_mean, init_pix_var, init_pix_mean2;
	//! mean, variance and standard deviation of the current pixel values
	double curr_pix_mean, curr_pix_var, curr_pix_mean2;

	double c1, c2;
	double a, b, c, d;
	double cross_pix_var;
	double cd;

	VectorXd hess;
	VectorXd init_pix_vals_cntr, curr_pix_vals_cntr;

	RowVectorXd init_grad_vec, curr_grad_vec;
	double init_grad_scal, curr_grad_scal;

	VectorXd curr_feat_vec;
};

_MTF_END_NAMESPACE

#endif