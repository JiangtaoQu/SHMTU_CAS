//
// Created by konghaomin on 24-2-13.
//

#include "STD_CPP_Utils.h"

#include <algorithm>

namespace std_extend {
    void string_trim(std::string &str) {
        // 去除首部空格
        str.erase(
            str.begin(),
            std::ranges::find_if(
                str,
                [](unsigned char ch) {
                    return !std::isspace(ch);
                }
            )
        );

        // 去除尾部空格
        str.erase(
            std::find_if(
                str.rbegin(), str.rend(),
                [](unsigned char ch) {
                    return !std::isspace(ch);
                }
            ).base(),
            str.end()
        );
    }
}
