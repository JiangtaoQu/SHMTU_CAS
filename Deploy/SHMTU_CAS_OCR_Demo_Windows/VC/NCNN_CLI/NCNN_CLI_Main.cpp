#include "CAS_OCR.h"

#include <iostream>
#include <filesystem>

const auto model_type = "fp16";

int main()
{
	// CAS_OCR::get_gpu_info().print_info();

	CAS_OCR::set_model_gpu_support(false);

	const auto checkpoint_dir_path = "../../Checkpoint";

	if (
		std::filesystem::exists(checkpoint_dir_path) &&
		std::filesystem::is_directory(checkpoint_dir_path)
	)
	{
		CAS_OCR::init_model(
			"../../Checkpoint",
			model_type
		);
	}
	else
	{
		CAS_OCR::init_model(
			"./",
			model_type
		);
	}

	std::string test_dir_path = "../../Test";

	if (
		!(
			std::filesystem::exists(test_dir_path) &&
			std::filesystem::is_directory(test_dir_path)
		)
	)
	{
		test_dir_path = "./Test";
	}

	if (
		!(
			std::filesystem::exists(test_dir_path) &&
			std::filesystem::is_directory(test_dir_path)
		)
	)
	{
		test_dir_path = ".";
	}

	const std::vector<std::string> file_name_list = {
		"test1_20240102160004_server.png",
		"test2_20240102160811_server.png",
		"test3_20240102160857_server.png",
		"test4_20240102160902_server.png",
		"test5_20240102160141_server.png",
		"test6_20240102160146_server.png",
	};

	for (const auto& file_name : file_name_list)
	{
		std::cout << "-----------------" << "\n";
		const auto result =
			CAS_OCR::predict_validate_code(test_dir_path + "/" + file_name);

		std::cout << file_name << " : " << std::get<1>(result) << '\n';
	}

	CAS_OCR::release_model();

	return 0;
}
