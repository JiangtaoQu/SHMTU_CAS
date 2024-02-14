#include "CAS_OCR.h"

#include <iostream>

int main()
{
	// CAS_OCR::get_gpu_info().print_info();

	CAS_OCR::set_model_gpu_support(true);

	CAS_OCR::init_model(
		"",
		"fp32"
	);
	
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
			CAS_OCR::predict_validate_code("../../test/" + file_name);

		std::cout << file_name << " : " << std::get<1>(result) << std::endl;
	}

	CAS_OCR::release_model();

	return 0;
}
