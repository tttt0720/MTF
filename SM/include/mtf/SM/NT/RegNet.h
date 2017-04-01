#ifndef MTF_REGNET_NT_H
#define MTF_REGNET_NT_H

#include "SearchMethod.h"
#include "mtf/SM/RegNetParams.h"
#include "mtf/Utilities/netUtils.h"

_MTF_BEGIN_NAMESPACE

namespace nt{
	class RegNet : public SearchMethod{
		init_profiling();
		char *log_fname;
		char *time_fname;

	public:
		typedef RegNetParams ParamType;

		using SearchMethod::am;
		using SearchMethod::ssm;
		using SearchMethod::cv_corners_mat;
		using SearchMethod::name;
		using SearchMethod::initialize;
		using SearchMethod::update;		

		RegNet(AM _am, SSM _ssm, const ParamType *rg_params = nullptr);
		~RegNet(){}

		void initialize(const cv::Mat &corners) override;
		void update() override;

	protected:
		ParamType params;

		utils::MTFNet *reg_net;

		int frame_id;
		int am_dist_size;
		int ssm_state_size;

		int n_samplers;
		//! no. of samples generated by each sampler
		VectorXi sampler_n_samples;

		bool using_pix_sigma;
		vector<VectorXd> state_sigma, state_mean;

		vector<VectorXd> ssm_perturbations;

		Matrix24d prev_corners;
		VectorXd inv_state_update;

		MatrixXdr eig_dataset;
		VectorXd eig_query;
		VectorXi eig_result;
		VectorXd eig_dists;

		int best_idx;
		double best_dist;

		string saved_db_path, saved_idx_path;
		cv::Mat curr_img_uchar;
		cv::Point2d curr_corners[4];
	};
}

_MTF_END_NAMESPACE

#endif

