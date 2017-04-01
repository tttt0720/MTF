#ifndef MTF_NN_NT_H
#define MTF_NN_NT_H

#include "SearchMethod.h"
#include "mtf/SM/GNN.h"
#include "mtf/SM/NNParams.h"

_MTF_BEGIN_NAMESPACE
namespace nt{
	class NN : public SearchMethod {
		init_profiling();
		char *log_fname;
		char *time_fname;
	public:

		typedef NNParams ParamType;
		typedef gnn::GNN<AppearanceModel> GNN;
		typedef unique_ptr<GNN> GNN_;

		NN(AM _am, SSM _ssm, const ParamType *nn_params = nullptr);
		~NN(){}

		void initialize(const cv::Mat &corners) override;
		void update() override;	

	private:
		ParamType params;
		GNN_ gnn_index;

		int frame_id;
		int am_dist_size;
		int ssm_state_size;
		//! no. of Gaussian distributions used for generating samples
		int n_distr;
		//! no. of samples generated by each distributions
		VectorXi distr_n_samples;

		bool using_pix_sigma;
		vector<VectorXd> state_sigma, state_mean;

		Matrix3d warp_update;

		Matrix24d prev_corners;
		VectorXd inv_state_update;

		vector<VectorXd> ssm_perturbations;

		MatrixXdr eig_dataset;

		int best_idx;
		double best_dist;

		string saved_db_path, saved_idx_path;
		cv::Mat curr_img_uchar;
		cv::Point2d curr_corners[4];

		bool dataset_loaded;

		void generateDataset();
		void loadDataset();
		void saveDataset();
	};
}
_MTF_END_NAMESPACE

#endif

