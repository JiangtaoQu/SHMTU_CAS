//
// Created by konghaomin on 24-2-15.
//

#ifndef CROSS_PLATFORM_MACRO_H
#define CROSS_PLATFORM_MACRO_H

#include <features.h>

#ifndef _WINDOWS

// Not Windows
#ifndef _DEBUG
#ifndef NDEBUG
#define _DEBUG
#endif
#endif

#endif

#endif //CROSS_PLATFORM_MACRO_H
