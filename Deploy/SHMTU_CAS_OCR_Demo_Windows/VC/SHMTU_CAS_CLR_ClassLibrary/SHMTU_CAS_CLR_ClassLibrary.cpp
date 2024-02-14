#include "SHMTU_CAS_CLR_ClassLibrary.h"

#include "../NCNN_CLI/CAS_OCR.h"

#include <msclr/marshal_cppstd.h>

namespace SHMTU_CAS_CLR_ClassLibrary
{
	bool CAS_OCR_CLR::init_model_clr(
		System::String^ checkpoint_dir_path,
		System::String^ type_name
	)
	{
		const auto checkpoint_dir_path_std =
			msclr::interop::marshal_as<std::string>(checkpoint_dir_path);
		const auto type_name_std =
			msclr::interop::marshal_as<std::string>(type_name);
		// init model

		const auto result = CAS_OCR::init_model(
			checkpoint_dir_path_std,
			type_name_std
		);

		return result;
	}

	void CAS_OCR_CLR::set_model_gpu_support_clr(bool use_gpu)
	{
		CAS_OCR::set_model_gpu_support(use_gpu);
	}

	System::Tuple<int, System::String^, int, int, int, int>^
	CAS_OCR_CLR::predict_validate_code_clr(System::Drawing::Bitmap^ bitmap, bool use_gpu)
	{
		const cv::Mat image = convert_bitmap_to_cv_mat(bitmap);

		const auto result =
			CAS_OCR::predict_validate_code(image, use_gpu);

		const auto str_cpp = std::get<1>(result);
		const auto str_cs = convert_std_string_to_system_string(str_cpp);

		return gcnew System::Tuple<int, System::String^, int, int, int, int>(
			std::get<0>(result),
			str_cs,
			std::get<2>(result),
			std::get<3>(result),
			std::get<4>(result),
			std::get<5>(result)
		);
	}

	void CAS_OCR_CLR::release_model_clr()
	{
		CAS_OCR::release_model();
	}

	cv::Mat CAS_OCR_CLR::convert_bitmap_to_cv_mat(System::Drawing::Bitmap^ bitmap)
	{
		const cv::Mat image(bitmap->Height, bitmap->Width, CV_8UC4);
		System::Drawing::Imaging::BitmapData^ bmpData =
			bitmap->LockBits(
				System::Drawing::Rectangle(0, 0, bitmap->Width, bitmap->Height),
				System::Drawing::Imaging::ImageLockMode::ReadWrite,
				bitmap->PixelFormat
			);
		auto byteArray =
			gcnew cli::array<System::Byte>(
				bmpData->Stride * bitmap->Height
			);
		System::Runtime::InteropServices::Marshal::Copy(
			bmpData->Scan0,
			byteArray,
			0,
			byteArray->Length
		);
		System::Runtime::InteropServices::Marshal::Copy(
			byteArray,
			0,
			System::IntPtr(image.data),
			byteArray->Length
		);
		bitmap->UnlockBits(bmpData);
		return image;
	}

	// 将std::string转换为System::String^
	System::String^ CAS_OCR_CLR::convert_std_string_to_system_string(const std::string& std_string)
	{
		// 使用msclr::interop::marshal_as进行转换
		return
			msclr::interop::marshal_as<System::String^>(
				std_string
			);
	}
}
