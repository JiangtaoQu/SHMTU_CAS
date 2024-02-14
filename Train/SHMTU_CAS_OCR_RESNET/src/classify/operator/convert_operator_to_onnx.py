import os

from src.classify.model.model_type import get_pth_name
from src.classify.step.export_to_onnx import export_to_onnx
from src.config import config


def export_operator_to_onnx(save_dir_path: str = "../../../workdir/Models"):
    pth_path = os.path.join(
        save_dir_path,
        get_pth_name(
            config.model_operator_type,
            "operator",
            str("latest")
        )
    )

    export_to_onnx(
        config.model_operator_type,
        output_features_count=6,
        pth_path=pth_path
    )


if __name__ == "__main__":
    export_operator_to_onnx(
        config.pth_save_dir_path
    )
