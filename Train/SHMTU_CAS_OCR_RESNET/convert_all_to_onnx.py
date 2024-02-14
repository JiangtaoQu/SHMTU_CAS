from src.classify.equal_symbol.convert_equal_symbol_to_onnx import export_equal_symbol_to_onnx
from src.classify.operator.convert_operator_to_onnx import export_operator_to_onnx
from src.classify.digit.convert_digit_to_onnx import export_digit_to_onnx
from src.config import config


def export_all_to_onnx(pth_save_dir_path: str = "./workdir/Models"):
    export_equal_symbol_to_onnx(pth_save_dir_path)
    export_operator_to_onnx(pth_save_dir_path)
    export_digit_to_onnx(pth_save_dir_path)


if __name__ == "__main__":
    export_all_to_onnx(
        config.pth_save_dir_path
    )
