//
// Created by konghaomin on 24-2-15.
//

#ifndef CROSS_PLATFORM_MACRO_H
#define CROSS_PLATFORM_MACRO_H

#ifndef _WINDOWS
// Not Windows

#ifndef __APPLE__
#include <features.h>
#endif

#ifndef _DEBUG
#ifndef NDEBUG
#define _DEBUG
#endif
#endif

#endif

#endif //CROSS_PLATFORM_MACRO_H
