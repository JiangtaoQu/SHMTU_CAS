#pragma once

#include <opencv2/core/core.hpp>

namespace SHMTU_CAS_CLR_ClassLibrary
{
	public ref class CAS_OCR_CLR
	{
	public:
		static bool init_model_clr(
			System::String^ checkpoint_dir_path,
			System::String^ type_name
		);

		static void set_model_gpu_support_clr(bool useGpu);

		static System::Tuple<int, System::String^, int, int, int, int>^
		predict_validate_code_clr(System::Drawing::Bitmap^ bitmap, bool useGpu);

		static void release_model_clr();

	private:
		static cv::Mat convert_bitmap_to_cv_mat(System::Drawing::Bitmap^ bitmap);
		static System::String^ convert_std_string_to_system_string(const std::string& std_string);
	};
}
